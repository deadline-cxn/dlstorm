/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_net.h
 **   Class:        CPacket
 **                 CCSocket
 **   Description:  Network class and library
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_C_NET
#define _DLCS_C_NET
#include "dlstorm.h"
//#define SIMULATE_CONNECTION // uncomment to simulate a internet connection w/packet loss
#ifndef DLSCM_WINDOWS
#ifdef SOCKET
#undef SOCKET
#endif
#define SOCKET int
#ifdef INVALID_SOCKET
#undef INVALID_SOCKET
#endif
#define INVALID_SOCKET -1
#ifdef SOCKET_ERROR
#undef SOCKET_ERROR
#endif
#define SOCKET_ERROR -1
#endif
#define NET_WAITFORANSWER               6000  // should be an inivar?
#define NET_MINMESSAGE                  8192  // do not let buffersize fall below this value
#define NET_HEADERSIZE                  (2 * sizeof(unsigned int))
#define NET_DATAGRAMSIZE                (MAX_DATAGRAM + NET_HEADERSIZE)
#define NET_FLAG_RELIABLE               (1<<31)
#define NET_FLAG_UNRELIABLE             (1<<30)
#define NET_FLAG_EOM                    (1<<29)
#define NET_FLAG_ACK                    (1<<28)
#define NET_FLAG_CTL                    (1<<27)
#define NET_FLAG_BACKUP                 (1<<26)
#define NET_FLAG_LENGTH_MASK            (~(NET_FLAG_RELIABLE|NET_FLAG_UNRELIABLE|NET_FLAG_EOM|NET_FLAG_ACK|NET_FLAG_CTL|NET_FLAG_BACKUP))
#define NET_TIMEOUT_ALIVE               8000
#define NET_TIMEOUT_ACK                 1000
#define NET_TIMEOUT_CONNECTIONREQUEST   4000
#define NET_TIMEOUT_REBOOTGUESS         2000
#define NET_RETRY_CONNECTION            5
#define NET_TIMEOUT_MASTER              4000
#define NET_RETRY_MASTER                3
#define NET_BUFFERED_SIZE               512
#define NET_BUFFERED_QUEUE              3
#define NET_FILE_XFER_BLOCK_SIZE		1024
#define	NET_NAMELEN 64
#define MAX_DATAGRAM                1024    // Length of unreliable message
#define WRITE(Type) if (pPacketBuffer==NULL) return; if (iPacketLen + (int)sizeof(Type) > iPacketSize) return; *((Type *)(pPacketBuffer+iPacketLen)) = Val; iPacketLen += sizeof(Type);
#define READ(Val) if(iPacketLen<1) return 0; if (pPacketBuffer==NULL) return 0; iPacketCursor += sizeof(Val); if (iPacketCursor > iPacketLen) return 0; return *((Val *)(pPacketBuffer+iPacketCursor-sizeof(Val)));
#ifdef _WIN32
static struct stErrorEntry {
    int iID;
    const char *pMessage;
} pErrorList[]= {
    0,                    "no error",
    WSAEACCES,            "WSAEACCES - permission denied",
    WSAEADDRINUSE,        "WSAEADDRINUSE - address already in use",
    WSAEADDRNOTAVAIL,     "WSAEADDRNOTAVAIL - cannot assign requested address",
    WSAEAFNOSUPPORT,      "WSAEAFNOSUPPORT [Lost internet connection]",
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
/////////////////////////// Login operation codes
typedef enum {
    LOGIN_WHAT,
    GOOD_LOGIN,
    BAD_LOGIN,
    NEW_ACCOUNT,
    ALREADY_LOGGED_IN,
    TOO_MANY_PLAYERS,
    SERVER_MAXXED_OUT,
    ACCOUNT_EXPIRED,
    MASTER_LOGIN,
} eLoginStuff;
/////////////////////////// Protocol op codes
typedef enum {
    NET_NOTCONNECT=0,     // not connected to any host
    NET_LOGGINPROC,     // connected but not logged in
    NET_SYSBUSY,        // if some class internal things (filetransfer) are done
    NET_CONNECTED,      // ok normal connecting
} eConState;
/////////////////////////// CPacket class
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
/////////////////////////// Control packet responses
typedef enum {
    CTL_CONNECT = 1,
    CTL_PLACEHOLDER,
    CTL_REJECT,
    CTL_ACCEPT,
    CTL_PING,
    CTL_SERVERINFO,
    CTL_SERVERINFO_RESET,
    CTL_SERVERINFO_END,
    CTL_SERVERINFO_GET
} tSocketCtrlMsg;
/////////////////////////// CCSocket class
class CCSocket {
public:
    CCSocket(void);
    CCSocket(int iPort);
    CCSocket(int iPort,bool bListen);
    CCSocket(int iPort,bool bListen, bool What2);
    ~CCSocket();
    void             initSocket(void);
    virtual int      iGetMessage();             // 0 if no incomming data, 1 unreliable, 2 reliable
    virtual char     *pcGetMessage();
    virtual int      iGetMessageSize();
    virtual          CPacket *pGetMessage();
    virtual double   dReceiveStatus();          // gets message data
    virtual bool     bCanSendMessage ();        // returns true if a reliable message can be send
    virtual void     SendReliableMessage (char *pData, int iSize);
    virtual void     SendReliableMessage (CPacket *pMessage);
    virtual int     SendUnreliableMessage(char *pData,int iSize);
    virtual int     SendUnreliableMessage(CPacket *pMessage);
    virtual double   dSendStatus();             // sends a message
    virtual void     Disconnect();              // if a dead connection is returned by a get or send function, this function should be called when it is convenient
    void              SendMessageNext(void);
    void              ReSendMessage (void);
    //queue<int>       BufferedQ;               // sizes of the buffered packets
    //virtual void     SendBufferedMessage(char *pData,int iSize);
    //virtual void     SendBufferedMessage(CPacket *pMessage);
    //int              iExtractBufferedMessage(int iPos=0);
    //bool             bExtractBuffer;            // indicates that a buffered message was received
    //int              iCurrentBuffer ;           // current size of buffered data
    //char             *pBackupBuffer;            // queuebuffer for buffered messages
    //unsigned int     iBufferedSendSequence;     // current sendsequenece for buffered messages
    //unsigned int     iBufferedReceivedSequence; // current receivesequenece for buffered messages
    void              FinishCtlPacket(CPacket *pPacket);
    bool              bConnecting;
    int               dConnectRetries;
    int               dConnectRetry;             // how often attempt to retry to connect
    int               dConnectTimeOut;           // timeout for a connection request
    int               iLastLength;
    CPacket           *pTempPacket;              // temporary packet for pGetMessage
    bool              bConnected;                // exists a validated connection?
    bool              bCanSend;                  // indicates wether reliable messages can be sent or not
    bool              bSendNext;                 // can we send next part of the message
    char              *pSendBuffer;              // pointer to sendbuffer
    int               iCurBufSize;               // current size of messagebuffer
    int               iSendBufferLength;         // length of current message
    char              *pReceiveBuffer;           // pointer to receivebuffer
    int               iCurReceiveSize;           // current size of receivebuffer
    int               iReceiveBufferLength;      // currently read data
    int               iReceivedMessageLength;    // length of the last message
    int               iCurRecSize;
    long              dLastSendTime;             // time when last reliable message was sent
    long              dLastMessage;              // time when last message was received
    unsigned int     iSendSequence;             // current sendsequence for reliable messages
    unsigned int     iUnreliableSendSequence;   // current sendsequence for unreliable messages
    unsigned int     iUnreliableReceiveSequence;// current receivesequence for unreliable messages
    unsigned int     iReceiveSequence;          // current receivesequence for reliable messages
    long              dAliveTimeout;             // after that time the server or client has timed out
    long              dAckTimeOut;               // how long wait before resending a message
    long              dGuessReboot;
    long              dMasterTimeout;
    long              dMasterRetry;
    long              dBufferedSize;
    long              dBufferedQueue;
    unsigned int     iPacketsSent;              // total number of sent packets
    unsigned int     iPacketsReSent;            // total number of resent packets
    unsigned int     iShortPacketCount;         // total number of packets that are shorter than the header itself
    unsigned int     iLongPacketCount;
    unsigned int     iPacketsReceived;
    unsigned int     iDroppedDatagrams;
    unsigned int     iReceivedDuplicateCount;
    int               NET_GetSocketAddr(int iSocket, struct sockaddr *pAddr);
    CCSocket*           pAccept(int iReSocket,struct sockaddr *ReAddr);
    void              SetRemoteIPAddress(char *iNetAddress);
    void              SetRemotePort(int iPort);
    char              *pGetRemoteIPAddress(void);
    int               iGetRemotePort(void);
    char              *pGetLocalIPAddress(void);
    int               iGetLocalPort(void);
    int               OpenSocket(char *pAddress, int iPort);
    int               OpenSocket(int iPort);
    int               zOpenSocket(int iPort);
    int               zOpenSocket(char *pAddress, int iPort);
    int               CloseSocket(int iSocket);
    int               Listen(int iHostPort, int iState);
    int               CheckNewConnections (void);
    int               nRecv(SOCKET iSocket, char *pBuf, int iLen, struct sockaddr *pAddr);
    int               nRecv(char *pBuf, int iLen, struct sockaddr *pAddr);
    int               nSend(SOCKET iSocket, char *pBuf, int iLen, struct sockaddr *pAddr);
    int               nSend(char *pBuf, int iLen, struct sockaddr *pAddr);
    SOCKET             iSocket;
    struct  sockaddr_in ToAddr;
    struct  sockaddr_in FromAddr;
#pragma pack(1)
struct {
    unsigned int  iLen;
    unsigned int  iSequence;
    char          pData[MAX_DATAGRAM];
} PacketBuffer;
#pragma pack()
};
/////////////////////////// Network utility functions
#ifdef __cplusplus
extern "C" {
#endif
int     NET_GetNameFromAddr(struct sockaddr *pAddr, char *pName);
int     NET_GetAddrFromName(char *pName, struct sockaddr *pAddr);
char   *NET_pGetLastError(void);
char   *NET_pAddrToString(struct sockaddr *pAddr);
int     NET_AddrCompare(struct sockaddr *pAddr1, struct sockaddr *pAddr2);
void    FinishCtlPacket(CPacket *pPacket);
int     NET_Init(void);
int     NET_Shutdown(void);
#ifdef __cplusplus
}
#endif

#endif // _DLCS_C_NET

