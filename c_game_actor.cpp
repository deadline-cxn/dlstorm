/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_game_actor.cpp
 **   Description:  Game Actor Class File
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **   Notes:
 **     This class is to be used with the Game Class
 **
 ***************************************************************/

#include "c_game_actor.h"

/////////////////////////////////////////////////////////////////////////////////

CGameActorAnimationFrame::CGameActorAnimationFrame() { Init(); }
CGameActorAnimationFrame::~CGameActorAnimationFrame() { }
void CGameActorAnimationFrame::Init(void) {
    pNext=0;
    rect.x=0;
    rect.y=0;
    rect.w=32;
    rect.h=32;
    pSurface=0;
    pParent=0;
}

/////////////////////////////////////////////////////////////////////////////////

CGameActorAnimation::CGameActorAnimation() { Init(); }
CGameActorAnimation::CGameActorAnimation(CGameActor *parent, const char *name) {
    Init();
    pParent=parent;
    strcpy(szName,name);
}

CGameActorAnimation::~CGameActorAnimation() {
    FramesClear(); 
}

void CGameActorAnimation::Init(void) {
    memset(szName,0,_NAME_SIZE);
    pFirstFrame=0;
    pCurrentFrame=0;
    pNext=0;
    pParent=0;
    visible=true;
    animtimer=dlcs_get_tickcount();
    animation_speed=500;
    animation_loop=true;
    animtimer=500;
}

void CGameActorAnimation::FrameAdd(int x,int y,int w,int h) {
    FrameAdd(pParent->source_surface,x,y,w,h);
}

void CGameActorAnimation::FrameAdd(SDL_Surface *pInSurface,int x,int y,int w,int h) {
    CGameActorAnimationFrame *which;
    which=0;
    which=FrameFindLast();
    if(!which) {
        pFirstFrame=new CGameActorAnimationFrame();
        which=pFirstFrame;
    }
    else {
        which->pNext=new CGameActorAnimationFrame();
        which=which->pNext;
    }
    // Added, now populate
    which->pSurface=pInSurface;
    which->rect.x=x;
    which->rect.y=y;
    which->rect.w=w;
    which->rect.h=h;
    which->pParent=this;
    pCurrentFrame=pFirstFrame;
}

CGameActorAnimationFrame *CGameActorAnimation::FrameFindLast(void) {
    CGameActorAnimationFrame *index;
    CGameActorAnimationFrame *which;
    which=0;
    index=pFirstFrame;
    while(index) {
        which=index;
        index=index->pNext;
    }
    return which;
}


void CGameActorAnimation::FramesClear(void) {
    CGameActorAnimationFrame *which;
    CGameActorAnimationFrame *delobj;
    which=pFirstFrame;
    while(which) {
        delobj=which;
        which=which->pNext;
        DEL(delobj);
    }
}

void CGameActorAnimation::IncrementCurrentFrame(void) {
    if(pCurrentFrame->pNext) {
        pCurrentFrame=pCurrentFrame->pNext;
    }
    else if(animation_loop) {
        pCurrentFrame=pFirstFrame;
    }
}

/////////////////////////////////////////////////////////////////////////////////

CGameActor::CGameActor() { Init(); }
CGameActor::CGameActor(CGame *inParent,const char *inName) {
    Init();
    pParent=inParent;
    strcpy(this->szName,inName);
}

CGameActor::~CGameActor() {
    AnimationsClear();
}

void CGameActor::Init(void) {
    pParent=0;
    source_surface=0;
    pNext=0;
    memset(szName,0,_NAME_SIZE);
    pFirstAnimation=0;
    memset(szCurrentAnimation,0,_NAME_SIZE);
    pCurrentAnimation=0;
    weapon1=0;
    weapon2=0;
    at_sta=0;
    at_int=0;
    at_spi=0;
    at_wis=0;
    at_agi=0;
    at_con=0;

    // From CSprite
    x=0;
    y=0;
    visible=false;
    animation_frame=0;
    animation_lastframe=0;
    animation_speed=180;
    for(int i=0;i<256;i++) {
        rect[i].x=0;
        rect[i].y=0;
        rect[i].w=64;
        rect[i].h=64;
    }
    xdir=0;
    ydir=0;
    xspeed=1;
    yspeed=1;

    // From CEntity
    type=G_ENTITY_FRIENDLY;
    life_points=100;
    mana_points=100;
    power_points=100;
    rage_points=100;
    resource_min=0;       // 0 = infinite resources   
    resource_max=0;       // 0 = infinite resources
    respawn_min=0;        // 0 = infinite respawns
    respawn_max=0;        // 0 = infinite respawns
    respawn_time_min=30000;   // 0 = default; default is 5 minutes (30000)
    respawn_time_max=30000;   // 0 = default; default is 5 minutes (30000)
}

void CGameActor::LoadGAFSurface(const char *szFile) {
    source_surface=pParent->SDL->LoadGAFSurface(szFile);
}

void CGameActor::AnimationAdd(const char *name) {
    CGameActorAnimation *which;
    which=0;
    which=AnimationFindLast();
    if(which==0) {
        pFirstAnimation=new CGameActorAnimation(this,name);
        return;
    }

    if(which) {
        which->pNext=new CGameActorAnimation(this,name);
        if(which->pNext) {
            which=which->pNext;
        }
    }
}

CGameActorAnimation *CGameActor::AnimationFindLast(void) {
    CGameActorAnimation *index;
    CGameActorAnimation *which;
    which=0;
    index=pFirstAnimation;
    while(index) {
        which=index;
        index=index->pNext;
    }
    return which;
}

CGameActorAnimation *CGameActor::Animation(const char *name) {
    CGameActorAnimation *which;
    CGameActorAnimation *index;
    which=0;
    index=pFirstAnimation;
    while(index) {
        if(dlcs_strcasecmp(index->szName,name)) which=index;
        index=index->pNext;
    }
    return which;
}

void CGameActor::AnimationsClear(void) {
    CGameActorAnimation *which;
    CGameActorAnimation *delobj;
    which=pFirstAnimation;
    while(which) {
        delobj=which;
        which=which->pNext;
        DEL(delobj);
    }
}

void CGameActor::AnimationSet(const char *name) {
    strcpy(szCurrentAnimation,name);
    pCurrentAnimation=Animation(szCurrentAnimation);
}
