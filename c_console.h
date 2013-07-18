/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef _C_CONS
#define _C_CONS
using namespace std;
#include "c_gltexture.h"
#include "c_log.h"
#include "c_gaf.h"
#include <map>
//#define MAX_CONSOLE_BUFFER			1024	// Lines in console scrollback buffer
//#define MAX_CONSOLE_MULTI_COMMANDS	32		// Number of console commands on a single line seperated by ;
//#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again
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
enum ember_convar_types {
    CVAR_NULL=0,
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
class C_CONS {
public:
    C_CONS();
    C_CONS(CLog *pInLog);
    C_CONS(CGAF *pInGAF, CLog *pInLog);
    ~C_CONS();

    CLog                *pLog;
    CGAF                *pGAF;

    bool                Init(void);
    void                Entry(char *fmt, ...);

    void                _Execute(const char *cmd);

    void                set_cvar(char *name, char *value);
    void                get_cvar(char *name, char *value);

    int                 get_cvartype(const char *s);
    const char *        get_cvartype_string(int t);
    const char *        get_cvarformatted(const char *f,void *cv);
    char *              get_cvarformat(int t);

    // void                _GlobalStrings(void);
    // void                _GlobalIntegers(void);

    void                RegFunc(char *name, void *func);
    void                RegVar(char *name, void *var);
    void                RegInt(char *name, int x);

    //void                _GlobalFunctions(void);
    //void                _GlobalVars(void);

    // function map
    typedef void strfunc_t(const string&);
    typedef map<string, strfunc_t*> strfmap_t;
    strfmap_t funcmap;

    // cvar map
    typedef void *strvar_t;
    typedef map<string, strvar_t> strvarmap_t;
    map <string, strvar_t>::iterator svm_i;
    strvarmap_t varmap;

    // buffer vector
    vector<string>      buf;

    // cvar type map
    map <string, int>   cvar_type_map;

    // cvar type format map
    map <int, string>   cvar_type_format_map;

    // string map
    map <string, string> stringmap;

    // int map
    map <string, int>    intmap;

};
#endif // _C_CONSOLE
