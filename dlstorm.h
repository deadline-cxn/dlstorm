/***************************************************************
    File:       DLSTORM Deadline's Code Storm library header file
    Author:     Seth Parson
    Twitter:    @Sethcoder
    Website:    www.sethcoder.com
    Email:      defectiveseth@gmail.com

 ***************************************************************/
#ifndef DLCS_DEADLINE_CODE_STORM_LIB
#define DLCS_DEADLINE_CODE_STORM_LIB
/**********************************************
    C Preprocessor Directives for various OS

    System:         Directive:

    AIX             _AIX
                    _AIX'VR'	V = Version R = Revision
                    __TOS_AIX__	Defined by xlC

    Android         __ANDROID__
                    __ANDROID_API__	V	V = API Version

                        Must be included from <android/api-level.h>
                        Notice that Android is based on Linux, and that the Linux macros also are defined for Android.
    Amdahl UTS      UTS
    AmigaOS         AMIGA
                    __amigaos__
    Apollo AEGIS    aegis
   Apollo Domain/OS apollo
    BeOS            __BEOS__
    Blue Gene       __bg__
    BSD Environment __bsdi__
                    __FreeBSD__
                    __NetBSD__
                    __OpenBSD__
                        BSD	YYYYMM	YYYY = Year MM = Month (Version)
                            Must be included from <sys/param.h>
                            Examples:
                            Version	BSD4_2
                            BSD4_3
                            BSD4_4
                    _SYSTYPE_BSD		Defined by DEC C
    ConvexOS        __convex__
    Cygwin          __CYGWIN__
    DG/UX           DGUX
                    __DGUX__
                    __dgux__
    DragonFly       __DragonFly__
    DYNIX/ptx       _SEQUENT_
                    sequent
    eCos            __ECOS
    EMX Environment __EMX__
    FreeBSD         __FreeBSD__
                    __FreeBSD_kernel__
                    __FreeBSD__	V	V = Version
                    __FreeBSD_version
                        Must be included from <osreldate.h>
                        Examples
                        FreeBSD	__FreeBSD__	__FreeBSD_version
                        1.x	1
                        2.0-RELEASE	2	119411
                        2.2-RELEASE	2	220000
                        3.0-RELEASE	3	300005
                        4.0-RELEASE	4	400017
                        4.5-RELEASE	4	450000
                        For more information see the FreeBSD porters handbook.
    GNU             __GNU__
                    __gnu_hurd__
                        (GNU/Hurd The official name of this operating system is GNU. Hurd is the kernel in the GNU operating system. It is often listed as GNU/Hurd since there is also GNU/Linux and GNU/kFreeBSD, which are most of the GNU operating system with the Linux and FreeBSD kernels respectively.)

    GNU/kFreeBSD    __FreeBSD_kernel__ && __GLIBC__
                        (one of the Debian distros that is based on the FreeBSD kernel rather than the Linux or Hurd kernels)
                        Notice that FreeBSD also defines __FreeBSD_kernel__ so the __GLIBC__ macro must be checked to distinguish it.
    GNU/Linux       __gnu_linux__
    HI-UX MPP       __hiuxmpp
    HP-UX           _hpux
                    hpux
                    __hpux
    IBM OS/400      __OS400__
    INTEGRITY       __INTEGRITY
    Interix         __INTERIX
    IRIX            sgi
                    __sgi
    Linux kernel    __linux__
                    linux	Obsolete (not POSIX compliant)
                    __linux	Obsolete (not POSIX compliant)
                        (Systems based on the Linux kernel define these macros. There are two major Linux-based operating systems: GNU/Linux and Android, and numerous others like Ångström or OpenEmbedded)
    LynxOS          __Lynx__
    MacOS           macintosh
                    Macintosh
                    __APPLE__ && __MACH__
    Microware OS-9  __OS9000
                    _OSK
    MINIX           __minix
    MorphOS         __MORPHOS__
    MPE/iX          mpeix
                    __mpexl
    MSDOS           MSDOS
                    __MSDOS__
                    _MSDOS
                    __DOS__
    NetBSD          __NetBSD__
                    BSD
                    NetBSD'V'_'R'	(V = Version R = Revision  Must be included from <sys/param.h>)
                    __NetBSD_Version__	VVRRAAPP00
                        VV = Version
                        RR = Revision
                        AA = Release
                        PP = Patch
                    __NetBSD_Version__	VVRR00PP00
                        VV = Version
                        RR = Revision
                        PP = Patch
                    __NetBSD_Version__	Macro
                                0.8		NetBSD0_8
                                0.9		NetBSD0_9
                                1.0		NetBSD1_0 = 1
                                1.0A	NetBSD1_0 = 2
                                1.2D	102040000
                                1.2.1	102000100
    NonStop         __TANDEM
    Nucleus RTOS    __nucleus__
    OpenBSD         __OpenBSD__
                    OpenBSD'V'_'R'	(V = Version R = Revision  Must be included from <sys/param.h>)
    OS/2            OS2
                    _OS2
                    __OS2__
                    __TOS_OS2__
    Palm OS         __palmos__
    Pyramid DC/OSx  pyr
    QNX             __QNX__		    (QNX 4.x)
                    __QNXNTO__	    (QNX 6.x)
                    _NTO_VERSION	VRR	V = Version RR = Revision
                        Only available when __QNXNTO__ is defined
                        Must be included from <sys/neutrino.h>
                        Example
                        QNX	_NTO_VERSION
                        6.2	620
    Reliant UNIX    sinux
    SCO OpenServer  M_I386
                    M_XENIX
                    _SCO_DS
    Solaris         sun
                    __sun
                    __'System'_'Version'
                        System = uname -s
                        Version = uname -r
                        Any illegal character is replaced by an underscore.
                        Defined by Sun Studio
                        Use the SVR4 macros to distinguish between Solaris and SunOS.
                        #if defined(sun) || defined(__sun)
                        # if defined(__SVR4) || defined(__svr4__)
                        // Solaris
                        # else
                        // SunOS
                        # endif
                        #endif
                            Example
                            Solaris	Macro
                            2.7	__SunOS_5_7
                            8	__SunOS_5_8
    Stratus VOS     __VOS__
                    __VOS__	V	V = Version Notice that the __VOS__ macro is defined by the compiler, but as several compilers can co-exist in the same OS release, the version number is not reliable.
    SVR4            __sysv__
                    __SVR4
                    __svr4__
                    _SYSTYPE_SVR4	Defined on IRIX
    Syllable        __SYLLABLE__
    Symbian OS      __SYMBIAN32__
    Tru64 (OSF/1)   __osf__
                    __osf
    Ultrix          ultrix
                    __ultrix
                    __ultrix__
                    unix & vax
    UNICOS          _UNICOS
                    _UNICOS	V	V = Version
    UNICOS/mp       _CRAY
                    __crayx1
    UNIX            __unix__
                    __unix
    UnixWare        sco
                    _UNIXWARE7
    U/Win           _UWIN
    VMS             VMS
                    __VMS
                    __VMS_VER	VVRREPPTT
                        VV = Version RR = Revision E = Edit number PP = Patch (01 = A, ... 26 = Z) TT = Type (22 = official)
                        Example
                        VMS	__VMS_VER
                        6.1	60100022
                        6.2	60200022
                        6.2-1I	60210922
    Windows         _WIN16	           (Defined for 16-bit environments)
                    _WIN32	           (Defined for both 32-bit and 64-bit environments)
                    _WIN64	           (Defined for 64-bit environments)
                    __WIN32__	        (Defined by Borland C++)
                    __TOS_WIN__	        (Defined by xlC)
                    __WINDOWS__	        (Defined by Watcom C/C++)
    Windows CE      _WIN32_WCE		    (Defined by Embedded Visual Studio C++)
                    _WIN32_WCE	VRR	V = Version R = Revision
                                WIN32_PLATFORM_'P'		P = Platform
                                WIN32_PLATFORM_'P'	V	P = Platform V = Version
                                    Example
                                    Version	_WIN32_WCE
                                    2.01	201
                                    2.11	211
                                    3.0	300
                                    4.0	400
                                    4.1	410
                                    4.2	420
                                    5.0	501
                                Platforms
                                    H/PC 2000	WIN32_PLATFORM_HPC2000
                                    H/PC Pro 2.11	WIN32_PLATFORM_HPCPRO	211
                                    H/PC Pro 3.0	WIN32_PLATFORM_HPCPRO	300
                                    Pocket PC	WIN32_PLATFORM_PSPC	1
                                    Pocket PC 2002	WIN32_PLATFORM_PSPC	310
                                    Windows Mobile 2003	WIN32_PLATFORM_PSPC	400
                                    Smartphone 2002	WIN32_PLATFORM_WFSP	100
    Wind/U          _WINDU_SOURCE
                    _WINDU_SOURCE	0xVVRRPP
                                    VV = Version
                                    RR = Revision
                                    PP = Patch
                                        Example
                                        Wind/U	_WINDU_SOURCE
                                        3.1.2	0x030102
    z/OS            __MVS__         (Host)
                    __HOS_MVS__	    (Host)
                    __TOS_MVS__     (Target)                                */

