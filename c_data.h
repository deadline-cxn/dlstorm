/***************************************************************
 **      DATA CONTAINER CLASS                                 **
 ***************************************************************/

#ifndef _SC_DATA_CLASS
#define _SC_DATA_CLASS

#include "dlstorm.h"

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
    char name[32];
    bool savepw;
    char passwd[32];
    CFM_Profile *pNext;
};

class CC_Data {
public:
	CC_Data(void);
	~CC_Data(void);

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

	char			Name[24];
	char			Password[24];
	bool			bSavePassword;
	char			ServerMessage[1024];
	char			ServerName[1024];
	char			ServerID[15];           // Server / Mission Identifier
    char            szServerVersion[15];
	char			IPAddress[255];
	char			Port[10];
    char            MasterIPAddress[255];
    char            MasterPort[10];
	bool			bLog;
	bool			bDownload;

    bool            bAudioFailure;
    bool            bSound;					    	// Is sound system present?
    bool			 bMusic;					    	// Play Music? (MIDI only for now)
	float           fSoundVolume;
	float           fMusicVolume;
	char			MouseLeftButtonSound[1024];

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
	char			ServerAuthor[1024];
	int				ServerListOffset;
	int				ServerCharacterListOffset;

	char            session_id[64];

	u_char          Access;
	char			szAccessName[255];

    //unsigned long  dwKeyPressTimer;

    bool            bDrawMap;
    bool            bDrawMapObjects;
    bool            bBlockGlow;
    bool            bVertice;
    bool            bServerInfoUpdated[64];

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

    int            GAME_MODE;

};

#endif // _SC_DATA_CLASS
