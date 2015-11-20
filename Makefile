CC=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
LIBS=-lsqlite3
INCLUDES=-I.

server : sql.o
	$(CC) -o server server.cxx sql.o $(CXXFLAGS) $(LIBS) $(INCLUDES)

sql.o : sql.cpp
	$(CC) -c sql.cpp

clean : 
	rm sql.o server
