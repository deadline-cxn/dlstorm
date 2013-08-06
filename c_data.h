/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef _SC_DATA_CLASS
#define _SC_DATA_CLASS

#include "dlstorm.h"
#include "c_log.h"

// Game play modes

#define PLAY            0
#define BUILD_GHOST     1
#define BUILD           2
#define SPECTATOR       3
#define SPECTATOR_LOGIN 4
#define CHATROOM        5
#define LOGIN           6

class CFM_Profile {
public:
    CFM_Profile();
    ~CFM_Profile();
    char name[TINYNAME_SIZE];
    bool savepw;
    char passwd[TINYNAME_SIZE];
    CFM_Profile *pNext;
};

class CC_Data {
public:
    CC_Data(void);
    CC_Data(CLog* pInLog);
    CC_Data(char* filename);
    CC_Data(char* filename,CLog *pLog);
    ~CC_Data(void);

    void Initialize(void);

    bool bLoad(void);
    bool bSave(void);

    void CleanUp(void);
    void SetToDefaults(void);

    void ClearFavoriteServers(void);

    void ClearProfiles(void);

    bool LoadFavoriteServers(void);
    bool SaveFavoriteServers(void);
    bool DiscardFavoriteServer(int i);
    //bool AddFavoriteServer(ServerData *pData);

    bool LoadProfiles(void);
    bool SaveProfiles(void);
    bool DiscardProfile(void);
    bool AddProfile(void);

    ///////////////////////////////////////////////////////////////
    char            filename[FILENAME_SIZE];
    char			Name[TINYNAME_SIZE];
    char			Password[TINYNAME_SIZE];
    bool			bSavePassword;
    char			ServerMessage[FILENAME_SIZE];
    char			ServerName[FILENAME_SIZE];
    char			ServerID[TINYNAME_SIZE];           // Server / Mission Identifier
    char            szServerVersion[TINYNAME_SIZE];
    char			IPAddress[TEXTNAME_SIZE];
    char			Port[TINYNAME_SIZE];
    char            MasterIPAddress[TEXTNAME_SIZE];
    char            MasterPort[TINYNAME_SIZE];
    bool			bLog;
    bool			bDownload;

    bool            bAudioFailure;
    bool            bSound;					    	// Is sound system present?
    bool			 bMusic;					    	// Play Music? (MIDI only for now)
    float           fSoundVolume;
    float           fMusicVolume;
    char			MouseLeftButtonSound[FILENAME_SIZE];

    bool            bFullScreen;
    int             ScreenWidth;
    int             ScreenHeight;
    int             ScreenColors;

    bool            drawoptions;

    ///////////////////////////////////////////////////////////////
    // Discardable variables:

    char            cDebug;                         // Debug level 1 = onscreen messages only 2 = log
    bool            bShowPING;			    		// On or Off show PING
    int				SelectedServer;			    	// For server selection
    char			CharacterSlots;			    	// Character slots on selected server
    u_char          SelectedCharacterSlot;	    	// For choosing your character when logging on or creating a character
    char			currentsample;				    // Which sample is positioned
    char			ServerAuthor[FILENAME_SIZE];
    int				ServerListOffset;
    int				ServerCharacterListOffset;

    char            session_id[TEXTNAME_SIZE];

    u_char          Access;
    char			szAccessName[TEXTNAME_SIZE];

    //unsigned long  dwKeyPressTimer;

    bool            bDrawMap;
    bool            bDrawMapObjects;
    bool            bBlockGlow;
    bool            bVertice;
    bool            bServerInfoUpdated[TEXTNAME_SIZE];

    CFM_Profile     *Profile;
    CFM_Profile     *FirstProfile;

//  ServerData      *FavoriteServer;
//  ServerData      *FirstFavoriteServer;
//	C_ServerInfo    *ServerInfo;

    float		    x;
    float		    y;
    float		    z;

    float           fCredscroll;
    long            dwScrollTime;

    bool            bShowOptions;
    bool            bTargeting;
    long            dwRetroTimer;
    char            cRetroMode;

    bool            bCreatedLog;
    CLog            *pLog;

    int            GAME_MODE;

};

#endif // _SC_DATA_CLASS
