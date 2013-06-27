/* Seth's FMOD Sound Wrapper */

#include "c_fmod_sound_util.h"
#include "fmod.h"

FMUSIC_MODULE * fmusic;

struct samplelist { FSOUND_SAMPLE *sptr; };

samplelist *sample;
#define max_channels 32

unsigned char svol=80;
unsigned char mvol=64;
bool bfmod;

extern "C" char InitializeSound()
{
    char x;

    x = FSOUND_Init(44100, max_channels, 0);
    if(x)
    {
        bfmod=1;
        // max_channels=FSOUND_GetMaxChannels();
        sample = new samplelist[max_channels];
        for(int i=0;i<max_channels;i++)
        {
            sample[i].sptr=0;
        }
    }
    else
    {
        bfmod=0;
    }

    SetSoundVolume(svol);
    // FSOUND_GetVersion();
    return x;
}
extern "C" void ShutDownSound(void)
{
    if(!bfmod) return;
    StopAudio();
    FSOUND_Close();
    delete [] sample;
}

extern "C" int PlaySample(char* szFilename)
{
    static int channel=7;
    int what;
    if(!bfmod) return 0;

    if(sample[channel].sptr) { FSOUND_Sample_Free(sample[channel].sptr); sample[channel].sptr; }

    sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,szFilename,FSOUND_LOOP_OFF,0,0);

    if(sample[channel].sptr)
    {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        channel++; if (channel>=max_channels) channel=7;
        return what;
    }
    return 0;
}

extern "C" int PlayMemSample(unsigned char *sdata,int size)
{
    static int channel=7;
    int what;
    if(!bfmod) return 0;

    if(sample[channel].sptr) { FSOUND_Sample_Free(sample[channel].sptr); sample[channel].sptr; }

    sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,(const char *)sdata,FSOUND_LOADMEMORY|FSOUND_LOOP_OFF,0,size);

    if(sample[channel].sptr)
    {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        channel++; if (channel>=max_channels) channel=7;
        return what;
    }
    return 0;
}

extern "C" int PlaySampleLoop(char* szFilename)
{
    static int channel=5;
    int what;
    if(!bfmod) return 0;

    if(sample[channel].sptr) { FSOUND_Sample_Free(sample[channel].sptr); sample[channel].sptr; }

    sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,szFilename,FSOUND_LOOP_NORMAL,0,0);

    if(sample[channel].sptr)
    {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        channel++; if(channel>6) channel=5;
        return what;
    }
    return 0;
}

extern "C" int  LoopSample(char *szFilename)
{
    return PlaySampleLoop(szFilename);
}

extern "C" int  LoopMemSample(unsigned char *sdata,int size)
{
    static int channel=5;
    int what;
    if(!bfmod) return 0;

    if(sample[channel].sptr) { FSOUND_Sample_Free(sample[channel].sptr); sample[channel].sptr; }

    sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,(const char *)sdata,FSOUND_LOADMEMORY|FSOUND_LOOP_NORMAL,0,size);

    if(sample[channel].sptr)
    {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        channel++; if(channel>6) channel=5;
        return what;
    }
    return 0;

}

extern "C" char PlayMusic(char* szFilename)
{
    if(!bfmod) return 0;
    StopMusic();
    fmusic=FMUSIC_LoadSong(szFilename);
    if(fmusic)
    {
        FMUSIC_SetMasterVolume(fmusic,mvol);
        return FMUSIC_PlaySong(fmusic);
    }
    return 0;
}

extern "C" void StopMusic(void)
{
    if(!bfmod) return;
    if(fmusic)
    {
        FMUSIC_StopSong(fmusic);
        FMUSIC_FreeSong(fmusic);
    }
    fmusic=0;

}

extern "C" void StopSound(void)
{
    if(!bfmod) return;
    for(int i=0;i<max_channels;i++)
    {
        FSOUND_StopSound(i);
        if(sample[i].sptr)
        {
            FSOUND_Sample_Free(sample[i].sptr);
            sample[i].sptr=0;
        }
    }
}

extern "C" void StopAudio(void)
{
    if(!bfmod) return;
    StopSound();
    StopMusic();
}

extern "C" void SetMusicVolume(float f)
{
    if(!bfmod) return;
    mvol=(unsigned char)f;
    FMUSIC_SetMasterVolume(fmusic,mvol);
}

extern "C" void SetSoundVolume(float f)
{
    if(!bfmod) return;
    svol=(unsigned char)f;
    FSOUND_SetSFXMasterVolume(svol);
}

