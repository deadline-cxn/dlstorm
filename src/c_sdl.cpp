/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_sdl.cpp
 **   Description:  SDL Wrapper Class
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_sdl.h"

#include "SDL.h"
#include "dlcs.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSDL_Wrap Class

CSDL_Wrap::CSDL_Wrap() {
    ZeroVars();
    SDL_Surface *s;
    s = 0;
    Begin(0, 800, 600, 32, s);
}
CSDL_Wrap::CSDL_Wrap(const char *appname, Uint32 nw, Uint32 nh, Uint32 nb) {
    ZeroVars();
    SDL_Surface *s;
    s = 0;
    Begin(appname, nw, nh, nb, s);
}
CSDL_Wrap::CSDL_Wrap(const char *appname, Uint32 nw, Uint32 nh, Uint32 nb, SDL_Surface *icon) {
    ZeroVars();
    Begin(appname, nw, nh, nb, icon);
}
CSDL_Wrap::CSDL_Wrap(const char *appname, Uint32 nw, Uint32 nh, Uint32 nb, const char *icon) {
    ZeroVars();
    Begin(appname, nw, nh, nb, icon);
}
CSDL_Wrap::CSDL_Wrap(const char *appname, Uint32 nw, Uint32 nh, Uint32 nb, const char *icon, bool fullscreen, CLog *inlog, CGAF *ingaf) {
    ZeroVars();
    pLog=inlog;
    pGAF=ingaf;
    Begin(appname,nw,nh,nb,icon);
}

CSDL_Wrap::~CSDL_Wrap() {
    pLog->AddEntry("CSDL_Wrap (DECONSTRUCTING)");
    DEL(pMouse);
    DEL(pGAF);
    DEL(Font);
    SpritesClear();
    DEL(pLog);
    SDL_Quit();
}

void CSDL_Wrap::ZeroVars(void) {
    pFirstSprite = 0;
    window       = 0;
    renderer     = 0;
    pGAF         = 0;
    pLog         = 0;
    Font         = 0;
}

bool CSDL_Wrap::Begin(const char *appname, Uint32 nw, Uint32 nh, Uint32 np, const char *icon) {
    if (!pLog) pLog = new CLog("gfx.log");
    RebuildGAF();
    Icon = LoadGAFSurface(icon);
    return Begin(appname, nw, nh, np, Icon);
}

bool CSDL_Wrap::Begin(const char *appname, Uint32 nw, Uint32 nh, Uint32 np, SDL_Surface *icon) {
    if (!pLog) pLog = new CLog("gfx.log");
    RebuildGAF();
    pLog->AddEntry("CSDL_Wrap::Begin(2)...");
    pMouse = new CMouse();
    Font   = new C2DFont(this);
    w      = nw;
    h      = nh;
    b      = np;
    if (!pGAF) pGAF = new CGAF("gfx.gaf", GAFCOMP_BEST);
    Icon = icon;
    strcpy(APP_NAME, appname);
    InitSuccess = Init2D(w, h, b, Icon);
    return InitSuccess;
}

void CSDL_Wrap::RebuildGAF(void) {
    if (!dlcs_file_exists("gfx.gaf")) {
        pLog->AddEntry("Rebuilding gfx.gaf gaf...");
        remove("gfx.pGAF");
        CGAF *pRebuildGAF;
        pRebuildGAF = new CGAF("gfx.gaf", GAFCOMP_BEST);
        pRebuildGAF->AddDir("", "gfx", 1);
        DEL(pRebuildGAF);
    } else {
        pLog->AddEntry("gfx.gaf GAF exists, not rebuilding...");
    }
}

