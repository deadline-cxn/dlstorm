/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2013
 **   File:         pre/os.h
 **   Description:  Determine the operating system
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_PRE_OS
#define _DLCS_PRE_OS
#define DLCS_OS_WINDOWS 1
#define DLCS_OS_LINUX 2
#define DLCS_OS_FREEBSD 3
#define DLCS_OS_MACOSX 4
#define DLCS_OS_MACOS 5
#define DLCS_OS_ANDROID 6
#define DLCS_SYSTEM_NONWINDOWS "sys/nonwindows.h"
// WINDOWS
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define DLCS_WINDOWS
#define DLCS_SYSTEM_H "sys/windows.h"
#undef  DLCS_SYSTEM_NONWINDOWS
#define DLCS_OS_WHICH DLCS_OS_WINDOWS
#endif
// MAC OS X Server defines
#if defined(__MACH__) && defined(__APPLE__)
#define DLCS_MACOSX
#define DLCS_SYSTEM_H "sys/macosx.h"
#define DLCS_OS_WHICH DLCS_OS_MACOSX
#endif
// MAC Defines
#ifdef __MACOS__
#define DLCS_MACOS
#define DLCS_SYSTEM_H "sys/macos.h"
#define DLCS_OS_WHICH DLCS_OS_MACOS
#endif
// LINUX Defines
#ifdef __linux__
#define DLCS_LINUX
#define DLCS_SYSTEM_H "sys/linux.h"
#define DLCS_OS_WHICH DLCS_OS_LINUX
#endif
// FREEBSD Defines
#ifdef FREEBSD_NATIVE
#define DLCS_FREEBSD
#define DLCS_SYSTEM_H "sys/freebsd.h"
#define DLCS_OS_WHICH DLCS_OS_FREEBSD
#endif
// ANDROID
#ifdef __ANDROID__
#define DLCS_ANDROID
#define DLCS_SYSTEM_H "sys/android.h"
#define DLCS_OS_WHICH DLCS_OS_ANDROID
#endif

#include DLCS_SYSTEM_H

#ifdef DLCS_SYSTEM_NONWINDOWS
#include DLCS_SYSTEM_NONWINDOWS
#endif

////////////////////////////////////////////////////////////////////////////////////////
// STANDARD INCLUDES

#include <dirent.h>
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
#include <fstream>
#include <iomanip>
#include "md5.h"

////////////////////////////////////////////////////////////////////////////////////
// System independant

typedef struct { float x, y, z; }               dlcs_V3;
typedef struct { float r, g, b; }               dlcs_C3;
typedef struct { float u,v; }                   dlcs_TC;

#ifndef _DLCS_CONSOLE
#endif // _DLCS_CONSOLE

#ifdef _DLCS_SDL
#include "SDL.h"
#endif // _DLCS_SDL

#ifdef _DLCS_OPENGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "glerrors.h"
#endif // _DLCS_OPENGL

#ifdef _DLCS_DEVIL
#define ILUT_USE_OPENGL
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#endif // ILUT_USE_OPENGL

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif
#define TINYNAME_SIZE 32
#define TEXTNAME_SIZE 256
#define FILENAME_SIZE 1024

#include "macros.h"

#if !defined(PRODUCT_ULTIMATE)
#define PRODUCT_UNDEFINED                       0x00000000
#define PRODUCT_ULTIMATE                        0x00000001
#define PRODUCT_HOME_BASIC                      0x00000002
#define PRODUCT_HOME_PREMIUM                    0x00000003
#define PRODUCT_ENTERPRISE                      0x00000004
#define PRODUCT_HOME_BASIC_N                    0x00000005
#define PRODUCT_BUSINESS                        0x00000006
#define PRODUCT_STANDARD_SERVER                 0x00000007
#define PRODUCT_DATACENTER_SERVER               0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER            0x00000009
#define PRODUCT_ENTERPRISE_SERVER               0x0000000A
#define PRODUCT_STARTER                         0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE          0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE            0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE          0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64          0x0000000F
#define PRODUCT_BUSINESS_N                      0x00000010
#define PRODUCT_WEB_SERVER                      0x00000011
#define PRODUCT_CLUSTER_SERVER                  0x00000012
#define PRODUCT_HOME_SERVER                     0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER          0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER         0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER        0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER       0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS        0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM    0x00000019
#define PRODUCT_UNLICENSED                      0xABCDABCD
#endif

#endif//_DLCS_PRE_OS


/**********************************************
    C Preprocessor Directives for various OS

    System:         Directive:

    AIX             _AIX
                    _AIX'VR'	V = Version R = Revision
                    __TOS_AIX__	Defined by xlC


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
                    __TOS_MVS__     (Target)

 */
