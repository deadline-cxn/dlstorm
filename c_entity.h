/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_entity.h
 **   Description:  Base Entity Class Header File
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_ENTITY_CLASS
#define _DLCS_ENTITY_CLASS

#include "c_event.h"
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


#define G_ENTITY_DEFAULT_RESPAWN_TIME   30000 // 5 minutes

class CEntity; 

/*struct G_ENTITY_EVENT_RESULTS
{
    int returned_event;
    char *args;
    CEntity *entity;
};
*/

class CEntity : public CEvent {
public:
    CEntity();
    CEntity(const char *nm);
    ~CEntity();
    char    szName[_NAME_SIZE];
    int     type;
    int     life_points;
    int     mana_points;
    int     power_points;
    int     rage_points;
    int     status;

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
};

#endif // _DLCS_ENTITY_CLASS
