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
 **   File:         dlstorm.h
 **   Description:  DLCODESTORM Namespace, where the magic happens
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ******************************************************************************/
#ifndef DLCS_DEADLINE_CODE_STORM_LIB
#define DLCS_DEADLINE_CODE_STORM_LIB

#ifdef _DLCS_DEBUG
#define _DLCS_DEBUG_GUI 1
#define _DLCS_DEBUG_LOG
#define _DLCS_DEBUG_GFX 1
#define _DLCS_DEBUG_SND 1
#endif

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
                    __TOS_MVS__     (Target)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPU types definitions


Alpha
Type	Macro	Description
Identification	__alpha__	Defined by GNU C
Version	__alpha_ev'V'__	V = Version
Identification	__alpha	Defined by DEC C
Identification	_M_ALPHA	Defined by Visual Studio
Example
CPU	Macro
Alpha EV4	__alpha_ev4__
Alpha EV5	__alpha_ev5__
Alpha EV6	__alpha_ev6__
AMD64
Type	Macro	Description
Identification	__amd64__
__amd64
__x86_64__
__x86_64
Defined by GNU C and Sun Studio
Identification	_M_X64
_M_AMD64	Defined by Visual Studio
Notice that x32 can be detected by checking if the CPU uses the ILP32 data model.
ARM
Type	Macro	Description
Identification	__arm__	Defined by GNU C and RealView
Identification	__thumb__	Defined by GNU C and RealView in Thumb mode
Identification	__TARGET_ARCH_ARM
__TARGET_ARCH_THUMB	Defined by RealView
Version	__TARGET_ARCH_ARM = V
__TARGET_ARCH_THUMB = V	V = Version
Version	__TARGET_ARCH_'VR'	VR = Version and Revision
Identification	_ARM	Defined by ImageCraft C
Identification	_M_ARM	Defined by Visual Studio
Identification	_M_ARMT	Defined by Visual Studio in Thumb mode
Version	_M_ARM = V	V = Version
Example
CPU	Macro	_M_ARM
ARM 4T	__TARGET_ARM_4T
ARM 5		5
ARM 6		6
ARM 7		7
ARM64
Type	Macro	Description
Identification	__aarch64__	Defined by GNU C 1
Blackfin
Type	Macro	Description
Identification	__bfin
__BFIN__	Defined by GNU C
Convex
Type	Macro	Description
Identification	__convex__	Defined by GNU C
Version	__convex_'V'__	V = Version
Example
CPU	Macro
Convex C1	__convex_c1__
Convex C2	__convex_c2__
Convex C32xx series	__convex_c32__
Convex C34xx series	__convex_c34__
Convex C38xx series	__convex_c38__
Epiphany
Type	Macro
Identification	__epiphany__
HP/PA RISC
Type	Macro	Description
Identification	__hppa__	Defined by GNU C
Identification	__HPPA__	Defined by Stratus VOS C
Identification	__hppa
Version	_PA_RISC'V'_'R'	V = Version
R = Revision
See also OpenPA.net.
Example
CPU	Macro
PA RISC 1.0	_PA_RISC1_0
PA RISC 1.1	_PA_RISC1_1
__HPPA11__
__PA7100__
PA RISC 2.0	_PA_RISC2_0
__RISC2_0__
__HPPA20__
__PA8000__
Intel x86
Type	Macro	Format	Description
Identification	i386
__i386
__i386__		Defined by GNU C
Version	__i386__
__i486__
__i586__
__i686__		Defined by GNU C
Identification	__i386		Defined by Sun Studio
Identification	__i386
__IA32__		Defined by Stratus VOS C
Identification	_M_I86		Only defined for 16-bits architectures

Defined by Visual Studio, Digital Mars, and Watcom C/C++ (see note below)
Identification	_M_IX86		Only defined for 32-bits architectures

