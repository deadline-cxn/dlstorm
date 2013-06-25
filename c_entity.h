/* Seth's Game Entity Class */

#ifndef SETH_GAME_ENTITY
#define SETH_GAME_ENTITY

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dlstorm.h"
#include "c_log.h"

// Basic Entity Types:
#define G_ENTITY_NONE           0
#define G_ENTITY_PLAYER         1 // An entity that is controlled by the user's input
#define G_ENTITY_REMOTE_PLAYER  2 // An entity that is controlled by a user on a network
#define G_ENTITY_AGGRESSOR      3 // CPU controlled enemy unit
#define G_ENTITY_FRIENDLY       4 // CPU controlled friendly unit
#define G_ENTITY_VENDOR         5 // An entity that sells or buys...
#define G_ENTITY_NODE           6 // Power ups, resource pools, etc. Stuff that respawns and can be harvested
#define G_ENTITY_MISSLE         7 // Stuff that moves or not, and has a short lifespan (ie; a bullet)
#define G_ENTITY_INFORMATION    8 // Stuff like a sign or other thing that gives the player information on interaction
#define G_ENTITY_WAYPOINT       9
#define G_ENTITY_AMBIENT        10 // for adding sound or lighting
#define G_ENTITY_PROP           11
#define G_ENTITY_ITEM           20 // Items... like a gun or backpack that the other entities can use or inventory

// Interaction event types:
#define G_ENTITY_HOVER          1
#define G_ENTITY_CLICK          2
#define G_ENTITY_DOUBLE_CLICK   3
#define G_ENTITY_NEAR           4
#define G_ENTITY_FAR            5
#define G_ENTITY_LEAVE          6
#define G_ENTITY_ENTER          7
#define G_ENTITY_WAIT           8
#define G_ENTITY_MOVE           9
#define G_ENTITY_CREATE         10
#define G_ENTITY_DESTROY        11
#define G_ENTITY_TIMER_CYCLE    12
#define G_ENTITY_USE            13
#define G_ENTITY_SPEAK          14
#define G_ENTITY_SOUND          15
#define G_ENTITY_ATTACK         16
#define G_ENTITY_RESET          17
#define G_ENTITY_HEAL           18
#define G_ENTITY_DIAGNOSE       19
#define G_ENTITY_TARGET         20
#define G_ENTITY_DEATH          21
#define G_ENTITY_SPAWN          22
#define G_ENTITY_RESURRECT      24

// Actions:
#define G_ENTITY_ACT_NONE       0
// sensory actions: (CPU AI CONTROLLED)
#define G_ENTITY_ACT_PATH       11
#define G_ENTITY_ACT_DIAGNOSE   5   // diagnose self
#define G_ENTITY_ACT_OBSERVE    14
#define G_ENTITY_ACT_LISTEN     16
// reactionary actions:
#define G_ENTITY_ACT_MOVE       2
#define G_ENTITY_ACT_TARGET     1
#define G_ENTITY_ACT_ATTACK     3
#define G_ENTITY_ACT_CONSUME    4
#define G_ENTITY_ACT_USE        15
#define G_ENTITY_ACT_SPEAK      6
#define G_ENTITY_ACT_SOUND      7
#define G_ENTITY_ACT_CREATE     8
#define G_ENTITY_ACT_MERGE      9   // two entities merge
#define G_ENTITY_ACT_SPLIT      10  // one entity splits into two (or more)

#define G_ENTITY_DEFAULT_RESPAWN_TIME   30000 // 5 minutes

class CEntity;

/*struct G_ENTITY_EVENT_RESULTS
{
    int returned_event;
    char *args;
    CEntity *entity;
};
*/

class CEntity
{
public:
    CEntity();
    CEntity(char *nm);
    ~CEntity();

    CLog    *Log;

    char    name[1024];
    int     type;

    int     x;
    int     y;
    int     z;

    int     life_points;
    int     mana_points;
    int     power_points;
    int     rage_points;

    int     at_sta;
    int     at_int;
    int     at_spi;
    int     at_wis;
    int     at_agi;
    int     at_con;

    // A power up would be:                             resource_min 1, resource_max 1, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 30000
    // A mine node would be:                            resource_min 1, resource_max 6, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 30000
    // A coin in a scrolling platform game:             resource_min 1, resource_max 1, respawn_min 1, respawn_max 1, respawn_timer_min 0    , respawn_timer_max 0
    // Node on random timer between 5 and 10 minutes:   resource_min 3, resource_max_9, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 60000

    int     resource_min;       // 0 = infinite resources
    int     resource_max;       // 0 = infinite resources

    int     respawn_min;        // 0 = infinite respawns
    int     respawn_max;        // 0 = infinite respawns

    long    respawn_time_min;   // 0 = default; default is 5 minutes (30000)
    long    respawn_time_max;   // 0 = default; default is 5 minutes (30000)

    CEntity *parent;

    CEntity *next;
    CEntity *prev;

    CEntity *target;

    void    set_defaults(void);

    bool    roam(void); //  brains of the entity, includes movement and other checking called once per cycle

    bool    push_event(CEntity *rcv_entity, int event, char *args, CEntity *action_entity);
    bool    exec_event(int event, char *args, CEntity *action_entity);

    /* use: push_event(entity,event,event_args,entity_to_take_action_on (if 0 it will action self));
       example:
        current_entity->push_event(current_entity->target,G_ENTITY_ATTACK,"243",current_entity);
        target->exec_event(sending_entity,G_ENTITY_ATTACK,"243",sending_entity);
        target->on_attack("243",sending_entity);             */

    void on_create(char *args,CEntity *entity);
    void on_destroy(char *args,CEntity *entity);
    void on_near(char *args,CEntity *entity);
    void on_far(char *args,CEntity *entity);
    void on_leave(char *args,CEntity *entity);
    void on_enter(char *args,CEntity *entity);
    void on_wait(char *args,CEntity *entity);
    void on_hover(char *args,CEntity *entity);
    void on_click(char *args, CEntity *entity);
    void on_double_click(char *args, CEntity *entity);
    void on_timer_cycle(char *args, CEntity *entity);
    void on_use(char *args, CEntity *entity);
    void on_speak(char *args, CEntity *entity);
    void on_sound(char *args, CEntity *entity);
    void on_attack(char *args, CEntity *entity);
    void on_target(char *args, CEntity *entity);
    void on_heal(char *args, CEntity *entity);
    void on_reset(char *args, CEntity *entity);
    void on_death(char *args, CEntity *entity);

    bool act_move(CEntity *entity);
    bool act_target(CEntity *entity);
    bool act_attack(CEntity *entity);
    bool act_consume(CEntity *entity);
    bool act_use(CEntity *entity);
    bool act_speak(CEntity *entity);
    bool act_sound(CEntity *entity);
    bool act_create(CEntity *entity);
    bool act_merge(CEntity *entity);
    bool act_split(CEntity *entity);
    bool act_path(CEntity *entity);
    bool act_diagnose(CEntity *entity);
    bool act_observe(CEntity *entity);
    bool act_listen(CEntity *entity);



};












#endif // SETH_ENTITY_CLASS
