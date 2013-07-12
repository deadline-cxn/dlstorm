/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_GFX_UTIL
#define _EMBER_GFX_UTIL

#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>

#endif//_MMEDIA
#endif//_WIN32

#include <GL/gl.h>
#include <GL/glu.h>
// #include <GL/glut.h>
#include <GL/glext.h>

#include "c_gaf.h"
#include "c_log.h"

#include "SDL.h"

#include "c_gltexture.h"
#include "c_gl3dmodel.h"

#include <math.h>

//#include "c_map.h"
#include "c_map_new.h"

#ifdef _WIN32
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "GLu32.lib" )
#pragma comment( lib, "GLaux.lib" )
#pragma comment( lib, "SDLmain.lib" )
#pragma comment( lib, "SDL.lib" )
#endif

#define MAX_BASE_GFX				160	    // GFX Index array
#define MAX_MODELS                  4

class C_Camera {
public:
    C_Camera();
    ~C_Camera();
    void    Initialize(void);
    void    Go(void);
    void    Rotate_Left(void);
    void    Rotate_Left_Start(void);
    void    Rotate_Left_Stop(void);

    void    Rotate_Right(void);
    void    Rotate_Right_Start(void);
    void    Rotate_Right_Stop(void);

    void    Move_Forward(void);
    void    Move_Forward_Start(void);
    void    Move_Forward_Stop(void);

    void    Move_Backward(void);
    void    Move_Backward_Start(void);
    void    Move_Backward_Stop(void);

    void    Move_Left(void);
    void    Move_Left_Start(void);
    void    Move_Left_Stop(void);

    void    Move_Right(void);
    void    Move_Right_Start(void);
    void    Move_Right_Stop(void);

    void mouseMovement(int x, int y);

    float   cScale;
    float   bounce;
    float   xpos;
    float   ypos;
    float   zpos;
    float   xrot;
    float   yrot;

    bool    bMovingLeft;
    bool    bMovingRight;
    bool    bMovingForward;
    bool    bMovingBackward;

    float   angle;
    float   lastx;
    float   lasty;


};

class C_GFX {
public:

    // C_GFX();
    C_GFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);
    ~C_GFX();
    CMesh   *g_pMesh;// = new CMesh();
    CMantraMap *pManMap;
    C_Camera *pCamera;
    // GLvoid  ReSizeGLScene(GLsizei width, GLsizei height);

    int  InitGL(void);
    void BeginScene(void);
    void SetWindowTitle(char *fmt, ...);
    void ShutDownGFX(void);
    bool InitializeGFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);
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
    CGLTexture  *GetBaseGFX(char *name);
    void DrawVertice(int x, int y);
    void DrawBaseGFX(int x,int y,int x2,int y2,char *name,unsigned char r,unsigned char g,unsigned char b);//long color);
    void DrawSun(void);
    void DrawBar(RECT rc,long color);
    void DrawBar(int iX,int iY,int iX2,int iY2,long color);
    void DrawBar(RECT rc,long color1,long color2);
    void DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
	void DrawRect(RECT rc, long color);
	void DrawRect(int iX,int iY,int iX2,int iY2,long color);
    void DrawRectangle(int iX,int iY,int iX2,int iY2,long color);
    void DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void draw_3d_box(RECT rect);
    void draw_3d_box(int x, int y, int x2, int y2);
    void DrawModels(void);
    void ToggleFullScreen(void);
    void SetScreenRes(int x,int y,int cl, bool fs);
    void UpdatePickRay(GLfloat x,GLfloat y);
    u_char GetFade(char cWhichFade);
    int     VideoFlags;
    bool    bFullScreen;
    int     ScreenWidth;
    int     ScreenHeight;
    int     ScreenColors;

    CGLTexture      *GetTexture(char *name);

    SDL_Surface		*pScreen;

    CGLTexture      *pDefaultTexture;
    CGLTexture      *pFirstTexture;
    CGLModel        *pFirstModel;

    CLog    *pLog;
    CGAF    *pGAF;
    char WindowCaption[1024];
};


extern "C" void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb);
extern "C" void drawsphere(int ndiv, float radius, float cr,float cg,float cb);
extern "C" void drawcube(void);



#endif

