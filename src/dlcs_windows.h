/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_windwos.h
 **   Description:  Windows Standard Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_WINDOWS
#define _DLCS_WINDOWS

/////////////////////////////////////////////////////////////////
// Windows Defines and Includes

#ifdef _WIN32

#define DLCS_WINDOWS
#define DLCS_OS_STRING "Windows"
#define DLCS_OS_WHICH  DLCS_OS_WINDOWS
#define PATH_SEP       '\\'
#define MAC_STATIC

// #ifdef _M_IX86 #define DLCS_OS_STRING "Windows-x86" #elif defined _M_ALPHA #define DLCS_OS_STRING "Windows-AXP" #endif

/*
#pragma warning(disable : 4018)  // signed/unsigned mismatch
#pragma warning(disable : 4032)
#pragma warning(disable : 4051)
#pragma warning(disable : 4057)  // slightly different base types
#pragma warning(disable : 4100)  // unreferenced formal parameter
#pragma warning(disable : 4115)
#pragma warning(disable : 4125)  // decimal digit terminates octal escape sequence
#pragma warning(disable : 4127)  // conditional expression is constant
#pragma warning(disable : 4136)
#pragma warning(disable : 4201)
#pragma warning(disable : 4214)
#pragma warning(disable : 4090)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)  // truncation from const double to float
#pragma warning(disable : 4310)  // cast truncates constant value
#pragma warning(disable : 4514)
#pragma warning(disable : 4711)  // selected for automatic inline expansion
#pragma warning(disable : 4220)  // varargs matches remaining parameters
#pragma warning(disable : 4786)
#pragma warning(disable : 4091)
#pragma warning(disable : 4006)  // second definition ignored
#pragma warning(disable : 4089)  // reference to library discarded because no calls were made
#pragma comment(lib, "winmm")
#pragma comment(lib, "wsock32")
//#pragma intrinsic( memset, memcpy )
*/

#include <winsock2.h>
//

#include <windows.h>
//

#include <conio.h>
#include <direct.h>
#include <errno.h>
#include <io.h>
#include <malloc.h>
#include <tchar.h>
#include <winbase.h>
#include <ws2tcpip.h>

#include "dlcs_windows_dirent.h"

#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmreg.h>
#include <mmsystem.h>
#endif  //_MMEDIA

#endif  //_WIN32

#define PROT_READ   0x0001
#define PROT_WRITE  0x0002
#define PROT_EXEC   0x0004
#define PROT_NONE   0x0008
#define MAP_SHARED  0x0001
#define MAP_PRIVATE 0x0002
#define MAP_FIXED   0x0004
#define MAP_FAILED  ((void *)-1)

#if !defined(PRODUCT_ULTIMATE)
#define PRODUCT_UNDEFINED                    0x00000000
#define PRODUCT_ULTIMATE                     0x00000001
#define PRODUCT_HOME_BASIC                   0x00000002
#define PRODUCT_HOME_PREMIUM                 0x00000003
#define PRODUCT_ENTERPRISE                   0x00000004
#define PRODUCT_HOME_BASIC_N                 0x00000005
#define PRODUCT_BUSINESS                     0x00000006
#define PRODUCT_STANDARD_SERVER              0x00000007
#define PRODUCT_DATACENTER_SERVER            0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER         0x00000009
#define PRODUCT_ENTERPRISE_SERVER            0x0000000A
#define PRODUCT_STARTER                      0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE       0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE         0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE       0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64       0x0000000F
#define PRODUCT_BUSINESS_N                   0x00000010
#define PRODUCT_WEB_SERVER                   0x00000011
#define PRODUCT_CLUSTER_SERVER               0x00000012
#define PRODUCT_HOME_SERVER                  0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER       0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER      0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER     0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER    0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS     0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM 0x00000019
#define PRODUCT_UNLICENSED                   0xABCDABCD
#endif

/*
#define byte   unsigned char
#define sbyte  char
#define word   unsigned short
#define sword  short
#define dword  unsigned long
#define sdword long
#define qword  unsigned _int64
#define sqword _int64
#define dwbool unsigned long
#define bbool  unsigned char

#define u_char  unsigned char
#define u_short unsigned short
#define u_int   unsigned int
#define u_long  unsigned long
*/

#endif  // _DLCS_WINDOWS
