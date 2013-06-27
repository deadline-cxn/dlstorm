/***************************************************************
 Deadline's Code Storm (Log Class)
 ***************************************************************/

#ifndef _DLCS_LOG_CLASS
#define _DLCS_LOG_CLASS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdlib.h>
class C_DLCS_Log
{
public:
	C_DLCS_Log(void);
    C_DLCS_Log(char *szFilename);
	virtual ~C_DLCS_Log(void);
    void Initialize(void);
    void AddEntry(char *fmt, ...);
    void AddEntryNoTime(char *fmt, ...);
	void AddLineSep(void);
	void SetName(char *szFilename);
	void Off(void);
	void On(void);
    void LineFeedsOn(void);
    void LineFeedsOff(void);
    bool IsActive(void);
	bool Restart(void);
	char szBegin[255];
	char szEnd[255];
	char szLineSep[255];
	char logfile[_MAX_PATH];
    char currentdir[_MAX_PATH];
    char logdir[_MAX_PATH];
	bool bActive;
    bool bLineFeeds;
};

#endif // _DLCS_LOG_CLASS
