/* Seth's FMOD Wrapper */

#ifndef _FMOD_UTIL
#define _FMOD_UTIL

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include "c_log.h"
#include "fmod.h"

//#ifdef _WIN32
// #pragma comment(lib,"fmodvc.lib" )
//#pragma comment(lib,"dsound")
//#pragma comment(lib,"winmm")
//#pragma comment(lib,"msacm32")
//#endif

#define MAX_SND_CHANNELS 32

#define GAME_FX_LO  0  // 16 Channels for game Channels 0-15
#define GAME_FX_HI  15
#define MUSIC_CH_1  16  // 16 Channels for game Music 8-23 (DLM Music Format)
#define MUSIC_CH_2  17
#define MUSIC_CH_3  18
#define MUSIC_CH_4  19
#define MUSIC_CH_5  20
#define MUSIC_CH_6  21
#define MUSIC_CH_7  22
#define MUSIC_CH_8  23
#define MUSIC_CH_9  24
#define MUSIC_CH_10 25
#define MUSIC_CH_11 26
#define MUSIC_CH_12 27
#define MUSIC_CH_13 28
#define MUSIC_CH_14 29
#define MUSIC_CH_15 30
#define MUSIC_CH_16 31

class CSample {
   public:
    CSample();
    CSample(char *file);
    ~CSample();

    CSample *pNext;
    CSample *pPrev;

    FSOUND_SAMPLE *data;
    int            size;
    char           name[256];
    unsigned char  volume;
    int            max_simultaneous;

    void LoadCabSample(unsigned char *sdata, int size);
    void Play(int channel);
};

class CFMOD {
   public:
    CFMOD();
    ~CFMOD();

    char InitializeSound();
    void ShutDownSound(void);
    char PlayMusic(char *szFilename);
    void StopMusic(void);
    void StopSound(void);
    void StopAudio(void);
    void SetMusicVolume(float f);
    void SetSoundVolume(float f);
    int  PlaySample(char *fn);
    int  PlaySample(char *fn, int channel, int flags);
    int  PlaySampleLoop(char *szFilename);
    int  LoopSample(char *szFilename) { return PlaySampleLoop(szFilename); }
    int  LoopMemSample(unsigned char *sdata, int size);

    FMUSIC_MODULE *fmusic;

    int maxchannels;

    unsigned char svol;
    unsigned char mvol;

    bool bfmod;

    CSample *FirstSample;
    CSample *Sample;

    CLog *Log;

    bool LoadCabSamples(void);
    void FlushCabSamples(void);

    CSample *NewSample(char *sname);
    CSample *GetSample(char *sname);
    void     DeleteSample(char *sname);
    void     DeleteAllSamples(void);
};

#endif
