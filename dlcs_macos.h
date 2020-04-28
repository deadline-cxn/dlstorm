/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_macos.h
 **   Description:  DL Code Storm Header for MAC OS's
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_MACOS
#define _DLCS_MACOS

/////////////////////////////////////////////////////////////////
// MAC OS Defines and Includes

#ifdef __MACOS__

#define DLCS_MACOS
#define DLCS_OS_STRING         "Mac OS"
#define DLCS_OS_WHICH          DLCS_OS_MACOS
#define PATH_SEP               ':'
#define DLCS_SYSTEM_NONWINDOWS "dlcs_nonwindows.h"

void Sys_PumpEvents(void);

#endif

#endif  // _DLCS_MACOS