////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS
#ifdef _WIN16
#define DLCSM_WINDOWS
#endif
#ifdef _WIN32
#define DLCSM_WINDOWS
#endif
#ifdef _WIN64
#define DLCSM_WINDOWS
#endif
#ifdef DLCSM_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <io.h>
#include <direct.h>
#include <conio.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//DLCSM_WINDOWS
////////////////////////////////////////////////////////////////////////////////////////
// NON WINDOWS
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
////////////////////////////////////////////////////////////////////////////////////////
// FREEBSD
#ifndef FREEBSD_NATIVE
#include <malloc.h>
#endif//FREEBSD_NATIVE
////////////////////////////////////////////////////////////////////////////////////////
// ALL
#include <time.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "../SDK/md5/md5.h"
using namespace std;
////////////////////////////////////////////////////////////////////////////////////
// WIN32 Defines
#ifdef DLCSM_WINDOWS
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
#ifndef RECT
typedef struct tagRECT {
    long    left;
    long    top;
    long    right;
    long    bottom;
} RECT;
#endif

typedef long LONG;

#ifndef DWORD
typedef unsigned long  DWORD;
#endif

#ifndef BYTE
typedef unsigned char  BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef UINT
typedef unsigned int   UINT;
#endif

#ifndef LPSTR
#define LPSTR char *
#endif

