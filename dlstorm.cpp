/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlstorm.cpp
 **   Description:  DL Storm CPP file
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "dlstorm.h"
////////////////////////////////////////////////////////////////////////////////////////////////
const char *va(const char *format, ...) {
    va_list argptr;
    char    string[1024];
    va_start(argptr, format);
    vsprintf(string, format, argptr);
    va_end(argptr);
    return strdup(string);
}
////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> dlcs_explode(const string &delimiter, const string &str) {
    vector<string> arr;
    arr.clear();
    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng == 0) return arr;
    int i = 0;
    int k = 0;
    while (i < strleng) {
        int j = 0;
        while (i + j < strleng && j < delleng && str[i + j] == delimiter[j]) j++;
        if (j == delleng) {
            arr.push_back(str.substr(k, i - k));
            i += delleng;
            k = i;
        } else {
            i++;
        }
    }
    arr.push_back(str.substr(k, i - k));
    return arr;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool Dir2File(const char *szDir, char *szFile, char *szWildCard) {
#ifdef _WIN32
    FILE *          fp;
    HANDLE          dirsearch;  // Directory handle for reading directory information
    WIN32_FIND_DATA FileData;   // WIN32_FIND_DATA structure needed for reading directory information
    char            szCurrentDir[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurrentDir);
    SetCurrentDirectory(szDir);
    dirsearch = FindFirstFile(szWildCard, &FileData);
    if (dirsearch == INVALID_HANDLE_VALUE) return false;
    fp = fopen(szFile, "wt");
    if (!fp) return false;
    while (GetLastError() != ERROR_NO_MORE_FILES) {
        if (!strcmp(FileData.cFileName, ".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        if (!strcmp(FileData.cFileName, "..")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        if (!strcmp(FileData.cFileName, ".")) {
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            fputs(FileData.cFileName, fp);
            fputs(" <DIR>", fp);
            fputc('\n', fp);
            FindNextFile(dirsearch, &FileData);
            continue;
        }
        fputs(FileData.cFileName, fp);
        fputc('\n', fp);
        FindNextFile(dirsearch, &FileData);
    }
    FindClose(dirsearch);
    SetCurrentDirectory(szCurrentDir);
    fclose(fp);
#endif
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_encrypt(const char *text) {
    char ntext[1024];
    memset(ntext, 0, 1024);
    char ntext2[1024];
    memset(ntext2, 0, 1024);
    int i;
    strcpy(ntext, text);
    for (i = 0; i < (int)strlen(ntext); i++) {
        ntext2[i] = ntext[i] << 2;
        if (ntext2[i] == 0) ntext2[i] = 1;
        if (ntext2[i] == '|') ntext2[i] = 1;
    }
    return strdup(ntext2);
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_decrypt(const char *text) { return strdup(text); }
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_strreplace(char cold, char cnew, char *strg) {
    for (int i = 0; i < (int)strlen(strg); i++) {
        if (strg[i] == cold) strg[i] = cnew;
    }
    return strdup(strg);
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_get_time(void) {
    struct tm *dc;
    time_t     td;
    time(&td);
    dc = localtime(&td);
    char time[1024];
    strcpy(time, asctime(dc));
    time[strlen(time) - 1] = 0;
    return (strdup(time));
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_convert_time(struct tm *dc) {
    time_t td;
    time(&td);
    dc = localtime(&td);
    char time[1024];
    strcpy(time, asctime(dc));
    time[strlen(time) - 1] = 0;
    return (strdup(time));
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_hex_to_dec(const char *pa) {
    if (pa == 0) return 0;
    if (!strlen(pa)) return 0;
    char a;
    int  result = 0;
    for (int i = 0; i < (int)strlen(pa); i++) {
        a = pa[i];
        if (a == ' ' || a == '\n' || a == 0) break;
        if (a >= '0' && a <= '9')
            a -= '0';
        else if (a >= 'A' && a <= 'F')
            a = a - 'A' + 10;
        else if (a >= 'a' && a <= 'f')
            a = a - 'a' + 10;
        else
            return 255;
        result <<= 4;
        result |= a;
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int dlcs_bin_to_dec(const char *pa) {
    if (!pa) return 0;
    if (!strlen(pa)) return 0;
    int  result = 0;
    char a;
    for (;;) {
        a = *pa++;
        if (a == ' ' || a == '\n' || a == 0) break;
        if (a != '0' && a != '1') return -1;
        a -= '0';
        result <<= 1;
        result |= a;
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void dlcs_suspend_power_management(void) {
#ifdef _WIN32
    TCHAR     szPath[MAX_PATH];
    HINSTANCE hInstKernel32 = NULL;
    typedef EXECUTION_STATE(WINAPI * LPSETTHREADEXECUTIONSTATE)(EXECUTION_STATE esFlags);
    LPSETTHREADEXECUTIONSTATE pSetThreadExecutionState = NULL;
    GetSystemDirectory(szPath, MAX_PATH);
    lstrcat(szPath, TEXT("\\kernel32.dll"));
    hInstKernel32 = LoadLibrary(szPath);
    if (hInstKernel32 != NULL) {
        pSetThreadExecutionState = (LPSETTHREADEXECUTIONSTATE)GetProcAddress(hInstKernel32, "SetThreadExecutionState");
        if (pSetThreadExecutionState != NULL) pSetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
        FreeLibrary(hInstKernel32);
    }
    // Log("Power management suspended");
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////
// long getticks(void) { return dlcs_get_tickcount(); }
long dlcs_get_tickcount(void) {
#ifdef _WIN32
    return GetTickCount();
#else
    long            curtime;
    struct timeval  tp;
    struct timezone tzp;
    static long     secbase;
    gettimeofday(&tp, &tzp);
    if (!secbase) {
        secbase = tp.tv_sec;
        return tp.tv_usec / 1000;
    }
    curtime = (tp.tv_sec - secbase) * 1000 + tp.tv_usec / 1000;
    return curtime;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////
// int sp_strcmp(const char *szOne, char *szTwo) { return dlcs_strcasecmp(szOne,szTwo); }
int dlcs_strcasecmp(const char *szOne, const char *szTwo) {
    int rval = 0;
#ifdef _WIN32
    if (strcmpi(szOne, szTwo) == 0) rval = 1;
#else
    if (strcasecmp(szOne, szTwo) == 0) rval = 1;
#endif
    return rval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// int sp_mkdir(const char *szDirectoryName) { return dlcs_mkdir(szDirectoryName); }
int dlcs_mkdir(const char *szDirectoryName) {
    int returnval = 0;
#ifdef _WIN32
    if (_mkdir(szDirectoryName) == 0) returnval = 1;
#else
    if (mkdir(szDirectoryName, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0) returnval = 1;
        // S_ISUID | //S_ISGID | //S_ISVTX | // (S_IREAD) // (S_IWRITE) // (S_IEXEC) // S_IWOTH|
#endif
    return returnval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// int sp_chdir(const char *szDirectoryName) { return dlcs_chdir(szDirectoryName); }
int dlcs_chdir(const char *szDirectory) {
    int returnval = 0;
#ifdef _WIN32
    if (_chdir(szDirectory) == 0) returnval = 1;
#else
    if (chdir(szDirectory) == 0) returnval = 1;
#endif
    return returnval;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *dlcs_getcwd(void) {
    static char string[1024];
    getcwd(string, _MAX_PATH);
    return string;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_file_exists(const char *szFile) {
    struct stat st;
    if (stat(szFile, &st) == 0) {
        return true;
    } else {
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_isdir(char const *dir) {
    struct stat st;
    if (stat(dir, &st) == -1) return false;
    if (st.st_mode & S_IFDIR) return true;
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char const *dlcs_timestamp(char const *x) {
    time_t td;
    time(&td);
    strcpy((char *)x, va("%ld", td));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string dlcs_md5_digest(string str) {  // return a md5 digest of text
    return md5(str);
}
////////////////////////////////////////////////////////////////////////////////////////////////
string dlcs_md5_file(string file) {  // return md5 digest of file
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
char *dlcs_get_os_version(void) {
#ifdef _WIN32

    char szTemp[128];
    memset(szTemp, 0, 128);
    char szTemp2[128];
    memset(szTemp2, 0, 128);
    strcpy(szTemp, "Unknown");
    OSVERSIONINFOEX osvi;
    BOOL            bOsVersionInfoEx;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi))) {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO *)&osvi)) return strdup("unknown");
    }

    switch (osvi.dwPlatformId) {
        case VER_PLATFORM_WIN32_NT:
            if (osvi.dwMajorVersion <= 4) strcpy(szTemp, "Windows NT ");
            if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
                strcpy(szTemp, "Windows 2000 ");
            } else if (bOsVersionInfoEx) {
                strcpy(szTemp, va("Windows XP (or higher) "));
            }
            if (osvi.dwMajorVersion <= 4) {
                sprintf(szTemp2, "version %ld.%ld %s (Build %ld)", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
                strcat(szTemp, szTemp2);
            } else {
                sprintf(szTemp2, "%s (Build %ld)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
                strcat(szTemp, szTemp2);
            }
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
                sprintf(szTemp, "Windows 95 %s", osvi.szCSDVersion);
            }
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
                sprintf(szTemp, "Windows 98 %s", osvi.szCSDVersion);
            }
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
                strcpy(szTemp, "Windows ME");
            }
            break;
    }

    return strdup(szTemp);
#else
    return strdup(CPUSTRING);
#endif
}

/*

//////////////////////////////////////////////////////////////////////////////////
static SOCKET AcceptSocket=INVALID_SOCKET;
//////////////////////////////////////////////////////////////////////////////////
void NET_SetPort(int iPort)
{
    if(iPort > 0xffff) return; //{ Log("NET_SetPort tried to set a port higher than 0xffff"); return; }
    if(iPort < 1024)   return; //{ Log("NET_SetPort tried to set a port lower than 1024");    return; }
    iHostPort = iPort;
}
//////////////////////////////////////////////////////////////////////////////////
int NET_GetPort() { return iHostPort; }
//////////////////////////////////////////////////////////////////////////////////
int NET_Init()
{
#ifdef _WIN32
    int err; WSADATA winsockdata; err=WSAStartup(MAKEWORD(1,1),&winsockdata);
    if (err != 0) { return -1; }
#endif
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int NET_Shutdown (void)
{
    NET_Listen(0);
#ifdef _WIN32
    if(WSACleanup ()!=0) return -1;
#endif
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int NET_Listen(int iState)
{
    if(iState)
    {
        if(AcceptSocket != INVALID_SOCKET) { return 0; }
        if((AcceptSocket = NET_OpenSocket(iHostPort)) == INVALID_SOCKET) return -1;
    }
    else
    {
        if(AcceptSocket == INVALID_SOCKET) return 0;
        NET_CloseSocket(AcceptSocket);
        AcceptSocket=INVALID_SOCKET;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int NET_OpenSocket(int iPort)
{
    unsigned long _true = 1;
    int newsocket;
    struct sockaddr_in address;
    if ((newsocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
        return -1;
#ifdef _WIN32
    if(ioctlsocket(newsocket, FIONBIO, &_true) == SOCKET_ERROR)
#else
    bzero(&address, sizeof(address)); // linux func
    if(fcntl(newsocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
                NET_CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((short)iPort);
    if(bind(newsocket,(struct sockaddr *)&address, sizeof(address))==SOCKET_ERROR)
    {
        NET_CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
    return newsocket;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_CloseSocket (int iSocket)
{
#ifdef _WIN32
    return closesocket(iSocket);
#else
        return close(iSocket);
#endif
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_CheckNewConnections (void)
{
    char pBuffer[65535];
    if(AcceptSocket == INVALID_SOCKET) { return -1; }
    if(recvfrom(AcceptSocket, pBuffer, sizeof(pBuffer), MSG_PEEK, NULL, NULL) > 0) { return AcceptSocket; }
    return -1;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_Read(int iSocket, char *pBuff, int iLen, struct sockaddr *pAddr)
{
    int iAddrlen = sizeof (struct sockaddr);
    int err,errno;
#ifndef _WIN32
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(iSocket,&readfds);
    tv.tv_sec=0;
    tv.tv_usec=5;
    if(select(iSocket+1, &readfds, NULL, NULL, &tv)==-1) return 0;
    err=0;
    if(FD_ISSET(iSocket, &readfds) > 0)
    err = recvfrom(iSocket, pBuff, iLen, 0, (struct sockaddr *)pAddr, (socklen_t *)&iAddrlen);
#endif

#ifdef _WIN32
    err = recvfrom(iSocket, pBuff, iLen, 0, (struct sockaddr *)pAddr, &iAddrlen);
    if (err == SOCKET_ERROR){errno=WSAGetLastError(); if(errno == WSAEWOULDBLOCK || errno == WSAECONNREFUSED) return 0;}
#endif
    //if(err==-1) { Log("%s on recvfrom()",NET_pGetLastError()); }
    return err;
}
//////////////////////////////////////////////////////////////////////////////////
int NET_Write(int iSocket, char *pBuf, int iLen, struct sockaddr *pAddr)
{
    int err;
#ifdef SIMULATE_CONNECTION
    if (!(rand()%10)) return iLen;
#endif //SIMULATE_CONNECTION
#ifdef _WIN32
    err = sendto(iSocket, pBuf, iLen, 0, (struct sockaddr *)pAddr, sizeof(struct sockaddr));
#else
    err = sendto(iSocket, pBuf, iLen, 0, pAddr, sizeof(struct sockaddr));
#endif
    //if (err== -1)  { Log("net_sys.c[NET_Write()]: %s",NET_pGetLastError()); }
        return err;
}
//////////////////////////////////////////////////////////////////////////////////
char *NET_pAddrToString(struct sockaddr *pAddr)
{
    static char buffer[22];
    if(pAddr==NULL) strcpy(buffer,"error!");
    else
    {
                int taddr=((struct sockaddr_in *)pAddr)->sin_addr.s_addr;
//		Log("%d.%d.%d.%d:%d",(taddr>>24)&0xff,(taddr >> 16) & 0xff, (taddr >> 8) & 0xff, taddr & 0xff, ntohs(((struct sockaddr_in *)pAddr)->sin_port));
        int haddr=ntohl(((struct sockaddr_in *)pAddr)->sin_addr.s_addr);
        sprintf(buffer, "%d.%d.%d.%d:%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff, ntohs(((struct sockaddr_in *)pAddr)->sin_port));
//		Log(buffer);
    }
    return buffer;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_StringToAddr(const char *pString, struct sockaddr *pAddr)
{
        int ha1,ha2,ha3,ha4,hp,ipaddr;
        sscanf(pString, "%d.%d.%d.%d:%d", &ha1, &ha2, &ha3, &ha4, &hp);
        ipaddr=(ha1<<24)|(ha2<<16)|(ha3<<8)|ha4;
        pAddr->sa_family=AF_INET;
        ((struct sockaddr_in *)pAddr)->sin_addr.s_addr=htonl(ipaddr);
        ((struct sockaddr_in *)pAddr)->sin_port=htons((short)hp);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_GetSocketAddr (int iSocket, struct sockaddr *pAddr)
{
    int addrlen = sizeof(struct sockaddr);
        unsigned int a;
    memset(pAddr, 0, sizeof(struct sockaddr));
#ifdef _WIN32
    getsockname(iSocket, (struct sockaddr *)pAddr, &addrlen);
#else
    getsockname(iSocket, (struct sockaddr *)pAddr, (socklen_t *)&addrlen);
#endif
        a = ((struct sockaddr_in *)pAddr)->sin_addr.s_addr;
        if (a == 0 || a == inet_addr("127.0.0.1")) ((struct sockaddr_in *)pAddr)->sin_addr.s_addr = myAddr;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_GetNameFromAddr (struct sockaddr *pAddr, char *pName)
{
    struct hostent *hostentry;
        hostentry = gethostbyaddr ((const char *)&((struct sockaddr_in *)pAddr)->sin_addr, sizeof(struct in_addr), AF_INET);
    if (hostentry) { strncpy (pName, (const char *)hostentry->h_name, NET_NAMELEN - 1); return 0; }
        strcpy (pName, NET_pAddrToString (pAddr)); return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_GetAddrFromName(const char *pName, struct sockaddr *pAddr)
{
    struct hostent *hostentry;
    if (pName[0] >= '0' && pName[0] <= '9') return PartialIPAddress (pName, pAddr);
    hostentry=gethostbyname(pName);
    if(!hostentry) return SOCKET_ERROR;
    pAddr->sa_family=AF_INET;
    ((struct sockaddr_in *)pAddr)->sin_port=htons((short)iHostPort);
    ((struct sockaddr_in *)pAddr)->sin_addr.s_addr=*(int *)hostentry->h_addr_list[0];
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_AddrCompare (struct sockaddr *pAddr1, struct sockaddr *pAddr2)
{
    if (pAddr1->sa_family != pAddr2->sa_family) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_addr.s_addr != ((struct sockaddr_in *)pAddr2)->sin_addr.s_addr) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_port != ((struct sockaddr_in *)pAddr2)->sin_port) return 1;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_GetSocketPort (struct sockaddr *pAddr)
{
    return ntohs(((struct sockaddr_in *)pAddr)->sin_port);
}
//////////////////////////////////////////////////////////////////////////////////
int  NET_SetSocketPort(struct sockaddr *pAddr, int iPort)
{
    ((struct sockaddr_in *)pAddr)->sin_port=htons((short)iPort);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
static struct stErrorEntry{
  int iID;
  const char *pMessage;
}pErrorList[]=
{
  0,                    "no error",
  WSAEACCES,            "WSAEACCES - permission denied",
  WSAEADDRINUSE,        "WSAEADDRINUSE - address already in use",
  WSAEADDRNOTAVAIL,     "WSAEADDRNOTAVAIL - cannot assign requested address",
  WSAEAFNOSUPPORT,      "[Lost internet connection]", //"WSAEAFNOSUPPORT - Probably not connected to internet",
  WSAEALREADY,          "WSAEALREADY - operation already in progress",
  WSAECONNABORTED,      "WSAECONNABORTED - software caused connection abort",
  WSAECONNREFUSED,      "WSAECONNREFUSED - connection refused",
  WSAECONNRESET,        "WSAECONNRESET - connection reset by peer",
  WSAEDESTADDRREQ,      "WSAEDESTADDRREQ - destination address required",
  WSAEFAULT,            "WSAEFAULT - bad address",
  WSAEHOSTDOWN,         "WSAEHOSTDOWN - host is down",
  WSAEHOSTUNREACH,      "WSAEHOSTUNREACH - no route to host",
  WSAEINPROGRESS,       "WSAEINPROGRESS - operation now in progress",
  WSAEINTR,             "WSAEINTR - interrupted function call",
  WSAEINVAL,            "WSAEINVAL - invalid argument",
  WSAEISCONN,           "WSAEISCONN - socket is already connected",
  WSAEMFILE,            "WSAEMFILE  - too many open files",
  WSAEMSGSIZE,          "WSAEMSGSIZE - message too long",
  WSAENETDOWN,          "WSAENETDOWN - network is down",
  WSAENETRESET,         "WSAENETRESET - network dropped connection on reset",
  WSAENETUNREACH,       "WSAENETUNREACH - network is unreachable",
  WSAENOBUFS,           "WSAENOBUFS - no buffer space available",
  WSAENOPROTOOPT,       "WSAENOPROTOOPT - bad protocol option",
  WSAENOTCONN,          "WSAENOTCONN - socket is not connected",
  WSAENOTSOCK,          "WSAENOTSOCK - socket operation on non-socket",
  WSAEOPNOTSUPP,        "WSAEOPNOTSUPP - operation not supported",
  WSAEPFNOSUPPORT,      "WSAEPFNOSUPPORT - protocol family not supported",
  WSAEPROCLIM,          "WSAEPROCLIM - too many processes",
  WSAEPROTONOSUPPORT,   "WSAEPROTONOSUPPORT - protocol not supported",
  WSAEPROTOTYPE,        "WSAEPROTOTYPE - protocol wrong type for socket",
  WSAESHUTDOWN,         "WSAESHUTDOWN - cannot send after socket shutdown",
  WSAESOCKTNOSUPPORT,   "WSAESOCKTNOSUPPORT - socket type not supported",
  WSAETIMEDOUT,         "WSAETIMEDOUT - connection timed out",
  WSATYPE_NOT_FOUND,    "WSATYPE_NOT_FOUND - class type not found",
  WSAEWOULDBLOCK,       "WSAEWOULDBLOCK - resource temporarily unavailable",
  WSAHOST_NOT_FOUND,    "WSAHOST_NOT_FOUND - host not found",
  WSA_INVALID_HANDLE,   "WSA_INVALID_HANDLE - specified event object handle is invalid",
  WSA_INVALID_PARAMETER,"WSA_INVALID_PARAMETER - one or more parameters are invalid",
  WSA_IO_INCOMPLETE,    "WSA_IO_INCOMPLETE - overlapped i/o event object not in signaled state",
  WSA_IO_PENDING,       "WSA_IO_PENDING - overlapped operations will complete later",
  WSA_NOT_ENOUGH_MEMORY,"WSA_NOT_ENOUGH_MEMORY - insufficient nemory available",
  WSANOTINITIALISED,    "WSANOTINITIALISED - successful WSASartup() not yet performened",
  WSANO_DATA,           "WSANO_DATA - valid name, no data record of requested type",
  WSANO_RECOVERY,       "WSANO_RECOVERY - this is a non-recoverable error",
  WSASYSCALLFAILURE,    "WSASYSCALLFAILURE - system call failure",
  WSASYSNOTREADY,       "WSASYSNOTREADY - network subsystem is unavailable",
  WSATRY_AGAIN,         "WSATRY_AGAIN - non-authoriative host not found",
  WSAVERNOTSUPPORTED,   "WSAVERNOTSUPPORTED - winsock.dll version out of range",
  WSAEDISCON,           "WSAEDISCON - graceful shutdown in progress",
  WSA_OPERATION_ABORTED,"WSA_OPERATION_ABORTED - overlapped operation aborted",
};
const int iNumMessages = sizeof(pErrorList) / sizeof(struct stErrorEntry);
#endif
//////////////////////////////////////////////////////////////////////////////////
char *NET_pGetLastError(void)
{
    int i,err;
#ifdef _WIN32
    err = WSAGetLastError();
    for (i = 0;i<iNumMessages;i++) { if (pErrorList[i].iID == err) { return (const char *)pErrorList[i].pMessage; } }
    return (const char *)pErrorList[0].pMessage;
#else
    return(strerror(errno));
#endif
}
//////////////////////////////////////////////////////////////////////////////////
static int PartialIPAddress (const char *in, struct sockaddr *pHostaddr)
{
    char buff[256];
    char *b; int addr,num,mask; buff[0] = '.'; b=buff; strcpy(buff+1, in); if (buff[1] == '.') b++;
    addr=0; mask=-1;
    while (*b == '.')
    {
        num = 0; if (*++b < '0' || *b > '9') return -1;
        while (!( *b < '0' || *b > '9')) num = num*10 + *(b++) - '0';
        mask<<=8; addr = (addr<<8) + num;
    }
        pHostaddr->sa_family = AF_INET;
    ((struct sockaddr_in *)pHostaddr)->sin_port=htons((short)iHostPort);
        ((struct sockaddr_in *)pHostaddr)->sin_addr.s_addr=(myAddr&htonl(mask))|htonl(addr);
        return 0;
}
//////////////////////////////////////////////////////////////////////////////////
void FinishCtlPacket(CPacket *pPacket)
{
    int j=(NET_FLAG_CTL|(pPacket->iGetCurSize() & NET_FLAG_LENGTH_MASK));
    *((int *)pPacket->pGetPacketBuffer()) = htonl(j);
}
//////////////////////////////////////////////////////////////////////////////////

CText::CText() { memset(szText,0,_MAX_PATH); pNext=NULL; }
CText::~CText() { }

////////////////////////////////////////////////////////////////////////

CPacket::CPacket()
{
    pPacketBuffer = NULL;
    iPacketSize = 0;
    iPacketCursor = 0;
    iPacketLen = 0;
    bUserAlloc = false;
}

////////////////////////////////////////////////////////////////////////

CPacket::CPacket(int iSize)
{
    if(iSize<0) return;
    pPacketBuffer = (const char *)malloc(iSize);
    if (pPacketBuffer == NULL)
        return;
    iPacketSize = iSize;
    iPacketCursor = 0;
    iPacketLen = 0;
    bUserAlloc = false;
}

////////////////////////////////////////////////////////////////////////

CPacket::CPacket(int iSize,char *pBuffer)
{
    if(iSize<0)
        return;
    if(pBuffer==NULL)
        return;
    pPacketBuffer = pBuffer;
    iPacketLen = iPacketSize = iSize;
    iPacketCursor = 0;
    bUserAlloc = true;
}


////////////////////////////////////////////////////////////////////////

CPacket::~CPacket()
{
     if((!bUserAlloc)&&(pPacketBuffer!=NULL))
         free(pPacketBuffer);
}

////////////////////////////////////////////////////////////////////////

void CPacket::DumpPacket(void)
{

    printf("-=[Packet Info]=========================================================-");


    for(int y=0;y<iPacketLen;y++)
    {
        if(pPacketBuffer[y]<32)
        {
            //LogC("{%d}",pPacketBuffer[y]);
        }
        else {}
            //LogC("%c",pPacketBuffer[y]);
    }
    //LogC("\n\r");
    printf("-==========================================================[End Packet]=-");
}

int CPacket::iGetMaxSize(void)
{
    return iPacketSize;
}

////////////////////////////////////////////////////////////////////////

void CPacket::SetMaxSize(int iSize,char *pBuffer)
{
    if(iSize<0)
        return;
    if((bUserAlloc)&&(pBuffer))
    {
        pPacketBuffer=pBuffer;
        iPacketSize=iSize;
        return ;
    }

    iPacketSize=iSize;
    if(pPacketBuffer==NULL)
    {
        pPacketBuffer=(const char *)malloc(iSize);
        if(!pPacketBuffer)
            return;
    }
    pPacketBuffer=(const char *)realloc(pPacketBuffer,iSize);
    if (pPacketBuffer==NULL)
        return;
}

////////////////////////////////////////////////////////////////////////

int CPacket::iGetCurSize(void)
{
    return iPacketLen-iPacketCursor;
}

////////////////////////////////////////////////////////////////////////

void CPacket::SetCurSize(int iNewLen)
{
    iPacketLen=iNewLen;
}


////////////////////////////////////////////////////////////////////////

void CPacket::Reset(void)
{
    iPacketCursor=0;
    iPacketLen=0;
}
////////////////////////////////////////////////////////////////////////

void CPacket::Rewind(void)
{
    iPacketCursor=0;
}

///////////////////////////////////////////////////////////////////////
char *CPacket::pGetPacketBuffer(void)
{
    return pPacketBuffer;
}

////////////////////////////////////////////////////////////////////////

#define WRITE(Type) if (pPacketBuffer==NULL) return; if (iPacketLen + (int)sizeof(Type) > iPacketSize) return; *((Type *)(pPacketBuffer+iPacketLen)) = Val; iPacketLen += sizeof(Type);

void CPacket::Write(int   Val){WRITE(int)}
void CPacket::Write(long  Val){WRITE(long)}
void CPacket::Write(char  Val){WRITE(char)}
void CPacket::Write(short Val){WRITE(short)}
void CPacket::Write(float Val){WRITE(float)}

void CPacket::Write(const char *Val)
{
    int i = strlen(Val)+1;
    if(pPacketBuffer==NULL) return;
    if(iPacketLen + i > iPacketSize) return;
    strcpy(pPacketBuffer+iPacketLen,Val);
    iPacketLen+=i;
}

void CPacket::Write(const char *Val,int iSize)
{
    if(!iSize) return;
    if(pPacketBuffer==NULL) return;
    if(iPacketLen + iSize > iPacketSize) return;
    memcpy(pPacketBuffer+iPacketLen,Val,iSize);
    iPacketLen+=iSize;
}

char *CPacket::pWrite(int iSize)
{
    if(!iSize) return NULL;
    if(!pPacketBuffer) return NULL;
    if(iPacketLen + iSize > iPacketSize) return NULL;
    iPacketLen+=iSize;
    return pPacketBuffer+iPacketLen-iSize;
}

////////////////////////////////////////////////////////////////////////

#define READ(Val) if(iPacketLen<1) return 0; if (pPacketBuffer==NULL) return 0; iPacketCursor += sizeof(Val); if (iPacketCursor > iPacketLen) return 0; return *((Val *)(pPacketBuffer+iPacketCursor-sizeof(Val)));

int     CPacket::iRead(void) {READ(int)}
long    CPacket::dwRead(void){READ(long)}
char    CPacket::cRead(void) {READ(char)}
short   CPacket::sRead(void) {READ(short)}
float   CPacket::fRead(void) {READ(float)}

char   *CPacket::pRead(void)
{
    int i;
    if(iPacketLen<1) return strdup("null");
    if(!pPacketBuffer) return strdup("null");

    i=iPacketCursor;

    iPacketCursor+=strlen(pPacketBuffer+iPacketCursor)+1;

    if(iPacketCursor > iPacketLen) return strdup("null");

    return (const char *) (pPacketBuffer+i);

}

char *CPacket::pRead(int iSize)
{
    int i;
    if(iSize==0) return strdup("null");
    if(iPacketLen<1) return strdup("null");
    if(pPacketBuffer==NULL) return strdup("null");
    i=iPacketCursor;
    iPacketCursor+=iSize;
    if(iPacketCursor > iPacketLen) return strdup("null");
    return (const char *) (pPacketBuffer+i);
}
*/
