#coding:utf-8
#B Tree
import math
class Entity(object):
    '''数据实体'''
    def __init__(self,key,value):
        self.key = key
        self.value = value
        
class Node(object):
    '''B树的节点'''
    def __init__(self):
        self.parent = None
        self.entitys = []
        self.childs = []
    def search(self,key):
        '''通过查找key返回一个数据实体，即键值对，失败返回None'''
        for entity in self.entitys:
            if key == entity.key:
                return entity
    def delete(self,key):
        '''通过key删除一个数据实体，并返回它和它的下标，(下标，实体)'''
        for i,entity in enumerate(self.entitys):
            if entity.key == key:
                del self.entitys[i]
                return (i,entity)
    def isleaf(self):
        '''判断该节点是否为一个子节点'''
        return len(self.childs)==0
    
    def add_entity(self,entity):
        '''添加一个数据实体'''
        self.entitys.append(entity)
        self.entitys.sort(key=lambda x:x.key)
    def del_entity(self,entity):
        '''删除一个实体'''
        self.entitys.remove(entity)
        self.entitys.sort(key=lambda x:x.key)
    def add_child(self,node):
        '''添加一个子节点'''
        node.parent = self
        self.childs.append(node)
        self.childs.sort(key = lambda x:x.entitys[0].key)
    def del_child(self,node):
        '''删除一个子节点'''
        self.childs.remove(node)
        self.childs.sort(key = lambda x:x.entitys[0].key)
    def join_node(self,other_node):
        '''将other节点合并到本节点'''
        self.entitys.extend(other_node.entitys)
        self.entitys.sort(key=lambda x:x.key)
        
        self.childs.extend(other_node.childs)
        self.childs.sort(key = lambda x:x.entitys[0].key)
        for i in self.childs:
            i.parent = self
        
