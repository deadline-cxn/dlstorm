/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_var.cpp
 **   Class:        CVar
 **   Description:  Var class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_var.h"
CVarSet::CVarSet() { Init(); }
CVarSet::CVarSet(char *infilename)
{
    Init();
    filename.assign(infilename);
}
CVarSet::~CVarSet() { varmap.clear(); }
void CVarSet::Init()
{
    varmap.clear();
    cvar_type_map.clear();
    cvar_type_format_map.clear();
    cvar_type_map["CVAR_NULL"] = CVAR_NULL;
    cvar_type_map["CVAR_BOOL"] = CVAR_BOOL;
    cvar_type_map["CVAR_STRING"] = CVAR_STRING;
    cvar_type_map["CVAR_CHAR"] = CVAR_CHAR;
    cvar_type_map["CVAR_UCHAR"] = CVAR_UCHAR;
    cvar_type_map["CVAR_INT"] = CVAR_INT;
    cvar_type_map["CVAR_UINT"] = CVAR_UINT;
    cvar_type_map["CVAR_LONG"] = CVAR_LONG;
    cvar_type_map["CVAR_ULONG"] = CVAR_ULONG;
    cvar_type_map["CVAR_FLOAT"] = CVAR_FLOAT;
    cvar_type_format_map[CVAR_NULL] = "-null-";
    cvar_type_format_map[CVAR_BOOL] = "%d";
    cvar_type_format_map[CVAR_STRING] = "%s";
    cvar_type_format_map[CVAR_CHAR] = "%d";
    cvar_type_format_map[CVAR_UCHAR] = "%d";
    cvar_type_format_map[CVAR_INT] = "%d";
    cvar_type_format_map[CVAR_UINT] = "%d";
    cvar_type_format_map[CVAR_LONG] = "%d";
    cvar_type_format_map[CVAR_ULONG] = "%d";
    cvar_type_format_map[CVAR_FLOAT] = "%0.3f";
    filename = "vars.ini";
}
void CVarSet::set_cvar(const char *name, int value) { set_cvar(name,va("%d",value));}
void CVarSet::set_cvar(const char *name, const char *value) {
    // printf("CVarSet: Setting %s to %s\n",name,value);
    int ivartype;
    bool bFound;
    int *newint;
    ivartype = get_cvartype(name);
    bFound = 0;
    for (svm_i = varmap.begin(); svm_i != varmap.end(); ++svm_i)
    {
        if (dlcs_strcasecmp(((*svm_i).first.c_str()), name))
        {
            bFound = 1;
            break;
        }
    }
    if (!bFound)
    {
        char *newchars = new char[256];
        varmap[name] = newchars;
        strcpy(newchars, value);
    }
    else
    {
        strcpy((char *)(varmap.find(name)->second), value);
    }
    /*
    switch(ivartype) {
    case CVAR_BOOL:
        (*(bool *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_INT:
        if(!bFound) {
            newint = new int(atoi(value));
            varmap[name]=newint;
        } else {
            (*(int *)(*svm_i).second) = atoi(value);
        }
        break;
    case CVAR_UINT:
        (*(unsigned int *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_CHAR:
        (*(char *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_UCHAR:
        (*(unsigned char *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_FLOAT:
        (*(float *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_LONG:
        (*(long *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_ULONG:
        (*(unsigned long *)(*svm_i).second) = atoi(value);
        break;
    case CVAR_STRING:
        if(!bFound) {
            char *newchars=new char[256];
            varmap[name]=newchars;
            strcpy(newchars,value);
        } else {
            strcpy( (char *)(varmap.find(name)->second), value);
        }
        break;
    }
    */
}
void *CVarSet::get_cvar(const char *name) {
    int ivartype;
    ivartype = get_cvartype(name);
    // strcpy(value, "NULL");
    switch (ivartype) {
    case CVAR_BOOL:
        // strcpy(value, va(get_cvarformat(ivartype), );
        return (void *)(*(bool *)varmap.find(name)->second);
        break;
    case CVAR_INT:
        // strcpy(value, va(get_cvarformat(ivartype), 
        return (void *)(*(int *)varmap.find(name)->second);
        break;
    case CVAR_UINT:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(unsigned int *)varmap.find(name)->second);
        break;
    case CVAR_CHAR:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(char *)varmap.find(name)->second);
        break;
    case CVAR_UCHAR:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(unsigned char *)varmap.find(name)->second);
        break;
    case CVAR_FLOAT:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(float *)varmap.find(name)->second);
        break;
    case CVAR_LONG:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(long *)varmap.find(name)->second);
        break;
    case CVAR_ULONG:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(*(unsigned long *)varmap.find(name)->second);
        break;
    case CVAR_STRING:
        // strcpy(value, va(get_cvarformat(ivartype), ));
        return (void *)(char *)(varmap.find(name)->second);
        break;
    default:
        return (void *)0; // strcpy(value, "UNKNOWN");
        break;
    }
}
const char *CVarSet::get_cvar(const char *name) {
    // char * value;
    // memset(value,0,1024);
    /*
    int ivartype;
    ivartype=get_cvartype(name);
    strcpy(value,"NULL");
    switch(ivartype) {
    case CVAR_BOOL:
        strcpy(value, va(get_cvarformat(ivartype), (*(bool *) varmap.find(name)->second)));
        break;
    case CVAR_INT:
        strcpy(value, va(get_cvarformat(ivartype), (*(int *) varmap.find(name)->second)));
        break;
    case CVAR_UINT:
        strcpy(value, va(get_cvarformat(ivartype), (*(unsigned int *) varmap.find(name)->second)));
        break;
    case CVAR_CHAR:
        strcpy(value, va(get_cvarformat(ivartype), (*(char *) varmap.find(name)->second)));
        break;
    case CVAR_UCHAR:
        strcpy(value, va(get_cvarformat(ivartype), (*(unsigned char *) varmap.find(name)->second)));
        break;
    case CVAR_FLOAT:
        strcpy(value, va(get_cvarformat(ivartype), (*(float *) varmap.find(name)->second)));
        break;
    case CVAR_LONG:
        strcpy(value, va(get_cvarformat(ivartype), (*(long *) varmap.find(name)->second)));
        break;
    case CVAR_ULONG:
        strcpy(value, va(get_cvarformat(ivartype), (*(unsigned long *) varmap.find(name)->second)));
        break;
    case CVAR_STRING:
        strcpy(value, va(get_cvarformat(ivartype), (char *)(varmap.find(name)->second)));
        break;
    default:
        strcpy(value,"UNKNOWN");
        break;
    }
     */
    return (const char *)varmap.find(name)->second;
}
char *CVarSet::get_cvarformat(int t) { return (char *)cvar_type_format_map[t].c_str(); }
const char *CVarSet::get_cvarformatted(const char *f, void *cv) { return va(f, cv); }
const char *CVarSet::get_cvartype_string(int t)
{
    map<string, int>::iterator ii;
    for (ii = cvar_type_map.begin(); ii != cvar_type_map.end(); ++ii)
    {
        if (((*ii).second) == (t))
        {
            return (*ii).first.c_str();
        }
    }
    return "null";
}
int CVarSet::get_cvartype(const char *s)
{
    vector<string> vt;
    vt = dlcs_explode("_", s);
    if (vt.size() > 1)
    {
        if (dlcs_strcasecmp(vt[0].c_str(), "b"))
            return CVAR_BOOL;
        if (dlcs_strcasecmp(vt[0].c_str(), "s"))
            return CVAR_STRING;
        if (dlcs_strcasecmp(vt[0].c_str(), "c"))
            return CVAR_CHAR;
        if (dlcs_strcasecmp(vt[0].c_str(), "uc"))
            return CVAR_UCHAR;
        if (dlcs_strcasecmp(vt[0].c_str(), "i"))
            return CVAR_INT;
        if (dlcs_strcasecmp(vt[0].c_str(), "ui"))
            return CVAR_UINT;
        if (dlcs_strcasecmp(vt[0].c_str(), "l"))
            return CVAR_LONG;
        if (dlcs_strcasecmp(vt[0].c_str(), "ul"))
            return CVAR_ULONG;
        if (dlcs_strcasecmp(vt[0].c_str(), "f"))
            return CVAR_FLOAT;
    }
    return CVAR_NULL;
}
bool CVarSet::bLoad(void)
{
    FILE *fp;
    char In[256];
    char temp[1024];
    float f;
    vector<string> lin;
    fp = fopen(filename.c_str(), "rt");
    if (!fp)
        return false;
    while (fgets(In, 255, fp))
    {
        In[strlen(In) - 1] = 0;
        lin = dlcs_explode("=", In);
        if (lin.size() > 1)
        {
            set_cvar(lin[0].c_str(), (char *)lin[1].c_str());
        }
    }
    fclose(fp);
    return true;
}
bool CVarSet::bSave(void)
{
    FILE *fout;
    char Temp[256];
    fout = fopen(filename.c_str(), "w");
    if (!fout)
        return false;
    fputs("# vars\n", fout);
    fputs("# NOTE: Try renaming this file if the program fails.\n", fout);
    for (svm_i = varmap.begin(); svm_i != varmap.end(); ++svm_i)
    {
        sprintf(Temp, "%s=%s\n", (*svm_i).first.c_str(), (const char *)(*svm_i).second);
        fputs(Temp, fout);
    }
    return true;
}