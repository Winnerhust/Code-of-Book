#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <stdlib.h>
template<typename T> class SkipList_traits;

template<typename T>
struct SkipList_traits<T *>{
	typedef const T * const_value_type;
};

template<typename T>
struct SkipList_traits<const T *>{
	typedef const T * const const_value_type;
};
template<>
struct SkipList_traits<int>{
	typedef const int const_value_type;
};

template<typename type,unsigned int MAXLEVEL = 8>
class SkipList
{
public:
	typedef typename SkipList_traits<type>::const_value_type const_value_type;
public:	

	SkipList();
	~SkipList();
	
	bool find(const_value_type &value) const;
	bool insert(const_value_type &value);
	void remove(const_value_type &value);
	
	//for deubug
	void print();

	unsigned long size() const{return m_size;}
	unsigned int level() const{return m_level;}
	unsigned int maxlevel() const{return MAXLEVEL;}
protected:
	unsigned int randlevel() const;
protected:	
	typedef struct SkipListNode
	{
		type value;
		struct skiplistLevel {
			struct SkipListNode *forward;
		}level[MAXLEVEL];

		SkipListNode()
		{
			for( int i = 0; i < MAXLEVEL; ++i){
				level[i].forward = NULL;
			}
		}
	}SkipListNode;

	SkipListNode *head;
private:
	unsigned int m_level;
	unsigned long m_size;
};

#include "skiplist.cpp" //!! importent,you shouldn't delete it

#endif
cxy-/home/chenxueyou/skiplist/src$ cat skiplist.cpp 
#ifndef _SKIPLIST_CPP
#define _SKIPLIST_CPP

#include "skiplist.h"
#include <time.h>
#include <iostream>
#include <stdio.h>


template<typename type,unsigned int MAXLEVEL>
SkipList<type,MAXLEVEL>::SkipList()
	:m_size(0),m_level(1)
{
	srand((unsigned int)(time(NULL)));

	head = new SkipListNode;

}

template<typename type,unsigned int MAXLEVEL>
SkipList<type,MAXLEVEL>::~SkipList()
{
	SkipListNode *node = head->level[0].forward;
	SkipListNode *next = NULL;
	
	delete head;

	while(node != NULL){
		next = node->level[0].forward;
		delete node;
		node = next;
	}
}

template<typename type,unsigned int MAXLEVEL>
bool SkipList<type,MAXLEVEL>::find(const_value_type &value) const
{
	SkipListNode *node = head;

	for(int i = m_level - 1; i >= 0 ; --i){
		while(node->level[i].forward && node->level[i].forward->value < value){
			node = node->level[i].forward;
		}
	}
	node = node->level[0].forward;

	if(node != NULL && node->value == value){
		return true;
	}

	return false;
}

template<typename type,unsigned int MAXLEVEL>
bool SkipList<type,MAXLEVEL>::insert(const_value_type &value)
{
	SkipListNode *update[MAXLEVEL];
	SkipListNode *node;
	
	node = head;
	for(int i = m_level - 1; i >= 0 ; --i){
		while(node->level[i].forward && node->level[i].forward->value < value){
			node = node->level[i].forward;
		}
		update[i] = node;
	}
	
	int level = randlevel();

	if( level > m_level){
		for(int i = m_level; i < level; ++i){
			update[i] = head;
		}

		m_level = level;
	}

	node = new SkipListNode();
	
	node->value = value;
	
	for(int i = 0; i < m_level; ++i){
		node->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = node;
	}
	
	m_size++;

	return true;
}

template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::remove(const_value_type &value)
{
	SkipListNode *update[MAXLEVEL];
	SkipListNode *node = head;

	for(int i = MAXLEVEL; i >= 0; --i){
		while(node->level[i].forward && node->level[i].forward->value < value){
			node = node->level[i].forward;
		}

		update[i] = node;
	}

	node = node->level[0].forward;

	if(node && node->value == value){
		for(int i = 0; i < m_level ; ++i){
			update[i]->level[i].forward = node->level[i].forward;
		}	
		while(m+level > 1 && head->level[m_level -1].forward == NULL){
			m_level--;
		}
		delete node;
		m_size--;
	}
}
	
template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::print()
{
	SkipListNode *node = NULL;

	for(int i = MAXLEVEL - 1 ; i >= 0 ; --i){
		printf("LEVEL[%d]:",i);

		node = head->level[i].forward;

		while(node != NULL){
			std::cout<<node->value<<" -> ";
			node = node->level[i].forward;
		}

		printf("NULL\n");
	}
}

// determine a random level for a new node,
//    a 1/2 chance it will be a single level,
//      1/4 chance it will be two levels,
//      1/8 chance it will be three levels,
//    etc
template<typename type,unsigned int MAXLEVEL>
unsigned int SkipList<type,MAXLEVEL>::randlevel() const
{
	for (int i = 0; i < MAXLEVEL; i++) {
		if ((rand()%2) == 0)
			return i;
	}
	return (MAXLEVEL - 1);
}

#endif

#ifdef SKIPLIST_TEST_MAIN

int main()
{
	SkipList<int> skiplist;
	int s[] ={1,3,5,7,2};

	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	skiplist.insert(10);
	skiplist.print();

	std::cout<<"find:"<<skiplist.find(1)<<std::endl;
	return 0;
}

#endif
