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
 **   File:         sys/windows.h
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_SYS_WINDOWS
#define _DLCS_SYS_WINDOWS
#define PATH_SEP '\\'
#include <tchar.h>
#include <winbase.h>
#include <windows.h>
#include <winsock2.h>
//#define _WIN32_WINNT 0x501
#include <conio.h>
#include <direct.h>
#include <ws2tcpip.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmreg.h>
#include <mmsystem.h>
#endif  //_MMEDIA

#define PROT_READ   0x0001
#define PROT_WRITE  0x0002
#define PROT_EXEC   0x0004
#define PROT_NONE   0x0008
#define MAP_SHARED  0x0001
#define MAP_PRIVATE 0x0002
#define MAP_FIXED   0x0004
#define MAP_FAILED  ((void *)-1)

#endif  // _DLCS_SYS_WINDOWS