bool CSDL_Wrap::Init2D(int width, int height, int bpp, SDL_Surface *icon) {
    pLog->AddEntry(va("CSDL_Wrap::Init2D(%d,%d,%d,(SDL_Surface *)%d)", width, height, bpp, icon));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    // videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
    /*
    SDL_WINDOW_FULLSCREEN fullscreen window
    SDL_WINDOW_FULLSCREEN_DESKTOP fullscreen window at the current desktop resolution
    SDL_WINDOW_OPENGL window usable with OpenGL context
    SDL_WINDOW_VULKAN window usable with a Vulkan instance
    SDL_WINDOW_HIDDEN window is not visible
    SDL_WINDOW_BORDERLESS no window decoration
    SDL_WINDOW_RESIZABLE window can be resized
    SDL_WINDOW_MINIMIZED window is minimized
    SDL_WINDOW_MAXIMIZED window is maximized
    SDL_WINDOW_INPUT_GRABBED window has grabbed input focus
    SDL_WINDOW_ALLOW_HIGHDPI window should be created in high-DPI mode if supported (>= SDL 2.0.1)
    */
    videoflags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    window     = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, videoflags);
    if (!window) {
        // videoflags=SDL_SWSURFACE|SDL_FULLSCREEN|SDL_NOFRAME;
        // screen = CreateScreen(width, height, bpp, videoflags);
        // if(!screen) return false;
        return false;
    }
    renderer = 0;
    SDL_CreateWindowAndRenderer(0, 0, videoflags, &window, &renderer);
    if (!renderer) {
        return false;
    }
    if (icon) SDL_SetWindowIcon(window, icon);

    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    // SDL_RenderSetpLogicalSize(sdlRenderer, 640, 480);

    LoadFont("gfx/font.bmp");
    pLog->AddEntry("SDL Video started...");
    return true;
}

SDL_Surface *CSDL_Wrap::LoadGAFSurface(const char *fn) {
    pLog->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\")...", fn));
    if (!pGAF) pGAF = new CGAF("gfx.gaf", GAFCOMP_BEST);
    SDL_Surface *what;
    what = 0;
    SDL_RWops *rw;
    rw = 0;
    // unsigned char * fi;
    int gf_size = 0;
    gf_size     = pGAF->GetFileSize(fn);
    if (gf_size >= 0) {
        unsigned char *gfb;
        gfb  = pGAF->GetFile(fn).fb;
        rw   = SDL_RWFromMem(gfb, gf_size);
        what = LoadMemSurface(rw);
        SDL_FreeRW(rw);

        SDL_SetColorKey(what, SDL_TRUE, SDL_MapRGB(what->format, 0, 0, 0));
        pLog->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\") - [%s] loaded to surface [%d]", fn, fn, what));
    } else {
        pLog->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\") - [%s] doesn't appear to be a valid resource.", fn, fn));
    }
    return what;
}

SDL_Surface *CSDL_Wrap::LoadMemSurface(SDL_RWops *rw) {
    pLog->AddEntry(va("CSDL_Wrap::LoadMemSurface((SDL_RWops *)%d)...", rw));
    SDL_Surface *surface;
    surface = 0;
    if (!rw) return 0;
    if (IMG_isBMP(rw)) {
        surface = IMG_LoadBMP_RW(rw);
        return surface;
    }
    if (IMG_isGIF(rw)) {
        surface = IMG_LoadGIF_RW(rw);
        return surface;
    }
    if (IMG_isJPG(rw)) {
        surface = IMG_LoadJPG_RW(rw);
        return surface;
    }
    if (IMG_isLBM(rw)) {
        surface = IMG_LoadLBM_RW(rw);
        return surface;
    }
    if (IMG_isPCX(rw)) {
        surface = IMG_LoadPCX_RW(rw);
        return surface;
    }
    if (IMG_isPNG(rw)) {
        surface = IMG_LoadPNG_RW(rw);
        return surface;
    }
    if (IMG_isPNM(rw)) {
        surface = IMG_LoadPNM_RW(rw);
        return surface;
    }
    if (IMG_isTIF(rw)) {
        surface = IMG_LoadTIF_RW(rw);
        return surface;
    }
    if (IMG_isXCF(rw)) {
        surface = IMG_LoadXCF_RW(rw);
        return surface;
    }
    if (IMG_isXPM(rw)) {
        surface = IMG_LoadXPM_RW(rw);
        return surface;
    }
    if (IMG_isXV(rw)) {
        surface = IMG_LoadXV_RW(rw);
        return surface;
    }
    if (surface == 0) {
        surface = IMG_LoadTGA_RW(rw);
        return surface;
    }
    pLog->AddEntry(va("CSDL_Wrap::LoadSurface(SDL_RWops *)%d) - end", rw));
    return surface;
}

