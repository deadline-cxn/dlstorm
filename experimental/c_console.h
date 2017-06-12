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
#include "dlstorm.h"

//#define MAX_CONSOLE_BUFFER			1024	// Lines in console scrollback buffer
//#define MAX_CONSOLE_MULTI_COMMANDS	32		// Number of console commands on a single line seperated by ;
//#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again


class C_CONS {
public:
    C_CONS();
    ~C_CONS();
    
    // buffer vector
    vector<string>      buf;

    bool                Init(void);
    void                Entry(char *fmt, ...);
    void                _Execute(const char *cmd);

    //void                RegFunc(char *name, void *func);
    //void                RegVar(char *name, void *var);
    //void                RegInt(char *name, int x);



};
#endif // _DLCS_C_CONS
