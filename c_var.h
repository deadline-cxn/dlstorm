/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_var.h
 **   Class:        CVar
 **   Description:  Var class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_CVAR
#define _DLCS_CVAR

#include "c_log.h"
#include "dlcs.h"

// name convention for the cvars:
// b_  = bool
// s_  = string
// c_  = char
// uc_ = unsigned char
// i_  = int
// ui_ = unsigned int
// l_  = long
// ul_ = unsigned long
// f_  = float

enum convar_types {
    CVAR_NULL = 0,
    CVAR_BOOL,
    CVAR_STRING,
    CVAR_CHAR,
    CVAR_UCHAR,
    CVAR_INT,
    CVAR_UINT,
    CVAR_LONG,
    CVAR_ULONG,
    CVAR_FLOAT,
};

class CVarSet {
   public:
    CVarSet();
    CVarSet(char *infilename);
    ~CVarSet();

    // cvar map
    typedef void *                  strvar_t;
    typedef map<string, strvar_t>   strvarmap_t;
    map<string, strvar_t>::iterator svm_i;
    strvarmap_t                     varmap;
    map<string, int>                cvar_type_map;         // cvar type map
    map<int, string>                cvar_type_format_map;  // cvar type format map
    char                            szFilename[_FILENAME_SIZE];
    bool                            bLoad(void);
    bool                            bSave(void);

    CLog *pLog;

    void        Init();
    void        set_cvar(const char *name, const char *value);
    void        set_cvar(const char *name, int value);
    void        get_cvar(const char *name, const char *value);
    void *      get_cvar(const char *name);
    const char *get_cvar(const char *name, char *varout);
    int         get_cvartype(const char *s);
    const char *get_cvartype_string(int t);
    const char *get_cvarformatted(const char *f, void *cv);
    char *      get_cvarformat(int t);
};

#endif  // _DLCS_CVAR
