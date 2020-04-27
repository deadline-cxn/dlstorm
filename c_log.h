/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
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
#include "os.h"
///////////////////////////////// CLog class
class CLog {
   public:
    CLog(void);
    CLog(const char *szFilename);
    CLog(const char *szFilename, bool bQ);
    virtual ~CLog(void);
    void Initialize(void);
    void AddEntry(const char *fmt, ...);
    void AddEntryNoTime(const char *fmt, ...);
    void AddLineSep(void);
    void SetName(const char *szFilename);
    void Off(void);
    void On(void);
    void LineFeedsOn(void);
    void LineFeedsOff(void);
    bool IsActive(void);
    bool Restart(void);
    char szBegin[_TEXTNAME_SIZE];
    char szEnd[_TEXTNAME_SIZE];
    char szLineSep[_TEXTNAME_SIZE];
    char logfile[_MAX_PATH];
    char currentdir[_MAX_PATH];
    char logdir[_MAX_PATH];
    bool bActive;
    bool bLineFeeds;
    bool disable;
    bool bDebug;
    bool bQuiet;
    void _Add(const char *fmt, ...);       // user defined
    void _DebugAdd(const char *fmt, ...);  // user defined
};

#endif  // _DLCS_C_LOG
