/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_data.h
 **   Class:        CC_Data
 **                 C_Profile
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_DATA_STORAGE_CLASS
#define _DLCS_DATA_STORAGE_CLASS
#include "dlcs.h"
#include "c_var.h"

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// CC_Data class
///////////////////////////////////////////////////////////////////////////////
class CC_Data : public CVarSet {
public:
    CC_Data();
    CC_Data(char* infilename);
    ~CC_Data(void);
    void Initialize(void);
    void CleanUp(void);
    void SetToDefaults(void);
};

#endif // _DLCS_DATA_STORAGE_CLASS

