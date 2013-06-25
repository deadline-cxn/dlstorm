/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_SOUND_UTIL
#define _EMBER_SOUND_UTIL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlstorm.h"

#ifdef _WIN32
#pragma comment(lib,"fmodvc")
#pragma comment(lib,"dsound")
#pragma comment(lib,"winmm")
#pragma comment(lib,"msacm32")
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    char InitializeSound();
    void ShutDownSound(void);
    char PlayMusic(char* szFilename);
    void StopMusic(void);
    void StopAudio(void);
    void SetMusicVolume(float f);
    void SetSoundVolume(float f);
    int  PlaySample(char* szFilename);
    int  PlayMemSample(unsigned char *sdata,int size);
    int  PlaySampleLoop(char* szFilename);
    int  LoopSample(char *szFilename);
    int  LoopMemSample(unsigned char *sdata,int size);
#ifdef __cplusplus
}
#endif


#endif

