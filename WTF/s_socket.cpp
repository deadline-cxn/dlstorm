/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "s_socket.h"
#include "fm_util.h"
//#include "b_netaddress.h"

////////////////////////////////////////////////////////

CSocket::CSocket()
{
    iSocket = -1;
    pAddr = 0;

    iLastLength = 0;
    pTempPacket = NULL;

    bCanSend = false;
    bSendNext = false;

    iCurrentBuffer = 0;
    iSendBufferLength=0;
    iReceiveBufferLength=0; 
    iReceivedMessageLength=0;

    dLastSendTime = 0;

    iSendSequence = 0;
    iUnreliableSendSequence = 0;
    iUnreliableReceiveSequence =0;
    iReceiveSequence=0;
    iBufferedSendSequence = 0;
    iBufferedReceivedSequence = 0;

    bExtractBuffer = false;

    iCurReceiveSize = iCurBufSize = NET_MINMESSAGE;

    pSendBuffer = (char *)malloc(iCurBufSize);
    if(!pSendBuffer)
    {
        Log("FATAL: s_socket.cpp CSocket::CSocket() can't initialize pSendBuffer");
        return;
    }
    
    pReceiveBuffer = (char *)malloc(iCurReceiveSize);
    if(!pReceiveBuffer)
    {
        FREE(pSendBuffer);
        Log("FATAL: s_socket.cpp CSocket::CSocket() can't initialize pReceiveBuffer 1");
        return;
    }

    pBackupBuffer = (char *)malloc((size_t)dBufferedSize);
    if (!pBackupBuffer)
    {
        FREE(pSendBuffer);
        FREE(pReceiveBuffer);
        Log("FATAL: s_socket.cpp CSocket::CSocket() can't initialize pRecieveBuffer 2");
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

CSocket::~CSocket()
{
    DEL(pTempPacket);
    FREE(pSendBuffer);
    FREE(pReceiveBuffer);
    FREE(pAddr);
    FREE(pBackupBuffer);
}

//////////////////////////////////////////////////////////////

void CSocket::SendReliableMessage(CPacket *pMessage)
{
    if(!pMessage)
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() pMessage is null!");
        return;
    }
    SendReliableMessage(pMessage->pGetPacketBuffer(),pMessage->iGetCurSize());
}

//////////////////////////////////////////////////////////////

void CSocket::SendReliableMessage(char *pData, int iSize)
{ 
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int  eom;
    int ret;

    if(iSize < 0)
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() packet size is not compliant.");
        return;
    }

    if(bCanSend==false)
    {
        Log("What the ");
        return;
    }

    if(!pSendBuffer)
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() pSendBuffer is null.");
        return;
    }

    if(!pData)
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() pData is null.");
        return;
    }

    if (iCurBufSize >= iSize)  // should we resize the buffer?
    {
        if ((iSize < NET_MINMESSAGE)&&(iCurBufSize != NET_MINMESSAGE))
        {
            pSendBuffer = (char *)realloc((void *)pSendBuffer,NET_MINMESSAGE);
            iCurBufSize = NET_MINMESSAGE;
        }
    }

    else   // buffer is too small make it bigger
    {
        pSendBuffer = (char *)realloc((void *)pSendBuffer,iSize);
        iCurBufSize = iSize;
    }

    if (pSendBuffer==NULL)
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() pSendBuffer is null. 2");
        return;
    }

    memcpy(pSendBuffer, pData, iSize);
    iSendBufferLength = iSize;

    if (iSize <= MAX_DATAGRAM)
    {
        iDataLen = iSize;
        eom = NET_FLAG_EOM;
    }
    else
    {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }
    iPacketLen = NET_HEADERSIZE + iDataLen;

    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy (PacketBuffer.pData, pSendBuffer, iDataLen);

    bCanSend = false;

    ret = NET_Write(iSocket, (char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if ((ret == -1)||(ret != (int)iPacketLen))
    {
        Log("s_socket.cpp CSocket::SendReliableMessage() error writing to network");
        return;
    }
    
    dLastSendTime = GetTickCount();
    iPacketsSent++;
    Log("packets sent[%d]",iPacketsSent);
}

////////////////////////////////////////////////////////

void CSocket::SendMessageNext ()
{
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int eom;
    int ret;

    if(iSendBufferLength <= MAX_DATAGRAM)
    {
        iDataLen = iSendBufferLength;
        eom = NET_FLAG_EOM;
    }
    else
    {
        iDataLen = MAX_DATAGRAM;
        eom = 0;
    }

    iPacketLen = NET_HEADERSIZE + iDataLen;

    PacketBuffer.iLen =      htonl(iPacketLen | (NET_FLAG_RELIABLE | eom));
    PacketBuffer.iSequence = htonl(iSendSequence++);
    memcpy(PacketBuffer.pData, pSendBuffer, iDataLen);

    bSendNext = false;

    ret = NET_Write(iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if(ret == -1)
        return;
    if(ret != (int)iPacketLen)
        return;

    dLastSendTime = GetTickCount();
    iPacketsSent++;
}

////////////////////////////////////////////////////////

void CSocket::ReSendMessage(void)
{
    unsigned int  iPacketLen;
    unsigned int  iDataLen;
    unsigned int  eom;
    int ret;

    if (iSendBufferLength <= MAX_DATAGRAM)
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
    
    ret = NET_Write(iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if ((ret == -1)||(ret != (int)iPacketLen))
    {
        return;
    }
    
    dLastSendTime = GetTickCount();
    iPacketsReSent++;
}

////////////////////////////////////////////////////////

bool CSocket::bCanSendMessage (void)
{
    if(bSendNext) SendMessageNext();
    if((!bCanSend)&&(GetTickCount() - dLastMessage > dAliveTimeout))
        return 0;
    return bCanSend;
}

//////////////////////////////////////////////////////////////

void CSocket::SendUnreliableMessage(CPacket *pMessage)
{
    if (pMessage==NULL) return;
    SendUnreliableMessage(pMessage->pGetPacketBuffer(),pMessage->iGetCurSize());
}

//////////////////////////////////////////////////////////////

void CSocket::SendUnreliableMessage(char *pData,int iSize)
{
    int iPacketLen;
    int ret;
    if(iSize<0) return;
    if(iSize>=MAX_DATAGRAM) return;
    if(!pData) return;
    
	iPacketLen = NET_HEADERSIZE + iSize;
	PacketBuffer.iLen     = htonl(iPacketLen |NET_FLAG_UNRELIABLE);
    PacketBuffer.iSequence= htonl(iUnreliableSendSequence++);

    memcpy(PacketBuffer.pData, pData, iSize);
    ret = NET_Write(iSocket,(char *)&PacketBuffer, iPacketLen, &pAddr->stAddr );
    if ((ret == -1) || (ret != iPacketLen))
    {
        Log("s_socket.cpp CSocket::SendUnreliableMessage() error writing to network");
        return;
    }
    iPacketsSent++;
}

////////////////////////////////////////////////////////

int CSocket::iExtractBufferedMessage(int iPos)
{
    int iLength,iFlags, iSequence;
    if (!bExtractBuffer) return false;
    iLength = ntohl(* (unsigned int *) (ExtrBackupBuffer.pData+iPos) );
    iFlags = iLength & (~NET_FLAG_LENGTH_MASK);
    iLength &= NET_FLAG_LENGTH_MASK;
    if (iLength+iPos > (ntohl(ExtrBackupBuffer.iLen)&NET_FLAG_LENGTH_MASK)-NET_HEADERSIZE)
    {
        bExtractBuffer = false;
        return false;
    }
    if (iLength+iPos > NET_DATAGRAMSIZE)
    {
        Log("net_lib.cpp -> forged backup message received");
        iLongPacketCount++;
        bExtractBuffer = false;
        return false;
    }
    iSequence = ntohl(* (unsigned int *) (ExtrBackupBuffer.pData+iPos+sizeof(int)));
    if (iSequence == iBufferedReceivedSequence)
    {
        memcpy(&PacketBuffer,ExtrBackupBuffer.pData+iPos,iLength);
        iBufferedReceivedSequence++;
        iLength-=NET_HEADERSIZE;
        iLastLength=iLength;
        return 3;
    }
    if(iLength+iPos==(ntohl(ExtrBackupBuffer.iLen)&NET_FLAG_LENGTH_MASK)-NET_HEADERSIZE)
    {
        bExtractBuffer = false;
        return false;
    }
    return iExtractBufferedMessage(iPos+iLength);
}


////////////////////////////////////////////////////////

int CSocket::iGetMessage ()
{
    unsigned int  iLength;
    unsigned int  iFlags;
    int       err = 0;
    CInetAddress ReadAddr;
    unsigned int  iSequence;
    unsigned int  iCount;

    if(!bCanSend)
    if((GetTickCount() - dLastSendTime) > dAckTimeOut)
        ReSendMessage();

    if(iExtractBufferedMessage())
        return 3;

    if(iSocket==-1)
        return 0;

    iLastLength = 0;
    while(1) 
    { 
        iLength = NET_Read(iSocket,(char *)&PacketBuffer, NET_DATAGRAMSIZE, &ReadAddr.stAddr);
       
        if (iLength == -1)
        {
            return -1;
        }

        if (iLength == 0)
            break;

        //if( dlcs_strcasecmp(NET_pAddrToString(&ReadAddr.stAddr),NET_pAddrToString(&pAddr->stAddr)) )
        //{
        ////    Log("net_lib.cpp -> forged packet received from [%s] - supposed to be from [%s]",NET_pAddrToString(&ReadAddr.stAddr),NET_pAddrToString(&pAddr->stAddr));
         //   continue;
        //}

        if (iLength < NET_HEADERSIZE)
        {
            iShortPacketCount++;
            continue;
        }

        iLength = ntohl(PacketBuffer.iLen);
        iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
        iLength &= NET_FLAG_LENGTH_MASK;

        if (iFlags & NET_FLAG_CTL)
            continue;

        if (iLength > NET_DATAGRAMSIZE)
        {
            Log("net_lib.cpp -> forged header received %s",NET_pAddrToString(&ReadAddr.stAddr));
            iLongPacketCount++;
            continue;
        }

        // finally a valid message
        iSequence = ntohl(PacketBuffer.iSequence);
        iPacketsReceived++;
        dLastMessage = GetTickCount();

        if (iFlags & NET_FLAG_BACKUP)
        {
            if (iSequence > iBufferedReceivedSequence)
            {
                iCount = iSequence - iBufferedReceivedSequence;
                iDroppedDatagrams += iCount;
                Log("net_lib.cpp -> dropped %u datagram(s)", iCount);
            }
            ExtrBackupBuffer = PacketBuffer;
            bExtractBuffer = true;
            if(iExtractBufferedMessage())
            {
                err = 3;
                break;
            }
            continue;
        }

        if (iFlags & NET_FLAG_UNRELIABLE)
        {
            if (iSequence < iUnreliableReceiveSequence)
            {
                Log("net_lib.cpp -> got a stale datagram");
                err =0;
                break;
            }
            
            if (iSequence != iUnreliableReceiveSequence)
            {
                iCount = iSequence - iUnreliableReceiveSequence;
                iDroppedDatagrams += iCount;
                Log("net_lib.cpp -> dropped %u datagram(s)", iCount);
            }
            
            iUnreliableReceiveSequence = iSequence + 1;
                    
            iLength -= NET_HEADERSIZE;
            iLastLength = iLength;
            
            err = 2;
            break;
        }
        
        if (iFlags & NET_FLAG_ACK)
        {
            if (iSequence != (iSendSequence - 1))
            {
                Log("net_lib.cpp -> stale ACK received %s",NET_pAddrToString(&ReadAddr.stAddr));
                continue;
            }
            iSendBufferLength -= MAX_DATAGRAM;
            if (iSendBufferLength > 0)
            { 
                memmove(pSendBuffer, pSendBuffer+MAX_DATAGRAM, iSendBufferLength);
                bSendNext = true;
            }
            else
            {
                iSendBufferLength = 0;
                bCanSend = true;
            }
            continue;
        }

        if (iFlags & NET_FLAG_RELIABLE)
        {
            PacketBuffer.iLen =      ntohl(NET_HEADERSIZE | NET_FLAG_ACK);
            PacketBuffer.iSequence = ntohl(iSequence);
            if (NET_Write (iSocket,(char *)&PacketBuffer, NET_HEADERSIZE, &ReadAddr.stAddr)==-1)
            {
                return -1;
            }

            if (iSequence != iReceiveSequence)
            {
                iReceivedDuplicateCount++;
                continue;
            }
            iReceiveSequence++;

            iLength -= NET_HEADERSIZE;
            
            if (iReceiveBufferLength + (int)iLength < iCurReceiveSize)
            {
                
                if ((iReceiveBufferLength + iLength < NET_MINMESSAGE)&&(iCurReceiveSize != NET_MINMESSAGE))
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

            if (iFlags & NET_FLAG_EOM)
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

  if (bSendNext)
      SendMessageNext();

  return err;
}

////////////////////////////////////////////////////////

char *CSocket::pcGetMessage ()
{
    assert(pReceiveBuffer != NULL);
    if (iLastLength)
        return PacketBuffer.pData;
    else
        return pReceiveBuffer;
}

////////////////////////////////////////////////////////

int CSocket::iGetMessageSize()
{
      if (iLastLength)
          return iLastLength;
      else
          return iReceivedMessageLength;        
}

////////////////////////////////////////////////////////

CPacket *CSocket::pGetMessage()
{
    if(pTempPacket==NULL)
    {
        pTempPacket=new CPacket(iGetMessageSize(),pcGetMessage());
        if(!pTempPacket)return 0;
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

void CSocket::Disconnect()
{
    NET_CloseSocket(iSocket);
}

////////////////////////////////////////////////////////

void CSocket::SetAddress(CInetAddress *pAddress)
{
    pAddr = pAddress;
	
}

////////////////////////////////////////////////////////

CInetAddress *CSocket::pGetAddress(void)
{
    return pAddr;
}

////////////////////////////////////////////////////////

void CSocket::FinishCtlPacket(CPacket *pPacket)
{
    ::FinishCtlPacket(pPacket);
}

////////////////////////////////////////////////////////
/*
void CServerSocket::NetStat()
{
    
    //Log("АБВллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллВБА");
    //Log("%i packets sent - %i packets had to be resent(%i%%)",  INet::iPacketsSent,INet::iPacketsReSent,INet::iPacketsReSent*100/(INet::iPacketsReSent?INet::iPacketsReSent:-1));
    //Log("%i packets recieved - %i of them where lost(%i%%)",    INet::iPacketsReceived,INet::iDroppedDatagrams,INet::iDroppedDatagrams*100/(INet::iPacketsReceived?INet::iPacketsReceived:-1));
    //Log("%i packets recieved more than once(%i%%)",             INet::iReceivedDuplicateCount,INet::iReceivedDuplicateCount*100/(INet::iPacketsReceived?INet::iPacketsReceived:-1));
    //Log("%i long and %i short corrupted packets received",      INet::iShortPacketCount,INet::iLongPacketCount);
    //Log("АБВллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллВБА");
    
}
*/

