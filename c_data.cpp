/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_data.h"
#include "SDL.h"

/***************************************************************
CFM_Character::CFM_Character() { memset(t_name,0,32); }
*/

/***************************************************************
CFM_Character::~CFM_Character() { } */

/***************************************************************/
CFM_Profile::CFM_Profile() {
    memset(name,0,32);
    memset(passwd,0,32);
    savepw=0;
    pNext=NULL;
}

/***************************************************************/
CFM_Profile::~CFM_Profile() { }
/***************************************************************/
CC_Data::CC_Data() {
    pLog=new CLog("cdata.log");
    bCreatedLog=true;
    Initialize();
}
CC_Data::CC_Data(CLog *pInLog) {
    bCreatedLog=false;
    pLog=pInLog;
    Initialize();
}

/***************************************************************/
CC_Data::~CC_Data() {
    CleanUp();
    if(bCreatedLog) DEL(pLog);
}

void CC_Data::Initialize(void) {
    //Log("Creating Client Data...");
	x=0;
	y=0;
	z=0;
    //	ServerInfo=new C_ServerInformation;
    //pCharacter = new C_Toon(); //CFM_Character();
	//ServerCharacter = NULL;
	//ServerCharacter = new C_Toon[MAX_TOONS]; //CFM_Character[MAX_SERVER_CHARS];
    //    ClearCharacters();
	//ChatBuffer = NULL;
	//ChatBuffer = new CFM_String[MAX_CHAT_BUFFER+1];
    //ClearChat();
    //    FavoriteServer=NULL;
    //    FirstFavoriteServer=NULL;
    //    FavoriteServer=new ServerData;
    //    FirstFavoriteServer=FavoriteServer;
    ClearFavoriteServers();
    Profile = NULL;
    FirstProfile=NULL;
    Profile = new CFM_Profile;
    FirstProfile=Profile;
    ClearProfiles();
	memset(szAccessName,0,255);
    strcpy(szServerVersion,"Unknown");
	SetToDefaults();
	//Log("Client Data Created");
	bLoad();
}
/***************************************************************/
void CC_Data::CleanUp(void) {
    //DLog("Cleaning up data...");
    //DEL(pCharacter);
    //if(ServerCharacter)
    //{
	//	delete [] ServerCharacter;
    //      ServerCharacter=NULL;
    //      DLog("ServerCharacter deleted");
    //}
    //DEL(ServerInfo);
    ClearFavoriteServers();
    ClearProfiles();

}

/***************************************************************/
void CC_Data::SetToDefaults(void) {
	memset(Name,0,24);
	memset(Password,0,24);
	memset(IPAddress,0,255);
	memset(Port,0,10);
	memset(MasterPort,0,10);
	memset(MasterIPAddress,0,255);
	memset(szServerVersion,0,15);
	memset(ServerMessage,0,1024);
	memset(ServerName,0,1024);

	strcpy(Name,"Enter Your Name");
	bSavePassword=true;
	strcpy(ServerMessage,"No server message.");
	strcpy(ServerName,"Ember Server");
    strcpy(ServerID,"standard"); // default
    strcpy(szServerVersion,"1");
    //    strcpy(IPAddress,NET_DEFAULT_EGS_IP);
    //	strcpy(Port,va("%d",NET_DEFAULT_EGS_PORT));
    //strcpy(MasterIPAddress,NET_DEFAULT_EMS_IP);
    //strcpy(MasterPort,va("%d",NET_DEFAULT_EMS_PORT));
	bLog=true;
	bDownload=true;
	//PlayerChatDelay=4000;

	fSoundVolume=100;
	fMusicVolume=100;

    //sprintf(MouseLeftButtonSound,"snd%cmouselb.wav",PATH_SEP);
    cDebug=0;
    bShowPING=false;
    bSound=true;
	bMusic=true;
	SelectedServer=0;
	CharacterSlots=8;
    SelectedCharacterSlot=0;
    currentsample=0;
    //    strcpy(LastDirectory,"");


	strcpy(ServerAuthor,"");

	ClearFavoriteServers();
    //	ClearCharacters();
	ServerListOffset=0;
	ServerCharacterListOffset=0;
	// ID=0;
	memset(session_id,0,64);
	Access=0;
	strcpy(szAccessName,"");
    //	strcpy(PlayMouseLB,"");
    //    dwKeyPressTimer=dlcs_get_tickcount();

    bDrawMapObjects=true;
    bDrawMap=true;
    bBlockGlow=true;
    bVertice=0;
    ClearProfiles();
    bFullScreen=false;
    ScreenWidth = 1366;// SCREEN_WIDTH;
    ScreenHeight= 768; //SCREEN_HEIGHT;
    ScreenColors= 24; //SCREEN_COLORS;

}

