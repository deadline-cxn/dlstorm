/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_event.h
 **   Description:  Base Event Class Header File
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_EVENT_CLASS
#define _DLCS_EVENT_CLASS

class CEvent {
   public:
    CEvent();
    ~CEvent();

    enum event_type {  // Interaction event types:
        DLCS_EVENT_NONE = 0,
        DLCS_EVENT_HOVER,
        DLCS_EVENT_CLICK,
        DLCS_EVENT_DOUBLE_CLICK,
        DLCS_EVENT_NEAR,
        DLCS_EVENT_FAR,
        DLCS_EVENT_LEAVE,
        DLCS_EVENT_ENTER,
        DLCS_EVENT_WAIT,
        DLCS_EVENT_MOVE,
        DLCS_EVENT_CREATE,
        DLCS_EVENT_DESTROY,
        DLCS_EVENT_TIMER_CYCLE,
        DLCS_EVENT_USE,
        DLCS_EVENT_SPEAK,
        DLCS_EVENT_SOUND,
        DLCS_EVENT_ATTACK,
        DLCS_EVENT_RESET,
        DLCS_EVENT_HEAL,
        DLCS_EVENT_DIAGNOSE,
        DLCS_EVENT_TARGET,
        DLCS_EVENT_DEATH,
        DLCS_EVENT_SPAWN,
        DLCS_EVENT_RESURRECT
    };
    enum event_action {  // Actions:
        DLCS_EVENT_ACT_NONE = 0,
        // sensory actions: (CPU AI CONTROLLED)
        DLCS_EVENT_ACT_PATH,
        DLCS_EVENT_ACT_DIAGNOSE,  // diagnose self
        DLCS_EVENT_ACT_OBSERVE,
        DLCS_EVENT_ACT_LISTEN,
        // reactionary actions:
        DLCS_EVENT_ACT_MOVE,
        DLCS_EVENT_ACT_TARGET,
        DLCS_EVENT_ACT_ATTACK,
        DLCS_EVENT_ACT_CONSUME,
        DLCS_EVENT_ACT_USE,
        DLCS_EVENT_ACT_SPEAK,
        DLCS_EVENT_ACT_SOUND,
        DLCS_EVENT_ACT_CREATE,
        DLCS_EVENT_ACT_MERGE,  // two entities merge
        DLCS_EVENT_ACT_SPLIT   // one entity splits into two (or more)
    };

    // bool    push_event(void *rcv_entity, int event, const char *args, void *action_entity);
    bool push_event(int event, const char *args);
    bool exec_event(int event, const char *args);  //, void *action_entity);

    /* use: push_event(entity,event,event_args,entity_to_take_action_on (if 0 it will action self));
       example:
        current_entity->push_event(current_entity->target,DLCS_EVENT_ATTACK,"243",current_entity);
        target->exec_event(sending_entity,DLCS_EVENT_ATTACK,"243",sending_entity);
        target->on_attack("243",sending_entity);             */

    void on_create(const char *args);        //,void *entity);
    void on_destroy(const char *args);       //,void *entity);
    void on_near(const char *args);          //,void *entity);
    void on_far(const char *args);           //,void *entity);
    void on_leave(const char *args);         //,void *entity);
    void on_enter(const char *args);         //,void *entity);
    void on_wait(const char *args);          //,void *entity);
    void on_hover(const char *args);         //,void *entity);
    void on_click(const char *args);         //, void *entity);
    void on_double_click(const char *args);  //, void *entity);
    void on_timer_cycle(const char *args);   //, void *entity);
    void on_use(const char *args);           //, void *entity);
    void on_speak(const char *args);         //, void *entity);
    void on_sound(const char *args);         //, void *entity);
    void on_attack(const char *args);        //, void *entity);
    void on_target(const char *args);        //, void *entity);
    void on_heal(const char *args);          //, void *entity);
    void on_reset(const char *args);         //, void *entity);
    void on_death(const char *args);         //, void *entity);

    bool act_move();      // void *entity);
    bool act_target();    // void *entity);
    bool act_attack();    // void *entity);
    bool act_consume();   // void *entity);
    bool act_use();       // void *entity);
    bool act_speak();     // void *entity);
    bool act_sound();     // void *entity);
    bool act_create();    // void *entity);
    bool act_merge();     // void *entity);
    bool act_split();     // void *entity);
    bool act_path();      // void *entity);
    bool act_diagnose();  // void *entity);
    bool act_observe();   // void *entity);
    bool act_listen();    // void *entity);
};

#endif  // _DLCS_EVENT_CLASS