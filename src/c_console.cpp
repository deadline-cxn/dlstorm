/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_console.cpp
 **   Class:        C_CONS
 **   Description:  Console class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/

#include "c_console.h"

C_CONS::C_CONS() { Init(); }

C_CONS::C_CONS(CLog *pInLog) {
    Init();
    pLog = pInLog;
    strcpy(szFilename, _DLCS_C_CONSOLE_DEFAULT_FILENAME);
    pCVars = new CVarSet(pLog, szFilename);
}

C_CONS::C_CONS(CLog *pInLog, CGAF *pInGAF) {
    Init();
    pLog = pInLog;
    pGAF = pInGAF;
    strcpy(szFilename, _DLCS_C_CONSOLE_DEFAULT_FILENAME);
    pCVars = new CVarSet(pLog, szFilename);
}

C_CONS::C_CONS(CLog *pInLog, CVarSet *pInCVars) {
    Init();
    pLog   = pInLog;
    pCVars = pInCVars;
    strcpy(szFilename, pCVars->szFilename);
}

C_CONS::C_CONS(CLog *pInLog, CGAF *pInGAF, CVarSet *pInCVars) {
    Init();
    pLog   = pInLog;
    pGAF   = pInGAF;
    pCVars = pInCVars;
    strcpy(szFilename, pCVars->szFilename);
}

C_CONS::C_CONS(CLog *pInLog, CGAF *pInGAF, const char *szInFilename) {
    Init();
    pLog = pInLog;
    pGAF = pInGAF;
    strcpy(szFilename, szInFilename);
    pCVars = new CVarSet(pLog, szFilename);
}

C_CONS::C_CONS(CLog *pInLog, const char *szInFilename) {
    Init();
    pLog = pInLog;
    strcpy(szFilename, szInFilename);
    pCVars = new CVarSet(pLog, szFilename);
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
    if (vConsoleBuffer.size() > iLines) {
        vConsoleBuffer.erase(vConsoleBuffer.begin());
    }
    char ach[512];
    memset(ach, 0, 512);
    va_list vaz;
    va_start(vaz, fmt);
    vsprintf(ach, fmt, vaz);
    va_end(vaz);
    vConsoleBuffer.push_back(ach);
}

void C_CONS::_Execute(const char *cmd) {
    LogEntry("C_CONS::_Execute> (%s)...", cmd);
    pCVars->bCallFunction(cmd);
}
