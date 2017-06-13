/////////////////////////////////////////
#include "test_server.h"


CServer     *pServer;


#ifdef __linux__
#include "linux.h"
#include "s_gnu.h"
void ConsoleSetup(void) { _kbhit(); }
void ConsoleShutDown(void) { close_keyboard(); }
#endif

#ifdef _WIN32
BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType){
    switch(dwCtrlType) {
        case CTRL_BREAK_EVENT:
            pServer->bQuit=true;
            pServer->Log("Break event killed server!");
            return true;
        case CTRL_SHUTDOWN_EVENT:
            pServer->bQuit=true;
            pServer->Log("Shutdown event killed server!");
            return true;
        case CTRL_LOGOFF_EVENT:
            pServer->bQuit=true;
            pServer->Log("Logoff event killed server!");
            return true;
        case CTRL_CLOSE_EVENT:
            pServer->bQuit=true;
            pServer->Log("Mouse [X] killed server!");
            return true;
        case CTRL_C_EVENT:
            pServer->bQuit=true;
            pServer->Log("CTRL-C killed server!");
            return true;
        default:
            break;
    }
    return false;
}
void ConsoleSetup(void){ // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C,TRUE);
    SetConsoleTitle(va("Mantra Server %s(%s) Net(%d) Build(%d) %s",VERSION,CPUSTRING,NET_REVISION,TEST_S_BUILD,COPYRIGHT));
}
void ConsoleShutDown(void){ }
#endif

void ShowHelp(void){
    printf("Game Server %s %s Help\n",VERSION,COPYRIGHT);
    printf("==============================================================\n");
    printf(" --help, -h, /? = this help\n");
    printf(" --quiet, -q, /q = quiet mode, no console output\n");
    printf("==============================================================\n");

}

int main(int argc, char *argv[]){
    printf("Test Server\n");
    bool bquiet=false;
    char nargs;
    if(argc>1){ nargs=1;
        while(nargs<argc){
            if( (!strcmp(argv[nargs],"--help")) || (!strcmp(argv[nargs],"-h")) || (!strcmp(argv[nargs],"/?")) ){
                ShowHelp();
                exit(0);
            }

            if( (!strcmp(argv[nargs],"-q")) || (!strcmp(argv[nargs],"--quiet")) ||	(!strcmp(argv[nargs],"/q"))){
                bquiet=true;
            }
            nargs++;
        }
    }
    ConsoleSetup(); // (POSIX) system specific console setup
    pServer=new CServer(bquiet);
    if(!pServer) { ConsoleShutDown(); return 1; }
    while(!pServer->bQuit)
        pServer->cycle();
    DEL(pServer);
    ConsoleShutDown();
    return 1;
}




