#include <iostream>
#include <gtest/gtest.h>
#include "skiplist.h"

#ifdef SKIPLIST_TEST_MAIN
#undef SKIPLIST_TEST_MAIN
#endif

using namespace std;

TEST(INIT_TEST,InitTest)
{
	SkipList<int> skiplist;

	EXPECT_EQ(skiplist.maxlevel(),8);
	EXPECT_EQ(skiplist.level(),1);
	EXPECT_EQ(skiplist.size(),0);
	EXPECT_EQ(skiplist.find(0),false);

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
		EXPECT_EQ(skiplist.find(s[i]),true);
	}
	EXPECT_EQ(skiplist.find(3),true);

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

	EXPECT_EQ(skiplist.find(s[0]),true);
	EXPECT_EQ(skiplist.find(s[1]),true);
	EXPECT_EQ(skiplist.find(s[2]),true);
	EXPECT_EQ(skiplist.find(s[3]),true);
	EXPECT_EQ(skiplist.find(s[4]),true);

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

	EXPECT_EQ(skiplist.find(s[0]),true);
	EXPECT_EQ(skiplist.find(s[1]),false);
	EXPECT_EQ(skiplist.find(s[2]),true);
	EXPECT_EQ(skiplist.find(s[3]),true);
	EXPECT_EQ(skiplist.find(s[4]),false);

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

	EXPECT_EQ(skiplist.find(s[0]),false);
	EXPECT_EQ(skiplist.find(s[1]),true);
	EXPECT_EQ(skiplist.find(s[2]),true);
	EXPECT_EQ(skiplist.find(s[3]),true);
	EXPECT_EQ(skiplist.find(s[4]),true);

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

	EXPECT_EQ(skiplist.find(s[0]),false);
	EXPECT_EQ(skiplist.find(s[1]),false);
	EXPECT_EQ(skiplist.find(s[2]),false);
	EXPECT_EQ(skiplist.find(s[3]),false);
	EXPECT_EQ(skiplist.find(s[4]),false);

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
