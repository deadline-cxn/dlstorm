/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_timer.cpp
 **   Class:        CTimer
 **   Description:  Timer class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_timer.h"
////////////////////////////// CTimer class
CTimer::CTimer() {
    duration=TIMER_DEFAULT;
    Reset();
}
CTimer::CTimer(long dur) {
    duration=dur;
    Reset();
}
CTimer::~CTimer() { }
void CTimer::Reset() {
    currenttick=dlcs_get_tickcount();
}
bool CTimer::Up() {
    if((dlcs_get_tickcount()-currenttick) > duration) {
        Reset();
        return true;
    }
    return false;
}


