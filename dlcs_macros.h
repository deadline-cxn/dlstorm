/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_macros.h
 **   Description:  Macros
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_MACROS
#define _DLCS_MACROS

/////////////////////////////////////////////////////////////////
// Macros

//#define	MAKERGB(v,r,g,b)       v=r+(g<<8)+(b<<16)
//#define	MAKERGBA(v,r,g,b,a)    v[0]=r;v[1]=g;v[2]=b;v[3]=a
// #define zl(x)             \    if (Log) {            \        Log->AddEntry(x); \     }
#define s_MAKEDWORD(a, b, c, d) ((a) + (b << 8) + (c << 16) + (d << 24))
#define s_MAKEWORD(a, b)        ((a) + (b << 8))
#define PI                      3.145f
#define DEG2RAD(a)              (((a)*PI) / 180.0F)
#define RAD2DEG(a)              (((a)*180.0f) / PI)
#define SQUARE(number)          (number * number)
#define randomize()             srand((unsigned int)time(NULL))
#define crandom()               (2.0 * (random() - 0.5))
#define MIN(a, b)               ((a < b) ? a : b)
#define MAX(a, b)               ((a > b) ? a : b)
#define TOBOOL(x)               (x ? 1 : 0)
#define REL(x)        \
    if (x != NULL) {  \
        x->Release(); \
        x = NULL;     \
    }
#define DEL(x)       \
    if (x != NULL) { \
        delete x;    \
        x = NULL;    \
    }
#define MAL(x, y) (x *)malloc(sizeof(x) * y);
#define FREE(x)      \
    if (x != NULL) { \
        free(x);     \
        x = NULL;    \
    }
#define CHECK_RANGE(x, min, max) ((x = (x < min ? min : x < max ? x : max)))
#define MapCoord(x)              (x / GMP_MAPSIZE)
#define RealCoord(x, y)          ((x * GMP_MAPSIZE) + y)
#define CamCoord(x)              (x - (GMP_MAPSIZE * (x / GMP_MAPSIZE)))
#define LONGRGB(r, g, b)         ((long)(((u_char)(r) | ((u_short)((u_char)(g)) << 8)) | (((long)(u_char)(b)) << 16)))
#define _PS                      PATH_SEP
#define DLSTR(x)  \
    char x[1024]; \
    memset(x, 0, 1024);

// MISC MACROS
#define dlcsm_GetRValue(rgb)   ((BYTE)(rgb))
#define dlcsm_GetGValue(rgb)   ((BYTE)(((WORD)(rgb)) >> 8))
#define dlcsm_GetBValue(rgb)   ((BYTE)((rgb) >> 16))
#define dlcsm_RGB(r, g, b)     ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(b)) << 16)))
#define dlcsm_RGBA(v, r, g, b) v = r + (g << 8) + (b << 16)
#define dlcsm_VRGBA(v, r, g, b, a) \
    v[0] = r;                      \
    v[1] = g;                      \
    v[2] = b;                      \
    v[3] = a
// DATA CONVERSION MACROS
#define dlcsm_to_dword(a, b, c, d) ((a) + (b << 8) + (c << 16) + (d << 24))
#define dlcsm_to_word(a, b)        ((a) + (b << 8))
// MEMORY MACROS
#define dlcsm_release(x) \
    if (x) x->Release(); \
    x = 0;
#define dlcsm_delete(x) \
    if (x) {            \
        delete x;       \
        x = 0;          \
    }
#define dlcsm_delete_array(x) \
    if (x) delete[] x;        \
    x = 0;
#define dlcsm_malloc(x, y) (x *)malloc(sizeof(x) * y);
#define dlcsm_free(x) \
    if (x) free(x);   \
    x = 0;
#define dlcsm_delete_vector(x, y)                            \
    for (vector<x>::iterator i = y.begin(); i != y.end();) { \
        dlcsm_delete(*i);                                    \
        i = y.erase(i);                                      \
    }
#define dlcsm_gl_delete(x)                       \
    if (glIsTexture(x)) glDeleteTextures(1, &x); \
    x = 0;
#define dlcsm_LONGRGB(r, g, b) ((long)(((u_char)(r) | ((u_short)((u_char)(g)) << 8)) | (((long)(u_char)(b)) << 16)))
// MATH MACROS
#define dlcsm_pi                 3.14159265359f
#define dlcsm_deg_to_rad(a)      (((a)*dlcsm_pi) / 180.0F)
#define dlcsm_rad_to_deg(a)      (((a)*180.0f) / dlcsm_pi)
#define dlcsm_randomize()        srand((unsigned int)time(NULL))
#define dlcsm_crandom()          (2.0 * (random() - 0.5))
#define dlcsm_range(x, min, max) ((x = (x < min ? min : x < max ? x : max)))
#define dlcsm_sq(number)         (number * number)
#define dlcsm_min(a, b)          ((a < b) ? a : b)
#define dlcsm_max(a, b)          ((a > b) ? a : b)
#define dlcsm_tobool(x)          (x ? 1 : 0)
// STRING MACROS
#define dlcsm_make_tinyname(x) \
    char x[_TINYNAME_SIZE];     \
    memset(x, 0, _TINYNAME_SIZE);
#define dlcsm_make_filename(x) \
    char x[_FILENAME_SIZE];     \
    memset(x, 0, _FILENAME_SIZE);
#define dlcsm_make_str(x)  \
    char x[_TEXTNAME_SIZE]; \
    memset(x, 0, _TEXTNAME_SIZE);
#define dlcsm_make_lstr(x, y) \
    char x[y];                \
    memset(x, 0, y);
#define dlcsm_zero(x)  memset(x, 0, sizeof(x));
#define dlcsm_debug(x) printf(x);

#endif  // _DLCS_MACROS
