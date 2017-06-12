
#include "c_lua.h"
/*

int *cfprint(lua_State* L)
{
    return 1;
}

C_LUA::C_LUA()
{

    L=lua_open();
    luaopen_base(L);
    luaopen_table(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);

    lua_register( (lua_State*) L, "cprint"  , cfprint              );


}


C_LUA::~C_LUA()
{
    if(L) lua_close( L );
}

///////////////////////////////////////////////////////////////
int C_LUA::l_interpret(char * string)
{
    return luaL_dostring(L, string);
}


//bool StartLUA(void)
//{

    //if(NULL == L) return 0; //{ Log("Error Initializing lua\n"); return 0; }
    //Log("%s (started)",LUA_VERSION);
   // Log(LUA_COPYRIGHT);
    //Log(LUA_AUTHORS);
   // Log("АБВллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллВБА");

    ///////////////////////////////////////////////
    // initialize lua standard library functions



    ///////////////////////////////////////////////
    // Register LUA Interface

    lua_register( L, "fm_log"           , l_fm_log              );
    lua_register( L, "fm_kick"          , l_fm_kick             );
    lua_register( L, "fm_printc"        , l_fm_printc           );
    lua_register( L, "fm_prints"        , l_fm_prints           );
    lua_register( L, "fm_kill"          , l_fm_kill             );
    lua_register( L, "fm_restart"       , l_fm_restart          );
    lua_register( L, "fm_get_os"        , l_fm_get_os           );
    lua_register( L, "fm_get_sid"       , l_fm_get_sid          );
    lua_register( L, "fm_mkdir"         , l_fm_mkdir            );
    lua_register( L, "fm_sync_gui"      , l_fm_sync_gui         );
    lua_register( L, "fm_sync_cam"      , l_fm_sync_cam         );
    lua_register( L, "fm_sync_vis"      , l_fm_sync_vis         );
    lua_register( L, "fm_sync_client"   , l_fm_sync_client      );
    lua_register( L, "fm_sync_cllimbo"  , l_fm_sync_client_limbo_state);

    lua_register( L, "fmap_set_tile"    , l_fmap_set_tile       );
    lua_register( L, "fmap_set_object"  , l_fmap_set_object     );
    lua_register( L, "fmap_set_vwidth"  , l_fmap_set_vwidth     );
    lua_register( L, "fmap_set_vheight" , l_fmap_set_vheight    );
    lua_register( L, "fmap_set_vlight"  , l_fmap_set_vlight     );
    lua_register( L, "fmap_set_prop"    , l_fmap_set_prop       );


    ///////////////////////////////////////////////
    // Execute startup scripts

//    ExecScript("base",0);
  //  ExecScript("system",0);
  //  ExecScript("game",0);
   // ExecScript("startup",0);

    //return 1;
//}
///////////////////////////////////////////////////////////////

//int l_fm_get_sid(lua_State * L)
//{
//    lua_pushnumber(L,GetSerialNumber(0));
 //   return 1;
//}

///////////////////////////////////////////////////////////////
char *l_sys(char *var)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "system");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,var);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,2);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
bool has_access(char *var,int access)
{
    if(atoi(l_axs(var))<=access)
        return 1;
    return 0;
}
///////////////////////////////////////////////////////////////
char *l_axs(char *var)
{
    int index;
    char t[1024];
    strcpy(t,"0");
    lua_getglobal(L, "access");
	if(!lua_istable(L, -1)) return strdup(t);
    index = lua_gettop(L);
    lua_pushstring(L,var);
    lua_gettable(L,index);
    if(!lua_tostring(L,-1)) { lua_pop(L,1); return strdup(t); }
    strcpy(t,lua_tostring(L,-1));
    lua_pop(L,2);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *access_group_name(int access)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "access_group");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"name");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,access);
    lua_gettable(L,index);
    strcpy(t,lua_tostring(L,-1));
    lua_pop(L,2);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *l_grp(char *var)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "access_group");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,var);
    lua_gettable(L,index);
    strcpy(t,lua_tostring(L,-1));
    lua_pop(L,1);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
int l_world_getvacancy(void)
{
    int x=0;
    lua_pushstring(L,"getvacancy");
    lua_gettable(L,LUA_GLOBALSINDEX);
    lua_call(L,0,1);
    if(lua_isnumber(L,-1))
    x=(int)lua_tonumber(L,-1);
    lua_pop(L,1); return x;
}
///////////////////////////////////////////////////////////////
char *l_client_prop(int worldindex,char *prop)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "world");
    if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,worldindex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,3);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *l_cl_avatar_prop(int worldindex, char* prop)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "world");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,worldindex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"avatar");
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,4);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *l_cl_cam_prop(int worldindex, char* prop)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "world");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,worldindex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"cam");
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,4);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
int l_fm_get_os(lua_State * L) { lua_pushstring(L,FM_GetOSVersion()); return 1; }
///////////////////////////////////////////////////////////////
int l_fm_mkdir(lua_State * L) { if(!lua_tostring(L,-1)) return 1; FM_MkDir((char*)lua_tostring(L,-1)); return 1; }
///////////////////////////////////////////////////////////////
int l_fm_log(lua_State* L) { if(!lua_tostring(L, -1)) return 1; Log((char*)lua_tostring(L, -1)); return 1; }
///////////////////////////////////////////////////////////////
int l_fm_printg(lua_State* L) { return 1; }
///////////////////////////////////////////////////////////////
int l_fm_kill(lua_State* L) { bQuit=1; return 0; }
///////////////////////////////////////////////////////////////
int l_fm_restart(lua_State* L) { bRestart=1; return 0; }
///////////////////////////////////////////////////////////////
int l_fm_motd(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_user_del(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_user_add(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_user_get(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_user_put(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_user_touch(lua_State* L);
///////////////////////////////////////////////////////////////
int l_fm_printc(lua_State* L)
{
    if(!lua_tostring(L,1)) return 1;
    if(!lua_tostring(L,2)) return 1;
    C_FM_CLIENT *user=GetClient((char *)lua_tostring(L,1));
    if(user) Announce(user,(char *)lua_tostring(L,2),0,0,0);
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fm_prints(lua_State* L)
{
    if(!lua_tostring(L,1)) return 1;
    AdminAnnounce((char *)lua_tostring(L,1),0,0,0);
    return 1;
}
///////////////////////////////////////////////////////////////
char *l_fm_get_gui_prop(int windex,char *prop)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "world");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,windex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"gui");
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,4);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *l_fm_get_gui_stump_prop(int windex, int stump, char *prop)
{
    int index;
    char t[1024]= {0};
    strcpy(t,"0");
    lua_getglobal(L, "world");
	if(!lua_istable(L, -1)) return strdup("err!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,windex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"gui");
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"stump");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,4); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,stump);
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,5); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
    if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    lua_pop(L,6);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
char *l_fm_get_gui_stump_control_prop(int windex,int stump,int control, char *prop)
{
    int index;
    char t[1024]; memset(t,0,1024);
    strcpy(t,"0");
    lua_getglobal(L, "world");
	if(!lua_istable(L,-1)) return strdup("fatal world is nil!");
    index = lua_gettop(L);
    lua_pushstring(L,"client");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,1); return strdup("fatal client is nil!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,windex);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"gui");
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"stump");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,4); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,stump);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,"control");
    lua_gettable(L,index);
    if(!lua_istable(L,-1)) { lua_pop(L,6); return strdup("err!"); }
    index = lua_gettop(L);
    lua_pushnumber(L,control);
    lua_gettable(L,index);
    index = lua_gettop(L);
    lua_pushstring(L,prop);
    lua_gettable(L,index);
    if(lua_isnil(L,-1)) strcpy(t,"nil");
    else
    {
        if(lua_isnumber(L,-1)) sprintf(t,"%d",lua_tonumber(L,-1));
        if(lua_isstring(L,-1)) strcpy(t,lua_tostring(L,-1));
    }
    lua_pop(L,9);
    return strdup(t);
}
///////////////////////////////////////////////////////////////
int l_fm_sync_gui(lua_State* L)
{
    if(!lua_tostring(L,1)) return 1; C_FM_CLIENT *client=GetClient((char *)lua_tostring(L,1)); if(!client) return 0;
    // go through current gui and transmit the gui information to the client
    client->ClearGUI();
    int i,j;
    int s=atoi(l_fm_get_gui_prop(client->windex,"total_stumps"));
    int c=0;
    for(i=0;i<s;i++)
    {
        client->AddGUIStump(         l_fm_get_gui_stump_prop(client->windex,i,"name"),
                                atoi(l_fm_get_gui_stump_prop(client->windex,i,"x")),
                                atoi(l_fm_get_gui_stump_prop(client->windex,i,"y")),
                                atoi(l_fm_get_gui_stump_prop(client->windex,i,"width")),
                                atoi(l_fm_get_gui_stump_prop(client->windex,i,"height")),
                                atoi(l_fm_get_gui_stump_prop(client->windex,i,"props")),
                                l_fm_get_gui_stump_prop(client->windex,i,"media")
                                );

        client->GUIStumpCaption(     l_fm_get_gui_stump_prop(client->windex,i,"name"),
                                     l_fm_get_gui_stump_prop(client->windex,i,"caption") );
        c=atoi(l_fm_get_gui_stump_prop(client->windex,i,"total_controls"));
        for(j=0;j<c;j++)
        {

             // (char *stump,char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value)

            client->AddGUIControl(      l_fm_get_gui_stump_prop(client->windex,i,"name"),
                                        l_fm_get_gui_stump_control_prop(client->windex,i,j,"name"),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"type")),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"x")),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"y")),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"width")),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"height")),
                                        atoi(l_fm_get_gui_stump_control_prop(client->windex,i,j,"props")),
                                        l_fm_get_gui_stump_control_prop(client->windex,i,j,"media"),
                                        l_fm_get_gui_stump_control_prop(client->windex,i,j,"value")

                                        );
            // Log("Added a control... [%s]",l_fm_get_gui_stump_control_prop(client->windex,i,j,"name"));

            //client->GUIControlValue(    l_fm_get_gui_stump_prop(client->windex,i,"name"),
            //                            l_fm_get_gui_stump_control_prop(client->windex,i,j,"name"),
            //                            l_fm_get_gui_stump_control_prop(client->windex,i,j,"value") );
        }
    }
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fm_kick(lua_State* L)
{
    if(!lua_tostring(L,1)) return 1;
    if(!lua_tostring(L,2)) Kick((char *)lua_tostring(L,1),"Kicked by Admin :P");
    else                   Kick((char *)lua_tostring(L,1),(char *)lua_tostring(L,2));
    return 1;
}
int l_fm_sync_client_limbo_state(lua_State *L)
{
    if(!lua_tostring(L,1)) return 1;
    C_FM_CLIENT *client=GetClient((char *)lua_tostring(L,1));
    if(!client) return 0;

    client->SetDrawMap(atoi(l_client_prop(client->windex,"drawmap")));
    client->SetLimboState(atoi(l_client_prop(client->windex,"in_limbo")));

    return 1;
}
///////////////////////////////////////////////////////////////
int l_fm_sync_client(lua_State *L)
{
    if(!lua_tostring(L,1)) return 1;
    C_FM_CLIENT *client=GetClient((char *)lua_tostring(L,1));
    if(!client) return 0;

    client->SetGameMode(atoi(l_client_prop(client->windex,"gamemode")));
    client->SetDayLight(atof(l_client_prop(client->windex,"daylight_r")),
                        atof(l_client_prop(client->windex,"daylight_g")),
                        atof(l_client_prop(client->windex,"daylight_b")));
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fm_sync_cam(lua_State* L)
{
    if(!lua_tostring(L,1)) return 1;
    C_FM_CLIENT *client=GetClient((char *)lua_tostring(L,1));
    if(!client) return 0;

    client->SetCam(     atoi(l_cl_cam_prop(client->windex,"x")),
                        atoi(l_cl_cam_prop(client->windex,"y")),
                        atoi(l_cl_cam_prop(client->windex,"z")),
                        atoi(l_cl_cam_prop(client->windex,"ax")),
                        atoi(l_cl_cam_prop(client->windex,"ay")),
                        atoi(l_cl_cam_prop(client->windex,"az")),
                        atoi(l_cl_cam_prop(client->windex,"ox")),
                        atoi(l_cl_cam_prop(client->windex,"oy")),
                        atoi(l_cl_cam_prop(client->windex,"oz")) );
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fm_sync_vis(lua_State* L)
{
    int index;

    char name[64]; memset(name,0,64);
    char media[1024]; memset(media,0,1024);
    int x,y,z;
    int sid;
    int head,dir;
    int media_type;
    bool remove;

    // fm_sync_vis(client_name,vis_object);
    if(!lua_isstring(L,1)) return 1;
    if(!lua_istable(L,2))  return 2;

    C_FM_CLIENT *client=GetClient((char *)lua_tostring(L,1));
    if(!client) return 0;

    index = lua_gettop(L)-1;

    lua_pushstring(L,"name");
    lua_gettable(L,index);
    strcpy(name,lua_tostring(L,-1));
    lua_pop(L,1);

    lua_pushstring(L,"x");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) x=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) x=atoi(lua_tostring(L,-1));
    else x=0;

    lua_pop(L,1);

    lua_pushstring(L,"y");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) y=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) y=atoi(lua_tostring(L,-1));
    else y=0;

    lua_pop(L,1);

    lua_pushstring(L,"z");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) z=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) z=atoi(lua_tostring(L,-1));
    else z=0;

    lua_pop(L,1);

    lua_pushstring(L,"heading");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) head=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) head=atoi(lua_tostring(L,-1));
    else head=0;

    lua_pop(L,1);

    lua_pushstring(L,"direction");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) dir=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) dir=atoi(lua_tostring(L,-1));
    else dir=0;

    lua_pop(L,1);

    lua_pushstring(L,"media");
    lua_gettable(L,index);

            if(lua_isstring(L,-1)) strcpy(media,lua_tostring(L,-1));
    else    if(lua_isnumber(L,-1)) strcpy(media,va("%d",lua_tonumber(L,-1)));
    else strcpy(media,"0000");
    lua_pop(L,1);

    lua_pushstring(L,"media_type");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) media_type=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) media_type=atoi(lua_tostring(L,-1));
    else media_type=0;

    lua_pop(L,1);

    lua_pushstring(L,"sid");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) sid=(int)lua_tonumber(L,-1);
    else    if(lua_isstring(L,-1)) sid=atoi(lua_tostring(L,-1));
    else sid=0;

    lua_pop(L,1);

    lua_pushstring(L,"remove");
    lua_gettable(L,index);

            if(lua_isnumber(L,-1)) remove=TOBOOL(lua_tonumber(L,-1));
    else    if(lua_isstring(L,-1)) remove=TOBOOL(atoi(lua_tostring(L,-1)));
    else remove=0;

    lua_pop(L,1);

    //Log("VIS:[%d][%s] @ (%d,%d,%d) media[%s] media_type=[%d] heading[%d] direction[%d]",sid,name,x,y,z,media,media_type,head,dir);

    C_FM_CLIENT *oclient=fmgs->pFirstPlayer;
    if(remove)
    {
        while(oclient!=NULL)
        {
            oclient->VisRemove(sid);
            oclient=oclient->pNext;
        }
    }
    else
    {
        while(oclient!=NULL)
        {

            //if( ( atoi(l_cl_avatar_prop(oclient->windex,"x")) >= (x-40) ) &&
             //   ( atoi(l_cl_avatar_prop(oclient->windex,"y")) >= (y-40) ) &&
              //  ( atoi(l_cl_avatar_prop(oclient->windex,"z")) == (z)    ) &&
               // ( atoi(l_cl_avatar_prop(oclient->windex,"x")) <= (x+40) ) &&
               // ( atoi(l_cl_avatar_prop(oclient->windex,"y")) <= (y+40) ) )

            oclient->VisMove(sid,x,y,z,media,media_type,head,dir);
            oclient=oclient->pNext;
        }
    }

    lua_pop(L,1);

    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_tile(lua_State* L)
{
    if(!lua_tonumber(L,1)) return 1;
    if(!lua_tonumber(L,2)) return 1;
    if(!lua_tonumber(L,3)) return 1;
    if(!lua_tonumber(L,4)) return 1;
    if(!lua_tonumber(L,5)) return 1;


    SetMapTile( lua_tonumber(L,1),
                lua_tonumber(L,2),
                lua_tonumber(L,3),
                lua_tonumber(L,4),
                lua_tonumber(L,5) );

    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_object(lua_State* L)
{
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_vwidth(lua_State* L)
{
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_vheight(lua_State* L)
{
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_vlight(lua_State* L)
{
    return 1;
}
///////////////////////////////////////////////////////////////
int l_fmap_set_prop(lua_State* L)
{
    return 1;
}
///////////////////////////////////////////////////////////////
*/


