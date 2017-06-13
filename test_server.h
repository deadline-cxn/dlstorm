/////////////////////////////////////////
#ifndef _MANTRA_SERVER_H
#define _MANTRA_SERVER_H
/////////////////////////////////////////

#include "dlstorm.h"
#include "c_net.h"
#include "c_log.h"
#include "c_timer.h"
#include "c_var.h"
// #include "c_sqlitedb.h"
// #include "mantra_version.h"
// #include "mantra_common.h"
// #include "manserver.client.h"
// #include "c_gui_chat_enum.h"
#include "s_gnu.h"
#include "net_messages.h"

#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again

/////////////////////////////////////////
#define NET_REVISION 1
#define TEST_S_BUILD 1
#define COPYRIGHT "(C) 2017 Seth Parson"
#define VERSION "v1.0"

#define DEFAULT_GAME_SERVER_IP   "localhost"
#define DEFAULT_GAME_SERVER_PORT 40404
#define DEFAULT_MASTER_SERVER_IP "localhost"
#define DEFAULT_MASTER_SERVER    40403

#define MANTRA_TEXT_LEN 128

typedef enum{
    NFILE_NOP=0,
    NFILE_ACK,
    NFILE_ABORT,
    NFILE_START,
    NFILE_START_OK,
    NFILE_DATA,
    NFILE_DATA_OK,
    NFILE_DATA_RESEND,
    NFILE_EOF,
    NFILE_EOF_RESEND,
    NFILE_RESUME,
    NFILE_RESUME_OK,
    NFILE_RESUME_CANT,
    NFILE_ERROR

} tfilexfer;
typedef enum{

    NM_NOMESSAGE = 1,
    NM_NOP,

    NM_FUNCTION,

    NM_LOGIN_REQUEST,
    NM_LOGIN_REQUEST_REPLY,
    NM_LOGOUT,

    NM_HEARTBEAT,
    NM_HEARTBEAT_REQUEST,

    NM_MESSAGE,

    NM_MOVEPLAYER,

    NM_CREATE_CHARACTER,
    NM_DELETE_CHARACTER,
    NM_MODIFY_CHARACTER,
    NM_RETRIEVECHARS,
    NM_RETRIEVECHARINFO,
    NM_GETINVENTORY,

    NM_CLIENT_SHUTDOWN,
    NM_OTHER_CLIENT_SHUTDOWN,

    NM_CHARACTER_STATUS,

    NM_FILE_XFER,
    NM_MANTRA_DOWNLOAD,
    NM_MANTRA_UPLOAD,

    NM_PING,

    NM_PROMPT,

    NM_GET_SERVER_INFO,
    NM_PUT_SERVER_INFO,
    NM_GET_SERVER_LIST,
    NM_MASTER_TO_GAME,

    NM_GUI,
    NM_GUI_CALLBACK,

} tNetmsg;

typedef enum{
    NF_CHAT_JOIN,
    NF_CHAT_LEAVE,
    NF_CHAT_WHO,

} tNetfunc;


typedef enum{

    MIT_EQUIP,
    MIT_CONSUMABLE,
    MIT_CRAFT,
    MIT_BOOK,

} tItemType;

typedef enum{ // CONSUMABLE SUBTYPES

    MIST_FOOD,
    MIST_DRINK,
    MIST_POTION,
    MIST_BUFFY,
    MIST_SKILL,   // skill increase
    MIST_RECIPE,  // skill learn new recipe
    MIST_REAGENT, // needed for actions

} tItemConsumableSubTypes;

typedef enum{ // CRAFT SUBTYPES

    MIST_HACKOLOGY, // technology stuff
    MIST_EMFOLOGY,  // environment stuff
    MIST_JACKOLOGY, // take random items and make stuff
    MIST_ANCIENTS,  // understanding of the ancients

} tItemCraftSubTypes;

typedef enum{ // EQUIP SUBTYPES

    MIST_HEAD,
    MIST_GLOVES,
    MIST_RING1,
    MIST_RING2,
    MIST_BRACER,
    MIST_CHEST,
    MIST_BELT,
    MIST_LEGS,
    MIST_FEET,
    MIST_TRINKET,
    MIST_NECK,
    MIST_MAIN_HAND,
    MIST_OFF_HAND,
    MIST_MAIN_OR_OFF_HAND,
    MIST_TWO_HAND,
    MIST_CHAKRA,

} tItemEquipSubTypes;

typedef enum{ // action types

    MACT_MODIFY_STAT,
    MACT_MODIFY_SKILL,
    MACT_ADD_EFFECT,
    MACT_REMOVE_EFFECT,
    MACT_CREATE_ITEM,
    MACT_CREATE_CURRENCY,
    MACT_TELEPORT,
    MACT_DAMAGE_SELF,
    MACT_DAMAGE_TARGET,
    MACT_DAMAGE_RADIUS,

} tActionTypes;

typedef enum{
    MEFT_MODIFY_STAT,
    MEFT_MODIFY_SKILL,


} tEffectTypes;

