# -*- coding: utf-8 -*-
 
import os
import shutil
import cPickle as pickle
from glob import glob
from binascii import crc32
from datetime import datetime
 

class Bitcask(object):
    def __init__(self,THRESHOLD=10000,path="./"):
        self.TABLE = {}
        self.THRESHOLD = THRESHOLD
        self.path = path
        self.curr_active = max([int(f.replace('.sst', '').replace(self.path,'')) for f in glob(path+'*.sst')] or [1])
        
        if os.path.exists(path) == False:
            shutil.mkdir(path)

    def get(self,key):
        '''根据Key获取对应的值'''
        record = self.__get_record(key)
        if record != None:
            return record[5]
        return None

    def put(self,key, value):
        '''将key,value写入数据文件'''
        key_size = len(key)
    
        value_size = 0
        if value is not None:
            value_size = len(value)

        ts = datetime.now()
        crc = crc32('{0}{1}{2}{3}{4}'.format(
            ts, key_size, value_size, key, value))
        info = self.__io_write(crc, ts, key_size, value_size, key, value)
        self.TABLE[key] = info
        return True

    def delete(self,key):
        '''删除Key对应的记录'''
        return  self.put(key,None)
    def update(self,key):
        '''更新Key对应的记录'''
        return self.put(key,value)

    def load_data(self,path=None):
        '''指定路径,加载该文件夹目录下所有数据文件'''
        if path is not None:
            self.path = path
            if os.path.exists(path) == False:
                shutil.mkdir(path)

        for filename in glob(self.path+"*.sst"):
            self.load_file(filename)

    def load_file(self,filename):
        '''加载文件名为filename的数据文件'''
        f = open(filename)
        data = f.read()
        list_items = data.split("t.")
    
        start = 0
        length = 0
        for item in list_items[:-1]:
            s= item+"t."
            record = pickle.loads(s)
            length = len(item)+2
            if self.TABLE.has_key(record[4]) is True:
                ts1 = record[1]
                ts2 = self.__get_record(record[4])[1]
                if ts1 > ts2:
                    self.TABLE[record[4]]=(filename,start,length)
            else:
                self.TABLE[record[4]]=(filename,start,length)
            start += length
        f.close()

    def __get_record(self,key):
        '''根据key取出数据文件中对应的整条记录'''
        info = self.TABLE.get(key)
        print info,type(info)
        if not info:
            return None
        pickled_data = self.__io_read(*info)
        data = pickle.loads(pickled_data)
        # check tuple, crc, the same key?
        return data

    
    def __io_read(self,filename, start, length):
        '''读取未经反序列化的文件数据'''
        with open(filename, 'rb') as f:
            f.seek(start)
            return f.read(length)
 
    def __io_write(self,crc, ts, key_size, value_size, key, value):
        '''将记录序列化后写入文件'''
        active_filename = '%s%s.sst' %(self.path,self.curr_active)
        
        try:
            if os.path.getsize(active_filename) >= self.THRESHOLD:
                self.curr_active += 1
                active_filename = '%s%s.sst' % self.path,self.curr_active
        except:
            pass
        with open(active_filename, 'a') as f:
            data = pickle.dumps((crc, ts, key_size, value_size, key, value))
            start = f.tell()
            length = len(data)
            f.write(data)
            return active_filename, start, length
 
 
    def list_keys(self):
        '''返回所有记录的Key列表'''
        return self.TABLE.keys()

    def marge(self,dst_path):
        '''合并数据，合并的路径不能与当前使用的路径相同，原有数据不能被删除'''
        shot_num = self.curr_active
        marge_bitcask = Bitcask(self.THRESHOLD,dst_path)
        
        for key in self.list_keys():
            info = self.TABLE.get(key)

            file_num = int(info[0].replace(".sst",'').replace(self.path,''))
            if file_num < shot_num:
                value = self.get(key)
                marge_bitcask.put(key,value)
    def marge2(self):
        '''合并数据文件，原有数据文件会被删除并重新生成'''
        tmp_path=self.path+"/_marge_tmp/"
        while(os.path.isdir(tmp_path)):
            tmp_path +="_marge_tmp/"

        os.makedirs(tmp_path)

        shot_num = self.curr_active
        marge_bitcask = Bitcask(self.THRESHOLD,tmp_path)
        
        for key in self.list_keys():
            info = self.TABLE.get(key)

            file_num = int(info[0].replace(".sst",'').replace(self.path,''))
            if file_num < shot_num:
                value = self.get(key)
                marge_bitcask.put(key,value)
        
        for i in glob(self.path+"*.sst"):
            old_file_num = int(i.replace(".sst",'').replace(self.path,''))

            if old_file_num < shot_num:
                os.remove(i)
        
        for i in glob(tmp_path+"*.sst"):
            shutil.move(i,self.path)

        os.rmdir(tmp_path)

#############################################
## load or marge with hint file
#############################################
    def load_data_with_hint(self,path=None):
        '''使用线索文件加载数据文件，数据文件和线索文件应在同一个文件夹路径path中'''
        if path is not None:
            self.path = path
        for filename in glob(self.path+"*.hit"):
            self.load_file_with_hint(filename)
    def load_file_with_hint(self,hintfilename):
        '''使用线索文件加载数据文件'''
        f = open(hintfilename)
        data = f.read()
        list_items = data.split("t.")
    
        datafilename = hintfilename.replace("hit","sst")
        for item in list_items[:-1]:
            s= item+"t."
            record = pickle.loads(s)
            print "hint:",record
            if self.TABLE.has_key(record[4]) is True:
                ts1 = record[0]
                ts2 = self.__get_record(record[4])[0]
                if ts1 > ts2:
                    self.TABLE[record[4]]=(datafilename,record[3],record[2])
            else:
                self.TABLE[record[4]]=(datafilename,record[3],record[2])
        f.close()
    def marge_with_hint(self,dst_path):
        '''合并数据文件时同时生成线索文件'''
        shot_num = self.curr_active
        marge_bitcask = Bitcask(self.THRESHOLD,dst_path)
        
        for key in self.list_keys():
            info = self.TABLE.get(key)

            file_num = int(info[0].replace(".sst",'').replace(self.path,''))
            if file_num < shot_num:
                value = self.get(key)
                marge_bitcask.put(key,value)

                info = marge_bitcask.TABLE.get(key)
                marge_bitcask.__put_hint(key,info[1],info[2])
    def __io_write_hint(self,ts, key_size, value_size, value,key):
        '''将线索记录写入线索文件'''
        active_hintfile = '%s%s.hit' %(self.path,self.curr_active)
        print "hint_w:",ts,key_size,value_size,value,key
        with open(active_hintfile, 'a') as f:
            data = pickle.dumps((ts, key_size, value_size, value,key))
            start = f.tell()
            length = len(data)
            f.write(data)
            return active_hintfile, start, length
    def __put_hint(self,key,value_pos,value_size):
        '''将线索记录写入线索文件'''
        key_size = len(key)
        ts = datetime.now()

        info = self.__io_write_hint(ts, key_size, value_size, value_pos,key)
        return True

if __name__ == '__main__':
#    bitcask = Bitcask()
 #   bitcask.load_data()
#    bitcask.load_data_with_hint("marge/")
#    print bitcask.get("test")
   # print bitcask.get("good")
   # print bitcask.list_keys()

#    bitcask.marge("marge/")
  #  bitcask.marge_with_hint("marge2/")
    
    s = Bitcask()
    s.load_data("marge3/")
    s.marge2()

   # t = Bitcask()
    #t.load_data_with_hint("marge/")

#    print t.get("test")
 #   print t.get("good")
  #  print t.list_keys()
