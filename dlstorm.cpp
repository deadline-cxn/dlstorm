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

//using namespace std;

vector <string> explode(const string &delimiter, const string &str) {
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

char* dlcs_get_filetype(char* x, char* in) {
    vector <string> ft;
    ft=explode(".",in);
    ft.size();
    strcpy(x,ft[ft.size()].c_str());
    return x;
}

bool sp_istrue(char *text) {
    if(dlcs_strcasecmp(text,"on"))      return true;
    if(dlcs_strcasecmp(text,"1"))       return true;
    if(dlcs_strcasecmp(text,"true"))    return true;
    if(dlcs_strcasecmp(text,"yes"))     return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
vector <string> Dir2Vector(char *szDir, char *szWildCard) {
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
bool Dir2File(char *szDir,char *szFile,char *szWildCard) {

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
inline int length(const char* s) {
    return s?strlen(s):0;
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
    n1 = length(str);
    n2 = length(what);
    n = n1 - n2 + 1;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n2 && what[j] == str[i+j]; ++j)
            ;
        if (j == n2) { // found
            n3 = length(to);
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
void md5_digest(char *str, char *text) { // return a md5 digest of text
    strcpy(str,md5(text).c_str());
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *encrypt(char *text) {
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
char *decrypt(char *text) {

    return (text);
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
char _vx[1024];
const char *va(const char *format, ...) {
    memset(_vx,0,1024);
    va_list argptr;
    va_start (argptr, format);
    //vsprintf (string, format,argptr);
    vsprintf (_vx, format,argptr);
    va_end (argptr);
    //return (string);
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
int b2d(char *pa) {
    return dlcs_bin_to_dec(pa);
}
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
#ifdef _WIN32
    //  TCHAR szPath[MAX_PATH];
    //  HINSTANCE hInstKernel32 = NULL;
    //typedef EXECUTION_STATE
    // __typeof__ (*WINAPI LPSETTHREADEXECUTIONSTATE) EXECUTION_STATE
    //( EXECUTION_STATE esFlags );
    //.static LPSETTHREADEXECUTIONSTATE pSetThreadExecutionState = NULL;
    //GetSystemDirectory(szPath, MAX_PATH);
    //lstrcat(szPath, TEXT("\\kernel32.dll"));
    //  hInstKernel32 = LoadLibrary("kernel32.dll");//szPath);
//   if (hInstKernel32 != NULL)
//   {
    //pSetThreadExecutionState = (LPSETTHREADEXECUTIONSTATE)GetProcAddress(hInstKernel32, "SetThreadExecutionState");
    //if( pSetThreadExecutionState != NULL ) p
//        SetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_CONTINUOUS );
//       FreeLibrary(hInstKernel32);
    //  }
    // Log("Power management suspended");
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////
long getticks(void) {
    return dlcs_get_tickcount();
}
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
int sp_strcmp(char *szOne, char *szTwo) {
    return dlcs_strcasecmp(szOne,szTwo);
}
int dscc(const char *x, const char *y) {
    return dlcs_strcasecmp(x,y);
}
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

bool sp_isdir(char *dir) {
    struct stat st;
    if(stat(dir,&st) == -1) return false;
    if (st.st_mode & S_IFDIR) return true;
    return false;
}

int sp_mkdir(char *szDirectoryName) {
    return dlcs_mkdir(szDirectoryName);
}
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
int sp_chdir(char *szDirectoryName) {
    return dlcs_chdir(szDirectoryName);
}
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
char *sp_getcwd(char *x) {
    return dlcs_getcwd(x);
}
char *dlcs_getcwd(char *x) {
    getcwd(x,_MAX_PATH);
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *getos(char *x) {
    return dlcs_get_os_version(x);
}
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

/* MD5 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org) based on:
   md5.h and md5.c reference implemantion of RFC 1321
   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.
License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.
License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.
RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software. */

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

///////////////////////////////////////////////
// F, G, H and I are basic MD5 functions.
inline MD5::uint4 MD5::F(uint4 x, uint4 y, uint4 z) {
    return x&y | ~x&z;
}
inline MD5::uint4 MD5::G(uint4 x, uint4 y, uint4 z) {
    return x&z | y&~z;
}
inline MD5::uint4 MD5::H(uint4 x, uint4 y, uint4 z) {
    return x^y^z;
}
inline MD5::uint4 MD5::I(uint4 x, uint4 y, uint4 z) {
    return y ^ (x | ~z);
}
// rotate_left rotates x left n bits.
inline MD5::uint4 MD5::rotate_left(uint4 x, int n) {
    return (x << n) | (x >> (32-n));
}
// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
    a = rotate_left(a+ F(b,c,d) + x + ac, s) + b;
}
inline void MD5::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
    a = rotate_left(a + G(b,c,d) + x + ac, s) + b;
}
inline void MD5::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
    a = rotate_left(a + H(b,c,d) + x + ac, s) + b;
}
inline void MD5::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
    a = rotate_left(a + I(b,c,d) + x + ac, s) + b;
}
//////////////////////////////////////////////
// default ctor, just initailize
MD5::MD5() {
    init();
}
//////////////////////////////////////////////
// nifty shortcut ctor, compute MD5 for string and finalize it right away
MD5::MD5(const std::string &text) {
    init();
    update(text.c_str(), text.length());
    finalize();
}
//////////////////////////////
void MD5::init() {
    finalized=false;
    count[0] = 0;
    count[1] = 0;
    // load magic initialization constants.
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}
//////////////////////////////
// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
void MD5::decode(uint4 output[], const uint1 input[], size_type len) {
    for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((uint4)input[j]) | (((uint4)input[j+1]) << 8) |
                    (((uint4)input[j+2]) << 16) | (((uint4)input[j+3]) << 24);
}
//////////////////////////////
// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::encode(uint1 output[], const uint4 input[], size_type len) {
    for (size_type i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = input[i] & 0xff;
        output[j+1] = (input[i] >> 8) & 0xff;
        output[j+2] = (input[i] >> 16) & 0xff;
        output[j+3] = (input[i] >> 24) & 0xff;
    }
}
//////////////////////////////
// apply MD5 algo on a block
void MD5::transform(const uint1 block[blocksize]) {
    uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    decode (x, block, blocksize);
    /// Round 1
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); // 1
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); // 2
    FF (c, d, a, b, x[ 2], S13, 0x242070db); // 3
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); // 4
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); // 5
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a); // 6
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); // 7
    FF (b, c, d, a, x[ 7], S14, 0xfd469501); // 8
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); // 9
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); // 10
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); // 11
    FF (b, c, d, a, x[11], S14, 0x895cd7be); // 12
    FF (a, b, c, d, x[12], S11, 0x6b901122); // 13
    FF (d, a, b, c, x[13], S12, 0xfd987193); // 14
    FF (c, d, a, b, x[14], S13, 0xa679438e); // 15
    FF (b, c, d, a, x[15], S14, 0x49b40821); // 16
    // Round 2
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); // 17
    GG (d, a, b, c, x[ 6], S22, 0xc040b340); // 18
    GG (c, d, a, b, x[11], S23, 0x265e5a51); // 19
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); // 20
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); // 21
    GG (d, a, b, c, x[10], S22,  0x2441453); // 22
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); // 23
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); // 24
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); // 25
    GG (d, a, b, c, x[14], S22, 0xc33707d6); // 26
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); // 27
    GG (b, c, d, a, x[ 8], S24, 0x455a14ed); // 28
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); // 29
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); // 30
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); // 31
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); // 32
    // Round 3
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); // 33
    HH (d, a, b, c, x[ 8], S32, 0x8771f681); // 34
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); // 35
    HH (b, c, d, a, x[14], S34, 0xfde5380c); // 36
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); // 37
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); // 38
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); // 39
    HH (b, c, d, a, x[10], S34, 0xbebfbc70); // 40
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); // 41
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); // 42
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); // 43
    HH (b, c, d, a, x[ 6], S34,  0x4881d05); // 44
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); // 45
    HH (d, a, b, c, x[12], S32, 0xe6db99e5); // 46
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); // 47
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); // 48
    // Round 4
    II (a, b, c, d, x[ 0], S41, 0xf4292244); // 49
    II (d, a, b, c, x[ 7], S42, 0x432aff97); // 50
    II (c, d, a, b, x[14], S43, 0xab9423a7); // 51
    II (b, c, d, a, x[ 5], S44, 0xfc93a039); // 52
    II (a, b, c, d, x[12], S41, 0x655b59c3); // 53
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); // 54
    II (c, d, a, b, x[10], S43, 0xffeff47d); // 55
    II (b, c, d, a, x[ 1], S44, 0x85845dd1); // 56
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); // 57
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0); // 58
    II (c, d, a, b, x[ 6], S43, 0xa3014314); // 59
    II (b, c, d, a, x[13], S44, 0x4e0811a1); // 60
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); // 61
    II (d, a, b, c, x[11], S42, 0xbd3af235); // 62
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); // 63
    II (b, c, d, a, x[ 9], S44, 0xeb86d391); // 64
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    // Zeroize sensitive information.
    memset(x, 0, sizeof x);
}
//////////////////////////////
// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5::update(const unsigned char input[], size_type length) {
    // compute number of bytes mod 64
    size_type index = count[0] / 8 % blocksize;

    // Update number of bits
    if ((count[0] += (length << 3)) < (length << 3))
        count[1]++;
    count[1] += (length >> 29);

    // number of bytes we need to fill in buffer
    size_type firstpart = 64 - index;

    size_type i;

    // transform as many times as possible.
    if (length >= firstpart) {
        // fill buffer first, transform
        memcpy(&buffer[index], input, firstpart);
        transform(buffer);

        // transform chunks of blocksize (64 bytes)
        for (i = firstpart; i + blocksize <= length; i += blocksize)
            transform(&input[i]);

        index = 0;
    } else
        i = 0;

    // buffer remaining input
    memcpy(&buffer[index], &input[i], length-i);
}
//////////////////////////////
// for convenience provide a verson with signed char
void MD5::update(const char input[], size_type length) {
    update((const unsigned char*)input, length);
}
//////////////////////////////
// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5& MD5::finalize() {
    static unsigned char padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (!finalized) {
        // Save number of bits
        unsigned char bits[8];
        encode(bits, count, 8);

        // pad out to 56 mod 64.
        size_type index = count[0] / 8 % 64;
        size_type padLen = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLen);

        // Append length (before padding)
        update(bits, 8);

        // Store state in digest
        encode(digest, state, 16);

        // Zeroize sensitive information.
        memset(buffer, 0, sizeof buffer);
        memset(count, 0, sizeof count);

        finalized=true;
    }
    return *this;
}
//////////////////////////////
// return hex representation of digest as string
std::string MD5::hexdigest() const {
    if (!finalized)
        return "";
    char buf[33];
    for (int i=0; i<16; i++)
        sprintf(buf+i*2, "%02x", digest[i]);
    buf[32]=0;
    return std::string(buf);
}
//////////////////////////////
std::ostream& operator<<(std::ostream& out, MD5 md5) {
    return out << md5.hexdigest();
}
//////////////////////////////
std::string md5(const std::string str) {
    MD5 md5 = MD5(str);
    return md5.hexdigest();
}