/***************************************************************/

//void CC_Data::ClearCharacters(void){
//    int i;
 //   for(i=0;i<MAX_TOONS;i++)
//        memset(ServerCharacter[i].t_name,0,32);
//}

/***************************************************************/

void CC_Data::ClearFavoriteServers(void){
 //   ServerData *DelMe;
//    FavoriteServer=FirstFavoriteServer;
//    while(FavoriteServer)
 //   {
   //     DelMe=FavoriteServer;
     //   FavoriteServer=FavoriteServer->pNext;
       // DEL(DelMe);
    //}
    //FirstFavoriteServer=NULL;
}

/***************************************************************/

void CC_Data::ClearProfiles(void) {
    CFM_Profile *DelMe;
    Profile=FirstProfile;
    while(Profile) {
        DelMe=Profile;
        Profile=Profile->pNext;
        DEL(DelMe);
    }
    FirstProfile=NULL;
}

/***************************************************************/

bool CC_Data::bLoad(void) {
	SetToDefaults();

	FILE *fp;
	char In[256];
    float f;
    vector <string> lin;

	fp=fopen("client.ini","rt");

	if(!fp)
        return false;

	while(1) {
		if(!fgets(In,255,fp)) break;
		lin = explode("=",In);
        if(lin.size()>1) {

        pLog->_Add("%s",lin[0].c_str());
        pLog->_Add("%s",lin[1].c_str());

		if(dlcs_strcasecmp(lin[0].c_str(),"name")) {
			strcpy(Name,lin[1].c_str());
            pLog->_Add(" Name [%s]",Name);
			continue;
		}

		if(dlcs_strcasecmp(lin[0].c_str(),"password")) {
			strcpy(Password,lin[1].c_str());
			continue;

		}

		if(dlcs_strcasecmp(lin[0].c_str(),"save password")) {
            bSavePassword=sp_istrue(lin[1].c_str());
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
            bLog=sp_istrue(lin[1].c_str());
			continue;
		}

		if(dlcs_strcasecmp(lin[0].c_str(),"download")) {
			bDownload=sp_istrue(lin[1].c_str());
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
			bSound=sp_istrue(lin[1].c_str());
			continue;
		}

		if(dlcs_strcasecmp(lin[0].c_str(),"music")) {
            bMusic=sp_istrue(lin[1].c_str());
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

        if(dlcs_strcasecmp(lin[0].c_str(),"full screen")) {
            bFullScreen=sp_istrue(lin[1].c_str());
            pLog->_Add(" bFullScreen [%d]",bFullScreen);
			continue;
        }

        if(dlcs_strcasecmp(lin[0].c_str(),"screen width")) {
            ScreenWidth=atoi(lin[1].c_str());
            pLog->_Add(" ScreenWidth [%d]",ScreenWidth);
            continue;
        }

        if(dlcs_strcasecmp(lin[0].c_str(),"screen height")) {
            ScreenHeight=atoi(lin[1].c_str());
            pLog->_Add(" ScreenHeight [%d]",ScreenHeight);
            continue;
        }

        if(dlcs_strcasecmp(lin[0].c_str(),"screen colors")) {
            ScreenColors=atoi(lin[1].c_str());
            pLog->_Add(" ScreenColors [%d]",ScreenColors);
            continue;
        }
	}

	}

	fclose(fp);

	if(bSavePassword==false) {
		memset(Password,0,sizeof(Password));
		bSave();
	}
	return true;
}

/***************************************************************/

bool CC_Data::bSave(void) {

	FILE *fout;
	char Temp[256];
	char Temp3[_MAX_PATH];
	char Temp4[_MAX_PATH];

	strcpy(Temp4,"client");//sprintf(Temp4,"%s%cclient",FMDir,PATH_SEP);
	strcpy(Temp3,Temp4);
	strcat(Temp3,".tmp");

	fout =fopen(Temp3,"w");
	if(!fout)
		return false;

	fputs("***************************************************************\n",fout);
	fputs("** Mantra initialization file                                **\n",fout);
	fputs("***************************************************************\n",fout);
    fputs("NOTE: If Mantra has problems starting, try renaming this file.\n",fout);

	// set name name
	sprintf(Temp,"Name=%s\n",Name);
	fputs(Temp,fout);

	// set password password
	if(bSavePassword)
		sprintf(Temp,"Password=%s\n",Password);
	else
		strcpy(Temp,"Password=\n");
	fputs(Temp,fout);

	// set savepassword on (or off)
	strcpy(Temp,"Save Password=");
	if(bSavePassword==true)
		strcat(Temp,"on\n");
	else
		strcat(Temp,"off\n");
	fputs(Temp,fout);

	sprintf(Temp,"Last Server=%s\n",ServerName);
	fputs(Temp,fout);

	// set IPAddress 127.0.0.1
	sprintf(Temp,"Last Server IP=%s\n",IPAddress);
	fputs(Temp,fout);

	// set port 29180
	sprintf(Temp,"Port=%s\n",Port);
	fputs(Temp,fout);

	// set log on (or off)
	strcpy(Temp,"Log=");
	if(bLog==true)
		strcat(Temp,"on\n");
	else
		strcat(Temp,"off\n");
	fputs(Temp,fout);

	// set download on (or off)
	strcpy(Temp,"Download=");
	if(bDownload==true)
		strcat(Temp,"on\n");
	else
		strcat(Temp,"off\n");
	fputs(Temp,fout);

	// set playerchatdelay ####
	//sprintf(Temp,"Player Chat Delay=%d\n",PlayerChatDelay);
	//fputs(Temp,fout);

	// set mouse left button click sound
	//sprintf(Temp,"Mouse Left Button Sound=%s\n",MouseLeftButtonSound);
	//fputs(Temp,fout);

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

	strcpy(Temp,"Full Screen=");
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

	strcpy(Temp3,Temp4);
	strcat(Temp3,".tmp");
	strcat(Temp4,".ini");
	remove(Temp4);
	rename(Temp3,Temp4);
	return true;
}


/***************************************************************/
/** Gather Favorite Servers                                   **/
/***************************************************************

bool CC_Data::LoadFavoriteServers(void)
{
    char szTemp1[1024];
    ServerData *pData=NULL;
    FILE *fp;
    int i;
    int j=0;
    char* Entry;
    ClearFavoriteServers();
    fp=fopen(va("servers%cfavorites.txt",PATH_SEP),"rt"); // Load in favorite server list
    if(fp)
    {
        while(1)
        {
            if(!fgets(szTemp1,96,fp))
                break;

            DEL(pData);
            pData=new ServerData;

            Entry=strtok(szTemp1,",\n\r");
            if(Entry==NULL)
                continue;
            i=atoi(Entry);
            if((i>0)||(!strcmp(Entry,"0")))
            {
                pData->key=i;

                Entry=strtok(NULL,",\n\r");
                if(Entry==NULL)
                    continue;
                strcpy(pData->name,Entry);
                Entry=strtok(NULL,",\n\r");
                if(Entry==NULL)
                    continue;
                strcpy(pData->ip_address,Entry);
                Entry=strtok(NULL,",\n\r");
                if(Entry==NULL)
                    continue;
                strcpy(pData->port,Entry);
                Entry=strtok(NULL,",\n\r");
                if(Entry!=NULL)
                {
                    strcpy(pData->user,Entry);
                    Entry=strtok(NULL,",\n\r");
                    if( (Entry!=NULL) && bSavePassword)
                        strcpy(pData->password,Entry);
                    else
                        pData->password[0]=0;
                }
                else
                {
                    strcpy(pData->user,"none");
                    pData->password[0]=0;
                }


                if(!FirstFavoriteServer)
                {
                    FirstFavoriteServer=pData;
                    FavoriteServer=pData;
                    pData=0;
                }
                else
                {
                    FavoriteServer=FirstFavoriteServer;
                    while(FavoriteServer->pNext)
                    {
                        FavoriteServer=FavoriteServer->pNext;
                    }
                    FavoriteServer->pNext=pData;
                    pData=0;
                }

                j++;
            }
        }
        fclose(fp);
        if(!j)
        {
            //Log("No valid entries in server list...");
        }
        return true;
    }
    //Log("No favorites.txt found...");

    return false;

}

 // **************************************************
 // ** Save Favorite Servers                        **
 // **************************************************

bool CC_Data::SaveFavoriteServers(void)
{
    char szTemp1[1024];
    FILE *fp;
    fp=fopen(va("servers%cfavorites.txt",PATH_SEP),"wt");
    if(fp)
    {
        fputs("*****************************************************************\n",fp);
        fputs("**                Ember Favorite Server List                   **\n",fp);
        fputs("*****************************************************************\n",fp);
        fputs("Format(Entry Number,Server Name,IPAddress,Port,Username,Password)\n",fp);

       FavoriteServer=FirstFavoriteServer;
        while(FavoriteServer)
        {
            sprintf(szTemp1,"%d,%s,%s,%s,%s,",FavoriteServer->key,FavoriteServer->name,FavoriteServer->ip_address,FavoriteServer->port,FavoriteServer->user);
            if(bSavePassword)
                strcat(szTemp1,FavoriteServer->password);
            strcat(szTemp1,"\n");
            fputs(szTemp1,fp);
            FavoriteServer=FavoriteServer->pNext;
        }

        fclose(fp);
        return true;
    }
    //Log("Failed to create favorites.txt");
    return false;
    return 0;
}

 // **************************************************
 // ** Discard Favorite Server                      **
 // **************************************************

bool CC_Data::DiscardFavoriteServer(int i)
{
    LoadFavoriteServers();
    char szTemp1[1024];
    FILE *fp;
    int j=0;
    fp=fopen(va("servers%cfavorites.txt",PATH_SEP),"wt");
    if(fp)
    {
        fputs("*****************************************************************\n",fp);
        fputs("**                Ember Favorite Server List                   **\n",fp);
        fputs("*****************************************************************\n",fp);
        fputs("Format(Entry Number,Server Name,IPAddress,Port,Username,Password)\n",fp);


        FavoriteServer=FirstFavoriteServer;
        while(FavoriteServer)
        {
            if(FavoriteServer->key!=i)
            {
                FavoriteServer->key=j;
                sprintf(szTemp1,"%d,%s,%s,%s,%s,",FavoriteServer->key,FavoriteServer->name,FavoriteServer->ip_address,FavoriteServer->port,FavoriteServer->user);
                if(bSavePassword)
                    strcat(szTemp1,FavoriteServer->password);
                strcat(szTemp1,"\n");
                fputs(szTemp1,fp);
                j++;
            }
            FavoriteServer=FavoriteServer->pNext;
        }


        fclose(fp);
        LoadFavoriteServers();
        return true;
    }
    //Log("Failed to create favorites.txt");
    return false;

}

 // **************************************************
 // ** Add Favorite Server To List                  **
 // **************************************************

bool CC_Data::AddFavoriteServer(ServerData *pData)
{
    if(!pData) return false;
    LoadFavoriteServers();
    char szTemp1[1024];
    FILE *fp;
    int i=0;
    fp=fopen(va("servers%cfavorites.txt",PATH_SEP),"wt");
    if(fp)
    {
        fputs("*****************************************************************\n",fp);
        fputs("**                Ember Favorite Server List                   **\n",fp);
        fputs("*****************************************************************\n",fp);
        fputs("Format(Entry Number,Server Name,IPAddress,Port,Username,Password)\n",fp);

        FavoriteServer=FirstFavoriteServer;
        while(FavoriteServer)
        {
            FavoriteServer->key=i;
            i++;
            sprintf(szTemp1,"%d,%s,%s,%s,%s,",FavoriteServer->key,FavoriteServer->name,FavoriteServer->ip_address,FavoriteServer->port,FavoriteServer->user);
            if(bSavePassword)
                strcat(szTemp1,FavoriteServer->password);
            strcat(szTemp1,"\n");
            fputs(szTemp1,fp);
            FavoriteServer=FavoriteServer->pNext;
        }

        sprintf(szTemp1,"%d,%s,%s,%s,%s,",i,pData->name,pData->ip_address,pData->port,pData->user);
        if(bSavePassword)
            strcat(szTemp1,pData->password);
        strcat(szTemp1,"\n");
        fputs(szTemp1,fp);
        fclose(fp);
        return true;
    }
    //Log("Failed to create favorites.txt");
    return false;
}
*/

/**************************************************/

bool CC_Data::LoadProfiles(void)
{
    char szTemp1[1024];
    FILE *fp;
    int j=0;
    //char* Entry;
    ClearProfiles();
    sprintf(szTemp1,"servers%cprofiles.txt",PATH_SEP); // Create path to serverlist file
    fp=fopen(szTemp1,"rt"); // Load in favorite server list
    if(fp)
    {

        fclose(fp);
        return 1;
    }
    return 0;
}

/***************************************************************

C_ServerInformation::C_ServerInformation(void)
{
    // Race = NULL;
    // Race = new CFM_String[MAX_RACES]; // max races
    // Class = NULL;
    // Class = new CFM_String[MAX_CLASSES]; // max races
}
*/

/***************************************************************

C_ServerInformation::~C_ServerInformation(void)
{

    if(Race)
    {
        delete [] Race;
        Race=NULL;
    }
    if(Class)
    {
        delete [] Class;
        Class=NULL;
    }
}
*/

/***************************************************************/


