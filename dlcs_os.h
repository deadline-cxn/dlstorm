/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_os.h
 **   Description:  Determine the OS and include OS specific stuff
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************

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
                            Must be included from <param.h>
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
                        (GNU/Hurd The official name of this operating system is GNU. Hurd is the kernel in the GNU operating system. It is often listed as GNU/Hurd since there is also GNU/Linux and GNU/kFreeBSD, which are most of the GNU operating system with the Linux and FreeBSD kernels
respectively.)

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
                    NetBSD'V'_'R'	(V = Version R = Revision  Must be included from <param.h>)
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
                    OpenBSD'V'_'R'	(V = Version R = Revision  Must be included from <param.h>)
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
                        Must be included from <neutrino.h>
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
***************************************************************/

#ifndef _DLCS_OS
#define _DLCS_OS

#define DLCS_OS_WINDOWS 1
#define DLCS_OS_LINUX   2
#define DLCS_OS_FREEBSD 3
#define DLCS_OS_MACOSX  4
#define DLCS_OS_MACOS   5
#define DLCS_OS_ANDROID 6

////////////////////////////////////////////////////////////////
// WINDOWS
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define DLCS_SYSTEM_H "dlcs_windows.h"
#endif

////////////////////////////////////////////////////////////////
// LINUX Defines
#ifdef __linux__
#define DLCS_SYSTEM_H "dlcs_linux.h"
#endif

////////////////////////////////////////////////////////////////
// MAC OS X Server defines
#if (defined(__MACH__) && defined(__APPLE__))
#define DLCS_SYSTEM_H "dlcs_macosx.h"
#endif

////////////////////////////////////////////////////////////////
// MAC Defines
#ifdef __MACOS__
#define DLCS_SYSTEM_H "dlcs_macos.h"
#endif

////////////////////////////////////////////////////////////////
// FREEBSD Defines
#ifdef FREEBSD_NATIVE
#define DLCS_SYSTEM_H "dlcs_freebsd.h"
#endif

////////////////////////////////////////////////////////////////
// ANDROID
#ifdef __ANDROID__
#define DLCS_SYSTEM_H "dlcs_android.h"
#endif

#include DLCS_SYSTEM_H

#endif  //_DLCS_OS
