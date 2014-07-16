#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <stdlib.h>
namespace skiplist{

template<typename type,unsigned int MAXLEVEL = 16>
struct SkipListNode
{
	typedef SkipListNode* ptr_t;
	typedef const SkipListNode* const_ptr_t;

	type value;
	struct SkipListNode *backward;
	struct skiplistLevel {
		struct SkipListNode *forward;
	}level[MAXLEVEL];

	SkipListNode():backward(NULL)
	{
		for( int i = 0; i < MAXLEVEL; ++i){
			level[i].forward = NULL;
		}
	}
};

template<typename List,unsigned int MAXLEVEL = 16>
class ConstIterator
{
typedef ConstIterator<List> self_t;

public:
	typedef typename List::value_type value_type;
	typedef typename List::const_reference reference;
	typedef typename List::const_pointer pointer;
private:
	typedef typename List::iterator iterator;
	typedef SkipListNode<value_type,MAXLEVEL> node_t;
	typedef typename node_t::ptr_t node_ptr_t;
	typedef typename node_t::const_ptr_t node_const_ptr_t;
public:
	ConstIterator():node_(NULL),level_(0)
	{
	}
	ConstIterator(const self_t &other)
	{
		*this = other;
	}
	self_t &operator =(const self_t &other)
	{
		node_ = other.node_;
		level_ = other.level_;
		return *this;
	}
	ConstIterator(const iterator &other)
	{
		*this = other;
	}
	self_t &operator = (const iterator &other)
	{
		node_ = other;
		return *this;
	}
	ConstIterator(node_ptr_t node):level_(0)
	{
		node_ = node;
	}
	ConstIterator(node_const_ptr_t node):level_(0)
	{
		node_ = const_cast<node_ptr_t>(node);
	}
public:
	void setlevel(int level)
	{
		level_ = level;
	}
	reference operator *() const
	{
		// NOTICE 对end的访问undefined
		return node_->value;
	}
	pointer operator ->() const
	{
		// NOTICE 对end的访问undefined
		return &(node_->value);
	}
	bool operator ==(const self_t &other) const
	{
		return (node_ == other.node_);
	}
	bool operator !=(const self_t &other) const
	{
		return (node_ != other.node_);
	}
	self_t &operator ++()
	{
		node_ = node_->level[level_].forward;
		return *this;
	}

	self_t operator ++(int )
	{
		self_t tmp = *this;

		node_ = node_->level[level_].forward;

		return tmp;
	}
	self_t &operator --()
	{
		node_ = node_->backward;
		return *this;
	}

	self_t operator --(int )
	{
		self_t tmp = *this;

		node_ = node_->backward;

		return tmp;
	}
private:
	node_ptr_t node_;	
	int level_;
};

template<typename List,unsigned int MAXLEVEL = 16>
class Iterator
{
typedef Iterator<List> self_t;

public:
	typedef typename List::value_type value_type;
	typedef typename List::const_reference reference;
	typedef typename List::const_pointer pointer;
private:
	typedef typename List::iterator iterator;
	typedef SkipListNode<value_type,MAXLEVEL> node_t;
	typedef typename node_t::ptr_t node_ptr_t;
	typedef typename node_t::const_ptr_t node_const_ptr_t;
public:
	Iterator():node_(NULL),level_(0)
	{
	}
	Iterator(const self_t &other)
	{
		*this = other;
	}
	self_t &operator =(const self_t &other)
	{
		node_ = other.node_;
		level_ = other.level_;
		return *this;
	}
	Iterator(node_ptr_t node):level_(0)
	{
		node_ = node;
	}
	Iterator(node_const_ptr_t node):level_(0)
	{
		node_ = const_cast<node_ptr_t>(node);
	}
public:
	void setlevel(int level)
	{
		level_ = level;
	}
	reference operator *() const
	{
		// NOTICE 对end的访问undefined
		return node_->value;
	}
	pointer operator ->() const
	{
		// NOTICE 对end的访问undefined
		return &(node_->value);
	}
	bool operator ==(const self_t &other) const
	{
		return (node_ == other.node_);
	}
	bool operator !=(const self_t &other) const
	{
		return (node_ != other.node_);
	}
	self_t &operator ++()
	{
		node_ = node_->level[level_].forward;

		return *this;
	}
	self_t operator ++(int )
	{
		self_t tmp = *this;
		node_ = node_->level[level_].forward;
		return tmp;
	}
	self_t &operator --()
	{
		node_ = node_->backward;
		return *this;
	}

	self_t operator --(int )
	{
		self_t tmp = *this;

		node_ = node_->backward;

		return tmp;
	}
private:
	node_ptr_t node_;	
	int level_;
};



////////////////////////////////////
template<typename T> class SkipList_traits;



template<typename T>
struct SkipList_traits<T *>{
	typedef T pointee_type;
	typedef T * value_type;
	typedef const T * const_value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef int difference_type;

};

template<typename T>
struct SkipList_traits<const T *>{
	typedef T pointee_type;
	typedef const T* value_type;
	typedef const T* const const_value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef int difference_type;
};
template<>
struct SkipList_traits<int>{
	typedef int& pointee_type;
	typedef int value_type;
	typedef const int const_value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef int difference_type;
};

template<typename type,unsigned int MAXLEVEL = 16>
class SkipList
{
	typedef SkipList<type,MAXLEVEL> self_t;
public:
	typedef typename SkipList_traits<type>::pointee_type pointee_type;
	typedef typename SkipList_traits<type>::value_type value_type;
	typedef typename SkipList_traits<type>::const_value_type const_value_type;
	typedef Iterator<self_t,MAXLEVEL> iterator;
	typedef ConstIterator<self_t,MAXLEVEL> const_iterator;
	typedef typename SkipList_traits<type>::difference_type difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef SkipListNode<type,MAXLEVEL> node_t;
public:	

	SkipList();
	~SkipList();
	
	void insert(const_value_type &value);
	void remove(const_value_type &value);
	void clear();
	

	iterator find(const_value_type &value) 
	{
		node_t *node = head;

		for(int i = level_ - 1; i >= 0 ; --i){
			while(node->level[i].forward && node->level[i].forward->value < value){
				node = node->level[i].forward;
			}
		}
		node = node->level[0].forward;

		if(node != NULL && node->value == value){
			return node;
		}

		return iterator((node_t *)NULL);
	}
	
	iterator begin(int level = 0) 
	{
		iterator ret(head->level[level].forward);
		ret.setlevel(level);

		return ret;
	}

	iterator end() const
	{
		return iterator((node_t *)NULL);
	}

	iterator rbegin() const
	{
		return tail;
	}
	
	iterator rend() const
	{
		return iterator((node_t *)NULL);
	}
	//for deubug
	void print();

	unsigned long size() const{return size_;}
	unsigned int level() const{return level_;}
	unsigned int maxlevel() const{return MAXLEVEL;}
	bool empty() const {return (size_ == 0);}
protected:
	unsigned int randlevel() const;
protected:	

	node_t *head,*tail;
private:
	unsigned int level_;
	unsigned long size_;
};

#include "skiplist.cpp" //!! importent,you shouldn't delete it
}//end nameapce
#endif
