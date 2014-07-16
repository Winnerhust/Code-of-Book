
CC=g++
GTEST_CPPFLAGS=-I/usr/include -pthread 
GTEST_LDFLAGS=-L/usr/lib64 -lgtest -pthread

CPPFLAGS=-Isrc/
all: testcase 

testcase:
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/skiplist_test.cpp
	$(CC) $(GTEST_LDFLAGS) -g -o testcase skiplist_test.o 
clean:	
	rm -f *.o testcase



	
#	g++  $(gtest-config --cppflags --cxxflags) -o $(TARGET).o -c test_main.cpp
#	g++ $(gtest-config --ldflags --libs) -o $(TARGET) $(TARGET).o
