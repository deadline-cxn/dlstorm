/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_net.cpp
 **   Class:        CPacket
 **                 CCSocket
 **   Description:  Network class and library
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/

#include "c_net.h"

/////////////////////////// CCSocket class
CCSocket::CCSocket() { initSocket(); }
CCSocket::CCSocket(int iPort) {
    initSocket();
    if (Listen(iPort, true) == -1) {
        printf("ERROR LISTENING ON PORT %d\n", iPort);
    } else {
        printf("SETTING LISTEN PORT TO %d\n", iPort);
    }
}
CCSocket::CCSocket(int iPort, bool bListen) { initSocket(); }
void CCSocket::initSocket(void) {
    iSocket                    = -1;
    iLastLength                = 0;
    pTempPacket                = NULL;
    bCanSend                   = false;
    bSendNext                  = false;
    iReceiveBufferLength       = 0;
    iReceivedMessageLength     = 0;
    dLastSendTime              = 0;
    iSendSequence              = 0;
    iUnreliableSendSequence    = 0;
    iUnreliableReceiveSequence = 0;
    iReceiveSequence           = 0;
    iCurReceiveSize            = NET_MINMESSAGE;
    iCurBufSize                = NET_MINMESSAGE;

    pSendBuffer = (char *)malloc(iCurBufSize);
    if (!pSendBuffer) {
        return;
    }

    pReceiveBuffer = (char *)malloc(iCurReceiveSize);
    if (!pReceiveBuffer) {
        FREE(pSendBuffer);
        return;
    }

    dAliveTimeout          = NET_TIMEOUT_ALIVE;
    dConnectRetry          = NET_RETRY_CONNECTION;
    dConnectTimeOut        = NET_TIMEOUT_CONNECTIONREQUEST;
    dAckTimeOut            = NET_TIMEOUT_ACK;
    dGuessReboot           = NET_TIMEOUT_REBOOTGUESS;
    dMasterTimeout         = NET_TIMEOUT_MASTER;
    dMasterRetry           = NET_RETRY_MASTER;
    dBufferedSize          = NET_BUFFERED_SIZE;
    dBufferedQueue         = NET_BUFFERED_QUEUE;
    PacketBuffer.iLen      = 0;
    PacketBuffer.iSequence = 0;
    memset(PacketBuffer.pData, 0, MAX_DATAGRAM);
}

