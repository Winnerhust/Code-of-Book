#ifndef _SKIPLIST_CPP
#define _SKIPLIST_CPP

#include "skiplist.h"
#include <time.h>
#include <iostream>
#include <stdio.h>

using namespace skiplist;

template<typename type,unsigned int MAXLEVEL>
SkipList<type,MAXLEVEL>::SkipList()
	:size_(0),level_(1),tail(NULL)
{
	srand((unsigned int)(time(NULL)));

	head = new node_t;

}

template<typename type,unsigned int MAXLEVEL>
SkipList<type,MAXLEVEL>::~SkipList()
{
	node_t *node = head->level[0].forward;
	node_t *next = NULL;
	
	delete head;

	while(node != NULL){
		next = node->level[0].forward;
		delete node;
		node = next;
	}
}


template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::insert(const_value_type &value)
{
	node_t *update[MAXLEVEL];
	node_t *node;
	
	node = head;
	for(int i = level_ - 1; i >= 0 ; --i){
		while(node->level[i].forward && node->level[i].forward->value < value){
			node = node->level[i].forward;
		}
		update[i] = node;
	}
	
	int level = randlevel();

	if( level > level_){
		for(int i = level_; i < level; ++i){
			update[i] = head;
		}

		level_ = level;
	}

	node = new node_t();
	
	node->value = value;
	
	for(int i = 0; i < level_; ++i){
		node->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = node;
	}
	
	node->backward = (update[0] == head ? NULL:update[0]);
	if(node->level[0].forward != NULL){
		node->level[0].forward->backward = node;
	}else{
		tail = node;
	}	
	size_++;

}

template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::remove(const_value_type &value)
{
	node_t *update[MAXLEVEL]={NULL};
	node_t *node = head;

	for(int i = MAXLEVEL - 1; i >= 0; --i){
		while(node->level[i].forward && node->level[i].forward->value < value){
			node = node->level[i].forward;
		}

		update[i] = node;
	}

	node = node->level[0].forward;

	if(node && node->value == value){
		for(int i = 0; i < level_ ; ++i){
			if(update[i]->level[i].forward  == node){
				update[i]->level[i].forward = node->level[i].forward;
			}
		}
		
		if(node->level[0].forward != NULL){
			node->level[0].forward->backward = node->backward;
		}else{
			tail = node->backward;
		}

		while(level_ > 1 && head->level[level_ -1].forward == NULL){
			level_--;
		}
		delete node;
		size_--;
	}
}
	
template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::clear()
{
	node_t *node = head->level[0].forward;
	node_t *next = NULL;
	
	while(node != NULL){
		next = node->level[0].forward;
		delete node;
		node = next;
	}
	
	for(int i = level_ - 1; i >= 0 ; --i){
		head->level[i].forward = NULL;
	}

	size_ = 0;
	level_ = 1;
	tail = NULL;
}

template<typename type,unsigned int MAXLEVEL>
void SkipList<type,MAXLEVEL>::print()
{
	node_t *node = NULL;

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
