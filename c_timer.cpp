/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    Timer class

****************************************************************/

#include "c_timer.h"

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
    currenttick=getticks();
}
bool CTimer::Up() {
    if((getticks()-currenttick) > duration) {
        return true;
    }
    return false;
}
