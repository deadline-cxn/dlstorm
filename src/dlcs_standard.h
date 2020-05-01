/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs_standard.h
 **   Description:  Standard Defines and Includes
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_STANDARD
#define _DLCS_STANDARD

/////////////////////////////////////////////////////////////////
// Standard Defines and Includes

#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "md5.h"

////////////////////////////////////////////////////////////////////////////////////
// System independant

typedef struct {
    float x, y, z;
} dlcs_V3;
typedef struct {
    float r, g, b;
} dlcs_C3;
typedef struct {
    float u, v;
} dlcs_TC;

#ifdef _DLCS_CONSOLE
#endif  // _DLCS_CONSOLE

#ifdef _DLCS_SDL
#include "SDL.h"
#endif  // _DLCS_SDL

#ifdef _DLCS_OPENGL
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "glerrors.h"
#endif  // _DLCS_OPENGL

#ifdef _DLCS_DEVIL
#define ILUT_USE_OPENGL
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#endif  // _DLCS_DEVIL

#endif  // _DLCS_STANDARD
