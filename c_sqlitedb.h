#ifndef _C_SQLITE_DB
#define _C_SQLITE_DB

#include "dlstorm.h"

// apt install libsqlite3-dev / will install it to /usr/include/sqlite3.h
#include "sqlite3.h" 

using namespace std;

extern char            **db_result;
extern int             dbr_nrow;
extern int             dbr_ncol;
extern vector <string> vcol_head;
extern vector <string> vdata;
extern bool            db_logresult;
extern sqlite3         *pDB;

extern "C" int    db_query(const char *fmt, ...);
extern "C" int    db_queryl(const char *fmt, ...);
extern "C" string db_getvalue(string s_col1, string s_key,string s_col2);

#endif
