/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_glfont.h
 **   Class:        CGLFont
 **   Description:  Fonts for OpenGL
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_CGLFONT
#define _DLCS_CGLFONT
#include "dlstorm.h"
#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"
class CGLTexture;
///////////////////////////// CGLFont class
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
    string      filename;          // file name of the font
    // char        Set1[64];              // name of font 0
    // char        Set2[64];              // name of font 1
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

    GLuint      Load(string f);
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
struct CGLFontList { CGLFont *ob; };
extern "C" int CGLFont_StrLen(const char *string);
#endif//DLCS_CGLFONT

