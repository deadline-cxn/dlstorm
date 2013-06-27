/* Seth's 2D Font Class */

#ifndef _SETH_2D_FONT
#define _SETH_2D_FONT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SDL.h"

#include "c_sdl_gfx_util.h"
#include "b_filecab.h"

class C2DFont
{
public:

    C2DFont();
    C2DFont(char *file);
    ~C2DFont();

    void Write(int x, int y, char *string,int bank);
    void Load(char *file);
	void LoadDLZ(CCab *cab,char *file);

    SDL_Surface *font;

};

#endif
