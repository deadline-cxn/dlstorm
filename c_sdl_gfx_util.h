/* Seth's SDL Graphics Wrapper */

#ifndef _SDL_GFX_LIBRARY
#define _SDL_GFX_LIBRARY

#ifdef TEST_VGA16
#define NUM_COLORS  16
#else
#define NUM_COLORS  256
#endif

#include "SDL.h"
#include "c_gaf.h"

// #define SDL_RGB(r,g,b) SDL_MapRGB(SDL->screen->format,r,g,b)
// SDL_Surface * LoadMemBMP(unsigned char *fb, int Size);


#endif

