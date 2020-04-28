/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_game.cpp
 **   Description:  Game Class
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_game.h"

#include <vector>

#include "dlcs.h"

CGame::CGame() {
    ZeroVars();
    if (!Log) Log = new CLog("game.log");
    screen_width = 800;
    screen_height = 600;
    screen_colors = 32;
}

CGame::CGame(const char *inAPP_NAME, Uint16 iFlags) {
    ZeroVars();
    if (!Log) Log = new CLog(va("%s.log", inAPP_NAME));
    screen_width = 800;
    screen_height = 600;
    screen_colors = 32;
    Start(inAPP_NAME, iFlags);
}

CGame::CGame(const char *inAPP_NAME, Uint32 iScreenWidth, Uint32 iScreenHeight, Uint32 iScreenColors, Uint16 iFlags) {
    ZeroVars();
    if (!Log) Log = new CLog(va("%s.log", inAPP_NAME));
    screen_width = iScreenWidth;
    screen_height = iScreenHeight;
    screen_colors = iScreenColors;
    Start(inAPP_NAME, iFlags);
}

void CGame::ZeroVars(void) {
    bShowConsole = 0;
    Log = 0;
    Console = 0;
    SDL = 0;
    pFirstActor = 0;
}

CGame::~CGame() { ShutDown(); }

void CGame::Start(const char *inAPP_NAME, Uint16 iFlags) {
    G_QUIT = false;
    flags = iFlags;
    SDL = 0;
    Console = new C_CONS();
    // SND=0;
    zl("------------------------------------------------------");
    zl(va("%s! Started...", inAPP_NAME));
    if (flags & G_SDL) {
        SDL = new CSDL_Wrap(inAPP_NAME, screen_width, screen_height, screen_colors, "gfx/icon.bmp");
        if (!SDL) ShutDown();
        if (!SDL->InitSuccess) {
            zl("SDL Subsystem Init FAILURE!");
            ShutDown();
            return;
        }
    }

    ConsoleSurface = SDL->LoadGAFSurface("gfx/console.jpg");

    zl("SDL Subsystem Initialized...");
    if (flags & G_FMOD) {
        // SND = new CFMOD();
        // zl("SND Subsystem Initialized...");
    }
    srand(time(NULL));
    UserInit();
    zl("UserInit() Completed...");
}

void CGame::ShutDown() {
    zl("Game shutting down...");

    ActorsClear();

    // dlcsm_delete(SND);
    // zl("Game shutting down... SND");

    dlcsm_delete(SDL);
    zl("Game shutting down... SDL");
    DEL(Console);
    dlcsm_delete(Log);
    G_QUIT = true;
}

bool CGame::Inject(Uint16 TYPE, const char *name) { return true; }

bool CGame::UpdateGFXStart() {
    if (!SDL) return false;
    SDL->ClearScreen(SDL_MapRGB(SDL_GetWindowSurface(SDL->window)->format, 255, 0, 0));
    // SDL->DrawSprites();

    return false;
}

bool CGame::UpdateGFXEnd() {
    if (!SDL) {
        G_QUIT = true;
        return false;
    }
    DrawConsole();
    SDL->Flip();
    return false;
}

void CGame::DrawConsole() {
    if (!bShowConsole) {
        return;
    }

    SDL_Rect srect, drect;
    srect.h = 500;
    srect.w = 1920;
    srect.x = 0;
    srect.y = 0;
    drect.h = 500;
    drect.w = 1920;
    drect.x = 0;
    drect.y = 0;
    SDL_BlitSurface(ConsoleSurface, &srect, SDL_GetWindowSurface(SDL->window), &drect);

    int l = 0;
    for (vector<string>::const_iterator i = Console->buf.begin(); i != Console->buf.end(); ++i) {
        SDL->Write(10, 10 + (l * 14), (*i).c_str(), 0);
        l++;
    }
}

void CGame::ConsoleLog(const char *szEntry) {
    Console->AddLine(szEntry);
    zl(va("CONSOLE_LOG:%s", szEntry));
}

bool CGame::UpdateInput() {
    SDL->pMouse->RefreshMouse();
    SDL_Event event;
    G_KEYDOWN = 0;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_MOUSEWHEEL: SDL->pMouse->UpdateWheel(event); break;
        case SDL_KEYDOWN: G_KEYDOWN = event.key.keysym.sym; break;
        // case SDL_VIDEOEXPOSE:            break;
        // case SDL_VIDEORESIZE:            break;
        case SDL_QUIT: G_QUIT = true; break;
        default: break;
    }
    UserKeys();
    return false;
}

