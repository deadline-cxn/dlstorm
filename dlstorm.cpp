/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2016
 **   File:         dlstorm.cpp
 **   Description:  DLCODESTORM Namespace, where the magic happens
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ******************************************************************************/
#include "dlstorm.h" // __stdcall DllMain() { return 0; }

////////////////////////////////////////////////////////////////////////////////////////////////
char _DLCODESTORM_vx[1024]; // keep this in the global space TODO: improve this
const char *DLCODESTORM::va(const char *format, ...) {
    memset(_DLCODESTORM_vx,0,1024);
    va_list argptr;
    va_start(argptr, format);
    vsprintf(_DLCODESTORM_vx, format,argptr);
    va_end (argptr);
    return (_DLCODESTORM_vx);
}

////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> DLCODESTORM::dlcs_explode(const string &delimiter, const string &str) {
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
char* DLCODESTORM::dlcs_get_filetype(char* x, char* in) {
    vector <string> ft;
    ft=dlcs_explode(".",in);
    ft.size();
    strcpy(x,ft[ft.size()].c_str());
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool DLCODESTORM::dlcs_istrue(string text) {
    text=dlcs_tolower(text);
    if(text=="on")  return true;
    if(text=="1")   return true;
    if(text=="true")return true;
    if(text=="yes")  return true;
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_tolower(string z) {
    dlcsm_make_filename(x);
    strcpy(x,z.c_str());
    int i=0;
    while (x[i]) { x[i] = tolower(x[i]); i++; }
    z.assign(x);
    return z;
}
////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> DLCODESTORM::dlcs_dir_to_vector(char* szDir, char* szWildCard) {
    vector <string> diro;
    string f;
    string d;
    d.assign(szDir);
    diro.clear();
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(szDir);
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if((!((dlcs_strcasecmp(epdf->d_name,".")) || (dlcs_strcasecmp(epdf->d_name,".."))))) {
                f.assign(va("%s%c%s",szDir,PATH_SEP,epdf->d_name));
                if(!dlcs_isdir((char *)f.c_str())) {
                    diro.push_back(f);
                }
                else {
                    vector<string>x=dlcs_dir_to_vector((char *)f.c_str(),szWildCard);
                    diro.insert( diro.end(), x.begin(), x.end() );
                }
            }
        }
    }
    closedir(dpdf);
    return diro;
