#include "dlcs_db.h"

// - create new database class that abstracts db layer (will be able to use different methods such as flatfile, mysql,sqlite)

C_DLCS_DB::C_DLCS_DB() { Init(); }

C_DLCS_DB::C_DLCS_DB(CLog *pInLog) {
    Init();
    pLog = pInLog;
}

C_DLCS_DB::~C_DLCS_DB() {
    switch (iType) {
        case _DLCS_DB_SQLITE: CloseSQLiteDB(); break;
        default: break;
    }
}

void C_DLCS_DB::Init(void) {
    pLog  = 0;
    iType = 0;
    memset(&SSQLite3Config.szFilename, 0, sizeof(SSQLite3Config));
    memset(&SMysqlConfig, 0, sizeof(SMysqlConfig));
    memset(&SLMDBConfig, 0, sizeof(SLMDBConfig));
};

int C_DLCS_DB::OpenSQLiteDB(const char *szInFilename) {
    LogEntry(va("Working in %s", dlcs_getcwd()));
    iType = _DLCS_DB_SQLITE;
    strcpy(SSQLite3Config.szFilename, szInFilename);
    int iResult = 0;
    iResult     = sqlite3_open(SSQLite3Config.szFilename, &SSQLite3Config.pDB);
    if (!iResult) {
        LogEntry(va("SQLite version %s using database: [%s]\n", SQLITE_VERSION, SSQLite3Config.szFilename));
    } else {
        LogEntry(va("SQLite version %s failed to load file [%s]\n", SQLITE_VERSION, SSQLite3Config.szFilename));
    }
    return iResult;
}

int C_DLCS_DB::CloseSQLiteDB() {
    LogEntry(va("SQLite version %s closing file [%s]\n", SQLITE_VERSION, SSQLite3Config.szFilename));
    sqlite3_close(SSQLite3Config.pDB);
    return 0;
}
