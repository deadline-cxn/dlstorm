/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_GFX_UTIL
#define _EMBER_GFX_UTIL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <winsock2.h>
//#include <io.h>
//#include <direct.h>
//#include <conio.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32
#include <GL/gl.h>
#include <GL/glu.h>
// #include "SDL.h"

#ifdef _WIN32
#pragma comment( lib, "OpenGL32.lib" )  
#pragma comment( lib, "GLu32.lib" )     
#pragma comment( lib, "GLaux.lib" )     
#pragma comment( lib, "SDLmain.lib" )   
#pragma comment( lib, "SDL.lib" )       
#endif

class CGLModel;
class CGLTexture;
class C_FM_VIS;
class C_FM_GUI_CONTROL;

struct stra
{
    float x;
    float y;
    float speed;
    int gfx;
};

#ifdef __cplusplus
extern "C"
{
#endif

    void ShutDownGFX(void);
    bool InitializeGFX(bool FullScreen);
    void RenderScene(void);
    void RestoreGraphics(void);
    bool FlashCursor(int iX, int iY);
	void WriteTextPct(float fX, float fY,char *szText,int fnt);
    void WriteText(int iX,int iY,char *szText,int font);

    void FlipSurfaces(void);

    bool InitGeneric(void);
    bool InitConsole(void);
    bool InitFonts(void);
    bool InitMouse(void);    
    bool InitBaseGFX(void);
    bool InitMapGFX(void);
    bool InitButtons(void);
    bool InitModels(void);
    bool InitVisibles(void);

    bool LoadGeneric(char *szFilename);
    bool LoadConsole(char *szFilename);
    bool LoadFonts(void);
    bool Load1Font(void);
    bool LoadMouse(char *path);    
    bool LoadBaseGFX(void);
    bool Load1BaseGFX(int which);
    bool LoadMapGFX(char *szPath);

    bool Load1MapGFXdt(int dt,char *szPath);
    bool Load1MapGFX(unsigned char cBank,unsigned char cTile,char *szPath);
    bool LoadButtons(char *szFilename);
    bool LoadModels(void);
    bool Load1Model(int which);

    bool DestroyGeneric(void);
    bool DestroyConsole(void);
    bool DestroyFonts(void);
    bool DestroyMouse(void);
    bool DestroyBaseGFX(void);
    bool DestroyMapGFX(void);
    bool DestroyButtons(void);
    bool DestroyModels(void);
    bool DestroyVisibles(void);

    CGLModel    *GetModel(char *name);
    CGLTexture  *GetBaseGFX(char *name);

    bool PushVis(int sid,int x,int y,int z,char *media,int media_type,int head,int dir);
    bool Pop_Vis(int sid);
    C_FM_VIS *GetVis(int sid);
    bool InsertVisible(C_FM_VIS *obj);
    bool RemoveVisible(C_FM_VIS *obj);
    bool UpdateVisibles(void);
    void DrawVisuals(void);

    void DrawStatBar(int iX,int iY, int iW, int iT,int iV); // iT is total value, iV is current value, will draw percentage bar
    void DrawCStatBar(int iX,int iY, int iW, int iT,int iV,long Color1,long Color2); // iT is total value, iV is current value, will draw percentage bar
    void DrawButton(int which, int updown, int x, int y);
    void DrawGenericSurface(void);
    void DrawGUIs(void);
    void DrawVertice(int x, int y);
    void DrawConsoleBMP(int iX1,int iY1,int iX2,int iY2);
    void DrawBaseGFX(int x,int y,int x2,int y2,int which,unsigned char r,unsigned char g,unsigned char b);//long color);
    void DrawPrompt(void);
    void DrawBit4ge(int x,int y,int x2,int y2,bool sin);    
    void DrawFileXferStatus(void);
    void DrawOptions(void);
    void DrawOSIcon(int iX,int iY,char *szOS);
    void DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void DrawRectangle(int iX,int iY,int iX2,int iY2,long color);
    void DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2);
    void DrawFPS(int iX,int iY);//,long color);
    void DrawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,unsigned char r, unsigned char g, unsigned char b);
    void DrawGUIButton(int x, int y,int whichslice,int size);
    int  DrawSlicedGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y); //int x, int y, int width);
    void DrawMouse(int iX,int iY,int iMouseMode);

    void DrawModels(void);
    
    void Prompt(char *szPrompt, char *szCommand);
    bool bIsPrompt(void);

    void ToggleFullScreen(void);
    void SetScreenRes(int x,int y,int cl);
  


    // GLvoid  ReSizeGLScene(GLsizei width, GLsizei height);
    int     InitGL(GLvoid);
    void    BeginScene(void);


    void UpdatePickRay(GLfloat x,GLfloat y);


#ifdef __cplusplus
}
#endif

#define DrawGUIResource(w,x,y,x2,y2) {DrawGUIResourceC(w,x,y,x2,y2,255,255,255);}



#endif

