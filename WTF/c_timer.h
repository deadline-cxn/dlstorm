/* Seth's Timer Class */

#ifndef _SETH_TIMER
#define _SETH_TIMER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlstorm.h"

#define TIMER_DEFAULT 200

class CTimer
{
public:

    CTimer();
    CTimer(long dur);
    ~CTimer();

    long duration;
    long currenttick;

    void Reset(void);
    bool Up(void);


};

#endif
