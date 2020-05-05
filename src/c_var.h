/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_var.h
 **   Class:        CVar
 **   Description:  Var class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_CVAR
#define _DLCS_CVAR

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include "c_log.h"
#include "dlcs.h"

using namespace std;

#define _DLCS_CVARSET_DEFAULT_FILENAME "default_cvar_filename.ini"
#define _DLCS_CVAR_VAR_SIZE            1024

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
/*
typedef void (*voidFunctionType)(void);

struct stFunctionInterface {
    std::map<std::string, std::pair<voidFunctionType, std::type_index>> m1;

    template <typename T>
    void insert(string s1, T f1) {
        auto tt = type_index(typeid(f1));
        m1.insert(std::make_pair(s1, std::make_pair((voidFunctionType)f1, tt)));
    }

    template <typename T, typename... Args>
    T searchAndCall(std::string s1, Args &&... args) {
        auto mapIter = m1.find(s1);
        // chk if not end
        auto mapVal = mapIter->second;

        // auto typeCastedFun = reinterpret_cast<T(*)(Args ...)>(mapVal.first);
        auto typeCastedFun = (T(*)(Args...))(mapVal.first);

        // compare the types is equal or not
        assert(mapVal.second == std::type_index(typeid(typeCastedFun)));
        return typeCastedFun(std::forward<Args>(args)...);
    }
};

template <class... Args>
struct MapHolder {
    static std::map<std::string, int (*)(Args...)> CallbackMap;
};

template <class... Args>
std::map<std::string, int (*)(Args...)> MapHolder<Args...>::CallbackMap;

class Callback {
   public:
    template <class  //
              ... Args>
    void RegisterFunction(std::string name, int (*func)(Args...)) {
        MapHolder<Args...>::CallbackMap[name] = func;
    }

    template <class  //
              ... Args>
    int ExecuteFunction(std::string name, Args &&... args) {
        return MapHolder<Args...>::CallbackMap[name](std::forward<Args>(args)...);
    };
};
*/

typedef void                               strfunc_t(const char *format, ...);
typedef std::map<std::string, strfunc_t *> strfmap_t;

class CVarSet {
   public:
    CVarSet();
    CVarSet(CLog *pInLog);
    CVarSet(CLog *pInLog, char *szInFilename);
    ~CVarSet();

    char                            szFilename[_FILENAME_SIZE];
    bool                            bLoad(void);
    bool                            bLoad(const char *szInFilename);
    bool                            bSave(void);
    bool                            bSave(const char *szInFilename);
    CLog *                          pLog;
    strfmap_t                       map_Functions;  // function map
    typedef void *                  strvar_t;       // cvar map
    typedef map<string, strvar_t>   strvarmap_t;
    map<string, strvar_t>::iterator svm_i;
    strvarmap_t                     map_CVars;
    map<string, int>                cvar_type_map;         // cvar type map
    map<int, string>                cvar_type_format_map;  // cvar type format map
    map<string, string>             stringmap;             // string map
    map<string, int>                intmap;                // int map

    void Init();

    void        Set(const char *szInName, const char *szInValue);
    void        Set(const char *szInName, int iInValue);
    const char *szGet(const char *szInName);
    int         iGet(const char *szInName);
    bool        bGet(const char *szInName);
    int         iGetCVarType(const char *szInName);
    const char *szGetCVarTypeString(int iInType);
    const char *szGetCVarFormatted(const char *szInFormattedString, void *cv);
    char *      szGetCVarFormat(int iInType);
    bool        bRegisterFunction(const char *szInFunctionname, strfunc_t pCFunction);
    bool        bCallFunction(const char *szFunctionnameAndArgs);
    bool        bDeleteFunction(const char *szInFunctionname);

    // cvar map
    // typedef void *                  strvar_t;
    // typedef map<string, strvar_t>   strvarmap_t;
    // map<string, strvar_t>::iterator svm_i;
    // strvarmap_t      varmap;
    //     map<string, int> cvar_type_map;         // cvar type map
    //     map<int, string> cvar_type_format_map;  // cvar type format map
    // typedef void (*strfunc_t)(void);  //(const string &);
    // typedef map<string, strfunc_t *> strfmap_t;
    // strfmap_t map_Functions;
    // void        test_var_func1(void);
    // int         i_test_var_func2();
    // int         i_test_var_func3(int a);
    // vector<int> vi_test_var_func4();
    // void RegFunc(const char *name, void *func);
    // void RegVar(const char *name, void *var);
    //    void RegInt(const char *name, int x);
    // void                _GlobalStrings(void);
    // void                _GlobalIntegers(void);
    // void                _GlobalFunctions(void);
    // void                _GlobalVars(void);
    // stFunctionInterface SFunctionInterface;
    // Callback            SFunctionCallback;
};

#endif  // _DLCS_CVAR