CCSocket::~CCSocket() {
    CloseSocket(iSocket);
    FREE(pSendBuffer);
    FREE(pReceiveBuffer);
}
void CCSocket::SendReliableMessage(CPacket *pMessage) {
    if (!pMessage) return;
    SendReliableMessage(pMessage->pGetPacketBuffer(), pMessage->iGetCurSize());
}
void CCSocket::SendReliableMessage(const char *pData, int iSize) {
    unsigned int iPacketLen;
    unsigned int iDataLen;
    unsigned int eom;
    int          ret;
    if (iSize < 0) return;       // Log("CCSocket::SendReliableMessage() packet size is not compliant.");
    if (!bCanSend) return;       // Log("CCSocket::SendReliableMessage() bCanSend is false.");
    if (!pSendBuffer) return;    // Log("CCSocket::SendReliableMessage() pSendBuffer is null (1)");
    if (!pData) return;          // Log("CCSocket::SendReliableMessage() pSendBuffer is null (1)");
    if (iCurBufSize >= iSize) {  // should we resize the buffer?
        if ((iSize < NET_MINMESSAGE) && (iCurBufSize != NET_MINMESSAGE)) {
            pSendBuffer = (char *)realloc((void *)pSendBuffer, NET_MINMESSAGE);
            iCurBufSize = NET_MINMESSAGE;
        }
    } else {
        pSendBuffer = (char *)realloc((void *)pSendBuffer, iSize);
        iCurBufSize = iSize;
    }
    if (!pSendBuffer) return;
    memcpy(pSendBuffer, pData, iSize);
    iSendBufferLength = iSize;
    if (iSize <= MAX_DATAGRAM) {
        iDataLen = iSize;
        eom      = NET_FLAG_EOM;
    } else {
        iDataLen = MAX_DATAGRAM;
        eom      = 0;
    }
    iPacketLen             = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen      = htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bCanSend = false;
    ret      = nSend((const char *)&PacketBuffer, iPacketLen, (struct sockaddr *)&ToAddr);
    if ((ret == -1) || (ret != (int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}
void CCSocket::SendMessageNext() {
    unsigned int iPacketLen, iDataLen, eom;
    int          ret;
    if (iSendBufferLength <= MAX_DATAGRAM) {
        iDataLen = iSendBufferLength;
        eom      = NET_FLAG_EOM;
    } else {
        iDataLen = MAX_DATAGRAM;
        eom      = 0;
    }
    iPacketLen             = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen      = htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret       = nSend((const char *)&PacketBuffer, iPacketLen, (struct sockaddr *)&ToAddr);
    if ((ret == -1) || (ret != (int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}
void CCSocket::ReSendMessage(void) {
    unsigned int iPacketLen, iDataLen, eom;
    iPacketLen = 0;
    iDataLen   = 0;
    eom        = 0;
    int ret;
    if (iSendBufferLength <= MAX_DATAGRAM) {
        iDataLen = iSendBufferLength;
        eom      = NET_FLAG_EOM;
    } else {
        iDataLen = MAX_DATAGRAM;
        eom      = 0;
    }

    if (iDataLen > MAX_DATAGRAM) return;

    iPacketLen = NET_HEADERSIZE + iDataLen;

    PacketBuffer.iLen      = htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence - 1);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret       = nSend((const char *)&PacketBuffer, iPacketLen, (struct sockaddr *)&ToAddr);
    if ((ret == -1) || (ret != (int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsReSent++;
}
bool CCSocket::bCanSendMessage(void) {
    if (bSendNext) SendMessageNext();
    if ((!bCanSend) && (dlcs_get_tickcount() - dLastMessage > dAliveTimeout)) return 0;
    return bCanSend;
}
int CCSocket::SendUnreliableMessage(CPacket *pMessage) {
    if (!pMessage) return -4;
    return SendUnreliableMessage(pMessage->pGetPacketBuffer(), pMessage->iGetCurSize());
}
int CCSocket::SendUnreliableMessage(const char *pData, int iSize) {
    int iPacketLen;
    int ret;
    if (iSize < 0) return -1;
    if (iSize >= MAX_DATAGRAM) return -2;
    if (!pData) return -3;
    iPacketLen             = NET_HEADERSIZE + iSize;
    PacketBuffer.iLen      = htonl(iPacketLen | NET_FLAG_UNRELIABLE);
    PacketBuffer.iSequence = htonl(iUnreliableSendSequence++);
    memcpy(PacketBuffer.pData, pData, iSize);
    ret = nSend((const char *)&PacketBuffer, iPacketLen, (struct sockaddr *)&ToAddr);
    // if ((ret == -1) || (ret != iPacketLen)) { }
    iPacketsSent++;
    return ret;
}

int CCSocket::iGetMessage() {
    int          iLength;
    unsigned int iFlags;
    int          err = 0;
    unsigned int iSequence;
    unsigned int iCount;
    if ((!bCanSend) && ((dlcs_get_tickcount() - dLastSendTime) > dAckTimeOut)) ReSendMessage();
    if (iSocket == -1) return 0;
    iLastLength = 0;
    while (1) {
        iLength = nRecv((char *)&PacketBuffer, NET_DATAGRAMSIZE, (struct sockaddr *)&FromAddr);

        if (iLength == -1) {
            printf("ya done goofed 000\n");
            return -1;
        }

        if (iLength == 0) break;
#ifdef _WIN32
        if (&FromAddr.sin_addr.s_addr != &ToAddr.sin_addr.s_addr)
#else

        if (&FromAddr.sin_addr.s_addr != &ToAddr.sin_addr.s_addr)
#endif

        {
            // Log
            // printf("CCSocket::iGetMessage() forged packet received from %s - supposed to be from %s",NET_pAddrToString(&Read_Addr),NET_pAddrToString(&reAddr));
            // printf("ya done goofed\n");
            // continue;
        }

        if (iLength < NET_HEADERSIZE) {
            iShortPacketCount++;
            continue;
        }
        iLength = ntohl(PacketBuffer.iLen);
        iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
        iLength &= NET_FLAG_LENGTH_MASK;
        if (iFlags & NET_FLAG_CTL) continue;
        if (iLength > NET_DATAGRAMSIZE) {
            // Log
            // printf("CCSocket::iGetMessage() forged header received %s",NET_pAddrToString(&Read_Addr.stAddr));
            printf("ya done goofed 2\n");
            iLongPacketCount++;
            continue;
        }

        // finally a valid message

        iSequence = ntohl(PacketBuffer.iSequence);
        iPacketsReceived++;
        dLastMessage = dlcs_get_tickcount();
        if (iFlags & NET_FLAG_UNRELIABLE) {
            if (iSequence < iUnreliableReceiveSequence) {
                // Log("CCSocket::iGetMessage() got a stale datagram");
                printf("ya done goofed 3\n");
                err = 0;
                break;
            }
            if (iSequence != iUnreliableReceiveSequence) {
                iCount = iSequence - iUnreliableReceiveSequence;
                iDroppedDatagrams += iCount;
                // Log("CCSocket::iGetMessage() dropped %u datagram(s)", iCount);
                printf("ya done goofed 4\n");
            }
            iUnreliableReceiveSequence = iSequence + 1;
            iLength -= NET_HEADERSIZE;
            iLastLength = iLength;
            err         = 2;
            break;
        }

        if (iFlags & NET_FLAG_ACK) {
            if (iSequence != (iSendSequence - 1)) {
                // Log("CCSocket::iGetMessage() stale ACK received %s",NET_pAddrToString(&ReadAddr.stAddr));
                printf("ya done goofed 5\n");
                continue;
            }
            iSendBufferLength -= MAX_DATAGRAM;
            if (iSendBufferLength > 0) {
                memmove(pSendBuffer, pSendBuffer + MAX_DATAGRAM, iSendBufferLength);
                bSendNext = true;
            } else {
                iSendBufferLength = 0;
                bCanSend          = true;
            }
            continue;
        }

        if (iFlags & NET_FLAG_RELIABLE) {
            PacketBuffer.iLen      = ntohl(NET_HEADERSIZE | NET_FLAG_ACK);
            PacketBuffer.iSequence = ntohl(iSequence);
            if (nSend((const char *)&PacketBuffer, NET_HEADERSIZE, (struct sockaddr *)&ToAddr) == -1) {
                printf("ya done goofed 7\n");
                return -1;
            }
            if (iSequence != iReceiveSequence) {
                iReceivedDuplicateCount++;
                continue;
            }
            iReceiveSequence++;
            iLength -= NET_HEADERSIZE;
            if (iReceiveBufferLength + (int)iLength < iCurReceiveSize) {
                if ((iReceiveBufferLength + iLength < NET_MINMESSAGE) && (iCurReceiveSize != NET_MINMESSAGE)) {
                    pReceiveBuffer  = (char *)realloc((void *)pReceiveBuffer, NET_MINMESSAGE);
                    iCurReceiveSize = NET_MINMESSAGE;
                }
            } else {
                pReceiveBuffer  = (char *)realloc((void *)pReceiveBuffer, iReceiveBufferLength + iLength);
                iCurReceiveSize = iReceiveBufferLength + iLength;
            }
            if (iFlags & NET_FLAG_EOM) {
                memcpy(pReceiveBuffer + iReceiveBufferLength, PacketBuffer.pData, iLength);
                iReceivedMessageLength = iReceiveBufferLength + iLength;
                iReceiveBufferLength   = 0;
                err                    = 1;
                break;
            }
            memcpy(pReceiveBuffer + iReceiveBufferLength, PacketBuffer.pData, iLength);
            iReceiveBufferLength += iLength;
            continue;
        }
    }
    if (bSendNext) SendMessageNext();
    return err;
}
const char *CCSocket::pcGetMessage() {
    assert(pReceiveBuffer != NULL);
    if (iLastLength)
        return PacketBuffer.pData;
    else
        return pReceiveBuffer;
}
int CCSocket::iGetMessageSize() {
    if (iLastLength)
        return iLastLength;
    else
        return iReceivedMessageLength;
}
CPacket *CCSocket::pGetMessage() {
    if (!pTempPacket) {
        pTempPacket = new CPacket(iGetMessageSize(), pcGetMessage());
        if (!pTempPacket) return 0;
    } else {
        pTempPacket->Reset();
        pTempPacket->SetMaxSize(iGetMessageSize(), pcGetMessage());
        pTempPacket->SetCurSize(iGetMessageSize());
    }
    return pTempPacket;
}
CCSocket *CCSocket::pAccept(int iReSocket, struct sockaddr *ReAddr) {
    CPacket         Send(NET_DATAGRAMSIZE);
    struct sockaddr NewAddr;

    iSocket = OpenSocket(0);
    if (iSocket == -1) {  // Log("CCSocket::pAccept() unable to connect to host");
        return 0;
    }

    NET_GetSocketAddr(iSocket, &NewAddr);  // here is where it gets port info

    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_ACCEPT);
    Send.Write(iGetRemotePort());  // SocketPort(&NewAddr)); // here is where it sends port back to client
    FinishCtlPacket(&Send);

    if (  // NET_Write(iReSocket,
        nSend(Send.pGetPacketBuffer(), Send.iGetCurSize(), ReAddr) == -1) {
        // Log("CCSocket::pAccept() error making a new connection");
        return 0;
    }
    bCanSend   = true;
    bConnected = true;
    return this;
}
void CCSocket::Disconnect() { CloseSocket(iSocket); }
// void CCSocket::SetAddress(CInetAddress *pAddress){    pAddr = pAddress;}
double CCSocket::dReceiveStatus() { return 0; }
double CCSocket::dSendStatus() { return 0; }
// CInetAddress *CCSocket::pGetAddress(void){    return pAddr; }
void CCSocket::FinishCtlPacket(CPacket *pPacket) { ::FinishCtlPacket(pPacket); }
int  CCSocket::Listen(int iHostPort, int iState) {
    if (iState) {
        if (iSocket != INVALID_SOCKET) {
            return 0;
        }
        if ((iSocket = OpenSocket(iHostPort)) == INVALID_SOCKET) return -1;
    } else {
        if (iSocket == INVALID_SOCKET) return 0;

        CloseSocket(iSocket);

        iSocket = INVALID_SOCKET;
    }
    return 0;
}
int CCSocket::OpenSocket(int iPort) { return OpenSocket("127.0.0.1", iPort); }
int CCSocket::OpenSocket(const char *pAddress, int iPort) {
    ToAddr.sin_family = AF_INET;
    ToAddr.sin_port   = htons((short)iPort);
#ifdef _WIN32
    ToAddr.sin_addr.s_addr = inet_addr(pAddress);
#else
    inet_pton(AF_INET, pAddress, &(ToAddr.sin_addr));
#endif

    iSocket = -1;

    if ((iSocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) return -1;

#ifdef _WIN32
    u_long _true = 1;
    if (ioctlsocket(iSocket, FIONBIO, &_true) == SOCKET_ERROR)
#else
    bzero(&pAddress, sizeof(pAddress));  // linux func
    if (fcntl(iSocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
        CloseSocket(iSocket);
        return SOCKET_ERROR;
    }

#ifdef _WIN32
    if (bind(iSocket, (struct sockaddr *)&ToAddr, sizeof(ToAddr)) == SOCKET_ERROR) {
        CloseSocket(iSocket);
        return SOCKET_ERROR;
    }

#else

    bind(iSocket, (struct sockaddr *)&ToAddr, sizeof ToAddr);

#endif
    return iSocket;
}
int CCSocket::zOpenSocket(int iPort) {
    int                newsocket;
    struct sockaddr_in address;
    if ((newsocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) return -1;
#ifdef _WIN32
    u_long _true = 1;
    if (ioctlsocket(newsocket, FIONBIO, &_true) == SOCKET_ERROR)
#else
    bzero(&address, sizeof(address));  // linux func
    if (fcntl(newsocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons((short)iPort);

    ToAddr.sin_family = AF_INET;
    // reAddr.sin_addr.s_addr = INADDR_ANY;
    ToAddr.sin_port = htons((short)iPort);

#ifdef _WIN32
    if (bind(newsocket, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
#else
    bind(newsocket, (struct sockaddr *)&address, sizeof(address));

#endif

    return newsocket;
}
int CCSocket::zOpenSocket(const char *pAddress, int iPort) {
    int                newsocket;
    struct sockaddr_in address;
    if ((newsocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) return -1;
#ifdef _WIN32
    u_long _true = 1;
    if (ioctlsocket(newsocket, FIONBIO, &_true) == SOCKET_ERROR)
#else
    bzero(&address, sizeof(address));  // linux func
    if (fcntl(newsocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = inet_addr(pAddress);  // INADDR_ANY;
    address.sin_port        = htons((short)iPort);

    ToAddr.sin_family      = AF_INET;
    ToAddr.sin_addr.s_addr = inet_addr(pAddress);  // INADDR_ANY;
    ToAddr.sin_port        = htons((short)iPort);

#ifdef _WIN32
    if (bind(newsocket, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
#else
    bind(newsocket, (struct sockaddr *)&address, sizeof(address));

#endif
    return newsocket;
}
int CCSocket::CloseSocket(int iSocket) {
#ifdef _WIN32
    return closesocket(iSocket);
#else
    return close(iSocket);
#endif
}
int CCSocket::CheckNewConnections(void) {
    char pBuffer[65535];
    if (iSocket == INVALID_SOCKET) return -1;
    if (recvfrom(iSocket, pBuffer, sizeof(pBuffer), MSG_PEEK, NULL, NULL) > 0) return iSocket;
    return -1;
}
int CCSocket::nRecv(char *pBuf, int iLen, struct sockaddr *pAddr) { return nRecv(iSocket, pBuf, iLen, pAddr); }
int CCSocket::nRecv(SOCKET iSocket, char *pBuff, int iLen, struct sockaddr *pAddr) {
    int iAddrlen = sizeof(struct sockaddr);
    int err;
#ifndef _WIN32
    fd_set         readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(iSocket, &readfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 5;
    if (select(iSocket + 1, &readfds, NULL, NULL, &tv) == -1) {
        return 0;
    }
    err = 0;

    if (FD_ISSET(iSocket, &readfds) > 0) {
        err = recvfrom(iSocket, pBuff, iLen, 0, (struct sockaddr *)pAddr, (socklen_t *)&iAddrlen);
    }

#endif

#ifdef _WIN32
    int errnum;

    err = recvfrom(iSocket, pBuff, iLen, 0, (struct sockaddr *)pAddr, &iAddrlen);

    if (err == SOCKET_ERROR) {
        errnum = WSAGetLastError();
        if (errnum == WSAEWOULDBLOCK || errnum == WSAECONNREFUSED) {
            return 0;
        }
    }

#endif
    // if(err==-1) { Log("%s on recvfrom()",NET_pGetLastError()); }
    return err;
}
int CCSocket::nSend(const char *pBuf, int iLen, struct sockaddr *pAddr) { return nSend(iSocket, pBuf, iLen, pAddr); }
int CCSocket::nSend(SOCKET iSocket, const char *pBuf, int iLen, struct sockaddr *pAddr) {
    int err;
#ifdef SIMULATE_CONNECTION
    if (!(rand() % 10)) return iLen;
#endif  // SIMULATE_CONNECTION

#ifdef _WIN32
    err = sendto(iSocket, pBuf, iLen, 0, (struct sockaddr *)pAddr, sizeof(struct sockaddr));
#else
    err = sendto(iSocket, pBuf, iLen, 0, pAddr, sizeof(struct sockaddr));
#endif
    return err;
}
int CCSocket::NET_GetSocketAddr(int iSocket, struct sockaddr *pAddr) {
    int addrlen = sizeof(struct sockaddr);
    memset(pAddr, 0, sizeof(struct sockaddr));
    int a;
#ifdef _WIN32
    a = getsockname(iSocket, (struct sockaddr *)pAddr, &addrlen);
#else
    a = getsockname(iSocket, (struct sockaddr *)pAddr, (socklen_t *)&addrlen);
#endif
    // unsigned int a; // a = ((struct sockaddr_in *)pAddr)->sin_addr.s_addr;
    return a;
}
void CCSocket::SetRemotePort(int iPort) {
    if (iPort > 0xffff) return;
    if (iPort < 1024) return;
    ToAddr.sin_port = htons((short)iPort);
}
int CCSocket::iGetLocalPort(void) {
    struct sockaddr_in local_address;
    socklen_t          addr_size = sizeof(local_address);
    getsockname(iSocket, (struct sockaddr *)&local_address, &addr_size);
    return ntohs(local_address.sin_port);
}
int   CCSocket::iGetRemotePort(void) { return ntohs(ToAddr.sin_port); }
char *CCSocket::pGetRemoteIPAddress() { return inet_ntoa(ToAddr.sin_addr); }
char *CCSocket::pGetLocalIPAddress() {
    struct sockaddr_in local_address;
    socklen_t          addr_size = sizeof(local_address);
    getsockname(iSocket, (struct sockaddr *)&local_address, &addr_size);
    return inet_ntoa(local_address.sin_addr);
}
/////////////////////////// CPacket class
CPacket::CPacket() {
    pPacketBuffer = NULL;
    iPacketSize   = 0;
    iPacketCursor = 0;
    iPacketLen    = 0;
    bUserAlloc    = false;
}
CPacket::CPacket(int iSize) {
    if (iSize < 0) return;
    pPacketBuffer = (char *)malloc(iSize);
    if (pPacketBuffer == NULL) return;
    iPacketSize   = iSize;
    iPacketCursor = 0;
    iPacketLen    = 0;
    bUserAlloc    = false;
}
CPacket::CPacket(int iSize, const char *pBuffer) {
    if (iSize < 0) return;
    if (pBuffer == NULL) return;
    pPacketBuffer = (char *)pBuffer;
    iPacketLen = iPacketSize = iSize;
    iPacketCursor            = 0;
    bUserAlloc               = true;
}
CPacket::~CPacket() {
    if ((!bUserAlloc) && (pPacketBuffer != NULL)) free(pPacketBuffer);
}
void CPacket::DumpPacket(void) {
    printf("-=[Packet Info]=========================================================-");
    for (int y = 0; y < iPacketLen; y++) {
        if (pPacketBuffer[y] < 32) {
        } else {
        }
    }
    printf("-==========================================================[End Packet]=-");
}
int  CPacket::iGetMaxSize(void) { return iPacketSize; }
void CPacket::SetMaxSize(int iSize, const char *pBuffer) {
    if (iSize < 0) return;
    if ((bUserAlloc) && (pBuffer)) {
        pPacketBuffer = (char *)pBuffer;
        iPacketSize   = iSize;
        return;
    }

    iPacketSize = iSize;
    if (pPacketBuffer == NULL) {
        pPacketBuffer = (char *)malloc(iSize);
        if (!pPacketBuffer) return;
    }
    pPacketBuffer = (char *)realloc(pPacketBuffer, iSize);
    if (pPacketBuffer == NULL) return;
}
int  CPacket::iGetCurSize(void) { return iPacketLen - iPacketCursor; }
void CPacket::SetCurSize(int iNewLen) { iPacketLen = iNewLen; }
void CPacket::Reset(void) {
    iPacketCursor = 0;
    iPacketLen    = 0;
}
void  CPacket::Rewind(void) { iPacketCursor = 0; }
char *CPacket::pGetPacketBuffer(void) { return pPacketBuffer; }
void  CPacket::Write(int Val) { WRITE(int) }
void  CPacket::Write(long Val) { WRITE(long) }
void  CPacket::Write(char Val) { WRITE(char) }
void  CPacket::Write(short Val) { WRITE(short) }
void  CPacket::Write(float Val) { WRITE(float) }
void  CPacket::Write(const char *Val) {
    int i = strlen(Val) + 1;
    if (pPacketBuffer == NULL) return;
    if (iPacketLen + i > iPacketSize) return;
    strcpy(pPacketBuffer + iPacketLen, Val);
    iPacketLen += i;
}
void CPacket::Write(const char *Val, int iSize) {
    if (!iSize) return;
    if (pPacketBuffer == NULL) return;
    if (iPacketLen + iSize > iPacketSize) return;
    memcpy(pPacketBuffer + iPacketLen, Val, iSize);
    iPacketLen += iSize;
}
const char *CPacket::pWrite(int iSize) {
    if (!iSize) return NULL;
    if (!pPacketBuffer) return NULL;
    if (iPacketLen + iSize > iPacketSize) return NULL;
    iPacketLen += iSize;
    return pPacketBuffer + iPacketLen - iSize;
}
int         CPacket::iRead(void) { READ(int) }
long        CPacket::dwRead(void) { READ(long) }
char        CPacket::cRead(void) { READ(char) }
short       CPacket::sRead(void) { READ(short) }
float       CPacket::fRead(void) { READ(float) }
const char *CPacket::pRead(void) {
    int i;
    if (iPacketLen < 1) return ("null");
    if (!pPacketBuffer) return ("null");
    i = iPacketCursor;
    iPacketCursor += strlen(pPacketBuffer + iPacketCursor) + 1;
    if (iPacketCursor > iPacketLen) return ("null");
    return (const char *)(pPacketBuffer + i);
}
const char *CPacket::pRead(int iSize) {
    int i;
    if (iSize == 0) return ("null");
    if (iPacketLen < 1) return ("null");
    if (pPacketBuffer == NULL) return ("null");
    i = iPacketCursor;
    iPacketCursor += iSize;
    if (iPacketCursor > iPacketLen) return ("null");
    return (const char *)(pPacketBuffer + i);
}
/////////////////////////// Network utility functions
int NET_Init() {
#ifdef _WIN32
    int     err;
    WSADATA winsockdata;
    err = WSAStartup(MAKEWORD(1, 1), &winsockdata);
    if (err != 0) {
        return -1;
    }
#endif
    return 0;
}
int NET_Shutdown(void) {
    // NET_Listen(0);
#ifdef _WIN32
    if (WSACleanup() != 0) return -1;
#endif
    return 0;
}
const char *NET_pGetLastError(void) {
#ifdef _WIN32
    map<int, std::string> WindowsErrorMap;
    WindowsErrorMap[0]                     = "no error";
    WindowsErrorMap[WSAEACCES]             = "WSAEACCES - permission denied";
    WindowsErrorMap[WSAEADDRINUSE]         = "WSAEADDRINUSE - address already in use";
    WindowsErrorMap[WSAEADDRNOTAVAIL]      = "WSAEADDRNOTAVAIL - cannot assign requested address";
    WindowsErrorMap[WSAEAFNOSUPPORT]       = "WSAEAFNOSUPPORT [Lost internet connection]";
    WindowsErrorMap[WSAEALREADY]           = "WSAEALREADY - operation already in progress";
    WindowsErrorMap[WSAECONNABORTED]       = "WSAECONNABORTED - software caused connection abort";
    WindowsErrorMap[WSAECONNREFUSED]       = "WSAECONNREFUSED - connection refused";
    WindowsErrorMap[WSAECONNRESET]         = "WSAECONNRESET - connection reset by peer";
    WindowsErrorMap[WSAEDESTADDRREQ]       = "WSAEDESTADDRREQ - destination address required";
    WindowsErrorMap[WSAEFAULT]             = "WSAEFAULT - bad address";
    WindowsErrorMap[WSAEHOSTDOWN]          = "WSAEHOSTDOWN - host is down";
    WindowsErrorMap[WSAEHOSTUNREACH]       = "WSAEHOSTUNREACH - no route to host";
    WindowsErrorMap[WSAEINPROGRESS]        = "WSAEINPROGRESS - operation now in progress";
    WindowsErrorMap[WSAEINTR]              = "WSAEINTR - interrupted function call";
    WindowsErrorMap[WSAEINVAL]             = "WSAEINVAL - invalid argument";
    WindowsErrorMap[WSAEISCONN]            = "WSAEISCONN - socket is already connected";
    WindowsErrorMap[WSAEMFILE]             = "WSAEMFILE  - too many open files";
    WindowsErrorMap[WSAEMSGSIZE]           = "WSAEMSGSIZE - message too long";
    WindowsErrorMap[WSAENETDOWN]           = "WSAENETDOWN - network is down";
    WindowsErrorMap[WSAENETRESET]          = "WSAENETRESET - network dropped connection on reset";
    WindowsErrorMap[WSAENETUNREACH]        = "WSAENETUNREACH - network is unreachable";
    WindowsErrorMap[WSAENOBUFS]            = "WSAENOBUFS - no buffer space available";
    WindowsErrorMap[WSAENOPROTOOPT]        = "WSAENOPROTOOPT - bad protocol option";
    WindowsErrorMap[WSAENOTCONN]           = "WSAENOTCONN - socket is not connected";
    WindowsErrorMap[WSAENOTSOCK]           = "WSAENOTSOCK - socket operation on non-socket";
    WindowsErrorMap[WSAEOPNOTSUPP]         = "WSAEOPNOTSUPP - operation not supported";
    WindowsErrorMap[WSAEPFNOSUPPORT]       = "WSAEPFNOSUPPORT - protocol family not supported";
    WindowsErrorMap[WSAEPROCLIM]           = "WSAEPROCLIM - too many processes";
    WindowsErrorMap[WSAEPROTONOSUPPORT]    = "WSAEPROTONOSUPPORT - protocol not supported";
    WindowsErrorMap[WSAEPROTOTYPE]         = "WSAEPROTOTYPE - protocol wrong type for socket";
    WindowsErrorMap[WSAESHUTDOWN]          = "WSAESHUTDOWN - cannot send after socket shutdown";
    WindowsErrorMap[WSAESOCKTNOSUPPORT]    = "WSAESOCKTNOSUPPORT - socket type not supported";
    WindowsErrorMap[WSAETIMEDOUT]          = "WSAETIMEDOUT - connection timed out";
    WindowsErrorMap[WSATYPE_NOT_FOUND]     = "WSATYPE_NOT_FOUND - class type not found";
    WindowsErrorMap[WSAEWOULDBLOCK]        = "WSAEWOULDBLOCK - resource temporarily unavailable";
    WindowsErrorMap[WSAHOST_NOT_FOUND]     = "WSAHOST_NOT_FOUND - host not found";
    WindowsErrorMap[WSA_INVALID_HANDLE]    = "WSA_INVALID_HANDLE - specified event object handle is invalid";
    WindowsErrorMap[WSA_INVALID_PARAMETER] = "WSA_INVALID_PARAMETER - one or more parameters are invalid";
    WindowsErrorMap[WSA_IO_INCOMPLETE]     = "WSA_IO_INCOMPLETE - overlapped i/o event object not in signaled state";
    WindowsErrorMap[WSA_IO_PENDING]        = "WSA_IO_PENDING - overlapped operations will complete later";
    WindowsErrorMap[WSA_NOT_ENOUGH_MEMORY] = "WSA_NOT_ENOUGH_MEMORY - insufficient nemory available";
    WindowsErrorMap[WSANOTINITIALISED]     = "WSANOTINITIALISED - successful WSASartup() not yet performened";
    WindowsErrorMap[WSANO_DATA]            = "WSANO_DATA - valid name, no data record of requested type";
    WindowsErrorMap[WSANO_RECOVERY]        = "WSANO_RECOVERY - this is a non-recoverable error";
    WindowsErrorMap[WSASYSCALLFAILURE]     = "WSASYSCALLFAILURE - system call failure";
    WindowsErrorMap[WSASYSNOTREADY]        = "WSASYSNOTREADY - network subsystem is unavailable";
    WindowsErrorMap[WSATRY_AGAIN]          = "WSATRY_AGAIN - non-authoriative host not found";
    WindowsErrorMap[WSAVERNOTSUPPORTED]    = "WSAVERNOTSUPPORTED - winsock.dll version out of range";
    WindowsErrorMap[WSAEDISCON]            = "WSAEDISCON - graceful shutdown in progress";
    WindowsErrorMap[WSA_OPERATION_ABORTED] = "WSA_OPERATION_ABORTED - overlapped operation aborted";
    return (const char *)WindowsErrorMap[WSAGetLastError()].c_str();

#else
    return (strerror(errno));
#endif
}
void FinishCtlPacket(CPacket *pPacket) {
    int j                                 = (NET_FLAG_CTL | (pPacket->iGetCurSize() & NET_FLAG_LENGTH_MASK));
    *((int *)pPacket->pGetPacketBuffer()) = htonl(j);
}
int NET_GetNameFromAddr(struct sockaddr *pAddr, char *pName) {
    struct hostent *hostentry;
    hostentry = gethostbyaddr((const char *)&((struct sockaddr_in *)pAddr)->sin_addr, sizeof(struct in_addr), AF_INET);
    if (hostentry) {
        strncpy(pName, (const char *)hostentry->h_name, NET_NAMELEN - 1);
        return 0;
    }
    strcpy(pName, NET_pAddrToString(pAddr));
    return 0;
}
int NET_GetAddrFromName(const char *pName, struct sockaddr *pAddr) {
    struct hostent *hostentry;
    hostentry = gethostbyname(pName);
    if (!hostentry) return SOCKET_ERROR;
    pAddr->sa_family                               = AF_INET;
    ((struct sockaddr_in *)pAddr)->sin_addr.s_addr = *(int *)hostentry->h_addr_list[0];
    return 0;
}
int NET_AddrCompare(struct sockaddr *pAddr1, struct sockaddr *pAddr2) {
    if (pAddr1->sa_family != pAddr2->sa_family) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_addr.s_addr != ((struct sockaddr_in *)pAddr2)->sin_addr.s_addr) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_port != ((struct sockaddr_in *)pAddr2)->sin_port) return 1;
    return 0;
}
const char *NET_pAddrToString(struct sockaddr *pAddr) {
    static char buffer[22];
    if (pAddr == NULL)
        strcpy(buffer, "error!");
    else {
        // int taddr = ((struct sockaddr_in *)pAddr)->sin_addr.s_addr;
        int haddr = ntohl(((struct sockaddr_in *)pAddr)->sin_addr.s_addr);
        sprintf(buffer, "%d.%d.%d.%d:%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff, ntohs(((struct sockaddr_in *)pAddr)->sin_port));
    }
    return buffer;
}

bool dlcs_get_hostname(char *szHost) {
    gethostname(szHost, _HOST_NAME_MAX);
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool dlcs_get_ipaddress(char *szIP) {
    dlcsm_make_str(szTemp1);
    dlcsm_make_str(szTemp2);
    strcpy(szTemp2, "127.0.0.1");  // TODO: Add actual computer ip address here
    dlcs_get_hostname(szTemp2);
    struct hostent *phe = gethostbyname(szTemp2);
    if (phe == 0) {
        strcpy(szTemp1, "ERROR");
    } else {
        for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
            struct in_addr addr;
            memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
            strcpy(szTemp1, inet_ntoa(addr));
        }
    }
    strcpy(szIP, szTemp1);
    return true;
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////
void DLCODESTORM::dlcs_get_webpage2(string url, char *x) {  }
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
        return 1;
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
    s.assign(tolower(instr.c_str()));
    word.assign(tolower(word.c_str()));
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


    }
    freeaddrinfo(servinfo); // all done with this structure
    return ip;
}
*/

// const char *dlcs_inet_ntoa(string ip) {    return ip; }
