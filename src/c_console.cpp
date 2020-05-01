/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_console.cpp
 **   Class:        C_CONS
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/

#include "c_console.h"

C_CONS::C_CONS() { Init(); }

C_CONS::C_CONS(CLog *pInLog) {
    Init();
    pLog = pInLog;
    strcpy(szFilename, _DLCS_C_CONSOLE_DEFAULT_FILENAME);
    pCVars = new CVarSet(szFilename);
}

C_CONS::C_CONS(CLog *pInLog, CGAF *pInGAF) {
    Init();
    pLog = pInLog;
    pGAF = pInGAF;
    strcpy(szFilename, _DLCS_C_CONSOLE_DEFAULT_FILENAME);
    pCVars = new CVarSet(szFilename);
}
C_CONS::C_CONS(CLog *pInLog, CGAF *pInGAF, const char *szInFilename) {
    Init();
    pLog = pInLog;
    pGAF = pInGAF;
    strcpy(szFilename, szInFilename);
    pCVars = new CVarSet(szFilename);
}

C_CONS::C_CONS(CLog *pInLog, const char *szInFilename) {
    Init();
    pLog = pInLog;
    strcpy(szFilename, szInFilename);
    pCVars = new CVarSet(szFilename);
}

C_CONS::~C_CONS() {}

bool C_CONS::Init(void) {
    memset(szFilename, 0, _FILENAME_SIZE);
    iLines = 32;
    pCVars = 0;
    pLog   = 0;
    pGAF   = 0;
    memset(szConsoleInput, 0, _DLCS_CONSOLE_ENTRY_SIZE);
    vConsoleBuffer.clear();
    // iConsoleHistoryPosition = 0;
    return 1;
}

void C_CONS::AddLine(const char *fmt, ...) {
    if (buf.size() > iLines) {
        buf.erase(buf.begin());
    }
    char ach[512];
    memset(ach, 0, 512);
    va_list vaz;
    va_start(vaz, fmt);
    vsprintf(ach, fmt, vaz);
    va_end(vaz);
    buf.push_back(ach);
}

void C_CONS::_Execute(const char *cmd) {
    char temp[1024];
    memset(temp, 0, 1024);

    int  cQuoteCount = 0;  // bool bQuote2=0;
    char cmd2[1024];
    memset(cmd2, 0, 1024);
    strcpy(cmd2, cmd);

    for (unsigned int i = 0; i < strlen(cmd2); i++) {
        switch (cmd2[i]) {
            case '\n':
            case '\r':
            case '\t': cmd2[i] = ' '; break;
            case '"': cQuoteCount++; break;
            case ';':
                if (cQuoteCount & 1) {
                    cmd2[i] = '';  // Check " - Any ; in between quotes will temp change to 0xFF
                    break;
                }  // else bQuote2=true;
                break;
            default: break;
        }
    }

    if (cQuoteCount & 1) return;  // mismatched quote
    vector<string> ncmd = dlcs_explode(";", cmd2);
    if (ncmd.size() > 1) {
        for (unsigned int i = 0; i < ncmd.size(); i++) {
            _Execute((char *)ncmd[i].c_str());
        }
        return;
    }

    for (unsigned int i = 0; i < strlen(cmd2); i++) {
        switch (cmd2[i]) {
            case '': cmd2[i] = ';'; break;
        }
    }

    vector<string> narg = dlcs_explode(" ", cmd2);

    if (narg.size() > 0) {
        if (map_Functions.find((char *)narg[0].c_str()) != map_Functions.end()) {
            if (narg.size() > 0) {
                memset(temp, 0, 1024);
                strcpy(temp, narg[1].c_str());
                if (narg.size() > 1) {
                    for (i = 2; i < narg.size(); i++) {
                        strcat(temp, va(" %s", narg[i].c_str()));
                    }
                } else
                    strcpy(temp, "(null)");
                map_Functions.find(narg[0].c_str())->second(temp);
            }
            return;
        }
    }
    LogEntry("C_CONS::_Execute> %s...", cmd);
}
