/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_game.h
 **   Description:  Game Class Header File
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_GAME_CLASS
#define _DLCS_GAME_CLASS

#include <vector>

#include "c_console.h"
#include "c_entity.h"
#include "c_event.h"
#include "c_game_actor.h"
#include "c_log.h"
#include "c_map.h"
#include "c_sdl.h"
#include "c_timer.h"

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// Game play modes
#define PLAY            0
#define BUILD_GHOST     1
#define BUILD           2
#define SPECTATOR       3
#define SPECTATOR_LOGIN 4
#define CHATROOM        5
#define LOGIN           6

#define G_CORE    1  // Always will be on
#define G_SDL     2
#define G_FMOD    4
#define G_LUA     8
#define G_GAF     16
#define G_CONSOLE 32
#define G_ENTITY  64
#define G_DATA    128
#define G_LOG     256
#define G_SPRITE  512
#define G_SAMPLE  1024
#define G_FONT    2048
#define G_TIMER   4096
#define G_SDL2D   8192

#define SGC_ALL 65535

class CGameActor;
class C_CONS;

class CGame {
   public:
    CGame();
    CGame(const char *inAPP_NAME, Uint16 iFlags);
    CGame(const char *inAPP_NAME, Uint32 iScreenWidth, Uint32 iScreenHeight, Uint32 iScreenColors, Uint16 iFlags);
    ~CGame();
    CLog *       Log;
    C_CONS *     Console;
    SDL_Surface *ConsoleSurface;
    CSDL_Wrap *  SDL;
    CGameActor * pFirstActor;  // CFMOD       *SND;
    Uint16       flags;
    Uint32       screen_width;
    Uint32       screen_height;
    Uint32       screen_colors;
    bool         bShowConsole;
    bool         G_QUIT;
    int          G_KEYDOWN;
    void         ZeroVars(void);
    bool         Loop(void);
    void         Start(const char *APP_NAME, Uint16 iFlags);
    void         ShutDown(void);
    bool         UpdateGFXStart(void);
    bool         DrawVortex(void);
    void         DrawConsole(void);
    void         ConsoleLog(const char *szEntry);
    bool         UpdateGFXEnd(void);
    bool         UpdateInput(void);
    bool         Inject(Uint16 TYPE, const char *name);
    void         UserInit(void);
    void         UserGFX(void);
    void         UserKeys(void);
    void         UserMain(void);
    void         ActorAdd(const char *name);
    void         ActorDel(const char *name);
    void         ActorsClear(void);
    void         ActorChangeRect(const char *name, int which, int x, int y, int w, int h);
    CGameActor * ActorFind(const char *name);
    CGameActor * Actor(const char *name);
    CGameActor * ActorFindLast(void);
    void         ActorShow(const char *name);
    void         ActorHide(const char *name);
    void         UpdateActorAnimations(void);
    void         ActorSetAnimated(const char *name, bool animated);
    void         ActorSetAnimFrame(const char *name, int frame);
    void         ActorSetAnimFrames(const char *name, int frames);
    void         ActorSetAnimSpeed(const char *name, long speed);
    void         ActorSetAnimLoop(const char *name, bool loop);
    void         ActorDraw(const char *name);
    void         ActorMoveTo(const char *name, int x, int y);
};

#endif  // _DLCS_GAME_CLASS
