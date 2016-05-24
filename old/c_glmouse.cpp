/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_glmouse.cpp
 **   Class:        C_Mouse
 **                 C_MouseCursor
 **   Description:  Mouse SDL/OpenGL wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_glmouse.h"
///////////////////////////// C_MouseCursor Class
C_MouseCursor::C_MouseCursor() {
    filename.clear();
    pTexture=0;
    pLog=new CLog("mousecursor.log");
    bCreatedLog=true;
    pLog->_DebugAdd("Mouse cursor created");
}
C_MouseCursor::C_MouseCursor(CLog *pInLog) {
    pGAF=0;
    bCreatedLog=false;
    pLog=pInLog;
    pTexture=0;
    x_offset=0;
    y_offset=0;
    x=0;
    y=0;
    x_hotspot=0;
    y_hotspot=0;
    pLog->_DebugAdd(">> C_MouseCursor::C_MouseCursor(CGAF *pInGAF, CLog *pInLog) OK");
}
C_MouseCursor::C_MouseCursor(CGAF *pInGAF, CLog *pInLog) {
    pGAF=pInGAF;
    pLog=pInLog;
    bCreatedLog=false;
    pTexture=0;
    x_offset=0;
    y_offset=0;
    x=0;
    y=0;
    x_hotspot=0;
    y_hotspot=0;
    pLog->_DebugAdd(">> C_MouseCursor::C_MouseCursor(CGAF *pInGAF, CLog *pInLog) OK");
}
C_MouseCursor::C_MouseCursor(string f) {
    filename.clear();
    pTexture=0;
    x_offset=0;
    y_offset=0;
    x=0;
    y=0;
    x_hotspot=0;
    y_hotspot=0;
    load(f);
    pLog=new CLog("mousecursor.log");
    bCreatedLog=true;
    pLog->_DebugAdd("Mouse cursor created");
}
C_MouseCursor::~C_MouseCursor() {
    kill();
    pLog->_DebugAdd("Mouse cursor destroyed");
    if(bCreatedLog) dlcsm_delete(pLog);
}
GLvoid C_MouseCursor::kill() {
    dlcsm_delete(pTexture);
}
GLvoid C_MouseCursor::loadGAF(string f) { }
GLvoid C_MouseCursor::load(string f) {
    filename=f;
    if(pGAF) loadGAF(filename);
    else {
        pTexture=new CGLTexture(pLog);
        pTexture->LoadGL(filename);
        pLog->_DebugAdd("MOUSE TEXTURE: %s\n",pTexture->filename.c_str());
    }
    /*  if(!strlen(file)) return;
        strcpy(filename,file);
        DEL(Cursor);
        Cursor=new CGLTexture(pLog);
        Cursor->pGAF=pGAF;
        if(!Cursor) return;
        Cursor->glBmap=0;
        Cursor->mask=0;
        Cursor->usemask=1;
        Cursor->LoadBMP(pGAF,va("%s.bmp",file),0);
    	std::vector <std::string> vs;
    	if(!pGAF) return;
    	GAF_FileBuffer nfb;
    	char fin[1024];    memset(fin,0,1024);
    	nfb=pGAF->GetFile((LPSTR)va("%s.ini",file));
    	if(nfb.Size>0) {
    	    vs=explode("\n",nfb.fb);
    		// Log("                    \\--> Point file: %s.ini",file);
    		x_offset=atoi(vs[0].c_str()); //Log("                       \\--> x_offset = %d",x_offset);
    		y_offset=atoi(vs[1].c_str()); //Log("                       \\--> y_offset = %d",y_offset);
    		x_hotspot=atoi(vs[2].c_str()); //Log("                       \\--> x_hopspot = %d",x_hotspot);
    		y_hotspot=atoi(vs[3].c_str()); //Log("                       \\--> y_hopspot = %d",y_hotspot);
    	}
    	*/
}
GLvoid C_MouseCursor::draw(void) {
    if(!pTexture) return;
    if(!pTexture->glBmap) load(filename);
    if(!pTexture->glBmap) return;
    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
    glBindTexture(GL_TEXTURE_2D,pTexture->glBmap);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(x ),    float(y - 64),    1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(x+64),  float(y - 64),    1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(x+64),  float(y ),        1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(x ),    float(y ),        1);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
}
///////////////////////////// C_Mouse class
C_Mouse::C_Mouse() { }
C_Mouse::C_Mouse(CLog *pInLog) {
    pGAF=0;
    pLog=pInLog;
    pCursor=new C_MouseCursor(pLog);
    pCursor->load("mouse/default.png");
}
C_Mouse::C_Mouse(CGAF *pInGAF, CLog *pInLog) {
    pGAF=pInGAF;
    pLog=pInLog;
    pCursor=new C_MouseCursor(pLog);
    pCursor->load("mouse/default.png");
}
C_Mouse::~C_Mouse() {
    dlcsm_delete(pCursor);
}
void C_Mouse::InitializeInput(void) {
    bLeftDown             = 0;
    bMiddleDown           = 0;
    bRightDown            = 0;
    bLeftDblClick         = 0;
    bMiddleDblClick       = 0;
    bRightDblClick        = 0;
    bWheelUp              = 0;
    bWheelDown            = 0;
    lLeftDownTick         = dlcs_get_tickcount();
    lMiddleDownTick       = dlcs_get_tickcount();
    lRightDownTick        = dlcs_get_tickcount();
    lLeftDblClickTimer    = dlcs_get_tickcount();
    lMiddleDblClickTimer  = dlcs_get_tickcount();
    lRightDblClickTimer   = dlcs_get_tickcount();
}
void C_Mouse::draw() {
    if(!bDraw) return;
    pCursor->x=ix+pCursor->x_offset;
    pCursor->y=iy+pCursor->y_offset;
    pCursor->draw();
}
void C_Mouse::ClearClicks(void) {
    bLeftRelease=0;
    bRightRelease=0;
    bMiddleRelease=0;
    bLeftDblClick=0;
    bMiddleDblClick=0;
    bRightDblClick=0;
}
void C_Mouse::Refresh(void) {
    bool bLeft=0;
    bool bRight=0;
    bool bMiddle=0;
    int butt;
    unsigned long current_tick=dlcs_get_tickcount();
    butt       = SDL_GetMouseState(&ix,&iy);
    bLeft      = dlcsm_tobool((SDL_BUTTON(SDL_BUTTON_LEFT)      & butt));
    bMiddle    = dlcsm_tobool((SDL_BUTTON(SDL_BUTTON_MIDDLE)    & butt));
    bRight     = dlcsm_tobool((SDL_BUTTON(SDL_BUTTON_RIGHT)     & butt));
    bLeftRelease=0;
    bRightRelease=0;
    bMiddleRelease=0;
    bLeftDblClick=0;
    bMiddleDblClick=0;
    bRightDblClick=0;
    bLeftDownTick=0;
    bMiddleDownTick=0;
    bRightDownTick=0;
    if(bLeftDown) {
        if(current_tick-lLeftDownTick > C_GLM_DOWNTICK_TIME) {
            bLeftDownTick=1;
            lLeftDownTick=current_tick;
        }
        if(!bLeft) {
            bLeftRelease = 1;
            if(current_tick-lLeftDblClickTimer<C_GLM_DOUBLECLICK_TIME) {
                bLeftDblClick = 1;
            }
            lLeftDblClickTimer=current_tick;
        }
    }
    if(bRightDown) {
        if(current_tick-lRightDownTick > C_GLM_DOWNTICK_TIME) {
            bRightDownTick=1;
            lRightDownTick=current_tick;
        }
        if(!bRight) {
            bRightRelease  = 1;
            if(current_tick-lRightDblClickTimer<C_GLM_DOUBLECLICK_TIME) {
                bRightDblClick = 1;
            }
            lRightDblClickTimer=current_tick;
        }
    }
    if(bMiddleDown) {
        if(current_tick-lMiddleDownTick > C_GLM_DOWNTICK_TIME) {
            bMiddleDownTick=1;
            lMiddleDownTick=current_tick;
        }
        if(!bMiddle) {
            bMiddleRelease = 1;
            if(current_tick-lMiddleDblClickTimer<C_GLM_DOUBLECLICK_TIME) {
                bMiddleDblClick = 1;
            }
            lMiddleDblClickTimer=current_tick;
        }
    }
    bLeftDown   = bLeft;
    bRightDown  = bRight;
    bMiddleDown = bMiddle;
}
bool C_Mouse::ButtonDownTick(int iWhich) {
    switch(iWhich) {
    case SDL_BUTTON_LEFT:
        return bLeftDownTick;
    case SDL_BUTTON_MIDDLE:
        return bMiddleDownTick;
    case SDL_BUTTON_RIGHT:
        return bRightDownTick;
    default:
        return 0;
        break;
    }
    return 0;
}
void C_Mouse::SetDownTick(int iWhich,bool set) {
    switch(iWhich) {
    case SDL_BUTTON_LEFT:
        bLeftDownTick=set;
        return;
    case SDL_BUTTON_MIDDLE:
        bMiddleDownTick=set;
        return;
    case SDL_BUTTON_RIGHT:
        bRightDownTick=set;
        return;
    default:
        break;
    }
}
bool C_Mouse::LeftClick() {
    return Click(SDL_BUTTON_LEFT);
}
bool C_Mouse::MiddleClick() {
    return Click(SDL_BUTTON_MIDDLE);
}
bool C_Mouse::RightClick() {
    return Click(SDL_BUTTON_RIGHT);
}
bool C_Mouse::Click(int iWhich) {
    switch(iWhich) {
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
void C_Mouse::SetClick(int iWhich,bool set) {
    switch(iWhich) {
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
void C_Mouse::SetButtonDown(int iWhich,bool set) {
    switch(iWhich) {
    case SDL_BUTTON_LEFT:
        bLeftDown=set;
    case SDL_BUTTON_RIGHT:
        bRightDown=set;
    case SDL_BUTTON_MIDDLE:
        bMiddleDown=set;
    default:
        return;
    }
}
bool C_Mouse::ButtonDown(int iWhich) {
    switch(iWhich) {
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
bool C_Mouse::DoubleClick(int iWhich) {
    switch(iWhich) {
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
int  C_Mouse::X(void) {
    return ix;
}
void C_Mouse::SetX(int x) {
    ix=x;
}
int  C_Mouse::Y(void) {
    return iy;
}
void C_Mouse::SetY(int y) {
    iy=y;
}
bool C_Mouse::InX(int x1,int x2) {
    //float sfx=(float)((float)x1/800)*(float)pClientData->ScreenWidth;
    //float mfx=(float)((float)GetMouseX()/800)*(float)pClientData->ScreenWidth;
    //float sfx2=(float)((float)x1/800)*(float)pClientData->ScreenWidth;
    //if((mfx>sfx) && (mfx<sfx2)) return true;
    if((X()>x1) && (X()<x2))
        return true;
    return false;
}
bool C_Mouse::InY(int y1,int y2) {
    //float sfy=(float)((float)y1/600)*(float)pClientData->ScreenHeight;
    //float mfy=(float)((float)GetMouseY()/600)*(float)pClientData->ScreenHeight;
    //float sfy2=(float)((float)y1/600)*(float)pClientData->ScreenHeight;
    //if((mfy>sfy) && (mfy<sfy2)) return true;
    if((Y()>y1) && (Y()<y2)) return true;
    return false;
}
bool C_Mouse::In( int x1,int y1,int x2,int y2) {
    if((InX(x1,x2))&&(InY(y1,y2)))
        return true;
    return false;
}
bool C_Mouse::InRect(RECT rc) {
    return (In(rc.left,rc.top,rc.left+rc.right,rc.top+rc.bottom));
}
bool C_Mouse::InXPct(float fx1,float fx2){
	// if((X()>(pClientData->ScreenWidth*fx1)) && (X()<(pClientData->ScreenWidth*fx2)))
    // return true;
	return false;
}
bool C_Mouse::InYPct(float fy1,float fy2) {
	// if((Y()>(pClientData->ScreenHeight*fy1)) && (Y()<(pClientData->ScreenHeight*fy2)))
    // return true;
	return false;
}
bool C_Mouse::InPct( float fx1,float fy1,float fx2,float fy2) {
	if((InXPct(fx1,fx2))&&(InYPct(fy1,fy2)))
		return true;
	return false;
}
void C_Mouse::SetWheelUp(bool m) {
    bWheelUp=m;
}
int  C_Mouse::WheelUp(void) {
    if(bWheelUp) {
        bWheelUp=0;
        return 1;
    } else {
        return 0;
    }
}
void C_Mouse::SetWheelDown(bool m) {
    bWheelDown=m;
}
int  C_Mouse::WheelDown(void) {
    if(bWheelDown) {
        bWheelDown=0;
        return 1;
    } else {
        return 0;
    }
}

