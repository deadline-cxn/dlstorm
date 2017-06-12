/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_log.h
 **   Class:        CLog
 **   Description:  Log file class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_C_LOG
#define _DLCS_C_LOG
#include "dlstorm.h"
///////////////////////////////// CLog class
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
    char szBegin[TEXTNAME_SIZE];
    char szEnd[TEXTNAME_SIZE];
    char szLineSep[TEXTNAME_SIZE];
    char logfile[_MAX_PATH];
    char currentdir[_MAX_PATH];
    char logdir[_MAX_PATH];
    bool bActive;
    bool bLineFeeds;
    bool disable;
    bool bDebug;
    bool bQuiet;
    void _Add(const char *fmt, ...); // user defined
    void _DebugAdd(const char *fmt, ...); // user defined
};

#endif // _DLCS_C_LOG
