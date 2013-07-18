/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    Network class

****************************************************************/

#include "c_net.h"

CCSocket::CCSocket() {
    initSocket();
}
////////////////////////////////////////////////////////
CCSocket::CCSocket(int iPort) {
    initSocket();
    if(Listen(iPort, true)==-1) {
        printf("ERROR LISTENING ON PORT %d\n",iPort);
    } else {
        printf("SETTING LISTEN PORT TO %d\n",iPort);
    }
}

CCSocket::CCSocket(int iPort,bool bListen) {
    initSocket();

}

void CCSocket::initSocket(void) {
    iSocket = -1;
    iLastLength = 0;
    pTempPacket = NULL;
    bCanSend = false;
    bSendNext = false;
    iReceiveBufferLength=0;
    iReceivedMessageLength=0;
    dLastSendTime = 0;
    iSendSequence = 0;
    iUnreliableSendSequence = 0;
    iUnreliableReceiveSequence =0;
    iReceiveSequence=0;
    iCurReceiveSize= NET_MINMESSAGE;
    iCurBufSize = NET_MINMESSAGE;

    pSendBuffer = (char *)malloc(iCurBufSize);
    if(!pSendBuffer)    {
        return;
    }

    pReceiveBuffer = (char *)malloc(iCurReceiveSize);
    if(!pReceiveBuffer)    {
        FREE(pSendBuffer);
        return;
    }

    dAliveTimeout   = NET_TIMEOUT_ALIVE;
    dConnectRetry   = NET_RETRY_CONNECTION;
    dConnectTimeOut = NET_TIMEOUT_CONNECTIONREQUEST;
    dAckTimeOut     = NET_TIMEOUT_ACK;
    dGuessReboot    = NET_TIMEOUT_REBOOTGUESS;
    dMasterTimeout  = NET_TIMEOUT_MASTER;
    dMasterRetry    = NET_RETRY_MASTER;
    dBufferedSize   = NET_BUFFERED_SIZE;
    dBufferedQueue  = NET_BUFFERED_QUEUE;
    PacketBuffer.iLen=0;
    PacketBuffer.iSequence=0;
    memset(PacketBuffer.pData,0,MAX_DATAGRAM);
}

////////////////////////////////////////////////////////

CCSocket::~CCSocket() {
    CloseSocket(iSocket);
    FREE(pSendBuffer);
    FREE(pReceiveBuffer);
}

//////////////////////////////////////////////////////////////

void CCSocket::SendReliableMessage(CPacket *pMessage) {
    if(!pMessage) return;
    SendReliableMessage( pMessage->pGetPacketBuffer(), pMessage->iGetCurSize() );
}

//////////////////////////////////////////////////////////////

