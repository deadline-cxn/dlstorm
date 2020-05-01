/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_gfx.h
 **   Class:        C_GFX
 **                 C_Camera
 **   Description:  SDL / OpenGL class wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **   Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL
 **   Notes:
 **     C_GFX is mutually exclusive with CSDL_Wrap class
 **     Either C_GFX is used OR CSDL_Wrap
 ***************************************************************/
#ifndef _DLCS_C_GFX
#define _DLCS_C_GFX

#include "SDL.h"
#include "dlcs.h"
// ubuntu linux: - Simple DirectMedia Layer
// apt install libsdl2-2.0-0 libsdl2-dev
// path to sdl2 includes: /usr/include/SDL2
#include "SDL_image.h"
// ubuntu linux: - SDL Image
// apt install libsdl2-image-2.0-0 libsdl2-image-dev
// path to sdl2-image includes: /usr/include/SDL2

#include <gl.h>
#include <glu.h>
//

#include <il.h>
#include <ilut.h>
// ubuntu linux: DevIL (Image Library)
// apt install libdevil-dev

#include "c_entity.h"
#include "c_gaf.h"
#include "c_gl3dmodel.h"
#include "c_gltexture.h"
#include "c_log.h"

class CGLModel;
class CGLTexture;

class C_Camera {
   public:
    C_Camera();
    ~C_Camera();
    void Initialize(void);
    void Go(void);
    void Update(void);
    void Follow(CEntity *pEntity);
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

    void     mouseMovement(int x, int y);
    CEntity *pFollowEntity;
    float    bounce;
    dlcs_V3  loc;
    dlcs_V3  rot;
    dlcs_V3  scale;

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

class C_GFX {
   public:
    C_GFX();
    C_GFX(int w, int h, int c, bool FullScreen, const char *szInWindowCaption, const char *szInIcon, CLog *pUseLOG, CGAF *pUseGAF);
    ~C_GFX();

    // GFX Class Members
    CLog *               pLog;
    CGAF *               pGAF;
    C_Camera *           pCamera;
    SDL_Window *         pWindow;
    SDL_Renderer *       pRenderer;
    char                 szIcon[1024];
    char                 szWindowCaption[1024];
    SDL_Surface *        pWindowIcon;
    bool                 bUse3d;
    SDL_GLContext        GL;
    bool                 bSDLFailed;
    int                  iVideoFlags;
    bool                 bFullScreen;
    int                  iScreenWidth;
    int                  iScreenHeight;
    int                  iScreenColors;
    bool                 bCreatedLog;
    vector<CGLTexture *> textures;
    vector<CGLModel *>   models;
    vector<CEntity *>    entities;
    CGLTexture *         pDefaultTexture;  // default texture pointer
    CEntity *            pSelectedEntity;  // Selected Entity
    GLuint               _glRendermode;
    string               windowcaption;
    bool                 bEditEntities;
    v3ops                OpRot;
    v3ops                OpLoc;
    v3ops                OpScale;
    SDL_Surface *        pFont;
    void                 InitVars(void);  // GFX System level functions
    bool                 InitGFX(int w, int h, int c, bool FullScreen, const char *szInWindowCaption, const char *szInIcon, CLog *pUSELOG, CGAF *pUSEGAF);
    bool                 Init3D(void);
    int                  InitGL(int x, int y);
    void                 BeginScene(void);
    void                 SetWindowTitle(string fmt, ...);
    void                 ShutDownGFX(void);
    bool                 ToggleFullScreen(void);
    GLvoid               ReSizeGLScene(GLsizei width, GLsizei height);
    void                 SetScreenRes(int x, int y, int cl, bool fs);
    void                 FlipSurfaces(void);
    void                 RenderScene(int mx, int my);
    void                 _RenderScene(unsigned int iGLRenderMode);
    void                 EndScene(void);
    SDL_Surface *        LoadMemSurface(SDL_RWops *rw);
    SDL_Surface *        LoadGAFSurface(CGAF *pUseGAF, const char *szFilename);
    SDL_Texture *        LoadGAFTexture(CGAF *pUseGAF, const char *szFilename);
    bool                 LoadTextures(CGAF *pGAF);  // OpenGL Texture Management (CGLTexture Class)
    CGLTexture *         GetTexture(const char *szInName);
    CGLTexture *         GetRandomTexture(void);
    void                 DrawTexture(int x, int y, int x2, int y2, const char *szInName, unsigned char r, unsigned char g, unsigned char b);  // long color);
    bool                 DestroyTextures(void);
    void                 Write(int x, int y, char *string, int bank);  // Font stuff
    bool                 LoadFont(const char *szInFile);
    void                 DrawVertice(int x, int y);  // 2D Draw Functions
    void                 DrawBar(RECT rc, long color);
    void                 DrawBar(int iX, int iY, int iX2, int iY2, long color);
    void                 DrawBar(RECT rc, long color1, long color2);
    void                 DrawBar(int iX, int iY, int iX2, int iY2, long color1, long color2);
    void                 DrawRect(RECT rc, long color);
    void                 DrawRect(int iX, int iY, int iX2, int iY2, long color);
    void                 DrawRectangle(int iX, int iY, int iX2, int iY2, long color);
    void                 DrawTransparentBar(int iX, int iY, int iX2, int iY2, long color1, long color2);
    void                 Draw3DBox(RECT rect);
    void                 Draw3DBox(int x, int y, int x2, int y2);
    void                 DrawFog(void);  // 3D Draw Functions
    void                 DrawSun(void);
    void                 DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r, float cr, float cg, float cb);
    void                 DrawSphere(int ndiv, float radius, float cr, float cg, float cb);
    void                 DrawCube(void);
    void                 DrawSkyBox(void);
    void                 DrawStarField(int iDir);
    u_char               GetFade(char cWhichFade);  // Miscellaneous GFX Functions
    CEntity *            GetSelectedEntity(void);   // Entity Functions
    void                 ClearSelectEntity(void);
    void                 SelectEntity(CEntity *pEntity);
    void                 SelectClosestEntity(void);
    void                 InitializeEntities(void);
    void                 MakeEntity(const char *szInName, float x, float y, float z);
    void                 DeleteEntity(CEntity *pEntity);
    void                 ClearEntities(void);
    void                 LoadEntities(dlcs_V3 WhichSector);
    void                 SaveEntities(dlcs_V3 WhichSector);
    void                 DrawEntities(void);
    /*  // OpenGL 3D Model Management (CGLModel Class)
        bool        LoadModels(void);
        CGLModel*   GetModel(string inname);
        CGLModel*   GetRandomModel(void);
        int         GetTotalModels(void);
        bool        DestroyModels(void);
        void setpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);
        // SDL_Surface *        pScreen;
        // SDL_Rect **          VideoModes;
        // dlcs_V3              Sector;
        // bool         Init2D(const char *caption, int width, int height, int bpp, SDL_Surface *icon);
    */
};

#endif  // _DLCS_C_GFX
