/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_console.h
 **   Class:        C_CONS
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_C_CONS
#define _DLCS_C_CONS

#include "c_gaf.h"
#include "c_log.h"
#include "c_var.h"
#include "dlcs.h"

#define _DLCS_C_CONSOLE_DEFAULT_FILENAME "default_console_cvar_filename.ini"
#define _DLCS_CONSOLE_ENTRY_SIZE         1024
#define MAX_CONSOLE_BUFFER               1024
#define MAX_CONSOLE_MULTI_COMMANDS       256
#define MAX_CONSOLE_INPUT_BUFFER         1024

class C_CONS {
   public:
    C_CONS();
    C_CONS(CLog *pInLog);
    C_CONS(CLog *pInLog, CGAF *pInGAF);
    C_CONS(CLog *pInLog, CGAF *pInGAF, const char *szInFilename);
    C_CONS(CLog *pInLog, const char *szInFilename);
    ~C_CONS();

    char         szFilename[_FILENAME_SIZE];
    CVarSet *    pCVars;
    unsigned int iLines;
    CLog *       pLog;
    CGAF *       pGAF;
    char         szConsoleInput[_DLCS_CONSOLE_ENTRY_SIZE];

    vector<string> vConsoleBuffer;  // buffer vector

    // int          iConsoleHistoryPosition;
    // stConHistory *SConsoleHistory;

    bool Init(void);
    void AddLine(const char *fmt, ...);
    void _Execute(const char *cmd);
};

#endif  // _DLCS_C_CONS
