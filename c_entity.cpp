#include "c_entity.h"
C_Entity::C_Entity() {
    Initialize();
}
C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX) {
    Initialize();
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
}
C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX, CGLModel *pInModel) {
    Initialize();
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
    pModel=pInModel;
}
C_Entity::~C_Entity() {
}
void C_Entity::Initialize(void) {
    pNext=0;
    pPrev=0;
    pLog=0;
    pGAF=0;
    pGFX=0;
    pModel=0;
    set_defaults();
}
void C_Entity::set_defaults() {
    strcpy(name,"unknown");
    type=ENTITY_INVISIBLE;
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
    respawn_time_min=ENTITY_DEFAULT_RESPAWN_TIME;   // 0 = default; default is 5 minutes (30000)
    respawn_time_max=ENTITY_DEFAULT_RESPAWN_TIME;   // 0 = default; default is 5 minutes (30000)
}
void C_Entity::Draw(void) {

    glTranslatef(Pos.x, Pos.y, Pos.z);  // location
    glRotatef(Rot.x,1.0f,0,0);
    glRotatef(Rot.y,0,1.0f,0);
    glRotatef(Rot.z,0,0,1.0f);       // rotation
    glScalef(Scale.x,Scale.y,Scale.z);  // scale
    glColor3f(1.0f,1.0f,1.0f);


    if(pModel) {

    }
    else {
        drawcube();
    }

/*  CVector3 Pos;       // position of the entity
    CVector3 Dir;       // direction vector (which way the entity is facing)
    CVector3 Scale;     // vector for the scale matrix
    CGLModel *pModel;   // pointer to model data to use
    CLog    *pLog; // pointer to log
    CGAF    *pGAF; // pointer to GAF
    C_GFX   *pGFX; // pointer to GFX    */
}
bool C_Entity::push_event(C_Entity *rcv_entity,int event,char *args,C_Entity *action_entity) {
    if(!rcv_entity) return false;
    if(!action_entity) action_entity=this;
    rcv_entity->exec_event(event,args,action_entity);
    return true;
}

bool C_Entity::exec_event(tEntityEvent event, char *args, C_Entity *action_entity) {
    if(action_entity==0) action_entity=this;
    switch(event) {
        case ENTITY_EVENT_NONE:
            return true;
        case ENTITY_EVENT_ATTACK:
            on_attack(args,action_entity);
            break;
        case ENTITY_EVENT_HEAL:
            on_heal(args,action_entity);
            break;
        case ENTITY_EVENT_TARGET:
            on_target(args,action_entity);
            break;
        case ENTITY_EVENT_DEATH:
            on_death(args,action_entity);
            break;
        default:
            break;
    }
    return true;
}

void C_Entity::on_death(char *args,C_Entity *entity) {
    if(entity==0) return;
    pLog->AddEntry(va("%s is slain by %s.",name,entity->name));
}

void C_Entity::on_target(char *args,C_Entity *entity) {
    if(entity==0) return;
    if(entity!=pTargetEntity) {
        pTargetEntity=entity;
        pLog->AddEntry(va("%s targetted %s...",name,entity->name));
    }
}

void C_Entity::on_attack(char *args,C_Entity *entity) {
    if(entity==0) return;
    int attack_dmg=atoi(args);
    if(life_points>0) {
        life_points-=attack_dmg;
        if(life_points<0) life_points=0;
        pLog->AddEntry(va("%s was attacked by %s for %d damage. %s now has %d life points.",name,entity->name,attack_dmg,name,life_points));
        if(life_points==0) {
            push_event(this,ENTITY_EVENT_DEATH,"0",entity);
        }
        else {
            push_event(this,ENTITY_EVENT_TARGET,"1",entity);
        }
    }
    else {
        pLog->AddEntry(va("%s is dead...",name));
    }
}

void C_Entity::on_heal(char *args,C_Entity *entity) {
    if(entity==0) return;
    int heal;
    heal=atoi(args);
    pLog->AddEntry(va("%s healed %s for %d...",entity->name,name,heal));
}
