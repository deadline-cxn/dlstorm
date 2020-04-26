/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_gfx_2d.h
 **   Class:        C_GFX2D
 **   Description:  SDL / OpenGL class wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 **   Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL
 **
 ***************************************************************/
#ifndef _DLCS_C_GFX2D
#define _DLCS_C_GFX2D

#ifdef _DLCS_SDL

#include "dlstorm.h"
// #include "c_gaf.h"
#include "c_log.h"
//#include "il.h"
//#include "ilut.h"

class C_GFX2D {
   public:
    C_GFX2D(int w, int h, int c, bool FullScreen, string wincaption, CLog *pUSELOG);
    ~C_GFX2D();
    SDL_Surface *pScreen;
    string       windowcaption;
    bool         bCreatedLog;
    CLog *       pLog;
    SDL_Rect **  VideoModes;
    bool         bSDLFailed;
    int          VideoFlags;
    bool         bFullScreen;
    int          ScreenWidth;
    int          ScreenHeight;
    int          ScreenColors;
    void         SetWindowTitle(string fmt, ...);
    void         ShutDownGFX(void);
    bool         InitializeGFX(int w, int h, int c, bool FullScreen, string wincaption, CLog *pUSELOG);
    void         Flip(void);
    u_char       GetFade(char cWhichFade);
    /*
    void        DrawVertice(int x, int y);
    void        DrawBar(RECT rc,long color);
    void        DrawBar(int iX,int iY,int iX2,int iY2,long color);
    void        DrawBar(RECT rc,long color1,long color2);
    void        DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void        DrawRect(RECT rc, long color);
    void        DrawRect(int iX,int iY,int iX2,int iY2,long color);
    void        DrawRectangle(int iX,int iY,int iX2,int iY2,long color);
    void        DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void        Draw3DBox(RECT rect);
    void        Draw3DBox(int x, int y, int x2, int y2);
     */
};
#endif  // _DLCS_SDL
#endif  // _DLCS_C_GAF
