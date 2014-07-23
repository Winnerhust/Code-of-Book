#coding:utf-8
#https://github.com/aholyoke/BPlusTree/blob/master/BPlusTree.py
from bisect import bisect_left,bisect_right

class BPTreeLeaf(object):
    def __init__(self,keys,next_leaf,capacity):
        self._keys = keys #sorted list
        self._next = next_leaf # next BPTreeLeaf
        self._capacity = capacity
    def find(self,key):
        '''返回key是否在叶子节点的keys列表中'''
        return key in self._keys
    def insert(self,key):
        '''插入key值到该叶子节点,不插入重复key值.
        如果不分裂返回(None,None);
        如果分裂，则返回(pkey,ppointer)，代表需要提升的key值和新创建的叶子节点'''
        index = bisect_left(self._keys,key)

        if index == len(self._keys) or self._keys[index] != key:
            self._keys.insert(index,key)

        cap = self._capacity
        split_index = (cap+1)//2

        if len(self._keys) > cap:
            new_leaf = BPTreeLeaf(self._keys[split_index:],self._next,cap)

            self._keys = self._keys[:split_index]
            self._next = new_leaf

            return (new_leaf._keys[0],new_leaf)
        return (None,None)
    def num_nodes(self):
        return 0
    def num_leaves(self):
        return 1
    def num_keys(self):
        return len(self._keys)
    def height(self):
        return 0
    def __str__(self):
        return str(self._keys)
class BPTreeNode(object):
    def __init__(self,keys,pointers,capacity):
        '''新建一个非叶子节点,keys = [key0,key1,...keyN],pointers = [pointer1,pointer2,...pointerN+1]'''
        self._keys = keys
        self._pointers = pointers
        self._capacity = capacity
    def find(self,key):
        '''返回key值是否在这个子树的叶子节点中'''
        return self._pointers[bisect_right(self._keys,key)].find(key)
    def insert_here(self,pos,key,pointer):
        '''将key 插入在位置pos,pointer插入在位置pos+1
        如果不分裂，返回(None,None)
        如果分裂，返回(pkey,ppointer),代表被提升的key和指向新创建的BPTreeNode的指针
        '''
        self._keys.insert(pos,key)
        self._pointers.insert(pos+1,pointer)

        cap = self._capacity
        split_index = (cap+1)//2

        if len(self._keys) > cap:
            pkey = self._keys[split_index]
            self._keys.remove(pkey)

            new_node = BPTreeNode(self._keys[split_index:],self._pointers[split_index+1:],cap)

            self._keys = self._keys[:split_index]
            self._pointers = self._pointers[:split_index+1]

            return (pkey,new_node)
        return (None,None)
    def insert(self,key):
        '''将key插入该子树
        如果没有分裂，返回(None,None)
        如何分裂,返回(pkey,ppointer)'''

        pkey,ppointer = self._pointers[bisect_right(self._keys,key)].insert(key)

        if pkey is not None:
            pos = bisect_left (self._keys,key)
            promoted_key,new_node = self.insert_here(pos,pkey,ppointer)
            return (promoted_key,new_node)
        return (None,None)
    def height(self):
        return 1 + self._pointers[0].height()
    def num_keys(self):
        return sum(map(lambda n:n.num_keys(),self._pointers))
    def num_leaves(self):
        return sum(map(lambda n: n.num_leaves(),self._pointers))
    def __str__(self):
        return str(self._keys)
        
class BPTree(object):
    def __init__(self,capacity):
        self._tree = BPTreeLeaf([],None,capacity)
        self._capacity = capacity
    def insert(self,key):
        pkey,ppointer = self._tree.insert(key)
        if pkey is not None:
            new_node = BPTreeNode([pkey],[self._tree,ppointer],self._capacity)
            self._tree = new_node
    def find(self,key):
        return self._tree.find(key)
    def height(self):
        return self._tree.height()+1
    def num_keys(self):
        return self._tree.num_keys()
    def __str__(self):
        s = ""
        stack = []

        for level in range(1, self.height() + 1):
            s +="level [%d]:"%level
            if level == 1:
                s += str(self._tree)
                stack.append(self._tree)
            elif level == self.height():
                
                leaf = self._tree._pointers[0]
                for i in range(self.height()-2):
                    leaf = leaf._pointers[0]

                    while leaf._next is not None:
                        s += str(leaf)+"->"
                        leaf = leaf._next
                    s += str(leaf)
            else:
                child_stack = []
                while len(stack) > 0:
                    for child in stack.pop()._pointers:
                        s+= str(child) + " "
                        child_stack.append(child)
                stack = child_stack
                
            s += "\n"
        return s
                    
def LeafTest():
    a = BPTreeLeaf([1,4,7],None,4)
    pkey,ppointer = a.insert(5)
    print pkey,ppointer
    print a,ppointer #[1, 4, 5, 7] None
    pkey,ppointer = a.insert(6)
    print pkey,ppointer
    print a,ppointer #[1, 4] [5, 6, 7]
def NodeTest():
    l1 = BPTreeLeaf([1,3],None,2)
    l2 = BPTreeLeaf([5,6],None,2)
    b = BPTreeNode([4],[l1,l2],2)
    print b
    print b.insert(9)
    print b.find(9)
    for i in b._pointers:
        print i,
def TreeTest():
    b = BPTree(2)
    b.insert(1)
    b.insert(3)
    b.insert(2)
    b.insert(4)
    b.insert(5)
   
    print b
    
if __name__ == '__main__':
    #LeafTest()    
    #NodeTest()
    TreeTest()
