/* Seth's FMOD Sound Wrapper */

#include "c_fmod.h"

#include "dlstorm.h"

CSample::CSample() {
    memset(name, 0, 256);
    size             = 0;
    volume           = 80;
    pNext            = 0;
    pPrev            = 0;
    data             = 0;
    max_simultaneous = 1;
}

CSample::CSample(char *file) {
    memset(name, 0, 256);
    size             = 0;
    volume           = 80;
    pNext            = 0;
    pPrev            = 0;
    data             = 0;
    max_simultaneous = 1;
}

CSample::~CSample() {
    if (data) {
        // FSOUND_StopSound(FSOUND_ALL);
        FSOUND_Sample_Free(data);
    }
}

void CSample::LoadCabSample(unsigned char *sdata, int ssize) {
    if (data) FSOUND_Sample_Free(data);
    size = ssize;
    data = FSOUND_Sample_Load(FSOUND_FREE, (const char *)sdata, FSOUND_LOADMEMORY | FSOUND_LOOP_OFF, 0, size);
}

void CSample::Play(int channel) {
    if (data) {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel, volume);
        FSOUND_PlaySound(channel, data);
    }
}

CFMOD::CFMOD() {
    FirstSample = 0;
    svol        = 50;
    fmusic      = 0;
    Log         = new CLog("fmod.log");
    //    Cab=new CCab("snd.dlz",NFCOMP_BEST);
    InitializeSound();
    LoadCabSamples();
}

CFMOD::~CFMOD() {
    ShutDownSound();
    // DEL(Cab);
    Log->AddEntry("FMOD Shutdown");
    DEL(Log);
}

char CFMOD::InitializeSound() {
    char x;
    FSOUND_SetMinHardwareChannels(8);
    FSOUND_SetMaxHardwareChannels(MAX_SND_CHANNELS);
    x = FSOUND_Init(44100, MAX_SND_CHANNELS, 0);
    Log->AddEntry(va("FMOD Version: %.2f started", FSOUND_GetVersion()));
    if (x) {
        bfmod       = 1;
        maxchannels = FSOUND_GetMaxChannels();
        Log->AddEntry(va("FMOD Max Channels [%d]", maxchannels));
    } else {
        bfmod = 0;
        return 0;
    }
    SetSoundVolume(svol);

    return x;
}

void CFMOD::ShutDownSound(void) {
    if (!bfmod) return;

    DeleteAllSamples();

    StopAudio();
    FSOUND_Close();
}

bool CFMOD::LoadCabSamples() {
    /*
    CSample *sample;
        if(!Cab->FileOpen)return false;
        int nd=Cab->FindDirNumber("snd");
        if(nd<0)return false; char Dir[NF_NAMESIZE]; char FileName[NF_NAMESIZE]; char FullPath[NF_NAMESIZE];
        for(int n=0;n<Cab->NumElements;n++)
        {
                if(Cab->Elements[n].DirPos==nd)
                {
                        Cab->GetFullPath(n,FullPath);
                        Cab->SplitNameAndDir(FullPath,FileName,Dir);
            Log->AddEntry(va("snd.dlz found: [%s]",FullPath));
            sample=NewSample(FullPath);
                }
        }*/
    return true;
}

CSample *CFMOD::NewSample(char *sname) {
    CSample *sample;
    CSample *newsample;
    sample = FirstSample;
    while (sample) {
        if (!sample->pNext) break;
        sample = sample->pNext;
    }
    if (!FirstSample) {
        newsample          = new CSample();
        FirstSample        = newsample;
        FirstSample->pNext = 0;
        Log->AddEntry("FirstSample created!");
        sample = newsample;
    } else {
        newsample     = new CSample();
        sample->pNext = newsample;
        sample        = sample->pNext;
    }
    strcpy(sample->name, sname);

    // sample->LoadCabSample(Cab->GetFile(sname).fb,Cab->GetFile(sname).Size);

    Log->AddEntry(va("sample(sample pointer:%d) [%s] (data pointer:%d) (%d bytes) loaded from snd.dlz ", sample, sample->name, sample->data, sample->size));
    return sample;
}

void CFMOD::DeleteSample(char *sname) {
    CSample *sample;
    CSample *prevsample;
    prevsample = 0;
    sample     = FirstSample;
    while (sample) {
        Log->AddEntry(va("DEL Searching samples [%s][%s]", sname, sample->name));

        if (dlcs_strcasecmp(sample->name, sname)) {
            if (prevsample) prevsample->pNext = sample->pNext;
            DEL(sample);
            return;
        }
        prevsample = sample;
        sample     = sample->pNext;
    }
}

