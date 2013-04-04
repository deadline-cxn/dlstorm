////////////////////////////////////////////////////
// Console class by Seth Parson
#include "c_console.h"
C_CONS::C_CONS(){
    Init();

}
C_CONS::C_CONS(CLog *pInLog){
    pLog=pInLog;
    Init();
}
C_CONS::C_CONS(CGAF *pInGAF, CLog *pInLog){
    pGAF=pInGAF;
    pLog=pInLog;
    Init();
}
C_CONS::~C_CONS(){

    for( svm_i=varmap.begin(); svm_i!=varmap.end(); ++svm_i)
    {
        if(get_cvartype((*svm_i).first.c_str())==CVAR_STRING)
            delete [] ((*svm_i).second);
        else
            delete ((*svm_i).second);
    }

    varmap.clear();
}
bool C_CONS::Init(void){

    buf.clear();

    funcmap.clear();
    varmap.clear();
    intmap.clear();

    //_GlobalFunctions();
    //_GlobalStrings();
    //_GlobalIntegers();
    //_GlobalVars();

    cvar_type_map.clear();
    cvar_type_map["CVAR_NULL"]  = CVAR_NULL;
    cvar_type_map["CVAR_BOOL"]  = CVAR_BOOL;
    cvar_type_map["CVAR_STRING"]= CVAR_STRING;
    cvar_type_map["CVAR_CHAR"]  = CVAR_CHAR;
    cvar_type_map["CVAR_UCHAR"] = CVAR_UCHAR;
    cvar_type_map["CVAR_INT"]   = CVAR_INT;
    cvar_type_map["CVAR_UINT"]  = CVAR_UINT;
    cvar_type_map["CVAR_LONG"]  = CVAR_LONG;
    cvar_type_map["CVAR_ULONG"] = CVAR_ULONG;
    cvar_type_map["CVAR_FLOAT"] = CVAR_FLOAT;

    cvar_type_format_map.clear();
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
    return 1;
}
void C_CONS::set_cvar(char *name, char *value){
    int ivartype;
    bool bFound;
    int *newint;

    ivartype=get_cvartype(name);

    bFound=0;
    for( svm_i=varmap.begin(); svm_i!=varmap.end(); ++svm_i)
    {
        if( dscc( ((*svm_i).first.c_str()), name) )
        {
            bFound=1;
            break;
        }
    }

    switch(ivartype)
    {
        case CVAR_BOOL:
            (*(bool *)(*svm_i).second) = atoi(value);
            break;

        case CVAR_INT:
                if(!bFound)
                {
                    newint = new int(atoi(value));
                    varmap[name]=newint;
                }
                else
                {
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
                if(!bFound)
                {
                    char *newchars=new char[256];
                    varmap[name]=newchars;
                    strcpy(newchars,value);
                }
                else
                {
                    strcpy( (char *)(varmap.find(name)->second), value);
                }

            break;
    }
}
void C_CONS::get_cvar(char *name, char *value){
    int ivartype;
    ivartype=get_cvartype(name);

    strcpy(value,"NULL");

    switch(ivartype)
    {
        case CVAR_BOOL:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(bool *) varmap.find(name)->second)));
            break;

        case CVAR_INT:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(int *) varmap.find(name)->second)));
                break;

        case CVAR_UINT:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(unsigned int *) varmap.find(name)->second)));
                break;

        case CVAR_CHAR:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(char *) varmap.find(name)->second)));
                break;

        case CVAR_UCHAR:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(unsigned char *) varmap.find(name)->second)));
                break;

        case CVAR_FLOAT:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(float *) varmap.find(name)->second)));
                break;

        case CVAR_LONG:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(long *) varmap.find(name)->second)));
                break;

        case CVAR_ULONG:
            strcpy(value, va(get_cvarformat(ivartype),
                (*(unsigned long *) varmap.find(name)->second)));
                break;

        case CVAR_STRING:
            strcpy(value, va(get_cvarformat(ivartype),
                 ( varmap.find(name)->second)));
            break;

        default:
            strcpy(value,"UNKNOWN");
        break;
    }
}
char * C_CONS::get_cvarformat(int t){
    return (char *)cvar_type_format_map[t].c_str();
}
const char * C_CONS::get_cvarformatted(const char *f,void *cv){
    return va(f,cv);
}
const char * C_CONS::get_cvartype_string(int t){
    map <string, int>::iterator ii;
    for( ii=cvar_type_map.begin(); ii!=cvar_type_map.end(); ++ii)
    {
        if( ( (*ii).second ) == (t) )
        {
            return (*ii).first.c_str();
        }
    }
    return "null";
}
int C_CONS::get_cvartype(const char * s){
    vector <string> vt;
    vt=explode("_",s);
    if(vt.size()>1)
    {
        if(dscc(vt[0].c_str(),"b"))  return CVAR_BOOL;
        if(dscc(vt[0].c_str(),"s"))  return CVAR_STRING;
        if(dscc(vt[0].c_str(),"c"))  return CVAR_CHAR;
        if(dscc(vt[0].c_str(),"uc")) return CVAR_UCHAR;
        if(dscc(vt[0].c_str(),"i"))  return CVAR_INT;
        if(dscc(vt[0].c_str(),"ui")) return CVAR_UINT;
        if(dscc(vt[0].c_str(),"l"))  return CVAR_LONG;
        if(dscc(vt[0].c_str(),"ul")) return CVAR_ULONG;
        if(dscc(vt[0].c_str(),"f"))  return CVAR_FLOAT;
    }
    return CVAR_NULL;
}
void C_CONS::Entry(char *fmt, ...){
    char ach[512];
    memset(ach,0,512);

    va_list vaz;
    va_start(vaz,fmt);
    vsprintf(ach,fmt,vaz);
    va_end(vaz);

    buf.push_back(ach);
}
void C_CONS::RegFunc(char *name,void *func){ funcmap[name]=(void (*)(const std::string&))func; }
void C_CONS::RegVar(char *name,void *var) { varmap[name]=var; }
void C_CONS::RegInt(char *name,int x) { intmap[name]=x; }

