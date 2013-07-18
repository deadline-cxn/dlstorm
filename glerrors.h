/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef B4_GL_ERRORS
#define B4_GL_ERRORS

#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32

#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

extern "C" char *GL_Error(GLenum err);
extern "C" void GL_ClearErrors(void);

#endif
