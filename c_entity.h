/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef _C_ENTITY
#define _C_ENTITY

#include "c_waypoint.h"
#include "c_log.h"
#include "c_gaf.h"
#include "c_gfx.h"
#include "c_gl3dmodel.h"

class CGLModel;
class C_GFX;

#define ENTITY_DEFAULT_RESPAWN_TIME   30000 // 5 minutes

enum tEntityActions {
    // Actions:
    ENTITY_ACT_NONE,
    // sensory actions: (CPU AI CONTROLLED)
    ENTITY_ACT_PATH,
    ENTITY_ACT_DIAGNOSE,   // diagnose self
    ENTITY_ACT_OBSERVE,
    ENTITY_ACT_LISTEN,
    // reactionary actions:
    ENTITY_ACT_MOVE,
    ENTITY_ACT_TARGET,
    ENTITY_ACT_ATTACK,
    ENTITY_ACT_CONSUME,
    ENTITY_ACT_USE,
    ENTITY_ACT_SPEAK,
    ENTITY_ACT_SOUND,
    ENTITY_ACT_CREATE,
    ENTITY_ACT_MERGE,   // two entities merge
    ENTITY_ACT_SPLIT,  // one entity splits into two (or more)
};

/*struct G_ENTITY_EVENT_RESULTS {
    int returned_event;
    char *args;
    CEntity *entity;
};
*/

enum tEntityType { // Basic entity types
    ENTITY_INVISIBLE,
    ENTITY_STATIC,
    ENTITY_STATIC_ANIMATED,
    ENTITY_PLAYER,
    ENTITY_NPC,
    ENTITY_LIGHT,
    ENTITY_SOUND,
    ENTITY_AURA,
};

enum tEntityEvent { // Interaction events
    ENTITY_EVENT_NONE,
    ENTITY_EVENT_HOVER,
    ENTITY_EVENT_LEFT_CLICK,
    ENTITY_EVENT_RIGHT_CLICK,
    ENTITY_EVENT_DOUBLE_CLICK,
    ENTITY_EVENT_NEAR,
    ENTITY_EVENT_FAR,
    ENTITY_EVENT_LEAVE,
    ENTITY_EVENT_ENTER,
    ENTITY_EVENT_WAIT,
    ENTITY_EVENT_MOVE,
    ENTITY_EVENT_CREATE,
    ENTITY_EVENT_DESTROY,
    ENTITY_EVENT_TIMER_CYCLE,
    ENTITY_EVENT_USE,
    ENTITY_EVENT_SPEAK,
    ENTITY_EVENT_SOUND,
    ENTITY_EVENT_ATTACK,
    ENTITY_EVENT_RESET,
    ENTITY_EVENT_HEAL,
    ENTITY_EVENT_DIAGNOSE,
    ENTITY_EVENT_TARGET,
    ENTITY_EVENT_DEATH,
    ENTITY_EVENT_SPAWN,
    ENTITY_EVENT_RESURRECT,
};

