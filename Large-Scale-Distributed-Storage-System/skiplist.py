import random

class SkipNode:
	def __init__(self, level, val):
		self.value = val
		self.forward = [None]*(level+1) # array of pointerS

class SkipList:

    # initializer
    def __init__(self,max_level=16,prob = 0.5,isdup = True):
        self.MAX_LEVEL = max_level
        self.p = prob
        self.isdup = isdup
        self.level = 0
        self.head = SkipNode(self.MAX_LEVEL, None)
    # print method
    def __repr__(self):
        curr = self.head.forward[0]
        pstring = "{"
        
        while curr != None:
            pstring += str(curr.value)
            
            curr = curr.forward[0]
            if curr != None:
                pstring += ", "
                
        return pstring + "}"
    
    # contains: returns true if value is contained in the list
    def find(self, value):
        curr = self.head
        for i in range(self.level, -1, -1):
            while curr.forward[i] != None and curr.forward[i].value < value:
                curr = curr.forward[i]
        curr = curr.forward[0]
        
        return curr != None and curr.value == value
        
    # find and record traversal update[i]
    # returns update[] and insertion/deletion point
    def findandupdate(self, value):
        update = [None]*(self.MAX_LEVEL+1)
        curr = self.head
        
        for i in range(self.MAX_LEVEL, -1, -1):
            while curr.forward[i] != None and curr.forward[i].value < value:
                curr = curr.forward[i]
            update[i] = curr
                
        return (curr.forward[0], update)
            

    # insert method
    def insert(self, value):
        curr, update = self.findandupdate(value)
		
		#
        if self.isdup is False:
			if curr != None and curr.value == value:
				return 

        newlevel = self.randomlevel()

        if newlevel > self.level:
            for i in range(self.level+1, newlevel+1):
                update[i] = self.head

            self.level = newlevel
            
        newnode = SkipNode(newlevel, value)

        for i in range(0, newlevel+1):
            newnode.forward[i] = update[i].forward[i]
            update[i].forward[i] = newnode

    # deletemethod
    def delete(self, value):
        curr, update = self.findandupdate(value)

        if curr != None and curr.value == value:
            # found it. Remove from list
            for i in range(0, self.level+1):
                if update[i].forward[i] != curr:
                    break
                update[i].forward[i] = curr.forward[i]

            # decrease list level if the node deleted
            # had the highest level in the skip list
            while self.level > 0 and self.head.forward[self.level] == None:
                self.level -= 1

    """
    randomlevel:
    random level generator for skip list node
    """
    def randomlevel(self):
        level = 1
        while random.random() < self.p and level < self.MAX_LEVEL:
            level = level + 1
        return level

if __name__ == "__main__":
	s = SkipList(isdup=False)
	
	s.insert(100)
	s.insert(200)
	s.insert(100)
	s.insert(200)
	
	s.delete(100)
	print s.find(100)
	print s.find(200)
	print s
	
