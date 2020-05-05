/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_var.cpp
 **   Class:        CVar
 **   Description:  Var class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/

#include "c_var.h"

#include "dlcs.h"

CVarSet::CVarSet() {
    Init();
    strcpy(szFilename, _DLCS_CVARSET_DEFAULT_FILENAME);
}

CVarSet::CVarSet(CLog *pInLog) {
    Init();
    strcpy(szFilename, "null");
    pLog = pInLog;
}

CVarSet::CVarSet(CLog *pInLog, char *szInFilename) {
    Init();
    strcpy(szFilename, szInFilename);
    pLog = pInLog;
    bLoad();
}

CVarSet::~CVarSet() { map_CVars.clear(); }

void CVarSet::Init() {
    pLog = 0;
    map_CVars.clear();
    cvar_type_map.clear();
    cvar_type_format_map.clear();
    cvar_type_map["CVAR_NULL"]        = CVAR_NULL;
    cvar_type_map["CVAR_BOOL"]        = CVAR_BOOL;
    cvar_type_map["CVAR_STRING"]      = CVAR_STRING;
    cvar_type_map["CVAR_CHAR"]        = CVAR_CHAR;
    cvar_type_map["CVAR_UCHAR"]       = CVAR_UCHAR;
    cvar_type_map["CVAR_INT"]         = CVAR_INT;
    cvar_type_map["CVAR_UINT"]        = CVAR_UINT;
    cvar_type_map["CVAR_LONG"]        = CVAR_LONG;
    cvar_type_map["CVAR_ULONG"]       = CVAR_ULONG;
    cvar_type_map["CVAR_FLOAT"]       = CVAR_FLOAT;
    cvar_type_format_map[CVAR_NULL]   = "-null-";
    cvar_type_format_map[CVAR_BOOL]   = "%d";
    cvar_type_format_map[CVAR_STRING] = "%s";
    cvar_type_format_map[CVAR_CHAR]   = "%d";
    cvar_type_format_map[CVAR_UCHAR]  = "%d";
    cvar_type_format_map[CVAR_INT]    = "%d";
    cvar_type_format_map[CVAR_UINT]   = "%d";
    cvar_type_format_map[CVAR_LONG]   = "%d";
    cvar_type_format_map[CVAR_ULONG]  = "%d";
    cvar_type_format_map[CVAR_FLOAT]  = "%0.3f";
    memset(szFilename, 0, _FILENAME_SIZE);
    strcpy(szFilename, "vars.ini");
}

