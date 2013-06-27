
#include "b_2d_font.h"

C2DFont::C2DFont()
{

}
C2DFont::C2DFont(char *file)
{
    Load(file);

}

C2DFont::~C2DFont()
{

}

void C2DFont::Load(char *file)
{
    font = SDL_LoadBMP(file);
    if(font) SDL_SetColorKey(font, SDL_SRCCOLORKEY,SDL_MapRGB(font->format, 0, 0, 0));
}

void C2DFont::LoadDLZ(CCab *cab,char *file)
{
	NF_FileBuffer fb;
	fb=cab->GetFile(file);
	font = LoadMemBMP(fb.FileBuffer,fb.Size);
}

void C2DFont::Write(int x, int y, char *string,int bank)
{
    char c;
    int getx;
    int gety;
    bool draw;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    src_rect.h=15;
    src_rect.w=15;
    dst_rect.x=x;
    dst_rect.y=y;
    for(int i=0;i<strlen(string);i++)
    {
        c=string[i];
        draw=0;
        if((c>32) && (c<128))
        {
            getx=(c-32)*16;
            gety=0;

            while(getx>255)
            {
                gety+=16;
                getx-=256;
            }
            draw=1;
        }
        if(draw)
        {
            src_rect.x=getx;
            src_rect.y=gety+(bank*128);
            SDL_BlitSurface(font, &src_rect, SDL_GetVideoSurface(), &dst_rect);
        }
        dst_rect.x+=10;
    }
}
