/***************************************************************
	Deadline's Code Storm
 ***************************************************************/

#ifndef _DEADLINE_CODE_STORM_LIB
#define _DEADLINE_CODE_STORM_LIB

#ifdef _WIN32
/*
#pragma warning(disable : 4018)     // signed/unsigned mismatch
#pragma warning(disable : 4032)
#pragma warning(disable : 4051)
#pragma warning(disable : 4057)		// slightly different base types
#pragma warning(disable : 4100)		// unreferenced formal parameter
#pragma warning(disable : 4115)
#pragma warning(disable : 4125)		// decimal digit terminates octal escape sequence
#pragma warning(disable : 4127)		// conditional expression is constant
#pragma warning(disable : 4136)
#pragma warning(disable : 4201)
#pragma warning(disable : 4214)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)		// truncation from const double to float
#pragma warning(disable : 4310)		// cast truncates constant value
#pragma warning(disable : 4514)
#pragma warning(disable : 4711)		// selected for automatic inline expansion
#pragma warning(disable : 4220)		// varargs matches remaining parameters
#pragma warning(disable : 4786)
#pragma warning(disable : 4091)
#pragma warning(disable : 4006)     // second definition ignored
#pragma warning(disable : 4089)     // reference to library discarded because no calls were made
*/
//#pragma comment(lib,"winmm")
//#pragma comment(lib,"wsock32")
//#pragma comment(lib,"ws2_32")
//#pragma comment(lib,"errno")
//#pragma intrinsic( memset, memcpy )
#endif

/**********************************************

    C Preprocessor Directives for various OS

    Windows
                _WIN32
                __WIN64 // new
    Unix
                __unix
                unix
                _LP64
                __LP64__
    Linux
                __linux__
                __i386__    // Linux-i386
                __axp__     // Linux-alpha
    FreeBSD
                FREEBSD_NATIVE
    MAC
                __MACOS__
    SunOS 4.1.x
                _POSIX_SOURCE
    SunOS 5.x
                _POSIX_SOURCE
                _XOPEN_SOURCE
                __STDC__
    HP-UX 9.x
                _POSIX_SOURCE
                _XOPEN_SOURCE
                __STDC__
                _HPUX_SOURCE
                _AES_SOURCE
                _cplusplus
    GCC
                _GNUC_

 **********************************************/

////////////////////////////////////////////////////////////////////////////////////////
// Includes

#ifndef _WIN32
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <term.h>
#include <unistd.h>
#include <string>
#ifndef FREEBSD_NATIVE
#include <tcl.h>
#endif//FREEBSD_NATIVE
#include <curses.h>
#endif//_WIN32

#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#include <direct.h>
#include <conio.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32

#include <time.h>

#ifndef FREEBSD_NATIVE
#include <malloc.h>
#endif//FREEBSD_NATIVE

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////////
// WIN32 Defines

#ifdef WIN32
#define byte unsigned char
#define sbyte char
#define _word unsigned short
#define sword short
#define _dword unsigned long
#define sdword long
#define qword unsigned _int64
#define sqword _int64
#define dwbool unsigned long
#define bbool unsigned char

#define u_char unsigned char
#define u_short unsigned short
#define u_int unsigned int
#define u_long unsigned long

#define	MAC_STATIC
#undef QDECL
#define	QDECL	__cdecl
#ifdef NDEBUG
#ifdef _M_IX86
#define	CPUSTRING	"Windows-x86"
#elif defined _M_ALPHA
#define	CPUSTRING	"Windows-AXP"
#endif
#else
#ifdef _M_IX86
#define	CPUSTRING	"Windows-x86(D)"
#elif defined _M_ALPHA
#define	CPUSTRING	"Windows-AXP(D)"
#endif
#endif
#define	PATH_SEP '\\'
#endif

////////////////////////////////////////////////////////////////////////////////////
// MAC OS X Server defines

#if defined(__MACH__) && defined(__APPLE__)
#define MAC_STATIC
#ifdef __ppc__
#define CPUSTRING	"MacOSXS-ppc"
#elif defined __i386__
#define CPUSTRING	"MacOSXS-i386"
#else
#define CPUSTRING	"MacOSXS-other"
#endif
#define	PATH_SEP	'/'
#endif

////////////////////////////////////////////////////////////////////////////////////
// MAC Defines

#ifdef __MACOS__
#define	MAC_STATIC	static
#define	CPUSTRING	"MacOS-PPC"
#define	PATH_SEP ':'
void Sys_PumpEvents( void );
#endif

////////////////////////////////////////////////////////////////////////////////////
// LINUX Defines

#ifdef __linux__
#define	MAC_STATIC
#ifdef __i386__
#define	CPUSTRING	"Linux-i386"
#elif defined __axp__
#define	CPUSTRING	"Linux-alpha"
#else
#define	CPUSTRING	"Linux-other"
#endif
#define	PATH_SEP '/'
#endif

////////////////////////////////////////////////////////////////////////////////////
// FREEBSD Defines

#ifdef FREEBSD_NATIVE
#define	MAC_STATIC
#ifdef __i386__
#define	CPUSTRING	"FreeBSD-i386"
#elif defined __axp__
#define	CPUSTRING	"FreeBSD-alpha"
#else
#define	CPUSTRING	"FreeBSD-other"
#endif
#define	PATH_SEP '/'
#endif

////////////////////////////////////////////////////////////////////////////////////
// Windows type structs for non windows conversions

#ifndef _WIN32

typedef struct RECT { long top; long left; long bottom; long right; } rchiw;

#endif

////////////////////////////////////////////////////////////////////////////////////
// Other Defines

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

///////////////////////////////////////////////////////////////////////////////////////////
// Macros

//#define	MAKERGB(v,r,g,b)       v=r+(g<<8)+(b<<16)
//#define	MAKERGBA(v,r,g,b,a)    v[0]=r;v[1]=g;v[2]=b;v[3]=a
#define s_MAKEDWORD(a,b,c,d)     ((a)+(b<<8)+(c<<16)+(d<<24))
#define s_MAKEWORD(a,b)          ((a)+(b<<8))
#define PI                     3.145f
#define DEG2RAD(a)             (((a) * PI)/180.0F)
#define RAD2DEG(a)             (((a) * 180.0f)/PI)
#define SQUARE(number)         (number*number)
#define randomize()            srand((unsigned int) time(NULL))
#define crandom()	           (2.0 * (random() - 0.5))
#define dMIN(a,b)               ((a<b)?a:b)
#define dMAX(a,b)               ((a>b)?a:b)
#define TOBOOL(x)              (x?1:0)
#define REL(x)                 if(x!=NULL){x->Release();x=NULL;}
#define DEL(x)                 if(x!=NULL){delete x;x=NULL;}
#define MAL(x,y)               (x*)malloc(sizeof(x) * y);
#define FREE(x)                if(x!=NULL){free(x);x=NULL;}
#define CHECK_RANGE(x,min,max) ((x= (x<min  ? min : x<max ? x : max)))
#define MapCoord(x)            (x/GMP_MAPSIZE)
#define RealCoord(x,y)         ((x*GMP_MAPSIZE)+y)
#define CamCoord(x)            (x-(GMP_MAPSIZE*(x/GMP_MAPSIZE)))
#define LONGRGB(r,g,b)         ((long)(((u_char)(r)|((u_short)((u_char)(g))<<8))|(((long)(u_char)(b))<<16)))
#define _PS PATH_SEP
#define DLSTR(x)				char x[1024]; memset(x,0,1024);




#endif // _DEADLINE_CODE_STORM_LIB
