/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_console.cpp
 **   Class:        C_CONS
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_console.h"
C_CONS::C_CONS() {
    Init();
}

C_CONS::~C_CONS() {

}
bool C_CONS::Init(void) {
    funcmap.clear();
    buf.clear();
    return 1;
}

void C_CONS::Entry(char *fmt, ...) {
    char ach[512];
    memset(ach,0,512);
    va_list vaz;
    va_start(vaz,fmt);
    vsprintf(ach,fmt,vaz);
    va_end(vaz);
    buf.push_back(ach);
}

svoid C_CONS::_Execute(const char *cmd) {
    int i;
    //,j;
    char temp[1024];
    memset(temp,0,1024);

    int cQuoteCount=0; // bool bQuote2=0;
    char cmd2[1024];
    memset(cmd2,0,1024);
    strcpy(cmd2,cmd);

    for(i=0; i<strlen(cmd2); i++) {
        switch(cmd2[i]) {
        case '\n':
        case '\r':
        case '\t':
            cmd2[i]=' ';
            break;
        case '"':
            cQuoteCount++;
            break;
        case ';':
            if(cQuoteCount&1) {
                cmd2[i]= ''; // Check " - Any ; in between quotes will temp change to 0xFF
                break;
            } // else bQuote2=true;
            break;
        default:
            break;
        }
    }

    if(cQuoteCount&1)  return; // mismatched quote
    vector <string> ncmd=dlcs_explode(";",cmd2);
    if(ncmd.size()>1) {
        for(i=0; i<ncmd.size(); i++) {
            _Execute((char *)ncmd[i].c_str());
        }
        return;
    }

    for(i=0; i<strlen(cmd2); i++) {
        switch(cmd2[i]) {
        case '':
            cmd2[i]=';';
            break;
        }
    }

    vector <string> narg=dlcs_explode(" ",cmd2);

    if(narg.size()>0) {
        if(funcmap.find((char *)narg[0].c_str())!=funcmap.end()) {
            if(narg.size()>0) {
                memset(temp,0,1024);
                strcpy(temp, narg[1].c_str());
                if(narg.size()>1){
                    for(i=2; i<narg.size(); i++) {
                        strcat(temp,va(" %s",narg[i].c_str()));
                    }
                }
                else strcpy(temp,"(null)");
                // dlcs_trim_lf(cmd);
                // dlcs_trim_lf(temp);
                // pLog->_Add("C_CONS::_Execute > [%s] [%s]...",cmd,temp);
                funcmap.find(narg[0].c_str())->second(temp);
            }
            return;
        }
    }
    // pLog->_Add("C_CONS::_Execute 6 > %s...",cmd);
}

/*
void C_CONS::RegFunc(char *name,void *func) {
    funcmap[name]=(void (*)(const std::string&))func;
}
void C_CONS::RegVar(char *name,void *var) {
    varmap[name]=var;
}
void C_CONS::RegInt(char *name,int x) {
    intmap[name]=x;
}
*/