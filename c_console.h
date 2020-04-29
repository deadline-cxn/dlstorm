/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_console.h
 **   Class:        C_CONS
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_C_CONS
#define _DLCS_C_CONS

// #include <string>
// #include <vector>
// using std::string;
// using std::vector;

#include "c_gaf.h"
#include "c_log.h"
#include "dlcs.h"

#define MAX_CONSOLE_BUFFER         1024
#define MAX_CONSOLE_MULTI_COMMANDS 256
#define MAX_CONSOLE_INPUT_BUFFER   1024

class C_CONS {
   public:
    C_CONS();
    C_CONS(CLog *pInLog);
    C_CONS(CGAF *pInGAF, CLog *pInLog);
    ~C_CONS();

    unsigned int iLines;
    CLog *       pLog;
    CGAF *       pGAF;

    bool        Init(void);
    void        AddLine(const char *fmt, ...);
    void        _Execute(const char *cmd);
    void        set_cvar(char *name, char *value);
    void        get_cvar(char *name, char *value);
    int         get_cvartype(const char *s);
    const char *get_cvartype_string(int t);
    const char *get_cvarformatted(const char *f, void *cv);
    char *      get_cvarformat(int t);

    void RegFunc(const char *name, void *func);
    void RegVar(const char *name, void *var);
    void RegInt(const char *name, int x);

    // function map
    typedef void                     strfunc_t(const string &);
    typedef map<string, strfunc_t *> strfmap_t;
    strfmap_t                        funcmap;

    // cvar map
    typedef void *                  strvar_t;
    typedef map<string, strvar_t>   strvarmap_t;
    map<string, strvar_t>::iterator svm_i;
    strvarmap_t                     varmap;

    // buffer vector
    vector<string> buf;

    // cvar type map
    map<string, int> cvar_type_map;

    // cvar type format map
    map<int, string> cvar_type_format_map;

    // string map
    map<string, string> stringmap;

    // int map
    map<string, int> intmap;

    // void                _GlobalStrings(void);
    // void                _GlobalIntegers(void);
    // void                _GlobalFunctions(void);
    // void                _GlobalVars(void);
};

#endif  // _DLCS_C_CONS
