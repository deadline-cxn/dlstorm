/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_timer.h
 **   Class:        CTimer
 **   Description:  Timer class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_CTIMER
#define _DLCS_CTIMER
#include "dlcs.h"
#define TIMER_DEFAULT 200
////////////////////////////// CTimer class
class CTimer {
   public:
    CTimer();
    CTimer(long dur);
    ~CTimer();
    long duration;
    long currenttick;
    void Reset(void);
    bool Up(void);
};
#endif  // _DLCS_CTIMER
