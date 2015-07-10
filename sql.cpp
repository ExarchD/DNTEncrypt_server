#include <cstring>
#include <vector>
#include <sqlite3.h> 
#include <iostream>

using namespace std;

string response;
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
  char* columnName;
  char* columnValueString;
  short int columnValueShortInt = 0;
  int columnValueInt = 0;

//  cout << "begin of callback function\n";

  for(i=0; i<argc; i++)
  {
    columnName = azColName[i];
    if (strcmp(columnName, "MESSAGE")==0 || strcmp(columnName, "LastName")==0)
    {
      columnValueString = argv[i];
      response = argv[i];
    //  cout << "columnName = " << columnName << "; value = " << columnValueString <<"\n";
    }
    
  }
//  cout << "end of call back function \n";
  return 0;
}


int database_insert(string idhash, string message,string readers)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    //   char *sql;

    /* Open database */
    rc = sqlite3_open("encrypt2.db", &db);
    if( rc ){
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }else{
        cout << "Opened database successfully\n";
    }

    /* Create SQL statement */

     string  create = "CREATE TABLE IF NOT EXISTS MESSAGES("  \
    "IDHASH         TEXT    NOT NULL," \
        "MESSAGE        TEXT    NOT NULL," \
        "READERS        TEXT    NOT NULL);";

    char *c_sql = new char[create.length() + 1];
    strcpy(c_sql, create.c_str());
    rc = sqlite3_exec(db, c_sql, callback, 0, &zErrMsg);

    /* Execute SQL statement */
    //   char * sql = message
    string ssql = "INSERT INTO MESSAGES (IDHASH,MESSAGE,READERS) "  \
                   "VALUES ('"+idhash+"', '"+message+"', '"+readers+"' ); " ;
    //   sql = "INSERT INTO MESSAGES (IDHASH,MESSAGE,READERS) "  \
    "VALUES ('d93e06289f776316337adbcc851cdf07b362b6a7', 'd93e06289f776316337adbcc851cdf07b362b6a7', 'd93e06289f776316337adbcc851cdf07b362b6a7' ); " ;
    char *i_sql = new char[ssql.length() + 1];
    strcpy(i_sql, ssql.c_str());
    rc = sqlite3_exec(db, i_sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout << "SQL error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }else{
        cout << "Table created successfully\n";
    }
    sqlite3_close(db);
    //   cout << "Hello World!";
    return 0;
}


string database_retrieve(string idhash, string readers)
{

    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;

    rc = sqlite3_open("encrypt2.db", &db);
    if( rc ){
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }else{
        cout << "Opened database successfully\n";
    }

    /* Create SQL statement */
//    cout << idhash << endl;
     string  read = "select MESSAGE from messages where IDHASH='"+idhash+"';";
     //string  read = "select MESSAGE from messages where idhash="+idhash;

//    char *c_sql = new char[create.length() + 1];
//    strcpy(c_sql, create.c_str());
    rc = sqlite3_exec(db, read.data(), callback, 0, &zErrMsg);
//    string response="HI";
    if( rc != SQLITE_OK ){
        cout << "SQL error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }else{
        cout << "Table created successfully\n";
    }
    sqlite3_close(db);
    return response;
}
