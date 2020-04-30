#ifndef _C_SQLITE_DB
#define _C_SQLITE_DB

#include "c_log.h"
#include "dlcs.h"
// apt install libsqlite3-dev / will install it to /usr/include/sqlite3.h
#include "sqlite3.h"
// using namespace std;

class C_SQLite {
   public:
    C_SQLite(const char *szInFilename, CLog *pInLog);
    ~C_SQLite();

    CLog *         pLog;
    char **        db_result;
    int            dbr_nrow;
    int            dbr_ncol;
    vector<string> vcol_head;
    vector<string> vdata;
    bool           db_logresult;
    sqlite3 *      pDB;
    char           szDBFilename[1024];

    void   Init();
    void   Shutdown();
    int    db_query(const char *fmt, ...);
    int    db_queryl(const char *fmt, ...);
    string db_getvalue(string s_col1, string s_key, string s_col2);
    int    OpenDB(const char *szInFilename);
};

#endif