#ifndef LPCTSTR
#define LPCTSTR char *
#endif

#ifndef LPCSTR
#define LPCSTR CONST char *
#endif

typedef DWORD          COLORREF;
#endif // _WIN32

////////////////////////////////////////////////////////////////////////////////////
// System independant defines

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif
#define TINYNAME_SIZE 32
#define TEXTNAME_SIZE 256
#define FILENAME_SIZE 1024
// SHORTCUTS
#define vecstr                  vector <string>
#define vecint                  vector <int>
// MISC MACROS
#define GetRValue(rgb)          ((BYTE)(rgb))
#define GetGValue(rgb)          ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)          ((BYTE)((rgb)>>16))
#define RGB(r,g,b)              ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define	RGBA(v,r,g,b)           v=r+(g<<8)+(b<<16)
#define	VRGBA(v,r,g,b,a)        v[0]=r;v[1]=g;v[2]=b;v[3]=a
// DATA CONVERSION MACROS
#define dlcsm_to_dword(a,b,c,d) ((a)+(b<<8)+(c<<16)+(d<<24))
#define dlcsm_to_word(a,b)      ((a)+(b<<8))
// MEMORY MACROS
#define dlcsm_release(x)        if(x) { x->Release(); x=NULL; }
#define dlcsm_delete(x)         if(x) { delete x; x=NULL;}
#define dlcsm_delete_array(x)   if(x) { delete [] x; x=NULL; }
#define dlcsm_malloc(x,y)       (x*) malloc(sizeof(x) * y);
#define dlcsm_free(x)           if(x) { free(x); x=NULL; }
// GFX MACROS
#define dlcsm_gl_delete(x)      if(glIsTexture(x)) { glDeleteTextures(1,&x); x=0; }
#define LONGRGB(r,g,b)          ((long)(((u_char)(r)|((u_short)((u_char)(g))<<8))|(((long)(u_char)(b))<<16)))
// MATH MACROS
#define dlcsm_pi                3.14159265359f
#define dlcsm_deg_to_rad(a)     (((a) * dlcsm_pi)/180.0F)
#define dlcsm_rad_to_deg(a)     (((a) * 180.0f)/dlcsm_pi)
#define dlcsm_randomize()       srand((unsigned int) time(NULL))
#define dlcsm_crandom()	        (2.0 * (random() - 0.5))
#define dlcsm_range(x,min,max)  ((x= (x<min  ? min : x<max ? x : max)))
#define dlcsm_sq(number)        (number*number)
#define dlcsm_min(a,b)          ((a<b)?a:b)
#define dlcsm_max(a,b)          ((a>b)?a:b)
#define dlcsm_tobool(x)         (x?1:0)
// STRING MACROS
#define dlcsm_make_tinyname(x)  char x[TINYNAME_SIZE]; memset(x,0,TINYNAME_SIZE);
#define dlcsm_make_filename(x)  char x[FILENAME_SIZE]; memset(x,0,FILENAME_SIZE);
#define dlcsm_make_str(x)       char x[TEXTNAME_SIZE]; memset(x,0,TEXTNAME_SIZE);
#define dlcsm_make_lstr(x,y)    char x[y]; memset(x,0,y);
#define dlcsm_zero(x)           memset(x,0,sizeof(x));
////////////////////////////////////////////////////////////////////////////////////////
// Do a log if defined
#ifdef _DL_INCLUDE_LOG
#include "c_log.h"
class CLog;
extern CLog* p_Log;
extern "C" void dLog(const char *format, ...);
#endif
////////////////////////////////////////////////////////////////////////////////////////
// DL Code Storm Library functions and definitions
extern char _vx[1024];
extern "C" const char *va(const char *format, ...);
#ifdef __cplusplus
extern "C" {
#endif
vecstr  dlcs_explode(const string &delimiter, const string &explodeme);
vecstr  dlcs_dir_to_vector(char *szDir, char *szWildCard);
bool    dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard);
void    dlcs_md5_digest(char *str, char *text); // md5 digest of the text
char*   dlcs_encrypt(char *text);
char*   dlcs_decrypt(char *text);
bool    dlcs_istrue(char *text);
void    dlcs_suspend_power_management(void);
int     dlcs_hex_to_dec(char *pa);
int     dlcs_bin_to_dec(char *pa);
long    dlcs_get_tickcount(void);
char*   dlcs_get_os_version(char *x);
char*   dlcs_get_hostname(char *x);
char*   dlcs_get_ipaddress(char *x);
bool    dlcs_isdir(char *dir);
int     dlcs_mkdir(char *szDirectoryName);
int     dlcs_chdir(char *szDirectory);
char*   dlcs_getcwd(char *x);
int     dlcs_strcasecmp(const char *szOne,const char *szTwo);
int     dlcs_str2mem(char *string, char &mem);
char*   dlcs_trim_lf(char *x);
char*   dlcs_charreplace(char *str, char cold,char cnew);
char*   dlcs_strreplace(char *str, const char* what, const char* to);
char*   dlcs_get_time(char *x);
char*   dlcs_convert_time(char *x,struct tm*);
char*   dlcs_timestamp(char *x);
char*   dlcs_readable_timestamp(char *x,char *in);
char*   dlcs_get_filetype(char*x,char*in);
#ifdef __cplusplus
}
#endif

#endif // DLCS_DEADLINE_CODE_STORM_LIB
