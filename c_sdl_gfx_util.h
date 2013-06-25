/* Seth's SDL Graphics Wrapper */

#ifndef _SDL_GFX_LIBRARY
#define _SDL_GFX_LIBRARY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef TEST_VGA16
#define NUM_COLORS  16
#else
#define NUM_COLORS  256
#endif

#include "SDL.h"
#include "b_filecab.h"

//#pragma comment( lib, "SDLmain.lib" )
//#pragma comment( lib, "SDL.lib" )

#define SDL_RGB(r,g,b) SDL_MapRGB(SDL->screen->format,r,g,b)
SDL_Surface * LoadMemBMP(unsigned char *fb, int Size);

class CSDL_Wrap
{
public:

    CSDL_Wrap();
    CSDL_Wrap(Uint32 w,Uint32 h,Uint32 b);
	CSDL_Wrap(Uint32 w,Uint32 h,Uint32 b,bool f);
    ~CSDL_Wrap();

    bool InitSuccess;

    Uint32 w;
    Uint32 h;
    Uint32 b;
    int    videoflags;

    SDL_Surface *screen;


    bool         Init2D(int width, int height, int bpp);
	bool         Init2D(int width, int height, int bpp, bool fullscreen);
    bool         ToggleFullScreen(void);
    Uint32       getpixel(int x, int y);
    void         putpixel(int x, int y, Uint32 pixel);
    void         ClearScreen(Uint32 color);
    SDL_Surface *CreateScreen(Uint16 w, Uint16 h, Uint8 bpp, Uint32 flags);
    void         Flip(void);



};



#endif