enum tGUINetworkMessages{
    FM_GUI_PROGRAM_FINISH=0,
    FM_GUI_STUMP_CREATE,
    FM_GUI_STUMP_UPDATE,
    FM_GUI_STUMP_REMOVE,
    FM_GUI_STUMP_CAPTION,
    FM_GUI_CONTROL_CREATE,
    FM_GUI_CONTROL_UPDATE,
    FM_GUI_CONTROL_REMOVE,
    FM_GUI_CONTROL_VALUE,
    FM_GUI_CLEAR_ALL,
    FM_GUI_CALL

    };

enum eCHAT_CHANNELS {
    CHANNEL_CLAN,
    CHANNEL_GENERAL,
    CHANNEL_TRADE,
    CHANNEL_SAY,
    CHANNEL_YELL,
    CHANNEL_LOCAL,
    CHANNEL_PARTY,
    CHANNEL_RAID,
    CHANNEL_SYSTEM,
    CHANNEL_WHISPER,
    CHANNEL_CUSTOM,
    CHANNEL_CONSOLE
};

/*
typedef struct RetainedData {
    char    s_name[MAX_TEXT_LEN];
    char    s_admin_email[MAX_TEXT_LEN];
    int     i_port;
    int     i_max_clients;
    bool    b_new_accounts;
    bool    b_require_website;
    char    s_website_link[MAX_TEXT_LEN];
    char    s_motd[MAX_TEXT_LEN];
};
*/

class CServer;


class C_GSC : public CCSocket {
public:
    C_GSC(CLog *pInLog,CServer *pInServer);
    ~C_GSC();

    void    load(void);
    void    save(void);

    CServer *pServer;   // parent server object
    void    db_update(string s_col,string s_val);

    CLog    *pLog;      // log object
	bool    bMadeLog;   // internal destruction flag

    bool    bDelete;        // flag for object destruction
    bool    bHeartBeatCheck;// heartbeat flag

    CTimer  *inactivity_timer;
    CTimer  *heartbeat_timer;

    C_GSC   *pNext;
    C_GSC   *pPrevious;

    // CMPCharacter *character;

    void    initc(void);

    void    do_script(char *file);
    void    do_func(char *func);
    void    do_net(void);

    // commands to push net messages to the actual client follow :
    bool    gui_clear(void);
    bool    gui_call(char *name);
    char    session_id[MAX_TEXT_LEN];
	u_char  access;         // server access (0-255)
    char    username[MAX_TEXT_LEN];
    char    password[MAX_TEXT_LEN];
    bool    timeout_override;
    void    init(void);
    bool    load_userdata(void);
    bool    save_userdata(void);
    vector <string> chat_channel;
    void    leave_channel(const char *channel);
    void    join_channel(const char *channel);

};


class CServer : public CCSocket {
public:
    CServer(bool bIsQuiet);
    ~CServer();

    /////////////////////////////////////////
    // flow stuff

    bool    bQuit;
    bool    bRestart;

    void    start_up(void);
    void    shut_down(void);
    void    cycle(void);
    bool    check_restart(void);

    /////////////////////////////////////////
    // console stuff

    bool    bQuiet;
    CLog    *pLog;
    void    Log(string s);
    void    Log(const char *fmt, ...);

    void    console_command(char *command);
    char    szConsoleInput[1024];
    int		ConsoleHistoryPosition;
    struct	ConHistory { char text[1024]; };
    ConHistory *ConsoleHistory;

    void    SetupConsoleHistory(void);
    void    RemoveConsoleHistory(void);
    void    AddToConsoleHistory(char *s);
    char    *GetConsoleHistory(int x);

    void    do_keyboard(void);

    /////////////////////////////////////////
    // unretained data

    long    start_time;

    /////////////////////////////////////////
    // retained data

    bool    load_data(void);
    bool    save_data(void);

    // RetainedData   r_data;
    CVarSet     *pVars;

    /////////////////////////////////////////
    // network / client stuff

    C_GSC   *pFirstPlayer;
    C_GSC   *get_client(const char *user_name);

    void    accept_connection(void);
    void    reject_connection(int iReSocket,sockaddr *pReAddr,const char *reason);
    void    send_all(CPacket *pPacket,float fBlockTime);
    void    disconnect(C_GSC *pClient, const char *reason);
    int     num_clients(void);
    void    purge_clients(void);

    void    do_world(void);
    void    save_world(void);
    void    load_world(void);

    void    kick_user(const char *name,const char *reason);
    void    kick_all(const char *reason);
    void    chat(C_GSC *pClient,const char *from,const char *msg, int iChannel);

    void    create_guid(const char * cin,char * cout);

    void    add_user(const char *in,u_char ia);
    void    remove_user(const char *szName);
    void    user_access(const char *in_user_name,int in_access);


    //void    ReportToMaster(void);
    //CPacket *pServerInfo;
    //void    SetServerInfo(CPacket *pPacket){ pServerInfo = pPacket; }
    //CPacket *pGetServerInfo(void){ return pServerInfo; } // serverinfo getter and setter
    //void    SetUpMessage(CPacket *pPacket); // configure the use of the master server
    //void    SetMaster(CMasterAddress *pMaster);
    //CMasterAddress *pGetMaster();
    //CMasterAddress *pMa;
    // char   szLastAddress[1024];
    //time_t  pServerTime;
    //struct  tm *tTime;

};
/////////////////////////////////////////
#endif//_MANTRA_SERVER_H
/////////////////////////////////////////
