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
 ***************************************************************/
#ifndef _DLCS_DATA_STORAGE_CLASS
#define _DLCS_DATA_STORAGE_CLASS
#include "c_log.h"
#include "c_var.h"
#include "dlcs.h"

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// CC_Data class
///////////////////////////////////////////////////////////////////////////////
class CC_Data : public CVarSet {
   public:
    CC_Data();
    CC_Data(const char* infilename);
    CC_Data(const char* infilename, CLog* inpLog);
    ~CC_Data(void);
    void  Init(void);
    void  CleanUp(void);
    void  SetToDefaults(void);
    CLog* pLog;
};

#endif  // _DLCS_DATA_STORAGE_CLASS
