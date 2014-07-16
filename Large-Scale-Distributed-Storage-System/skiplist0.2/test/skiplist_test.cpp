#include <iostream>
#include <gtest/gtest.h>
#include "skiplist.h"

#ifdef SKIPLIST_TEST_MAIN
#undef SKIPLIST_TEST_MAIN
#endif

using namespace std;
using namespace skiplist;

TEST(INIT_TEST,InitTest)
{
	SkipList<int> skiplist;

	EXPECT_EQ(skiplist.maxlevel(),16);
	EXPECT_EQ(skiplist.level(),1);
	EXPECT_EQ(skiplist.size(),0);
	EXPECT_EQ(skiplist.begin(),skiplist.end());
	EXPECT_EQ(skiplist.rbegin(),skiplist.rend());
	EXPECT_EQ(skiplist.find(0),skiplist.end());

}

TEST(INSERT_TEST,InsertTest)
{
	SkipList<int> skiplist;
	
	int s[]={1,5,7,2};
	for(int i = 0; i < 4; ++i){
		skiplist.insert(s[i]);
	}
	skiplist.insert(3);

	EXPECT_EQ(skiplist.size(),5);

	for(int i = 0; i < 4; ++i){
		EXPECT_NE(skiplist.find(s[i]),skiplist.end());
	}
	EXPECT_NE(skiplist.find(3),skiplist.end());

}

TEST(INSERT_TEST,InsertDupTest)
{
	SkipList<int> skiplist;
	
	int s[]={1,5,5,2,1};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	EXPECT_EQ(skiplist.size(),5);

	skiplist.remove(s[1]);
	skiplist.remove(s[4]);

	EXPECT_EQ(skiplist.size(),3);

	EXPECT_NE(skiplist.find(s[0]),skiplist.end());
	EXPECT_NE(skiplist.find(s[1]),skiplist.end());
	EXPECT_NE(skiplist.find(s[2]),skiplist.end());
	EXPECT_NE(skiplist.find(s[3]),skiplist.end());
	EXPECT_NE(skiplist.find(s[4]),skiplist.end());

}

TEST(DELETE_TEST,DeleteTest)
{
	SkipList<int> skiplist;
	
	int s[]={1,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	skiplist.remove(s[1]);
	skiplist.remove(s[4]);

	EXPECT_EQ(skiplist.size(),3);

	EXPECT_NE(skiplist.find(s[0]),skiplist.end());
	EXPECT_NE(skiplist.find(s[2]),skiplist.end());
	EXPECT_NE(skiplist.find(s[3]),skiplist.end());

	EXPECT_EQ(skiplist.find(s[1]),skiplist.end());
	EXPECT_EQ(skiplist.find(s[4]),skiplist.end());

}

TEST(DELETE_TEST,DeleteDupTest)
{
	SkipList<int> skiplist;
	
	int s[]={1,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	skiplist.remove(s[0]);
	skiplist.remove(s[0]);

	EXPECT_EQ(skiplist.size(),4);

	EXPECT_NE(skiplist.find(s[1]),skiplist.end());
	EXPECT_NE(skiplist.find(s[2]),skiplist.end());
	EXPECT_NE(skiplist.find(s[3]),skiplist.end());
	EXPECT_NE(skiplist.find(s[4]),skiplist.end());

	EXPECT_EQ(skiplist.find(s[0]),skiplist.end());
}

TEST(DELETE_TEST,DeleteAllTest)
{
	SkipList<int> skiplist;
	
	int s[]={1,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}

	for(int i = 0; i < 5; ++i){
		skiplist.remove(s[i]);
	}
	
	EXPECT_EQ(skiplist.size(),0);

	EXPECT_EQ(skiplist.find(s[0]),skiplist.end());
	EXPECT_EQ(skiplist.find(s[1]),skiplist.end());
	EXPECT_EQ(skiplist.find(s[2]),skiplist.end());
	EXPECT_EQ(skiplist.find(s[3]),skiplist.end());
	EXPECT_EQ(skiplist.find(s[4]),skiplist.end());

}
TEST(ITERATOR_TEST,BeginTest)
{
	SkipList<int> skiplist;

	int s[]={4,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}

	SkipList<int>::iterator it=skiplist.begin();
	
	//min value
	EXPECT_EQ(*it,2);

}
TEST(ITERATOR_TEST,IteratorTest)
{
	SkipList<int> skiplist;

	int s[]={4,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}

	SkipList<int>::iterator it=skiplist.begin();
	
	//min value
	EXPECT_EQ(*it++,2);
	EXPECT_EQ(*it,3);
	
	EXPECT_EQ(*(++it),4);
}
TEST(ITERATOR_TEST,LevelTest)
{
	SkipList<int> skiplist;

	int s[] = {4,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	if(skiplist.level() > 2){
		SkipList<int>::iterator it=skiplist.begin();
		it.setlevel(2);
		std::cout<<"Level 2:";
		for(;it != skiplist.end(); it++){
			std::cout<<*it<<" ";
		}
		std::cout<<"\n";
	
		skiplist.print();
	}
}

TEST(ITERATOR_TEST,RangeTest)
{
	SkipList<int> skiplist;

	int s[] = {4,5,7,2,3};
	for(int i = 0; i < 5; ++i){
		skiplist.insert(s[i]);
	}
	
	SkipList<int>::iterator it = skiplist.find(3);
	
	for(;it != skiplist.end() && (*it < 6) ; ++it){
		std::cout<<*it<<" ";
	}
	std::cout<<endl;
}

TEST(ITERATOR_TEST,ReverseTest)
{
	SkipList<int> skiplist;
	int s[] = {1,3,5,7,9,11,13,15,17,19};
	for(int i = 0; i < 10; ++i){
		skiplist.insert(s[i]);
	}

	SkipList<int>::iterator it = skiplist.rbegin();

	for(int i = 10 - 1; it != skiplist.rend(); --i,++it){
		EXPECT_EQ(*it,s[i]);
	}

}
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
