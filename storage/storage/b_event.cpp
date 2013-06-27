/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "b_event.h"
#include "fm_util.h"
#include "dlstorm.h"

//////////////////////////////////////////////////////
C_FM_EVENT::C_FM_EVENT() { timer=dlcs_get_tickcount(); timer_expire=0; }
//////////////////////////////////////////////////////
C_FM_EVENT::~C_FM_EVENT() { }
//////////////////////////////////////////////////////

bool C_FM_EVENT::expired(void)
{
    if(dlcs_get_tickcount()-timer>timer_expire)
    {
        return 1;
    }
    return 0;
}
//////////////////////////////////////////////////////////
void C_FM_EVENT::reset_timer(void)
{
    timer=dlcs_get_tickcount();
}
//////////////////////////////////////////////////////////
void C_FM_EVENT::set_timer(long new_time)
{
    timer_expire=new_time;
}
