/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_nonwindows.h
 **   Description:  Non Windows Standard Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_SYS_NONWINDOWS
#define _DLCS_SYS_NONWINDOWS

/////////////////////////////////////////////////////////////////
// Non Windows Defines and Includes

#ifndef _WIN32

#include <arpa/inet.h>
#include <ctype.h>
#include <curses.h>  // apt-get install libncurses5-dev
#include <dirent.h>
#include <errno.h>
#include <ncurses.h>  // apt-get install libncurses5-dev
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <tcl.h>   // apt install tcl-dev
#include <term.h>  // apt install libncurses5-dev
#include <termios.h>
#include <unistd.h>

#ifndef FREEBSD_NATIVE
//#include <tcl.h>
#endif  // FREEBSD_NATIVE
//#include <curses.h>

typedef struct RECT {
    long top;
    long left;
    long bottom;
    long right;
} rchiw;

typedef long LONG;

#ifndef DWORD
typedef unsigned int DWORD;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef LPSTR
#define LPSTR char*
#endif

#ifndef LPCTSTR
#define LPCTSTR char*
#endif

#ifndef LPCSTR
#define LPCSTR const char*
#endif

typedef DWORD COLORREF;
#endif  // not _WIN32

#endif  // _DLCS_SYS_NONWINDOWS
