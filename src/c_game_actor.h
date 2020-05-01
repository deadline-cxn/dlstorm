/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_game_actor.h
 **   Description:  Game Actor Class Header File
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **   Notes:
 **     This class is to be used with the Game Class
 **
 ***************************************************************/
#ifndef _DLCS_GAME_ACTOR_CLASS
#define _DLCS_GAME_ACTOR_CLASS

#include "c_entity.h"
#include "c_game.h"
#include "c_sdl.h"
#include "dlcs.h"

class CGame;
class CGameActor;
class CGameActorAnimation;

/////////////////////////////////////////////////////////////////////////////////

class CGameActorAnimationFrame {
   public:
    CGameActorAnimationFrame();
    ~CGameActorAnimationFrame();
    CGameActorAnimation *     pParent;
    CGameActorAnimationFrame *pNext;
    SDL_Rect                  rect;
    SDL_Surface *             pSurface;
    void                      Init(void);
};

/////////////////////////////////////////////////////////////////////////////////

class CGameActorAnimation {
   public:
    CGameActorAnimation();
    CGameActorAnimation(CGameActor *parent, const char *name);
    ~CGameActorAnimation();
    char                      szName[_NAME_SIZE];
    CGameActor *              pParent;
    CGameActorAnimation *     pNext;
    CGameActorAnimationFrame *pFirstFrame;
    CGameActorAnimationFrame *pCurrentFrame;
    bool                      visible;
    long                      animation_speed;
    bool                      animation_loop;
    long                      animtimer;
    void                      Init(void);
    void                      FrameAdd(int x, int y, int w, int h);
    void                      FrameAdd(SDL_Surface *pInSurface, int x, int y, int w, int h);
    CGameActorAnimationFrame *FrameFindLast(void);
    void                      FramesClear(void);
    void                      IncrementCurrentFrame(void);
};

/////////////////////////////////////////////////////////////////////////////////

class CGameActor : public CEntity, public CSprite {
   public:
    CGameActor();
    CGameActor(CGame *inParent, const char *inName);
    ~CGameActor();
    CGame *              pParent;
    CGameActor *         pNext;
    CGameActorAnimation *pFirstAnimation;
    char                 szName[_NAME_SIZE];
    char                 szCurrentAnimation[_NAME_SIZE];
    CGameActorAnimation *pCurrentAnimation;
    int                  weapon1;
    int                  weapon2;
    int                  at_sta;
    int                  at_int;
    int                  at_spi;
    int                  at_wis;
    int                  at_agi;
    int                  at_con;
    void                 Init(void);
    void                 LoadGAFSurface(const char *szFile);

    void                 AnimationAdd(const char *name);
    CGameActorAnimation *AnimationFindLast(void);
    CGameActorAnimation *Animation(const char *name);
    void                 AnimationsClear(void);
    void                 AnimationSet(const char *name);
};

#endif  // _DLCS_GAME_ACTOR_CLASS