/* #include <fnmatch.h>
int fnmatch(const char *pattern, const char *string, int flags);
Description
The fnmatch() function checks whether the string argument matches the pattern argument, which is a shell wildcard pattern.
The flags argument modifies the behavior; it is the bitwise OR of zero or more of the following flags:
FNM_NOESCAPE If this flag is set, treat backslash as an ordinary character, instead of an escape character.
FNM_PATHNAME If this flag is set, match a slash in string only with a slash in pattern and not by an asterisk (*) or a question mark (?) metacharacter, nor by a bracket expression ([]) containing a slash.
FNM_PERIOD   If this flag is set, a leading period in string has to be matched exactly by a period in pattern. A period is considered to be leading if it is the first character in string, or if both FNM_PATHNAME is set and the period immediately follows a slash.
FNM_FILE_NAME This is a GNU synonym for FNM_PATHNAME.
FNM_LEADING_DIR If this flag (a GNU extension) is set, the pattern is considered to be matched if it matches an initial segment of string which is followed by a slash. This flag is mainly for the internal use of glibc and is only implemented in certain cases.
FNM_CASEFOLD  If this flag (a GNU extension) is set, the pattern is matched case-insensitively.
Return Value  Zero if string matches pattern, FNM_NOMATCH if there is no match or another nonzero value if there is an error.*/
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool DLCODESTORM::dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard) {
    FILE *fp;
    fp=fopen(szFile,"wt");
    if(!fp) return false;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(szDir);
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if(!((dlcs_strcasecmp(epdf->d_name,".")) || (dlcs_strcasecmp(epdf->d_name,".."))))
               if(!dlcs_isdir(epdf->d_name))
                fputs(epdf->d_name,fp);
        }
    }
    closedir(dpdf);
    fclose(fp);
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char* DLCODESTORM::dlcs_trim_lf(char* x) {
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_md5_digest(string str) { // return a md5 digest of text
    return md5(str);
}
string DLCODESTORM::dlcs_md5_file(string file) { // return md5 digest of file
    string in_md5;
    string out_md5;
    /*(int file_descript;
    char* file_buffer;
    file_descript = open(file.c_str(), O_RDONLY);
    if(file_descript < 0) out_md5="(ERROR)";
    else {
        struct stat statbuf;
        if(fstat(file_descript, &statbuf) < 0) out_md5="(ERROR)";
        else {
            file_buffer =

            // mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, file_descript, 0);

            for(i=0; i <MD5_DIGEST_LENGTH; i++) {
                printf("%02x",md[i]);
            }
            MD5((unsigned char*) file_buffer, file_size, result);
            print_md5_sum(result);
            // printf("  %s\n", argv[1]);
            }
        in_md5.assign(file_buffer)
        out_md5=md5(in_md5);
    }
    */
    return out_md5;

}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_encrypt(string text) { return text; } // TODO
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_decrypt(string text) { return (text); } // TODO
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_get_time(char *x) {
    struct tm *dc;
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_convert_time(char *x,struct tm* dc) {
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_timestamp(char *x) {
    time_t td;
    time( &td );
    strcpy(x,va("%ld",td));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_readable_timestamp(char *x,char *in) {
    long wtf;
    wtf=atoi(in);
    strcpy(x,ctime(&wtf));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int DLCODESTORM::dlcs_hex_to_dec(char *pa) {
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
int DLCODESTORM::dlcs_bin_to_dec(char *pa) {
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
void DLCODESTORM::dlcs_suspend_power_management(void) {
#ifdef DLCS_WINDOWS
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
long DLCODESTORM::dlcs_get_tickcount(void) {
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
int DLCODESTORM::dlcs_strcasecmp(const char *szOne,const char *szTwo) {
    int rval=0;
#ifdef _WIN32
    if(strcasecmp(szOne,szTwo)==0) rval=1;
#else
    if(strcasecmp(szOne,szTwo)==0) rval=1;
#endif
    return rval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool DLCODESTORM::dlcs_isdir(char *dir) {
    struct stat st;
    if(stat(dir,&st)==-1) return false;
    if(st.st_mode&S_IFDIR) return true;
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int DLCODESTORM::dlcs_get_filesize(string f) {
    struct stat st;
    if(stat(f.c_str(),&st)==-1) return false;
    return st.st_size;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_filetype(string pathName){
	// Finds the last persiod character of the string
	int period = pathName.find_last_of(".");
	// I use  + 1 because I don't really need the to include the period
	string ext = pathName.substr(period + 1);
	return ext;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int DLCODESTORM::dlcs_mkdir(char *szDirectoryName) {
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
int DLCODESTORM::dlcs_chdir(char *szDirectory) {
    int returnval=0;
#ifdef _WIN32
    if(_chdir(szDirectory)==0) returnval=1;
#else
    if(chdir(szDirectory)==0)  returnval=1;
#endif
    return returnval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_getcwd() {
    char buffer[FILENAME_SIZE];
    getcwd(buffer,FILENAME_SIZE);
    string x;
    x.assign(buffer);
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_get_os_version() {
    string x="Unknown OS";
#ifdef DLCS_WINDOWS
    dlcsm_make_str(szTemp); dlcsm_make_str(szTemp2);
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) ) {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) {
            x="Windows Unknown Version";
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
    /*

 PGPI pGPI = (PGPI) GetProcAddress(
            GetModuleHandle(TEXT("kernel32.dll")),
            "GetProductInfo");


    pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);
     switch( dwType )  {
        case PRODUCT_ULTIMATE:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
           break;
        case PRODUCT_PROFESSIONAL:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
           break;
        case PRODUCT_HOME_PREMIUM:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
           break;
        case PRODUCT_HOME_BASIC:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
           break;
        case PRODUCT_ENTERPRISE:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
           break;
        case PRODUCT_BUSINESS:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
           break;
        case PRODUCT_STARTER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
           break;
        case PRODUCT_CLUSTER_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
           break;
        case PRODUCT_DATACENTER_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
           break;
        case PRODUCT_DATACENTER_SERVER_CORE:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
           break;
        case PRODUCT_ENTERPRISE_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
           break;
        case PRODUCT_ENTERPRISE_SERVER_CORE:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
           break;
        case PRODUCT_ENTERPRISE_SERVER_IA64:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
           break;
        case PRODUCT_SMALLBUSINESS_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
           break;
        case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
           break;
        case PRODUCT_STANDARD_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
           break;
        case PRODUCT_STANDARD_SERVER_CORE:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
           break;
        case PRODUCT_WEB_SERVER:
           // StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
           break;
        default:
            break;
        }
        }
        */


    x.assign(szTemp);

#endif
#ifdef DLCS_LINUX
    x.assign("LINUX");
    int z;
    struct utsname l_un;
    z=uname(&l_un);
    if(z==-1) {}
    else x.assign( va("%s %s %s", l_un.sysname, l_un.release, l_un.version));
#endif
    return x;
}
string DLCODESTORM::dlcs_get_hostname() {
    char y [HOST_NAME_MAX]; memset(y,0,HOST_NAME_MAX);
    string x;
    gethostname(y, HOST_NAME_MAX);
    x.assign(y);
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_get_ipaddress() {
    string x;
    char y [FILENAME_SIZE]; memset(y,0,FILENAME_SIZE);
    strcpy(y,"127.0.0.1"); // TODO: Add actual computer ip address here
    strcpy(y,dlcs_get_hostname().c_str());
    struct hostent *phe = gethostbyname(y);
    if (phe == 0) {
        x="ERROR";
        return x;
    }
    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        x.assign(inet_ntoa(addr));
        return x;
    }
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void DLCODESTORM::dlcs_get_webpage2(string url, char *x) {

}
string  DLCODESTORM::dlcs_get_webpage(string url) {
    string rstr, host, page; page.clear();
    string uri;

    string vars; vector<string> strv;
    strv=dlcs_explode("?",url);
    if(strv.size()>1) {
        url=strv[0];
        vars=strv[1];
    }

    vector<string> strx; strx=dlcs_explode("/",url);  host=strx[1];

    if( (strx[0]!="http:") && (strx[0]!="https:") ) {
            return "You must put http: or https:";
    }
    else {
        host=strx[2];
        for(int i=3;i<strx.size();i++)
            page=page+"/"+strx[i];
    }
    uri=host+page;

    // cout << host << endl;
    int s, error;
    struct sockaddr_in addr;
    if((s = socket(AF_INET,SOCK_STREAM,0))<0) {
            // cout<<"Error 01: creating socket failed!\n";
        close(s);
        return (string) "1";
    }
    addr.sin_family = AF_INET;
    char ip[1024]; memset(ip,0,1024);
    strcpy(ip,dlcs_dns_lookup(host).c_str());
    // cout << "IP:" << ip << endl;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(80);
    error = connect(s,(sockaddr*)&addr,sizeof(addr));
    if(error!=0) {
        rstr= "Error: conecting to server failed!";
        close(s);
        return rstr;
    }
    page=page+"?"+vars;
    string get_string = "GET "+page+" HTTP/1.1\r\nHost: "+host+"\r\n";
    //get_string=get_string+"Content-Type: application/x-www-form-urlencoded\r\n";
    get_string=get_string+"Connection: close\r\n\r\n";

    // if(vars.length())  get_string=get_string+"\r\n"+vars+"\r\n";
    // cout << get_string << endl;
    // cout << "***************************************************************" << endl;

    int x=get_string.length();
    char msg[x];
    strcpy(msg,get_string.c_str());
    char answ[1024];
    send(s,msg,sizeof(msg),0);
    ssize_t len;
    bool endofheader=0;
	while((len = recv(s, answ, 1024, 0)) > 0)
        for(int zz=0;zz<len;zz++) {
            if(!endofheader) {
                if(answ[zz]=='<') {
                    if(answ[zz+1]=='!') {
                        endofheader=true;
                        rstr+=answ[zz];
                    }
                }
            }
            else
                rstr+=answ[zz];
        }
	close(s);
	return rstr;
}
int DLCODESTORM::dlcs_count_words(string instr,string word) {
    unsigned int count=0;
    string s;
    s.assign(dlcs_tolower(instr.c_str()));
    word.assign(dlcs_tolower(word.c_str()));
    size_t found;
    int wl=word.length();
    found=s.find(word); if(found) count++;
    while(1) {
        found=s.find(word,found+1);
        if(found!=string::npos)
            count ++;
        else
            return count;
    }
    return count;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_dns_lookup(string url) {
    string ip;
    int sockfd;
    struct addrinfo hints, *servinfo, *ptr;
    int rv;
    struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(url.c_str(), "http", &hints, &servinfo)) != 0) {
        ip="ERROR";
        return ip;
    }
    for(ptr=servinfo; ptr != NULL ;ptr=ptr->ai_next) {
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                break;
            case AF_INET:
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                ip.assign(inet_ntoa(sockaddr_ipv4->sin_addr));
                break;
            case AF_INET6:
                // printf("AF_INET6 (IPv6)\n");
                // the InetNtop function is available on Windows Vista and later
                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                // printf("\tIPv6 address %s\n",
                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );

                // We use WSAAddressToString since it is supported on Windows XP and later
                //sockaddr_in6 = (LPSOCKADDR) ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                //ipbufferlength = 46;
                //WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength );
                //ip.assign(ipstringbuffer);
                //if(iRetval) printf("WSAAddressToString failed with %u\n", WSAGetLastError() );
                //else        printf("\tIPv6 address %s\n", ipstringbuffer);
                break;
            //case AF_NETBIOS:                break;
            default:
                break;
        }
        /*
        printf("\tSocket type: ");
        switch (ptr->ai_socktype) {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol) {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);

    */
    }
    freeaddrinfo(servinfo); // all done with this structure
    return ip;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_inet_ntoa(string ip) {
    return ip;
}
////////////////////////////////////////////////////////////////////////////////////////////////

