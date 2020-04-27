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
 **   File:         pre/cpu.h
 **   Description:  Determine the CPU type
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_PRE_CPU
#define _DLCS_PRE_CPU

////////////////////////////////////////////////////////////////////////////////////
// Determine CPU type
#if defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define CPUSTRING "AMD64 (x86_64)"
#endif
#if defined(__i386__) || defined(i386) || defined(__i386)
#define CPUSTRING "Intel IA-32 (386)"
#endif
#ifdef __i486__
#define CPUSTRING "Intel IA-32 (486)"
#endif
#ifdef __i586__
#define CPUSTRING "Intel IA-32 (586)"
#endif
#ifdef __i686__
#define CPUSTRING "Intel IA-32 (686)"
#endif
#if defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__I86__) || defined(__INTEL__)
#define CPUSTRING "Intel IA-32 (x86)"
#endif
#if defined(__ia64__) || defined(_IA64) || defined(__IA64__)
#define CPUSTRING "Intel IA-64"
#endif
#ifdef __arm__
#define CPUSTRING "ARM"
#endif
#ifdef __aarch64__
#define CPUSTRING "ARM64"
#endif
#ifdef __powerp
#define CPUSTRING "PowerPC"
#endif
#ifdef __powerpc__
#define CPUSTRING "PowerPC"
#endif
#ifdef __powerpc64__
#define CPUSTRING "PowerPC"
#endif
#ifdef __POWERPC__
#define CPUSTRING "PowerPC"
#endif
#ifdef __ppc__
#define CPUSTRING "PowerPC"
#endif
#ifdef __ppc64__
#define CPUSTRING "PowerPC"
#endif
#ifdef __alpha__
#define CPUSTRING "Alpha"
#endif
#ifdef __alpha
#define CPUSTRING "Alpha"
#endif
#ifdef _M_ALPHA
#define CPUSTRING "Alpha"
#endif

#ifndef CPUSTRING
#define CPUSTRING "Unknown processor"
#endif

#endif  // _DLCS_PRE_CPU

/*


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
Identification	__TMS470__             */