bool CGame::Loop() {
    UserMain();
    UpdateActorAnimations();
    UpdateGFXStart();
    UserGFX();
    UpdateGFXEnd();
    UpdateInput();
    return G_QUIT;
}

void CGame::UpdateActorAnimations(void) {
    CGameActor *         index;
    CGameActorAnimation *Anim;
    index = pFirstActor;
    while (index) {
        Anim = index->Animation(index->szCurrentAnimation);
        if (Anim) {
            if (dlcs_get_tickcount() - Anim->animtimer > Anim->animation_speed) {
                Anim->animtimer = dlcs_get_tickcount();
                Anim->IncrementCurrentFrame();
            }
        }
        index = index->pNext;
    }
}

void CGame::ActorAdd(const char *name) {
    zl(va("Adding game actor %s", name));
    CGameActor *which;
    which = 0;
    which = ActorFindLast();
    if (which == 0) {
        pFirstActor = new CGameActor(this, name);
        return;
    }
    if (which) {
        which->pNext = new CGameActor(this, name);
    }
}

void CGame::ActorsClear(void) {
    CGameActor *which;
    CGameActor *delActor;
    which = pFirstActor;
    while (which) {
        delActor = which;
        which = which->pNext;
        zl(va("Removing actor %s", delActor->szName));
        DEL(delActor);
    }
}

CGameActor *CGame::Actor(const char *name) { return ActorFind(name); }
CGameActor *CGame::ActorFind(const char *name) {
    CGameActor *index;
    CGameActor *which;
    which = 0;
    index = pFirstActor;
    while (index) {
        if (dlcs_strcasecmp(index->szName, name)) which = index;
        index = index->pNext;
    }
    return which;
}

CGameActor *CGame::ActorFindLast(void) {
    CGameActor *index;
    CGameActor *which;
    which = 0;
    index = pFirstActor;
    while (index) {
        which = index;
        index = index->pNext;
    }
    return which;
}

void CGame::ActorDel(const char *name) {}
void CGame::ActorChangeRect(const char *name, int which, int x, int y, int w, int h) {}
void CGame::ActorShow(const char *name) {}
void CGame::ActorHide(const char *name) {}
void CGame::ActorSetAnimated(const char *name, bool animated) {}
void CGame::ActorSetAnimFrame(const char *name, int frame) {}
void CGame::ActorSetAnimFrames(const char *name, int frames) {}
void CGame::ActorSetAnimSpeed(const char *name, long speed) {}
void CGame::ActorSetAnimLoop(const char *name, bool loop) {}
void CGame::ActorDraw(const char *name) {
    CGameActor * pActor;
    SDL_Surface *pWhichSurface;
    SDL_Rect     srect;
    SDL_Rect     drect;
    pWhichSurface = 0;
    pActor = 0;
    pActor = ActorFind(name);
    if (pActor) {
        if (pActor->pCurrentAnimation) {
            pWhichSurface = pActor->pCurrentAnimation->pCurrentFrame->pSurface;
            srect.h = pActor->pCurrentAnimation->pCurrentFrame->rect.h;
            srect.w = pActor->pCurrentAnimation->pCurrentFrame->rect.w;
            srect.x = pActor->pCurrentAnimation->pCurrentFrame->rect.x;
            srect.y = pActor->pCurrentAnimation->pCurrentFrame->rect.y;
        } else {
            pWhichSurface = pActor->source_surface;
            srect.h = pActor->rect[pActor->animation_frame].h;
            srect.w = pActor->rect[pActor->animation_frame].w;
            srect.x = pActor->rect[pActor->animation_frame].x;
            srect.y = pActor->rect[pActor->animation_frame].y;
        }
        drect.x = pActor->x;
        drect.y = pActor->y;
        drect.w = srect.w;
        drect.h = srect.h;
        SDL_BlitSurface(pWhichSurface, &srect, SDL_GetWindowSurface(this->SDL->window), &drect);
    }
}
void CGame::ActorMoveTo(const char *name, int x, int y) {
    CGameActor *pActor;
    pActor = 0;
    pActor = ActorFind(name);
    if (pActor) {
        pActor->x = x;
        pActor->y = y;
    }
}