Defined by Visual Studio, Intel C/C++, Digital Mars, and Watcom C/C++
Version	_M_IX86	V00	V = Version
Identification	__X86__		Defined by Watcom C/C++
Identification	_X86_		Defined by MinGW32
Identification	__THW_INTEL__		Defined by XL C/C++
Identification	__I86__		Defined by Digital Mars
Version	__I86__	V	V = Version
Identification	__INTEL__		Defined by CodeWarrior
Notice that Watcom C/C++ defines _M_IX86 for both 16-bits and 32-bits architectures. Use __386__ or _M_I386 to detect 32-bits architectures in this case.
Notice that the Stratus VOS is big-endian on IA32, so these macros cannot be used to detect endianness if __VOS__ is set.
Example
CPU	_M_IX86	__I86__
80386	300	3
80486	400	4
Pentium	500	5
Pentium Pro/II	600	6
Intel Itanium (IA-64)
Type	Macro	Format	Description
Identification	__ia64__
_IA64
__IA64__		Defined by GNU C
Identification	__ia64		Defined by HP aCC
Identification	_M_IA64		Defined by Visual Studio
Identification	_M_IA64		Defined by Intel C/C++
Version	_M_IA64	?
Identification	__itanium__		Defined by Intel C/C++
Example
CPU	_M_IA64 (Intel C/C++)
64100
Motorola 68k
Type	Macro	Description
Identification	__m68k__	Defined by GNU C
Version	__mc'V'__
__mc'V'
mc'V'	V = Version
Identification	M68000	Defined by SAS/C
Identification	__MC68K__	Defined by Stratus VOS C
Version	__MC'V'__	V = Version
Example
CPU	Macro
68000	__mc68000__
__MC68000__
68010	__mc68010__
68020	__mc68020__
__MC68020__
68030	__mc68030__
__MC68030__
68040	__mc68040__
68060	__mc68060__
MIPS
Type	Macro	Description
Identification	__mips__
mips	Defined by GNU C
Version	_MIPS_ISA = _MIPS_ISA_MIPS'V'	V = MIPS ISA level
Version	_R3000
_R4000
_R5900
Identification	__mips	Defined by MIPSpro and GNU C
Version	__mips	The value indicates the MIPS ISA (Instruction Set Architecture) level
Version	__MIPS_ISA'V'__	V = MIPS ISA level
Identification	__MIPS__	Defined by Metrowerks
Example
CPU	_MIPS_ISA	GNU C Macro	__mips	MIPSpro Macro
R2000	_MIPS_ISA_MIPS1		1
R3000	_MIPS_ISA_MIPS1	_R3000	1
R6000	_MIPS_ISA_MIPS2		2	__MIPS_ISA2__
R4000		_R4000
R4400	_MIPS_ISA_MIPS3		3	__MIPS_ISA3__
R8000	_MIPS_ISA_MIPS4		4	__MIPS_ISA4__
R10000	_MIPS_ISA_MIPS4		4	__MIPS_ISA4__
PowerPC
Type	Macro	Description
Identification	__powerpc
__powerpc__
__powerpc64__
__POWERPC__
__ppc__
__ppc64__	Defined by GNU C
Version	__ppc'V'__	V = Version
Identification	_M_PPC	Defined by Visual Studio
Version	_M_PPC	?
Identification	_ARCH_PPC	Defined by XL C/C++
Version	_ARCH_'V'	V = Version
Version	__PPCGECKO__	Gekko

Defined by CodeWarrior
Version	__PPCBROADWAY__	Broadway

Defined by CodeWarrior
Version	_XENON	Xenon
Example
CPU	_M_PPC	Macro	XL Macro
PowerPC 440			_ARCH_440
PowerPC 450			_ARCH_450
PowerPC 601	601	__ppc601__	_ARCH_601
PowerPC 603	603	__ppc603__	_ARCH_603
PowerPC 604	604	__ppc604__	_ARCH_604
PowerPC 620	620
Pyramid 9810
Type	Macro
Identification	pyr
RS/6000
Type	Macro	Description
Identification	__THW_RS6000	Defined by XL C/C++
Identification	_IBMR2
Identification	_POWER
Identification	_ARCH_PWR
_ARCH_PWR2
_ARCH_PWR3
_ARCH_PWR4
SPARC
Type	Macro	Description
Identification	__sparc__	Defined by GNU C
Identification	__sparc	Defined by Sun Studio
Version	__sparcv8
__sparcv9	Defined by Sun Studio
Example
CPU	Macro
SPARC v8 (SuperSPARC)	__sparcv8
SPARC v9 (UltraSPARC)	__sparcv9
SuperH
Type	Macro	Description
Identification	__sh__	Defined by GNU C
Version	__sh1__
__sh2__
__sh3__
__SH3__
__SH4__
__SH5__
SystemZ
Type	Macro	Description
Identification	__370__
__THW_370__	Identifies System/370

Defined by XL C/C++
Identification	__s390__	Identifies System/390

Defined by GNU C
Identification	__s390x__	Identifies z/Architecture

Defined by GNU C
Identification	__zarch__	Identifies z/Architecture

Defined by clang
Identification	__SYSC_ZARCH__	Identifies z/Architecture

