/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_event.cpp
 **   Description:  Base Event Class
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_event.h"

#include "dlcs.h"

CEvent::CEvent() {}

// CEvent::CEvent(const char *nm) { }

CEvent::~CEvent() {}

bool CEvent::push_event(int event, const char *args) {
    // if(!rcv_entity) return false; //if(!action_entity) action_entity=this; //,action_entity);
    // rcv_entity->
    exec_event(event, args);
    return true;
}

bool CEvent::exec_event(int event, const char *args) {  // }, void *action_entity) {
    // if(action_entity==0) action_entity=this;

    switch (event) {
        case DLCS_EVENT_NONE: return true;
        case DLCS_EVENT_ATTACK: on_attack(args); break;
        case DLCS_EVENT_HEAL: on_heal(args); break;
        case DLCS_EVENT_TARGET: on_target(args); break;
        case DLCS_EVENT_DEATH: on_death(args); break;
        default: break;
    }

    return true;
}

void CEvent::on_death(const char *args) {  //},void *entity) {
    // if(entity==0) return;
}

void CEvent::on_target(const char *args) {  //},void *entity) {
    // if(entity==0) return;
    // if(entity!=target) {
    // target=entity;
    //}
}

void CEvent::on_attack(const char *args) {  //,void *entity) {
    // if(entity==0) return;
    // int attack_dmg=atoi(args);

    // if(life_points>0) {
    //    life_points-=attack_dmg;
    //    if(life_points<0) life_points=0;
    //    if(life_points==0) {
    //        push_event(this,DLCS_EVENT_DEATH,(const char *)"0",entity);
    //    }
    //    else {
    //        push_event(this,DLCS_EVENT_TARGET,(const char *)"1",entity);
    //    }
    // }
    // else {
    // }
}

void CEvent::on_heal(const char *args) {  //,void *entity) {
    // if(entity==0) return;
    // int heal;
    // heal=atoi(args);
}