/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_gfx_2d.cpp
 **   Class:        C_GFX2D
 **   Description:  SDL
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 **   Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL
 **
 ***************************************************************/
#include "c_gfx_2d.h"
//////////////////////////////////////////////////////////////// C_GFX2D CLASS CONSTRUCTOR / DESTRUCTOR
C_GFX2D::C_GFX2D(int w, int h, int c, bool FullScreen, string wincaption, CLog *pUSELOG) {
    if (!InitializeGFX(w, h, c, FullScreen, wincaption, pUSELOG)) {
        ShutDownGFX();
        SDL_Quit();
        pLog->_Add("SDL shut down (INIT)...");
    }
}
C_GFX2D::~C_GFX2D() {
    ShutDownGFX();
    SDL_Quit();
    pLog->_Add("SDL shut down...");
}
//////////////////////////////////////////////////////////////// GFX SYSTEM FUNCTIONS
bool C_GFX2D::InitializeGFX(int w, int h, int c, bool FullScreen, string wincaption, CLog *pUSELOG) {
    pLog = pUSELOG;
    pLog->_Add("Init SDL GFX Subsystem...");
    bSDLFailed = false;
#ifdef __linux__
    putenv("SDL_VIDEODRIVER=x11");
#endif
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    bCreatedLog  = false;
    bFullScreen  = FullScreen;
    ScreenWidth  = w;
    ScreenHeight = h;
    ScreenColors = c;
    VideoFlags   = SDL_HWPALETTE | SDL_DOUBLEBUF;
    if (bFullScreen) VideoFlags |= SDL_FULLSCREEN;
    SDL_version ver;
    SDL_VERSION(&ver);
    pLog->_Add("SDL Version %d.%d.%d", ver.major, ver.minor, ver.patch);
    dlcsm_make_str(vdriver);
    SDL_VideoDriverName(vdriver, sizeof(vdriver));
    pLog->_Add("Video driver[%s]", vdriver);
    VideoModes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    if (VideoModes == (SDL_Rect **)0) {
    } else {
        if (VideoModes == (SDL_Rect **)-1)
            pLog->_DebugAdd("All resolutions available");
        else {
            pLog->_DebugAdd("Available Modes");
            for (int i = 0; VideoModes[i]; ++i) pLog->_DebugAdd("  %d x %d", VideoModes[i]->w, VideoModes[i]->h);
        }
    }
    const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();
    if (VideoInfo) {
        pLog->_DebugAdd("VideoInfo->hw_available [%d]        ", VideoInfo->hw_available);
        pLog->_DebugAdd("VideoInfo->wm_available [%d]        ", VideoInfo->wm_available);
        pLog->_DebugAdd("VideoInfo->blit_hw [%d]             ", VideoInfo->blit_hw);
        pLog->_DebugAdd("VideoInfo->blit_hw_CC [%d]          ", VideoInfo->blit_hw_CC);
        pLog->_DebugAdd("VideoInfo->blit_hw_A [%d]           ", VideoInfo->blit_hw_A);
        pLog->_DebugAdd("VideoInfo->blit_sw [%d]             ", VideoInfo->blit_sw);
        pLog->_DebugAdd("VideoInfo->blit_sw_CC [%d]          ", VideoInfo->blit_sw_CC);
        pLog->_DebugAdd("VideoInfo->blit_sw_A [%d]           ", VideoInfo->blit_sw_A);
        pLog->_DebugAdd("VideoInfo->blit_fill [%d]           ", VideoInfo->blit_fill);
        pLog->_DebugAdd("VideoInfo->video_mem [%d]           ", VideoInfo->video_mem);
        pLog->_DebugAdd("VideoInfo->vfmt->BitsPerPixel [%d]  ", VideoInfo->vfmt->BitsPerPixel);
        pLog->_DebugAdd("VideoInfo->vfmt->BytesPerPixel [%d] ", VideoInfo->vfmt->BytesPerPixel);
        pLog->_DebugAdd("VideoInfo->vfmt->Rloss [%d]         ", VideoInfo->vfmt->Rloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Gloss [%d]         ", VideoInfo->vfmt->Gloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Bloss [%d]         ", VideoInfo->vfmt->Bloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Aloss [%d]         ", VideoInfo->vfmt->Aloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Rshift [%d]        ", VideoInfo->vfmt->Rshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Gshift [%d]        ", VideoInfo->vfmt->Gshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Bshift [%d]        ", VideoInfo->vfmt->Bshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Ashift [%d]        ", VideoInfo->vfmt->Ashift);
        pLog->_DebugAdd("VideoInfo->vfmt->Rmask [%d]         ", VideoInfo->vfmt->Rmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Gmask [%d]         ", VideoInfo->vfmt->Gmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Bmask [%d]         ", VideoInfo->vfmt->Bmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Amask [%d]         ", VideoInfo->vfmt->Amask);
        pLog->_DebugAdd("VideoInfo->vfmt->colorkey [%d]      ", VideoInfo->vfmt->colorkey);
        pLog->_DebugAdd("VideoInfo->vfmt->alpha [%d]         ", VideoInfo->vfmt->alpha);
    } else {
        bSDLFailed = true;
        pLog->_Add("Failed getting Video Info : %s", SDL_GetError());
        return false;
    }
    if (VideoInfo->hw_available) {
        VideoFlags |= SDL_HWSURFACE;
        pLog->_Add("Hardware surfaces...");
    } else {
        VideoFlags |= SDL_SWSURFACE;
        pLog->_Add("Software surfaces...");
    }
    if (VideoInfo->blit_hw) {
        VideoFlags |= SDL_HWACCEL;
        pLog->_Add("Hardware acceleration enabled!");
    }

    if (SDL_VideoModeOK(ScreenWidth, ScreenHeight, ScreenColors, VideoFlags)) {
    } else {
        pLog->_Add("SDL_VideoModeOK failure");
        return false;
    }
    pScreen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ScreenColors, VideoFlags);
    if (!pScreen) {
        bSDLFailed = true;
        pLog->_Add("SDL_SetVideoMode failed");
        return false;
    }
    SDL_ShowCursor(SDL_DISABLE);
    SetWindowTitle(wincaption);
    pLog->_Add("SDL GFX Initialized");
    return true;
}
void C_GFX2D::SetWindowTitle(string fmt, ...) {
    char ach[512];
    char f2[512];
    strcpy(f2, fmt.c_str());
    va_list va;
    va_start(va, f2);
    vsprintf(ach, f2, va);
    va_end(va);
    windowcaption.assign(ach);
    SDL_WM_SetCaption(windowcaption.c_str(), 0);
}
void   C_GFX2D::ShutDownGFX(void) { SDL_FreeSurface(pScreen); }
void   C_GFX2D::Flip(void) { SDL_GL_SwapBuffers(); }
u_char C_GFX2D::GetFade(char cWhichFade) {
    static u_char        cFader1           = 0;
    static unsigned long dwFadeTimer1      = dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime1 = 400;
    static char          cFadeDir1         = 1;
    static u_char        cFader2           = 0;
    static unsigned long dwFadeTimer2      = dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime2 = 300;
    static char          cFadeDir2         = 1;
    static u_char        cFader3           = 0;
    static unsigned long dwFadeTimer3      = dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime3 = 100;
    static char          cFadeDir3         = 1;

    static unsigned long dwFadeTimer4      = dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime4 = 100;
    static u_char        cFader4           = 0;

    if (dlcs_get_tickcount() - dwFadeTimer1 > dwFadeChangeTime1) {
        dwFadeTimer1 = dlcs_get_tickcount();
        if (cFader1 == 255) cFadeDir1 = -1;
        if (cFader1 == 0) cFadeDir1 = 1;
        cFader1 += cFadeDir1;
    }
    if (dlcs_get_tickcount() - dwFadeTimer2 > dwFadeChangeTime2) {
        dwFadeTimer2 = dlcs_get_tickcount();
        if (cFader2 == 255) cFadeDir2 = -1;
        if (cFader2 == 0) cFadeDir2 = 1;
        cFader2 += cFadeDir2;
    }
    if (dlcs_get_tickcount() - dwFadeTimer3 > dwFadeChangeTime3) {
        dwFadeTimer3 = dlcs_get_tickcount();
        if (cFader3 >= 252) cFadeDir3 = -3;
        if (cFader3 <= 3) cFadeDir3 = 3;
        cFader3 += cFadeDir3;
    }
    if (dlcs_get_tickcount() - dwFadeTimer4 > dwFadeChangeTime4) {
        cFader4++;
    }
    if (cWhichFade == 1) return cFader1;
    if (cWhichFade == 2) return cFader2;
    if (cWhichFade == 3) return cFader3;
    if (cWhichFade == 4) return cFader4;
    return 0;
}
