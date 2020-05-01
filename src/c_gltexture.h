/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_gltexture.h
 **   Class:        CGLTexture
 **   Description:  SDL/OpenGL Texture manager class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_CGLTEXTURE
#define _DLCS_CGLTEXTURE

#include <gl.h>
#include <glu.h>

#include "c_gaf.h"
#include "c_gfx.h"
#include "c_log.h"
#include "dlcs.h"
//#include <glaux.h>

#include <il.h>
#include <ilut.h>

class C_GFX;

/////////////////////////////// CGLTexture class
class CGLTexture {
   public:
    CGLTexture();
    CGLTexture(C_GFX *pInGFX, CGAF *pInGAF, CLog *pInLog, const char *szInFilename);
    ~CGLTexture();

    C_GFX *      pGFX;
    CGAF *       pGAF;
    CLog *       pLog;
    SDL_Texture *pTexture;
    char         szFilename[_FILENAME_SIZE];
    bool         bGL;
    u_long       width;
    u_long       height;
    GLenum       format;
    int          bpp;
    CGLTexture * pNext;
    bool         bMadeLog;

    void Init(void);
    bool Clear(u_char R, u_char G, u_char B);
    bool Draw2d(int x, int y, int x2, int y2, u_char r, u_char g, u_char b);
    bool Draw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b);
    bool Draw2d(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2);
    bool Draw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2);
    bool DrawRaw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2);
    bool Load(const char *szInFilename);

    ///////////////////////////////////////////////////
    // Old Constructors that are no longer necessary
    //   CGLTexture();
    //   CGLTexture(CLog* pLog);
    //   CGLTexture(string f);
    //   CGLTexture(CLog* pLog, string f);
    GLuint glBmap;
    //
    // GLuint Create(int x, int y);
    GLuint LoadGL(const char *szInFilename);
};

struct CGLTextureList {
    CGLTexture *texture;
};

// bool    ImageLoad(string filename, Image *image);
// bool    ReLoad(void);

#endif  //_DLCS_CGLTEXTURE
