#include "c_sqlitedb.h"

C_SQLite::C_SQLite(const char *szInFilename, CLog *pInLog) {
    Init();
    pLog = pInLog;
    OpenDB(szInFilename);
}
C_SQLite::~C_SQLite() {
    Shutdown();  //
}

/////////////////////////////////////////

void C_SQLite::Init() {
    pDB  = 0;
    pLog = 0;
    memset(szDBFilename, 0, _FILENAME_SIZE);
}
void C_SQLite::Shutdown() {
    sqlite3_close(pDB);  //
    LogEntry("SQLite shut down.\n");
}

int C_SQLite::OpenDB(const char *szInFilename) {
    strcpy(szDBFilename, szInFilename);
    int iResult = sqlite3_open(szDBFilename, &pDB);
    if (iResult) {
        LogEntry(va("SQLite version %s using database: [%s]\n", SQLITE_VERSION, szDBFilename));
    } else {
        LogEntry(va("SQLite version %s failed to load file [%s]\n", SQLITE_VERSION, szDBFilename));
    }
    return iResult;
}

string C_SQLite::db_getvalue(string s_col1, string s_key, string s_col2) {
    int i, w_col1, w_col2;
    // x, w_row

    w_col1 = -1;
    w_col2 = -1;

    string r_str;

    for (i = 0; i < dbr_ncol; i++) {
        if (vcol_head[i] == s_col1) {
            w_col1 = i;
        }
        if (vcol_head[i] == s_col2) {
            w_col2 = i;
        }
    }

    if ((w_col1 == -1) || (w_col2 == -1)) {
        r_str = "null";
        if (w_col1 == -1) r_str += "(col1)";
        if (w_col2 == -1) r_str += "(col2)";
    } else {
        for (i = 0; i < dbr_nrow; i++) {
            if (vdata[i * dbr_ncol + w_col1] == s_key) {
                r_str = vdata[i * dbr_ncol + w_col2];
            }
        }
    }

    return r_str;
}
/////////////////////////////////////////
int C_SQLite::db_queryl(const char *fmt, ...) {
    char    ach[1024];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);
    db_logresult = true;
    return db_query(ach);
}
/////////////////////////////////////////
int C_SQLite::db_query(const char *fmt, ...) {
    char    ach[1024];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);

    vector<string>::iterator it;
    string                   st;
    string                   s;
    string                   s_exe = ach;

    char *zErrMsg = 0;
    int   rc, x;

    rc = sqlite3_get_table(pDB, s_exe.c_str(), &db_result, &dbr_nrow, &dbr_ncol, &zErrMsg);

    if (vcol_head.size()) {
        vcol_head.clear();
    }
    if (vdata.size()) {
        vdata.clear();
    }

    if (rc == SQLITE_OK) {
        for (int i = 0; i < dbr_ncol; ++i) {
            vcol_head.push_back(db_result[i]);  // First row heading
        }
        for (int i = 0; i < dbr_ncol * dbr_nrow; ++i) {
            vdata.push_back(db_result[dbr_ncol + i]);
        }
    }

    sqlite3_free_table(db_result);

    if (db_logresult) {
        s.clear();
        for (it = vcol_head.begin(); it != vcol_head.end(); ++it) {
            if (s.length()) {
                while (s.length() < 8) s = s + " ";
                s = s + "\t";
            }
            s = s + (*it).c_str();
        }

        // dLog((char *)s.c_str());
        // dLog("=============================================================");

        s.clear();
        x = 0;
        for (it = vdata.begin(); it != vdata.end(); ++it) {
            st.clear();
            st = (*it);

            while (st.length() < 8) st = st + " ";
            st = st + "\t";
            s  = s + st;
            x++;
            if (x == dbr_ncol) {
                x = 0;
                // dLog((char *)s.c_str());
                s.clear();
            }
        }
    }
    db_logresult = 0;
    return rc;
}
