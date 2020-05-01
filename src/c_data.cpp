/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_data.h
 **   Class:        CC_Data
 **                 C_Profile
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_data.h"

//////////////////// CC_Data class
CC_Data::CC_Data() {
    Init();
    SetToDefaults();
}

CC_Data::CC_Data(const char* szInFilename) {
    Init();
    SetToDefaults();
    strcpy(szFilename, szInFilename);
}

CC_Data::CC_Data(const char* szInFilename, CLog* inpLog) {
    Init();
    pLog          = inpLog;
    CVarSet::pLog = pLog;
    SetToDefaults();
    strcpy(szFilename, szInFilename);
    LogEntry(va("CC_Data::CC_Data set filename to %s\n", szFilename));
    bLoad();
}

CC_Data::~CC_Data() { CleanUp(); }
void CC_Data::Init(void) {
    pLog = 0;
    memset(szFilename, 0, _FILENAME_SIZE);
    strcpy(szFilename, "vars.ini");
}
void CC_Data::CleanUp(void) {}
void CC_Data::SetToDefaults(void) {}
