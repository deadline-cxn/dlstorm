#ifndef _DLCS_DB
#define _DLCS_DB

#include "c_log.h"
#include "dlcs.h"
// apt install libsqlite3-dev / will install it to /usr/include/sqlite3.h
#include "sqlite3.h"

#define _DLCS_DB_FLATFILE 1
#define _DLCS_DB_MYSQL    2
#define _DLCS_DB_SQLITE   3

struct stSQLite3Config {  // SQLite uses file database
    char     szFilename[_FILENAME_SIZE];
    sqlite3 *pDB;
};

struct stMysqlConfig {  // MySQL requires login information
    char szHost[_HOST_NAME_MAX];
    char szPort[_TINYNAME_SIZE];
    char szUser[_TEXTNAME_SIZE];
    char szPassword[_TEXTNAME_SIZE];
    char szDB[_TEXTNAME_SIZE];
};

struct stLMDBConfig {  // LMDB uses folder for database
    char szFoldername[_FILENAME_SIZE];
};

class C_DLCS_DB {
   public:
    C_DLCS_DB();
    C_DLCS_DB(CLog *pLog);
    ~C_DLCS_DB();

    CLog *          pLog;
    unsigned int    iType;
    stSQLite3Config SSQLite3Config;
    stMysqlConfig   SMysqlConfig;
    stLMDBConfig    SLMDBConfig;

    void Init(void);

    //////////////////////////////////////////////////////////
    // SQLLite Stuff
    int OpenSQLiteDB(const char *szInFilename);
    int CloseSQLiteDB(void);

    //////////////////////////////////////////////////////////
    // MySQL stuff

    void SetMySQLDB(const char *szInHost, const char *szPort, const char *szUser, const char *szPassword, const char *szDB);

    //////////////////////////////////////////////////////////
    // LMDB stuff

    void SetLMDBDB(const char *szInFolderName);
};

#endif