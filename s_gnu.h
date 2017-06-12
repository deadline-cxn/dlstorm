/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2017
 **   File:         gnu.h
 **   Description:  GNU compatibility
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef DLCS_S_GNU_H
#define DLCS_S_GNU_H
#ifndef DLCS_WINDOWS
#include "dlstorm.h"
typedef unsigned char byte;
extern "C" long GetTickCount(void);
extern "C" void Sleep(int ms);
extern "C" int _kbhit(void);
extern "C" int _getch(void);
extern "C" void close_keyboard(void);
#endif // DLCS_WINDOWS
#endif // DLCS_S_GNU_H
