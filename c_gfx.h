/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_gfx.h
 **   Class:        C_GFX
 **                 C_Camera
 **   Description:  SDL / OpenGL class wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 **   Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL
 **
 ***************************************************************/
#ifndef _DLCS_C_GFX
#define _DLCS_C_GFX

#ifdef _DLCS_SDL

#include "c_gaf.h"
#include "c_gltexture.h"
#include "c_log.h"
#include "dlstorm.h"
// #include "c_gl3dmodel.h"
// #include "c_entity.h"
#include "IL/il.h"
#include "IL/ilut.h"

// class C_Entity;
// class CGLModel;

class C_Camera {
   public:
    C_Camera();
    ~C_Camera();
    void Initialize(void);
    void Go(void);
    void Update(void);
    // void        Follow(C_Entity* pEntity);
    void Rotate_Left(void);
    void Rotate_Left_Start(void);
    void Rotate_Left_Stop(void);
    void Rotate_Right(void);
    void Rotate_Right_Start(void);
    void Rotate_Right_Stop(void);
    void Move_Forward(void);
    void Move_Forward_Start(void);
    void Move_Forward_Stop(void);
    void Move_Backward(void);
    void Move_Backward_Start(void);
    void Move_Backward_Stop(void);
    void Move_Left(void);
    void Move_Left_Start(void);
    void Move_Left_Stop(void);
    void Move_Right(void);
    void Move_Right_Start(void);
    void Move_Right_Stop(void);
    void Move_Up(void);
    void Move_Up_Start(void);
    void Move_Up_Stop(void);
    void Move_Down(void);
    void Move_Down_Start(void);
    void Move_Down_Stop(void);

    void mouseMovement(int x, int y);
    //     C_Entity*   pFollowEntity;
    float   bounce;
    dlcs_V3 loc;
    dlcs_V3 rot;
    dlcs_V3 scale;

    bool bMovingLeft;
    bool bMovingRight;
    bool bMovingForward1;
    bool bMovingForward2;
    bool bMovingBackward;
    bool bMovingUp;
    bool bMovingDown;
    bool bRotatingLeft;
    bool bRotatingRight;

    float angle;
    float lastx;
    float lasty;
};

typedef struct {
    bool bXp;
    bool bXn;
    bool bYp;
    bool bYn;
    bool bZp;
    bool bZn;
} v3ops;

class C2DFont {
   public:
    C2DFont();
    C2DFont(char *file);
    ~C2DFont();

    void Write(int x, int y, char *string, int bank);
    void Load(char *file);

    SDL_Surface *Font;
};

class C_GFX : public C2DFont {
   public:
    C_GFX(int w, int h, int c, bool FullScreen, string wincaption, bool use3d, CLog *pUSELOG, CGAF *pUSEGAF);
    ~C_GFX();

    // GFX Class Members
    SDL_Surface *pScreen;
    bool         bUse3d;
    bool         bSDLFailed;
    int          VideoFlags;
    bool         bFullScreen;
    int          ScreenWidth;
    int          ScreenHeight;
    int          ScreenColors;

    C_Camera *pCamera;

    bool  bCreatedLog;
    CLog *pLog;
    CGAF *pGAF;

    SDL_Rect **VideoModes;

    vector<CGLTexture *> textures;
    // vector<CGLModel*>   models;
    // vector<C_Entity*>   entities;

    CGLTexture *pDefaultTexture;  // default texture pointer
                                  //     C_Entity*   pSelectedEntity;// Selected Entity
    GLuint _glRendermode;

    dlcs_V3 Sector;

    string windowcaption;

    // bool        bEditEntities;

    v3ops OpRot;
    v3ops OpLoc;
    v3ops OpScale;

    // GFX System level functions
    bool   Init3d(void);
    int    InitGL(int x, int y);
    void   BeginScene(void);
    void   SetWindowTitle(string fmt, ...);
    void   ShutDownGFX(void);
    bool   InitializeGFX(int w, int h, int c, bool FullScreen, string wincaption, bool use3d, CLog *pUSELOG, CGAF *pUSEGAF);
    bool   ToggleFullScreen(void);
    GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
    void   SetScreenRes(int x, int y, int cl, bool fs);
    // void        FlipSurfaces(void);
    void RenderScene(int mx, int my);
    // void        _RenderScene(unsigned int iGLRenderMode);
    void EndScene(void);

    // OpenGL Texture Management (CGLTexture Class)
    bool        LoadTextures(CGAF *pGAF);
    CGLTexture *GetTexture(string inname);
    CGLTexture *GetRandomTexture(void);
    void        DrawTexture(int x, int y, int x2, int y2, string name, unsigned char r, unsigned char g, unsigned char b);  // long color);
    bool        DestroyTextures(void);

    /*
        // OpenGL 3D Model Management (CGLModel Class)
        bool        LoadModels(void);
        CGLModel*   GetModel(string inname);
        CGLModel*   GetRandomModel(void);
        int         GetTotalModels(void);
        bool        DestroyModels(void);
         */

    // void setpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);

    // 2D Draw Functions
    void DrawVertice(int x, int y);
    void DrawBar(RECT rc, long color);
    void DrawBar(int iX, int iY, int iX2, int iY2, long color);
    void DrawBar(RECT rc, long color1, long color2);
    void DrawBar(int iX, int iY, int iX2, int iY2, long color1, long color2);
    void DrawRect(RECT rc, long color);
    void DrawRect(int iX, int iY, int iX2, int iY2, long color);
    void DrawRectangle(int iX, int iY, int iX2, int iY2, long color);
    void DrawTransparentBar(int iX, int iY, int iX2, int iY2, long color1, long color2);
    void Draw3DBox(RECT rect);
    void Draw3DBox(int x, int y, int x2, int y2);

    // 3D Draw Functions
    void DrawFog(void);
    void DrawSun(void);
    void DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r, float cr, float cg, float cb);
    void DrawSphere(int ndiv, float radius, float cr, float cg, float cb);
    void DrawCube(void);
    void DrawSkyBox(void);
    void DrawStarField(int iDir);

    // Miscellaneous GFX Functions
    u_char GetFade(char cWhichFade);

    /*
        // Entity Functions
        C_Entity*   GetSelectedEntity(void);
        void        ClearSelectEntity(void);
        void        SelectEntity(C_Entity* pEntity);
        void        SelectClosestEntity(void);
        void        InitializeEntities(void);
        void        MakeEntity(string inname,float x, float y, float z);
        void        DeleteEntity(C_Entity* pEntity);
        void        ClearEntities(void);
        void        LoadEntities(dlcs_V3 WhichSector);
        void        SaveEntities(dlcs_V3 WhichSector);
        void        DrawEntities(void);
         */
};
#endif  // _DLCS_SDL
#endif  // _DLCS_C_GAF
