/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_SOUND_UTIL
#define _EMBER_SOUND_UTIL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#pragma comment(lib,"fmodvc")
#pragma comment(lib,"dsound")
#pragma comment(lib,"winmm")
#pragma comment(lib,"msacm32")
#endif

#include "dlstorm.h"
#include "fmod.h"

#define MAX_CHANNELS 32


class C_Sound {
public:
    C_Sound();
    ~C_Sound();

	char *FMODVersion(void);
    char InitializeSound();
    void ShutDownSound(void);
    char PlayMusic(char* szFilename);
    void StopMusic(void);
    void StopAudio(void);
    void StopSound(void);
    void SetMusicVolume(float f);
    void SetSoundVolume(float f);
    int  PlaySample(char* szFilename);

    FMUSIC_MODULE * fmusic;

    typedef struct
    samplelist {

    FSOUND_SAMPLE *sptr;
    };

    samplelist *sample;

    unsigned char svol;
    unsigned char mvol;
    bool bfmod;

};

#endif