class C_Entity {
public:
    C_Entity();
    C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX);
    C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX, CGLModel *pInModel);
    ~C_Entity();

    // Basic info
    char        name[1024];
    int         type;
    bool        hidden;

    // Attributes
    int         life_points;
    int         mana_points;
    int         power_points;
    int         rage_points;
    int         at_sta;
    int         at_int;
    int         at_spi;
    int         at_wis;
    int         at_agi;
    int         at_con;

    C_Entity    *pParent;
    C_Entity    *pNext;
    C_Entity    *pPrev;
    C_Entity    *pTargetEntity;

    // OpenGL Stuff
    CVector3    loc;        // position of the entity
    CVector3    rot;        // rotation of the entity
    CVector3    autorot;    // will continuously rotate based on this setting
    CVector3    scale;      // vector for the scale matrix
    CVector3    dir;        // direction vector (which way the entity is facing)
    CColor3     color;      // color of the entity;
    GLfloat     trans;      // use this for fading in and out, and for ghost/window effects
    GLint       glname;     // for picking
    CGLModel*   pModel;             // pointer to model data to use
    int         iModelAnim;         // The current index into pAnimations list (NEW)
    int         iModelAnimFrame;    // The current frame of the current animation (NEW)
    CGLTexture* pTexture;           // use this texture for rendering (overrides pModel texture)

    // Movement
    CWayPoint*  pFirstWayPoint;

    // Sound
    char        soundname[1024];    // sound filename
    float       sounddistance;      // how far away the sound can be heard
    bool        soundloop;
    float       soundvolume;

    bool        bMadeLog;
    CLog*       pLog; // pointer to log
    CGAF*       pGAF; // pointer to GAF
    C_GFX*      pGFX; // pointer to GFX

    /************************************************************************************************************************
    RESPAWN EXAMPLES:
    A power up would be:
    resource_min 1, resource_max 1, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 30000
    A mine node would be:
    resource_min 1, resource_max 6, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 30000
    A coin in a scrolling platform game:
    resource_min 1, resource_max 1, respawn_min 1, respawn_max 1, respawn_timer_min 0    , respawn_timer_max 0
    Node on random timer between 5 and 10 minutes:
    resource_min 3, resource_max_9, respawn_min 0, respawn_max 0, respawn_timer_min 30000, respawn_timer_max 60000
    *************************************************************************************************************************/
    int     resource_min;       // 0 = infinite resources
    int     resource_max;       // 0 = infinite resources
    int     respawn_min;        // 0 = infinite respawns
    int     respawn_max;        // 0 = infinite respawns
    long    respawn_time_min;   // 0 = default; default is 5 minutes (30000)
    long    respawn_time_max;   // 0 = default; default is 5 minutes (30000)
    /*************************************************************************************************************************/
    void    Initialize(void);
    void    Draw(void);
    bool    roam(void); //  brains of the entity, includes movement and other checking called once per cycle
    bool    push_event(C_Entity *rcv_entity, int event, char *args, C_Entity *action_entity);
    /*************************************************************************************************************************
       use: push_event(entity,event,event_args,entity_to_take_action_on (if 0 it will action self));
           example:
            current_entity->push_event(current_entity->target,G_ENTITY_ATTACK,"243",current_entity);
            target->exec_event(sending_entity,G_ENTITY_ATTACK,"243",sending_entity);
            target->on_attack("243",sending_entity);
    **************************************************************************************************************************/
    bool    exec_event(tEntityEvent event, char *args, C_Entity *action_entity);
    void    on_create(char *args,C_Entity *entity);
    void    on_destroy(char *args,C_Entity *entity);
    void    on_near(char *args,C_Entity *entity);
    void    on_far(char *args,C_Entity *entity);
    void    on_leave(char *args,C_Entity *entity);
    void    on_enter(char *args,C_Entity *entity);
    void    on_wait(char *args,C_Entity *entity);
    void    on_hover(char *args,C_Entity *entity);
    void    on_click(char *args, C_Entity *entity);
    void    on_double_click(char *args, C_Entity *entity);
    void    on_timer_cycle(char *args, C_Entity *entity);
    void    on_use(char *args, C_Entity *entity);
    void    on_speak(char *args, C_Entity *entity);
    void    on_sound(char *args, C_Entity *entity);
    void    on_attack(char *args, C_Entity *entity);
    void    on_target(char *args, C_Entity *entity);
    void    on_heal(char *args, C_Entity *entity);
    void    on_reset(char *args, C_Entity *entity);
    void    on_death(char *args, C_Entity *entity);

    bool    act_move(C_Entity *entity);
    bool    act_target(C_Entity *entity);
    bool    act_attack(C_Entity *entity);
    bool    act_consume(C_Entity *entity);
    bool    act_use(C_Entity *entity);
    bool    act_speak(C_Entity *entity);
    bool    act_sound(C_Entity *entity);
    bool    act_create(C_Entity *entity);
    bool    act_merge(C_Entity *entity);
    bool    act_split(C_Entity *entity);
    bool    act_path(C_Entity *entity);
    bool    act_diagnose(C_Entity *entity);
    bool    act_observe(C_Entity *entity);
    bool    act_listen(C_Entity *entity);
};


#endif
