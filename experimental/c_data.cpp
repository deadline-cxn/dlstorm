/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2016
 **   File:         c_data.h
 **   Class:        CC_Data
 **                 C_Profile
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_data.h"
//////////////////// C_Profile class
C_Profile::C_Profile() {
    name.clear();
    passwd.clear();
    savepw=0;
    pNext=NULL;
}
C_Profile::~C_Profile() { }
//////////////////// CC_Data class
CC_Data::CC_Data() {
    Initialize();
    bLoad();
}
CC_Data::CC_Data(char* infilename) {
    Initialize();
    filename.assign(infilename);
    bLoad();
}
CC_Data::~CC_Data() {
    CleanUp();
}
void CC_Data::Initialize(void) {
    filename="client.ini";
    ClearFavoriteServers();
    Profile = NULL;
    FirstProfile=NULL;
    Profile = new C_Profile;
    FirstProfile=Profile;
    ClearProfiles();
    memset(szAccessName,0,255);
    strcpy(szServerVersion,"Unknown");
    SetToDefaults();
}
void CC_Data::CleanUp(void) {
    ClearFavoriteServers();
    ClearProfiles();
}
void CC_Data::SetToDefaults(void) {
    Name.clear();
    Password.clear();
    memset(IPAddress,0,255);
    memset(Port,0,10);
    memset(MasterPort,0,10);
    memset(MasterIPAddress,0,255);
    memset(szServerVersion,0,15);
    memset(ServerMessage,0,1024);
    memset(ServerName,0,1024);
    Name="Enter Your Name";
    bSavePassword=true;
    strcpy(ServerMessage,"No server message.");
    strcpy(ServerName,"Ember Server");
    strcpy(ServerID,"standard");
    strcpy(szServerVersion,"1");
    bLog=true;
    bDownload=true;
    fSoundVolume=100;
    fMusicVolume=100;
    cDebug=0;
    bShowPING=false;
    bSound=true;
    bMusic=true;
    SelectedServer=0;
    CharacterSlots=8;
    SelectedCharacterSlot=0;
    currentsample=0;
    strcpy(ServerAuthor,"");
    ClearFavoriteServers();
    ServerListOffset=0;
    ServerCharacterListOffset=0;
    memset(session_id,0,64);
    Access=0;
    strcpy(szAccessName,"");
    bDrawMapObjects=true;
    bDrawMap=true;
    bBlockGlow=true;
    bVertice=0;
    ClearProfiles();
    bFullScreen=false;
    ScreenWidth = 1024;// SCREEN_WIDTH;
    ScreenHeight= 768; //SCREEN_HEIGHT;
    ScreenColors= 24; //SCREEN_COLORS;
}
void CC_Data::ClearFavoriteServers(void) {
}
void CC_Data::ClearProfiles(void) {
    C_Profile *DelMe;
    Profile=FirstProfile;
    while(Profile) {
        DelMe=Profile;
        Profile=Profile->pNext;
        dlcsm_delete(DelMe);
    }
    FirstProfile=NULL;
}
bool CC_Data::bLoad(void) {
    SetToDefaults();
    FILE *fp;
    char In[256];
    char temp[1024];
    float f;
    vector <string> lin;
    fp=fopen(filename.c_str(),"rt");
    if(!fp) return false;
    while(fgets(In,255,fp)) {
        In[strlen(In)-1]=0;
        lin = dlcs_explode("=",In);
        if(lin.size()>1) {
            strcpy(temp,lin[1].c_str());
            if(dlcs_strcasecmp(lin[0].c_str(),"name")) {
                Name.assign(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"password")) {
                Password.assign(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"save password")) {
                bSavePassword=dlcs_istrue(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"last server")) {
                strcpy(ServerName,lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"last server ip")) {
                strcpy(IPAddress,lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"port")) {
                strcpy(Port,lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"log")) {
                bLog=dlcs_istrue(temp);
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"download")) {
                bDownload=dlcs_istrue(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"sound volume")) {
                f=atof(lin[1].c_str());
                if(f>100) f=100;
                if(f<0)   f=0;
                fSoundVolume=f;
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"music volume")) {
                f=atof(lin[1].c_str());
                if(f>100) f=100;
                if(f<0)   f=0;
                fMusicVolume=f;
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"sound")) {
                bSound=dlcs_istrue(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"music")) {
                bMusic=dlcs_istrue(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"master server")) {
                strcpy(MasterIPAddress,lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"master server port")) {
                strcpy(MasterPort,lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"fullscreen")) {
                bFullScreen=dlcs_istrue(temp);
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"screen width")) {
                ScreenWidth=atoi(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"screen height")) {
                ScreenHeight=atoi(lin[1].c_str());
                continue;
            }
            if(dlcs_strcasecmp(lin[0].c_str(),"screen colors")) {
                ScreenColors=atoi(lin[1].c_str());
                continue;
            }
        }
    }
    fclose(fp);
    if(bSavePassword==false) {
        Password.clear();
        bSave();
    }
    return true;
}
bool CC_Data::bSave(void) {
    FILE *fout;
    char Temp[256];
    char Temp3[_MAX_PATH];
    char Temp4[_MAX_PATH];
    fout =fopen(filename.c_str(),"w");
    if(!fout)
        return false;
    fputs("***************************************************************\n",fout);
    fputs("** DLStorm initialization file                               **\n",fout);
    fputs("***************************************************************\n",fout);
    fputs("NOTE: Try renaming this file if the program fails.\n",fout);
    sprintf(Temp,"Name=%s\n",Name.c_str());
    fputs(Temp,fout);
    if(bSavePassword)
        sprintf(Temp,"Password=%s\n",Password.c_str());
    else
        strcpy(Temp,"Password=\n");
    fputs(Temp,fout);
    strcpy(Temp,"Save Password=");
    if(bSavePassword==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);

    sprintf(Temp,"Last Server=%s\n",ServerName);
    fputs(Temp,fout);
    sprintf(Temp,"Last Server IP=%s\n",IPAddress);
    fputs(Temp,fout);
    sprintf(Temp,"Port=%s\n",Port);
    fputs(Temp,fout);
    strcpy(Temp,"Log=");
    if(bLog==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);
    strcpy(Temp,"Download=");
    if(bDownload==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);
    strcpy(Temp,"Sound=");
    if(bSound==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);
    sprintf(Temp,"Sound Volume=%f\n",fSoundVolume);
    fputs(Temp,fout);
    strcpy(Temp,"Music=");
    if(bMusic==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);
    sprintf(Temp,"Music Volume=%f\n",fMusicVolume);
    fputs(Temp,fout);
    sprintf(Temp,"Master Server=%s\n",MasterIPAddress);
    fputs(Temp,fout);
    sprintf(Temp,"Master Server Port=%s\n",MasterPort);
    fputs(Temp,fout);
    strcpy(Temp,"FullScreen=");
    if(bFullScreen==true)
        strcat(Temp,"on\n");
    else
        strcat(Temp,"off\n");
    fputs(Temp,fout);
    sprintf(Temp,"Screen Width=%d\n",ScreenWidth);
    fputs(Temp,fout);
    sprintf(Temp,"Screen Height=%d\n",ScreenHeight);
    fputs(Temp,fout);
    sprintf(Temp,"Screen Colors=%d\n",ScreenColors);
    fputs(Temp,fout);
    fclose(fout);
    return true;
}
bool CC_Data::LoadProfiles(void) {
    dlcsm_make_filename(szTemp1);
    FILE *fp;
    int j=0;
     ClearProfiles();
    sprintf(szTemp1,"servers%cprofiles.txt",PATH_SEP);
    fp=fopen(szTemp1,"rt");
    if(fp) {
        fclose(fp);
        return true;
    }
    return false;
}
