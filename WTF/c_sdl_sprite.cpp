
#include "c_sdl_sprite.h"

CSprite::CSprite(){
    Init();
}
CSprite::CSprite(SDL_Surface *source_surface){
    Init();
    source_surface=source_surface;
    target_surface=0;
}

CSprite::CSprite(SDL_Surface *s_surface,SDL_Surface *t_surface){
    Init();
    source_surface=s_surface;
    target_surface=t_surface;
}

CSprite::~CSprite() {
}

void CSprite::Init(){
	source_surface=0;
	target_surface=0;
    x=0;
    y=0;
    visible=false;
    animation_frame=0;
    animation_lastframe=0;
    animation_speed=180;
    for(int i=0;i<256;i++)
    {
        rect[i].x=0;
        rect[i].y=0;
        rect[i].w=0;
        rect[i].h=0;
    }
    xdir=0;
    ydir=0;
    xspeed=1;
    yspeed=1;
}

void CSprite::Draw() { Draw(x,y); }

void CSprite::Draw(float x, float y){
    if(animated)
    {
        if(animtimer < ( getticks()-animation_speed))
        {
            animtimer= getticks();


            animation_frame++;

            if(animation_frame>animation_lastframe)
            {
                if(animation_loop==true) animation_frame=0;
                else animation_frame=animation_lastframe;
            }
        }
    }
    SDL_Rect destrec; destrec.x=x; destrec.y=y;
	if(target_surface==0) target_surface=SDL_GetVideoSurface();
    SDL_BlitSurface(source_surface, &rect[animation_frame], target_surface, &destrec);
}

void CSprite::LoadDLZ(CGAF *cab,char *file){
	GAF_FileBuffer fb;
	fb=cab->GetFile(file);
	LoadMemBMP(fb.fb);
}

void CSprite::LoadMemBMP(unsigned char *fb) {
    // source_surface=fb;
}

void CSprite::LoadSurface(char *fname){
    source_surface=SDL_LoadBMP(fname);
}

void CSprite::SetColorKey(int r, int g, int b){
    SDL_SetColorKey(source_surface, SDL_SRCCOLORKEY,SDL_MapRGB(source_surface->format, 0, 0, 0));
}
