/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_log.cpp
 **   Class:        CLog
 **   Description:  Log file class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_log.h"
/////////////////////////////////// CLog class
CLog::CLog() {
    memset(logfile, 0, _MAX_PATH);
    strcpy(logfile, "log.log");
    Initialize();
    //    if(bDebug) _Add("DEBUGGING ACTIVE");
}
CLog::CLog(const char *szFilename) {
    Initialize();
    SetName(szFilename);
    Restart();
    LineFeedsOn();
    //    if(bDebug) _Add("DEBUGGING ACTIVE");
}
CLog::CLog(const char *szFilename, bool bQ) {
    Initialize();
    SetName(szFilename);
    bQuiet = bQ;
    Restart();
    LineFeedsOn();
    //    if(bDebug) _Add("DEBUGGING ACTIVE");
}
CLog::~CLog() {}
void CLog::Initialize(void) {
#ifdef _DLCS_DEBUG_LOG
    bDebug = true;
#else
    bDebug = false;
#endif
    strcpy(logfile, "[CLog].log");
    strcpy(szBegin, "(");
    strcpy(szEnd, "):");
    strcpy(szLineSep, "*****************************************************************");
    bActive    = true;
    bLineFeeds = false;
    bQuiet     = true;
}
void CLog::SetName(const char *szFilename) { strcpy(logfile, szFilename); }
/*
void CLog::_Add(const char *fmt, ...) {

    if(!bActive) return;
    char ach[512];
    char temp[512];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    time_t td;
    struct tm *dc;
    time(&td);
    dc=localtime(&td);
    strcpy(temp,asctime(dc));
    temp[strlen(temp)-1]=0;

    FILE *fp=fopen(logfile,"a+");
    if(fp) {
        if(!bLineFeeds) {
            fprintf(fp,"%s%s%s%s",szBegin,temp,szEnd,ach);
            if(!bQuiet)
                printf("%s%s%s%s",szBegin,temp,szEnd,ach);

        } else {
            fprintf(fp,"%s%s%s%s\n",szBegin,temp,szEnd,ach);
            if(!bQuiet)
                printf("%s%s%s%s\n",szBegin,temp,szEnd,ach);

        }
        fclose(fp);
    }
}

void CLog::_DebugAdd(const char *fmt, ...) {

    if(!bActive) return;
    char ach[512];
    char temp[512];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    time_t td;
    struct tm *dc;
    time(&td);
    dc=localtime(&td);
    strcpy(temp,asctime(dc));
    temp[strlen(temp)-1]=0;

    FILE *fp=fopen(logfile,"a+");
    if(fp) {
        if(!bLineFeeds) {
            fprintf(fp,"%s%s%s%s",szBegin,temp,szEnd,ach);
            if(!bQuiet)
                printf("%s%s%s%s",szBegin,temp,szEnd,ach);

        } else {
            fprintf(fp,"%s%s%s%s\n",szBegin,temp,szEnd,ach);
            if(!bQuiet)
                printf("%s%s%s%s\n",szBegin,temp,szEnd,ach);

        }
        fclose(fp);
    }
}

*/
void CLog::AddEntry(const char *fmt, ...) {
    if (!bActive) return;
    char    ach[512];
    char    temp[512];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);
    time_t     td;
    struct tm *dc;
    time(&td);
    dc = localtime(&td);
    strcpy(temp, asctime(dc));
    temp[strlen(temp) - 1] = 0;

    FILE *fp = fopen(logfile, "a+");
    if (fp) {
        if (!bLineFeeds) {
            fprintf(fp, "%s%s%s%s", szBegin, temp, szEnd, ach);
            if (!bQuiet) printf("%s%s%s%s", szBegin, temp, szEnd, ach);

        } else {
            fprintf(fp, "%s%s%s%s\n", szBegin, temp, szEnd, ach);
            if (!bQuiet) printf("%s%s%s%s\n", szBegin, temp, szEnd, ach);
        }
        fclose(fp);
    }
}
void CLog::AddEntryNoTime(const char *fmt, ...) {
    if (!bActive) return;
    char    ach[512];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);

    FILE *fp = fopen(logfile, "a+");
    if (fp) {
        if (!bLineFeeds) {
            fprintf(fp, "%s", ach);
            if (!bQuiet) printf("%s\n",ach);
        } else {
            fprintf(fp, "%s\n", ach);
            if (!bQuiet) printf("%s\n", ach);
        }
        fclose(fp);
    }
}
void CLog::AddLineSep(void) {
    if (bActive == false) return;

    FILE *fp = fopen(logfile, "a+");
    if (fp) {
        fprintf(fp, "%s\n", szLineSep);
        fclose(fp);
    }
}
void CLog::Off(void) { bActive = false; }
void CLog::On(void) { bActive = true; }
bool CLog::IsActive(void) { return bActive; }
bool CLog::Restart(void) {
    if (!bActive) return false;
    FILE *fp = fopen(logfile, "w+");
    if (fp) {
        fclose(fp);
        return true;
    }

    return false;
}
void CLog::LineFeedsOn(void) { bLineFeeds = true; }
void CLog::LineFeedsOff(void) { bLineFeeds = false; }
