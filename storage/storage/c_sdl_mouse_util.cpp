/* Seth's SDL Mouse wrapper */

#include "c_sdl_mouse_util.h"
#include "dlstorm.h"
#include "SDL.h"

//#pragma comment( lib, "SDLmain.lib" )
//#pragma comment( lib, "SDL.lib" )

bool bLeftDown,bMiddleDown,bRightDown;
bool bLeftRelease,bMiddleRelease,bRightRelease;
bool bLeftDblClick,bMiddleDblClick,bRightDblClick;
bool bWheelUp, bWheelDown;
long dwLeftDblClickTimer,dwMiddleDblClickTimer,dwRightDblClickTimer;
int ix,iy;

///////////////////////////////////////////////////////////////////////
void InitializeInput(void)
{
    bLeftDown             = 0;
    bMiddleDown           = 0;
    bRightDown            = 0;
    bLeftDblClick         = 0;
    bMiddleDblClick       = 0;
    bRightDblClick        = 0;
    bWheelUp              = 0;
    bWheelDown            = 0;
    dwLeftDblClickTimer   = getticks();
    dwMiddleDblClickTimer = getticks();
    dwRightDblClickTimer  = getticks();
}
///////////////////////////////////////////////////////////////////////
void ShutDownInput(void) { } // Obsolete
///////////////////////////////////////////////////////////////////////
void RefreshMouse(void)
{
    bool bLeft,bRight,bMiddle;
    int butt;
    butt       = SDL_GetMouseState(&ix,&iy);
    bLeft      = TOBOOL((SDL_BUTTON(SDL_BUTTON_LEFT)      & butt));
    bMiddle    = TOBOOL((SDL_BUTTON(SDL_BUTTON_MIDDLE)    & butt));
    bRight     = TOBOOL((SDL_BUTTON(SDL_BUTTON_RIGHT)     & butt));
    bWheelUp   = TOBOOL((SDL_BUTTON(SDL_BUTTON_WHEELUP)   & butt));
    bWheelDown = TOBOOL((SDL_BUTTON(SDL_BUTTON_WHEELDOWN) & butt));

    bLeftRelease=0;
    bRightRelease=0;
    bMiddleRelease=0;
    bLeftDblClick=0;
    bMiddleDblClick=0;
    bRightDblClick=0;
    if( (bLeftDown)   & (!bLeft))
    {
        bLeftRelease   = 1;
        if(getticks()-dwLeftDblClickTimer<300) bLeftDblClick = 1;
        dwLeftDblClickTimer=getticks();
    }
    if( (bRightDown)  & (!bRight))
    {
        bRightRelease  = 1;
        if(getticks()-dwRightDblClickTimer<300) bRightDblClick = 1;
        dwRightDblClickTimer=getticks();
    }
    if( (bMiddleDown) & (!bMiddle))
    {
        bMiddleRelease = 1;
        if(getticks()-dwMiddleDblClickTimer<300) bMiddleDblClick = 1;
        dwMiddleDblClickTimer=getticks();
    }
    bLeftDown=bLeft;
    bRightDown=bRight;
    bMiddleDown=bMiddle;
}
///////////////////////////////////////////////////////////////////////
bool GetMouseRelease(int iWhich)
{
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftRelease;
        case SDL_BUTTON_MIDDLE:
            return bMiddleRelease;
        case SDL_BUTTON_RIGHT:
            return bRightRelease;
        default:
            return 0;
            break;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
void SetMouseRelease(int iWhich,bool set)
{
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            bLeftRelease=set;
            return;
        case SDL_BUTTON_MIDDLE:
            bMiddleRelease=set;
            return;
        case SDL_BUTTON_RIGHT:
            bRightRelease=set;
            return;
        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////
bool GetMouseDown(int iWhich)
{
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftDown;
        case SDL_BUTTON_RIGHT:
            return bRightDown;
        case SDL_BUTTON_MIDDLE:
            return bMiddleDown;
        default:
            return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
bool GetMouseDblClick(int iWhich)
{
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftDblClick;
        case SDL_BUTTON_RIGHT:
            return bRightDblClick;
        case SDL_BUTTON_MIDDLE:
            return bMiddleDblClick;
        default:
            return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
int  GetMouseX(void){ return ix; }
///////////////////////////////////////////////////////////////////////
void SetMouseX(int x) { ix=x; }
///////////////////////////////////////////////////////////////////////
int  GetMouseY(void){return iy;}
///////////////////////////////////////////////////////////////////////
void SetMouseY(int y) { iy=y; }
///////////////////////////////////////////////////////////////////////
bool MouseInX(int x1,int x2)
{
	if((GetMouseX()>x1) && (GetMouseX()<x2)) return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInY(int y1,int y2)
{
	if((GetMouseY()>y1) && (GetMouseY()<y2)) return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseIn( int x1,int y1,int x2,int y2) { if((MouseInX(x1,x2))&&(MouseInY(y1,y2))) return true; return false; }
/*
///////////////////////////////////////////////////////////////////////
bool MouseInXPct(float fx1,float fx2)
{
	if((GetMouseX()>(pClientData->ScreenWidth*fx1)) && (GetMouseX()<(pClientData->ScreenWidth*fx2)))
		return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInYPct(float fy1,float fy2)
{
	if((GetMouseY()>(pClientData->ScreenHeight*fy1)) && (GetMouseY()<(pClientData->ScreenHeight*fy2)))
		return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInPct( float fx1,float fy1,float fx2,float fy2)
{
	if((MouseInXPct(fx1,fx2))&&(MouseInYPct(fy1,fy2)))
		return true;
	return false;
}
&*/
///////////////////////////////////////////////////////////////////////
void SetMouseWheelUp(bool m) { bWheelUp=m; }
///////////////////////////////////////////////////////////////////////
int  GetMouseWheelUp(void)   { return bWheelUp;   }
///////////////////////////////////////////////////////////////////////
void SetMouseWheelDown(bool m) { bWheelDown=m; }
///////////////////////////////////////////////////////////////////////
int  GetMouseWheelDown(void) { return bWheelDown; }
///////////////////////////////////////////////////////////////////////
