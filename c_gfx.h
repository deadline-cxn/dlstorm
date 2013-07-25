/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL

****************************************************************/

#ifndef _EMBER_GFX_UTIL
#define _EMBER_GFX_UTIL

class CVector2 { public: float x, y; };
class CVector3 { public: float x, y, z; };
class CColor3  { public: float r, g, b; };
class CTexCoord { public: float u,v; };
struct tFace   { int vertIndex[3]; int coordIndex[3]; };
typedef struct tagVERTEX   { float x, y, z; float u, v; } VERTEX;
typedef struct tagTRIANGLE { VERTEX  vertex[3]; } TRIANGLE;
typedef struct tagSECTOR   { int numtriangles; TRIANGLE* triangle; } SECTOR;

#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "SDL.h"

#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"
#include "c_gl3dmodel.h"
#include "c_entity.h"
#include "c_mapmodel.h"
#include "c_map_new.h"

class C_Entity;
class C_MapModel;
class C_MapModelList;
class CGLModel;
class CMesh;

class C_Camera {
public:
    C_Camera();
    ~C_Camera();

    void        Initialize(void);
    void        Go(void);
    void        Update(void);
    void        Follow(C_Entity* pEntity);
    void        Rotate_Left(void);
    void        Rotate_Left_Start(void);
    void        Rotate_Left_Stop(void);
    void        Rotate_Right(void);
    void        Rotate_Right_Start(void);
    void        Rotate_Right_Stop(void);
    void        Move_Forward(void);
    void        Move_Forward_Start(void);
    void        Move_Forward_Stop(void);
    void        Move_Backward(void);
    void        Move_Backward_Start(void);
    void        Move_Backward_Stop(void);
    void        Move_Left(void);
    void        Move_Left_Start(void);
    void        Move_Left_Stop(void);
    void        Move_Right(void);
    void        Move_Right_Start(void);
    void        Move_Right_Stop(void);
    void        mouseMovement(int x, int y);

    C_Entity*   pFollowEntity;

    float       bounce;

    float       cScale;

    float       xpos;
    float       ypos;
    float       zpos;

    float       xrot;
    float       yrot;

    bool        bMovingLeft;
    bool        bMovingRight;
    bool        bMovingForward;
    bool        bMovingBackward;

    float       angle;
    float       lastx;
    float       lasty;
};

class C_GFX {
public:
    C_GFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);
    ~C_GFX();
    // GFX System level functions
    int         InitGL(void);
    void        BeginScene(void);
    void        SetWindowTitle(char *fmt, ...);
    void        ShutDownGFX(void);
    bool        InitializeGFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);
    void        ToggleFullScreen(void);
    GLvoid      ReSizeGLScene(GLsizei width, GLsizei height);
    void        SetScreenRes(int x,int y,int cl, bool fs);
    void        FlipSurfaces(void);
    void        RenderScene(void);
    void        _RenderScene(unsigned int iGLRenderMode);
    void        EndScene(void);
    // OpenGL Texture Management (CGLTexture Class)
    bool        LoadTextures(CGAF *pGAF);
    CGLTexture* GetTexture(char *name);
    CGLTexture* GetRandomTexture(void);
    int         GetTotalTextures(void); // return number of loaded textures
    void        DrawTexture(int x,int y,int x2,int y2,char *name,unsigned char r,unsigned char g,unsigned char b);//long color);
    bool        DestroyTextures(void);
    // OpenGL 3D Model Management (CGLModel Class)
    bool        LoadModels(void);
    void        DrawModels(void);
    CGLModel*   GetModel(char* name);
    bool        DestroyModels(void);
    // 2D Draw Functions
    void        DrawVertice(int x, int y);
    void        DrawBar(RECT rc,long color);
    void        DrawBar(int iX,int iY,int iX2,int iY2,long color);
    void        DrawBar(RECT rc,long color1,long color2);
    void        DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void        DrawRect(RECT rc, long color);
    void        DrawRect(int iX,int iY,int iX2,int iY2,long color);
    void        DrawRectangle(int iX,int iY,int iX2,int iY2,long color);
    void        DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void        StarField(int iDir);
    void        Draw3DBox(RECT rect);
    void        Draw3DBox(int x, int y, int x2, int y2);
    // 3D Draw Functions
    void        DrawSun(void);
    void        DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb);
    void        DrawSphere(int ndiv, float radius, float cr,float cg,float cb);
    void        DrawCube(void);
    void        DrawSkyBox(void);
    // Miscellaneous GFX Functions
    u_char      GetFade(char cWhichFade);
    // GFX Class Members
    SDL_Surface* pScreen;
    int         VideoFlags;
    bool        bFullScreen;
    int         ScreenWidth;
    int         ScreenHeight;
    int         ScreenColors;
    CGLTexture* pDefaultTexture;
    CGLTexture* pFirstTexture;
    CGLModel*   pFirstModel;
    CMesh*      pMap;
    C_MapModel*     pFirstMapModel;
    C_MapModelList* pFirstMapModelList;
    C_Camera*   pCamera;
    CLog*       pLog;
    CGAF*       pGAF;
    char        WindowCaption[1024];
};

#endif