void CVarSet::Set(const char *szInName, int iInValue) { Set(szInName, va("%d", iInValue)); }
void CVarSet::Set(const char *szInName, const char *szValue) {
    LogEntry(va("CVarSet::set_cvar [%s]=[%s]", szInName, szValue));
    bool bFound;
    int  ivartype;
    ivartype = iGetCVarType(szInName);
    if (ivartype) {
        bFound = 0;
        for (svm_i = map_CVars.begin(); svm_i != map_CVars.end(); ++svm_i) {
            if (dlcs_strcasecmp(((*svm_i).first.c_str()), szInName)) {
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            char *newchars      = new char[256];
            map_CVars[szInName] = newchars;
            strcpy(newchars, szValue);
        } else {
            strcpy((char *)(map_CVars.find(szInName)->second), szValue);
        }

        switch (ivartype) {
            case CVAR_BOOL:
                // LogEntry("[%s]: Data type: CVAR_BOOL", name);
                // (*(bool *)(*svm_i).second) = atoi(value);  //
                // break;
            case CVAR_INT:
                // LogEntry("[%s]: Data type: CVAR_INT", name);
                // if (!bFound) {
                // newint          = new int(atoi(value));
                // map_CVars[name] = newint;
                //} else {
                //(*(int *)(*svm_i).second) = atoi(value);
                //}
                // break;
            case CVAR_UINT:
                // LogEntry("[%s]: Data type: CVAR_UINT", name);
                //(*(unsigned int *)(*svm_i).second) = atoi(value);
                // break;
            case CVAR_LONG:
                // LogEntry("[%s]: Data type: CVAR_LONG", name);
                // (*(long *)(*svm_i).second) = atoi(value);
                // break;
            case CVAR_ULONG:
                // LogEntry("[%s]: Data type: CVAR_ULONG", name);
                //(*(unsigned long *)(*svm_i).second) = atoi(value);
                // break;

            case CVAR_CHAR:   //(*(char *)(*svm_i).second) = value; break;
                              // LogEntry("[%s]: Data type: CVAR_CHAR", name);
            case CVAR_UCHAR:  //(*(char *)(*svm_i).second) = value; break;
                              // LogEntry("[%s]: Data type: CVAR_UCHAR", name);
            case CVAR_FLOAT:  // (*(float *)(*svm_i).second) = value; break;
                              // LogEntry("[%s]: Data type: CVAR_FLOAT", name);
            case CVAR_STRING:
                // LogEntry("[%s]: Data type: CVAR_STRING", name);
                if (!bFound) {
                    char *newchars      = new char[_DLCS_CVAR_VAR_SIZE];
                    map_CVars[szInName] = newchars;
                    strcpy(newchars, szValue);
                } else {
                    strcpy((char *)map_CVars.find(szInName)->second, szValue);
                }
                break;
        }
    } else {
        LogEntry("Invalid datatype designator [%s]", szInName);
        strcpy((char *)map_CVars.find(szInName)->second, "null");
    }
}

int         CVarSet::iGet(const char *szInName) { return atoi((char *)szGet(szInName)); }
bool        CVarSet::bGet(const char *szInName) { return dlcs_istrue((char *)szGet(szInName)); }
const char *CVarSet::szGet(const char *szInName) { return (const char *)map_CVars.find(szInName)->second; }

char *      CVarSet::szGetCVarFormat(int iInType) { return (char *)cvar_type_format_map[iInType].c_str(); }
const char *CVarSet::szGetCVarFormatted(const char *szInFormat, void *cv) { return va(szInFormat, cv); }
const char *CVarSet::szGetCVarTypeString(int iInType) {
    map<string, int>::iterator ii;
    for (ii = cvar_type_map.begin(); ii != cvar_type_map.end(); ++ii) {
        if (((*ii).second) == (iInType)) {
            return (*ii).first.c_str();
        }
    }
    return "null";
}

int CVarSet::iGetCVarType(const char *szInName) {
    vector<string> vt;
    vt = dlcs_explode("_", szInName);
    if (vt.size() > 1) {
        if (dlcs_strcasecmp(vt[0].c_str(), "b")) return CVAR_BOOL;
        if (dlcs_strcasecmp(vt[0].c_str(), "s")) return CVAR_STRING;
        if (dlcs_strcasecmp(vt[0].c_str(), "c")) return CVAR_CHAR;
        if (dlcs_strcasecmp(vt[0].c_str(), "uc")) return CVAR_UCHAR;
        if (dlcs_strcasecmp(vt[0].c_str(), "i")) return CVAR_INT;
        if (dlcs_strcasecmp(vt[0].c_str(), "ui")) return CVAR_UINT;
        if (dlcs_strcasecmp(vt[0].c_str(), "l")) return CVAR_LONG;
        if (dlcs_strcasecmp(vt[0].c_str(), "ul")) return CVAR_ULONG;
        if (dlcs_strcasecmp(vt[0].c_str(), "f")) return CVAR_FLOAT;
    }
    return CVAR_NULL;
}

bool CVarSet::bLoad() { return bLoad(szFilename); }
bool CVarSet::bLoad(const char *szInFilename) {  // LogEntry(va("CVarSet::bLoadCVars() szFilename from class member: %s\n", szFilename));
    strcpy(szFilename, szInFilename);
    FILE *fp;
    char  In[256];
    memset(In, 0, 256);
    vector<string> lin;
    fp = fopen(szFilename, "rt");
    if (!fp) return false;
    while (fgets(In, 255, fp)) {
        In[strlen(In) - 1] = 0;
        lin                = dlcs_explode("=", In);

        if (lin.size() > 1) {
            Set(lin[0].c_str(), (char *)lin[1].c_str());
        }
    }
    fclose(fp);
    return true;
}

bool CVarSet::bSave() { return bSave(szFilename); }
bool CVarSet::bSave(const char *szInFilename) {
    strcpy(szFilename, szInFilename);
    FILE *fout;
    char  Temp[_TEXTNAME_SIZE];
    memset(Temp, 0, _TEXTNAME_SIZE);
    fout = fopen(szFilename, "w");
    if (!fout) return false;
    fputs(va("# %s vars\n", szFilename), fout);
    fputs("# NOTE: Try renaming this file if the program fails.\n", fout);
    for (svm_i = map_CVars.begin(); svm_i != map_CVars.end(); ++svm_i) {
        sprintf(Temp, "%s=%s\n", (*svm_i).first.c_str(), (const char *)(*svm_i).second);
        fputs(Temp, fout);
    }
    return true;
}

bool CVarSet::bRegisterFunction(const char *szInFunctionname, strfunc_t pCFunction) {
    // SFunctionInterface.insert(szInFunctionname, pCFunction);
    map_Functions[szInFunctionname] = pCFunction;  //
    return true;
}

bool CVarSet::bCallFunction(const char *szInFunction) {
    LogEntry("C_VarSet::bCallFunction> (%s)...", szInFunction);

    char szPassedArgs[1024];
    memset(szPassedArgs, 0, 1024);

    int  cQuoteCount = 0;  // bool bQuote2=0;
    char cmd2[1024];
    memset(cmd2, 0, 1024);
    strcpy(cmd2, szInFunction);

    for (unsigned int i = 0; i < strlen(cmd2); i++) {
        switch (cmd2[i]) {
            case '\n':
            case '\r':
            case '\t': cmd2[i] = ' '; break;
            case '"': cQuoteCount++; break;
            case ';':
                if (cQuoteCount & 1) {
                    cmd2[i] = '';  // Check " - Any ; in between quotes will temp change to 0xFF
                    break;
                }  // else bQuote2=true;
                break;
            default: break;
        }
    }

    if (cQuoteCount & 1) return false;  // mismatched quote

    vector<string> ncmd = dlcs_explode(";", cmd2);
    if (ncmd.size() > 1) {
        for (unsigned int i = 0; i < ncmd.size(); i++) {
            bCallFunction((char *)ncmd[i].c_str());
        }
        return false;
    }

    for (unsigned int i = 0; i < strlen(cmd2); i++) {
        switch (cmd2[i]) {
            case '': cmd2[i] = ';'; break;
        }
    }

    vector<string> narg = dlcs_explode(" ", cmd2);

    if (narg.size() > 0) {
        if (map_Functions.find((char *)narg[0].c_str()) != map_Functions.end()) {
            if (narg.size() > 0) {
                memset(szPassedArgs, 0, 1024);
                strcpy(szPassedArgs, narg[1].c_str());
                if (narg.size() > 1) {
                    for (unsigned int i = 2; i < narg.size(); i++) {
                        strcat(szPassedArgs, va(" %s", narg[i].c_str()));
                    }
                } else
                    strcpy(szPassedArgs, "(null)");
                // SFunctionInterface.searchAndCall<void>(narg[0].c_str());
                // map_Functions.find(narg[0].c_str())->second(szPassedArgs);
            }
            return true;
        }
    }
    return false;
}
/*
void CVarSet::test_var_func1(void) {
    std::cout << "inside fun1\n";
    LogEntry("Inside func1");
}

int CVarSet::i_test_var_func2() {
    std::cout << "inside fun2\n";
    LogEntry("Inside func2");
    return 2;
}

int CVarSet::i_test_var_func3(int a) {
    std::cout << "inside fun3\n";
    LogEntry("Inside func3");
    return a;
}

vector<int> CVarSet::vi_test_var_func4() {
    std::cout << "inside fun4\n";
    LogEntry("Inside func4");
    std::vector<int> v(4, 100);
    return v;
}
*/

// void CVarSet::RegFunc(const char *szInName, void *func) { map_Functions[name] = (void (*)(const std::string &))func; }
// void CVarSet::RegVar(const char *szInName, void *var) { map_CVars[name] = var; }
// void CVarSet::RegInt(const char *szInName, int x) { intmap[name] = x; }

/*
void *CVarSet::get_cvar(const char *szInName, char *varout) {
    int ivartype;
    ivartype = get_cvartype(name);
    strcpy(varout, "NULL");
    switch (ivartype) {
        case CVAR_BOOL: strcpy(varout, va(get_cvarformat(ivartype), (*(bool *)map_CVars.find(name)->second))); break;
        case CVAR_INT: strcpy(varout, va(get_cvarformat(ivartype), (*(int *)map_CVars.find(name)->second))); break;
        case CVAR_UINT: strcpy(varout, va(get_cvarformat(ivartype), (*(unsigned int *)map_CVars.find(name)->second))); break;
        case CVAR_CHAR: strcpy(varout, va(get_cvarformat(ivartype), (*(char *)map_CVars.find(name)->second))); break;
        case CVAR_UCHAR: strcpy(varout, va(get_cvarformat(ivartype), (*(unsigned char *)map_CVars.find(name)->second))); break;
        case CVAR_FLOAT: strcpy(varout, va(get_cvarformat(ivartype), (*(float *)map_CVars.find(name)->second))); break;
        case CVAR_LONG: strcpy(varout, va(get_cvarformat(ivartype), (*(long *)map_CVars.find(name)->second))); break;
        case CVAR_ULONG: strcpy(varout, va(get_cvarformat(ivartype), (*(unsigned long *)map_CVars.find(name)->second))); break;
        case CVAR_STRING: strcpy(varout, va(get_cvarformat(ivartype), (char *)(map_CVars.find(name)->second))); break;
        default: strcpy(varout, "UNKNOWN"); break;
    }
    return (const char *)varout;
}
*/

// void *CVarSet::get_cvar(const char *szInName) {
//    dlcsm_make_str(szTemp);
//     return get_cvar(name, szTemp);
// }

// bool CVarSet::get_cvar_s(const char *szInName) {  //}, char *szReturnVal) {
// LogEntry(va("CVarSet::get_cvar_s(%s)", name));
// return (const char *)get_cvar(name);  //
// return true;
//}

/*
void        C_CONS::set_cvar(char *szInName, char *value) {}
const char *C_CONS::get_cvar(char *szInName) {}
void        C_CONS::get_cvar(char *szInName, char *value) {}
int         C_CONS::get_cvartype(const char *s) { return 0; }
const char *C_CONS::get_cvartype_string(int t) { return 0; }
const char *C_CONS::get_cvarformatted(const char *f, void *cv) { return 0; }
char *      C_CONS::get_cvarformat(int t) { return 0; }
*/