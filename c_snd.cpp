/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_snd.cpp
 **   Class:        C_Sound
 **   Description:  FMOD wrapper class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
 #ifdef DLCS_WINDOWS

#include "c_snd.h"
C_Sound::C_Sound() {
    pLog=0;
    bLogCreated=false;
    InitializeSound();
}
C_Sound::C_Sound(CLog* pInLog) {
    pLog=pInLog;
    bLogCreated=false;
    InitializeSound();
}
C_Sound::~C_Sound() {
    ShutDownSound();
    if(bLogCreated) dlcsm_delete(pLog);
}
char C_Sound::InitializeSound() {
    fmusic=0;
    svol=255;
    mvol=64;
    char x;
    x = FSOUND_Init(44100, MAX_CHANNELS, 0);
    if(x) {
        bfmod=1;
        //MAX_CHANNELS = 6;
        //FSOUND_GetMaxChannels();
        //Log("Max channels:[%d]",MAX_CHANNELS);
        sample = new samplelist[MAX_CHANNELS];
        for(int i=0; i<MAX_CHANNELS; i++) {
            sample[i].sptr=0;
            //DLog("sample[%d].sptr=%d",i,sample[i].sptr);
        }
        //DLog("What");
    } else {

        Log("FMOD init failure...");
        bfmod=0;
    }
    if(x) {
        Log("FMOD initialized ");
        Log(FMODVersion());

        return x;
    }
}

char *C_Sound::FMODVersion() {
    if(!bfmod) return strdup("(fmod uninitialized)");
    return strdup(va("%.2f",FSOUND_GetVersion() ));
}
void C_Sound::ShutDownSound(void) {
    if(!bfmod) return;
    StopAudio();
    FSOUND_Close();
    delete [] sample;
}

int C_Sound::PlaySample(char* szFilename) {
    static int channel=6;
    int what;
    if(!bfmod) return 0;

    if(sample[channel].sptr) {
        FSOUND_Sample_Free(sample[channel].sptr);
        sample[channel].sptr;
    }

    sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,szFilename,FSOUND_LOOP_OFF,0,0);

    if(sample[channel].sptr) {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        //DLog("channel %d [%s]",channel,szFilename);
        channel++;
        if (channel>=MAX_CHANNELS) channel=6;
        return what;
    }
    return 0;
}

char C_Sound::PlayMusic(char* szFilename) {
    if(!bfmod) return 0;
    StopMusic();
    fmusic=FMUSIC_LoadSong(szFilename);
    if(fmusic) {
        FMUSIC_SetMasterVolume(fmusic,mvol);
        return FMUSIC_PlaySong(fmusic);
    }
    return 0;
}

void C_Sound::StopMusic(void) {
    if(!bfmod) return;
    if(fmusic) {
        FMUSIC_StopSong(fmusic);
        FMUSIC_FreeSong(fmusic);
    }
    fmusic=0;

}

void C_Sound::StopSound(void) {
    if(!bfmod) return;
    for(int i=0; i<MAX_CHANNELS; i++) {
        FSOUND_StopSound(i);
        if(sample[i].sptr) {
            FSOUND_Sample_Free(sample[i].sptr);
            sample[i].sptr=0;
        }
    }
}

void C_Sound::StopAudio(void) {
    if(!bfmod) return;
    StopSound();
    StopMusic();
}

void C_Sound::SetMusicVolume(float f) {
    if(!bfmod) return;
    mvol=(u_char)f;
    FMUSIC_SetMasterVolume(fmusic,mvol);
}

void C_Sound::SetSoundVolume(float f) {
    if(!bfmod) return;
    svol=(u_char)f;
    FSOUND_SetSFXMasterVolume(svol);
}
void C_Sound::Log(string x) {
    if(pLog) {
#ifndef _DLCS_CONSOLE
        pLog->_Add(x.c_str());
#endif
    } else {
        cout << x << endl;
    }
}


#endif // DLCS_WINDOWS
