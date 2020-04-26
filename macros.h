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
 **   File:         macros.h
 **   Description:  Macros
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_MACROS
#define _DLCS_MACROS

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
#define dlcsm_malloc(x, y) (x*)malloc(sizeof(x) * y);
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
    char x[TINYNAME_SIZE];     \
    memset(x, 0, TINYNAME_SIZE);
#define dlcsm_make_filename(x) \
    char x[FILENAME_SIZE];     \
    memset(x, 0, FILENAME_SIZE);
#define dlcsm_make_str(x)  \
    char x[TEXTNAME_SIZE]; \
    memset(x, 0, TEXTNAME_SIZE);
#define dlcsm_make_lstr(x, y) \
    char x[y];                \
    memset(x, 0, y);
#define dlcsm_zero(x)  memset(x, 0, sizeof(x));
#define dlcsm_debug(x) printf(x);

#endif  // _DLCS_MACROS
