/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         glerrors.h
 **   Description:  OpenGL errors
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_GL_ERRORS
#define _DLCS_GL_ERRORS
#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmreg.h>
#include <mmsystem.h>
#endif  //_MMEDIA
#endif  //_WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
extern "C" char *GL_Error(GLenum err);
extern "C" void  GL_ClearErrors(void);
#endif  // _DLCS_GL_ERRORS