void CCSocket::SendReliableMessage (char *pData, int iSize) {
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int  eom;
    int ret;
    if(iSize < 0) return;       //Log("CCSocket::SendReliableMessage() packet size is not compliant.");
    if(!bCanSend) return;       //Log("CCSocket::SendReliableMessage() bCanSend is false.");
    if(!pSendBuffer) return;    //Log("CCSocket::SendReliableMessage() pSendBuffer is null (1)");
    if(!pData) return;          //Log("CCSocket::SendReliableMessage() pSendBuffer is null (1)");
    if(iCurBufSize>=iSize) {    //should we resize the buffer?
        if((iSize<NET_MINMESSAGE)&&(iCurBufSize!=NET_MINMESSAGE)) {
            pSendBuffer = (char *)realloc((void *)pSendBuffer,NET_MINMESSAGE);
            iCurBufSize = NET_MINMESSAGE;
        }
    } else {
        pSendBuffer = (char *)realloc((void *)pSendBuffer,iSize);
        iCurBufSize = iSize;
    }
    if(!pSendBuffer) return;
    memcpy(pSendBuffer, pData, iSize);
    iSendBufferLength = iSize;
    if(iSize<=MAX_DATAGRAM) {
        iDataLen = iSize;
        eom = NET_FLAG_EOM;
    } else                    {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }
    iPacketLen = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy (PacketBuffer.pData, pSendBuffer, iDataLen);
    bCanSend = false;
    ret=nSend((char *)&PacketBuffer, iPacketLen, (struct sockaddr*)&ToAddr );
    if((ret==-1)||(ret!=(int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}

////////////////////////////////////////////////////////

void CCSocket::SendMessageNext() {
    unsigned int  iPacketLen,iDataLen,eom;
    int ret;
    if(iSendBufferLength<= MAX_DATAGRAM) {
        iDataLen = iSendBufferLength;
        eom = NET_FLAG_EOM;
    } else                                 {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }
    iPacketLen = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret=nSend( (char *)&PacketBuffer, iPacketLen, (struct sockaddr*)&ToAddr );
    if((ret==-1)||(ret!=(int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}

////////////////////////////////////////////////////////

void CCSocket::ReSendMessage(void) {
    unsigned int  iPacketLen,iDataLen,eom;
    iPacketLen=0;
    iDataLen=0;
    eom=0;
    int ret;
    if(iSendBufferLength<=MAX_DATAGRAM) {
        iDataLen = iSendBufferLength ;
        eom = NET_FLAG_EOM;
    } else                                {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }

    if(iDataLen > MAX_DATAGRAM) return;

    iPacketLen = NET_HEADERSIZE + iDataLen;

    PacketBuffer.iLen      = htonl(iPacketLen |(NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence - 1);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret=nSend((char *)&PacketBuffer, iPacketLen, (struct sockaddr*)&ToAddr);
    if((ret==-1)||(ret!=(int)iPacketLen)) return;
    dLastSendTime = dlcs_get_tickcount();
    iPacketsReSent++;
}

////////////////////////////////////////////////////////

bool CCSocket::bCanSendMessage (void) {
    if(bSendNext) SendMessageNext();
    if((!bCanSend)&&(dlcs_get_tickcount() - dLastMessage > dAliveTimeout)) return 0;
    return bCanSend;
}

//////////////////////////////////////////////////////////////

int CCSocket::SendUnreliableMessage(CPacket *pMessage) {
    if(!pMessage) return -4;
    return SendUnreliableMessage(pMessage->pGetPacketBuffer(),pMessage->iGetCurSize());
}

//////////////////////////////////////////////////////////////

int CCSocket::SendUnreliableMessage(char *pData,int iSize) {
    int iPacketLen;
    int ret;
    if(iSize<0) return -1;
    if(iSize>=MAX_DATAGRAM) return -2;
    if(!pData) return -3;
    iPacketLen = NET_HEADERSIZE + iSize;
    PacketBuffer.iLen     = htonl(iPacketLen |NET_FLAG_UNRELIABLE);
    PacketBuffer.iSequence= htonl(iUnreliableSendSequence++);
    memcpy (PacketBuffer.pData, pData, iSize);
    ret=nSend((char *)&PacketBuffer, iPacketLen, (struct sockaddr*)&ToAddr );
    if((ret==-1)||(ret!=iPacketLen)) {
        return ret;
    }
    iPacketsSent++;
}

////////////////////////////////////////////////////////

int CCSocket::iGetMessage() {

    unsigned int  iLength;
    unsigned int  iFlags;
    int       err = 0;

    unsigned int  iSequence;
    unsigned int  iCount;
    if((!bCanSend)&&((dlcs_get_tickcount() - dLastSendTime) > dAckTimeOut)) ReSendMessage();
    if(iSocket==-1) return 0;
    iLastLength = 0;
    while(1) {
        iLength = nRecv((char *)&PacketBuffer, NET_DATAGRAMSIZE, (struct sockaddr*)&FromAddr);

        if(iLength==-1) {
            printf("ya done goofed 000\n");
            return -1;
        }


        if(iLength==0)  break;
#ifdef _WIN32
        if(&FromAddr.sin_addr.s_addr != &ToAddr.sin_addr.s_addr)
#else

        if(&FromAddr.sin_addr.s_addr != &ToAddr.sin_addr.s_addr)
#endif

        {
            //Log
            //printf("CCSocket::iGetMessage() forged packet received from %s - supposed to be from %s",NET_pAddrToString(&Read_Addr),NET_pAddrToString(&reAddr));
            //printf("ya done goofed\n");
            //continue;
        }

        if(iLength<NET_HEADERSIZE) {
            iShortPacketCount++;
            continue;
        }
        iLength = ntohl(PacketBuffer.iLen);
        iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
        iLength &= NET_FLAG_LENGTH_MASK;
        if(iFlags&NET_FLAG_CTL) continue;
        if(iLength>NET_DATAGRAMSIZE) {
            //Log
            //printf("CCSocket::iGetMessage() forged header received %s",NET_pAddrToString(&Read_Addr.stAddr));
            printf("ya done goofed 2\n");
            iLongPacketCount++;
            continue;
        }


        // finally a valid message

        iSequence = ntohl(PacketBuffer.iSequence);
        iPacketsReceived++;
        dLastMessage = dlcs_get_tickcount();
        if(iFlags&NET_FLAG_UNRELIABLE) {
            if(iSequence<iUnreliableReceiveSequence) {
                //Log("CCSocket::iGetMessage() got a stale datagram");
                printf("ya done goofed 3\n");
                err =0;
                break;
            }
            if(iSequence!=iUnreliableReceiveSequence) {
                iCount = iSequence - iUnreliableReceiveSequence;
                iDroppedDatagrams += iCount;
                //Log("CCSocket::iGetMessage() dropped %u datagram(s)", iCount);
                printf("ya done goofed 4\n");
            }
            iUnreliableReceiveSequence = iSequence + 1;
            iLength -= NET_HEADERSIZE;
            iLastLength = iLength;
            err = 2;
            break;
        }

        if(iFlags&NET_FLAG_ACK) {
            if(iSequence!=(iSendSequence-1)) {
                //Log("CCSocket::iGetMessage() stale ACK received %s",NET_pAddrToString(&ReadAddr.stAddr));
                printf("ya done goofed 5\n");
                continue;
            }
            iSendBufferLength -= MAX_DATAGRAM;
            if(iSendBufferLength>0) {
                memmove(pSendBuffer, pSendBuffer+MAX_DATAGRAM, iSendBufferLength);
                bSendNext=true;
            } else {
                iSendBufferLength=0;
                bCanSend=true;
            }
            continue;
        }

        if(iFlags&NET_FLAG_RELIABLE) {
            PacketBuffer.iLen =      ntohl(NET_HEADERSIZE | NET_FLAG_ACK);
            PacketBuffer.iSequence = ntohl(iSequence);
            if(nSend((char *)&PacketBuffer, NET_HEADERSIZE, (struct sockaddr*)&ToAddr)==-1) {
                printf("ya done goofed 7\n");
                return -1;
            }
            if(iSequence != iReceiveSequence) {
                iReceivedDuplicateCount++;
                continue;
            }
            iReceiveSequence++;
            iLength -= NET_HEADERSIZE;
            if(iReceiveBufferLength + (int)iLength < iCurReceiveSize) {
                if((iReceiveBufferLength + iLength < NET_MINMESSAGE)&&(iCurReceiveSize != NET_MINMESSAGE)) {
                    pReceiveBuffer = (char *)realloc((void *)pReceiveBuffer,NET_MINMESSAGE);
                    iCurReceiveSize = NET_MINMESSAGE;
                }
            } else {
                pReceiveBuffer = (char *)realloc((void *)pReceiveBuffer,iReceiveBufferLength + iLength);
                iCurReceiveSize = iReceiveBufferLength + iLength;
            }
            if(iFlags&NET_FLAG_EOM) {
                memcpy(pReceiveBuffer + iReceiveBufferLength, PacketBuffer.pData, iLength);
                iReceivedMessageLength = iReceiveBufferLength+iLength;
                iReceiveBufferLength = 0;
                err = 1;
                break;
            }
            memcpy(pReceiveBuffer + iReceiveBufferLength, PacketBuffer.pData, iLength);
            iReceiveBufferLength += iLength;
            continue;
        }
    }
    if(bSendNext) SendMessageNext();
    return err;
}

////////////////////////////////////////////////////////

char *CCSocket::pcGetMessage () {
    assert(pReceiveBuffer != NULL);
    if(iLastLength) return PacketBuffer.pData;
    else            return pReceiveBuffer;
}

////////////////////////////////////////////////////////

int CCSocket::iGetMessageSize() {
    if(iLastLength) return iLastLength;
    else            return iReceivedMessageLength;
}

////////////////////////////////////////////////////////

CPacket *CCSocket::pGetMessage() {
    if(!pTempPacket) {
        pTempPacket = new CPacket(iGetMessageSize(),pcGetMessage());
        if(!pTempPacket) return 0;
    } else {
        pTempPacket->Reset();
        pTempPacket->SetMaxSize(iGetMessageSize(),pcGetMessage());
        pTempPacket->SetCurSize(iGetMessageSize());
    }
    return pTempPacket;
}
////////////////////////////////////////////////////////

/*
CCSocket *CCSocket::pAccept(int iReSocket,struct sockaddr *ReAddr)
{
    CPacket Send(NET_DATAGRAMSIZE);
    struct sockaddr NewAddr;

    iSocket = OpenSocket(0);
    if(iSocket==-1)
    {        //Log("CCSocket::pAccept() unable to connect to host");
            return 0;
    }

    NET_GetSocketAddr(iSocket,&NewAddr); // here is where it gets port info

    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_ACCEPT);
    Send.Write(iGetRemotePort()); //SocketPort(&NewAddr)); // here is where it sends port back to client
    FinishCtlPacket(&Send);

    if(//NET_Write(iReSocket,
    nSend(Send.pGetPacketBuffer(),Send.iGetCurSize(),ReAddr)==-1)
    {
        //Log("CCSocket::pAccept() error making a new connection");
        return 0;
    }
    bCanSend = true;
    bConnected = true;
    return this;
}*/

////////////////////////////////////////////////////////

void CCSocket::Disconnect() {
    CloseSocket(iSocket);
}

////////////////////////////////////////////////////////

//void CCSocket::SetAddress(CInetAddress *pAddress){    pAddr = pAddress;}

////////////////////////////////////////////////////////

double CCSocket::dReceiveStatus() {
    return 0;
}

////////////////////////////////////////////////////////

double CCSocket::dSendStatus() {
    return 0;
}

////////////////////////////////////////////////////////

// CInetAddress *CCSocket::pGetAddress(void){    return pAddr; }

////////////////////////////////////////////////////////

void CCSocket::FinishCtlPacket(CPacket *pPacket) {
    ::FinishCtlPacket(pPacket);
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::Listen(int iHostPort, int iState) {
    if(iState) {
        if(iSocket != INVALID_SOCKET) {
            return 0;
        }
        if((iSocket = OpenSocket(iHostPort)) == INVALID_SOCKET)
            return -1;
    } else {
        if(iSocket == INVALID_SOCKET)
            return 0;

        CloseSocket(iSocket);

        iSocket=INVALID_SOCKET;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////

int CCSocket::OpenSocket(int iPort) {
    return OpenSocket("127.0.0.1",iPort);
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::OpenSocket(char *pAddress, int iPort) {
    ToAddr.sin_family = AF_INET;
    ToAddr.sin_port=htons((short)iPort);
#ifdef _WIN32
    ToAddr.sin_addr.s_addr=inet_addr(pAddress);
#else
    inet_pton(AF_INET, pAddress, &(ToAddr.sin_addr));
#endif

    iSocket=-1;
    unsigned long _true = 1;

    if((iSocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
        return -1;

#ifdef _WIN32
    if(ioctlsocket(iSocket, FIONBIO, &_true) == SOCKET_ERROR)
#else
    bzero(&pAddress, sizeof(pAddress)); // linux func
    if(fcntl(iSocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
        CloseSocket(iSocket);
        return SOCKET_ERROR;
    }

#ifdef _WIN32
    if(bind(iSocket,(struct sockaddr *)& ToAddr, sizeof(ToAddr))==SOCKET_ERROR) {

        CloseSocket(iSocket);
        return SOCKET_ERROR;
    }

#else


    bind(iSocket, (struct sockaddr*)&ToAddr, sizeof ToAddr);

#endif
    return iSocket;
}

//////////////////////////////////////////////////////////////////////////////////
int CCSocket::zOpenSocket(int iPort) {
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
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((short)iPort);

    ToAddr.sin_family = AF_INET;
    // reAddr.sin_addr.s_addr = INADDR_ANY;
    ToAddr.sin_port = htons((short)iPort);

#ifdef _WIN32
    if(bind(newsocket,(struct sockaddr *)&address, sizeof(address))==SOCKET_ERROR) {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
#else
    bind(newsocket,(struct sockaddr *)&address, sizeof(address));

#endif

    return newsocket;
}



//////////////////////////////////////////////////////////////////////////////////
int CCSocket::zOpenSocket(char *pAddress, int iPort) {
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
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(pAddress);//INADDR_ANY;
    address.sin_port = htons((short)iPort);

    ToAddr.sin_family = AF_INET;
    ToAddr.sin_addr.s_addr = inet_addr(pAddress);//INADDR_ANY;
    ToAddr.sin_port = htons((short)iPort);

#ifdef _WIN32
    if(bind(newsocket,(struct sockaddr *)&address, sizeof(address))==SOCKET_ERROR) {
        CloseSocket(newsocket);
        return SOCKET_ERROR;
    }
#else
    bind(newsocket,(struct sockaddr *)&address, sizeof(address));

#endif
    return newsocket;
}


//////////////////////////////////////////////////////////////////////////////////

int CCSocket::CloseSocket(int iSocket) {
#ifdef _WIN32
    return closesocket(iSocket);
#else
    return close(iSocket);
#endif
}

//////////////////////////////////////////////////////////////////////////////////

int  CCSocket::CheckNewConnections (void) {
    char pBuffer[65535];
    if(iSocket == INVALID_SOCKET) return -1;
    if(recvfrom(iSocket, pBuffer, sizeof(pBuffer), MSG_PEEK, NULL, NULL) > 0) return iSocket;
    return -1;
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::nRecv(char *pBuf, int iLen, struct sockaddr *pAddr) {
    return nRecv(iSocket, pBuf, iLen, pAddr);
}

//////////////////////////////////////////////////////////////////////////////////

int  CCSocket::nRecv(SOCKET iSocket, char *pBuff, int iLen, struct sockaddr *pAddr) {
    int iAddrlen = sizeof (struct sockaddr);
    int err,errnum;

#ifndef _WIN32

    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(iSocket,&readfds);
    tv.tv_sec=0;
    tv.tv_usec=5;
    if(select(iSocket+1, &readfds, NULL, NULL, &tv)==-1) {
        return 0;
    }
    err=0;

    if(FD_ISSET(iSocket, &readfds) > 0) {
        err = recvfrom(	iSocket,
                        pBuff,
                        iLen,
                        0,
                        (struct sockaddr *)pAddr,
                        (socklen_t *)&iAddrlen);
    }

#endif

#ifdef _WIN32

    err = recvfrom( iSocket,
                    pBuff,
                    iLen,
                    0,
                    (struct sockaddr *)pAddr,
                    &iAddrlen);

    if (err == SOCKET_ERROR) {
        errnum=WSAGetLastError();
        if(errnum == WSAEWOULDBLOCK || errnum == WSAECONNREFUSED) {
            return 0;
        }
    }

#endif
    //if(err==-1) { Log("%s on recvfrom()",NET_pGetLastError()); }
    return err;
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::nSend(char *pBuf, int iLen, struct sockaddr *pAddr) {
    return nSend(iSocket,pBuf,iLen,pAddr);
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::nSend(SOCKET iSocket, char *pBuf, int iLen, struct sockaddr *pAddr) {
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

char *NET_pAddrToString(struct sockaddr *pAddr) {
    static char buffer[22];
    if(pAddr==NULL) strcpy(buffer,"error!");
    else {
        int taddr=((struct sockaddr_in *)pAddr)->sin_addr.s_addr;
//		Log("%d.%d.%d.%d:%d",(taddr>>24)&0xff,(taddr >> 16) & 0xff, (taddr >> 8) & 0xff, taddr & 0xff, ntohs(((struct sockaddr_in *)pAddr)->sin_port));
        int haddr=ntohl(((struct sockaddr_in *)pAddr)->sin_addr.s_addr);
        sprintf(buffer, "%d.%d.%d.%d:%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff, ntohs(((struct sockaddr_in *)pAddr)->sin_port));
//		Log(buffer);
    }
    return buffer;
}

//////////////////////////////////////////////////////////////////////////////////

/*
int  CCSocket::NET_StringToAddr(char *pString, struct sockaddr *pAddr)
{
 	int ha1,ha2,ha3,ha4,hp,ipaddr;
	sscanf(pString, "%d.%d.%d.%d:%d", &ha1, &ha2, &ha3, &ha4, &hp);
	ipaddr=(ha1<<24)|(ha2<<16)|(ha3<<8)|ha4;
	pAddr->sa_family=AF_INET;
	((struct sockaddr_in *)pAddr)->sin_addr.s_addr=htonl(ipaddr);
	((struct sockaddr_in *)pAddr)->sin_port=hp;
    return 0;
}*/

//////////////////////////////////////////////////////////////////////////////////

int  CCSocket::NET_GetSocketAddr (int iSocket, struct sockaddr *pAddr) {
    int addrlen = sizeof(struct sockaddr);
    unsigned int a;
    memset(pAddr, 0, sizeof(struct sockaddr));
#ifdef _WIN32
    getsockname(iSocket, (struct sockaddr *)pAddr, &addrlen);
#else
    getsockname(iSocket, (struct sockaddr *)pAddr, (socklen_t *)&addrlen);
#endif
    a = ((struct sockaddr_in *)pAddr)->sin_addr.s_addr;

    //if (a == 0 || a == inet_addr("127.0.0.1"))
    //((struct sockaddr_in *)pAddr)->sin_addr.s_addr = myAddr;

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void CCSocket::SetRemotePort(int iPort) {

    if(iPort > 0xffff) return; //{ Log("NET_SetPort tried to set a port higher than 0xffff"); return; }
    if(iPort < 1024)   return; //{ Log("NET_SetPort tried to set a port lower than 1024");    return; }

    ToAddr.sin_port=htons((short)iPort);

    //  setsockopt(iSocket);
//    address.sin_port = htons((short)iPort);
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::iGetLocalPort(void) {
    struct sockaddr_in local_address;
    int addr_size = sizeof(local_address);
    getsockname(iSocket, (struct sockaddr *)&local_address, &addr_size);
    return ntohs(local_address.sin_port);
}

//////////////////////////////////////////////////////////////////////////////////

int CCSocket::iGetRemotePort(void) {
    return ntohs(ToAddr.sin_port);
}

//////////////////////////////////////////////////////////////////////////////////

char *CCSocket::pGetRemoteIPAddress() {
    return inet_ntoa(ToAddr.sin_addr);
}

char *CCSocket::pGetLocalIPAddress() {
    struct sockaddr_in local_address;
    int addr_size = sizeof(local_address);
    getsockname(iSocket, (struct sockaddr *)&local_address, &addr_size);
    return inet_ntoa(local_address.sin_addr);
}

////////////////////////////////////////////////////////////////////////

CPacket::CPacket() {
    pPacketBuffer = NULL;
    iPacketSize = 0;
    iPacketCursor = 0;
    iPacketLen = 0;
    bUserAlloc = false;
}

////////////////////////////////////////////////////////////////////////

CPacket::CPacket(int iSize) {
    if(iSize<0) return;
    pPacketBuffer = (char *)malloc(iSize);
    if (pPacketBuffer == NULL)
        return;
    iPacketSize = iSize;
    iPacketCursor = 0;
    iPacketLen = 0;
    bUserAlloc = false;
}

////////////////////////////////////////////////////////////////////////

CPacket::CPacket(int iSize,char *pBuffer) {
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

CPacket::~CPacket() {
    if((!bUserAlloc)&&(pPacketBuffer!=NULL))
        free(pPacketBuffer);
}

////////////////////////////////////////////////////////////////////////

void CPacket::DumpPacket(void) {

    printf("-=[Packet Info]=========================================================-");


    for(int y=0; y<iPacketLen; y++) {
        if(pPacketBuffer[y]<32) {
            //LogC("{%d}",pPacketBuffer[y]);
        } else {}
        //LogC("%c",pPacketBuffer[y]);
    }
    //LogC("\n\r");
    printf("-==========================================================[End Packet]=-");
}

int CPacket::iGetMaxSize(void) {
    return iPacketSize;
}

////////////////////////////////////////////////////////////////////////

void CPacket::SetMaxSize(int iSize,char *pBuffer) {
    if(iSize<0)
        return;
    if((bUserAlloc)&&(pBuffer)) {
        pPacketBuffer=pBuffer;
        iPacketSize=iSize;
        return ;
    }

    iPacketSize=iSize;
    if(pPacketBuffer==NULL) {
        pPacketBuffer=(char *)malloc(iSize);
        if(!pPacketBuffer)
            return;
    }
    pPacketBuffer=(char *)realloc(pPacketBuffer,iSize);
    if (pPacketBuffer==NULL)
        return;
}

////////////////////////////////////////////////////////////////////////

int CPacket::iGetCurSize(void) {
    return iPacketLen-iPacketCursor;
}

////////////////////////////////////////////////////////////////////////

void CPacket::SetCurSize(int iNewLen) {
    iPacketLen=iNewLen;
}


////////////////////////////////////////////////////////////////////////

void CPacket::Reset(void) {
    iPacketCursor=0;
    iPacketLen=0;
}
////////////////////////////////////////////////////////////////////////

void CPacket::Rewind(void) {
    iPacketCursor=0;
}

///////////////////////////////////////////////////////////////////////
char *CPacket::pGetPacketBuffer(void) {
    return pPacketBuffer;
}

////////////////////////////////////////////////////////////////////////

#define WRITE(Type) if (pPacketBuffer==NULL) return; if (iPacketLen + (int)sizeof(Type) > iPacketSize) return; *((Type *)(pPacketBuffer+iPacketLen)) = Val; iPacketLen += sizeof(Type);

void CPacket::Write(int   Val) {
    WRITE(int)
}
void CPacket::Write(long  Val) {
    WRITE(long)
}
void CPacket::Write(char  Val) {
    WRITE(char)
}
void CPacket::Write(short Val) {
    WRITE(short)
}
void CPacket::Write(float Val) {
    WRITE(float)
}

void CPacket::Write(char *Val) {
    int i = strlen(Val)+1;
    if(pPacketBuffer==NULL) return;
    if(iPacketLen + i > iPacketSize) return;
    strcpy(pPacketBuffer+iPacketLen,Val);
    iPacketLen+=i;
}

void CPacket::Write(char *Val,int iSize) {
    if(!iSize) return;
    if(pPacketBuffer==NULL) return;
    if(iPacketLen + iSize > iPacketSize) return;
    memcpy(pPacketBuffer+iPacketLen,Val,iSize);
    iPacketLen+=iSize;
}

char *CPacket::pWrite(int iSize) {
    if(!iSize) return NULL;
    if(!pPacketBuffer) return NULL;
    if(iPacketLen + iSize > iPacketSize) return NULL;
    iPacketLen+=iSize;
    return pPacketBuffer+iPacketLen-iSize;
}

////////////////////////////////////////////////////////////////////////

#define READ(Val) if(iPacketLen<1) return 0; if (pPacketBuffer==NULL) return 0; iPacketCursor += sizeof(Val); if (iPacketCursor > iPacketLen) return 0; return *((Val *)(pPacketBuffer+iPacketCursor-sizeof(Val)));

int     CPacket::iRead(void) {
    READ(int)
}
long    CPacket::dwRead(void) {
    READ(long)
}
char    CPacket::cRead(void) {
    READ(char)
}
short   CPacket::sRead(void) {
    READ(short)
}
float   CPacket::fRead(void) {
    READ(float)
}

char   *CPacket::pRead(void) {
    int i;
    if(iPacketLen<1) return ("null");
    if(!pPacketBuffer) return ("null");
    i=iPacketCursor;
    iPacketCursor+=strlen(pPacketBuffer+iPacketCursor)+1;
    if(iPacketCursor > iPacketLen) return ("null");
    return (char *) (pPacketBuffer+i);
}

char *CPacket::pRead(int iSize) {
    int i;
    if(iSize==0) return ("null");
    if(iPacketLen<1) return ("null");
    if(pPacketBuffer==NULL) return ("null");
    i=iPacketCursor;
    iPacketCursor+=iSize;
    if(iPacketCursor > iPacketLen) return ("null");
    return (char *) (pPacketBuffer+i);
}



//////////////////////////////////////////////////////////////////////////////////

int NET_Init() {
#ifdef _WIN32
    int err;
    WSADATA winsockdata;
    err=WSAStartup(MAKEWORD(1,1),&winsockdata);
    if (err != 0) {
        return -1;
    }
#endif
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int NET_Shutdown (void) {
    //NET_Listen(0);
#ifdef _WIN32
    if(WSACleanup ()!=0) return -1;
#endif
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

char *NET_pGetLastError(void) {
    int i,err;
#ifdef _WIN32
    err = WSAGetLastError();
    for (i = 0; i<iNumMessages; i++) {
        if (pErrorList[i].iID == err) {
            return (char *)pErrorList[i].pMessage;
        }
    }
    return (char *)pErrorList[0].pMessage;
#else
    return(strerror(errno));
#endif
}

//////////////////////////////////////////////////////////////////////////////////

void FinishCtlPacket(CPacket *pPacket) {
    int j=(NET_FLAG_CTL|(pPacket->iGetCurSize() & NET_FLAG_LENGTH_MASK));
    *((int *)pPacket->pGetPacketBuffer()) = htonl(j);
}

//////////////////////////////////////////////////////////////////////////////////

int  NET_GetNameFromAddr (struct sockaddr *pAddr, char *pName) {
    struct hostent *hostentry;
    hostentry = gethostbyaddr ((char *)&((struct sockaddr_in *)pAddr)->sin_addr, sizeof(struct in_addr), AF_INET);
    if (hostentry) {
        strncpy (pName, (char *)hostentry->h_name, NET_NAMELEN - 1);
        return 0;
    }
    strcpy (pName, NET_pAddrToString (pAddr));
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int  NET_GetAddrFromName(char *pName, struct sockaddr *pAddr) {
    struct hostent *hostentry;
    //if (pName[0] >= '0' && pName[0] <= '9') return PartialIPAddress (pName, pAddr);
    hostentry=gethostbyname(pName);
    if(!hostentry) return SOCKET_ERROR;
    pAddr->sa_family=AF_INET;
//    ((struct sockaddr_in *)pAddr)->sin_port=iHostPort;
    ((struct sockaddr_in *)pAddr)->sin_addr.s_addr=*(int *)hostentry->h_addr_list[0];
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int  NET_AddrCompare (struct sockaddr *pAddr1, struct sockaddr *pAddr2) {
    if (pAddr1->sa_family != pAddr2->sa_family) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_addr.s_addr != ((struct sockaddr_in *)pAddr2)->sin_addr.s_addr) return -1;
    if (((struct sockaddr_in *)pAddr1)->sin_port != ((struct sockaddr_in *)pAddr2)->sin_port) return 1;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
