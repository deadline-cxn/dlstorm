/***************************************************************
 **      EMBER                                                **
 ***************************************************************/


#include "c_socket.h"

////////////////////////////////////////////////////////

CCSocket::CCSocket()
{
    iSocket = -1;
 //   pAddr = 0;
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
    if(!pSendBuffer)
    {
        //Log("CCSocket::CCSocket() can't initialize pSendBuffer");
        return;
    }
    pReceiveBuffer = (char *)malloc(iCurReceiveSize);
    if(!pReceiveBuffer)
    {
        FREE(pSendBuffer);
        //Log("CCSocket::CCSocket() can't initialize pReceiveBuffer");
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
}
////////////////////////////////////////////////////////
CCSocket::~CCSocket()
{
    FREE(pSendBuffer);
    FREE(pReceiveBuffer);
//    DEL(pAddr);
}
//////////////////////////////////////////////////////////////
void CCSocket::SendReliableMessage(CPacket *pMessage)
{
    if(!pMessage)
	{
		//Log("CCSocket::SendReliableMessage() pMessage is null!");
		return;
	}
    SendReliableMessage(
			pMessage->pGetPacketBuffer(),
			pMessage->iGetCurSize()
			);
}
//////////////////////////////////////////////////////////////
void CCSocket::SendReliableMessage (char *pData, int iSize)
{
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int  eom;
    int ret;
    if(iSize < 0)
    {
        //Log("CCSocket::SendReliableMessage() packet size is not compliant.");
        return;
    }
    if(!bCanSend)
    {
        //Log("CCSocket::SendReliableMessage() bCanSend is false.");
        return;
    }
    if(!pSendBuffer)
    {
        //Log("CCSocket::SendReliableMessage() pSendBuffer is null (1)");
        return;
    }
    if(!pData)
    {
        //Log("CCSocket::SendReliableMessage() pData is null.");
        return;
    }
    if(iCurBufSize>=iSize)  // should we resize the buffer?
    {
        if((iSize<NET_MINMESSAGE)&&(iCurBufSize!=NET_MINMESSAGE))
        {
            pSendBuffer = (char *)realloc((void *)pSendBuffer,NET_MINMESSAGE);
            iCurBufSize = NET_MINMESSAGE;
        }
    }
    else { pSendBuffer = (char *)realloc((void *)pSendBuffer,iSize); iCurBufSize = iSize; }
    if(!pSendBuffer)
    {
        //Log("CCSocket::SendReliableMessage() pSendBuffer is null (2)");
        return;
    }
    memcpy(pSendBuffer, pData, iSize);
    iSendBufferLength = iSize;
    if(iSize<=MAX_DATAGRAM) { iDataLen = iSize;         eom = NET_FLAG_EOM; }
    else                    { iDataLen = MAX_DATAGRAM;  eom = 0;            }
    iPacketLen = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy (PacketBuffer.pData, pSendBuffer, iDataLen);
    bCanSend = false;

	ret=NET_Write(iSocket, (char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );

    if((ret==-1)||(ret!=(int)iPacketLen))
    {
        //Log("CCSocket::SendReliableMessage() error writing to network");
        return;
    }
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}
////////////////////////////////////////////////////////
void CCSocket::SendMessageNext ()
{
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int eom;
    int ret;
    if(iSendBufferLength<= MAX_DATAGRAM) { iDataLen = iSendBufferLength; eom = NET_FLAG_EOM; }
    else                                 { iDataLen = MAX_DATAGRAM;      eom = 0;            }
    iPacketLen = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret=NET_Write ( iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if((ret==-1)||(ret!=(int)iPacketLen))
    {
        //Log("CCSocket::SendMessageNext() error writing to network");
        return;
    }
    dLastSendTime = dlcs_get_tickcount();
    iPacketsSent++;
}
////////////////////////////////////////////////////////
void CCSocket::ReSendMessage(void)
{
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int  eom;
    int ret;
    if(iSendBufferLength<=MAX_DATAGRAM)
    {
        iDataLen = iSendBufferLength ;
        eom = NET_FLAG_EOM;
    }
    else
    {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }
    iPacketLen = NET_HEADERSIZE + iDataLen;
    PacketBuffer.iLen      = htonl(iPacketLen |(NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence - 1);
    memcpy (PacketBuffer.pData, pSendBuffer, iDataLen);
    bSendNext = false;
    ret=NET_Write(iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr);
    if((ret==-1)||(ret!=(int)iPacketLen))
    {
        //Log("CCSocket::ReSendMessage() error writing to network");
        return;
    }
    dLastSendTime = dlcs_get_tickcount();
    iPacketsReSent++;
}
////////////////////////////////////////////////////////
bool CCSocket::bCanSendMessage (void)
{
    if(bSendNext) SendMessageNext();
    if((!bCanSend)&&(dlcs_get_tickcount() - dLastMessage > dAliveTimeout)) return 0;
    return bCanSend;
}
//////////////////////////////////////////////////////////////
void CCSocket::SendUnreliableMessage(CPacket *pMessage)
{
    if(!pMessage) return;
    SendUnreliableMessage(pMessage->pGetPacketBuffer(),pMessage->iGetCurSize());
}
//////////////////////////////////////////////////////////////
void CCSocket::SendUnreliableMessage(char *pData,int iSize)
{
    int iPacketLen;
    int ret;
    if(iSize<0) return;
    if(iSize>=MAX_DATAGRAM) return;
    if(!pData) return;
    iPacketLen = NET_HEADERSIZE + iSize;
    PacketBuffer.iLen     = htonl(iPacketLen |NET_FLAG_UNRELIABLE);
    PacketBuffer.iSequence= htonl(iUnreliableSendSequence++);
    memcpy (PacketBuffer.pData, pData, iSize);
    ret=NET_Write(iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if((ret==-1)||(ret!=iPacketLen))
    {
        //Log("CCSocket::SendUnreliableMessage() error writing to network");
        return;
    }
    iPacketsSent++;
}
////////////////////////////////////////////////////////
int CCSocket::iGetMessage ()
{
    unsigned int  iLength;
    unsigned int  iFlags;
    int       err = 0;
    CInetAddress ReadAddr;
    unsigned int  iSequence;
    unsigned int  iCount;
    if((!bCanSend)&&((dlcs_get_tickcount() - dLastSendTime) > dAckTimeOut)) ReSendMessage();
    if(iSocket==-1) return 0;
    iLastLength = 0;
    while(1)
    {
        iLength = NET_Read(iSocket,(char *)&PacketBuffer, NET_DATAGRAMSIZE, &ReadAddr.stAddr);

        if(iLength==-1) return -1;
        if(iLength==0)  break;
        if(ReadAddr!=*pAddr)
        {
            //Log("CCSocket::iGetMessage() forged packet received from %s - supposed to be from %s",NET_pAddrToString(&ReadAddr.stAddr),NET_pAddrToString(&pAddr->stAddr));
            continue;
        }
        if(iLength<NET_HEADERSIZE) { iShortPacketCount++; continue; }
        iLength = ntohl(PacketBuffer.iLen);
        iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
        iLength &= NET_FLAG_LENGTH_MASK;
        if(iFlags&NET_FLAG_CTL) continue;
        if(iLength>NET_DATAGRAMSIZE)
        {
            //Log("CCSocket::iGetMessage() forged header received %s",NET_pAddrToString(&ReadAddr.stAddr));
            iLongPacketCount++;
            continue;
        }
        // finally a valid message
        iSequence = ntohl(PacketBuffer.iSequence);
        iPacketsReceived++;
        dLastMessage = dlcs_get_tickcount();
        if(iFlags&NET_FLAG_UNRELIABLE)
        {
            if(iSequence<iUnreliableReceiveSequence)
            {
                //Log("CCSocket::iGetMessage() got a stale datagram");
                err =0;
                break;
            }
            if(iSequence!=iUnreliableReceiveSequence)
            {
                iCount = iSequence - iUnreliableReceiveSequence;
                iDroppedDatagrams += iCount;
                //Log("CCSocket::iGetMessage() dropped %u datagram(s)", iCount);
            }
            iUnreliableReceiveSequence = iSequence + 1;
            iLength -= NET_HEADERSIZE;
            iLastLength = iLength;
            err = 2;
            break;
        }
        if(iFlags&NET_FLAG_ACK)
        {
            if(iSequence!=(iSendSequence-1))
            {
                //Log("CCSocket::iGetMessage() stale ACK received %s",NET_pAddrToString(&ReadAddr.stAddr));
                continue;
            }
            iSendBufferLength -= MAX_DATAGRAM;
            if(iSendBufferLength>0)
            {
                memmove(pSendBuffer, pSendBuffer+MAX_DATAGRAM, iSendBufferLength);
                bSendNext=true;
            }
            else
            {
                iSendBufferLength=0;
                bCanSend=true;
            }
            continue;
        }
        if(iFlags&NET_FLAG_RELIABLE)
        {
            PacketBuffer.iLen =      ntohl(NET_HEADERSIZE | NET_FLAG_ACK);
            PacketBuffer.iSequence = ntohl(iSequence);
            if(NET_Write(iSocket,(char *)&PacketBuffer, NET_HEADERSIZE, &ReadAddr.stAddr)==-1) return -1;
            if(iSequence != iReceiveSequence) { iReceivedDuplicateCount++; continue; }
            iReceiveSequence++;
            iLength -= NET_HEADERSIZE;
            if(iReceiveBufferLength + (int)iLength < iCurReceiveSize)
            {
                if((iReceiveBufferLength + iLength < NET_MINMESSAGE)&&(iCurReceiveSize != NET_MINMESSAGE))
                {
                    pReceiveBuffer = (char *)realloc((void *)pReceiveBuffer,NET_MINMESSAGE);
                    iCurReceiveSize = NET_MINMESSAGE;
                }
            }
            else
            {
                pReceiveBuffer = (char *)realloc((void *)pReceiveBuffer,iReceiveBufferLength + iLength);
                iCurReceiveSize = iReceiveBufferLength + iLength;
            }
            if(iFlags&NET_FLAG_EOM)
            {
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

char *CCSocket::pcGetMessage ()
{
    assert(pReceiveBuffer != NULL);
    if(iLastLength) return PacketBuffer.pData;
    else            return pReceiveBuffer;
}
////////////////////////////////////////////////////////
int CCSocket::iGetMessageSize()
{
      if(iLastLength) return iLastLength;
      else            return iReceivedMessageLength;
}
////////////////////////////////////////////////////////
CPacket *CCSocket::pGetMessage()
{
      if(!pTempPacket)
      {
          pTempPacket = new CPacket(iGetMessageSize(),pcGetMessage());
            if(!pTempPacket) return 0;
      }
      else
      {
          pTempPacket->Reset();
          pTempPacket->SetMaxSize(iGetMessageSize(),pcGetMessage());
          pTempPacket->SetCurSize(iGetMessageSize());
      }
      return pTempPacket;
}
////////////////////////////////////////////////////////
CCSocket *CCSocket::pAccept(int iReSocket,struct sockaddr *ReAddr)
{
    CPacket Send(NET_DATAGRAMSIZE);
    struct sockaddr NewAddr;
    iSocket = NET_OpenSocket(0);
    if(iSocket==-1)
    {
        //Log("CCSocket::pAccept() unable to connect to host");
        return 0;
    }
    NET_GetSocketAddr(iSocket,&NewAddr); // here is where it gets port info
    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_ACCEPT);
    Send.Write(NET_GetSocketPort(&NewAddr)); // here is where it sends port back to client
    FinishCtlPacket(&Send);
    if(NET_Write(iReSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),ReAddr)==-1)
    {
        //Log("CCSocket::pAccept() error making a new connection");
        return 0;
    }
    bCanSend = true;
    bConnected = true;
    return this;
}
////////////////////////////////////////////////////////
CCSocket *CCSocket::pAccept(void) { return NULL; }
////////////////////////////////////////////////////////
void CCSocket::Disconnect() { NET_CloseSocket(iSocket); }
////////////////////////////////////////////////////////
void CCSocket::SetAddress(CInetAddress *pAddress) { pAddr = pAddress; }
////////////////////////////////////////////////////////
double CCSocket::dReceiveStatus() { return 0; }
////////////////////////////////////////////////////////
double CCSocket::dSendStatus() { return 0; }
////////////////////////////////////////////////////////
CInetAddress *CCSocket::pGetAddress(void) { return pAddr; }
////////////////////////////////////////////////////////
void CCSocket::FinishCtlPacket(CPacket *pPacket) { ::FinishCtlPacket(pPacket); }