///////////////////////////////////////////
C_GSC::C_GSC(CLog *pInLog, CServer *pInServer){
    initc();
    pLog=pInLog;
    bMadeLog=false;
    pServer=pInServer;
}
///////////////////////////////////////////
void C_GSC::initc(void){

    // character = 0;
    memset(session_id,0,64);
    pServer = 0;
    bDelete = 0;
	inactivity_timer= new CTimer(300000);
	heartbeat_timer = new CTimer(5000);
	bHeartBeatCheck = 0;

    // 	character=new CMPCharacter();
	// character->pOwner=this;

    pNext=NULL;
    pPrevious=NULL;
}
///////////////////////////////////////////
C_GSC::~C_GSC(){
    save();
    DEL(inactivity_timer);
    DEL(heartbeat_timer);
    // DEL(character);
    if(bMadeLog) DEL(pLog);
}
///////////////////////////////////////////
void C_GSC::load(void){
    /*
    pServer->Log("Loading %s",username);
    int i;
    vector <string> tv;
    db_query(va("select * from users where username='%s'",username),0);
    access              = atoi(db_getvalue("username",username,"access").c_str());
    timeout_override    = atoi(db_getvalue("username",username,"timeout_override").c_str());
    tv=dlcs_explode(",",db_getvalue("username",username,"chat_channels").c_str());
    for(i=0;i<tv.size();i++){
        join_channel(tv[i].c_str());
        pServer->Log("Joined chat channel: %s",tv[i].c_str());
    }
    */

}
void C_GSC::save(void) {
    
    /*
    pServer->Log("Saving %s",username);
    int i;

    string s_temp;

    db_update("access",va("%d",access));
    db_update("timeout_override",va("%d",timeout_override));

    s_temp.clear();
    vector <string>::iterator vi;
    for(vi=chat_channel.begin();vi!=chat_channel.end();++vi){
        if(GUI_CHAT[(*vi).c_str()]){
            s_temp = s_temp + (*vi).c_str();
            s_temp = s_temp + ",";
            pServer->Log("%s->%d",(*vi).c_str(),GUI_CHAT[(*vi).c_str()]);
        }
    }
     
    if(!s_temp.empty()){
        s_temp.erase(s_temp.size()-1);
        pServer->Log("Saving chat setup %s",s_temp.c_str());
        db_update("chat_channels",s_temp.c_str());
    }
     */

}
///////////////////////////////////////////
void C_GSC::do_script(char *file){ }
///////////////////////////////////////////
void C_GSC::do_func(char *func) { }
///////////////////////////////////////////
bool C_GSC::gui_call(char *gui){
    CPacket Send(8192);
    Send.Reset();
    Send.Write((char)NM_GUI);
    Send.Write((char)FM_GUI_CALL);
    Send.Write((char *)gui);
    SendUnreliableMessage(&Send);
    return true;
}///////////////////////////////////////////
bool C_GSC::gui_clear(){
    CPacket Send(8192);
    Send.Reset();
    Send.Write((char)NM_GUI);
    Send.Write((char)FM_GUI_CLEAR_ALL);
    SendUnreliableMessage(&Send);
    return true;
}
///////////////////////////////////////////
void C_GSC::do_net(void){

    CPacket Send(8192);
	CPacket *Recv=NULL;
	map <string,string> pairdata;
	pairdata.clear();
	vector <string> tv;
    char  szTemp[1024];     memset(szTemp,0,1024);
    char szTemp2[1024];    memset(szTemp2,0,1024);
    char szTemp3[1024];    memset(szTemp3,0,1024);
    char szTemp4[2048];    memset(szTemp4,0,2048);
    int iMsg,i,j,k,x,y,z,ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez,fx,fy,fz;
    iMsg=i=j=k=x=y=z=ax=ay=az=bx=by=bz=cx=cy=cz=dx=dy=dz=ex=ey=ez=fx=fy=fz=0;
    double gx,gy,gz; gx=gy=gz=0; unsigned char vx,vy,vz; vx=vy=vz=0;
    char cMsgType;
    C_GSC *other_client;

    iMsg=iGetMessage();
    if(iMsg==-1) return;

    if(iMsg>0){

        Recv=pGetMessage();

        if(!Recv) return;

        cMsgType=Recv->cRead();

        Recv=pGetMessage();

        if(ToAddr.sin_addr.s_addr!=FromAddr.sin_addr.s_addr){
            pServer->Log("forged net message");
            return;

        cMsgType=Recv->cRead();

        }

        switch(cMsgType)
        {
            case NM_FUNCTION:
                ax=Recv->iRead();
                switch(ax){

                    case NF_CHAT_JOIN:
                        strcpy(szTemp, Recv->pRead());
                        join_channel(szTemp);
                        break;

                    case NF_CHAT_LEAVE:
                        strcpy(szTemp, Recv->pRead());
                        leave_channel(szTemp);
                        break;

                    case NF_CHAT_WHO:
                        break;

                    default:
                        break;

                }

                break;

            case NM_LOGIN_REQUEST: 
                // set ax to default login result, copy username, password
                ax=BAD_LOGIN;
                strcpy(szTemp,"null");
                strcpy(username , Recv->pRead());
                strcpy(szTemp4  , Recv->pRead());

                // check for NULL result
                if((username==NULL) || (!strlen(username))){
                    strcpy(szTemp,"Bad network message");
                    bDelete=true;
                    break;

                load();

                // if(dbr_nrow==0)                    ax=NEW_ACCOUNT;

                // check for maximum players logged in
                if(pServer->num_clients() > 
                        atoi(pServer->pVars->get_cvar("i_max_clients")))
                    ax=TOO_MANY_PLAYERS;

                // check to see if player is already logged in
                // SYSTEM is reserved
                // add any other reserved usernames here
                if(dlcs_strcasecmp(username,"SYSTEM"))
                    ax=ALREADY_LOGGED_IN;

				other_client=pServer->pFirstPlayer;
                while(other_client){
                    if(other_client!=this)
                    if(dlcs_strcasecmp(other_client->username,username))
                        ax=ALREADY_LOGGED_IN;
                    other_client=other_client->pNext;
                }

                // compare passwords
                // if(dbr_nrow==0){                    ax==BAD_LOGIN;                }
                // else{
                    if(ax==BAD_LOGIN){
                        // if((dlcs_strcasecmp(szTemp4,db_getvalue("username",username,"password").c_str() )))
                            ax=GOOD_LOGIN;
                    }
                //}

                switch(ax){
                    case BAD_LOGIN:         strcpy(szTemp,"Incorrect password...");            break;
                    case ALREADY_LOGGED_IN: strcpy(szTemp,"This user is already logged in from another machine...");       break;
                    case ACCOUNT_EXPIRED:   strcpy(szTemp,"Your account has expired. Contact system administrator.");      break;
                    case TOO_MANY_PLAYERS:  strcpy(szTemp,"Server is full. Too many players already logged in...");  break;
                    case GOOD_LOGIN: 
                        if(!strlen(username)){
                            ax=BAD_LOGIN;
                            strcpy(szTemp,"Bad network try again.");
                            break;
                        }
                        break; 
                    case NEW_ACCOUNT: 
                        if(dlcs_strcasecmp(pServer->pVars->get_cvar("b_new_accounts"),"1")){

                            // if(!pServer->r_data.b_require_website){

                                ax=GOOD_LOGIN;
                                strcpy(szTemp,"Welcome to the server! New account created.");
                                strcpy(password, szTemp4);
                                access=0;
                                pServer->add_user(username,0);
                                // db_update("password",password);
                                // pServer->db_query((char *)va("INSERT INTO users VALUES ('%s','%s', 0)",username,szTemp4));
                            //}
                            //else{

                                //ax=BAD_LOGIN;
                                //strcpy(szTemp,va("Please register @ %s", pServer->r_data.s_website_link));
                            //}
                             
                            break;
                        } else {
                            ax=BAD_LOGIN;
                            strcpy(szTemp,"Sorry, we're not accepting new accounts at this time. Try again later.");
                            break;
                        }
                        break;
                    default:
                        break;
                }

                if(ax==GOOD_LOGIN){
                    strcpy(szTemp,"Welcome to the server.");
                }


                pServer->create_guid(username,session_id);

                /*
                db_update("access","32");
                db_queryl("select * from users where username=%s",username);

                pServer->Log("%s.access [%d]",username,
                atoi((char *)db_getvalue("username",username,"access").c_str()) );
                 

                Send.Reset();
                Send.Write((char) NM_LOGIN_REQUEST_REPLY);
                Send.Write((char)   ax);
                Send.Write((char *) szTemp);
                Send.Write((char *) VERSION);
                Send.Write((char *) session_id);
                Send.Write((char *) "sys.media");
                Send.Write((char *) pServer->r_data.s_name);
                Send.Write((char *) pServer->r_data.s_admin_email);
                Send.Write((int)    atoi((char *)db_getvalue("username",username,"access").c_str()));
                Send.Write((char *) "access what");
                Send.Write((char)   10); // character slots
                SendUnreliableMessage(&Send);
                 */

                if(ax!=GOOD_LOGIN){
                    bDelete=true;
                    return;
                }

                pServer->Log("%s logged in from %s:%d",username,inet_ntoa(ToAddr.sin_addr),ntohs(ToAddr.sin_port));

                tv.clear();
                // tv=dlcs_explode(",",db_getvalue("username",username,"chat_channels").c_str());
                
                for(i=0;i<tv.size();i++){
                    join_channel(tv[i].c_str());
                }

                break; }

            case NM_LOGOUT: 

                bDelete=true;
                pServer->Log("%s logged out",username);

                break;

            case NM_GUI_CALLBACK:

                strcpy(szTemp3,Recv->pRead());
                ax=Recv->cRead(); // number of data to get
                for(j=0;j<ax;j++)
                {
                    strcpy(szTemp ,Recv->pRead());
                    strcpy(szTemp2,Recv->pRead());
                    pairdata[szTemp]=szTemp2;
                    // for now, only pull out console function
                    if(dlcs_strcasecmp(szTemp,"console")){
                        pServer->Log("%s GUI_CALLBACK (%s)",username,szTemp2);
                    }
                }
                break;

            case NM_GET_SERVER_INFO:

                switch(Recv->cRead()){
                    case NETMSG_SERVERINFORMATION:
                        break;
                    default:
                        break;
                }

                break; 

            case NM_MESSAGE: // Global Chat / System Message

                inactivity_timer->Reset();

                strcpy(szTemp,Recv->pRead());
                ax=Recv->iRead();

                switch(ax){ // channel

                    case CHANNEL_LOCAL:
                    case CHANNEL_TRADE:
                    case CHANNEL_GENERAL:
                    case CHANNEL_WHISPER:
                    case CHANNEL_PARTY:
                    case CHANNEL_RAID:
                    case CHANNEL_SAY:
                    case CHANNEL_YELL:
                    case CHANNEL_CLAN:
                    case CHANNEL_SYSTEM:

                        pServer->chat(this,this->username,(char *)va("%s:%s",username,szTemp),ax);
                        break;

                    case CHANNEL_CUSTOM:
                        break;

                    case CHANNEL_CONSOLE:

                        inactivity_timer->Reset();
                        strcpy(szTemp,Recv->pRead());
                        memset(szTemp2,0,1024);
                        j=0;
                        for(i=0;i<strlen(szTemp);i++)
                        {
                            if(szTemp[i]=='\'')
                            {
                                szTemp2[j]='\\';
                                j++;
                                szTemp2[j]='\'';
                                j++;
                            }
                            else
                            {
                                szTemp2[j]=szTemp[i];
                                j++;
                            }
                        }
                        for(i=0;i<strlen(szTemp2);i++) szTemp2[i]=szTemp2[i+1]; // should have a / in front, remove it
                        //l_interpret(va("world.client[%d]:con_parse(\"%s\")",windex,szTemp2));
                        break;

                    default:
                        break;
                }
                break; 

            case NM_MOVEPLAYER:

                break;

            case NM_CREATE_CHARACTER:
                /* inactivity_timer->Reset();
                client->avatar->hail(Recv->cRead());
                strcpy(client->avatar->name,Recv->pRead());

                // call hook to do stuff with client here...

                client->avatar->x=15015;
                client->avatar->y=15015;
                client->avatar->z=15015;

                client->avatar->save(client->avatar->slot); // Save the character slot
                Send.Reset();
                Send.Write((char)NM_CREATE_CHARACTER);
                Send.Write((char)client->avatar->slot);
                client->SendUnreliableMessage((CPacket *)&Send);
                */

                break;

            case NM_DELETE_CHARACTER:
                inactivity_timer->Reset();
                cx = Recv->cRead();
                 // !DoesHaveAccess(client->cGetAccess(),"fast_char_delete"))

                    //avatar->load(cx);
                    //tTime=localtime(&client->CL_Data->tCreationTime);
                    //ax=tTime->tm_year;
                    //ay=tTime->tm_yday;
                    //time(&fmgsTime);
                    //tTime=localtime(&fmgsTime);
                    //bx=tTime->tm_year;
                    //by=tTime->tm_yday;
                    //if(bx==ax)
                    //-
                    //    if( (by-ay) < l_sys("minimum_char_age)
                    //    -
                    //        sprintf(szTemp,"Character must age at least %s real days!",l_sys("minimum_char_age"));
                    //        Announce(client,szTemp,50,175,255);
                    //        break;
                    //    -
                    // -
                
                //client->avatar->wipe();
                //client->avatar->save(cx);
                Send.Reset();
                Send.Write((char)NM_DELETE_CHARACTER);
                Send.Write((char)cx);
                SendUnreliableMessage((CPacket *)&Send);

                break;

            case NM_RETRIEVECHARS:
             /* inactivity_timer->Reset();
                client->avatar->wipe();
                for(i=0;i<atoi(l_sys("character_slots"));i++)
                {
                    client->avatar->load(i);
                    if(strlen(client->avatar->name))
                    {
                        Send.Reset();
                        Send.Write((char)NM_RETRIEVECHARS);
                        Send.Write((char)i);
                        Send.Write((char *)client->avatar->name);
                        client->SendUnreliableMessage((CPacket *)&Send);
                        client->avatar->wipe();
                    }
                }*/

                break; 

            case NM_RETRIEVECHARINFO:

                /*inactivity_timer->Reset();
                cx=Recv->cRead();
                if(client->in_limbo==false)
                {
                    client->avatar->load(cx);
                    client->in_limbo=true;
                    Send.Reset();
                    Send.Write((char)NM_RETRIEVECHARINFO);
                    Send.Write((char *)client->avatar->name);
                    Send.Write((int)client->sid);
                    Send.Write((char)client->avatar->direction);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    client->SendUnreliableMessage((CPacket *)&Send);
                    MovePlayer(client,client->avatar->x,client->avatar->y,client->avatar->z,1);
                }*/

                break; 
                
            case NM_MODIFY_CHARACTER:
                inactivity_timer->Reset();

              /*if(!DoesHaveAccess(client->cGetAccess(),"modify_character"))
                {
                    // Player doesn't have access to change character information
                    // Send back a refuse request message
                    Announce(client,"Can't modify your character. You don't have the access.",255,0,0);
                    Log("%s(%s) tried to modify character!",client->avatar->name,client->avatar->name);
                    break;
                }

                Send.Reset();
                Send.Write((char)NM_MODIFY_CHARACTER);
                Send.Write((int)Recv->iRead());

                switch(Recv->cRead())
                {
                    case MC_RACE:
                        client->SetRace(Recv->cRead());
                        Send.Write((char)MC_RACE);
                        Send.Write((int)client->GetRace());
                        break;

                    case MC_GENDER:
                        client->SetGender(Recv->cRead());
                        Send.Write((char)MC_GENDER);
                        Send.Write((char)client->GetGender());
                        break;

                    case MC_CLASS:
                        client->SetClass(Recv->cRead());
                        Send.Write((char)MC_CLASS);
                        Send.Write((int)client->GetClass());
                        break;

                    default:
                        break;
                }

                client->avatar->save("users");

                // Send only to on screen players
                // do really all players need this information?
                // This packet will be sent when your characters appearance
                // changes somehow, if there are players on screen, go ahead
                // and describe the appearance so the client can render it

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    if( ( other_client->GetMapX() >= ( client->GetMapX()-1) ) &&
                        ( other_client->GetMapX() <= ( client->GetMapX()+1) ) &&
                        ( other_client->GetMapY() >= ( client->GetMapY()-1) ) &&
                        ( other_client->GetMapY() <= ( client->GetMapY()+1) ) &&
                        ( other_client->GetMapZ() ==   client->GetMapZ()    ) )
                    {
                        other_client->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client=other_client->pNext;
                }
                */

                break; 

            case NM_CLIENT_SHUTDOWN:

                other_client=pServer->pFirstPlayer;
                while(other_client)
                {
                    if(other_client != this)
                    {
                        Send.Reset();
                        Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
                        Send.Write((char *)session_id);
                        other_client->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client=other_client->pNext;
                }
                bDelete=true;
                return;

                break;

            case NM_HEARTBEAT:

                //pServer->Log("Heartbeat check from %s",username);
                bHeartBeatCheck=false; // OK, clear the heart beat check
                break;

            case NM_PING:

                inactivity_timer->Reset();
                gx=Recv->dwRead();
                Send.Reset();
                Send.Write((char)NM_PING);
                Send.Write((long)gx);
                SendUnreliableMessage((CPacket *)&Send);

                break; 

            case NM_FILE_XFER:

                inactivity_timer->Reset();
                char *pFileBuf;
                FILE *pFile;
                bool bFileFail;
                switch(Recv->cRead())
                {
                    // NET_FILE_RES_MEDIA
                    // NET_FILE_RES_SCRIPT
                    /*

                    case NET_FILE_NOP:
                        break;

                    case NET_FILE_START:
                        // filename
                        // filesize
                        // start file, put a temporary sequencer in temp folder for the file
                        // ie; hey.bmp will create temp/hey.bmp.sequence
                        strcpy(szTemp1,Recv->pRead());
                        cx=Recv->iRead();
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp3,"wb");
                        if(pFile)
                        {
                            fclose(pFile);
                            pFile=fopen(szTemp2,"wt");
                            if(pFile)
                            {
                                bx=1;
                                fputs(va("%d\n%d\n",bx,cx),pFile);
                                fclose(pFile);
                                SendData.Reset();
                                SendData.Write((char)NM_FILE_XFER);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((char)NET_FILE_START_OK);
                                SendNetMessage(0);
                            }
                            else bFileFail=true;
                        }
                        else bFileFail=true;
                        if(bFileFail)
                        {
                            SendData.Reset();
                            SendData.Write((char)NM_FILE_XFER);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((char)NET_FILE_ERROR);
                            SendNetMessage(0);
                        }

                        break;
                        */

                    /*
                    case NET_FILE_DATA:

                        // sequence number
                        // filename
                        // data block (1024)

                        ax=Recv->iRead(); // sequence number
                        strcpy(szTemp1,Recv->pRead()); // filename
                        pFileBuf=Recv->pRead(NET_FILE_XFER_BLOCK_SIZE);
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp2,"rb");
                        if(pFile)
                        {
                            fgets(szTemp4,256,pFile); bx=atoi(szTemp4);
                            fgets(szTemp4,256,pFile); cx=atoi(szTemp4);
                            fclose(pFile);
                        }
                        else bFileFail=true;
                        if(ax==bx)
                        {
                            pFile=fopen(szTemp3,"a");
                            if(pFile)
                            {
                                fwrite(pFileBuf,NET_FILE_XFER_BLOCK_SIZE,1,pFile);
                                fclose(pFile);
                                pFile=fopen(szTemp2,"wt");
                                if(pFile)
                                {
                                    bx++;
                                    fputs(va("%d\n%d\n",bx,cx),pFile);
                                    fclose(pFile);
                                }

                                SendData.Reset();
                                SendData.Write((char)NM_FILE_XFER);
                                SendData.Write((char)NET_FILE_DATA_OK);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((int)bx);
                            }
                            else
                            {
                                bFileFail=true;
                            }

                        }
                        if(bFileFail)
                        {
                            SendData.Reset();
                            SendData.Write((char)NM_FILE_XFER);
                            SendData.Write((char)NET_FILE_DATA_RESEND);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((int)bx);
                        }
                        break;

                    case NET_FILE_ACK:
                        break;

                    case NET_FILE_ABORT:
                        break;

                    case NET_FILE_EOF:

                        // sequence number
                        // filename
                        // size of data block
                        // data block

                        ax=Recv->iRead(); // sequence number
                        strcpy(szTemp1,Recv->pRead()); // filename
                        dx=Recv->iRead();
                        pFileBuf=Recv->pRead(dx);
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp3,"a");
                        if(pFile)
                        {
                            fwrite(pFileBuf,dx,1,pFile);
                            fclose(pFile);
                            remove(szTemp2);
                        }
                        else
                        {
                            SendData.Reset();
                            SendData.Write((char)NM_FILE_XFER);
                            SendData.Write((char)NET_FILE_EOF_RESEND);
                            SendData.Write((char *)szTemp1);
                        }
                        break;

                    case NET_FILE_RESUME:
                        break;
                        */

//						case NET_FILE_START_OK:
//							break;

                    default:
                        break;

                    }

                break; 

            case NM_NOP:
                if (strcmp(Recv->pRead(),"1234567890")==0)
                {
                    Send.Reset();
                    Send.Write((char)NM_NOP);
                    Send.Write("0987654321");
                    SendUnreliableMessage(&Send);
                }

                break;

            default:
                break;
        }
    }

    if(inactivity_timer->Up()){ // Check for inactivity

        // pServer->Log("No activity from %s",username);

        Send.Reset();
        Send.Write((char)NM_HEARTBEAT);
        Send.Write((int)1);
        SendUnreliableMessage((CPacket *)&Send);
        bHeartBeatCheck=true;
        heartbeat_timer->Reset();
        inactivity_timer->Reset();

        if(timeout_override==false){

            // Log("%s(%s) being kicked due to inactivity.",client->avatar->name,client->avatar->name);
            inactivity_timer->Reset(); //to prevent server from doing this twice (or more)

            other_client=pServer->pFirstPlayer;
            while(other_client){

                // change to only send to onscreen players later...
                if(other_client != this){
                    Send.Reset();
                    Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
                    Send.Write((char *)session_id);
                    other_client->SendUnreliableMessage((CPacket *)&Send);
                }
                other_client=other_client->pNext;
            }
            pServer->disconnect(this,"Inactivity");
        }
    }

    if(bHeartBeatCheck) { // Check for heartbeats and remove if not found
        if(heartbeat_timer->Up())
            pServer->disconnect(this,"Network disconnected");
    }

}

void C_GSC::db_update(string s_col,string s_val){

    /*
    db_query(va("UPDATE users SET %s = '%s' where username = '%s'",
            (char *)s_col.c_str(),
            (char *)s_val.c_str(),
            username    ));
             */
}

void    C_GSC::leave_channel(const char *channel) {
    
}
void    C_GSC::join_channel(const char *channel) {
    
}


/////////////////////////////////////////
CServer::CServer(bool bIsQuiet){
    NET_Init();
    bQuiet=bIsQuiet;
    bQuit=false;
    bRestart=false;
    dlcsm_randomize();
    pLog = new CLog("test_server.log",bQuiet);
    if(!pLog) { printf("\nNo memory for log!\n"); exit(0); }
    pLog->On();
    pLog->Restart();
    pVars = new CVarSet("test_server.ini");
    start_up();
    memset(szConsoleInput,0,1024);
    pFirstPlayer=0;
    initSocket();
    if(Listen(atoi(pVars->get_cvar("i_port")), true)==-1)
        Log("ERROR LISTENING ON PORT %d\n",pVars->get_cvar("i_port"));
    Log("Listening on port [%d]", iGetLocalPort());
}
/////////////////////////////////////////
CServer::~CServer(){
    kick_all("Server shutting down");
    C_GSC*c=pFirstPlayer;
    while(c) {
        pFirstPlayer=c;
        c=c->pNext;
        DEL(pFirstPlayer);
    }
    shut_down();
    NET_Shutdown();
    DEL(pLog);
}
/////////////////////////////////////////
bool CServer::check_restart(void) {
    if(bRestart) {
        Log("[Initiating server restart]=====================================");
        shut_down();
        start_up();
        Log("==============================================[Server restarted]");
        bRestart=0;
        return true;
    }
    return false;
}
/////////////////////////////////////////
void CLog::_Add(const char *fmt, ...){
    char ach[1024];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    AddEntryNoTime(ach);
}
/////////////////////////////////////////
void CServer::Log(string s) { Log((const char *)s.c_str()); }
void CServer::Log(const char *fmt, ...){
    char ach[1024];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    pLog->_Add(ach);
}
/////////////////////////////////////////
void CServer::start_up(void){
    ////////////////////////////////////////////////////////////////////////////
    // Setup server variables
    // dfGUI_CHAT();
    start_time=dlcs_get_tickcount();
    int i=0;
    int j=0;
    int k=0;
    char t[1024]; memset(t,0,1024);
/*  Log("            ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
    Log("            º.  °°°°°° °°   °° °°°°°. °°°°°° °°°°°    º");
    Log("            º   °°     °°° °°° °°  °° °° .   °° .°°   º");
    Log("            º   ±±±±   ±±°°°±± °°°±±  ±°°°±  °°  ±°.  º");
    Log("            º   ²±    .±± ± ±± ±±  ²± ±±     ±±²²±    º");
    Log("            º  .²²     ²².  ²² ±²  ²² ²²     ²²  ²² . º");
    Log("            º   ²ÛÛ²²Û ÛÛ   ²Û ²ÛÛ²Û  Û²²Û²² Û²  Û²   º");
    Log("            ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");
    Log("EGS Version (%s) Patch (%s) Net Revision (%s) %s",VERSION,EGS_REVISION,NET_REVISION,COPYRIGHT);
    Log("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛßßßßÛßßßßßßßßÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛ²±°");
    Log("                  ÜÛÛÜ ÜÛÛÛÛÛß     ÜÛÛ             ÛÛ ");
    Log("                ÜÛß      ÞÛ  ÜÛÛ  Ûß ßÛ  ÜÛÛÛÛÛß  ÞÛ  ");
    Log("                ßÛÜÜÜ    ÛÝ Ûß Û ÛÝ ÜÛ     ÞÛ     Û   ");
    Log("                  ßßÛÛ  ÞÛ ÞÛ ÜÛ ÛÛßÛÛ     ÛÝ    ÞÝ   ");
    Log("                ÜÜ  ÞÛ  ÛÝ ÛÛßÛÛ ÛÛ  ÞÛ   ÞÛ          ");
    Log("                 ßßßß      ÛÝ ÛÝ Û    Û   ÛÝ    Û     ");
    Log("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÜÜÜÜÜÜÜÜÜÛÛÜÜÜÛÜÜÜÛÛÛÜÜÜÛÛÛÛÛÛÛÛÛÛÛÛÛ²±°"); */

    Log("Test Server %s(%s) Net(%d) Build(%d) %s",VERSION,CPUSTRING,NET_REVISION,TEST_S_BUILD,COPYRIGHT);
    // Log(dlcs_get_os_version());
    dlcs_suspend_power_management();
    SetupConsoleHistory();
    ////////////////////////////////////////////////////////////////////////////
    // Open User Database

    /* i = sqlite3_open("data.sqlite",&pDB);
    if(i){
        Log("SQLite version %s failed to load file data.sqlite",SQLITE_VERSION);
        sqlite3_close(pDB);
    }else{
        Log("SQLite version %s [data.sqlite]",SQLITE_VERSION);
    }
     */

    // db_query("update users set chat_channels = 'SYSTEM'",0);
    //db_query("alter table users add column NAME char(256) default 'SYSTEM'");

/*
    if(db_query("select * from users")!=SQLITE_OK){

        Log("=====================[WARNING! ERROR!]======================");
        Log("Database [data.sqlite] empty or corrupt");
        Log("=====================[WARNING! ERROR!]======================");

        bQuit=true;

        // where username='seth'
        //db_query("CREATE TABLE users (username varchar(32), password varchar(32), access smallint)",0);
        //db_query("CREATE TABLE users(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT default 'user',password TEXT default 'none',access smallint default '0',chat_channels TEXT default 'SYSTEM')",0);
        //db_query((char *)va("INSERT INTO users VALUES (1, 'seth', '%s', 255)",md5_digest("123")),0);
        //db_query((char *)va("INSERT INTO users (username) VALUES ('seth_also')",0));
        //db_query((char *)va("INSERT INTO users VALUES ('seth','%s', 255)",md5_digest("123")),0);
        //db_query((char *)va("INSERT INTO users VALUES ('test','%s', 5)",  md5_digest("226fi3")),0);
        //db_query((char *)va("INSERT INTO users VALUES ('test2','%s', 6)", md5_digest("2326df3")),0);
        //db_query((char *)va("INSERT INTO users VALUES ('test4','%s', 7)", md5_digest("223k6gf3")),0);
        //db_query((char *)va("INSERT INTO users VALUES ('zany','%s', 8)",  md5_digest("22lg63f3")),0);
    }
    else {
        //Log("seth.password=[%s]",       (char *)  db_getvalue("username","seth","password").c_str());
        //Log("seth.access=[%d]",     atoi((char *) db_getvalue("username","seth","access").c_str()));
    }
     */

    load_data();
    
    Log("name  = [%s]",pVars->get_cvar("s_name"));
    Log("admin = [%s]",pVars->get_cvar("s_admin_email"));
    Log("web   = [%s]",pVars->get_cvar("s_website_link"));
    Log("MOTD  = [%s]",pVars->get_cvar("s_motd"));

    load_world();

    Log("(Type '/help' for help on commands)");
    //Log("АБВллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллВБА");

    C_GSC *c_x=0;
    c_x=new C_GSC(pLog,this);
    strcpy(c_x->username,"seth");
    c_x->load();
    // CMPCharacter* pc=new CMPCharacter(c_x);
    // pc->load();
    //    pc->gender=1;
    // pc->health->current_value=2;
    DEL(c_x);
    // DEL(pc);

}
/////////////////////////////////////////
void CServer::shut_down(void){

    save_data();

    ////////////// MAP STUFF

    save_world();

    RemoveConsoleHistory();

    // sqlite3_close(pDB);

    Log("Closed user database");

    ///////////////////////////////////////////
/*
    Log("°±²ÛÛÛÛÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÛÛÛÛ²±°");
    Log("        ÜÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÜ ");
    Log("        Û        ÜÛÛÛÜ                      ÜÛÛÛÜ        Û ");
    Log("        Û        ÛúÛÜÛ    ßÛÜß ß Û   Û      ÛÜÛúÛ        Û ");
    Log("        Û        ßÛÛÛß     Û Û Û Û   Û      ßÛÛÛß        Û ");
    Log("        Û         ÝÝÞ      ß Û ß ßßß ßßß     ÝÞÞ         Û ");
    Log("        Û         þÜþ     Server Killed!     þÜþ         Û ");
    Log("        ßÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛß ");
    Log("°±²ÛÛÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛÛÛ²±°");
    Log("    °±²ÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛ²±°");
    Log("         ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß");
    */
    Log("Server killed");

}
/////////////////////////////////////////
void CServer::do_keyboard(){

    u_char ch=0;
    int i;

    if(_kbhit()){
        ch=_getch();
        switch(ch){

            case 0:
                ch=_getch();
                switch(ch){

                    default:
                    break;
                }
                break;

            case 13:
            case 10:

                AddToConsoleHistory(szConsoleInput);
                ConsoleHistoryPosition=0;

                Log(">%s",szConsoleInput);

                if(szConsoleInput[0]==0) break;
                else{
                    console_command(szConsoleInput);
                    memset(szConsoleInput,0,1024);
                }
                break;

            case 27:
                break;

            case 8: // backspace
                i=strlen(szConsoleInput);
                szConsoleInput[i-1]=0;
                printf(">                                                  \r>%s \r",szConsoleInput);

                break;

            case 21: // ctrl-U
                printf(">                                                  \r>");
                memset(szConsoleInput,0,1024);
                break;

            case 0xE0: // Arrow keys...
                ch=_getch();
                switch(ch)
                {
                    case 72:    // UP
                        strcpy(szConsoleInput,
                        GetConsoleHistory(
                        ConsoleHistoryPosition));
                        ConsoleHistoryPosition++;
                        if(
                        ConsoleHistoryPosition>MAX_CONSOLE_INPUT_BUFFER)
                            ConsoleHistoryPosition=0;
                        printf(">                                                  \r>%s \r",szConsoleInput);
                        break;

                    case 80:    // DOWN
                        strcpy(szConsoleInput,
                        GetConsoleHistory(ConsoleHistoryPosition));
                        ConsoleHistoryPosition--;
                        if(ConsoleHistoryPosition<0)
                            ConsoleHistoryPosition=MAX_CONSOLE_INPUT_BUFFER;
                        printf(">                                                  \r>%s \r",szConsoleInput);
                        break;

                    case 75:    // LEFT (TODO: build cursor into console command input)
                        break;

                    case 77:    // RIGHT (TODO: build cursor into console command input)
                        break;

                    default:
                        break;
                }
                break;

            default:
                i=strlen(szConsoleInput);
                szConsoleInput[i]=ch;
                szConsoleInput[i+1]=0;
                printf(">                                                  \r>%s \r",szConsoleInput);
                break;
        }

    }
}
/////////////////////////////////////////
void CServer::cycle(void){

    static C_GSC *pClient=0;

    if(pClient==0) pClient=pFirstPlayer;
    if(pClient){
        pClient->do_net();
        pClient=pClient->pNext;
    }

    check_restart();
    do_keyboard();
    accept_connection();
    do_world();
    purge_clients();

}
/////////////////////////////////////////
void CServer::accept_connection(void){

	C_GSC           *client;

    int             iLength,iFlags,iHelp;
    int             iAcceptSocket;
    char            cType;

    struct sockaddr_in ServerAddr;
    memset(&ServerAddr,0,sizeof(ServerAddr));

    CPacket Send(NET_DATAGRAMSIZE);
	CPacket Recv(NET_DATAGRAMSIZE);

	iAcceptSocket = CheckNewConnections();
    if(iAcceptSocket == -1) return;

    if((iLength = nRecv(    iAcceptSocket,
                            Recv.pGetPacketBuffer(),
                            Recv.iGetMaxSize(),
                            (struct sockaddr*)&ServerAddr))<=0) return;

    Recv.SetCurSize(iLength);

    iHelp   = ntohl(Recv.iRead());
    iFlags  = iHelp & (~NET_FLAG_LENGTH_MASK);
    iHelp  &= NET_FLAG_LENGTH_MASK;

    if (!(iFlags & NET_FLAG_CTL))   return;
    if (iLength != iHelp)           return;
    if (iLength < sizeof(int)*2)   return;

    Recv.iRead();
    cType=Recv.cRead();

    switch(cType)
    {
        case CTL_CONNECT:

            if(strcmp(Recv.pRead(),"MANTRA")){
                Log("Fail attempt, not MANTRA packet");
                reject_connection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"This is a Ember Game Server");
                return;
            }

            if(Recv.iRead() != NET_REVISION){
                reject_connection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server uses newer protocol! Update available.");
                return;
            }

            // TODO: Add reject (PRIVATE SERVER)

            // create new client object
            client=new C_GSC(pLog,this);
            if(!client)	{ Log("manserver.server.cpp -> Can't create client"); return; }


            if(!client){
                reject_connection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server can't assign a new socket!");
                return;
            }

            memcpy(&client->ToAddr,&ServerAddr,sizeof(ServerAddr));

			client->iSocket=zOpenSocket(0);
            if(client->iSocket==-1){
				DEL(client);
				reject_connection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server can't assign new socket!");
				return;
			}

			Send.Reset();
			Send.Write(0);
			Send.Write(0);
			Send.Write((char)CTL_ACCEPT);
			Send.Write(client->iGetLocalPort());
			FinishCtlPacket(&Send);

            if(nSend(iAcceptSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),(struct sockaddr*)&ServerAddr)==-1){
				DEL(client);
				Log("s_server.cpp -> CSocket::pAccept() -> error making a new connection");
				return;
			}

			client->bCanSend = true;
            client->bConnected = true;

            break;

        case CTL_PING:

            Send.Reset();
            Send.Write(0);
            Send.Write(0);
            Send.Write((char)CTL_PING);
            Send.Write((long)Recv.dwRead());
            FinishCtlPacket(&Send);

            if(nSend(iAcceptSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),(struct sockaddr*)&ServerAddr)==-1){
                Log("s_server.cpp -> respond to pingrequest failed");
            }
            return;

        case CTL_REJECT:
            Log("CTL_REJECT ?");
            return;

        default:
            reject_connection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"This is a Mantra gameserver");
            Log("UNKNOWN CTL_PACKET (%d) received",cType);
            return;
    }


	// insert client into client list
    if(pFirstPlayer) client->pNext=pFirstPlayer;
    pFirstPlayer=client;


}
/////////////////////////////////////////
void CServer::reject_connection(int iReSocket,sockaddr *pReAddr,const char *reason){
    CPacket Send(NET_DATAGRAMSIZE);
    if(!reason) return;
    Send.Reset();
    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_REJECT);
    Send.Write((char *)reason);
    FinishCtlPacket(&Send);
    if(nSend(iReSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),pReAddr)==-1){
        Log("Reject message failed (%s)",reason);
    }
}
/////////////////////////////////////////
void CServer::send_all(CPacket *pPacket,float fBlockTime){
    C_GSC *pClient;
    pClient=pFirstPlayer;
    while(pClient){
        if(!pClient->bDelete) pClient->SendUnreliableMessage(pPacket);
        pClient=pClient->pNext;
    }
}
/////////////////////////////////////////
void CServer::disconnect(C_GSC *client,const char *reason){
    CPacket Send(NET_DATAGRAMSIZE);

    if(!client) return;
    if(client){
        Send.Reset();
        Send.Write((char)NM_CLIENT_SHUTDOWN);
        Send.Write((char *) client->session_id);
        Send.Write((char *) reason);
        client->SendUnreliableMessage(&Send);
    }

    Send.Reset();
    Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
    Send.Write((char *) client->session_id);
    send_all(&Send,0);

    client->bDelete=true;
}
/////////////////////////////////////////
void CServer::save_world(void){

	static long dwSaveTimer=dlcs_get_tickcount();
	if(	(dlcs_get_tickcount()-dwSaveTimer) > 100000 ){
		dwSaveTimer=dlcs_get_tickcount();
		Log("World saved...");
	}
}
/////////////////////////////////////////
void CServer::load_world(void){
	Log("World loaded...");
}
/////////////////////////////////////////
C_GSC *CServer::get_client(const char *user_name){
    C_GSC *find = pFirstPlayer;
    while(find) {
        if(dlcs_strcasecmp(find->username,user_name)) return find;
        find=find->pNext;
    }
    return 0;
}
/////////////////////////////////////////
void CServer::kick_all(const char *reason){
    C_GSC *pClient;
    pClient = pFirstPlayer;
    if(!pClient) return;
    Log("Kicking online players");
    while(pClient){
        Log("       ....%s", pClient->username );
        disconnect(pClient,reason);
        pClient=pClient->pNext;
    }
    Log("All players kicked!");
}
/////////////////////////////////////////
void CServer::kick_user(const char *name,const char *reason){
    C_GSC *client = pFirstPlayer;
    while(client){
        if(dlcs_strcasecmp(client->username,name)){
            disconnect(client,reason);
            return;
        }
        client=client->pNext;
    }
    Log("%s is not online!",name);
    return;
}
/////////////////////////////////////////
void CServer::add_user(const char *in,u_char ia){
    //     db_query(va("insert into users (username,access) values ('%s',%d)",in,ia));
}
/////////////////////////////////////////
void CServer::remove_user(const char *szName){
   // db_query("delete * from users where usersname = '%s'",szName);
}
/////////////////////////////////////////
void CServer::user_access(const char *in_user_name,int in_access){
    if(in_access>255) return;
    if(in_access<0) return;
    // db_query("update users set access=%d where username='%s'",in_access,in_user_name);
    C_GSC* c = get_client(in_user_name);
    if(c) c->access=in_access;
}
/////////////////////////////////////////
void CServer::chat(C_GSC *pClient,const char *from,const char *msg,int channel){

	CPacket Send(NET_DATAGRAMSIZE);

	Send.Reset();
	Send.Write((char)NM_MESSAGE);

	Send.Write((char *)msg);
    Send.Write((char *)from);
	Send.Write((int) channel); // CHANNEL
	if(pClient) Send.Write((char *)pClient->session_id);

	switch(channel){

        case CHANNEL_CLAN:
            break;

        case CHANNEL_GENERAL:
            break;

        case CHANNEL_TRADE:
            break;

        case CHANNEL_SAY:
        case CHANNEL_YELL:
        case CHANNEL_LOCAL:
            break;

        case CHANNEL_PARTY:
            break;

        case CHANNEL_RAID:
            break;

        case CHANNEL_SYSTEM:
            send_all((CPacket *)&Send,0);
            break;

        case CHANNEL_WHISPER:
            pClient->SendUnreliableMessage((CPacket *)&Send);
            break;

        case CHANNEL_CUSTOM:
            break;

        default:
            break;
    }
}
/////////////////////////////////////////
void CServer::purge_clients(void){

	C_GSC *pClient=0;
	C_GSC *pDelete=0;

	pClient = pFirstPlayer;
	if(!pClient) return;

	while(pClient->pNext){
        if(pClient->pNext->bDelete){
            pDelete=pClient->pNext;

            if(pClient->pNext->pNext)
                pClient->pNext=pClient->pNext->pNext;
                DEL(pDelete);
        }
        if(pClient->pNext)
            pClient = pClient->pNext;
	}
	if(pClient==pFirstPlayer){
        if(pClient){
            if(pClient->bDelete){
                DEL(pFirstPlayer);
            }
        }
	}
}
/////////////////////////////////////////
void CServer::do_world(void){
	static long dwWorldSaveTimer=dlcs_get_tickcount();

	save_world();
}
/////////////////////////////////////////
void CServer::console_command(char * command){
    int i;
    vector <string> v=dlcs_explode(" ",command);
    char temp[1024];   memset(temp,0,1024);
    char targs[1024]; memset(targs,0,1024);

    if(v.size()>0){

        strcpy(targs," ");
        if(v.size()>1){
            for(i=1;i<v.size();i++){
                strcat(targs,v[i].c_str());
                strcat(targs," ");
            }
        }

        if( dlcs_strcasecmp(v[0].c_str(),"/help") ){
            Log("[List of available commands]====================================");

            Log("/nuy.............................Allow new user accounts");
            Log("/nun.............................Do not allow new user accounts");

            Log("/ann <message>...................Announce a message to the entire server");
            Log("/msg <name> <message>............Send message to specific player");

            Log("/motd [new motd].................View, or change the message of the day");

            Log("/who.............................List online players");
            Log("/users...........................List users from database");
            Log("/add_user <name> [access]........Add new user to database");
            Log("/remove_user <name>..............Remove a user account");
            Log("/kick_user <name> [message]......Kick player with optional message");
            Log("/access <name> <access>..........Change player's access level");


            Log("/kill [minutes]..................Kill the server [minutes] (CTRL-C immediate)");
            Log("/unkill..........................Stop kill countdown");
            Log("/restart [minutes]...............Restart the server [minutes]");
            Log("/unrestart.......................Stop restart countdown");
            Log("/time............................See time / timestamp");

          /*Log("/ban <user>......................Ban player (will ban user)");
            Log("/banip <user>....................Ban player (will ban user's ip address)");
            Log("/unban <user>....................Unban player (will unban user)");
            Log("/unbanip <ipaddress>.............Unban player (will unban user's ip address)");
            Log("/bandomain <domain>..............Ban entire domain");
            Log("/unbandomain <domain>............Unban entire domain");
            Log("/banlist.........................List all banned ip's");*/

            Log("======================================================[End List]");
            return;
        }

        ///////////////////////////////////////////////////////////////////////////

        if( (dlcs_strcasecmp(v[0].c_str(),"/sql")) ){
            // db_queryl(targs);
        }

        //////////////////////////////////////////

        if( (dlcs_strcasecmp(v[0].c_str(),"/users")) ){
            // db_queryl("select username,access from users");
        }

        //////////////////////////////////////////

        if( (dlcs_strcasecmp(v[0].c_str(),"/add_user")) ){

            if(v.size()>1){
                if(v.size()>2){
                    add_user((char *)v[1].c_str(),atoi((char *)v[2].c_str()));
                } else {
                    add_user((char *)v[1].c_str(),0);
                }
            }
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/remove_user")){
            if(v.size()>1)
            remove_user((char *)v[1].c_str());
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/access")){
            if(v.size()>2){
                int dx = atoi(v[2].c_str()); // dx is access
                if( (dx<0) || (dx>255) ) dx=0;
                user_access((char *)v[1].c_str(),dx);
            }
        }

        //////////////////////////////////////////

        if( (dlcs_strcasecmp(v[0].c_str(),"/time")) ){
            dlcs_get_time(temp);
            Log(temp);
            dlcs_timestamp(temp);
            Log("timestamp: %s",temp);
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/ann")){
            if(v.size()>1){
                chat(0,"Admin",targs,CHANNEL_SYSTEM);
                Log(targs);
            }
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/msg")){
            if(v.size()>2){

                strcpy(targs," ");
                if(v.size()>2){
                    for(i=2;i<v.size();i++){
                        strcat(targs,v[i].c_str());
                        strcat(targs," ");
                    }
                }
                chat(get_client((char *)v[1].c_str()),"Admin",targs,CHANNEL_WHISPER);
            }
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/kick_user")){

            if(v.size()>2){
                kick_user((char *)v[1].c_str(),(char *)v[2].c_str());
            }
            else if(v.size()>1){
                kick_user((char *)v[1].c_str(),"Kicked by admin");
            }
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/kill")) bQuit=true;

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/restart")) bRestart=1;

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/nuy")) {
            pVars->set_cvar("b_new_accounts","1");
            Log("Now accepting new users");
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/nun")) {
            pVars->set_cvar("b_new_accounts","0");
            Log("New users will not be accepted");
        
        }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/who")){ C_GSC *c=pFirstPlayer; while(c){ Log("%s",c->username); c=c->pNext; } }

        //////////////////////////////////////////

        if(dlcs_strcasecmp(v[0].c_str(),"/motd")){
        }

        //////////////////////////////////////////
    }
}
/////////////////////////////////////////
char *CServer::GetConsoleHistory(int x){
    if(ConsoleHistory) return (ConsoleHistory[x].text);
    return (char *)(va("con history error![%d]",x));
}
/////////////////////////////////////////
void CServer::SetupConsoleHistory(void){
    ConsoleHistory=0;
    ConsoleHistory=new ConHistory[MAX_CONSOLE_INPUT_BUFFER+1];
    for(int i=0;i<MAX_CONSOLE_INPUT_BUFFER;i++)
        memset(ConsoleHistory[i].text,0,1024);
    ConsoleHistoryPosition=0;
}
/////////////////////////////////////////
void CServer::AddToConsoleHistory(char *s){
    for(int i=MAX_CONSOLE_INPUT_BUFFER;i>0;i--)
        strcpy(ConsoleHistory[i].text,ConsoleHistory[i-1].text);
    strcpy(ConsoleHistory[0].text,s);
}
/////////////////////////////////////////
void CServer::RemoveConsoleHistory(void){
    delete [] ConsoleHistory;
    ConsoleHistory=0;
}
/////////////////////////////////////////
bool CServer::load_data(void) {
    pVars->bLoad();
    Log("%s loaded",pVars->filename.c_str());
    return true;
}
/////////////////////////////////////////
bool CServer::save_data(void){
    pVars->bSave();
    Log("%s saved",pVars->filename.c_str());
    return true;
}
/////////////////////////////////////////
int CServer::num_clients(){
    int x=0;
    C_GSC* c=pFirstPlayer;
    while(c){
        x++;
        c=c->pNext;
    }
    return x;
}

/////////////////////////////////////////
void CServer::create_guid(const char *cin,char *cout){
    char t[1024]; memset(t,0,1024);
    char t2[1024]; memset(t2,0,1024);
    strcpy(t,va("%s%d",cin,rand()%99999));
    strcpy(t2,dlcs_md5_digest(t).c_str());
    dlcs_timestamp(t);
    strcpy(cout,va("%s-%s-%05d",t2,t,rand()%99999));
}
/////////////////////////////////////////
/*
void CServer::ReportToMaster(void){
    static bool starter=false; static long dwReportTimer=GetTickCount();
     //if(!dlcs_strcasecmp(gmvar("sys.master_report"),"yes")) return;
	 if(!pFMMS_Connection) return;
    if((GetTickCount()-dwReportTimer)<15000) {
        if(!starter) starter=true; else return;
    }
    dwReportTimer=GetTickCount(); //Log("Reported to master");
    pFMMS_Connection->Connect("127.0.0.1","40404");
}
*/
