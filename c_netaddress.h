/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#ifndef _FM_NET_ADDRESS_
#define _FM_NET_ADDRESS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlstorm.h"

class CInetAddress
{
public:
    CInetAddress();
    ~CInetAddress();

    int operator==(CInetAddress rp);                
	// compares address against another address
    int operator!=(CInetAddress rp);                
	// compares address against another address
    inline char *pGetAddress();                     
	// returns the raw IP address of this InetAddress object
    inline char *pGetHostAddress();                 
	// returns the IP address string "%d.%d.%d.%d"
    char    *pGetHostName();                        
	// returns the hostname for this address
    static  CInetAddress *pGetByName(char *pHost);  
	// determines the IP address of a host given the host's name

    void    Ping(void);
    void    Ping(int iCount);
    char    *GetPort(void);
    void    SetPort(char *pPort);
    long    dGetPing(void);
    struct  sockaddr stAddr;
    char    pName[128];
    int     iPingCount;
    long    fPing;
    int     iSocket;
};

#endif
