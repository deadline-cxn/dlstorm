/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef _SETH_LOG_CLASS
#define _SETH_LOG_CLASS

#ifdef _WIN32
#include <direct.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "dlstorm.h"
//#include <stdlib.h>

class CLog {
public:
    CLog(void);
    CLog(char *szFilename);
    CLog(char *szFilename, bool bQ);
    virtual ~CLog(void);

    void Initialize(void);
    void AddEntry(char *fmt, ...);
    void AddEntryNoTime(char *fmt, ...);
    void AddLineSep(void);
    void SetName(char *szFilename);
    void Off(void);
    void On(void);
    void LineFeedsOn(void);
    void LineFeedsOff(void);
    bool IsActive(void);
    bool Restart(void);

    char szBegin[255];
    char szEnd[255];
    char szLineSep[255];
    char logfile[_MAX_PATH];
    char currentdir[_MAX_PATH];
    char logdir[_MAX_PATH];
    bool bActive;
    bool bLineFeeds;
    bool disable;
    bool bDebug;
    bool bQuiet;

    void _Add(const char *fmt, ...);
    void _DebugAdd(const char *fmt, ...);
};

#endif
