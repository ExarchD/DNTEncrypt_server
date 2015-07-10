g++ sql.cpp -c
g++ server.cxx -o server sql.o -I. -std=c++11 -l sqlite3
