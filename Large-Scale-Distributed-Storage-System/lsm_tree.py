# -*- coding: utf-8 -*-
 
import os
import shutil
import skiplist 
import cPickle as pickle
from glob import glob
from binascii import crc32
from datetime import datetime

class KVTuple(object):
    '''at least define __lt__ and __eq__'''
    def __init__(self,key,value):
        self.key = key
        self.value = value
    def __eq__(self,other):
        return self.key == other.key
    def __lt__(self,other):
        return self.key < other.key
    def __repr__(self):
        return "(%s:%s)"%(str(self.key),str(self.value))

class LSMTree(object):
    def __init__(self,THRESHOLD=10000,path="./"):
        self.TABLE = skiplist.SkipList(isdup=False)
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
        self.TABLE.insert(KVTuple(key,info))
        return True

    def delete(self,key):
        '''删除Key对应的记录'''
        return  self.put(key,None)
    def update(self,key):
        '''更新Key对应的记录'''
        return self.put(key,value)


    def __get_record(self,key):
        '''根据key取出数据文件中对应的整条记录'''
        info = None
        try:
        	info = self.TABLE.get(KVTuple(key,None)).value
        except:
            pass
        if not info:
            #read from other older data file
            return self.get_record_from_files(key)
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
        pass
        #return self.TABLE.keys()
    def get_record_from_files(self,key):    
        filenums=[]
        for filename in glob(self.path+"*.sst"):
            filenum = int(filename.replace(self.path,'').replace('.sst',''))
            filenums.append(filenum)
        filenums.sort(reverse=True)    
        for i in filenums:
            filename = self.path+str(i)+'.sst'
            record = self.get_record_from_file(key,filename)
            if record != None:
                return record
        return None
    def get_record_from_file(self,key,filename):
        try:
            f = open(filename)
        except:
            f.close()
            return None
        data = f.read()
        list_items = data.split("t.")
        
        records=[]
        for item in list_items[:-1]:
            s= item+"t."
            record = pickle.loads(s)

            if key == record[4]:
                records.append(record)
        f.close()
        if len(records) > 0:
            return records[len(records)-1]
        else:
            return None

if __name__ == '__main__':
    lsm = LSMTree()
#    lsm.put("100","good")
#    lsm.put("200","cood")
#    lsm.put("100","xood")
    print lsm.TABLE
    print lsm.get("100")
    print lsm.get("200")
  
