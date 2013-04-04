
#ifndef _C_EM_LUA
#define _C_EM_LUA

#ifdef __cplusplus
extern "C"
{
#endif

    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"

#ifdef __cplusplus
}
#endif


class C_LUA
{
public:

    C_LUA();
    ~C_LUA();

    lua_State* L;


    bool StartLUA(void);
    bool ShutDownLUA(void);

    int l_interpret(char *string);

    int *cprint(lua_State* L);


/*
    char *l_sys(char *var);
    char *l_axs(char *var);
    char *l_grp(char *var);

    char *access_group_name(int access);
    bool has_access(char *var,int access);

    char *l_client_prop(int worldindex,char *prop);
    char *l_cl_avatar_prop(int worldindex, char* prop);
    char *l_cl_cam_prop(int worldindex, char* prop);

    int l_world_getvacancy(void);
*/

};




#endif // _C_EM_LUA
