/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_linux.h
 **   Description:  Linux Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_SYS_LINUX
#define _DLCS_SYS_LINUX

/////////////////////////////////////////////////////////////////
// LINUX Defines and Includes

#ifdef __linux__

#define DLCS_LINUX
#define DLCS_OS_STRING         "Linux"
#define DLCS_OS_WHICH          DLCS_OS_LINUX
#define PATH_SEP               '/'
#define DLCS_SYSTEM_NONWINDOWS "dlcs_nonwindows.h"

// #define MAC_STATIC
// #define _MAX_PATH 1024

#ifdef DEL
#undef DEL
#define DEL delete

extern void ConsoleSetup(void);
extern void ConsoleShutDown(void);
extern void LoadPlugIns(void);
extern void UnLoadPlugIns(void);

#endif

#endif  // _DLCS_SYS_LINUX
