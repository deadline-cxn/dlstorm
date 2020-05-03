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

CVarSet::~CVarSet() {
    // Destroy cvar map
    /*
    for (auto itr = map_CVars.begin(); itr != map_CVars.end(); itr++) {
        // cout <<
        itr->first;        // << "      " <<
        DEL(itr->second);  // << endl;
    }
    */
    map_CVars.clear();
}
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
void CVarSet::set_cvar(const char *name, int value) { set_cvar(name, va("%d", value)); }
void CVarSet::set_cvar(const char *name, const char *value) {
    LogEntry(va("CVarSet::set_cvar [%s]=[%s]", name, value));
    bool bFound;
    // int *newint;
    int ivartype;
    ivartype = get_cvartype(name);

    if (ivartype) {
        bFound = 0;
        for (svm_i = map_CVars.begin(); svm_i != map_CVars.end(); ++svm_i) {
            if (dlcs_strcasecmp(((*svm_i).first.c_str()), name)) {
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            char *newchars  = new char[256];
            map_CVars[name] = newchars;
            strcpy(newchars, value);
        } else {
            strcpy((char *)(map_CVars.find(name)->second), value);
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
                    char *newchars  = new char[_DLCS_CVAR_VAR_SIZE];
                    map_CVars[name] = newchars;
                    strcpy(newchars, value);
                } else {
                    strcpy((char *)map_CVars.find(name)->second, value);
                }
                break;
        }
    } else {
        LogEntry("Invalid datatype designator [%s]", name);
        strcpy((char *)map_CVars.find(name)->second, "null");
    }
}

// void *CVarSet::get_cvar(const char *name) {
//    dlcsm_make_str(szTemp);
//     return get_cvar(name, szTemp);
// }

// bool CVarSet::get_cvar_s(const char *name) {  //}, char *szReturnVal) {
// LogEntry(va("CVarSet::get_cvar_s(%s)", name));
// return (const char *)get_cvar(name);  //
// return true;
//}

void *CVarSet::get_cvar(const char *name) {  //}, char *szReturnVal) {
    // LogEntry(va("CVarSet::get_cvar(%s)", name));
    int ivartype;
    ivartype = get_cvartype(name);
    /*char szValuez[_FILENAME_SIZE];
    memset(szValuez, 0, _FILENAME_SIZE);
    strcpy(szValuez, (const char *)map_CVars.find(name)->second);
    strcpy(szReturnVal, szValuez);*/
    if ((ivartype) == CVAR_NULL) {
        return 0;
    }

    const char *pTest = (const char *)map_CVars.find(name)->second;

    switch (ivartype) {
        case CVAR_BOOL:
            // return (void *)(const char *)map_CVars.find(name)->second;
            // return (void *)(bool)pTest;  //
            // break;
        case CVAR_INT:
            // return (void *)(int)atoi(((char *)pTest));  //
            // break;
        case CVAR_UINT:
            // return (void *)(unsigned int)atoi(((char *)pTest));  //
            // break;
        case CVAR_CHAR:
            // return (void *)(char)pTest[0];  //
            // break;
        case CVAR_UCHAR:
            // return (void *)(unsigned char)pTest[0];  //
            // break;
        case CVAR_FLOAT:
            // return (void *)(unsigned int)atoi(((char *)map_CVars.find(name)->second));  //
            // break;
        case CVAR_LONG:
            // return (void *)(long)atol(((char *)map_CVars.find(name)->second));  //
            // break;
        case CVAR_ULONG:
            // return (void *)(unsigned long)atol(((char *)map_CVars.find(name)->second));  //
            // break;
        case CVAR_STRING:
            return (void *)(const char *)map_CVars.find(name)->second;  //
            break;
        default: return (void *)0; break;
    }
}
/*
void *CVarSet::get_cvar(const char *name, char *varout) {
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

char *      CVarSet::get_cvarformat(int t) { return (char *)cvar_type_format_map[t].c_str(); }
const char *CVarSet::get_cvarformatted(const char *f, void *cv) { return va(f, cv); }
const char *CVarSet::get_cvartype_string(int t) {
    map<string, int>::iterator ii;
    for (ii = cvar_type_map.begin(); ii != cvar_type_map.end(); ++ii) {
        if (((*ii).second) == (t)) {
            return (*ii).first.c_str();
        }
    }
    return "null";
}
int CVarSet::get_cvartype(const char *s) {
    vector<string> vt;
    vt = dlcs_explode("_", s);
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

bool CVarSet::bLoad() {
    bLoad(szFilename);  //
}

bool CVarSet::bLoad(const char *szInFilename) {
    // LogEntry(va("CVarSet::bLoadCVars() szFilename from class member: %s\n", szFilename));
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
            set_cvar(lin[0].c_str(), (char *)lin[1].c_str());
            // LogEntry(va("[%s]=[%s]);\n", lin[0].c_str(), get_cvar(lin[0].c_str())));
        }
    }
    fclose(fp);
    // LogEntry(va("CVarSet::bLoadCVars() szFilename from class member: %s... FINISHED!\n", szFilename));
    return true;
}

bool CVarSet::bSave() {
    bSave(szFilename);  //
}

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
    // LogEntry("CVarSet::bSave(%s)", szFilename);
}

/*
void        C_CONS::set_cvar(char *name, char *value) {}
const char *C_CONS::get_cvar(char *name) {}
void        C_CONS::get_cvar(char *name, char *value) {}
int         C_CONS::get_cvartype(const char *s) { return 0; }
const char *C_CONS::get_cvartype_string(int t) { return 0; }
const char *C_CONS::get_cvarformatted(const char *f, void *cv) { return 0; }
char *      C_CONS::get_cvarformat(int t) { return 0; }
*/

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

// void CVarSet::RegFunc(const char *name, void *func) { map_Functions[name] = (void (*)(const std::string &))func; }
// void CVarSet::RegVar(const char *name, void *var) { map_CVars[name] = var; }
// void CVarSet::RegInt(const char *name, int x) { intmap[name] = x; }