Defined by Systems/C
TMS320
Type	Macro	Description
Identification	_TMS320C2XX
__TMS320C2000__	C2000 series
Identification	_TMS320C5X
__TMS320C55X__	C5000 series
Identification	_TMS320C6X
__TMS320C6X__	C6000 series
Example
DSP	Macro
C28xx	_TMS320C28X
C54x	_TMS320C5XX
C55x	__TMS320C55X__
C6200	_TMS320C6200
C6400	_TMS320C6400
C6400+	_TMS320C6400_PLUS
C6600	_TMS320C6600
C6700	_TMS320C6700
C6700+	_TMS320C6700_PLUS
C6740	_TMS320C6740
TMS470
Type	Macro
Identification	__TMS470__












                     */


////////////////////////////////////////////////////////////////////////////////////
// WIN32 Defines
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
#define	PATH_SEP '\\'
#endif
////////////////////////////////////////////////////////////////////////////////////
// MAC OS X Server defines
#if defined(__MACH__) && defined(__APPLE__)
#define DLCSM_MACOSX
#define MAC_STATIC
#define	PATH_SEP	'/'
#endif
////////////////////////////////////////////////////////////////////////////////////
// MAC Defines
#ifdef __MACOS__
#define DLCSM_MACOS
#define	MAC_STATIC	static
#define	PATH_SEP ':'
void Sys_PumpEvents( void );
#endif
////////////////////////////////////////////////////////////////////////////////////
// LINUX Defines
#ifdef __linux__
#define DLCSM_LINUX
#define	MAC_STATIC
#define	PATH_SEP '/'
#endif
////////////////////////////////////////////////////////////////////////////////////
// FREEBSD Defines
#ifdef FREEBSD_NATIVE
#define DLCSM_FREEBSD
#define	MAC_STATIC
#define	PATH_SEP '/'
#endif
////////////////////////////////////////////////////////////////////////////////////
// Determine CPU type
#define CPUSTRING "UNKOWN PROCESSOR"
#ifdef __amd64
#define     CPUSTRING   "AMD64 (x86_64)"
#endif
#ifdef __x86_64__
#define   CPUSTRING   "AMD64 (x86_64)"
#endif

#ifdef __x86_64
#define     CPUSTRING   "AMD64 (x86_64)"
#endif
#ifdef _M_X64
#define       CPUSTRING   "AMD64 (x86_64)"
#endif
#ifdef _M_AMD64
#define     CPUSTRING   "AMD64 (x86_64)"
#endif
#ifdef __i386__
#define	    CPUSTRING   "Intel IA-32 (386)"
#endif
#ifdef i386
#define	    CPUSTRING	"Intel IA-32 (386)"
#endif
#ifdef __i386
#define	    CPUSTRING	"Intel IA-32 (386)"
#endif
#ifdef __i486__
#define	CPUSTRING	"Intel IA-32 (486)"
#endif
#ifdef __i586__
#define	CPUSTRING	"Intel IA-32 (586)"
#endif
#ifdef __i686__
#define	CPUSTRING	"Intel IA-32 (686)"
#endif
#ifdef __IA32__
#define	    CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef _M_I86
#define	    CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef _M_IX86
#define	    CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef __X86__
#define	    CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef _X86_
#define	    CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef __I86__
#define      CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef __INTEL__
#define	CPUSTRING	"Intel IA-32 (x86)"
#endif
#ifdef __ia64__
#define	    CPUSTRING	"Intel IA-64"
#endif
#ifdef _IA64
#define	    CPUSTRING	"Intel IA-64"
#endif
#ifdef __IA64__
#defin      CPUSTRING	"Intel IA-64"
#endif
#ifdef __arm__
#define      CPUSTRING   "ARM"
#endif
#ifdef __aarch64__
#define  CPUSTRING   "ARM64"
#endif
#ifdef __powerp
#define     CPUSTRING   "PowerPC"
#endif
#ifdef __powerpc__
#define  CPUSTRING   "PowerPC"
#endif
#ifdef __powerpc64__
#define CPUSTRING  "PowerPC"
#endif
#ifdef __POWERPC__
#define  CPUSTRING   "PowerPC"
#endif
#ifdef __ppc__
#define      CPUSTRING   "PowerPC"
#endif
#ifdef __ppc64__
#define    CPUSTRING   "PowerPC"
#endif
#ifdef __alpha__
#define	CPUSTRING	"Alpha"
#endif
#ifdef __alpha
#define	    CPUSTRING	"Alpha"
#endif
#ifdef _M_ALPHA
#define     CPUSTRING	"Alpha"
#endif
////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS
#ifdef DLCSM_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <tchar.h>
#include <winsock2.h>
#include <direct.h>
#include <conio.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//DLCSM_WINDOWS
////////////////////////////////////////////////////////////////////////////////////////
// LINUX
#ifdef DLCSM_LINUX
#include <sys/utsname.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <termios.h>
#include <term.h>
#include <unistd.h>
#include <tcl.h>
#include <curses.h>
#endif//_LINUX
////////////////////////////////////////////////////////////////////////////////////////
// FREEBSD
#ifndef DLCSM_FREEBSD
#include <malloc.h>
#endif//FREEBSD_NATIVE
////////////////////////////////////////////////////////////////////////////////////////
// UNIVERSAL
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
#include "md5.h"
#include "SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "glerrors.h"
#define ILUT_USE_OPENGL
#include <il.h>
#include <ilu.h>
#include <ilut.h>
using namespace std;
////////////////////////////////////////////////////////////////////////////////////
// Windows type structs for non windows conversions
#ifndef DLCSM_WINDOWS
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
#define LPCSTR const char *
#endif

