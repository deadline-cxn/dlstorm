/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_snd.h
 **   Class:        C_Sound
 **   Description:  FMOD wrapper class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_C_SOUND
#define _DLCS_C_SOUND

#ifdef DLCS_WINDOWS

#include "c_log.h"
#include "dlstorm.h"
#include "fmod.h"
#define MAX_CHANNELS 32
//////////////////////////////////// C_Sound class
class C_Sound {
   public:
    C_Sound();
    C_Sound(CLog* pLogIn);
    ~C_Sound();
    void           Log(string x);
    char*          FMODVersion(void);
    char           InitializeSound();
    void           ShutDownSound(void);
    char           PlayMusic(char* szFilename);
    void           StopMusic(void);
    void           StopAudio(void);
    void           StopSound(void);
    void           SetMusicVolume(float f);
    void           SetSoundVolume(float f);
    int            PlaySample(char* szFilename);
    FMUSIC_MODULE* fmusic;
    typedef struct samplelist {
        FSOUND_SAMPLE* sptr;
    };
    samplelist*   sample;
    unsigned char svol;
    unsigned char mvol;
    bool          bfmod;
    bool          bConsoleMode;
    CLog*         pLog;
    bool          bLogCreated;
};

#endif  // DLCS_WINDOWS
#endif  // _DLCS_C_SOUND