void CSDL_Wrap::LoadFont(const char *fn) {
    pLog->AddEntry(va("CSDL_Wrap::LoadFont(\"%s\")...", fn));
    Font->FontSurface = LoadGAFSurface(fn);
}

bool CSDL_Wrap::ToggleFullScreen() {
    /*
    int videoflags;
    videoflags = screen->flags;
    videoflags ^= SDL_FULLSCREEN;
    screen = CreateScreen( screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
    if(screen == NULL ) return false;
    return true;
    */
    return true;  // false;
}
Uint32 CSDL_Wrap::GetPixel(int x, int y) {
    if (x > (screen->w - 1)) return 0;
    if (x < 0) return 0;
    if (y > (screen->h - 1)) return 0;
    if (y < 0) return 0;
    int    bpp = screen->format->BytesPerPixel;
    Uint8 *p   = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    switch (bpp) {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32 *)p;
        default: return 0;
    }
}
void CSDL_Wrap::PutPixel(int x, int y, Uint32 pixel) {
    if (x > (screen->w - 1)) return;
    if (x < 0) return;
    if (y > (screen->h - 1)) return;
    if (y < 0) return;
    int    bpp = screen->format->BytesPerPixel;
    Uint8 *p   = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    switch (bpp) {
        case 1: *p = pixel; break;
        case 2: *(Uint16 *)p = pixel; break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4: *(Uint32 *)p = pixel; break;
    }
}

void CSDL_Wrap::ClearScreen(Uint32 color) {
    SDL_Surface *screen;
    screen = SDL_GetWindowSurface(window);
    SDL_Rect barea2;
    barea2.x = 0;
    barea2.y = 0;
    barea2.w = screen->w;
    barea2.h = screen->h;
    SDL_FillRect(screen, &barea2, color);
    //
    //
    // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
}

void CSDL_Wrap::Flip() {
    SDL_UpdateWindowSurface(window);

    // if(videoflags&&SDL_DOUBLEBUF) SDL_Flip(screen);
}

void CSDL_Wrap::DrawRect(int x, int y, int w, int h, Uint32 color) {
    SDL_Rect barea2;
    barea2.x = x;
    barea2.y = y;
    barea2.w = w;
    barea2.h = h;
    SDL_FillRect(screen, &barea2, color);
}

void CSDL_Wrap::DrawMap() {
    int x, y;
    // int dt;
    // SDL_LockSurface(screen);
    for (y = h; y > -1; y--) {
        for (x = 0; x < w; x++) {
            int fartr = rand() % 255;
            // int fartg=rand()%255;
            // int fartb=rand()%255;
            // DrawRect(x*2,y*2,2,2,__RGB(fartr,0,0));
            PutPixel(x, y, SDL_MapRGB(screen->format, 0, 0, fartr));
        }
    }
    // SDL_UnlockSurface(screen);
}

void CSDL_Wrap::Write(int x, int y, const char *string, int bank) { Font->Write(x, y, string, bank); }

void CSDL_Wrap::SpritesDraw(void) {}
void CSDL_Wrap::SpriteAdd(const char *name, const char *file) {
    pLog->AddEntry(va("Adding sprite %s", name));
    CSprite *which;
    which = 0;
    which = SpriteFindLast();
    if (which == 0) {
        pFirstSprite = new CSprite(this);
        strcpy(pFirstSprite->szName, name);
        pFirstSprite->source_surface = LoadGAFSurface(file);
        return;
    }

    if (which) {
        which->pNext = new CSprite(this);
        if (which->pNext) {
            which = which->pNext;
            strcpy(which->szName, name);
            which->source_surface = LoadGAFSurface(file);
        }
    }
}
void CSDL_Wrap::SpriteDel(const char *name) {}

