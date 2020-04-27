/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_glfont.cpp
 **   Class:        CGLFont
 **   Description:  Fonts for OpenGL
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_glfont.h"
/////////////////////////////////////// Utility function
extern "C" int CGLFont_StrLen(const char *string) {
    int  i, j = 0;
    char ch[2];
    for (i = 0; i < (int)strlen(string); i++) {
        if (string[i] == '^') {
            i++;
            if (string[i] == 0) return j;
            if (string[i] == '^') {
                j++;
                ch[0] = string[i];
                ch[1] = 0;
            }
            if (string[i] == '>') {
                i += 6;
            }
        } else {
            j++;
            ch[0] = string[i];
            ch[1] = 0;
        }
    }
    return j;
}
/////////////////////////////////////// CGLFont class
CGLFont::CGLFont() {
    pNext    = 0;
    pFontTex = 0;
    r        = 220;
    g        = 220;
    b        = 220;
    width    = 8.5f;
    height   = 8.5f;
    // memset(Set1,0,64);
    // memset(Set2,0,64);
    // memset(szFile,0,1024);
    filename.clear();
}
CGLFont::CGLFont(CLog *pInLog) {
    pNext    = 0;
    pLog     = pInLog;
    pFontTex = 0;
    r        = 220;
    g        = 220;
    b        = 220;
    width    = 8.5f;
    height   = 8.5f;
    filename.clear();
}
CGLFont::CGLFont(CGAF *pInGAF, CLog *pInLog) {
    pNext    = 0;
    pGAF     = pInGAF;
    pLog     = pInLog;
    pFontTex = 0;
    r        = 220;
    g        = 220;
    b        = 220;
    width    = 8.5f;
    height   = 8.5f;
    filename.clear();
}
CGLFont::CGLFont(char *fn) {
    pNext    = 0;
    pFontTex = 0;
    Load(fn);
    r      = 120;
    g      = 120;
    b      = 120;
    width  = 8.5f;
    height = 8.5f;
    filename.clear();
}
CGLFont::~CGLFont() { Kill(); }
GLuint CGLFont::Load(string f) {  // Build Our Font Display List
    float cx, cy;
    int   loop;
    filename = f;
    pFontTex = new CGLTexture(pLog, filename);
    if (!pFontTex->glBmap) dlcsm_delete(pFontTex);
    if (!pFontTex) return 0;
    pFontList = glGenLists(256);                       // Creating 256 Display Lists
    glBindTexture(GL_TEXTURE_2D, pFontTex->glBmap);    // Select Our Font Texture
    for (loop = 0; loop < 256; loop++) {               // Loop Through All 256 Lists
        cx = float(loop % 16) / 16.0f;                 // X Position Of Current Character
        cy = float(loop / 16) / 16.0f;                 // Y Position Of Current Character
        glNewList(pFontList + loop, GL_COMPILE);       // Start Building A List
        glBegin(GL_QUADS);                             // Use A Quad For Each Character
        glTexCoord2f(cx, 1 - cy - 0.0625f);            // Texture Coord (Bottom Left)
        glVertex2i(0, 0);                              // Vertex Coord (Bottom Left)
        glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);  // Texture Coord (Bottom Right)
        glVertex2i(16, 0);                             // Vertex Coord (Bottom Right)
        glTexCoord2f(cx + 0.0625f, 1 - cy);            // Texture Coord (Top Right)
        glVertex2i(16, 16);                            // Vertex Coord (Top Right)
        glTexCoord2f(cx, 1 - cy);                      // Texture Coord (Top Left)
        glVertex2i(0, 16);                             // Vertex Coord (Top Left)
        glEnd();                                       // Done Building Our Quad (Character)
        glTranslated(16, 0, 0);                        // Move To The Right Of The Character
        glEndList();                                   // Done Building The Display List
    }                                                  // Loop Until All 256 Are Built
    return pFontTex->glBmap;
}
GLvoid CGLFont::Kill() {
    if (pFontList) {
        glDeleteLists(pFontList, 256);
        pFontList = 0;
    }
    dlcsm_delete(pFontTex);
}
GLvoid CGLFont::SinPrint(GLint x, GLint y, const char *string, int set, u_char r, u_char g, u_char b) {}
GLvoid CGLFont::PrintSolid(GLint x, GLint y, const char *string, u_char nr, u_char ng, u_char nb) {
    int  i, j = 0;
    char ch[2];
    for (i = 0; i < (int)strlen(string); i++) {
        if (string[i] == '^') {
            i++;
            if (string[i] == 0) return;
            if (string[i] == '^') {
                j++;
                ch[0] = string[i];
                ch[1] = 0;
                RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, 1, nr, ng, nb);
            }
            if (string[i] == '>') {
                i += 6;
            }
        } else {
            j++;
            ch[0] = string[i];
            ch[1] = 0;
            RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, 1, nr, ng, nb);
        }
    }
}
GLvoid CGLFont::Print(GLint x, GLint y, const char *string, int set) {
    int  i, j;
    char ch[2];
    j = 0;
    u_char nr, ng, nb;
    bool   shadowed = 0;
    bool   bolded   = 0;
    nr              = r;
    ng              = g;
    nb              = b;

    for (i = 0; i < (int)strlen(string); i++) {
        if (string[i] == '^') {
            i++;
            switch (string[i]) {
                case 0:
                case '\n': return;
                case '0':
                    nr = 0;
                    ng = 0;
                    nb = 0;
                    break;
                case '1':
                    nr = 255;
                    ng = 255;
                    nb = 255;
                    break;
                case '2':
                    nr = 0;
                    ng = 0;
                    nb = 255;
                    break;
                case '3':
                    nr = 0;
                    ng = 255;
                    nb = 0;
                    break;
                case '4':
                    nr = 255;
                    ng = 0;
                    nb = 0;
                    break;
                case '5':
                    nr = 0;
                    ng = 255;
                    nb = 255;
                    break;
                case '6':
                    nr = 255;
                    ng = 255;
                    nb = 0;
                    break;
                case '7':
                    nr = 255;
                    ng = 0;
                    nb = 255;
                    break;
                case '8':
                    nr = 100;
                    ng = 0;
                    nb = 255;
                    break;
                case '9':
                    nr = 100;
                    ng = 255;
                    nb = 0;
                    break;
                case 'A':
                case 'a':
                    nr = 255;
                    ng = 0;
                    nb = 100;
                    break;
                case 'B':
                case 'b':
                    nr = 100;
                    ng = 255;
                    nb = 100;
                    break;
                case 'C':
                case 'c':
                    nr = 200;
                    ng = 200;
                    nb = 200;
                    break;
                case 'D':
                case 'd':
                    nr = 150;
                    ng = 150;
                    nb = 150;
                    break;
                case 'E':
                case 'e':
                    nr = 100;
                    ng = 100;
                    nb = 100;
                    break;
                case 'F':
                case 'f':
                    nr = 50;
                    ng = 50;
                    nb = 50;
                    break;
                case 'G':
                case 'g':
                    nr = 50;
                    ng = 0;
                    nb = 0;
                    break;
                case 'H':
                case 'h':
                    nr = 0;
                    ng = 50;
                    nb = 0;
                    break;
                case 'I':
                case 'i':
                    nr = 0;
                    ng = 0;
                    nb = 50;
                    break;
                case 'J':
                case 'j':
                    nr = 50;
                    ng = 0;
                    nb = 50;
                    break;
                case 'K':
                case 'k':
                    nr = 50;
                    ng = 50;
                    nb = 0;
                    break;
                case 'L':
                case 'l':
                    nr = 0;
                    ng = 50;
                    nb = 50;
                    break;

                case '>':
                    // get the 6 character hex code for the colors
                    ch[0] = string[i + 1];
                    ch[1] = string[i + 2];
                    ch[2] = 0;
                    nr    = dlcs_hex_to_dec(ch);
                    ch[0] = string[i + 3];
                    ch[1] = string[i + 4];
                    ch[2] = 0;
                    ng    = dlcs_hex_to_dec(ch);
                    ch[0] = string[i + 5];
                    ch[1] = string[i + 6];
                    ch[2] = 0;
                    nb    = dlcs_hex_to_dec(ch);
                    i     = i + 6;
                    break;

                case '^':
                    j++;
                    ch[0] = string[i];
                    ch[1] = 0;
                    if (bolded) BoldPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, 0, 0, 0);
                    if (shadowed) {
                        RawPrint((GLint)(x + (j * (width)) + 1), (GLint)y + 1, ch, set, 0, 0, 0);
                        RawPrint((GLint)(x + (j * (width)) + 2), (GLint)y + 2, ch, set, 0, 0, 0);
                    }
                    RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, nr, ng, nb);
                    break;

                case '+':
                    i++;
                    j++;

                    if (!shadowed)
                        shadowed = true;
                    else
                        shadowed = false;

                    ch[0] = string[i];
                    ch[1] = 0;

                    if (ch[0] != '^') {
                        if (bolded) BoldPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, 0, 0, 0);
                        if (shadowed) {
                            RawPrint((GLint)(x + (j * (width)) + 1), (GLint)y + 1, ch, set, 0, 0, 0);
                            RawPrint((GLint)(x + (j * (width)) + 2), (GLint)y + 2, ch, set, 0, 0, 0);
                        }
                        RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, nr, ng, nb);
                    } else {
                        i--;
                        j--;
                    }
                    break;

                case '&':
                    i++;
                    j++;

                    if (!bolded)
                        bolded = true;
                    else
                        bolded = false;

                    ch[0] = string[i];
                    ch[1] = 0;

                    if (ch[0] != '^') {
                        if (bolded) BoldPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, 0, 0, 0);
                        if (shadowed) {
                            RawPrint((GLint)(x + (j * (width)) + 1), (GLint)y + 1, ch, set, 0, 0, 0);
                            RawPrint((GLint)(x + (j * (width)) + 2), (GLint)y + 2, ch, set, 0, 0, 0);
                        }
                        RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, nr, ng, nb);
                    } else {
                        i--;
                        j--;
                    }
                    break;

                default:
                    nr = 255;
                    ng = 255;
                    nb = 0;
                    break;
            }
        } else {
            j++;
            ch[0] = string[i];
            ch[1] = 0;
            if (bolded) BoldPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, 0, 0, 0);
            if (shadowed) RawPrint((GLint)(x + (j * (width)) + 2), (GLint)y + 2, ch, set, 0, 0, 0);
            RawPrint((GLint)(x + (j * (width))), (GLint)y, ch, set, nr, ng, nb);
        }
    }
}
GLvoid CGLFont::Stuff(GLenum target, GLint x, GLint y, const char *string, int set, u_char r, u_char g, u_char b) {
    GLint tx, ty, sx, sy, sw, sh;
    tx = 10;
    ty = 10;
    sx = 32;
    sy = 32;
    sw = 32;
    sh = 32;
    glColor3ub(r, g, b);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, pFontTex->glBmap);  // Select Our Font Texture
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslated(x, y, 0);
    glCopyTexSubImage2D(target, 0, tx, ty, sx, sy, sw, sh);
    glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
    glPopMatrix();                // Restore The Old Projection Matrix
    glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix
    glPopMatrix();                // Restore The Old Projection Matrix
    glEnable(GL_DEPTH_TEST);      // Enables Depth Testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPopMatrix();
}
GLvoid CGLFont::RawPrint(GLint x, GLint y, const char *string, int wset, u_char r, u_char g, u_char b) {
    if (!pFontTex) return;
    if (!pFontTex->glBmap) {
        pFontTex->LoadGL(va("%s.png", filename.c_str()));
    }
    y = (-y) + (SDL_GetVideoSurface()->h) - 16;
    if (wset < 2) {
        glDisable(GL_DEPTH_TEST);                                              // Disables Depth Testing
        glMatrixMode(GL_PROJECTION);                                           // Select The Projection Matrix
        glPushMatrix();                                                        // Store The Projection Matrix
        glLoadIdentity();                                                      // Reset The Projection Matrix
        gluOrtho2D(0, SDL_GetVideoSurface()->w, 0, SDL_GetVideoSurface()->h);  // Set Up An Ortho Screen
        glMatrixMode(GL_MODELVIEW);                                            // Select The Modelview Matrix
        glPushMatrix();                                                        // Store The Modelview Matrix
        glColor3ub(r, g, b);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, pFontTex->glBmap);  // Select Our Font Texture
        glEnable(GL_TEXTURE_2D);
        glLoadIdentity();
        glTranslated(x, y, 0);
        glListBase(pFontList - 32 + (128 * wset));     // Choose The Font Set (0 or 1)
        glCallLists(strlen(string), GL_BYTE, string);  // Write The Text To The Screen
        glMatrixMode(GL_PROJECTION);                   // Select The Projection Matrix
        glPopMatrix();                                 // Restore The Old Projection Matrix
        glMatrixMode(GL_MODELVIEW);                    // Select The Modelview Matrix
        glPopMatrix();                                 // Restore The Old Projection Matrix
        glEnable(GL_DEPTH_TEST);
    }
}
GLvoid CGLFont::BoldPrint(GLint x, GLint y, const char *string, int wset, u_char r, u_char g, u_char b) {
    RawPrint(x - 1, y - 1, string, wset, r, g, b);
    RawPrint(x - 1, y, string, wset, r, g, b);
    RawPrint(x - 1, y + 1, string, wset, r, g, b);
    RawPrint(x, y - 1, string, wset, r, g, b);
    RawPrint(x, y + 1, string, wset, r, g, b);
    RawPrint(x + 1, y - 1, string, wset, r, g, b);
    RawPrint(x + 1, y, string, wset, r, g, b);
    RawPrint(x + 1, y + 1, string, wset, r, g, b);
}
