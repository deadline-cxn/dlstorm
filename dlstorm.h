/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlstorm.h
 **   Description:  DL Storm Header
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_LIBRARY
#define _DLCS_LIBRARY

#include "dlstorm_lib.h"

#ifdef _WIN32
#include <winsock2.h>
#include <stdio.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    const char *va(const char *format, ...);
	bool	dlcs_Dir2File(const char *szDir,char *szFile,char *szWildCard);
	char   *dlcs_encrypt(const char *text); // encrypt text
	char   *dlcs_decrypt(const char *text); // decrypt text	
	void    dlcs_suspend_power_management(void);
    int     dlcs_hex_to_dec(const char *pa);
    int     dlcs_bin_to_dec(const char *pa);
    long    dlcs_get_tickcount(void);
    char   *dlcs_getos(void);                  
    char   *dlcs_get_os_version(void);
    int     dlcs_mkdir(const char *szDirectoryName);
    int     dlcs_chdir(const char *szDirectory);
    char   *dlcs_getcwd(void);
    bool    dlcs_isdir(char const * dir);
    int     dlcs_strcasecmp(const char *szOne,const char *szTwo);
    char   *dlcs_strreplace(char cold,char cnew, char *strg);
	char   *dlcs_get_time(void);
	char   *dlcs_convert_time(struct tm*);
#ifdef __cplusplus
}
#endif

class CExplodeList {
public:
    CExplodeList() { memset(text,0,1024); pNext=0; }
	~CExplodeList() { DEL(pNext); }
	char text[1024];
    CExplodeList	*pNext;
};
extern "C" CExplodeList *explode(const char *delimiter,char *text);

class CText {
public:
    CText();
    ~CText();
    char    szText[_MAX_PATH];
    CText   *pNext;
};

/*
// NET STUFF
#define	    NET_NAMELEN 64
class CPacket {
public:
    CPacket();
    CPacket(int iSize);
    CPacket(int iSize,char *pBuffer);
    ~CPacket();
    void DumpPacket(void);
    int iGetMaxSize(void);
    void SetMaxSize(int iSize,char *pBuffer=NULL);
    int iGetCurSize(void);
    void SetCurSize(int iNewSize);
    void Reset(void);
    void Rewind(void);
    char *pGetPacketBuffer(void);
    void Write(int     Val);
    void Write(long    Val);
    void Write(char    Val);
    void Write(short   Val);
    void Write(float   Val);
    void Write(char   *Val);
    void Write(char   *Val,int iSize);
    char *pWrite(int iSize);
    int   iRead(void);
    long dwRead(void);
    char  cRead(void);
    short sRead(void);
    float fRead(void);
    char *pRead(void);
    char *pRead(int iSize);
    bool bUserAlloc;
    char *pPacketBuffer;
    int iPacketSize;
    int iPacketCursor;
    int iPacketLen;
};

class CDLNet {
    CDLNet();
    ~CDLNet();
    int iHostPort=20202; // udp port number for acceptsocket
    int iCounter=0;
    unsigned long myAddr;
    int  PartialIPAddress (const char *in, struct sockaddr *pHostaddr);
	void    FinishCtlPacket(CPacket *pPacket);
	void    NET_SetPort(int iPort);
	int     NET_GetPort(void);
	int     NET_Init(void);
	int     NET_Shutdown(void);
	int     NET_Listen(int iState);
	int     NET_OpenSocket(int iPort);
	int     NET_CloseSocket(int iSocket);
	int     NET_CheckNewConnections (void);
	int     NET_Read(int iSocket, char *pBuf, int iLen, struct sockaddr *pAddr);
	int     NET_Write(int iSocket, char *pBuf, int iLen, struct sockaddr *pAddr);
	char    *NET_pAddrToString(struct sockaddr *pAddr);
	int     NET_StringToAddr(const char *pString, struct sockaddr *pAddr);
	int     NET_GetSocketAddr(int iSocket, struct sockaddr *pAddr);
	int     NET_GetNameFromAddr(struct sockaddr *pAddr, char *pName);
	int     NET_GetAddrFromName(const char *pName, struct sockaddr *pAddr);
	int     NET_AddrCompare(struct sockaddr *pAddr1, struct sockaddr *pAddr2);
	int     NET_GetSocketPort(struct sockaddr *pAddr);
	int     NET_SetSocketPort(struct sockaddr *pAddr, int iPort);
	char    *NET_pGetLastError(void);
};
*/

#endif // _DLCS_LIBRARY
