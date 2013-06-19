/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_GFX_UTIL
#define _EMBER_GFX_UTIL

#include "SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"
#include "c_gl3dmodel.h"
#include <math.h>

#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32

#ifdef _WIN32
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "GLu32.lib" )
#pragma comment( lib, "GLaux.lib" )
#pragma comment( lib, "SDLmain.lib" )
#pragma comment( lib, "SDL.lib" )
#endif

#define MAX_BASE_GFX				4096	    // GFX Index array
#define MAX_MODELS                  128

class C_GFX
{
public:

    C_GFX();

    C_GFX(int w, int h, int c, bool FullScreen,const char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);

    ~C_GFX();


    // GLvoid  ReSizeGLScene(GLsizei width, GLsizei height);

    int  InitGL(void);
    void BeginScene(void);

    void ShutDownGFX(void);

    bool InitializeGFX(int w, int h, int c, bool FullScreen,const char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);

    void RenderScene(void);
    void RestoreGraphics(void);

    void EndScene(void);
    void FlipSurfaces(void);

    bool InitBaseGFX(void);
    bool InitModels(void);

    bool LoadBaseGFX(CGAF *pGAF);
    bool Load1BaseGFX(CGAF *pGAF,int which);

    bool LoadModels(void);
    bool Load1Model(int which);

    bool DestroyBaseGFX(void);
    bool DestroyModels(void);

    CGLModel    *GetModel(char *name);
    CGLTexture  *GetBaseGFX(char *name);

    void DrawVertice(int x, int y);

    void DrawBaseGFX(int x,int y,int x2,int y2,int which,unsigned char r,unsigned char g,unsigned char b);//long color);


    void DrawBar(RECT rc,long color);
    void DrawBar(int iX,int iY,int iX2,int iY2,long color);

    void DrawBar(RECT rc,long color1,long color2);
    void DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2);

	void DrawRect(RECT rc, long color);
    void DrawRectangle(int iX,int iY,int iX2,int iY2,long color);
    void DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2);

    void DrawModels(void);

    void ToggleFullScreen(void);
    void SetScreenRes(int x,int y,int cl, bool fs);

    void UpdatePickRay(GLfloat x,GLfloat y);

    u_char GetFade(char cWhichFade);

    int VideoFlags;

    SDL_Surface		*pScreen;

    CGLTextureList  *BaseTexture;
    CGLModel        *Model;
    CGLModel        *FirstModel;

    CLog    *pLog;
    CGAF    *pGAF;

    char *WindowCaption;


};






#endif

