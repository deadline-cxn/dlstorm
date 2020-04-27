/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gltexture.cpp
 **   Class:        CGLTexture
 **   Description:  SDL/OpenGL Texture manager class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_gltexture.h"
extern GAF_SCANCALLBACK what(GAFFile_ElmHeader *ElmInfo, LPSTR FullPat);
/////////////////////////////// CGLTexture class
CGLTexture::CGLTexture() {
    Initialize();
    pLog     = new CLog("texture.log");
    bMadeLog = true;
}
CGLTexture::CGLTexture(string f) {
    Initialize();
    pLog     = new CLog("texture.log");
    bMadeLog = true;
    LoadGL(f);
}
CGLTexture::CGLTexture(CLog *pInLog) {
    Initialize();
    if (pInLog) {
        bMadeLog = false;
        pLog     = pInLog;
    }
}
CGLTexture::CGLTexture(CLog *pInLog, string f) {
    Initialize();
    if (pInLog) {
        bMadeLog = false;
        pLog     = pInLog;
    }
    LoadGL(f);
}
CGLTexture::~CGLTexture() {
    if (bMadeLog) dlcsm_delete(pLog);
    if (glIsTexture(glBmap)) glDeleteTextures(1, &glBmap);
    glBmap = 0;
}
void CGLTexture::Initialize() {
    bMadeLog = true;
    pLog     = 0;
    pNext    = 0;
    glBmap   = 0;
    filename.clear();
    width  = 0;
    width  = 0;
    height = 0;
    bpp    = 0;
    format = 0;
}
GLuint CGLTexture::Create(int x, int y) {
    width  = x;
    height = y;
    dlcsm_gl_delete(glBmap);
    GLuint *data;
    data = (unsigned int *)new GLuint[((width * height) * 4 * sizeof(unsigned int))];
    memset(data, 0, ((width * height) * 4 * sizeof(unsigned int)));
    glGenTextures(1, &glBmap);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete[] data;
    if (pLog) pLog->_DebugAdd("CGLTexture::Create() -> created a texture!");
    return glBmap;
}
bool CGLTexture::Clear(u_char R, u_char G, u_char B) {
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SDL_GetVideoSurface()->w, 0, SDL_GetVideoSurface()->h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glBegin(GL_QUADS);
    glColor3ub(R, G, B);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(0), float(0), 1.0f);
    glColor3ub(R, G, B);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(width), float(0), 1.0f);
    glColor3ub(R, G, B);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(width), float(height), 1.0f);
    glColor3ub(R, G, B);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(0), float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return true;
}
GLuint CGLTexture::LoadGL(string f) {
    glBmap   = 0;
    filename = f;
    ILuint    imageID;
    ILboolean success;
    ILenum    error;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    success = ilLoadImage(filename.c_str());
    if (success) {
        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);
        if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) iluFlipImage();
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (!success) {
            error = ilGetError();
            pLog->_Add("  CGLTexture::LoadGL() Image conversion failed - IL reports error: %d %s", error, iluErrorString(error));
            return 0;
        }
        width  = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        format = ilGetInteger(IL_IMAGE_FORMAT);
        bpp    = ilGetInteger(IL_IMAGE_BPP) * 8;
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &glBmap);
        glBindTexture(GL_TEXTURE_2D, glBmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    } else {
        error = ilGetError();
        pLog->_Add("  CGLTexture::LoadGL() Image load failed - IL reports error: %d %s", error, iluErrorString(error));
        return 0;
    }
    ilDeleteImages(1, &imageID);
    return glBmap;
}
bool CGLTexture::Draw2d(int x, int y, int x2, int y2, u_char r, u_char g, u_char b) { return Draw2d(x, y, x2, y2, r, g, b, 255, 255, 255); }
bool CGLTexture::Draw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b) { return Draw(x, y, x2, y2, r, g, b, 255, 255, 255); }
bool CGLTexture::Draw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2) {
    if (!glBmap) {
        LoadGL(filename.c_str());
        return false;
    }
    int x3 = (x2 - x);
    int y3 = (y2 - y);
    x      = x / 2;
    y      = (-y / 2) + (SDL_GetVideoSurface()->h / 2);

    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, SDL_GetVideoSurface()->w, 0, SDL_GetVideoSurface()->h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glColor3ub(r, g, b);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(x), float(y - y3), 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(x + x3), float(y - y3), 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(x + x3), float(y), 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(x), float(y), 1);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}
bool CGLTexture::Draw2d(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2) {
    if (!glBmap) {
        LoadGL(filename.c_str());
        return 0;
    }
    int x3 = (x2 - x);
    int y3 = (y2 - y);
    x      = x / 2;
    y      = (-y / 2) + (SDL_GetVideoSurface()->h / 2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SDL_GetVideoSurface()->w, 0, SDL_GetVideoSurface()->h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glColor3ub(r, g, b);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex3d(x, y - y3, 1);
    glTexCoord2d(1, 0);
    glVertex3d(x + x3, y - y3, 1);
    glTexCoord2d(1, 1);
    glVertex3d(x + x3, y, 1);
    glTexCoord2d(0, 1);
    glVertex3d(x, y, 1);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}
bool CGLTexture::DrawRaw(int x, int y, int x2, int y2, u_char r, u_char g, u_char b, u_char r2, u_char g2, u_char b2) {
    int x3 = (x2 - x);
    int y3 = (y2 - y);
    x      = x / 2;
    y      = (-y / 2) + (SDL_GetVideoSurface()->h / 2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(x), float(y - y3), 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(x + x3), float(y - y3), 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(x + x3), float(y), 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(x), float(y), 1);
    glEnd();
    return 1;
}
