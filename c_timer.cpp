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
    currenttick=dlcs_get_tickcount();
}
bool CTimer::Up() {
    if((dlcs_get_tickcount()-currenttick) > duration) {
        Reset();
        return true;
    }
    return false;
}