typedef DWORD          COLORREF;
#endif // _DLCSM_WINDOWS
////////////////////////////////////////////////////////////////////////////////////
// System independant
// GFX Stuff
typedef struct { float x, y; } CVector2;
typedef struct { float x, y, z; } CVector3 ;
typedef struct { float r, g, b; } CColor3;
typedef struct { float u,v; } CTexCoord;
typedef struct { int vertIndex[3]; int coordIndex[3]; } tFace;
typedef struct { float x, y, z; float u, v; } VERTEX;
typedef struct { VERTEX  vertex[3]; } TRIANGLE;
typedef struct { int numtriangles; TRIANGLE* triangle; } SECTOR;
// SIZE
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif
#define TINYNAME_SIZE 32
#define TEXTNAME_SIZE 256
#define FILENAME_SIZE 1024
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
#define dlcsm_release(x)        if(x) x->Release(); x=0;
#define dlcsm_delete(x)         if(x) { if(pLog) pLog->_DebugAdd("DELETED POINTER X[%d]",x); delete x; x=0; }
#define dlcsm_delete_array(x)   if(x) delete [] x; x=0;
#define dlcsm_malloc(x,y)       (x*) malloc(sizeof(x) * y);
#define dlcsm_free(x)           if(x) free(x); x=0;
#define dlcsm_delete_vector(x,y) for(vector<x>::iterator i=y.begin();i!=y.end(); ){ dlcsm_delete(*i);i=y.erase(i); pLog->_Add("Deleting vector [%d]",y.size()); }
#define dlcsm_gl_delete(x)      if(glIsTexture(x)) glDeleteTextures(1,&x); x=0;
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
// #define dlcsm_zero(x)           memset(x,0,sizeof(x));
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
namespace DLCODESTORM {
const char *va(const char *format, ...);
vector<string>  dlcs_explode(const string &delimiter, const string &explodeme);
vector<string>  dlcs_dir_to_vector(char *szDir, char *szWildCard);
bool    dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard);
string  dlcs_md5_digest(string str);
string  dlcs_encrypt(string text);
string  dlcs_decrypt(string text);
bool    dlcs_istrue(string text);
void    dlcs_suspend_power_management(void);
int     dlcs_hex_to_dec(char *pa);
int     dlcs_bin_to_dec(char *pa);
long    dlcs_get_tickcount(void);
string  dlcs_get_os_version(void);
string  dlcs_get_hostname(void);
string  dlcs_get_ipaddress(void);
bool    dlcs_isdir(char *dir);
string  dlcs_filetype(string pathName);
int     dlcs_mkdir(char *szDirectoryName);
int     dlcs_chdir(char *szDirectory);
string  dlcs_getcwd(void);
int     dlcs_strcasecmp(const char* szOne,const char* szTwo);
// int     dlcs_str2mem(char *string, char &mem);
char*   dlcs_trim_lf(char* x);
char*   dlcs_charreplace(char* str, char cold,char cnew);
char*   dlcs_strreplace(char* str, const char* what, const char* to);
char*   dlcs_get_time(char* x);
char*   dlcs_convert_time(char *x,struct tm*);
char*   dlcs_timestamp(char* x);
char*   dlcs_readable_timestamp(char* x,char* in);
char*   dlcs_get_filetype(char* x,char* in);
};
using namespace DLCODESTORM;

#endif // DLCS_DEADLINE_CODE_STORM_LIB

