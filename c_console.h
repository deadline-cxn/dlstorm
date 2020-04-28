/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_console.h
 **   Class:        C_CONS
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_C_CONS
#define _DLCS_C_CONS
#include <string>
#include <vector>

#include "dlcs.h"
using std::string;
using std::vector;

#define MAX_CONSOLE_BUFFER         1024
#define MAX_CONSOLE_MULTI_COMMANDS 256
#define MAX_CONSOLE_INPUT_BUFFER   1024

class C_CONS {
   public:
    C_CONS();
    ~C_CONS();

    vector<string> buf;  // buffer vector
    unsigned int   iLines;
    bool           Init(void);
    void           AddLine(const char *fmt, ...);

    // void                _Execute(const char *cmd);
    // void                RegFunc(char *name, void *func);
    // void                RegVar(char *name, void *var);
    // void                RegInt(char *name, int x);
};
#endif  // _DLCS_C_CONS
