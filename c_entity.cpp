/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_entity.cpp
 **   Description:  Base Entity Class
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_entity.h"

#include "c_event.h"

CEntity::CEntity() {
    set_defaults();
    // Log=new CLog((const char *)"Entity.log");
}

CEntity::CEntity(const char *nm) {
    set_defaults();
    strcpy(szName, nm);
    // Log=new CLog(va("%s.log",name));
}

CEntity::~CEntity() {
    // dlcsm_delete(Log);
}

void CEntity::set_defaults() {
    strcpy(szName, "Unknown");
    type = G_ENTITY_FRIENDLY;
    // x=100;
    // y=100;
    // z=0;
    life_points      = 100;
    mana_points      = 100;
    power_points     = 100;
    rage_points      = 100;
    resource_min     = 0;      // 0 = infinite resources
    resource_max     = 0;      // 0 = infinite resources
    respawn_min      = 0;      // 0 = infinite respawns
    respawn_max      = 0;      // 0 = infinite respawns
    respawn_time_min = 30000;  // 0 = default; default is 5 minutes (30000)
    respawn_time_max = 30000;  // 0 = default; default is 5 minutes (30000)
}
