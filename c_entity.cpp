#include "c_entity.h"

CEntity::CEntity() {
    set_defaults();
    Log=new CLog((const char *)"Entity.log");
}

CEntity::CEntity(const char *nm) {
    set_defaults();
    strcpy(name,nm);
    Log=new CLog(va("%s.log",name));
}

CEntity::~CEntity() {
    dlcsm_delete(Log);
}

void CEntity::set_defaults() {
    strcpy(name,"unknown");
    type=G_ENTITY_FRIENDLY;
    x=100;
    y=100;
    z=0;
    life_points=100;
    mana_points=100;
    power_points=100;
    rage_points=100;
    at_sta=15;
    at_int=15;
    at_spi=15;
    at_wis=15;
    at_agi=15;
    at_con=15;
    resource_min=0;       // 0 = infinite resources   
    resource_max=0;       // 0 = infinite resources
    respawn_min=0;        // 0 = infinite respawns
    respawn_max=0;        // 0 = infinite respawns
    respawn_time_min=30000;   // 0 = default; default is 5 minutes (30000)
    respawn_time_max=30000;   // 0 = default; default is 5 minutes (30000)
}

bool CEntity::push_event(CEntity *rcv_entity,int event,const char *args,CEntity *action_entity) {
    if(!rcv_entity) return false;
    if(!action_entity) action_entity=this;
    rcv_entity->exec_event(event,args,action_entity);
    return true;
}

bool CEntity::exec_event(int event, const char *args, CEntity *action_entity) {
    if(action_entity==0) action_entity=this;
    
    switch(event) {
        case G_ENTITY_NONE:
            return true;

        case G_ENTITY_ATTACK:
            on_attack(args,action_entity);
            break;

        case G_ENTITY_HEAL:
            on_heal(args,action_entity);
            break;

        case G_ENTITY_TARGET:
            on_target(args,action_entity);
            break;

        case G_ENTITY_DEATH:
            on_death(args,action_entity);
            break;

        default:
            break;
    }

    return true;
}

void CEntity::on_death(const char *args,CEntity *entity) {
    if(entity==0) return;
    zl(va("%s is slain by %s.",name,entity->name));
}

void CEntity::on_target(const char *args,CEntity *entity) {
    if(entity==0) return;
    if(entity!=target) {
        target=entity;
        zl(va("%s targetted %s...",name,entity->name));
    }
}

void CEntity::on_attack(const char *args,CEntity *entity) {
    if(entity==0) return;
    int attack_dmg=atoi(args);

    if(life_points>0) {
        life_points-=attack_dmg;
        if(life_points<0) life_points=0;

        zl(va("%s was attacked by %s for %d damage. %s now has %d life points.",name,entity->name,attack_dmg,name,life_points));
        
        if(life_points==0) {
            push_event(this,G_ENTITY_DEATH,(const char *)"0",entity);
        }
        else {
            push_event(this,G_ENTITY_TARGET,(const char *)"1",entity);
        }
    }
    else {
        zl(va("%s is dead...",name));
    }
}

void CEntity::on_heal(const char *args,CEntity *entity) {
    if(entity==0) return;
    int heal;
    heal=atoi(args);
    zl(va("%s healed %s for %d...",entity->name,name,heal));
}