class BTree(object):
    '''B 树'''
    def __init__(self,size = 6):
        self.size = size
        self.root = None
        self.length = 0
    def middle_size(self):
        ''''''
        return math.ceil((self.size+1)*1.0/2)
    
        
    def insert(self,key,value=None):
        '''插入一条数据到B树'''
        self.length += 1

        if self.root:
            curr = self.root
            while not curr.isleaf():
                for i,entity in enumerate(curr.entitys):
                    #print '---'
                    #print_nodes(curr)
                    #print i,entity.key,key
                    #print curr.childs
                    if key < entity.key :
                        curr = curr.childs[i]
                        break
                    elif key == entity.key:
                        entity.value = value
                        print 'duplicate key error,key = ',key
                        self.length -= 1
                        return
                else:
                    curr = curr.childs[-1]
            curr.add_entity(Entity(key,value))
            if len(curr.entitys) > self.size:
                self.__split(curr)
        else:
            self.root = Node()
            self.root.add_entity(Entity(key,value))
    def search(self,key):
            '''查询key，返回一个数据'''
            node = self.__find_node(key)
            if node:
                return node.search(key).value
    def delete(self,key):
        '''通过key删除一个数据项并返回删除的数据
            删除规则：
            1.1.叶节点如果拥有的数据个数>= ceil(M/2),直接删除
            1.2.叶节点如果拥有的数据个数< ceil(M/2),能像左右兄弟节点借的话，
                将对应父节点下移一个实体，然后将兄弟节点上移一个实体
            1.3.如果左右节点都不能借的话，下移父节点的一个实体，将该节点和左(右)兄弟节点合并。
                检查父节点是否需要调整，需要的话调整，循环调整父节点，可能会使树的高度降低
            2.1.如果是非叶子节点,找到该实体的右子树的最小值（一定在叶子节点），替换该实体。
                然后转换为删除叶节点
        '''
        
        node = self.__find_node(key)
        if not node:
            return
        if node.isleaf():
            i,entity = node.delete(key)
            self.length -= 1
            self.__adjustnode(node)
        else:
            i,entity = node.delete(key)
            self.length -= 1
            
            p = node.childs[i+1]
            while not p.isleaf():
                p = p.childs[0]

            node.add_entity(p.entitys[0])
            p.del_entity(p.entitys[0])
            self.__adjustnode(p)
                
    def __adjustnode(self,node):
        if len(node.entitys) >= self.middle_size()-1:
            return
        if self.need_adjust(node):
            if self.can_borrow_left(node) == 1:
                self.borrow_left(node)
            elif self.can_borrow_right(node) == 1:
                self.borrow_right(node)
            else:
                self.combine(node)
                if len(node.parent.entitys)==0:
                    self.root = node
                    self.root.parent = None
                else:
                    print "adjust parent"
                    self.__adjustnode(node.parent)
    def need_adjust(self,node):
        if len(node.entitys)< self.middle_size()-1 and node.parent != None:
            return 1
        return 0
    def can_borrow_left(self,node):
        pi = node.parent.childs.index(node)
        parent = node.parent
        if pi != 0:
            if len(parent.childs[pi-1].entitys) >=self.middle_size():
                return 1
        return 0
    def can_borrow_right(self,node):
        pi = node.parent.childs.index(node)
        parent = node.parent
        if pi != len(parent.childs)-1:
            if len(parent.childs[pi+1].entitys) >=self.middle_size():
                return 1
        return 0    
    def borrow_left(self,node):
        pi = node.parent.childs.index(node)

        parent = node.parent
        if pi == len(parent.childs)-1:
            node.add_entity(parent.entitys[-1])
            parent.del_entity(parent.entitys[-1])
        else:
            node.add_entity(parent.entitys[pi])
            parent.del_entity(parent.entitys[pi])

        left = node.parent.childs[pi-1]
        parent.add_entity(left.entitys[-1])
        left.del_entity(left.entitys[-1])
    def borrow_right(self,node):
        pi = node.parent.childs.index(node)

        parent = node.parent

        node.add_entity(parent.entitys[pi])
        parent.del_entity(parent.entitys[pi])

        right = node.parent.childs[pi+1]
        parent.add_entity(right.entitys[0])
        right.del_entity(right.entitys[0])
        
    def combine(self,node):
        pi = node.parent.childs.index(node)
        parent = node.parent
        
        if pi == len(parent.childs)-1:
            node.add_entity(parent.entitys[-1])
            parent.del_entity(parent.entitys[-1])
            
            node.join_node(parent.childs[pi-1])
            parent.del_child(parent.childs[pi-1])
        else:
            node.add_entity(parent.entitys[pi])
            parent.del_entity(parent.entitys[pi])
            
            node.join_node(parent.childs[pi+1])
            parent.del_child(parent.childs[pi+1])           
                        
    def empty(self):
        return self.length==0
    def __find_node(self,key):
        '''通过key值查询一个数据在那个节点，找的并返回该节点'''

        if self.root:
            curr = self.root
            while curr.isleaf() != None:
                for i,entity in enumerate(curr.entitys):
                    if entity.key > key:
                        curr = curr.childs[i]
                        break
                    if entity.key == key:
                        return curr
                    else:
                        pass
                else:
                    curr = curr.childs[-1]
            if curr.search(key):
                return curr
    def __split(self,node):
        '''
        分裂一个节点，规则为：
        1.中间节点前面的数据项移到父节点
        2.新建一个右兄弟节点，将中间节点右边的数据移到新节点
        '''
        middle = len(node.entitys)/2
        top = node.entitys[middle]

        right = Node()

        for entity in node.entitys[middle+1:]:
            right.add_entity(entity)
        for child in node.childs[middle+1:]:
            right.add_child(child)

        node.entitys = node.entitys[:middle]
        node.childs = node.childs[:middle+1]

        parent = node.parent
        
        if parent:
            parent.add_entity(top)
            parent.add_child(right)
            
            if len(parent.entitys) > self.size:
                self.__split(parent)
        else:
            self.root = Node()
            self.root.add_entity(top)
            self.root.add_child(node)
            self.root.add_child(right)
def insert_test(t):
    
    t.insert(20,'200')
    t.insert(40,'400')
    t.insert(60,'600')
    t.insert(70,'700')
    t.insert(80,'800')
    t.insert(10,'100')
    t.insert(50,'500')
    t.insert(51,'510')
    t.insert(52,'520')
    t.insert(53,'530')
    t.insert(54,'540')
    t.insert(55,'550')
    t.insert(56,'560')
    t.insert(57,'570')
    t.insert(58,'580')
    t.insert(81,'810')
    t.insert(82,'820')
    t.insert(83,'830')
def delete_test(t):
    t.delete(10)
    t.delete(52)
    t.delete(51)
    
def print_entitys(t):
    for i in t.root.entitys:
        print i.key,i.value
    print '-----------'
def print_nodes(t):
    for i in t.entitys:
        print i.key,i.value
def print_level(node,f=1):
    for j,e in enumerate(node.entitys):
        print '-'*4*f,e.key,e.value
    for i in node.childs:
        print_level(i,f+1)
        print '-'*4*(f+1)
        if i.parent !=node:
            print "error parent"


        
if __name__ == '__main__':
    t = BTree(4)
    insert_test(t)
    delete_test(t)
    print_level(t.root)
    
