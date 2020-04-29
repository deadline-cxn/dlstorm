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

CEntity::CEntity(const char *szInName) {
    set_defaults();
    strcpy(szName, szInName);
    // Log=new CLog(va("%s.log",name));
}

CEntity::~CEntity() {
    // dlcsm_delete(Log);
}

void CEntity::set_defaults() {
    strcpy(szName, "Unknown");
    type             = G_ENTITY_FRIENDLY;
    loc.x            = 0;
    loc.y            = 0;
    loc.z            = 0;
    rot.x            = 0;
    rot.y            = 0;
    rot.z            = 0;
    autorot.x        = 0;
    autorot.y        = 0;
    autorot.z        = 0;
    scale.x          = 1;
    scale.y          = 1;
    scale.z          = 0;
    color.r          = 1;
    color.g          = 1;
    color.b          = 1;
    bSelected        = false;
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
