/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#ifndef _EMBER_SOCKET
#define _EMBER_SOCKET

#include "dlstorm.h"
#include "c_netaddress.h"
#include "c_net.h"

#define MAX_DATAGRAM                1024    // Length of unreliable message



typedef enum{
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


class CCSocket
{
public:
    CCSocket(void);
    ~CCSocket();
    virtual CCSocket *pAccept(void);
    CCSocket         *pAccept(int iReSocket,struct sockaddr *ReAddr);
    virtual int      iGetMessage();             // 0 if no incomming data, 1 unreliable, 2 reliable
    virtual char     *pcGetMessage();
    virtual int      iGetMessageSize();
    virtual          CPacket *pGetMessage();
    virtual double   dReceiveStatus();          // gets message data
    virtual bool     bCanSendMessage ();        // returns true if a reliable message can be send
    virtual void     SendReliableMessage (char *pData, int iSize);
    virtual void     SendReliableMessage (CPacket *pMessage);
    virtual void     SendUnreliableMessage(char *pData,int iSize);
    virtual void     SendUnreliableMessage(CPacket *pMessage);
    //virtual void     SendBufferedMessage(char *pData,int iSize);
    //virtual void     SendBufferedMessage(CPacket *pMessage);
    virtual double   dSendStatus();             // sends a message
    virtual void     Disconnect();              // if a dead connection is returned by a get or send function, this function should be called when it is convenient
    CInetAddress     *pGetAddress(void);
    void             SetAddress(CInetAddress *pAddress); // connection information
    void             SendMessageNext(void);
    void             ReSendMessage (void);
    //int              iExtractBufferedMessage(int iPos=0);
    void             FinishCtlPacket(CPacket *pPacket);
    bool             bConnecting;
    int              dConnectRetries;
    int              dConnectRetry;             // how often attempt to retry to connect
    int              dConnectTimeOut;           // timeout for a connection request
    int              iLastLength;
    int              iSocket;
    CInetAddress     *pAddr;
    CPacket          *pTempPacket;              // temporary packet for pGetMessage
    bool             bConnected;                // exists a validated connection?
    bool             bCanSend;                  // indicates wether reliable messages can be sent or not
    bool             bSendNext;                 // can we send next part of the message

    //bool             bExtractBuffer;            // indicates that a buffered message was received
    //int              iCurrentBuffer ;           // current size of buffered data
    //char             *pBackupBuffer;            // queuebuffer for buffered messages

    char             *pSendBuffer;              // pointer to sendbuffer
    int              iCurBufSize;               // current size of messagebuffer
    int              iSendBufferLength;         // length of current message
    char             *pReceiveBuffer;           // pointer to receivebuffer

    int              iCurReceiveSize;           // current size of receivebuffer
    int              iReceiveBufferLength;      // currently read data
    int              iReceivedMessageLength;    // length of the last message
    int              iCurRecSize;
    long             dLastSendTime;             // time when last reliable message was sent
    long             dLastMessage;              // time when last message was received
    unsigned int     iSendSequence;             // current sendsequence for reliable messages
    unsigned int     iUnreliableSendSequence;   // current sendsequence for unreliable messages
    unsigned int     iUnreliableReceiveSequence;// current receivesequence for unreliable messages

    //unsigned int     iBufferedSendSequence;     // current sendsequenece for buffered messages
    //unsigned int     iBufferedReceivedSequence; // current receivesequenece for buffered messages

    unsigned int     iReceiveSequence;          // current receivesequence for reliable messages
    long             dAliveTimeout;             // after that time the server or client has timed out
    long             dAckTimeOut;               // how long wait before resending a message
    long             dGuessReboot;
    long             dMasterTimeout;
    long             dMasterRetry;
    long             dBufferedSize;
    long             dBufferedQueue;
    unsigned int     iPacketsSent;              // total number of sent packets
    unsigned int     iPacketsReSent;            // total number of resent packets
    unsigned int     iShortPacketCount;         // total number of packets that are shorter than the header itself
    unsigned int     iLongPacketCount;
    unsigned int     iPacketsReceived;
    unsigned int     iDroppedDatagrams;
    unsigned int     iReceivedDuplicateCount;
    //queue<int>       BufferedQ;               // sizes of the buffered packets

    #pragma pack(1)
    struct
    {
        unsigned int  iLen;
        unsigned int  iSequence;
        char          pData[MAX_DATAGRAM];
    } PacketBuffer;//,ExtrBackupBuffer;
    #pragma pack()
};

#endif // _EMBER_SOCKET

