/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#ifndef _EMBER_GAME_SERVER_SOCKET
#define _EMBER_GAME_SERVER_SOCKET

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
#include "global.h"

//#include <queue>
//#include <set>

#include "dlstorm.h" //class CInetAddress;
#include "b_netaddress.h" //class CPacket;

using namespace std;

class CSocket
{
public:
    CSocket(void);
    ~CSocket();

    virtual int     iGetMessage();
	// 0 if no incomming data, 1 unreliable, 2 reliable
    virtual char    *pcGetMessage();
    virtual int     iGetMessageSize();
    virtual         CPacket *pGetMessage();
    virtual bool    bCanSendMessage ();
	// returns true if a reliable message can be send
    virtual void    SendReliableMessage (char *pData, int iSize);
    virtual void    SendReliableMessage (CPacket *pMessage);
    virtual void    SendUnreliableMessage(char *pData,int iSize);
    virtual void    SendUnreliableMessage(CPacket *pMessage);
    virtual void    Disconnect();
    // if a dead connection is returned by a get or send function,
	// this function should be called when it is convenient
    CInetAddress    *pGetAddress(void);
    void            SetAddress(CInetAddress *pAddress);
	// connection information
    void            SendMessageNext(void);
    void            ReSendMessage (void);
    void            FinishCtlPacket(CPacket *pPacket);

    //virtual void    SendBufferedMessage(char *pData,int iSize);
    //virtual void    SendBufferedMessage(CPacket *pMessage);

    int             iExtractBufferedMessage(int iPos=0);

    #pragma pack(1)
    struct jerry
    {
        unsigned int  iLen;
        unsigned int  iSequence;
        char          pData[MAX_DATAGRAM];
    } PacketBuffer,ExtrBackupBuffer;
    #pragma pack()
    int iLastLength;
    int iSocket;
    CInetAddress *pAddr;
    CPacket *pTempPacket;       // temporary packet for pGetMessage
    bool bConnected ;           // exists a validated connection?
    bool bCanSend;              // indicates wether reliable messages can be sent or not
    bool bSendNext;             // can we send next part of the message

	bool bExtractBuffer;        // indicates that a buffered message was received

    //queue<int> BufferedQ;       // sizes of the buffered packets

	int iCurrentBuffer ;        // current size of buffered data
    char *pBackupBuffer;        // queuebuffer for buffered messages
    char *pSendBuffer;          // pointer to sendbuffer
    int   iCurBufSize;          // current size of messagebuffer
    int   iSendBufferLength;    // length of current message
    char *pReceiveBuffer;       // pointer to receivebuffer
    int iCurReceiveSize;        // current size of receivebuffer
    int iReceiveBufferLength;   // currently read data
    int iReceivedMessageLength; // length of the last message
    int   iCurRecSize;
    long dLastSendTime;       // time when last reliable message was sent
    long dLastMessage;        // time when last message was received
    unsigned int iSendSequence;           // current sendsequence for reliable messages
    unsigned int iUnreliableSendSequence; // current sendsequence for unreliable messages
    unsigned int iUnreliableReceiveSequence; // current receivesequence for unreliable messages
    unsigned int iBufferedSendSequence; // current sendsequenece for buffered messages
    unsigned int iBufferedReceivedSequence; // current receivesequenece for buffered messages
    unsigned int iReceiveSequence;        // current receivesequence for reliable messages
    long dAliveTimeout;        // after that time the server or client has timed out
    long dConnectRetry;        // how often attempt to retry to connect
    long dConnectTimeOut;      // timeout for a connection request
    long dAckTimeOut;          // how long wait before resending a message
    long dGuessReboot;
    long dMasterTimeout;
    long dMasterRetry;
    long dBufferedSize;
    long dBufferedQueue;

    unsigned int iPacketsSent;  // total number of sent packets
    unsigned int iPacketsReSent;// total number of resent packets
    unsigned int iShortPacketCount; // total number of packets that are shorter than the header itself
    unsigned int iLongPacketCount;
    unsigned int iPacketsReceived;
    unsigned int iDroppedDatagrams;
    unsigned int iReceivedDuplicateCount;
};

*/

class CServerSocket : public CCSocket
{
public:
    CServerSocket(void) {}
    CServerSocket(int iPort);
    ~CServerSocket(){}// {NetStat();}

	//CSocket *pAccept(void);
	// returns a new connection number if there is one pending, else NULL

	CCSocket *pGetFirstSocket();
    CCSocket *pGetNextSocket(); // returns pointer to connections
    void SetServerInfo(CPacket *pPacket){ pServerInfo = pPacket; }
    CPacket *pGetServerInfo(void){ return pServerInfo; } // serverinfo getter and setter
    //void SetMaster(CMasterAddress *pMaster);
    //CMasterAddress *pGetMaster();
    void SetUpMessage(CPacket *pPacket); // configure the use of the master server
    //void NetStat(); // prints the net statistics
    void RejectConnection(int iReSocket,sockaddr *pReAddr,char *pText);
    //CMasterAddress *pMa;
    CPacket *pServerInfo;
};
/* */
#endif
