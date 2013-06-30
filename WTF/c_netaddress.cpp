/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_net.h"
#include "c_netaddress.h"
#include "dlstorm.h"
//#include "global.h"

/*
#include "dlstorm.h"
#ifdef __linux__
#include "s_gnu.h"
#endif
*/

////////////////////////////////////////////////////////
CInetAddress::CInetAddress() { iSocket=-1; }
////////////////////////////////////////////////////////
CInetAddress::~CInetAddress() { }
////////////////////////////////////////////////////////
int CInetAddress::operator==(CInetAddress rp)  // compares address against another address
{
    int ret = NET_AddrCompare(&stAddr,&rp.stAddr);
    if (ret == 0) return true; return false;
}
////////////////////////////////////////////////////////
int CInetAddress::operator!=(CInetAddress rp)  // compares address against another address
{
    int ret = NET_AddrCompare(&stAddr,&rp.stAddr);
    if (ret != 0) return true; return false;
}
////////////////////////////////////////////////////////
CInetAddress *CInetAddress::pGetByName(char *pHost) // determines the IP address of a host given the host's name
{
    CInetAddress *pCurAddr;
    struct sockaddr stTempAddr;
    NET_GetAddrFromName(pHost,&stTempAddr);
    pCurAddr = new CInetAddress;
    pCurAddr->stAddr = stTempAddr;
    return pCurAddr;
}
////////////////////////////////////////////////////////
char *CInetAddress::pGetHostName(void)
{
    if(NET_GetNameFromAddr(&stAddr,pName)!=0) { return 0; }
    return pName;
}
////////////////////////////////////////////////////////
void CInetAddress::SetPort(char *pPort) { }
////////////////////////////////////////////////////////
char *CInetAddress::GetPort(void)
{
    char szTemp[256];
    memset(szTemp,0,256);
    return strdup(szTemp);
}
////////////////////////////////////////////////////////
void CInetAddress::Ping(void)
{
    //Log("Ping() start");
    CPacket Send(64);
    if (iSocket == -1)
    {
        iPingCount = 0;
        fPing = 0;
        //iSocket = OpenSocket(0);
        if (iSocket==-1)
        {
            //Log("Ping() ping to host failed");
            return;
        }
    }
    /*
    Send.Write(0);  // save header space which is filled in later
    Send.Write(0);
    Send.Write((char)CTL_PING);
    Send.Write((long)dlcs_get_tickcount());
    Send.pWrite(32-sizeof(double));
    FinishCtlPacket(&Send);
    // send connection request
    if (NET_Write(iSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),&stAddr)==-1)
    {
        //Log("Ping() ping to host failed 2");
    }
    */
    //Log("Ping() end");
}
////////////////////////////////////////////////////////
void CInetAddress::Ping(int iCount) { for (int i = 0; i<iCount; i++) Ping(); }
///////////////////////////////////////////////////////
long CInetAddress::dGetPing(void) { return 9999; }