void C_CONS::_Execute(const char *cmd){

    pLog->_DebugAdd("C_CONS::_Execute 0 > %s...",cmd);

    int i,j;
    char temp[1024]; memset(temp,0,1024);

    pLog->_DebugAdd("C_CONS::_Execute 1 > %s...",cmd);

    int cQuoteCount=0;
    bool bQuote2=0;
    char cmd2[1024]; memset(cmd2,0,1024);
    strcpy(cmd2,cmd);

    pLog->_DebugAdd("C_CONS::_Execute 2 > %s...",cmd);

    for(i=0;i<strlen(cmd2);i++){
        switch(cmd2[i]){
            case '\n':
            case '\r':
            case '\t':
                cmd2[i]=' ';
                break;
            case '"':
                cQuoteCount++;
                break;
            case ';':
                if(cQuoteCount&1){
                    cmd2[i]= ''; // Check " - Any ; in between quotes will temp change to 0xFF
                    break;
                }
                else
                    bQuote2=true;
                break;
            default:
                break;
        }
    }

    pLog->_DebugAdd("C_CONS::_Execute 3 > %s...",cmd);

    if(cQuoteCount&1)  return; // mismatched quote
    vector <string> ncmd=explode(";",cmd2);
    if(ncmd.size()>1){
        for(i=0;i<ncmd.size();i++){
            _Execute((char *)ncmd[i].c_str());
        }
        return;
    }
    pLog->_DebugAdd("C_CONS::_Execute 4 > %s...",cmd);

    for(i=0;i<strlen(cmd2);i++){
        switch(cmd2[i]){
            case '':
                cmd2[i]=';';
                break;
        }
    }

    pLog->_DebugAdd("C_CONS::_Execute 5 > %s...",cmd);

    vector <string> narg=explode(" ",cmd2);

    if(narg.size()>1){
        pLog->_DebugAdd("C_CONS::_Execute 5 1 > %s...",cmd);

        if(funcmap.find((char *)narg[0].c_str())!=funcmap.end()){

            pLog->_DebugAdd("C_CONS::_Execute 5 2 > %s...",cmd);

            if(narg.size()>0){

                pLog->_DebugAdd("C_CONS::_Execute 5 3 > %s...",cmd);

                memset(temp,0,1024);

                pLog->_DebugAdd("C_CONS::_Execute 5 4 > %s...",cmd);

                strcpy(temp, narg[1].c_str());

                pLog->_DebugAdd("C_CONS::_Execute 5 5 > %s...",cmd);

                if(narg.size()>1)
                for(i=2;i<narg.size();i++){
                    pLog->_DebugAdd("C_CONS::_Execute 5 6 > %s...",cmd);
                    strcat(temp,va(" %s",narg[i].c_str()));
                }

                pLog->_DebugAdd("C_CONS::_Execute 5 7 > %s...",cmd);

                funcmap.find(narg[0].c_str())->second(temp);

                pLog->_DebugAdd("C_CONS::_Execute 5 8 > %s...",cmd);
            }
            return;
        }
    }

    pLog->_DebugAdd("C_CONS::_Execute 6 > %s...",cmd);
}



