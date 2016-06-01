/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gltexture.h
 **   Class:        CGLTexture
 **   Description:  SDL/OpenGL Texture manager class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_CGLTEXTURE
#define _DLCS_CGLTEXTURE
#include "dlstorm.h"
#include "c_gaf.h"
#include "c_log.h"
#include <gl.h> 
#include <glu.h> 
//#include <glaux.h>

/////////////////////////////// CGLTexture class
class CGLTexture {
public:
    CGLTexture();
    CGLTexture(CLog* pLog);
    CGLTexture(string f);
    CGLTexture(CLog* pLog,string f);
    ~CGLTexture();
    string      filename;
    bool        bGL;
    u_long      width;
    u_long      height;
    GLenum      format;
    int         bpp;
    GLuint      glBmap;
    CGLTexture* pNext;
    CLog*       pLog;
    bool        bMadeLog;
    CGAF*       pGAF;
    void    Initialize(void);
    GLuint  Create(int x,int y);
    bool    Clear(u_char R,u_char G,u_char B);
    bool    Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b);
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b);
    bool    Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    bool    DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    GLuint  LoadGL(string f);

};

struct CGLTextureList {  CGLTexture *texture; };
// bool    ImageLoad(string filename, Image *image);
// bool    ReLoad(void);
#endif//_DLCS_CGLTEXTURE

