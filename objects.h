#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>
#include <string>

using namespace std;

int database_insert(string IDHASH, string MESSAGE, string READERS);
string database_retrieve(string IDHASH, string READERS);

#endif
