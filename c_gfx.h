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

class C_Entity;
class CGLModel;

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
    void        Move_Up(void);
    void        Move_Up_Start(void);
    void        Move_Up_Stop(void);
    void        Move_Down(void);
    void        Move_Down_Start(void);
    void        Move_Down_Stop(void);

    void        mouseMovement(int x, int y);
    C_Entity*   pFollowEntity;
    float       bounce;
    CVector3    loc;
    CVector3    rot;
    CVector3    scale;

    bool        bMovingLeft;
    bool        bMovingRight;
    bool        bMovingForward1;
    bool        bMovingForward2;
    bool        bMovingBackward;
    bool        bMovingUp;
    bool        bMovingDown;
    bool        bRotatingLeft;
    bool        bRotatingRight;

    float       angle;
    float       lastx;
    float       lasty;
};

typedef struct {
    bool        bXp;
    bool        bXn;
    bool        bYp;
    bool        bYn;
    bool        bZp;
    bool        bZn;
} v3ops;

class C_GFX {

public:
    C_GFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF);
    ~C_GFX();

    // GFX Class Members
    SDL_Surface* pScreen;
    int         VideoFlags;
    bool        bFullScreen;
    int         ScreenWidth;
    int         ScreenHeight;
    int         ScreenColors;
    C_Camera*   pCamera;
    bool        bCreatedLog;
    CLog*       pLog;
    CGAF*       pGAF;
    CGLTexture* pDefaultTexture; // default texture storage
    CGLTexture* pFirstTexture; // texture storage
    CGLModel*   pFirstModel; // model storage
    C_Entity*   pFirstNTT;      // Entities
    C_Entity*   pFirstMapNTT;   // Map Entities
    C_Entity*   pSelectedEntity;// Selected Entity
    GLuint      _glRendermode;
    CVector3    Sector;
    char        WindowCaption[1024];
    bool        bEditEntities;

    v3ops OpRot;
    v3ops OpLoc;
    v3ops OpScale;

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
    void        RenderScene(int mx, int my);
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
    CGLModel*   GetModel(char* name);
    CGLModel*   GetRandomModel(void);
    int         GetTotalModels(void);
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
    void        Draw3DBox(RECT rect);
    void        Draw3DBox(int x, int y, int x2, int y2);

    // 3D Draw Functions
    void        DrawFog(void);
    void        DrawSun(void);
    void        DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb);
    void        DrawSphere(int ndiv, float radius, float cr,float cg,float cb);
    void        DrawCube(void);
    void        DrawSkyBox(void);
    void        DrawStarField(int iDir);

    // Miscellaneous GFX Functions
    u_char      GetFade(char cWhichFade);

    // Entity Functions
    C_Entity*   GetSelectedEntity(void);
    void        ClearSelectEntity(void);
    void        SelectEntity(C_Entity* pEntity);
    void        SelectClosestEntity(void);
    void        InitializeEntities(void);
    void        MakeNewRandomEntity(void);
    void        ClearEntities(void);
    void        LoadEntities(CVector3 WhichSector);
    void        SaveEntities(CVector3 WhichSector);
    void        DrawEntities(void);
};

#endif
