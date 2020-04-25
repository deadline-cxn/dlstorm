/* Seth's Game Class  */

#ifndef _S_GAME_CLASS
#define _S_GAME_CLASS

#include "c_log.h"
#include "c_sdl.h"
// #include "c_fmod.h"
#include "c_timer.h"
#include "c_map.h"

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// Game play modes
#define PLAY            0
#define BUILD_GHOST     1
#define BUILD           2
#define SPECTATOR       3
#define SPECTATOR_LOGIN 4
#define CHATROOM        5
#define LOGIN           6

#define G_CORE        1 // Always will be on
#define G_SDL         2
#define G_FMOD        4
#define G_LUA         8
#define G_GAF         16
#define G_CONSOLE     32
#define G_ENTITY      64
#define G_DATA        128
#define G_LOG         256
#define G_SPRITE      512
#define G_SAMPLE      1024
#define G_FONT        2048
#define G_TIMER       4096
#define G_SDL2D       8192

#define SGC_ALL       65535

class CGame {
public:
    CGame();
    CGame(char *APP_NAME, Uint16 iFlags);
	CGame(char *APP_NAME, Uint32 iScreenWidth, Uint32 iScreenHeight, Uint32 iScreenColors, Uint16 iFlags);
    ~CGame();
    CLog        *Log;
    CSDL_Wrap   *SDL;
    // CFMOD       *SND;
    Uint16      flags;
	Uint32		screen_width;
	Uint32		screen_height;
	Uint32		screen_colors;
    bool        G_QUIT;
    int         G_KEYDOWN;
    bool Loop(void);
    void Start(char *APP_NAME,Uint16 iFlags);
    void ShutDown(void);
    bool UpdateGFXStart(void);
    bool DrawVortex(void);
    bool UpdateGFXEnd(void);
    bool UpdateInput(void);
    bool Inject(Uint16 TYPE,char *name);
    void UserInit(void);
    void UserGFX(void);
    void UserKeys(void);
};

#endif