/*
void testwavload()
{
FILE *fp;

fp = fopen("sound.wav","rb");
if (fp)
{
    BYTE id[4]; //four bytes to hold 'RIFF'
    DWORD size; //32 bit value to hold file size

    fread(id,sizeof(BYTE),4,fp); //read in first four bytes
    if (!strcmp(id,"RIFF"))
    { //we had 'RIFF' let's continue
        fread(size,sizeof(DWORD),1,fp);
        //read in 32bit size value
    }
}

// Ok that'll get the first part read in if we read in id and it isn't equal to RIFF then we know this isn't a wave file,
// if it is then we can continue on. After the 32 bit size value we now should have two more strings to read in that will
// positively ID the file as a wave file or not. We'll now read in another 4 bytes which should contain the string "WAVE"
// and then another 4 byte string containing the word "fmt " (notice the extra space after fmt ). The "fmt " string let's
// us know that the format chunk is coming up afterwards. Let's update our code to reflect this now:

FILE *fp;

fp = fopen("sound.wav","rb);
if (fp)
{
    BYTE id[4]; //four bytes to hold 'RIFF'
    DWORD size; //32 bit value to hold file size

    fread(id,sizeof(BYTE),4,fp); //read in first four bytes
    if (!strcmp(id,"RIFF"))
    { //we had 'RIFF' let's continue
        fread(size,sizeof(DWORD),1,fp);
        //read in 32bit size value
        fread(id,sizeof(BYTE),4,fp);
        //read in 4 byte string now
        if (!strcmp(id,"WAVE"))
        { //this is probably a wave file since it contained "WAVE"
            fread(id,sizeof(BYTE),4,fp); //read in 4 bytes "fmt ";
        }
    }
}

//After "fmt " we have a 32 bit value that says how big our following format chunk is going to be, for a typical wave file this will be "16", meaning that we're going to use the next 16 bytes in the file to describe the sound data's format. Be careful though, this value may not always be a 16 - wave files that are compressed (like ADPCM or such) use different format chunk sizes and you may need to adjust, if you don't have a 16 here you may want to abort the load unless you know about the way the compressed file stores it's format info. Let's see what those next 16 bytes are going to be in a typical wave file:
//32 bit value saying how big the format chunk is (in bytes)
//16 bit value identifying the format tag (identifies way data is stored, 1 here means no compression (PCM), if otherwise it's some other type of format)
//16 bit value describing # of channels (1 means mono, 2 means stereo)
//32 bit value describing sample rate or number of samples per second (like 44100, 22050, or 11025)
//32 bit value describing average # of bytes per second (found by: samplerate*channels*(bitspersample/8)) you probably won't need or use this value
//16 bit value describing block alignment (found by: (bitspersample/8)*channels) you probably won't need or use this value either
//16 bit value describing bits per sample (8bit or 16bit sound)
//Ok now that we know what those next 16 bytes describing the format are, let's add some source code to reflect this:

FILE *fp;

fp = fopen("sound.wav","rb);
if (fp)
{
    BYTE id[4]; //four bytes to hold 'RIFF'
    DWORD size; //32 bit value to hold file size
	//our 16 bit format info values
    short format_tag, channels, block_align, bits_per_sample;
    DWORD format_length, sample_rate, avg_bytes_sec; //our 32 bit format info values

    fread(id, sizeof(BYTE), 4, fp); //read in first four bytes
    if (!strcmp(id, "RIFF"))
    { //we had 'RIFF' let's continue
        fread(size, sizeof(DWORD), 1, fp); //read in 32bit size value
        fread(id, sizeof(BYTE), 4, fp); //read in 4 byte string now
        if (!strcmp(id,"WAVE"))
        { //this is probably a wave file since it contained "WAVE"
            fread(id, sizeof(BYTE), 4, fp); //read in 4 bytes "fmt ";
            fread(format_length, sizeof(DWORD),1,fp);
            fread(format_tag, sizeof(short), 1, fp);
            fread(channels, sizeof(short),1,fp);
            fread(sample_rate, sizeof(DWORD), 1, fp);
            fread(avg_bytes_sec, sizeof(short), 1, fp);
            fread(block_align, sizeof(short), 1, fp);
            fread(bits_per_sample, sizeof(short), 1, fp);
        }
        else
            printf("Error: RIFF file but not a wave file\n");
    }
    else
        printf("Error: not a RIFF file\n");
}

// Alright, now we're just about ready to get to the good stuff - the actual sound data, but first we must read in, you guessed it, a 4 byte string containing the word "data" and then a 32 bit value describing how big our data chunk or raw sound chunk is (in bytes). Once we know how big our data chunk is we can set aside the space needed using malloc and start reading in the good stuff. That's all there is to reading in a wave file, and we'll finish up our function:

void Load_Wave_File(char *fname)
{
    FILE *fp;

    fp = fopen(fname,"rb);
    if (fp)
    {
        BYTE id[4], *sound_buffer; //four bytes to hold 'RIFF'
        DWORD size; //32 bit value to hold file size
        short format_tag, channels, block_align, bits_per_sample; //our 16 values
        DWORD format_length, sample_rate, avg_bytes_sec, data_size, i; //our 32 bit values

        fread(id, sizeof(BYTE), 4, fp); //read in first four bytes
        if (!strcmp(id, "RIFF"))
        { //we had 'RIFF' let's continue
            fread(size, sizeof(DWORD), 1, fp); //read in 32bit size value
            fread(id, sizeof(BYTE), 4, fp); //read in 4 byte string now
            if (!strcmp(id,"WAVE"))
            { //this is probably a wave file since it contained "WAVE"
                fread(id, sizeof(BYTE), 4, fp); //read in 4 bytes "fmt ";
                fread(&format_length, sizeof(DWORD),1,fp);
                fread(&format_tag, sizeof(short), 1, fp); //check mmreg.h (i think?) for other
                                                              // possible format tags like ADPCM
                fread(&channels, sizeof(short),1,fp); //1 mono, 2 stereo
                fread(&sample_rate, sizeof(DWORD), 1, fp); //like 44100, 22050, etc...
                fread(&avg_bytes_sec, sizeof(short), 1, fp); //probably won't need this
                fread(&block_align, sizeof(short), 1, fp); //probably won't need this
                fread(&bits_per_sample, sizeof(short), 1, fp); //8 bit or 16 bit file?
                fread(id, sizeof(BYTE), 4, fp); //read in 'data'
                fread(&data_size, sizeof(DWORD), 1, fp); //how many bytes of sound data we have
                sound_buffer = (BYTE *) malloc (sizeof(BYTE) * data_size); //set aside sound buffer space
                fread(sound_buffer, sizeof(BYTE), data_size, fp); //read in our whole sound data chunk
            }
            else
                printf("Error: RIFF file but not a wave file\n");
        }
        else
            printf("Error: not a RIFF file\n");
    }
}

}
*/
