/* Seth's SDL Graphics Wrapper */

#ifndef _SDL_GFX_SPRITE_LIBRARY
#define _SDL_GFX_SPRITE_LIBRARY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlstorm.h"
#include "SDL.h"
#include "b_filecab.h"

#include "c_sdl_gfx_util.h"

#pragma comment( lib, "SDLmain.lib" )   
#pragma comment( lib, "SDL.lib" )

class CSprite
{
public:

    CSprite();
    CSprite(SDL_Surface *source_surface);
    CSprite(SDL_Surface *source_surface,SDL_Surface *target_surface);
    ~CSprite();

    float x;
    float y;
    
    SDL_Surface *source_surface;

    SDL_Surface *target_surface;
    SDL_Rect rect[256];
    bool visible;
    int animation_frame;
    int animation_lastframe;
    long animation_speed;
    bool animation_loop;
    bool animated;
    void Init(void);

    void LoadSurface(char *filename);
	void LoadDLZ(CCab *cab,char *file);

    void SetColorKey(int r, int g, int b);

    void Draw(float x, float y);
    void Draw();
    long animtimer;

    float xdir;
    float ydir;
    float xspeed;
    float yspeed;
};

#endif