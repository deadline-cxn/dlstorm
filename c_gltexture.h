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
#include "png.h"
#include "jpeglib.h"
#include "glerrors.h"
/////////////////////////////// TGA Stuff
typedef struct {    GLubyte     Header[12]; } TGAHeader;
typedef struct {    GLubyte		header[6];								// First 6 Useful Bytes From The Header
                    GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
                    GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
                    GLuint		temp;									// Temporary Variable
                    GLuint		type;
                    GLuint		Height;									//Height of Image
                    GLuint		Width;									//Width ofImage
                    GLuint		Bpp; } TGA;
/////////////////////////////// Image struct
typedef struct Image {
    u_long width;
    u_long height;
    int bpp;
    int type;
    char *data;
};
typedef	struct Texture {
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
};

/////////////////////////////// CGLTexture class
class CGLTexture {
public:
    CGLTexture();
    CGLTexture(CLog* pLog);
    CGLTexture(const char* file);
    CGLTexture(CLog* pLog,const char* file);
    ~CGLTexture();

    CLog*       pLog;
    bool        bMadeLog;
    CGAF*       pGAF;
    char        name[FILENAME_SIZE];
    GLuint      bmap;
    unsigned long width;
    unsigned long height;
    char        filename[FILENAME_SIZE];
    CGLTexture* pNext;

    void    Initialize(void);
    GLuint  Create(int x,int y);
    bool    Clear(u_char R,u_char G,u_char B);
    bool    Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b);
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b);
    bool    Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    bool    DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);

    GLuint  Load(const char* file);
    GLuint  LoadFromMem(const char* file);
private:
    GLuint  LoadPNG(const char* file);
    GLuint  LoadTGA(const char* file);
    GLuint  LoadBMP(const char* file);
    GLuint  LoadJPG(const char* file);
    GLuint  LoadTIF(const char* file);
    GLuint  LoadDDS(const char* file);
    GLuint  LoadGIF(const char* file);
    GLuint  LoadBMPFromMem(unsigned char* fb, Image *image);
    GLuint  LoadTextureFromMem(unsigned char *fb);
};

struct CGLTextureList {  CGLTexture *texture; };
// bool    ImageLoad(char *filename, Image *image);
// bool    ReLoad(void);
#endif//_DLCS_CGLTEXTURE

