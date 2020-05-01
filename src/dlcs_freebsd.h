/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_freebsd.h
 **   Description:  BSD Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_SYS_FREEBSD
#define _DLCS_SYS_FREEBSD

/////////////////////////////////////////////////////////////////
// FREEBSD Defines and Includes

#ifdef FREEBSD_NATIVE

#define DLCS_FREEBSD
#define DLCS_OS_STRING         "FreeBSD"
#define DLCS_OS_WHICH          DLCS_OS_FREEBSD
#define PATH_SEP               '/'
#define DLCS_SYSTEM_NONWINDOWS "dlcs_nonwindows.h"

#include <malloc.h>

#endif  // _DLCS_SYS_FREEBSD
