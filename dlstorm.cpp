/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/
#include "dlstorm.h"
#ifdef _DL_INCLUDE_LOG
CLog* p_Log;
void dLog(const char *fmt, ...) {
    if(!p_Log) return;
    char ach[1024];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    p_Log->_Add(ach);
}
#endif
// __stdcall DllMain() { return 0; }
////////////////////////////////////////////////////////////////////////////////////////////////
vector <string> dlcs_explode(const string &delimiter, const string &str) {
    vector <string> arr;
    arr.clear();
    int strleng=str.length();
    int delleng=delimiter.length();
    if(delleng==0)
        return arr;
    int i=0;
    int k=0;
    while(i<strleng) {
        int j=0;
        while(i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if(j==delleng) {
            arr.push_back( str.substr(k,i-k) );
            i+=delleng;
            k=i;
        } else {
            i++;
        }
    }
    arr.push_back( str.substr(k,i-k) );
    return arr;

}
////////////////////////////////////////////////////////////////////////////////////////////////
char* dlcs_get_filetype(char* x, char* in) {
    vector <string> ft;
    ft=dlcs_explode(".",in);
    ft.size();
    strcpy(x,ft[ft.size()].c_str());
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_istrue(char *text) {
    if(dlcs_strcasecmp(text,"on"))      return true;
    if(dlcs_strcasecmp(text,"1"))       return true;
    if(dlcs_strcasecmp(text,"true"))    return true;
    if(dlcs_strcasecmp(text,"yes"))     return true;
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
vector <string> dlcs_dir_to_vector(char *szDir, char *szWildCard) {
    vector <string> diro;
    diro.clear();


#ifdef _WINDOWS_
    //FILE *fp;
    HANDLE          dirsearch;  // Directory handle for reading directory information
    WIN32_FIND_DATA FileData;   // WIN32_FIND_DATA structure needed for reading directory information
    char szCurrentDir[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH,szCurrentDir);
    SetCurrentDirectory(szDir);
    dirsearch = FindFirstFile( szWildCard, &FileData );
    if(dirsearch==INVALID_HANDLE_VALUE) return diro;

    //fp=fopen(szFile,"wt");
    //if(!fp) return false;

    while(GetLastError() != ERROR_NO_MORE_FILES) {
        if(!strcmp(FileData.cFileName,".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        if(!strcmp(FileData.cFileName,"..")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }

        if(!strcmp(FileData.cFileName,".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }

        if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            diro.push_back( va("<DIR> %s",FileData.cFileName) );
            //fputs("<DIR>.",fp);
            //fputs(FileData.cFileName,fp);
            //fputc('\n',fp);
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        diro.push_back(FileData.cFileName);
        //fputs(FileData.cFileName,fp);
        //fputc('\n',fp);
        FindNextFile(dirsearch, &FileData);
    }
    FindClose(dirsearch);
    SetCurrentDirectory(szCurrentDir);
    //fclose(fp);

#endif

    return diro;

}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard) {
#ifdef _WINDOWS_
    FILE *fp;
    HANDLE          dirsearch;  // Directory handle for reading directory information
    WIN32_FIND_DATA FileData;   // WIN32_FIND_DATA structure needed for reading directory information
    char szCurrentDir[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH,szCurrentDir);
    SetCurrentDirectory(szDir);
    dirsearch = FindFirstFile( szWildCard, &FileData );
    if(dirsearch==INVALID_HANDLE_VALUE) return false;
    fp=fopen(szFile,"wt");
    if(!fp) return false;
    while(GetLastError() != ERROR_NO_MORE_FILES) {
        if(!strcmp(FileData.cFileName,".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }

        if(!strcmp(FileData.cFileName,"..")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }

        if(!strcmp(FileData.cFileName,".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }

        if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            fputs("<DIR>.",fp);
            fputs(FileData.cFileName,fp);
            fputc('\n',fp);

            FindNextFile(dirsearch, &FileData);
            continue;
        }
        fputs(FileData.cFileName,fp);
        fputc('\n',fp);
        FindNextFile(dirsearch, &FileData);
    }
    FindClose(dirsearch);
    SetCurrentDirectory(szCurrentDir);
    fclose(fp);
#endif
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
inline int dlcs_length(const char* s) {
    return s?strlen(s):0;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_trim_lf(char *x) {
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_charreplace(char *str, char cold,char cnew) {
    for(int i=0; i<(int)strlen(str); i++) {
        if(str[i]==cold)
            str[i]=cnew;
    }
    return (str);
}
////////////////////////////////////////////////////////////////////////////////////////////////
char* dlcs_strreplace(char *str, const char* what, const char* to) {
    int i, j, k, m, n, delta;
    int n1, n2, n3;
    char* p, *q;
    if (!str || !what)
        return 0;
    if (!to)
        to = "";
    n1 = dlcs_length(str);
    n2 = dlcs_length(what);
    n = n1 - n2 + 1;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n2 && what[j] == str[i+j]; ++j)
            ;
        if (j == n2) { // found
            n3 = dlcs_length(to);
            delta = n3 - n2;
            m = n1 - i - n2;
            if (delta < 0) { /* move left */
                p = str + (i + n2 + delta);
                q = p - delta;
                for (k = 0; k <= m; ++k)
                    p[k] = q[k];
            } else if (delta > 0) { /* move right */
                q = str + n1 - m;
                p = q + delta;
                for (k = m; k >= 0; --k)
                    p[k] = q[k];
            }
            for (k = 0; k < n3; ++k)
                str[i+k] = to[k];
            return str + i + n3;
        }
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char* dlcs_strreplaceall(char* str, const char* what, const char* to) {
    char* p = str;
    while ((p=dlcs_strreplace(p,what,to)) != 0);
    return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void dlcs_md5_digest(char *str, char *text) { // return a md5 digest of text
    strcpy(str,md5(text).c_str());
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_encrypt(char *text) {
    char ntext[1024];
    memset(ntext,0,1024);
    char ntext2[1024];
    memset(ntext2,0,1024);
    int i;
    strcpy(ntext,text);
    for(i=0; i<(int)strlen(ntext); i++) {
        ntext2[i] = ntext[i]<<2;
        if(ntext2[i]==0) ntext2[i]=1;
        if(ntext2[i]=='|') ntext2[i]=1;
    }
    return (ntext2);
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_decrypt(char *text) {

    return (text);
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_get_time(char *x) {
    struct tm *dc;
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_convert_time(char *x,struct tm* dc) {
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_timestamp(char *x) {
    time_t td;
    time( &td );
    strcpy(x,va("%ld",td));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_readable_timestamp(char *x,char *in) {
    long wtf;
    wtf=atoi(in);
    strcpy(x,ctime(&wtf));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char _vx[1024]; // keep this in the global space TODO: improve this
const char *va(const char *format, ...) {
    memset(_vx,0,1024);
    va_list argptr;
    va_start(argptr, format);
    vsprintf(_vx, format,argptr);
    va_end (argptr);
    return (_vx);
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_hex_to_dec(char *pa) {
    if(pa==0) return 0;
    if(!strlen(pa)) return 0;
    char a;
    int result=0;
    for(int i=0; i<(int)strlen(pa); i++) {
        a=pa[i];
        if(a==' ' || a=='\n' || a==0) break;
        if (a>='0' && a<='9')	a-='0';
        else if (a>='A' && a<='F')	a=a-'A'+10;
        else if (a>='a' && a<='f') a=a-'a'+10;
        else return 255;
        result<<=4;
        result|=a;
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_bin_to_dec(char *pa) {
    if(!pa) return 0;
    if(!strlen(pa)) return 0;
    int result=0;
    char a;
    for(;;) {
        a=*pa++;
        if (a==' ' || a=='\n' || a==0) break;
        if (a!='0' && a!='1') return -1;
        a-='0';
        result<<=1;
        result|=a;
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void dlcs_suspend_power_management(void) {
#ifdef DLCSM_WINDOWS
/*
    TCHAR szPath[MAX_PATH];
    HINSTANCE hInstKernel32 = NULL;
    typedef EXECUTION_STATE
     __typeof__ (*WINAPI LPSETTHREADEXECUTIONSTATE) EXECUTION_STATE
    ( EXECUTION_STATE esFlags );
    .static LPSETTHREADEXECUTIONSTATE pSetThreadExecutionState = NULL;
    GetSystemDirectory(szPath, MAX_PATH);
    lstrcat(szPath, TEXT("\\kernel32.dll"));
      hInstKernel32 = LoadLibrary("kernel32.dll");//szPath);
  if (hInstKernel32 != NULL)
   {
    pSetThreadExecutionState = (LPSETTHREADEXECUTIONSTATE)GetProcAddress(hInstKernel32, "SetThreadExecutionState");
    if( pSetThreadExecutionState != NULL ) p
        SetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_CONTINUOUS );
       FreeLibrary(hInstKernel32);
      }
    */
    // Power management suspended
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////
long dlcs_get_tickcount(void) {
#ifdef _WIN32
    return GetTickCount();
#else
    long curtime;
    struct timeval tp;
    struct timezone tzp;
    static long secbase;
    gettimeofday(&tp,&tzp);
    if(!secbase) {
        secbase = tp.tv_sec;
        return tp.tv_usec/1000;
    }
    curtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;
    return curtime;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_strcasecmp(const char *szOne,const char *szTwo) {
    int rval=0;
#ifdef _WIN32
    if(strcasecmp(szOne,szTwo)==0) rval=1;
#else
    if(strcasecmp(szOne,szTwo)==0) rval=1;
#endif
    return rval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_isdir(char *dir) {
    struct stat st;
    if(stat(dir,&st)==-1) return false;
    if(st.st_mode&S_IFDIR) return true;
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string dlcs_filetype(string pathName){
	// Finds the last persiod character of the string
	int period = pathName.find_last_of(".");
	// I use  + 1 because I don't really need the to include the period
	string ext = pathName.substr(period + 1);
	return ext;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_mkdir(char *szDirectoryName) {
    int returnval=0;
#ifdef _WIN32
    if(_mkdir(szDirectoryName)==0) returnval=1;
#else
    if(mkdir(szDirectoryName,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH)==0) returnval=1;
    //S_ISUID | //S_ISGID | //S_ISVTX | // (S_IREAD) // (S_IWRITE) // (S_IEXEC) // S_IWOTH|
#endif
    return returnval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_chdir(char *szDirectory) {
    int returnval=0;
#ifdef _WIN32
    if(_chdir(szDirectory)==0) returnval=1;
#else
    if(chdir(szDirectory)==0)  returnval=1;
#endif
    return returnval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_getcwd(char *x) {
    getcwd(x,_MAX_PATH);
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_get_os_version(char *x) {
#ifdef _WIN32
    char szTemp[128];
    memset(szTemp,0,128);
    char szTemp2[128];
    memset(szTemp2,0,128);
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) ) {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) {
            strcpy(x,"unknown");
            return x;
        }
    }
    switch (osvi.dwPlatformId) {
    case VER_PLATFORM_WIN32_NT:
        if(osvi.dwMajorVersion <= 4 ) strcpy(szTemp,"Windows NT ");
        if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) strcpy(szTemp,"Windows 2000 ");
        if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) strcpy(szTemp,"Windows XP ");
        if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion==0) {
            if(osvi.wProductType == VER_NT_WORKSTATION)     strcpy(szTemp,"Windows Server 2008 ");
            else                                            strcpy(szTemp,"Windows Vista ");
        }
        if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion==1) {
            if(osvi.wProductType == VER_NT_WORKSTATION)     strcpy(szTemp,"Windows Server 2008 R2 ");
            else                                            strcpy(szTemp,"Windows 7 ");
        }
        if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion==2) {
            if(osvi.wProductType == VER_NT_WORKSTATION)     strcpy(szTemp,"Windows 8 ");
            else                                            strcpy(szTemp,"Windows Server 2012 ");
        }

        sprintf(szTemp2,"version %d.%d %s (Build %d)",
                (int)osvi.dwMajorVersion,
                (int)osvi.dwMinorVersion,
                osvi.szCSDVersion,
                (int)osvi.dwBuildNumber & 0xFFFF);
        strcat(szTemp,szTemp2);
        break;
    case VER_PLATFORM_WIN32_WINDOWS:
        if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) sprintf(szTemp,"Windows 95 %s",osvi.szCSDVersion);
        if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)sprintf(szTemp,"Windows 98 %s",osvi.szCSDVersion);
        if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) strcpy(szTemp,"Windows ME");
        break;
    }
    strcpy(x,szTemp);
    return x;
#else
    strcpy(x,CPUSTRING);
    return x;
#endif
}
char *dlcs_get_hostname(char *x) {
    gethostname(x, HOST_NAME_MAX);
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_get_ipaddress(char *x) {
    strcpy(x,"127.0.0.1"); // TODO: Add actual computer ip address here
    dlcsm_make_str(tmp);
    dlcs_get_hostname(tmp);
    struct hostent *phe = gethostbyname(tmp);
    if (phe == 0) {
        strcpy(x,"ERROR");
        return x;
    }
    // strcpy(x,"IP:");
    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        strcpy(x,inet_ntoa(addr));
        return x;
    }


    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
