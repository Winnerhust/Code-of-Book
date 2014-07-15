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