void CSDL_Wrap::SpritesClear(void) {
    CSprite *which;
    CSprite *delSprite;
    which = pFirstSprite;
    while (which) {
        delSprite = which;
        which     = which->pNext;
        pLog->AddEntry(va("Removing sprite %s", delSprite->szName));
        DEL(delSprite);
    }
}

void     CSDL_Wrap::SpriteChangeRect(const char *name, int which, int x, int y, int w, int h) {}
CSprite *CSDL_Wrap::SpriteFind(const char *name) {
    CSprite *index;
    CSprite *which;
    which = 0;
    index = pFirstSprite;
    while (index) {
        if (dlcs_strcasecmp(index->szName, name)) which = index;
        index = index->pNext;
    }
    return which;
}
CSprite *CSDL_Wrap::SpriteFindLast(void) {
    CSprite *index;
    CSprite *which;
    which = 0;
    index = pFirstSprite;
    while (index) {
        which = index;
        index = index->pNext;
    }
    return which;
}
void CSDL_Wrap::SpriteShow(const char *name) {}
void CSDL_Wrap::SpriteHide(const char *name) {}
void CSDL_Wrap::SpriteSetAnimated(const char *name, bool animated) {}
void CSDL_Wrap::SpriteSetAnimFrame(const char *name, int frame) {}
void CSDL_Wrap::SpriteSetAnimFrames(const char *name, int frames) {}
void CSDL_Wrap::SpriteSetAnimSpeed(const char *name, long speed) {}
void CSDL_Wrap::SpriteSetAnimLoop(const char *name, bool loop) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2DFont::C2DFont(CSDL_Wrap *inSDL) {
    SDL         = inSDL;
    FontSurface = 0;
}
C2DFont::C2DFont(CSDL_Wrap *inSDL, const char *file) {
    SDL         = inSDL;
    FontSurface = 0;
    Load(file);
}
C2DFont::~C2DFont() { SDL_FreeSurface(FontSurface); }
void C2DFont::Load(const char *file) {
    /*
    FontSurface = SDL_LoadBMP(file);
    if(FontSurface) {
        SDL_SetColorKey(FontSurface, SDL_TRUE,SDL_MapRGB(FontSurface->format, 0, 0, 0));
    }
    */
}
void C2DFont::Write(int x, int y, const char *string, int bank) {
    char     c;
    int      getx;
    int      gety;
    bool     draw;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    src_rect.h = 15;
    src_rect.w = 15;
    dst_rect.x = x;
    dst_rect.y = y;
    for (unsigned int i = 0; i < strlen(string); i++) {
        c    = string[i];
        draw = 0;
        if ((c > 32) && (c < 128)) {
            getx = (c - 32) * 16;
            gety = 0;
            while (getx > 255) {
                gety += 16;
                getx -= 256;
            }
            draw = 1;
        }
        if (draw) {
            src_rect.x = getx;
            src_rect.y = gety + (bank * 128);
            SDL_BlitSurface(FontSurface, &src_rect, SDL_GetWindowSurface(SDL->window), &dst_rect);
        }
        dst_rect.x += 10;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSprite::CSprite() {}
CSprite::CSprite(CSDL_Wrap *inSDL) {
    Init();
    SDL            = inSDL;
    source_surface = NULL;
    target_surface = SDL_GetWindowSurface(SDL->window);
}
CSprite::CSprite(CSDL_Wrap *inSDL, SDL_Surface *source_surface) {
    SDL = inSDL;
    Init();
    source_surface = source_surface;
    target_surface = 0;
}
CSprite::CSprite(CSDL_Wrap *inSDL, SDL_Surface *s_surface, SDL_Surface *t_surface) {
    SDL = inSDL;
    Init();
    source_surface = s_surface;
    target_surface = t_surface;
}
CSprite::~CSprite() {}

void CSprite::Init() {
    memset(szName, 0, _TEXTNAME_SIZE);
    pNext               = 0;
    x                   = 0;
    y                   = 0;
    visible             = false;
    animation_frame     = 0;
    animation_lastframe = 0;
    animation_speed     = 180;
    for (int i = 0; i < 256; i++) {
        rect[i].x = 0;
        rect[i].y = 0;
        rect[i].w = 64;
        rect[i].h = 64;
    }
    xdir   = 0;
    ydir   = 0;
    xspeed = 1;
    yspeed = 1;
}

void CSprite::SetRect(int which, int x, int y, int w, int h) {
    rect[which].x = x;
    rect[which].y = y;
    rect[which].w = w;
    rect[which].h = h;
}

void CSprite::Draw() { Draw(x, y); }
void CSprite::Draw(float x, float y) {
    if (animated) {
        if (animtimer < (dlcs_get_tickcount() - animation_speed)) {
            animtimer = dlcs_get_tickcount();
            animation_frame++;
            if (animation_frame > animation_lastframe) {
                if (animation_loop == true)
                    animation_frame = 0;
                else
                    animation_frame = animation_lastframe;
            }
        }
    }
    SDL_Rect destrec;
    destrec.x = x;
    destrec.y = y;
    SDL_BlitSurface(source_surface, &rect[animation_frame], target_surface, &destrec);
}
void CSprite::LoadSurface(const char *fname) { source_surface = SDL_LoadBMP(fname); }
/*
void CSprite::LoadMemSurface(unsigned char *fb) {
    source_surface=LoadMemBMP(fb);
}
*/

void CSprite::SetColorKey(int r, int g, int b) { SDL_SetColorKey(source_surface, SDL_TRUE, SDL_MapRGB(source_surface->format, 0, 0, 0)); }

CMouse::CMouse() { Init(); }
CMouse::~CMouse() { ShutDown(); }
void CMouse::Init(void) {
    bLeftDown             = 0;
    bMiddleDown           = 0;
    bRightDown            = 0;
    bLeftDblClick         = 0;
    bMiddleDblClick       = 0;
    bRightDblClick        = 0;
    bWheelUp              = 0;
    bWheelDown            = 0;
    dwLeftDblClickTimer   = dlcs_get_tickcount();
    dwMiddleDblClickTimer = dlcs_get_tickcount();
    dwRightDblClickTimer  = dlcs_get_tickcount();
}

///////////////////////////////////////////////////////////////////////
void CMouse::ShutDown(void) {}  // Obsolete
///////////////////////////////////////////////////////////////////////
void CMouse::RefreshMouse(void) {
    bool bLeft, bRight, bMiddle;
    int  butt;
    butt            = SDL_GetMouseState(&ix, &iy);
    bLeft           = TOBOOL((SDL_BUTTON(SDL_BUTTON_LEFT) & butt));
    bMiddle         = TOBOOL((SDL_BUTTON(SDL_BUTTON_MIDDLE) & butt));
    bRight          = TOBOOL((SDL_BUTTON(SDL_BUTTON_RIGHT) & butt));
    bLeftRelease    = 0;
    bRightRelease   = 0;
    bMiddleRelease  = 0;
    bLeftDblClick   = 0;
    bMiddleDblClick = 0;
    bRightDblClick  = 0;
    if ((bLeftDown) & (!bLeft)) {
        bLeftRelease = 1;
        if (dlcs_get_tickcount() - dwLeftDblClickTimer < 300) bLeftDblClick = 1;
        dwLeftDblClickTimer = dlcs_get_tickcount();
    }
    if ((bRightDown) & (!bRight)) {
        bRightRelease = 1;
        if (dlcs_get_tickcount() - dwRightDblClickTimer < 300) bRightDblClick = 1;
        dwRightDblClickTimer = dlcs_get_tickcount();
    }
    if ((bMiddleDown) & (!bMiddle)) {
        bMiddleRelease = 1;
        if (dlcs_get_tickcount() - dwMiddleDblClickTimer < 300) bMiddleDblClick = 1;
        dwMiddleDblClickTimer = dlcs_get_tickcount();
    }
    bLeftDown   = bLeft;
    bRightDown  = bRight;
    bMiddleDown = bMiddle;
}
void CMouse::UpdateWheel(SDL_Event e) {
    if (e.wheel.y > 0) {  // scroll up
        bWheelUp = true;
    }
    else if (e.wheel.y < 0) {  // scroll down
        bWheelDown = true;
    }
    if (e.wheel.x > 0) {         // scroll right
    }
    else if (e.wheel.x < 0) {  // scroll left
    }
}
///////////////////////////////////////////////////////////////////////
bool CMouse::GetMouseRelease(int iWhich) {
    switch (iWhich) {
        case SDL_BUTTON_LEFT: return bLeftRelease;
        case SDL_BUTTON_MIDDLE: return bMiddleRelease;
        case SDL_BUTTON_RIGHT: return bRightRelease;
        default: return 0; break;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
void CMouse::SetMouseRelease(int iWhich, bool set) {
    switch (iWhich) {
        case SDL_BUTTON_LEFT: bLeftRelease = set; return;
        case SDL_BUTTON_MIDDLE: bMiddleRelease = set; return;
        case SDL_BUTTON_RIGHT: bRightRelease = set; return;
        default: break;
    }
}
///////////////////////////////////////////////////////////////////////
bool CMouse::GetMouseDown(int iWhich) {
    switch (iWhich) {
        case SDL_BUTTON_LEFT: return bLeftDown;
        case SDL_BUTTON_RIGHT: return bRightDown;
        case SDL_BUTTON_MIDDLE: return bMiddleDown;
        default: return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
bool CMouse::GetMouseDblClick(int iWhich) {
    switch (iWhich) {
        case SDL_BUTTON_LEFT: return bLeftDblClick;
        case SDL_BUTTON_RIGHT: return bRightDblClick;
        case SDL_BUTTON_MIDDLE: return bMiddleDblClick;
        default: return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
int CMouse::GetMouseX(void) { return ix; }
///////////////////////////////////////////////////////////////////////
void CMouse::SetMouseX(int x) { ix = x; }
///////////////////////////////////////////////////////////////////////
int CMouse::GetMouseY(void) { return iy; }
///////////////////////////////////////////////////////////////////////
void CMouse::SetMouseY(int y) { iy = y; }
///////////////////////////////////////////////////////////////////////
bool CMouse::MouseInX(int x1, int x2) {
    if ((GetMouseX() > x1) && (GetMouseX() < x2)) return true;
    return false;
}
///////////////////////////////////////////////////////////////////////
bool CMouse::MouseInY(int y1, int y2) {
    if ((GetMouseY() > y1) && (GetMouseY() < y2)) return true;
    return false;
}
///////////////////////////////////////////////////////////////////////
bool CMouse::MouseIn(int x1, int y1, int x2, int y2) {
    if ((MouseInX(x1, x2)) && (MouseInY(y1, y2))) return true;
    return false;
}
/*
///////////////////////////////////////////////////////////////////////
bool MouseInXPct(float fx1,float fx2)
{
        if((GetMouseX()>(pClientData->ScreenWidth*fx1)) && (GetMouseX()<(pClientData->ScreenWidth*fx2)))
                return true;
        return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInYPct(float fy1,float fy2)
{
        if((GetMouseY()>(pClientData->ScreenHeight*fy1)) && (GetMouseY()<(pClientData->ScreenHeight*fy2)))
                return true;
        return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInPct( float fx1,float fy1,float fx2,float fy2)
{
        if((MouseInXPct(fx1,fx2))&&(MouseInYPct(fy1,fy2)))
                return true;
        return false;
}
&*/
///////////////////////////////////////////////////////////////////////
void CMouse::SetMouseWheelUp(bool m) { bWheelUp = m; }
///////////////////////////////////////////////////////////////////////
int CMouse::GetMouseWheelUp(void) { return bWheelUp; }
///////////////////////////////////////////////////////////////////////
void CMouse::SetMouseWheelDown(bool m) { bWheelDown = m; }
///////////////////////////////////////////////////////////////////////
int CMouse::GetMouseWheelDown(void) { return bWheelDown; }
///////////////////////////////////////////////////////////////////////
