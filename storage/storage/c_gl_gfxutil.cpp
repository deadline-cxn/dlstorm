/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_gfxutil.h"
#include "b_cam.h"
#include "SDL.h"
#include "b_gl3dmodel.h"
#include "b_gltexture.h"
#include "b_glmouse.h"
#include "b_glfont.h"
#include "b_gui.h" 
#include "c_util.h"
#include "fm_util.h"
#include "c_data.h"
#include "c_map.h"
#include "c_maputil.h"
#include "c_console.h"
#include "c_inpututil.h"
#include "c_netutil.h"
#include "c_sndutil.h"

#include <math.h>

#define X .525731112119133606 
#define Z .850650808352039932

static GLfloat vdata[12][3] = {    
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};
static GLuint tindices[20][3] = { 
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}

void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb) {
    if (div<=0) {
        glNormal3fv(a); glColor3f(cr,cg,cb); glVertex3f(a[0]*r, a[1]*r, a[2]*r);
        glNormal3fv(b); glColor3f(cr,cg,cb); glVertex3f(b[0]*r, b[1]*r, b[2]*r);
        glNormal3fv(c); glColor3f(cr,cg,cb); glVertex3f(c[0]*r, c[1]*r, c[2]*r);
    } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0;i<3;i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab); normalize(ac); normalize(bc);
        drawtri(a, ab, ac, div-1, r,cr,cg,cb);
        drawtri(b, bc, ab, div-1, r,cr,cg,cb);
        drawtri(c, ac, bc, div-1, r,cr,cg,cb);
        drawtri(ab, bc, ac, div-1, r,cr,cg,cb);  //<--Comment this line and sphere looks really cool!
    }  
}

void drawsphere(int ndiv, float radius, float cr,float cg,float cb) {
    glBegin(GL_TRIANGLES);
    for (int i=0;i<20;i++)
        drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius,cr,cg,cb);
    glEnd();
}

/****************************************************************************************************/

int VideoFlags = 0;
SDL_Surface		*pScreen;
CGLMouseList    *mouse;
CGLTextureList  *BaseTexture;
CGLTextureList  *Map_Texture;
CGLTexture      *Default_MapTile;
CGLTexture		*SunTex;
CGLTextureList  *Obj_Texture;
CGLTextureList  *ButtonTexture;
CGLTextureList  *cgltlOptionKeys;
CGLTexture      *cgltBit4ge;
CGLTexture      *cgltFMOD;
CGLTexture      *cgltLUA;
CGLTexture      *cgltSDL;
CGLTexture      *cgltOpenGL;
CGLTexture      *cgltLinux;
CGLTexture      *cgltFreeBSD;
CGLTexture      *cgltWindows;
CGLTexture      *Generic;
CGLTexture      *Console;
CGLModel        *Model;
CGLModel        *FirstModel;
CGLFontList     *font;
C_FM_VIS        *VIS;
C_FM_VIS        *FirstVIS;
GLfloat			LightAmbient[4];
GLfloat			LightDiffuse[4];
GLfloat			LightPosition[4];
GLfloat			LightDir[3];

/****************************************************************************************************/
bool InitializeGFX(bool FullScreen)
{    
    focus_stump=0; focus_control=0;
    mouse=0; BaseTexture=Map_Texture=Obj_Texture=ButtonTexture=0; Generic=Console=0;

    cgltBit4ge=0;
    cgltFMOD=0;
    cgltLUA=0;
    cgltSDL=0;
    cgltLinux=0;
    cgltFreeBSD=0;
    cgltWindows=0;
    cgltlOptionKeys=0;
    cgltOpenGL=0;

    Log("Init SDL/OpenGL GFX Subsystem...");
    
#ifdef __linux__
	putenv("SDL_VIDEODRIVER=dga");
#endif

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    VideoFlags = SDL_OPENGL|SDL_HWPALETTE|SDL_DOUBLEBUF;
    if(FullScreen) VideoFlags |= SDL_FULLSCREEN;
    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware
    if(!VideoInfo) { DLog("Failed getting Video Info : %s",SDL_GetError()); return false; }
    if(VideoInfo->hw_available) { VideoFlags |= SDL_HWSURFACE; Log("Hardware surfaces..."); }
    else                        { VideoFlags |= SDL_SWSURFACE; Log("Software surfaces..."); }

    if(VideoInfo->blit_hw)      { VideoFlags |= SDL_HWACCEL;   Log("Hardware acceleration enabled!"); }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,     1);               // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    Log("Video memory:[%d]",VideoInfo->video_mem);

    pClientData->screen_res_640x480x16   = SDL_VideoModeOK(600,480,16, VideoFlags);
    pClientData->screen_res_800x600x16   = SDL_VideoModeOK(800,600,16, VideoFlags);
    pClientData->screen_res_1024x768x16  = SDL_VideoModeOK(1024,768,16, VideoFlags);
    pClientData->screen_res_1280x1024x16 = SDL_VideoModeOK(1280,1024,16, VideoFlags);
    pClientData->screen_res_640x480x32   = SDL_VideoModeOK(600,480,32, VideoFlags);
    pClientData->screen_res_800x600x32   = SDL_VideoModeOK(800,600,32, VideoFlags);
    pClientData->screen_res_1024x768x32  = SDL_VideoModeOK(1024,768,32, VideoFlags);
    pClientData->screen_res_1280x1024x32 = SDL_VideoModeOK(1280,1024,32, VideoFlags);

    pClientData->ScreenColors=SDL_VideoModeOK(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors,VideoFlags);

    if(!pClientData->ScreenColors) { pClientData->ScreenColors=16; return false; }

    pScreen = SDL_SetVideoMode(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors,VideoFlags);
    if(!pScreen) { DLog("Can't set up pScreen! ErroR!"); return false; }
    SDL_WM_SetCaption(va("EGC %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT),"icon");
    SDL_ShowCursor(SDL_DISABLE);
    if(!InitGeneric())  return false;
    if(!InitConsole())  return false;
    if(!InitFonts())    return false;
    if(!InitMouse())    return false;
    if(!InitBaseGFX())  return false;
    if(!InitMapGFX())   return false;
    if(!InitButtons())  return false;
    if(!InitModels())   return false;
    if(!InitVisibles()) return false;
    if(!InitGL())       return false;
    Log("SDL version[%d.%d.%d]/OpenGL Graphics initialized...",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);
    return true;
}

/****************************************************************************************************/

void ShutDownGFX(void)
{
    Log("Shutting down SDL/OpenGL GFX subsystem...");
    DestroyGeneric();
    DestroyFonts();
    DestroyConsole();
    DestroyMouse();
    DestroyMapGFX();
    DestroyModels();
    DestroyBaseGFX();

    DEL(cgltBit4ge);
    DEL(cgltFMOD);
    DEL(cgltLUA);
    DEL(cgltSDL);
    DEL(cgltLinux);
    DEL(cgltFreeBSD);
    DEL(cgltWindows);
    DEL(cgltOpenGL);

    glFinish();
    glFlush();
    SDL_FreeSurface(pScreen);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    Log("SDL/OpenGL GFX subsystem shutdown...");
}

/****************************************************************************************************/

int InitGL(GLvoid) // All Setup For OpenGL Goes Here
{

	glViewport(0, 0, pClientData->ScreenWidth, pClientData->ScreenHeight);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,1.333f,0.1f,20000.0f);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.30f, 0.90f, 1.0f);				
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

/*  LightAmbient[0]=0.5f;
    LightAmbient[1]=1.5f;
    LightAmbient[2]=1.5f;
    LightAmbient[3]=0.0f;
    LightDiffuse[0]=1.0f;
    LightDiffuse[1]=1.0f;
    LightDiffuse[2]=1.0f;
    LightDiffuse[3]=0.0f;
    LightPosition[0]=0.0f;
    LightPosition[1]=0.0f;
    LightPosition[2]=0.0f;
    LightPosition[3]=-20.0f;
    LightDir[0]=0.0f;
    LightDir[1]=0.0f;
    LightDir[2]=1.0f;
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDir);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One     */

    DLog("InitGL() from [%s]",pClientData->GDDir);
    LoadMouse(va("%s%cgfx%cmouse",pClientData->GDDir,PATH_SEP,PATH_SEP));
    LoadBaseGFX();
    LoadMapGFX(pClientData->GDDir);
    LoadButtons("hi");
    LoadFonts();
    LoadConsole(pClientData->ConsoleBMP);
    LoadModels();
    LoadGeneric(pClientData->szGeneric);
    DLog("OpenGL Initialized...");

    return true; // Initialization Went OK
}

/****************************************************************************************************/

void FlipSurfaces(void) 
{ 
	glFlush(); 
	SDL_GL_SwapBuffers();
}

/****************************************************************************************************/

void BeginScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
    static stra star[500];
    static bool bstars;
    static float ffy;
    static float fff;
    static float dir=-.405f;
    int y;
    glClearColor( (0.2f),(0.4f),(1.0f),0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // change to draw sky here
    DrawBaseGFX(0,0,pClientData->ScreenWidth,pClientData->ScreenHeight,10,155,155,155);

    if(GAME_MODE==GAME_ON) { return; }
    if(!Generic)
    {
        if(!bstars)
        {
            for(y=0;y<500;y++)
            {
                star[y].x=(rand()%900-100);
                star[y].y= rand()%900-150;
                star[y].speed=(rand()%24)*0.48f+1.3f;
                star[y].gfx=rand()%2+12;
            }
            bstars=1;
        }
        fff+=dir;
        if(fff>34)  dir =(-0.405f);
        if(fff<-34) dir =( 0.405f);
        for(y=0;y<500;y++)
        {
            if(GAME_MODE!=GAME_ON)
            {
                DrawBaseGFX( (int)(star[y].x),
                             (int)(600-star[y].y-30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                             (int)(star[y].x+2+star[y].speed ),
                             (int)(600-star[y].y+1+star[y].speed -30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                             star[y].gfx,                        
                             GetFade(3),
                             GetFade(2),
                             GetFade(1) );
            }
            star[y].x+=star[y].speed;
            if(star[y].x> pClientData->ScreenWidth+100) star[y].x=0-(50*star[y].speed);
        }
        y=GetFade(2);
        ffy -= 3.4;
        if(ffy<(-100)) ffy=pClientData->ScreenWidth+100;
        if(GAME_MODE!=RETRO_INTRO_PLAY)
        {
            DrawBar(0,147+y,pClientData->ScreenWidth,168+y,LONGRGB(0,0,0),LONGRGB(255,255,255));
            DrawBar(0,148+y,pClientData->ScreenWidth,167+y,LONGRGB(255,255,255),LONGRGB(0,0,0));
            DrawBar(0,149+y,pClientData->ScreenWidth,166+y,LONGRGB(0,0,0),LONGRGB(255,255,255));
            WriteText(ffy   ,149+y+18*sin(ffy/23)     ,"^+^&^1S",7);
            WriteText((int)ffy+8 ,(int)149+y+18*sin((ffy+8)/23) ,"^+^&^ct",7);
            WriteText((int)ffy+16,(int)149+y+18*sin((ffy+16)/23),"^+^&^ca",7);
            WriteText((int)ffy+24,(int)149+y+18*sin((ffy+24)/23),"^+^&^dn",7);
            WriteText((int)ffy+32,(int)149+y+18*sin((ffy+32)/23),"^+^&^dd",7);
            WriteText((int)ffy+40,(int)149+y+18*sin((ffy+40)/23),"^+^&^eb",7);
            WriteText((int)ffy+48,(int)149+y+18*sin((ffy+48)/23),"^+^&^ey",7);
        }
        else
        {
            if(!cgltSDL)
            {
                cgltSDL=new CGLTexture();
                cgltSDL->Load(va("%s%cdata%csdl.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltSDL->Draw(80,pClientData->ScreenHeight-96,80+128,pClientData->ScreenHeight+22,255,255,255,255,255,255);
            }

            if(!cgltFMOD)
            {
                cgltFMOD=new CGLTexture();
                cgltFMOD->Load(va("%s%cdata%cfmod.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltFMOD->Draw(218,pClientData->ScreenHeight-74,282,pClientData->ScreenHeight-10,255,255,255,255,255,255);
            }

            if(!cgltOpenGL)
            {
                cgltOpenGL=new CGLTexture();
                cgltOpenGL->Load(va("%s%cdata%copengl.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltOpenGL->Draw(292,pClientData->ScreenHeight-108,292+128,pClientData->ScreenHeight+28,255,255,255,255,255,255);
            }

			if(!cgltBit4ge)
			{
				cgltBit4ge=new CGLTexture();
				cgltBit4ge->Load(va("%s%cdata%cdlstorm.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
			}
			else
			{
				cgltBit4ge->Draw(512,pClientData->ScreenHeight-168,768,pClientData->ScreenHeight+88,255,255,255,255,255,255);
				//DrawBit4ge(10,pClientData->ScreenHeight-74,74,pClientData->ScreenHeight-10,0);
			}

        }
    }
		*/
}

/****************************************************************************************************/

void ToggleFullScreen(void)
{
	if(pClientData->bFullScreen==false) pClientData->bFullScreen=true;
	else pClientData->bFullScreen=false;
#ifdef _WIN32
 	ShutDownGFX();
 	InitializeGFX(pClientData->bFullScreen);
#else
	if(!pScreen)  { Log("FATAL ERROR: no pScreen!"); return; }
	if(SDL_WM_ToggleFullScreen(pScreen)!=1)
		Log("Unable to toggle fullscreen: [%s]\n", SDL_GetError() );
#endif
}

/****************************************************************************************************/

void SetScreenRes(int x,int y,int cl)
{
	ShutDownGFX();
 	InitializeGFX(pClientData->bFullScreen);
}

/****************************************************************************************************/

bool InitGeneric()
{
    DLog("Begin Generic init..."); Generic=new CGLTexture();
    if(!Generic) { DLog("Generic initialization failure..."); return false; }
    DLog("Generic initialized..."); return 1;
}

/****************************************************************************************************/

bool LoadGeneric(char *szFilename)
{
    DLog("Begin Generic load...");
    DestroyGeneric(); if(!InitGeneric()) return false;
    Generic->Load(szFilename,0);
    if(Generic->Loaded()) DLog("Generic loaded...");
    else                  DLog("Generic load failure...");
    strcpy(pClientData->szGeneric,szFilename);
    return 1;
}

/****************************************************************************************************/

bool DestroyGeneric(void)
{
    DLog("Begin Generic destroy..."); DEL(Generic);
    DLog("Generic destroyed..."); return 1;
}

/****************************************************************************************************/

bool InitConsole(void)
{
    DLog("Begin Console init..."); Console=new CGLTexture();
    if(!Console) { DLog("Console initialization failure..."); return false; }
    DLog("Console initialized...");    
    return 1;
}

/****************************************************************************************************/

bool LoadConsole(char *fname)
{
    DLog("Begin Console load..."); DestroyConsole();
    if(!InitConsole()) return false;
    Console->Transparent(1); Console->Load(fname,1);
    if(Console->Loaded()) DLog("Console [%s] loaded...",fname);
    else {                DLog("Console [%s] load failure...",fname); return false; }
    strcpy(pClientData->ConsoleBMP,fname);
    return TOBOOL(Console->bmap);
}

/****************************************************************************************************/

bool DestroyConsole(void)
{
	DLog("Begin Console destroy...");
	DEL(Console);
	DLog("Console destroyed...");
	return 1;
}

/****************************************************************************************************/

bool InitFonts()
{
    DLog("Begin Fonts init...");
    font=0;
    font=new CGLFontList[MAX_FONTS];
    if(!font)
    {
       DLog("Fonts initialization failure...");
        return false;
    }
    for(int i=0;i<MAX_FONTS;i++)
        font[i].ob=0;
    DLog("Fonts initialized...");
    return 1;
}

/****************************************************************************************************/

bool LoadFonts(void)
{
    DLog("Begin Fonts load...");
    DestroyFonts();
    if(!InitFonts()) return false;
    bool loaded=1;
    for(int i=0;i<MAX_FONTS;i++)
    {
        if(!font[i].ob)
            font[i].ob=new CGLFont();
        if(font[i].ob)
            if(!font[i].ob->Load(va("%s%cmedia%cstandard%cgfx%cfonts%cfnt%02d",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP,i)))
                loaded=0;
    }
    if(font[2].ob) { font[2].ob->r=255; font[2].ob->g=255; font[2].ob->b=255; }
    if(loaded) DLog("Fonts loaded...");
    else       DLog("Fonts load failure...");
    return loaded;
}

/****************************************************************************************************/

bool DestroyFonts(void)
{
    DLog("Begin Fonts destroy...");
    for(int i=0;i<MAX_FONTS;i++) DEL(font[i].ob);
    delete [] font; font=0;
    DLog("Fonts destroyed...");
    return 1;
}

/****************************************************************************************************/ // MOUSE

bool InitMouse()
{
    DLog("Begin Mouse init..."); mouse=new CGLMouseList[MAX_MOUSE_CURSORS];
    if(!mouse) { DLog("Mouse initialization failure..."); return false; }
    for(int i=0;i<MAX_MOUSE_CURSORS;i++) mouse[i].cursor=0;
    DLog("Mouse initialized..."); return 1;
}

/****************************************************************************************************/

bool LoadMouse(char *path)
{
    DLog("Begin Mouse load..."); DestroyMouse(); if(!InitMouse()) return false;
    if(!strlen(path)) sprintf(path,"%s%cmedia%cstandard%cgfx%cmouse",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP);
    for(int i=0;i<MAX_MOUSE_CURSORS;i++) { DEL(mouse[i].cursor); mouse[i].cursor=new CGLMouse(va("%s%c%d",path,PATH_SEP,i)); }
    DLog("Mouse loaded..."); return 1;
}

/****************************************************************************************************/

bool DestroyMouse(void)
{
    DLog("Begin Mouse destroy...");
    for(int i=0;i<MAX_MOUSE_CURSORS;i++) DEL(mouse[i].cursor);
    delete [] mouse; mouse=0;
    DLog("Mouse destroyed..."); return 1;
}

/****************************************************************************************************/ // BASE GFX

bool InitBaseGFX()
{
    DLog("Begin BaseTexture init...");
    BaseTexture=0; BaseTexture=new CGLTextureList[MAX_BASE_GFX];
    if(!BaseTexture) { DLog("BaseTexture initialization failure..."); return false; }
    for(int i=0;i<MAX_BASE_GFX;i++) BaseTexture[i].texture=0;
    DLog("BaseTexture initialized..."); return 1;
}

/****************************************************************************************************/

bool LoadBaseGFX(void) // Load in GFX Base
{
    DLog("Begin BaseTexture load...");
    DestroyBaseGFX(); if(!InitBaseGFX()) { DLog("BaseTexture load failure..."); return false; }
    for(int i=0;i<MAX_BASE_GFX;i++) Load1BaseGFX(i); DLog("BaseTexture loaded..."); return 1;
}

/****************************************************************************************************/

bool Load1BaseGFX(int which)
{
    char fname[1024]; char path[1024];
    sprintf(path,"%s%cmedia%cstandard%cgfx%cbase",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP);
    sprintf(fname,"%s%cb%04d.bmp",path,PATH_SEP,which);
    DEL(BaseTexture[which].texture);
    BaseTexture[which].texture=new CGLTexture();
    if(!BaseTexture[which].texture) return false;
    BaseTexture[which].texture->usemask=true;
    BaseTexture[which].texture->Load(fname,1);
    strcpy(BaseTexture[which].texture->name,va("%d",which));
    return 1;
}

/****************************************************************************************************/

CGLTexture *GetBaseGFX(char *name)
{
    if(!BaseTexture) return false;
    for(int i=0;i<MAX_BASE_GFX;i++)
        if(BaseTexture[i].texture)
            if(dlcs_strcasecmp(BaseTexture[i].texture->name,name))
                return BaseTexture[i].texture;
    return false;
}

/****************************************************************************************************/

bool DestroyBaseGFX(void)
{
    DLog("Begin BaseTexture destroy...");
    for(int i=0;i<MAX_BASE_GFX;i++) DEL(BaseTexture[i].texture);
    delete [] BaseTexture; BaseTexture=0;
    DLog("BaseTexture destroyed..."); return false;
}

/****************************************************************************************************/ // MAP GFX

bool InitMapGFX()
{
    DLog("Begin MapGFX init...");
    Default_MapTile=0; Map_Texture=0; SunTex=0;
    Map_Texture=new CGLTextureList[BANKMAX*BANKMAX+BANKMAX];
    if(!Map_Texture) return false; Obj_Texture=0;
    Obj_Texture=new CGLTextureList[BANKMAX*BANKMAX+BANKMAX];
    if(!Obj_Texture) return false;
    for(int i=0;i<BANKMAX*BANKMAX+BANKMAX;i++) { Map_Texture[i].texture = 0; Obj_Texture[i].texture = 0; }
    DLog("MapGFX initialized..."); return 1;
}

/****************************************************************************************************/

bool LoadMapGFX(char *szPath) // Load in map textures
{
	u_char cTile,cBank;
    DLog("Begin MapGFX load from [%s]",szPath);
    DestroyMapGFX(); if(!InitMapGFX()) return false;
    if(!Default_MapTile)
    {
        DLog("%s%cgfx%ctiles%cdefault.bmp",szPath,PATH_SEP,PATH_SEP,PATH_SEP);
        Default_MapTile=new CGLTexture();
        Default_MapTile->Load(va("%s%cgfx%ctiles%cdefault.bmp",szPath,PATH_SEP,PATH_SEP,PATH_SEP),0);
    }
	if(!Default_MapTile) { DLog("Default_Maptile=0"); return false; }

	if(!SunTex)
	{
        DLog("%s%cgfx%csun.bmp",szPath,PATH_SEP,PATH_SEP);
        SunTex=new CGLTexture();
        SunTex->Load(va("%s%cgfx%csun.bmp",szPath,PATH_SEP,PATH_SEP),0);
	}    

    for(cBank=0;cBank<8;cBank++) for(cTile=0;cTile<64;cTile++) Load1MapGFX(cBank,cTile,szPath);
    DLog("MapGFX loaded..."); return 1;
}

/****************************************************************************************************/

bool Load1MapGFXdt(int dt,char *szPath)
{
	return false;
}

/****************************************************************************************************/

bool Load1MapGFX(u_char cBank,u_char cTile,char *szPath)
{
    if(!Map_Texture) if(!InitMapGFX()) return false;
    DEL(Map_Texture[cBank*BANKMAX+cTile].texture); Map_Texture[cBank*BANKMAX+cTile].texture=new CGLTexture();
    if(!Map_Texture[cBank*BANKMAX+cTile].texture->Load(va("%s%cgfx%ctiles%ct%03d-%03d.bmp",szPath,PATH_SEP,PATH_SEP,PATH_SEP,cBank,cTile),0)) return true;
    DEL(Map_Texture[cBank*BANKMAX+cTile].texture); return false;
}

/****************************************************************************************************/

bool DestroyMapGFX(void)
{
	DLog("Begin MapGFX destroy...");
	for(int i=0;i<BANKMAX*BANKMAX+BANKMAX;i++)
	{
		DEL(Map_Texture[i].texture);
		DEL(Obj_Texture[i].texture);
	}
	delete [] Map_Texture;
	Map_Texture=0;
	delete [] Obj_Texture;
	Obj_Texture=0;
	DEL(Default_MapTile);
	DEL(SunTex);
	DLog("MapGFX destroyed...");
	return 1;
}

/****************************************************************************************************/

bool InitButtons()
{
	DLog("Begin ButtonTexture init...");
	ButtonTexture=new CGLTextureList[MAX_BUTTONS];
	if(!ButtonTexture)
	{
		DLog("ButtonTexture initalization failure...");
		return false;
	}
	for(int i=0;i<MAX_BUTTONS;i++)
		ButtonTexture[i].texture=0;
	DLog("ButtonTexture initialized...");
	return 1;
}

/****************************************************************************************************/
bool LoadButtons(char *szFilename){ DLog("Begin ButtonTexture load..."); DestroyButtons();if(!InitButtons()) { DLog("ButtonTexture load failure..."); return false; }char szPath[1024]; strcpy(szPath,pClientData->FMDir);for(int iButton=0;iButton<MAX_BUTTONS;iButton++){ButtonTexture[iButton].texture=new CGLTexture();ButtonTexture[iButton].texture->usemask=1;ButtonTexture[iButton].texture->Load(va("%s%cmedia%cstandard%cgfx%cbuttons%c%03d.bmp",szPath,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP,iButton),0);}DLog("ButtonTexture loaded..."); return 1;}
/****************************************************************************************************/
bool DestroyButtons() { DLog("Begin ButtonTexture destroy..."); for(int i=0;i<MAX_BUTTONS;i++) DEL(ButtonTexture[i].texture); delete [] ButtonTexture; ButtonTexture=0; DLog("ButtonTexure destroyed..."); return 1; }
/****************************************************************************************************/ // MODELS
bool InitModels() { DLog("Begin Models init..."); Model=new CGLModel(); FirstModel=Model; DLog("Models initialized..."); return 1; }
/****************************************************************************************************/
bool LoadModels(void) { int i=0; DLog("Begin Models load..."); Model=FirstModel; if(!Model) Model=new CGLModel(); FirstModel=Model; if(Model) { while(i<MAX_MODELS) { if(Load1Model(i)) { Model->next=new CGLModel(); Model=Model->next; DLog("Found model [%d]",i); } i++; } } DLog("Models loaded..."); return 1; }
/****************************************************************************************************/
bool Load1Model(int i) { if(!Model) return false; if(!Model->Load( va("%s%c%04d%ctris.md2",pClientData->MDDir,PATH_SEP,i,PATH_SEP,PATH_SEP), va("%s%c%04d%cskin.bmp",pClientData->MDDir,PATH_SEP,i,PATH_SEP,PATH_SEP))) return false; strcpy(Model->name,va("%d",i)); return 1; }
/****************************************************************************************************/
CGLModel *GetModel(char *name) { Model=FirstModel; while(Model) { if(atoi(Model->name)==atoi(name)) return Model; Model=Model->next; } return false; }
/****************************************************************************************************/
bool DestroyModels(void){ DLog("Begin Models destroy..."); CGLModel *del; Model=FirstModel; while(Model) { del=Model; Model=Model->next; DEL(del); } DLog("Models destroyed..."); return 1; }
/****************************************************************************************************/ // VISIBLES
bool InitVisibles(void) { DLog("Begin VIS init..."); VIS=0; FirstVIS=0; DLog("VIS initialized..."); return 1; }
/****************************************************************************************************/
C_FM_VIS *GetVis(int sid) { VIS=FirstVIS; while(VIS) { if(VIS->sid==sid) return VIS; VIS=VIS->next; } return false; }
/****************************************************************************************************/
bool PushVis(int sid,int x,int y,int z,char *media,int media_type,int head,int dir)
{
    C_FM_VIS *L_VIS=0;
    L_VIS=GetVis(sid);// check to see if the vis exists...
    if(L_VIS)
    {
        L_VIS->sid=sid;
        L_VIS->x=x;
        L_VIS->y=y;
        L_VIS->z=z;
        strcpy(L_VIS->media,media);
        L_VIS->media_type=media_type;
        L_VIS->heading=head;
        L_VIS->direction=dir;
    }
    else
    {
        L_VIS = new C_FM_VIS;
        if(L_VIS)
        {
            L_VIS->sid=sid;
            L_VIS->x=x;
            L_VIS->y=y;
            L_VIS->z=z;
            strcpy(L_VIS->media,media);
            L_VIS->media_type=media_type;
            L_VIS->heading=head;
            L_VIS->direction=dir;
            InsertVisible(L_VIS);
        }
    }
    Log("VIS [%d] pushed...",sid);
    return 1;
}
/****************************************************************************************************/
bool Pop_Vis(int sid)
{
    // check to see if the vis exists...
    C_FM_VIS *hi=0;
    hi=GetVis(sid);
    if(!hi) { DLog("Pop_Vis(%d) can't pop a vis that does not exist (1)!",sid); return false; }
    C_FM_VIS* last_vis=0;
    VIS=FirstVIS;
    while(VIS)
    {
        if(VIS->sid==sid)
        {
            if(!last_vis)
            {
                FirstVIS=VIS->next;
                VIS->next=0;
                DEL(VIS);
            }
            else
            {
                last_vis->next=VIS->next;
                VIS->next=0;
                DEL(VIS);
            }
            DLog("VIS popped...");
            return 1;
        }
        last_vis=VIS;
        VIS=VIS->next;
    }
    DLog("Pop_Vis(%d) can't pop a vis that does not exist (2)!",sid);
    return false;
}
/****************************************************************************************************/
bool InsertVisible(C_FM_VIS* obj)
{
    if(FirstVIS)
    {
        obj->next=FirstVIS;
        FirstVIS=obj;
    }
    else FirstVIS=obj;

    DLog("VIS inserted...");
    return 1;
}
/****************************************************************************************************/
bool RemoveVisible(C_FM_VIS *obj)
{
    return 1;
}
/****************************************************************************************************/
bool UpdateVisibles(void)
{
    CC_M_VIS_DATA *dvdata;
    int dt=0;
    int px=0;
    int py=0;
    int cx=0;
    int cy=0;
    int hx=0;
    int hy=0;
    for(dt=0;dt<pMapBuffer->SIZE;dt++)
    {
        if(pMapBuffer->VISData[dt].pFirstVISData)
        {
            pMapBuffer->VISData[dt].pVISData=pMapBuffer->VISData[dt].pFirstVISData;
            while(pMapBuffer->VISData[dt].pVISData)
            {
                dvdata=pMapBuffer->VISData[dt].pVISData;
                pMapBuffer->VISData[dt].pVISData=pMapBuffer->VISData[dt].pVISData->next;
                DEL(dvdata);
            }
            pMapBuffer->VISData[dt].pVISData=0;
            pMapBuffer->VISData[dt].pFirstVISData=0;
        }
    }
    VIS=FirstVIS;
    while(VIS)
    {
        // define media to use
        switch(VIS->media_type)
        {
            case FM_VIS_MODEL:
                VIS->model=0;
                VIS->texture=0;
                VIS->model=GetModel(VIS->media);

                if(VIS->path_get())
                {
                    if(VIS->model)
                    {
                        if(VIS->moving)
                        {
                            if(VIS->stopped)
                            {
                                VIS->model->Model->currentAnim=1;
                                tAnimationInfo *pAnim = &(VIS->model->Model->pAnimations[VIS->model->Model->currentAnim]);
                                VIS->model->Model->currentFrame=pAnim->startFrame;
                                VIS->moving=true;
                                VIS->stopped=false;
                            }

                            // move the thing...

                            VIS->direction=5;

                            if(VIS->x<VIS->path_get()->x) { VIS->mx+=4; VIS->direction+=1; }
                            if(VIS->x>VIS->path_get()->x) { VIS->mx-=4; VIS->direction-=1; }
                            if(VIS->y<VIS->path_get()->y) { VIS->my+=4; VIS->direction+=3; }
                            if(VIS->y>VIS->path_get()->y) { VIS->my-=4; VIS->direction-=3; }

                            VIS->heading=VIS->direction;

                            if(VIS->mx>64)
                            {
                                //cx=camera->x;
                                VIS->mx=0;
                                VIS->x++;
                                camera->mx=VIS->mx;
                                camera->my=VIS->my;
                                camera->x=VIS->x;
                                camera->y=VIS->y;
                                //if(MapCoord(cx)<MapCoord(camera->x))
                                    LoadMap(camera->x,camera->y,camera->z);
                            }
                            if(VIS->mx<-64)
                            {
                                //cx=camera->x;
                                VIS->mx=0;
                                VIS->x--;
                                camera->mx=VIS->mx;
                                camera->my=VIS->my;
                                camera->x=VIS->x;
                                camera->y=VIS->y;
                                //if(MapCoord(cx)>MapCoord(camera->x))
                                    LoadMap(camera->x,camera->y,camera->z);
                            }
                            if(VIS->my>64)
                            {
                                //cy=camera->y;
                                VIS->my=0;
                                VIS->y++;
                                camera->mx=VIS->mx;
                                camera->my=VIS->my;
                                camera->x=VIS->x;
                                camera->y=VIS->y;
                                //if(MapCoord(cy)<MapCoord(camera->y))
                                    LoadMap(camera->x,camera->y,camera->z);
                            }
                            if(VIS->my<-64)
                            {
                                //cy=camera->y;
                                VIS->my=0;
                                VIS->y--;
                                camera->mx=VIS->mx;
                                camera->my=VIS->my;
                                camera->x=VIS->x;
                                camera->y=VIS->y;
                                //if(MapCoord(cy)>MapCoord(camera->y))
                                    LoadMap(camera->x,camera->y,camera->z);
                            }

                            camera->mx=VIS->mx;
                            camera->my=VIS->my;
                            camera->x=VIS->x;
                            camera->y=VIS->y;
                            
                            if( (VIS->x==VIS->path_get()->x) &&
                                (VIS->y==VIS->path_get()->y) )
                                VIS->path_pop();

                            //if(camera->lock_object ==VIS->sid)
                            //{
                            //}

                            

                            // check here if it has reached destination


                        }
                    }
                }

                if(!VIS->path_get())
                {
                    if((VIS->moving)||(!VIS->stopped))
                    {
                        VIS->model->Model->currentAnim=0;
                        VIS->model->Model->currentFrame=0;
                        VIS->stopped=true;
                        VIS->moving=false;
                    }
                }
                break;

            case FM_VIS_SPRITE:
                VIS->model=0;
                VIS->texture=0;
                VIS->texture=GetBaseGFX(VIS->media);
                break;

            case FM_VIS_MULTI_SPRITE:
                break;

            default:
                break;
        }

        if(VIS->in(camera->x-30,camera->y-30,camera->z,camera->x+30,camera->y+30,camera->z))
        {
            dt=30+CamCoord(VIS->x)+((30+CamCoord(VIS->y))*pMapBuffer->Width);
            if((dt>0)&&(dt<(pMapBuffer->SIZE-1)))
            {
                if(pMapBuffer->VISData[dt].pFirstVISData)
                {
                    pMapBuffer->VISData[dt].pVISData=pMapBuffer->VISData[dt].pFirstVISData;
                    while(pMapBuffer->VISData[dt].pVISData->next)
                    {
                        pMapBuffer->VISData[dt].pVISData=pMapBuffer->VISData[dt].pVISData->next;
                    }
                    pMapBuffer->VISData[dt].pVISData->next=new CC_M_VIS_DATA;
                    pMapBuffer->VISData[dt].pVISData->next->pVIS=VIS;
                }
                else
                {
                    pMapBuffer->VISData[dt].pFirstVISData=new CC_M_VIS_DATA;
                    pMapBuffer->VISData[dt].pFirstVISData->pVIS=VIS;
                }
            }
        }
        // define mouse rect
        VIS=VIS->next;
    }
    return 1;
}
/****************************************************************************************************/
bool DestroyVisibles(void)
{
    DLog("Begin VIS destroy...");

    C_FM_VIS *del_vis;
    VIS=FirstVIS;
    while(VIS)
    {
        del_vis=VIS;
        VIS=VIS->next;
        DEL(del_vis);
    }
    DLog("VIS destroyed...");
    return 1;
}
/****************************************************************************************************/
bool FlashCursor(int xi, int yi) // Get cursor time
{
    static bool bCursor;
    static unsigned long dwCursorTime=dlcs_get_tickcount();
    if((dlcs_get_tickcount()-dwCursorTime)>400)
    {
        dwCursorTime=dlcs_get_tickcount();
        if(bCursor) bCursor=false;
        else        bCursor=true;
    }
    return bCursor;
}
/****************************************************************************************************/
//void WriteTextPct(float fX, float fY, char *szText,int fnt)
//{
//	WriteText(  pClientData->ScreenWidth*fX,
//				pClientData->ScreenHeight*fY,
//				szText,
//				fnt);
//}
void WriteText(int iX,int iY,char *szText,int fnt)
{
	//float fx=(float)((float)iX/800)*(float)pClientData->ScreenWidth;
	//float fy=(float)((float)iY/600)*(float)pClientData->ScreenHeight;
	if(fnt>MAX_FONTS) return; if(!font[fnt].ob)
	{
		LoadFonts();
		return;
	}
	font[fnt].ob->Print(iX,iY,szText,0); //fx,fy,szText,0); //
}
/****************************************************************************************************/
void RenderScene(void) // Render the game scene Frame
{
    int x,y ; //,px,py,
	int i,dt; //,dx,dy;
    FM_VERTEX pMapTileVertex[4];
    CC_M_VIS_DATA *dVIS=0;
    float dirr;
	static float fLiquid;

	//float wr,wg,wb,xr,xg,xb,yr,yg,yb;

    /*
                 23          56          8
     0           90          90          9
    0*************************************
     *           *           *           *
     *           *           *           *
     *           *           *           *
     *   NORTH   *   NORTH   *   NORTH   *
     *   WEST    *           *    EAST   *
     *           *           *           *
     *           *           *           *  pMapBuffer:
   29*************************************
   30*           *           *           *  This is the configuration for
     *           *           *           *  the map. The CENTER is where
     *           *           *           *  the RenderScene() function
     *   WEST    *     X     *    EAST   *  draws from. The other map 
     *           *  (45,45)  *           *  quadrants are buffer zones 
     *           *           *           *  that the center uses for 
     *           *           *           *  loading it's data when the 
   59*************************************  player walks into them. 
   60*           *           *           *  
     *           *           *           *
     *           *           *           *
     *   SOUTH   *   SOUTH   *   SOUTH   *
     *   WEST    *           *    EAST   *
     *           *           *           *
     *           *           *           *
   89*************************************/

    if(!pMapBuffer)  return;
    if(!Map_Texture) return;
    if(!camera)      return;

    UpdateVisibles();

	static float lxp=0; lxp=lxp+.000021; if(lxp>360) lxp=0;
	static float lyp=0; lyp=lyp+.000021; if(lyp>360) lyp=0;
	static float lzp=0; lzp=lzp+.000021; if(lyp>360) lyp=0;

	static float sxp;
	static float syp;
	static float szp;

	sxp=(sin(lxp)/12)*1232;
	syp=(cos(lyp)/132)*1232;
	szp=(cos(lzp)/12)*1232;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRotatef(camera->rz,1.0f,0,0);
	glRotatef(camera->ry,0,1.0f,0);
	glRotatef(camera->rx,1.0f,0,0);

	glTranslatef(camera->x,camera->y,camera->z);

	glTranslatef(syp,sxp,szp);

	glDisable(GL_BLEND);

	//glEnable(GL_TEXTURE_2D);
    //if(SunTex) glBindTexture(GL_TEXTURE_2D, SunTex->bmap);
		
	drawsphere(3,3.0f,1.0f,1.0f,.40f);

	glTranslatef(-syp,-sxp,-szp);

	//glTranslatef(0,0,10.0f);

	//glTranslatef(camera->ux,camera->uy,camera->uz);

	/*
    LightAmbient[0]=1.8f;
    LightAmbient[1]=0.8f;
    LightAmbient[2]=1.8f;
    LightAmbient[3]=0.0f;

    LightDiffuse[0]=1.5f;
    LightDiffuse[1]=1.5f;
    LightDiffuse[2]=1.5f;
    LightDiffuse[3]=0.0f;

	static float lxp=0;
	lxp=lxp+.721; if(lxp>222) lxp=0;

	static float lyp=0;
	lyp=lyp+.21; if(lyp>222) lyp=0;

    LightPosition[0]=lxp;
    LightPosition[1]=lyp;
    LightPosition[2]=100 ;
    LightPosition[3]=0.0f;

    LightDir[0]=lxp;
    LightDir[1]=lyp;
    LightDir[2]=0.0f;

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		//  Setup The Ambient Light
    glLightfv(GL_LIGHT1,   GL_SPOT_DIRECTION , LightDir); //GL_SPOT_EXPONENT , LightDir);   
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		//  Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHTING);


/*
	GLuint	buffer[512];										// Set Up A Selection Buffer
	GLint	hits;												// The Number Of Objects That We Selected

	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
	GLint	viewport[4];

	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// Tell OpenGL To Use Our Array For Selection

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	(void) glRenderMode(GL_SELECT);

	glInitNames();												// Initializes The Name Stack
	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
	glPushMatrix();												// Push The Projection Matrix
	glLoadIdentity();											// Resets The Matrix

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]-mouse_y), 1.0f, 1.0f, viewport);

	// Apply The Perspective Matrix
	gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	DrawTargets();												// Render The Targets To The Selection Buffer
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	hits=glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
																// Objects Were Drawn Where The Mouse Was
	if (hits > 0)												// If There Were More Than 0 Hits
	{
		int	choose = buffer[3];									// Make Our Selection The First Object
		int depth = buffer[1];									// Store How Far Away It Is 

		for (int loop = 1; loop < hits; loop++)					// Loop Through All The Detected Hits
		{
			// If This Object Is Closer To Us Than The One We Have Selected
			if (buffer[loop*4+1] < GLuint(depth))
			{
				choose = buffer[loop*4+3];						// Select The Closer Object
				depth = buffer[loop*4+1];						// Store How Far Away It Is
			}       
		}

		if (!object[choose].hit)								// If The Object Hasn't Already Been Hit
		{
			object[choose].hit=TRUE;							// Mark The Object As Being Hit
			score+=1;											// Increase Score
			kills+=1;											// Increase Level Kills
			if (kills>level*5)									// New Level Yet?
			{
				miss=0;											// Misses Reset Back To Zero
				kills=0;										// Reset Level Kills
				level+=1;										// Increase Level
				if (level>30)									// Higher Than 30?
					level=30;									// Set Level To 30 (Are You A God?)
			}
		}
    }
	*/

    ///////////////////////////////////////////////////////////////////////////////////
    // Stage 1 : Draw map tiles
              


    if(pClientData->bDrawMap)
    {
        for(y=(GMP_MAPSIZE*3);y>-1;y--)
        {
            for(x=0;x<(GMP_MAPSIZE*3);x++)
            {
                dt=((GMP_MAPSIZE*3)-x)+((GMP_MAPSIZE*3)-y)*pMapBuffer->Width;
                if((dt<0)||(dt>(pMapBuffer->SIZE))) continue;
                else
                {
                    //dx=64*x; dy=64*y;
                    ////////////////////////////////////////////////////////////////////
                    for(i=0;i<4;i++)
                    {
                        pMapTileVertex[i].Height = pMapBuffer->pMapData[dt].Vertex[i].Height;
                        pMapTileVertex[i].Width  = pMapBuffer->pMapData[dt].Vertex[i].Width;

                        pMapTileVertex[i].R = (pMapBuffer->pMapData[dt].Vertex[i].R);// + camera->r)/2;
                        pMapTileVertex[i].G = (pMapBuffer->pMapData[dt].Vertex[i].G);// + camera->g)/2;
                        pMapTileVertex[i].B = (pMapBuffer->pMapData[dt].Vertex[i].B);// + camera->b)/2;
                    }

					/*
                    ////////////////////////////////////////////////////////////////////
                    if( ((pClientData->Mode == BUILD) || (pClientData->Mode == BUILD_GHOST)) &&
                        ( pClientData->bBlockGlow ) ) // Fade blocked tiles red (build mode only)
                    {
                        if(pMapBuffer->GetProperty(dt,GMP_PROPERTY_BLOCKED)) //>bIsBlocked(dt))
                        {
                            for(i=0;i<4;i++)
                            {
                                pMapTileVertex[i].R = GetFade(2);
                                pMapTileVertex[i].G = 0;
                                pMapTileVertex[i].B = 0;
                            }
                        }
                    }
					

                    if(pMapBuffer->GetProperty(dt,GMP_PROPERTY_TELEPORT)) //>bIsTeleport(dt)) // Fade teleports blue
                    {
                        for(i=0;i<4;i++)
                        {
                            pMapTileVertex[i].R = 0;
                            pMapTileVertex[i].G = 100;
                            pMapTileVertex[i].B = float((float)GetFade(3)/255.0f);
                        }
                    }
					
                    fLiquid += 0.0005f; if(fLiquid>=16384.0f) fLiquid = 0.0f;
                    if(pMapBuffer->GetProperty(dt,GMP_PROPERTY_LIQUID))
                    {
                        // draw something under the liquid such as a sandy area here
                        pMapTileVertex[0].Height += (char)(sin((pMapTileVertex[0].Height+fLiquid)/422) * 3);
                        pMapTileVertex[0].Width  += (char)(sin((pMapTileVertex[0].Width)+fLiquid/32)  * 2);
                        pMapTileVertex[1].Height += (char)(sin((pMapTileVertex[1].Height+fLiquid)/422) * 3);
                        pMapTileVertex[1].Width  += (char)(sin((pMapTileVertex[1].Width)+fLiquid/32)  * 2);
                        pMapTileVertex[2].Height += (char)(sin((pMapTileVertex[2].Height+fLiquid)/422) * 3);
                        pMapTileVertex[2].Width  += (char)(sin((pMapTileVertex[2].Width)+fLiquid/32)  * 2);
                        pMapTileVertex[3].Height += (char)(sin((pMapTileVertex[3].Height+fLiquid)/422) * 3);
                        pMapTileVertex[3].Width  += (char)(sin((pMapTileVertex[3].Width)+fLiquid/32)  * 2);
                    }
					*/

                    ////////////////////////////////////////////////////////////////////

					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glDisable(GL_BLEND);
                    
					glTranslated(x-45,y-45,-10);
                    
					glEnable(GL_TEXTURE_2D);

                    if(Default_MapTile) glBindTexture(GL_TEXTURE_2D, Default_MapTile->bmap);
                    if(Map_Texture)
						if(Map_Texture[pMapBuffer->GetTile(dt)].texture)
							if(Map_Texture[pMapBuffer->GetTile(dt)].texture->bmap)
								glBindTexture(GL_TEXTURE_2D, Map_Texture[pMapBuffer->GetTile(dt)].texture->bmap);

                    glBegin(GL_QUADS);

                    glColor3f(  pMapTileVertex[0].R, pMapTileVertex[0].G, pMapTileVertex[0].B);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f( pMapTileVertex[0].Width,  0.0f , pMapTileVertex[0].Height);

                    glColor3f(pMapTileVertex[1].R,pMapTileVertex[1].G,pMapTileVertex[1].B);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f( 1.0f +pMapTileVertex[1].Width, 0.0f , pMapTileVertex[1].Height);

                    glColor3f(pMapTileVertex[2].R,pMapTileVertex[2].G,pMapTileVertex[2].B);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f( 1.0f+pMapTileVertex[2].Width, 1.0f, pMapTileVertex[2].Height);

                    glColor3f(pMapTileVertex[3].R,pMapTileVertex[3].G,pMapTileVertex[3].B);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f( pMapTileVertex[3].Width,   1.0f, pMapTileVertex[3].Height);

                    glEnd();

					glMatrixMode(GL_MODELVIEW);
					glPopMatrix();
                }
            }
        }
    }




    ///////////////////////////////////////////////////////////////////////////////////
    // Stage 2 : Draw Map Objects



    ///////////////////////////////////////////////////////////////////////////////////
    // Stage 3 : Draw world VIS

    for(y=(GMP_MAPSIZE*3);y>-1;y--)
    {
        for(x=0;x<(GMP_MAPSIZE*3);x++)
        {
            dt = ((GMP_MAPSIZE*3)-x)+((GMP_MAPSIZE*3)-y)*pMapBuffer->Width;
            if( (dt<0) || (dt>(pMapBuffer->SIZE-1)) ) continue;
            else
            {
                if(pMapBuffer->VISData[dt].pFirstVISData)
                {
                    dVIS=pMapBuffer->VISData[dt].pFirstVISData;
                    while(dVIS)
                    {
                        if(dVIS->pVIS)
                        if(dVIS->pVIS->model)
                        {
                           // LightPosition[0]=-dx-32+dVIS->pVIS->mx;
                           // LightPosition[1]=-dy-32+dVIS->pVIS->my;
                           // LightDir[0]=-dx-32+dVIS->pVIS->mx;
                           // LightDir[1]=-dy-32+dVIS->pVIS->my;
                           // glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
                           // glEnable(GL_LIGHT1);

                            switch(dVIS->pVIS->heading)
                            {
                                case 3:
                                    dirr=270.0f;
                                    break;
                                case 4:
                                    dirr=180.0f;
                                    break;
                                case 6:
                                    dirr=135.0f;
                                    break;
                                case 7:
                                    dirr=225.0f;
                                    break;
                                case 9:
                                    dirr=90.0f;
                                    break;
                                case 8:
                                    dirr=315.0f;
                                    break;
                                case 1:
                                    dirr=0.0f;
                                    break;
                                case 2:
                                    dirr=45.0f;
                                    break;
                                default:
                                    dirr=0.0f;
                                    break;                                    
                            }
							/*
                            dVIS->pVIS->model->Rotate(270,dirr,0);
                            dVIS->pVIS->model->Scale(12.7f,12.7f,12.7f);
                            dVIS->pVIS->model->Locate(  -x-45+dVIS->pVIS->mx,
                                                        -y-45+dVIS->pVIS->my,
                                                        -40 +
                                                         ( (pMapBuffer->GetVertexHeight(dt,0)+
                                                            pMapBuffer->GetVertexHeight(dt,1)+
                                                            pMapBuffer->GetVertexHeight(dt,2)+
                                                            pMapBuffer->GetVertexHeight(dt,3))/4.0f)
                                                        );*/
                            //dVIS->pVIS->model->Color((float) dVIS->pVIS->r, (float) dVIS->pVIS->g, (float) dVIS->pVIS->b);

							glMatrixMode(GL_MODELVIEW);
							glPushMatrix();
							glLoadIdentity();
							glDisable(GL_BLEND);

							//glTranslated(x-43,y-43,-10);
                    
                            dVIS->pVIS->model->RenderSceneDraw();
							
							glMatrixMode(GL_MODELVIEW);
							glPopMatrix();


                        }
                        if(dVIS->pVIS->texture)
                        {
                            //dVIS->pVIS->texture->Draw(dx-32,pClientData->ScreenHeight-dy-32,dx+32,pClientData->ScreenHeight-dy+32,255,255,255);
                        }
                        dVIS=dVIS->next;
                    }
                }
            }            
        }
    }



    ///////////////////////////////////////////////////////////////////////////////////
    // Stage 5 : Draw Screen Text


    ///////////////////////////////////////////////////////////////////////////////////
    // Stage 6 : Draw Item Names


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

//WriteText(0,400,va("LP[%04.2f][%04.2f][%04.2f][%04.2f] LD[%04.2f][%04.2f][%04.2f] GMP[%d]",LightPosition[0],LightPosition[1],LightPosition[2],LightPosition[3],LightDir[0],LightDir[1],LightDir[2],GMP_MAPSIZE),5);
//if(camera->z>0) camera->z=0; 
    WriteText(0,420, va("PRNx[%04.2f] PRNy[%04.2f] PRNz[%04.2f] ",PickRayNear.x,PickRayNear.y,PickRayNear.z ),5);
    WriteText(0,440, va("PRFx[%04.2f] PRFy[%04.2f] PRFz[%04.2f] ",PickRayFar.x,PickRayFar.y,PickRayFar.z),5);
	WriteText(0,460, va("CX  [%04.2f] CY  [%04.2f] CZ  [%04.2f]",camera->x,camera->y,camera->z),5);
	WriteText(0,480, va("CPX [%04.2f] CPY [%04.2f] CPZ [%04.2f]",camera->px,camera->py,camera->pz),5);
    WriteText(0,500, va("CVX [%04.2f] CVY [%04.2f] CVZ [%04.2f]",camera->vx,camera->vy,camera->vz),5);
	WriteText(0,520, va("CUX [%04.2f] CUY [%04.2f] CUZ [%04.2f]",camera->ux,camera->uy,camera->uz),5);
	WriteText(0,540, va("CRX [%04.2f] CRY [%04.2f] CRZ [%04.2f]",camera->rx,camera->ry,camera->rz),5);
//	WriteText(0,560, va("LXP [%04.2f] LYP [%04.2f]",lxp,lyp),5);
//  WriteText(0,540, va("CZOOM[%04.2f]",camera->zoom),5);

}
/****************************************************************************************************/
void DrawVisuals(void)
{
    VIS=FirstVIS;
    while(VIS)
    {
        if(VIS->visible==FM_VIS_VISIBLE)
        {
            switch(VIS->type)
            {
                case FM_VIS_MODEL:

                    VIS->model->Rotate(0,22,0);
                    VIS->model->Scale(2.0f,2.0f,2.0f);
                    VIS->model->Locate(160,-90,0);
                    VIS->model->Draw();
                    break;                

                case FM_VIS_SPRITE:
                   
                    VIS->texture->Draw(10,10,74,74,255,255,255);

                    break;

                case FM_VIS_MULTI_SPRITE:

                    break;
            
                default:
                    break;
            }
        }
        VIS=VIS->next;
    }
}
/****************************************************************************************************/
void DrawRectangle(int iX,int iY,int iX2,int iY2,long color)
{
    DrawBar(iX,iY,iX+1,iY2,color,color);
    DrawBar(iX,iY,iX2,iY+1,color,color);
    DrawBar(iX2,iY,iX2+1,iY2,color,color);
    DrawBar(iX,iY2,iX2,iY2+1,color,color);
}
/****************************************************************************************************/
void DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2)
{
    iY=pClientData->ScreenHeight-iY;
    iY2=pClientData->ScreenHeight-iY2;
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
/****************************************************************************************************/
void DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2)
{
    iY=pClientData->ScreenHeight-iY;
    iY2=pClientData->ScreenHeight-iY2;
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
/****************************************************************************************************/
void DrawStatBar(int iX,int iY,int iW, int iT,int iV) // iT is total value, iV is current value, will draw percentage bar
{
    DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    DrawBar(iX,iY,iX+(int)fPercent,iY+3,LONGRGB(0,100,0),LONGRGB(0,100,0));
}
/****************************************************************************************************/
void DrawCStatBar(int iX,int iY,int iW, int iT,int iV,long Color1,long Color2) // iT is total value, iV is current value, will draw percentage bar
{
    DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    DrawBar(iX,iY,iX+(int)fPercent,iY+3,Color1,Color2);
}
/****************************************************************************************************/
void DrawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,u_char r, u_char g, u_char b) // Draw a GUI resource at x,y
{
    if(!ButtonTexture)
    {
        LoadButtons(" ");
        return;
    }
    if(!ButtonTexture[which].texture)
    {
        LoadButtons(" ");
        return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
        LoadButtons(" ");
        return;
    }
    ButtonTexture[which].texture->Draw(iX,iY,iX2,iY2,r,g,b);
}
/****************************************************************************************************/
void DrawButton(int which, int updown, int x, int y) // Draw a button at x,y in updown state
{
    if(!ButtonTexture)
    {
        LoadButtons(" ");
        return;
    }
    if(!ButtonTexture[which].texture)
    {
        LoadButtons(" ");
        return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
        LoadButtons(" ");
        return;
    }
    ButtonTexture[which].texture->Draw(x,y,x+64,y+64,255,255,255);
}

/****************************************************************************************************/
void DrawGUIButton(int x, int y, int which, int size) // Draw a button at x,y in updown state
{
    if(!ButtonTexture)
    {
        LoadButtons(" ");
        return;
    }
    if(!ButtonTexture[which].texture)
    {
        LoadButtons(" ");
        return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
        LoadButtons(" ");
        return;
    }

    ButtonTexture[which].texture->Draw(x,y,x+size,y+size,255,255,255);
}

/****************************************************************************************************/
void DrawGenericSurface(void)
{
    if(!Generic) LoadGeneric(pClientData->szGeneric);
    if(!Generic) return;
    if(!Generic->bmap) LoadGeneric(pClientData->szGeneric);
    if(!Generic) return;
    if(!Generic->bmap) return;
    //float butt=((float)pClientData->ScreenHeight)/((float)pClientData->ScreenWidth);
    //float height=(((float)pClientData->ScreenWidth)-((float)pClientData->ScreenWidth*butt));
    Generic->Draw(0,0,pClientData->ScreenWidth,pClientData->ScreenHeight,255,255,255);
}

/****************************************************************************************************/
void DrawConsoleBMP(int iX1,int iY1,int iX2,int iY2)
{

    if(!Console)
    {
        ProcessConsoleCommand("set consolebmp console.bmp",0);
        return;
    }

    if(!Console->bmap)
    {
        ProcessConsoleCommand("set consolebmp console.bmp",0);
        return;
    }
    Console->Draw(iX1,iY1,iX2,iY2,255,255,255);

}

/****************************************************************************************************/
void DrawOSIcon(int iX,int iY,char *szOS)
{
    switch(szOS[0])
    {
        case 'W': //win32
            WriteText(iX,iY,"U",2);
            break;
        case 'L': //linux
            WriteText(iX,iY,"T",2);
            break;
        case 'F': // freebsd
            WriteText(iX,iY,"S",2);
            break;
        default: // what the?
            WriteText(iX,iY,"?",2);
            break;
    }
}
/****************************************************************************************************/
int DrawSlicedGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y) // Draw a button at x,y in updown state
{
    int j=0;
    int width=CGLFont_StrLen(gui_control->value)*8;

    gui_control->x=gui_control->rect.left + x;
    gui_control->y=gui_control->rect.top  + y;

    DrawGUIButton(gui_control->x,gui_control->y,30,32);
    j+=32;
    while(width>16)
    {
        DrawGUIButton(gui_control->x+j,gui_control->y,31,32);
        j+=32;
        width-=32;
    }
    DrawGUIButton(gui_control->x+j,gui_control->y,32,32);

    gui_control->rect.right=gui_control->rect.left+j+32;
    gui_control->rect.bottom=gui_control->rect.top+32;

    if(MouseIn(gui_control->rect.left+x,gui_control->rect.top+y,gui_control->rect.right+x,gui_control->rect.bottom+y))
    {
        WriteText(gui_control->x+8,gui_control->y+8,va("^+^>ffcc00%s",gui_control->value),1);
    }
	else
    WriteText(gui_control->x+8,gui_control->y+8,va("^>00ccff%s",gui_control->value),1);

    return false;
}
/****************************************************************************************************/
int DrawBaseGFXGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y) // Draw a button at x,y in updown state
{

    int j=0;

    DrawBaseGFX(gui_control->rect.left   +x,
                gui_control->rect.top    +y,
                gui_control->rect.right  +x,
                gui_control->rect.bottom +y,
                atoi(gui_control->media)   ,
                255,255,255 );

    WriteText(gui_control->rect.left+8+x,gui_control->rect.top+8+y,gui_control->value,0);
    if(MouseIn(gui_control->rect.left+x,gui_control->rect.top+y,gui_control->rect.right+x,gui_control->rect.bottom+y))
    {
        WriteText(gui_control->rect.left+8+x,gui_control->rect.top+8+y,va("^g%s",gui_control->value),0);

    }
 
    return false;
}

/****************************************************************************************************/
void DrawMouse(int x,int y,int mode)
{
    if(bTargeting) mode=MP_TARGET;
    if(mode>MAX_MOUSE_CURSORS) mode=MP_NONE;
    mouse[mode].cursor->x=x+mouse[mode].cursor->x_offset;
    mouse[mode].cursor->y=y+mouse[mode].cursor->y_offset;
    mouse[mode].cursor->draw();
}
/****************************************************************************************************/
void DrawGUIs(void) // Draw and check any active GUIs
{
    C_FM_GUI_STUMP      *tstump=0;
    C_FM_GUI_CONTROL    *tctrl=0;

    float FONT_CHAR_WIDTH=8.5;
    
    int x,y,x2,y2;
    int j,k;
    char temp[1024]; memset(temp,0,1024);

    if(!gui) return;

	DLog("DrawGUIs gui=[%d]",gui);

    if(focus_stump)
        if(focus_control)
            if(focus_control->parent_stump!=focus_stump)
                focus_control=0;

    tstump=gui->first_gui_stump;

    while(tstump)
    {
		DLog("DrawGUIs tstump=[%d]",tstump);
		if(tstump->props&FM_GP_FULLSCREEN)
		{
			tstump->rect.left=0;
			tstump->rect.top=0;
			tstump->rect.bottom=pClientData->ScreenHeight;
			tstump->rect.right=pClientData->ScreenWidth;
		}

        if(tstump->moving==true)
        {
            DrawTransparentBar(    
                                tstump->rect.left-4,
                                tstump->rect.top-4,
                                tstump->rect.left+tstump->rect.right+4,
                                tstump->rect.top+tstump->rect.bottom+4,
                                LONGRGB(GetFade(3),GetFade(3),255) ,
                                LONGRGB(GetFade(3),GetFade(3),255)) ;
        }

        if(tstump->props&FM_GP_USEBACKGROUND)
        {
            DrawBaseGFX( tstump->rect.left,
                         tstump->rect.top,
                         tstump->rect.left+tstump->rect.right+2,
                         tstump->rect.top+tstump->rect.bottom+2,
                         atoi(tstump->media),
                         255,255,255 );
        }
        else
        {

            DrawBar(    tstump->rect.left-2,
                        tstump->rect.top-2,
                        tstump->rect.left+tstump->rect.right,
                        tstump->rect.top+tstump->rect.bottom+2,
                        LONGRGB(240,240,240),
                        LONGRGB(240,240,240));

            DrawBar(    tstump->rect.left+1,
                        tstump->rect.top+1,
                        tstump->rect.left+tstump->rect.right+2,
                        tstump->rect.top+tstump->rect.bottom+2,
                        LONGRGB(80,80,80),
                        LONGRGB(80,80,80));

            DrawBar(    tstump->rect.left,
                        tstump->rect.top,
                        tstump->rect.left+tstump->rect.right,
                        tstump->rect.top+tstump->rect.bottom,
                        LONGRGB(180,180,180),
                        LONGRGB(180,180,180));
        }

        if(tstump->props&FM_GP_MENU)
        {
            if(focus_stump==tstump)
            {
                DrawBar(    tstump->rect.left+1,
                            tstump->rect.top+1,
                            tstump->rect.left+tstump->rect.right-1,
                            tstump->rect.top+18,
                            LONGRGB(160,60,160),
                            LONGRGB(60,0,60));
            }
            else
            {
                DrawBar(    tstump->rect.left+1,
                            tstump->rect.top+1,
                            tstump->rect.left+tstump->rect.right-1,
                            tstump->rect.top+18,
                            LONGRGB(60,60,60),
                            LONGRGB(10,10,10));
            }

            WriteText(tstump->rect.left+9,tstump->rect.top+2,va("^1^+%s",tstump->caption),0);
            WriteText(tstump->rect.left-8,tstump->rect.top+2,"R",2);
        }

        if(tstump->props&FM_GP_CLOSEX)
        {
            WriteText(tstump->rect.left+tstump->rect.right-25,tstump->rect.top+2,"^1^+X",2);
            if(MouseIn(tstump->rect.left+tstump->rect.right-25,tstump->rect.top,tstump->rect.left+tstump->rect.right,tstump->rect.top+16))
            {
                WriteText(tstump->rect.left+tstump->rect.right-25,tstump->rect.top+2,"^6^+X",2);
            }
        }

        tstump->gui_control=tstump->first_gui_control;
        while(tstump->gui_control)
        {


            x=tstump->rect.left;
            y=tstump->rect.top;
            x2=tstump->rect.right;
            y2=tstump->rect.bottom;

            if(tstump->gui_control->visible)
            {
                switch(tstump->gui_control->type)
                {

					case FM_GC_LISTBOX:

							DrawBar(    x+tstump->gui_control->rect.left-2,
										y+tstump->gui_control->rect.top-2,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2,
										LONGRGB(240,240,240),
										LONGRGB(240,240,240));

							DrawBar(    x+tstump->gui_control->rect.left+1,
										y+tstump->gui_control->rect.top+1,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right+2,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2,
										LONGRGB(80,80,80),
										LONGRGB(80,80,80));

							DrawBar(    x+tstump->gui_control->rect.left,
										y+tstump->gui_control->rect.top,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom,
										LONGRGB(180,180,180),
										LONGRGB(180,180,180));

							x=x+tstump->gui_control->rect.left;
							y=y+tstump->gui_control->rect.top;
							y2=y+tstump->gui_control->rect.bottom;

							WriteText(10,10,va("[%d][%d]",y,y2),1);

							tstump->gui_control->control_data=tstump->gui_control->first_control_data;
							while(tstump->gui_control->control_data)
							{
								if(y>(y2-12)) break;
								strcpy(temp,tstump->gui_control->control_data->value);
								while( (strlen(temp)*9) > tstump->gui_control->rect.right)
								{
									temp[strlen(temp)-4]='.';
									temp[strlen(temp)-3]='.';
									temp[strlen(temp)-2]='.';
									temp[strlen(temp)-1]=0;
								}
								
								WriteText(x,y,temp,0);
								
								y+=12;

								tstump->gui_control->control_data = 
									tstump->gui_control->control_data->next;
							}
				
							break;

                    case FM_GC_GROUPTICK:
                    case FM_GC_TICKBOX:

						if(!strlen(tstump->gui_control->media))
						{
							DrawBar(    x+tstump->gui_control->rect.left-2,
										y+tstump->gui_control->rect.top-2,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2,
										LONGRGB(240,240,240),
										LONGRGB(240,240,240));

							DrawBar(    x+tstump->gui_control->rect.left+1,
										y+tstump->gui_control->rect.top+1,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right+2,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2,
										LONGRGB(80,80,80),
										LONGRGB(80,80,80));

							DrawBar(    x+tstump->gui_control->rect.left,
										y+tstump->gui_control->rect.top,
										x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
										y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom,
										LONGRGB(180,180,180),
										LONGRGB(180,180,180));
						}
						else
						{
							DrawBaseGFX( x+tstump->gui_control->rect.left-2,
										 y+tstump->gui_control->rect.top-2,
									     x+tstump->gui_control->rect.left+tstump->gui_control->rect.right+2,
										 y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2,
										 atoi(tstump->gui_control->media),
										 255,255,255 );
						}

                        if(dlcs_strcasecmp(tstump->gui_control->value,"on"))
                        {
                            WriteText(x+tstump->gui_control->rect.left-7,y+tstump->gui_control->rect.top,"H",2);
                        }

                        break;

                    case FM_GC_STATIC_TEXT:
                        if(tstump->gui_control->props&FM_GP_BORDER)
                            DrawRectangle(tstump->gui_control->rect.left+x+6,tstump->gui_control->rect.top+y-2,tstump->gui_control->rect.left+x+(CGLFont_StrLen(tstump->gui_control->value))*8+24,tstump->gui_control->rect.top+y+16,LONGRGB(0,0,0));
                        WriteText(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y,
                                va("^g%s",tstump->gui_control->value),0);
                        break;

                    case FM_GC_TEXTBOX:
					case FM_GC_PASSWORD:

                        if(MouseIn( tstump->gui_control->rect.left+x+6,
                                    tstump->gui_control->rect.top+y,
                                    tstump->gui_control->rect.left+x+tstump->gui_control->rect.right, //+178,//+(strlen(tstump->gui_control->value))*8+24,
                                    tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom) )//17) )
                        {
                            if( (INPUTMODE!=CONSOLE1) &&
                                (INPUTMODE!=CONSOLE2) )
                            {
								/*
                                if(focus_stump!=tstump)
                                {
                                    k=0;
                                    for(j=0;j<MAX_GUIS;j++)
                                    {
                                        //if(!gui->stump_array[j].st)
                                           // continue;

                                        if(MouseIn( gui->stump_array[j].st->rect.left,
                                                    gui->stump_array[j].st->rect.top,
                                                    gui->stump_array[j].st->rect.left+gui->stump_array[j].st->rect.right,
                                                    gui->stump_array[j].st->rect.top+gui->stump_array[j].st->rect.bottom) )
                                        {
                                            k++;
                                        }
                                    }
                                    if(!k)
                                        MOUSEMODE=MP_TEXTINPUT;
                                }
                                else
								*/
                                    MOUSEMODE=MP_TEXTINPUT;
                            }
                        }

                        if(focus_control==tstump->gui_control)
                        {
                    
                            DrawBar( tstump->gui_control->rect.left+x+1,
                                     tstump->gui_control->rect.top+y-1,
                                     tstump->gui_control->rect.left+x+tstump->gui_control->rect.right+1,//+180,//+(strlen(tstump->gui_control->value))*8+24,
                                     tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom+1,//17,
                                     LONGRGB(GetFade(3),GetFade(3),255),LONGRGB(GetFade(3),GetFade(3),255));

                            DrawBar( tstump->gui_control->rect.left+x+8,
                                     tstump->gui_control->rect.top+y,
                                     tstump->gui_control->rect.left+x+tstump->gui_control->rect.right,//178,//+(strlen(tstump->gui_control->value))*8+24,
                                     tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom, //15,
                                     LONGRGB(255,255,255),LONGRGB(255,255,255));

                            
                            strcpy(temp,tstump->gui_control->value);

                            k=(tstump->gui_control->rect.right/FONT_CHAR_WIDTH)-1;

                            while(CGLFont_StrLen(temp)>k)
                            {
                                j=0;
                                while(temp[j]!=0)
                                {
                                    temp[j]=temp[j+1];
                                    j++;
                                }
                            }
                            
							if(tstump->gui_control->type==FM_GC_PASSWORD)
							{
								for(k=0;k<strlen(temp);k++)
								{
									temp[k]='*';
								}
							}

                            WriteText(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y,va("^0%s",temp),0);
                            if(FlashCursor(0,0))
                                WriteText((int)(tstump->gui_control->rect.left+x+(CGLFont_StrLen(temp)*8.5)),(int)(tstump->gui_control->rect.top+y-2),"A",2);
                            

                        }
                        else
                        {
                    
                            DrawBar( tstump->gui_control->rect.left+x+1,
                                     tstump->gui_control->rect.top+y-1,
                                     tstump->gui_control->rect.left+x+tstump->gui_control->rect.right+1,//180,//+(strlen(tstump->gui_control->value))*8+24,
                                     tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom+1,//17,
                                     LONGRGB(0,0,0),LONGRGB(0,0,0));

                            DrawBar( tstump->gui_control->rect.left+x+8,
                                     tstump->gui_control->rect.top+y,
                                     tstump->gui_control->rect.left+x+tstump->gui_control->rect.right,//178,//+(strlen(tstump->gui_control->value))*8+24,
                                     tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom,//15,
                                     LONGRGB(255,255,255),LONGRGB(255,255,255));

                            strcpy(temp,tstump->gui_control->value);
                            k=(tstump->gui_control->rect.right/FONT_CHAR_WIDTH)-1;
                            temp[k]=0;

							if(tstump->gui_control->type==FM_GC_PASSWORD)
							{
								for(k=0;k<strlen(temp);k++)
								{
									temp[k]='*';
								}
							}
                    
                            WriteText(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y,va("^0%s",temp),0);

                        }

                        break;

                    case FM_GC_SUBMIT:
                    case FM_GC_BUTTON:
                    case FM_GC_CANCEL:
                        if(tstump->gui_control->props&FM_GP_USEBACKGROUND)
                        {
                            DrawBaseGFXGUIButton(tstump->gui_control,x,y);
                        }
                        else
                        {
                            DrawSlicedGUIButton(tstump->gui_control,x,y);
                        }
                        break;

                    case FM_GC_HIDDEN: // do nothing
                        break;

                    default:
                        WriteText(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y-2,va("Control[%d]",tstump->gui_control->type),0);
                        break;
                }
            }




            tstump->gui_control=tstump->gui_control->next;
        }
		tstump=tstump->next;
    }

    tstump=gui->first_gui_stump;
	while(tstump)
	{

       

        if(tstump->props&FM_GP_CLOSEX)
        {
            if(MouseIn(tstump->rect.left+tstump->rect.right-25,tstump->rect.top,tstump->rect.left+tstump->rect.right,tstump->rect.top+16))
            {
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    tstump->deleteme=true;
                    SetMouseRelease(SDL_BUTTON_LEFT,0);
                }
            }
        }

        tstump->gui_control=tstump->first_gui_control;
        while(tstump->gui_control)
        {
            x=tstump->rect.left;
            y=tstump->rect.top;
            x2=tstump->rect.right;
            y2=tstump->rect.bottom;

            if(tstump->gui_control->visible)
            {
                switch(tstump->gui_control->type)
                {
                    case FM_GC_GROUPTICK:
                        if(MouseIn( x+tstump->gui_control->rect.left-2,
                                    y+tstump->gui_control->rect.top-2,
                                    x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
                                    y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2 ) )
                        {
                            if(GetMouseRelease(SDL_BUTTON_LEFT))
                            {
                                tstump->clear_grouptick(tstump->gui_control->name);

                                if(dlcs_strcasecmp(tstump->gui_control->value,"on"))
                                    strcpy(tstump->gui_control->value,"off");
                                else
                                    strcpy(tstump->gui_control->value,"on");

                                focus_control=tstump->gui_control;
                                SetMouseRelease(SDL_BUTTON_LEFT,0);
                            }

                        }
                        break;

                    case FM_GC_TICKBOX:

                        if(MouseIn( x+tstump->gui_control->rect.left-2,
                                    y+tstump->gui_control->rect.top-2,
                                    x+tstump->gui_control->rect.left+tstump->gui_control->rect.right,
                                    y+tstump->gui_control->rect.top+tstump->gui_control->rect.bottom+2 ) )
                        {
                            if(GetMouseRelease(SDL_BUTTON_LEFT))
                            {
                                if(dlcs_strcasecmp(tstump->gui_control->value,"on"))
                                    strcpy(tstump->gui_control->value,"off");
                                else
                                    strcpy(tstump->gui_control->value,"on");

                                focus_control=tstump->gui_control;
                                SetMouseRelease(SDL_BUTTON_LEFT,0);
                            }
                        }

                    case FM_GC_STATIC_TEXT:
                        break;

                    case FM_GC_TEXTBOX:
					case FM_GC_PASSWORD:

                        if(MouseIn( tstump->gui_control->rect.left+x+6,
                                    tstump->gui_control->rect.top+y,
                                    tstump->gui_control->rect.left+x+tstump->gui_control->rect.right, //178,
                                    tstump->gui_control->rect.top+y+tstump->gui_control->rect.bottom) ) //17) )
                        {
                            if( (INPUTMODE!=CONSOLE1) &&
                                (INPUTMODE!=CONSOLE2) )
                            {
                                if(GetMouseRelease(SDL_BUTTON_LEFT))
                                {
                                    focus_control=tstump->gui_control;
                                    focus_stump=tstump;
                                    SetMouseRelease(SDL_BUTTON_LEFT,0);
                                }
                            }
                        }
                        break;


                    case FM_GC_CANCEL:
                        if(MouseIn(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y,tstump->gui_control->rect.right+x,tstump->gui_control->rect.bottom+y))
                        {
                            if(GetMouseRelease(SDL_BUTTON_LEFT))
                            {
                                gui->del_stump(tstump->name);
                                return;
                            }
                        }

                    case FM_GC_SUBMIT:
                    case FM_GC_BUTTON:

                        if(MouseIn(tstump->gui_control->rect.left+x,tstump->gui_control->rect.top+y,tstump->gui_control->rect.right+x,tstump->gui_control->rect.bottom+y))
                        {
                            if(GetMouseRelease(SDL_BUTTON_LEFT))
                            {

                                if(tstump->props&FM_GP_CLIENT_ONLY)
                                {
                                    // process client side gump
									sprintf(temp,"guigo action=%s",tstump->action);
									                                    
                                    tctrl=tstump->first_gui_control;
									if(tctrl)
									{
										strcat(temp,"|");
										strcat(temp,tstump->gui_control->name);
										strcat(temp,"=");
										strcat(temp,tstump->gui_control->value);

										while(tctrl)
										{
											if(
												(tctrl->type!=FM_GC_BUTTON) &&
												(tctrl->type!=FM_GC_STATIC_TEXT)
												)
											{
												strcat(temp,"|");
												strcat(temp,tctrl->name);
												strcat(temp,"=");
												if(tctrl->type==FM_GC_PASSWORD)
													strcat(temp,encrypt(tctrl->value));
												else
													strcat(temp,tctrl->value);
											}
											tctrl=tctrl->next;
										}
									}
                                    tctrl=0;
									
                                    ProcessConsoleCommand(temp,0);

                                    Log(temp);
									if(!(tstump->props&FM_GP_DO_NOT_DIE))
										gui->del_stump(tstump->name);
                                    return;
                                }

                                else
                                {
                                    SendData.Reset();
                                    SendData.Write((char)NETMSG_GUI);
                                    SendData.Write((char *)tstump->name);

                                    j=0;
                                    tctrl=tstump->first_gui_control;
                                    while(tctrl)
                                    {
                                        if(!(tctrl->props&FM_GP_NO_RETURN))
                                        {
                                            j++;
                                        }
                                        tctrl=tctrl->next;
                                    }

                                    SendData.Write((char)j);
                                    tctrl=tstump->first_gui_control;
                                    while(tctrl)
                                    {
                                        if(!(tctrl->props&FM_GP_NO_RETURN))
                                        {
                                            SendData.Write((char *)tctrl->name);
                                            SendData.Write((char *)tctrl->value);
                                        }
                                        tctrl=tctrl->next;
                                    }
                                    tctrl=0;
                                    SendNetMessage(0);
                                    DLog("Send submit for stump[%s] with %d controls",tstump->name,j);
                                    focus_control=0;
                                    SetMouseRelease(SDL_BUTTON_LEFT,0);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            tstump->gui_control=tstump->gui_control->next;
        }

        if(tstump)
        if((tstump->props&FM_GP_MOVEABLE)>0)
        {
            if(tstump->moving==false)
            {
                if(MouseIn( tstump->rect.left,
                            tstump->rect.top,
                            tstump->rect.left + tstump->rect.right,
                            tstump->rect.top  + tstump->rect.bottom ) )
                {
                    if(GetMouseRelease(SDL_BUTTON_RIGHT))
                    {
                        tstump->moving=true;
                        focus_stump=tstump;
                        gui->top_stump(focus_stump->name);
                        SetMouseRelease(SDL_BUTTON_RIGHT,0);
                        tstump->mrect.left=(GetMouseX()-tstump->rect.left);
                        tstump->mrect.top =(GetMouseY()-tstump->rect.top);
                    }
                }
            }
            else
            {
                tstump->rect.left=GetMouseX()-tstump->mrect.left;
                tstump->rect.top =GetMouseY()-tstump->mrect.top;
                if(GetMouseRelease(SDL_BUTTON_RIGHT))
                {
                    tstump->moving=false;
                    SetMouseRelease(SDL_BUTTON_RIGHT,0);
                }
            }            
        }

        if(MouseIn( tstump->rect.left,
                    tstump->rect.top,
                    tstump->rect.left+tstump->rect.right,
                    tstump->rect.top+tstump->rect.bottom ) )
        {
            if(GetMouseRelease(SDL_BUTTON_LEFT))
            {
                if(focus_stump==tstump)
                {
                    return;
                }
                else
                {

                    focus_stump=tstump;
                    gui->top_stump(focus_stump->name);
                    return;
                }
            }
        }
		tstump=tstump->next;
    }

	tstump=gui->first_gui_stump;
	while(tstump)
    {
        if(tstump->deleteme)
        {
            if(focus_stump==tstump)
                focus_stump=0;
            gui->del_stump(tstump);
            
        }

		tstump=tstump->next;
    }
}
/****************************************************************************************************/
void DrawVertice(int x, int y)
{
    DrawBar(x,y,x+2,y+2,LONGRGB(255,0,0),LONGRGB(0,0,255));
}
/****************************************************************************************************/
void DrawBaseGFX(int x,int y,int x2,int y2,int which,u_char r,u_char g,u_char b)
{   
    if(!BaseTexture) { LoadBaseGFX(); return; }
    if(!BaseTexture[which].texture) { LoadBaseGFX(); return; }
    if(!TOBOOL(BaseTexture[which].texture->bmap)) { Load1BaseGFX(which); return; }
    BaseTexture[which].texture->Draw(x,y,x2,y2,r,g,b);
}
/****************************************************************************************************/
void Prompt(char *szPrompt, char *szCommand)
{
    ProcessConsoleCommand("play prompt.wav",0);
    strcpy(pClientData->szPromptMsg,szPrompt);
    strcpy(pClientData->szCommand,szCommand);
}
/****************************************************************************************************/
void DrawPrompt(void)
{
    if(!CGLFont_StrLen(pClientData->szPromptMsg)) return;    
    DrawBaseGFX(250,200,550,500,9,255,255,255);
    int i,j,k;
    char szTemp[1024];
    // TODO: if the message is larger than the window, wrap it somehow here, maybe add a scrollbar if necessary
    memset(szTemp,0,1024); j=0; k=0;
    for(i=0;i<CGLFont_StrLen(pClientData->szPromptMsg);i++)
    {
        szTemp[j]=pClientData->szPromptMsg[i];
        j++;
        if(j>25)
        {
            k++; j=0;
            WriteText(278,225+k*12,szTemp,0);
            memset(szTemp,0,1024);
        }
    }
    if(j!=0) { k++; WriteText(278,225+k*12,szTemp,0); }
    DrawButton(BC_CANCEL,UP,271,360);
    if(MouseIn(271,360,318,380))
    {
        if(GetMouseRelease(SDL_BUTTON_LEFT))
        {
            ProcessConsoleCommand(pClientData->PlayMouseLB,0);
            memset(pClientData->szPromptMsg,0,_MAX_PATH);
            return;
        }
    }
    DrawButton(BC_OK,UP,482,360);
    if(MouseIn(482,360,530,380))
    {
        if(GetMouseRelease(SDL_BUTTON_LEFT))
        {
            ProcessConsoleCommand(pClientData->PlayMouseLB,0);
            ProcessConsoleCommand(pClientData->szCommand,0);
            memset(pClientData->szPromptMsg,0,_MAX_PATH);
            return;
        }
    }
    if(MouseIn(250,200,550,400)) MOUSEMODE=MP_NORTHWEST;
}
/****************************************************************************************************/
bool bIsPrompt(void) { if(strlen(pClientData->szPromptMsg)) return true; return false; }
/****************************************************************************************************/
void DrawBit4ge(int x,int y,int x2,int y2,bool bsin)
{
    if(!cgltBit4ge)
    {
        cgltBit4ge=new CGLTexture();
        cgltBit4ge->Load(va("%s%cdata%cdlstorm.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
        return;
    }

	

    static float fB4;
    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(pClientData->ScreenHeight/2);

    fB4 +=.5;

	// cgltBit4ge->Draw(x,y,x3,y3,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f);

	//*

    // glLoadIdentity();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);

	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D,cgltBit4ge->bmap);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS );
    if(bsin)
    {


        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y-y3+ 5*sin(fB4/12)),    1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3+ 5*sin(fB4/15)),  float(y-y3+ 5*sin(fB4/13)),    1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3+ 5*sin(fB4/14)),  float(y+ 5*sin(fB4/25)),    1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y+ 5*sin(fB4/12)),    1.0f);
    }
    else
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),   float(y-y3),    1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),  float(y-y3),    1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),  float(y),    1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),   float(y),    1.0f);

    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
	/*	*/
}
/****************************************************************************************************/
void DrawFileXferStatus(void) // Draw download stats at bottom of screen
{
    /*
    if(!pPlayer) return;
    if(!pPlayer->DownloadInfo.bIsDownloading) return;
    WriteText(10,310,va("Recieving file[%s]",pPlayer->DownloadInfo.szLocalXferName),LONGRGB(0,255,0));
    WriteText(10,320,va("File size[%d] u_chars",pPlayer->DownloadInfo.iFileSize),LONGRGB(0,255,0));
    WriteText(10,330,va("Progress[%d] u_chars",pPlayer->DownloadInfo.iTotalBytesRead),LONGRGB(0,255,0));
    // WriteText(10,340,va("Transfer Rate :%d u_chars per second",pPlayer->iBytesPerSecond),LONGRGB(0,255,0));
    DrawStatBar(10,345,150,(int)pPlayer->DownloadInfo.iTotalBytesRead,(int)pPlayer->DownloadInfo.iFileSize);
    */
}
/****************************************************************************************************/
void DrawOptions(void)
{
	/*
    static char mode;

    DrawBar(38,20,pClientData->ScreenWidth-38,38,LONGRGB(100,100,0),LONGRGB(100,200,0));
    WriteText(40,22,"^1Options",5);
    DrawBar(38,38,pClientData->ScreenWidth-38,40,LONGRGB(0,140,140),LONGRGB(0,100,100));
    DrawBar(38,38,40,pClientData->ScreenHeight-38,LONGRGB(0,140,140),LONGRGB(0,100,100));
    DrawTransparentBar(40,40,pClientData->ScreenWidth-40,pClientData->ScreenHeight-40,LONGRGB(0,80,80),LONGRGB(0,100,100));
    DrawBar(38,pClientData->ScreenHeight-40,pClientData->ScreenWidth-38,pClientData->ScreenHeight-38,LONGRGB(0,60,60),LONGRGB(0,30,30));
    DrawBar(pClientData->ScreenWidth-38,38,pClientData->ScreenWidth-40,pClientData->ScreenHeight-38,LONGRGB(0,60,60),LONGRGB(0,30,30));

    DrawBar(pClientData->ScreenWidth-41-12,21,pClientData->ScreenWidth-39,38,LONGRGB(220,220,220),LONGRGB(90,90,90));
    DrawBar(pClientData->ScreenWidth-41-11,22,pClientData->ScreenWidth-40,37,LONGRGB(180,180,180),LONGRGB(180,180,180));
    WriteText(pClientData->ScreenWidth-40-20,22,"X",5);
    if((MouseIn(pClientData->ScreenWidth-41-12,21,pClientData->ScreenWidth-39,38))&&(GetMouseRelease(SDL_BUTTON_LEFT)))
    {
        pClientData->drawoptions=false;
        ProcessConsoleCommand("play testsound.mp3",0);        
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Draw Tabs for mode selection

    DrawBaseGFX(60,60,pClientData->ScreenWidth-60,pClientData->ScreenHeight-60,0,255,255,255);
    
    // video            0
    if(mode==0) DrawBar(42,42,130,60,LONGRGB(GetFade(3),GetFade(3),255),LONGRGB(GetFade(2),GetFade(2),255));
    else        DrawBar(42,42,130,60,LONGRGB(20,20,20),LONGRGB(80,80,80));
    if((MouseIn(42,42,130,60))&&(GetMouseRelease(SDL_BUTTON_LEFT))) mode=0;
    WriteText(42,42,"Video",5);
    if(mode==0)
    {
        DrawGUIResource(BC_TICKBOX,96,190,96+16,190+16);
        WriteText(107,190,"640x480x16",5);
        if( (pClientData->ScreenWidth ==640) &&
            (pClientData->ScreenHeight==480) &&
            (pClientData->ScreenColors==16) )
        {
            WriteText(89,190,"H",2);
        }
        if(MouseIn(96,190,96+16,190+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=640;
            pClientData->ScreenHeight=480;
            pClientData->ScreenColors=16;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,210,96+16,210+16);
        WriteText(107,210,"800x600x16",5);
        if( (pClientData->ScreenWidth ==800) &&
            (pClientData->ScreenHeight==600) &&
            (pClientData->ScreenColors==16) )
        {
            WriteText(89,210,"H",2);
        }
        if(MouseIn(96,210,96+16,210+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=800;
            pClientData->ScreenHeight=600;
            pClientData->ScreenColors=16;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,230,96+16,230+16);
        WriteText(107,230,"1024x768x16",5);
        if( (pClientData->ScreenWidth ==1024) &&
            (pClientData->ScreenHeight==768) &&
            (pClientData->ScreenColors==16) )
        {
            WriteText(89,230,"H",2);
        }
        if(MouseIn(96,230,96+16,230+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=1024;
            pClientData->ScreenHeight=768;
            pClientData->ScreenColors=16;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,250,96+16,250+16);
        WriteText(107,250,"1280x1024x16",5);
        if( (pClientData->ScreenWidth ==1280) &&
            (pClientData->ScreenHeight==1024) &&
            (pClientData->ScreenColors==16) )
        {
            WriteText(89,250,"H",2);
        }
        if(MouseIn(96,250,96+16,250+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=1280;
            pClientData->ScreenHeight=1024;
            pClientData->ScreenColors=16;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,270,96+16,270+16);
        WriteText(107,270,"640x480x32",5);
        if( (pClientData->ScreenWidth ==640) &&
            (pClientData->ScreenHeight==480) &&
            (pClientData->ScreenColors==32) )
        {
            WriteText(89,270,"H",2);
        }
        if(MouseIn(96,270,96+16,270+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=640;
            pClientData->ScreenHeight=480;
            pClientData->ScreenColors=32;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,290,96+16,290+16);
        WriteText(107,290,"800x600x32",5);
        if( (pClientData->ScreenWidth ==800) &&
            (pClientData->ScreenHeight==600) &&
            (pClientData->ScreenColors==32) )
        {
            WriteText(89,290,"H",2);
        }
        if(MouseIn(96,290,96+16,290+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=800;
            pClientData->ScreenHeight=600;
            pClientData->ScreenColors=32;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,310,96+16,310+16);
        WriteText(107,310,"1024x768x32",5);
        if( (pClientData->ScreenWidth ==1024) &&
            (pClientData->ScreenHeight==768) &&
            (pClientData->ScreenColors==32) )
        {
            WriteText(89,310,"H",2);
        }
        if(MouseIn(96,310,96+16,310+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=1024;
            pClientData->ScreenHeight=768;
            pClientData->ScreenColors=32;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

        DrawGUIResource(BC_TICKBOX,96,330,96+16,330+16);
        WriteText(107,330,"1280x1024x32",5);
        if( (pClientData->ScreenWidth ==1280) &&
            (pClientData->ScreenHeight==1024) &&
            (pClientData->ScreenColors==32) )
        {
            WriteText(89,330,"H",2);
        }
        if(MouseIn(96,330,96+16,330+16)&&GetMouseRelease(SDL_BUTTON_LEFT))
        {
            pClientData->ScreenWidth=1280;
            pClientData->ScreenHeight=1024;
            pClientData->ScreenColors=32;
            SetScreenRes(pClientData->ScreenWidth,pClientData->ScreenHeight,pClientData->ScreenColors);
        }

    }

    // sound            1
    if(mode==1) DrawBar(130,42,218,60,LONGRGB(GetFade(3),GetFade(3),255),LONGRGB(GetFade(2),GetFade(2),255));
    else        DrawBar(130,42,218,60,LONGRGB(20,20,20),LONGRGB(80,80,80));
    if((MouseIn(130,42,218,60))&&(GetMouseRelease(SDL_BUTTON_LEFT))) mode=1;
    WriteText(130,42,"Audio",5);
    if(mode==1)
    {
        if(!pClientData->bAudioFailure)
        {
            if( (MouseIn(98,190,109,204)) && (GetMouseRelease(SDL_BUTTON_LEFT)) )
            {
                ProcessConsoleCommand("stop audio",0);
                ProcessConsoleCommand("toggle music",0);
            }
            WriteText(107,190,"^1^&Music",0);
            DrawGUIResource(BC_TICKBOX,96,190,96+16,190+16);
            if(pClientData->bMusic) WriteText(89,190,"H",2);
            else                    WriteText(89,190,"I",2);
            WriteText(150,190,"EFFFFFFFFFFFFFFFG",2);
            WriteText((int) (150+pClientData->fMusicVolume*(17*8)),190,"A",2);
            if((MouseIn(170,190,(int)(170+((1.0f)*17*8)),204)) && (GetMouseDown(SDL_BUTTON_LEFT)))
            {
                pClientData->fMusicVolume=(GetMouseX()-170)*.008;
                if(pClientData->fMusicVolume>1.0f) pClientData->fMusicVolume=1.0f;
                SetMusicVolume((pClientData->fMusicVolume*255));
            }
            if((MouseIn(98,210,109,224))&&(GetMouseRelease(SDL_BUTTON_LEFT))) ProcessConsoleCommand("toggle sound",0);
            WriteText(107,210,"^1^&Sound",0);
            DrawGUIResource(BC_TICKBOX,96,210,96+16,210+16);
            if(pClientData->bSound) WriteText(89,210,"H",2);
            else                    WriteText(89,210,"I",2);
            WriteText(150,210,"EFFFFFFFFFFFFFFFG",2);
            WriteText((int) (150+pClientData->fSoundVolume*(17*8)) ,210,"A",2);
            if((MouseIn(170,210,(int)(170+((1.0f)*17*8)),224)) )
            {
                if(GetMouseDown(SDL_BUTTON_LEFT))
                {
                    pClientData->fSoundVolume=(GetMouseX()-170)*.008;
                    if(pClientData->fSoundVolume>1.0f) pClientData->fSoundVolume=1.0f;
                    SetSoundVolume((pClientData->fSoundVolume*255));
                }
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                    ProcessConsoleCommand("play testsound.mp3",0);
            }
        }
        else
        {
            WriteText(107,190,"^1Audio hardware initialization failure!",5);
        }
    }

    // controls         2
    if(mode==2) DrawBar(218,42,306,60,LONGRGB(GetFade(3),GetFade(3),255),LONGRGB(GetFade(2),GetFade(2),255));
    else        DrawBar(218,42,306,60,LONGRGB(20,20,20),LONGRGB(80,80,80));
    if((MouseIn(218,42,306,60))&&(GetMouseRelease(SDL_BUTTON_LEFT))) mode=2;
    WriteText(218,42,"Controls",5);
    if(mode==2)
    {
        WriteText(107,190,"Control Options",5);


    }

    // miscellaneous    3

    if(mode==3) DrawBar(306,42,394,60,LONGRGB(GetFade(3),GetFade(3),255),LONGRGB(GetFade(2),GetFade(2),255));
    else        DrawBar(306,42,394,60,LONGRGB(20,20,20),LONGRGB(80,80,80));
    if((MouseIn(306,42,394,60))&&(GetMouseRelease(SDL_BUTTON_LEFT))) mode=3;
    WriteText(306,42,"Misc",5);
    if(mode==3)
    {
        WriteText(107,190,"Miscellaneous Options",5);


    }


    // game system font
    // console font
    // DLog on or off
    // downloads on or off
    // player chat delay
    // Draw a slider bar for sound and music
    // Video Options
    // Control Options
    // Bind keys via the new options interface
    // Misc Options
    // Nitnoid stuff
	*/
}
/****************************************************************************************************/
void DrawFPS(int iX,int iY)//,long color)
{
    static unsigned long   FPS;
    static unsigned long   LastShowFPS;
    static unsigned long   FrameCount;
    if(dlcs_get_tickcount()-LastShowFPS>1000)
    {
        FPS=(long)FrameCount*1000/(dlcs_get_tickcount()-LastShowFPS);
        LastShowFPS=dlcs_get_tickcount();
        FrameCount=0;
    }
    FrameCount++;
    WriteText(iX,iY,va("^5FPS ^1[^+^5%d^1^+]",FPS),5);
}
/****************************************************************************************************/
void DrawModels(void)
{
    static float x,y,z;

    x=320.4f;
    y=90.5f;

    Model=FirstModel;
    if(Model)
    {
        Model->Rotate(x,y,0);
        Model->Scale(1.7f,1.7f,1.7f);
        Model->Locate(-160,-90,-10);
        glDisable(GL_BLEND);
        Model->Draw();
        Model=Model->next;

        if(Model)
        {
            Model->Rotate(x,y,0);
            Model->Scale(1.7f,1.7f,1.7f);
            Model->Locate(160,-90,-10);
            glDisable(GL_BLEND);
            Model->Draw();
            Model=Model->next;

            if(Model)
            {
                glEnable(GL_BLEND);
                Model->Rotate(x,y,0);
                Model->Scale(1.7f,1.7f,1.7f);
                Model->Draw();
            }
        }
    }
}

/****************************************************************************************************/
void UpdatePickRay(GLfloat x,GLfloat y)
{
    GLfloat ray_pnt[4];
    GLfloat ray_vec[4];
    /*
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	glGetDoublev(  GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev(  GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	winX = x; winY = (GLfloat)viewport[3] - y;
	glReadPixels( x, GLint(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);
    gluUnProject( winX, winY, 1, modelview, projection, viewport, &posX2, &posY2, &posZ2);
    */
    GLint   mouse_x=x;//GetMouseX();//+x_offset;
    GLint   mouse_y=y;//GetMouseY();//+y_offset;
    GLfloat near_height,zNear,zFar,near_distance;
    GLint   window_width  = pClientData->ScreenWidth;
    GLint   window_height = pClientData->ScreenHeight;
    GLfloat aspect = double(window_width)/double(window_height);
    near_height=1.0f;
    near_distance=2.0f;
    zNear=0.0f;
    zFar=1.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_height * aspect, near_height * aspect, -near_height, near_height, zNear, zFar );
    // you can build your pick ray vector like this:
    GLint     window_y    = (window_height - mouse_y) - window_height/2;
    GLdouble  norm_y      = double(window_y)/double(window_height/2);
    GLint     window_x    = mouse_x - window_width/2;
    GLdouble  norm_x      = double(window_x)/double(window_width/2);
    // (Note that most window systems place the mouse coordinate origin in the upper left of the window instead of the lower left. That's why window_y is calculated the way it is in the above code. When using a glViewport() that doesn't match the window height, the viewport height and viewport Y are used to determine the values for window_y and norm_y.)
    // The variables norm_x and norm_y are scaled between -1.0 and 1.0. Use them to find the mouse location on your zNear clipping plane like so:
    //GLfloat y = near_height * norm_y;
    //GLfloat x = near_height * aspect * norm_x;
    // Now your pick ray vector is (x, y, -zNear).
    // To transform this eye coordinate pick ray into object coordinates,
    // multiply it by the inverse of the ModelView matrix in use when the
    // scene was rendered. When performing this multiplication, remember
    // that the pick ray is made up of a vector and a point, and that
    // vectors and points transform differently. You can translate and
    // rotate points, but vectors only rotate. The way to guarantee that
    // this is working correctly is to define your point and vector as
    // four-element arrays, as the following pseudo-code shows:
    PickRayNear.x = ray_pnt[0] = 0.0f;
    PickRayNear.y = ray_pnt[1] = 0.0f;
    PickRayNear.z = ray_pnt[2] = 0.0f;
    ray_pnt[3] = 1.0f;
    PickRayFar.x = ray_vec[0] = x;
    PickRayFar.y =ray_vec[1] = y; 
    PickRayFar.z =ray_vec[2] = -near_distance;
    ray_vec[3] = 0.0f;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    // GLdouble gl_z = 100.0 ;  gl_x = mX + ((mZoom - gl_z) * (gl_x - mX) / (mZoom - gl_z - z))) ;  gl_y = mY + ((mZoom - gl_z) * (gl_y - mY) / (mZoom - gl_z - z))) ; 
  
}