CSample *CFMOD::GetSample(char *sname) {
    CSample *sample = FirstSample;
    while (sample) {
        if (dlcs_strcasecmp(sample->name, sname)) {
            return sample;
        }
        sample = sample->pNext;
    }
    return sample;
}

void CFMOD::DeleteAllSamples(void) {
    Log->AddEntry("Deleting All Samples");
    CSample *delsample;
    CSample *nextsample;
    delsample = FirstSample;
    while (delsample) {
        nextsample = delsample->pNext;
        Log->AddEntry(va("sample: [%s] DEL!", delsample->name));
        DEL(delsample);
        delsample = nextsample;
    }
}

// int CFMOD::PlaySample(char* szFilename)
//{
//   static int channel=7;
//    int what;
// if(!bfmod) return 0;

//  if(sample[channel].sptr) { FSOUND_Sample_Free(sample[channel].sptr); sample[channel].sptr; }

//  sample[channel].sptr=FSOUND_Sample_Load(FSOUND_FREE ,szFilename,FSOUND_LOOP_OFF,0,0);

/*
    if(sample[channel].sptr)
    {
        FSOUND_StopSound(channel);
        FSOUND_SetVolume(channel,svol);
        what=FSOUND_PlaySound(channel,sample[channel].sptr);
        channel++; if (channel>=max_channels) channel=7;
        return what;
    }
    */
// return 0;
//}

int CFMOD::PlaySample(char *sname, int wchannel, int flags) {
    int      number_samples_playing = 0;
    CSample *sample;
    if (!bfmod) return 0;
    sample = GetSample(sname);
    if (!sample) return 0;
    for (int i = 0; i < FSOUND_GetMaxChannels(); i++) {
        if (sample->data == FSOUND_GetCurrentSample(i)) {
            number_samples_playing++;
        }
    }
    if (number_samples_playing >= sample->max_simultaneous) return 0;
    if (flags) FSOUND_Sample_SetMode(sample->data, flags);
    FSOUND_SetVolume(wchannel, sample->volume);
    return FSOUND_PlaySound(wchannel, sample->data);
}

int CFMOD::PlaySample(char *sname) { return PlaySample(sname, FSOUND_FREE, 0); }

int CFMOD::PlaySampleLoop(char *szFilename) {
    /*
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
    }*/

    return 0;
}

int CFMOD::LoopMemSample(unsigned char *sdata, int size) {
    // FSOUND_FREE (Free Channel)
    /*
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
    */
    return 0;
}

char CFMOD::PlayMusic(char *szFilename) {
    if (!bfmod) return 0;
    StopMusic();
    fmusic = FMUSIC_LoadSong(szFilename);
    if (fmusic) {
        FMUSIC_SetMasterVolume(fmusic, mvol);
        return FMUSIC_PlaySong(fmusic);
    }
    return 0;
}

void CFMOD::StopMusic(void) {
    if (!bfmod) return;
    if (fmusic) {
        FMUSIC_StopSong(fmusic);
        FMUSIC_FreeSong(fmusic);
    }
    fmusic = 0;
}

void CFMOD::StopSound(void) {
    if (!bfmod) return;
    FSOUND_StopSound(FSOUND_ALL);
}

void CFMOD::StopAudio(void) {
    if (!bfmod) return;
    StopSound();
    StopMusic();
}

void CFMOD::SetMusicVolume(float f) {
    if (!bfmod) return;
    mvol = (unsigned char)f;
    FMUSIC_SetMasterVolume(fmusic, mvol);
}

void CFMOD::SetSoundVolume(float f) {
    if (!bfmod) return;
    svol = (unsigned char)f;
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

//After "fmt " we have a 32 bit value that says how big our following format chunk is going to be, for a typical wave file this will be "16", meaning that we're going to use the next 16 bytes in the file to describe the sound data's format. Be careful though, this value may not always be a 16 - wave
files that are compressed (like ADPCM or such) use different format chunk sizes and you may need to adjust, if you don't have a 16 here you may want to abort the load unless you know about the way the compressed file stores it's format info. Let's see what those next 16 bytes are going to be in a
typical wave file:
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

// Alright, now we're just about ready to get to the good stuff - the actual sound data, but first we must read in, you guessed it, a 4 byte string containing the word "data" and then a 32 bit value describing how big our data chunk or raw sound chunk is (in bytes). Once we know how big our data
chunk is we can set aside the space needed using malloc and start reading in the good stuff. That's all there is to reading in a wave file, and we'll finish up our function:

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
