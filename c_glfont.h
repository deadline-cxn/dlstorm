/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef B4_GL_FONT
#define B4_GL_FONT

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

#include "c_gaf.h"
#include "c_log.h"

class CGLTexture;

class CGLFont {
public:
    CGLFont();
    CGLFont(CLog *pLog);
    CGLFont(CGAF *pGAF, CLog *pLog);
    CGLFont(char *fn);
    ~CGLFont();

    CGAF*       pGAF;
    CLog*       pLog;
    CGLFont*    pNext;
    int         iWhich;
    char        szFile[1024];          // file name of the font
    char        Set1[64];              // name of font 0
    char        Set2[64];              // name of font 1
    GLfloat     width;              // specify the width of one drawn character
    GLfloat     height;             // specify the height of one drawn character
    u_char      r;                   // default r color
    u_char      g;                   // default g color
    u_char      b;                   // default b color
    GLuint      pFontList;          // Base Display List For The Font

    // GLuint      pFontMaskList;      // Base Display List For The Font

    CGLTexture* pFontTex;

    //GLuint    pFontTexture;       // Storage For Our Font Texture
    //GLuint    pFontMaskTexture;   // Storage For Our Font Texture

    bool        Load(const char *file);
    GLvoid      Reload();
    GLvoid      Kill();

    GLvoid PrintSolid(GLint x, GLint y, const char *string, u_char r, u_char g, u_char b); // will print solid color ignoring ^# codes
    GLvoid Print(GLint x, GLint y,const char *string,int set); // will print with ^# codes
    GLvoid SinPrint(GLint x, GLint y, const char *string,int set, u_char r, u_char g, u_char b);
    GLvoid Stuff(GLenum target, GLint x, GLint y, const char *string, int set, u_char r, u_char g, u_char b);
    GLvoid RawPrint(GLint x, GLint y,const  char *string,int set, u_char r, u_char g, u_char b);
    GLvoid BoldPrint(GLint x,GLint y, const char *string,int set, u_char r, u_char g, u_char b);
    GLvoid WhatPrint(GLint x,GLint y, const char *string,int set, u_char r, u_char g, u_char b);


};

//typedef
struct CGLFontList {
    CGLFont *ob;
};
extern "C" int CGLFont_StrLen(const char *string);

#endif//B4_GL_FONT


