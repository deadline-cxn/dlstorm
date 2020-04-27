

#ifndef _DLCS_SYS_NONWINDOWS
#define _DLCS_SYS_NONWINDOWS

////////////////////////////////////////////////////////////////////////////////////
// Windows type structs for non windows conversions

#ifndef DLCS_WINDOWS
/*
#ifndef RECT
typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT;
#endif
*/
typedef long LONG;

#ifndef DWORD
typedef unsigned long DWORD;
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
#endif  // _DLCS_WINDOWS

#endif  // _DLCS_SYS_NONWINDOWS
