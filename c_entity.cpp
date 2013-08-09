/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_entity.cpp
 **   Class:        C_Entity
 **   Description:  Generic entity object for games
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_entity.h"
///////////////////////////// C_Entity class
C_Entity::C_Entity() {
    Initialize();
}
C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX) {
    Initialize();
    bMadeLog=false;
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
}
C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX, CGLModel *pInModel) {
    Initialize();
    bMadeLog=false;
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
    pModel=pInModel;
}
C_Entity::~C_Entity() {
    if(bMadeLog) dlcsm_delete(pLog);
    dlcsm_delete(pSelectTimer);
}
void C_Entity::Initialize(void) {
    name.clear();
    pSelectTimer=new CTimer(200);
    bMadeLog=true;
    pTexture=0;
    pLog=0;
    pGAF=0;
    pGFX=0;
    pModel=0;
    name="Unknown";
    type=ENTITY_INVISIBLE;
    resource_min=0;
    resource_max=0;
    respawn_min=0;
    respawn_max=0;
    respawn_time_min=ENTITY_DEFAULT_RESPAWN_TIME;
    respawn_time_max=ENTITY_DEFAULT_RESPAWN_TIME;
    loc.x=0;
    loc.y=0;
    loc.z=0;
    rot.x=0;
    rot.y=0;
    rot.z=0;
    autorot.x=0;
    autorot.y=0;
    autorot.z=0;
    scale.x=1.0f;
    scale.y=1.0f;
    scale.z=1.0f;
    color.r=1.0f;
    color.g=1.0f;
    color.b=1.0f;
    iModelAnim=0;
    iModelAnimFrame=0;
}
void C_Entity::DrawLight(void) {

     if(type==ENTITY_LIGHT) {
        // glLoadIdentity();
        glPushMatrix();
        // pGFX->pCamera->Go();
        GLfloat ambient[]   = { 0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat location[]  = { loc.x, loc.y, loc.z, 1.0f};
        GLfloat diffuse[]   = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat specular[]  = { 1.0f, 0.5f, 0.5f, 0.5f};
        glLightfv(GL_LIGHT0, GL_AMBIENT,    ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,    diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR,   specular);
        glLightfv(GL_LIGHT0, GL_POSITION,   location);
        //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,    1.5);
        //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,      0.5);
        //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION,   0.2);
        // GLfloat spot[]      = {autorot.x, autorot.y, autorot.z};
        GLfloat spot[]      = {0.0f, 0.0f, -132.0f};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 12.0f);
        glPopMatrix();
    }

}
void C_Entity::Draw(bool bSelecting) {

    glEnable(GL_TEXTURE_2D);

    if(!pGFX) return;
    switch(type) {
        case ENTITY_INVISIBLE:
            return;
        case ENTITY_STATIC:
        case ENTITY_STATIC_ANIMATED:
        case ENTITY_PLAYER:
        case ENTITY_NPC:
        case ENTITY_NPC_SPAWN:
        case ENTITY_NPC_GENERATOR:
            break;

        case ENTITY_PLAYER_SPAWN:
        case ENTITY_SOUND:
        case ENTITY_AURA:
            rot.x=0;
            rot.y=0;
            rot.z=0;
            autorot.x=0;
            autorot.y=0;
            autorot.z=0;
            scale.x=0.5f;
            scale.y=0.5f;
            scale.z=0.5f;
            color.r=1.0f;
            color.g=1.0f;
            color.b=1.0f;
            pModel=0;
            if(!bSelecting) return;
            break;

        case ENTITY_LIGHT:
            pTexture=pGFX->GetTexture("base/ntt.light.png");
            scale.x=0.5f;
            scale.y=0.5f;
            scale.z=0.5f;
            pModel=0;
            if(!bSelecting) return;
            break;

        default:
            break;
    }


    rot.x+=autorot.x;
    rot.y+=autorot.y;
    rot.z+=autorot.z;

    glPushMatrix();

    glTranslatef(   loc.x,
                    loc.y,
                    loc.z); // location

    glRotatef(      rot.x,1.0f,0,0);
    glRotatef(      rot.y,0,1.0f,0);
    glRotatef(      rot.z,0,0,1.0f); // rotation

    glScalef(       scale.x,
                    scale.y,
                    scale.z);  // scale

    glColor3f( color.r, color.g, color.b); // color

    if(bSelecting){
        if(bSelected) {
            if(pSelectTimer->Up()) {
                if(bSelectMode) bSelectMode=false;
                else bSelectMode=true;
            }
            if(bSelectMode)
                glColor3f(1.0f,1.0f,0.0f);
        }
    }

    bool bFoundTex=false;
    CGLTexture *pTex;
    pTex=pTexture;
    if(pTex) if(pTex->glBmap) bFoundTex=true;
    if(!bFoundTex) {
        pTex=pGFX->pDefaultTexture;
        if(pTex) if(pTex->glBmap) bFoundTex=true;
    }


   if(pModel) pModel->Draw(0);

    else {
        if(pTex) if(pTex->glBmap) glBindTexture(GL_TEXTURE_2D, pTex->glBmap);
        pGFX->DrawCube();
    }

    glPopMatrix();

}
void C_Entity::Save() {
/*  dlcsm_make_filename(filename);
    ifstream myFile ("data.bin", ios::out | ios::binary);
    if(myFile) {

/*  char        name[TEXTNAME_SIZE];
    int         type;
    bool        hidden;
    bool        bSelected;
    CVector3    loc;        // position of the entity
    CVector3    rot;        // rotation of the entity
    CVector3    autorot;    // will continuously rotate based on this setting
    CVector3    scale;      // vector for the scale matrix
    CVector3    dir;        // direction vector (which way the entity is facing)
    CColor3     color;      // color of the entity;
    GLfloat     trans;      // use this for fading in and out, and for ghost/window effects
    if(pModel) pModel->name
    if(pTexture) pTexture->name

    char        soundname[FILENAME_SIZE];    // sound filename
    float       sounddistance;      // how far away the sound can be heard
    bool        soundloop;
    float       soundvolume;
    int     resource_min;       // 0 = infinite resources
    int     resource_max;       // 0 = infinite resources
    int     respawn_min;        // 0 = infinite respawns
    int     respawn_max;        // 0 = infinite respawns
    long    respawn_time_min;   // 0 = default; default is 5 minutes (30000)
    long    respawn_time_max;   // 0 = de
    CWayPoint*  pFirstWayPoint; // leave out waypoints for now
    */

/*
        myFile.close();
    }
    */
}
bool C_Entity::push_event(C_Entity *rcv_entity,int event,string args,C_Entity *action_entity) {
    if(!rcv_entity) return false;
    if(!action_entity) action_entity=this;
    rcv_entity->exec_event(event,args,action_entity);
    return true;
}
bool C_Entity::exec_event(tEntityEvent event, string args, C_Entity *action_entity) {
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
void C_Entity::on_death(string args,C_Entity *entity) {
    if(entity==0) return;
    pLog->AddEntry(va("%s is slain by %s.",name.c_str(),entity->name.c_str()));
}
void C_Entity::on_target(string args,C_Entity *entity) {
    if(entity==0) return;
    if(entity!=pTargetEntity) {
        pTargetEntity=entity;
        pLog->AddEntry(va("%s targetted %s...",name.c_str(),entity->name.c_str()));
    }
}
void C_Entity::on_attack(string args,C_Entity *entity) {
    if(entity==0) return;
/*  int attack_dmg=atoi(args);
    if(life_points>0) {
        life_points-=attack_dmg;
        if(life_points<0) life_points=0;
        pLog->AddEntry(va("%s was attacked by %s for %d damage. %s now has %d life points.",name,entity->name,attack_dmg,name,life_points));
        if(life_points==0) {
            push_event(this,ENTITY_EVENT_DEATH,"0",entity);
        } else {
            push_event(this,ENTITY_EVENT_TARGET,"1",entity);
        }
    } else {
        pLog->AddEntry(va("%s is dead...",name));
    }
    */
}
void C_Entity::on_heal(string args,C_Entity *entity) {
    if(entity==0) return;
    int heal;
    heal=atoi(args.c_str());
    pLog->AddEntry(va("%s healed %s for %d...",entity->name.c_str(),name.c_str(),heal));

}


