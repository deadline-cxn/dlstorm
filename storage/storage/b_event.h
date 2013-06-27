/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_BASE_EVENT
#define _EMBER_BASE_EVENT

#include "global.h"
#include "b_base.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C_FM_EVENT : public C_FM_BASE
{
public:
    C_FM_EVENT();
    virtual ~C_FM_EVENT();
    long timer;
    long timer_expire;
    void set_timer(long);
    bool expired(void);
    void reset_timer(void);

};

#endif // _EMBER_BASE_EVENT
