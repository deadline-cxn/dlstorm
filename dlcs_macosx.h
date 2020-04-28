/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_macosx.h
 **   Description:  DL Code Storm Header for MAC OSX
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_MACOSX
#define _DLCS_MACOSX

/////////////////////////////////////////////////////////////////
// MAC OS X Defines and Includes

#if (defined(__MACH__) && defined(__APPLE__))

#define DLCS_MACOSX
#define DLCS_OS_STRING         "Mac OSXS"
#define DLCS_OS_WHICH          DLCS_OS_MACOSX
#define PATH_SEP               '/'
#define DLCS_SYSTEM_NONWINDOWS "dlcs_nonwindows.h"

#endif  // _DLCS_MACOS
