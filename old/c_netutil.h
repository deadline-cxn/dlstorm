/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_NET_UTIL
#define _EMBER_NET_UTIL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C"
{
#endif

    void DoNetwork(void);
    int  NetworkConnect();
    int  NetworkSetup(void);
    long Ping(void);
    //void FindTarget(void);
	//void GetFile(char *filename);
#ifdef __cplusplus
}
#endif


#endif
