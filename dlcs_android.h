/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_android.h
 **   Description:  Android Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_OS_ANDROID
#define _DLCS_OS_ANDROID

/////////////////////////////////////////////////////////////////
// Android Defines and Includes

#ifdef __ANDROID__

#define DLCS_ANDROID
#define DLCS_OS_STRING         "Android"
#define DLCS_OS_WHICH          DLCS_OS_ANDROID
#define PATH_SEP               '/'
#define DLCS_SYSTEM_NONWINDOWS "dlcs_nonwindows.h"

#endif  // _DLCS_SYS_ANDROID
