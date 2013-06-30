/***************************************************************
 ** EMBER													  **
 ***************************************************************/

#include "c_netutil.h"
// #include "c_data.h"
// #include "c_gfxutil.h"
// #include "c_gui.h"
#include "c_cfmgs.h"

/**************************************************
 ** Network Loop                                 **
 **************************************************/

extern "C" void DoNetwork(void)
{
    CPacket *RecvData=NULL; // Recv Packet
    char temp1[1024]; memset(temp1,0,1024);
    char temp2[1024]; memset(temp2,0,1024);
    char temp3[1024]; memset(temp3,0,1024);
    char temp4[1024]; memset(temp4,0,1024);
    char szTemp1[1024]; memset(szTemp1,0,1024);
    char szTemp2[1024]; memset(szTemp2,0,1024);
    char szTemp3[1024]; memset(szTemp3,0,1024);
    char szTemp4[1024]; memset(szTemp4,0,1024);
    int i,ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez,fx,fy,fz;
    long bytetimer,gx,gy,gz;
    u_char vx,vy,vz;
    bytetimer=dlcs_get_tickcount();
    ax=ay=az=bx=by=bz=cx=cy=cz=dx=dy=dz=ex=ey=ez=fx=fy=fz=0;
    gx=gy=gz=0;
    vx=vy=vz=0;
    char NetMessage;
    int  iState;
    if(!pFMGS) return;// In case of accident
    iState=pFMGS->eGetState();
    /*******************************************************
    sprintf(temp1,"NET: Network status[%d]",iState);
    switch(iState)
    {
        case CON_NOTCONNECT:
            strcat(temp1,"-[CON_NOTCONNECT]");
            break;
        case CON_SYSBUSY:
            strcat(temp1,"-[CON_SYSBUSY]");
            break;
        case CON_LOGGINPROC:
            strcat(temp1,"-[CON_LOGGINPROC]");
            break;
        case CON_CONNECTED:
            strcat(temp1,"-[CON_CONNECTED]");
            break;
        default:
            strcat(temp1,"-[ERROR!]");
            break;
    }
    //DLog(temp1); // DebuggingDLog out string
    /*******************************************************/
    if(iState==CON_NOTCONNECT) return; // If you aren't connected, then return. No sense in trying to read packets if they aren't there.
    if( (iState!=CON_LOGGINPROC) && (iState!=CON_SYSBUSY) && (iState!=CON_CONNECTED) ) return;
    NetMessage=pFMGS->iGetMessage(); // Update the Network once per frame here
    if( (iState==CON_LOGGINPROC) || (iState==CON_SYSBUSY) ) return;
    if(NetMessage==-1)
    {
        // GAME_MODE=LOGIN_SCREEN_ENTRY;
        //pGUI->Prompt("Lost connection to server.","disconnect");
        pFMGS->SetState(CON_NOTCONNECT);
        return;
    }
    if(NetMessage) // Check for network message
    {
        RecvData=pFMGS->pSocket->pGetMessage();
        pFMGS->cLastNetMessage=RecvData->cRead();
        // //DLog("%d = LASTNETMESSAGE",pFMGS->cLastNetMessage);
        switch(pFMGS->cLastNetMessage)
        {


            /*********************************************************************************
             ** NETMSG_SERVERINFORMATION                                                    **
             *********************************************************************************/

			case NETMSG_SERVERINFORMATION:

				//Log("NETMSG_SERVERINFORMATION");

/*
				ax=RecvData->cRead();

				//Log("%d) Races",ax);

				for(i=0;i<ax;i++)
				{
					ay=RecvData->cRead();
					//strcpy(pClientData->ServerInfo->Race[ay].Text,RecvData->pRead());
					//Log("  %d) %s",ay,pClientData->ServerInfo->Race[ay].Text);

				}

				ax=RecvData->cRead();

				//Log("%d) Classes",ax);

				for(i=0;i<ax;i++)
				{
					ay=RecvData->cRead();
					//strcpy(pClientData->ServerInfo->Class[ay].Text,RecvData->pRead());
					//Log("  %d) %s",ay,pClientData->ServerInfo->Class[ay].Text);
				}

*/
				//SetGameMode(GET_CHARACTERS);
				break;



            /*********************************************************************************
             ** NETMSG_RETRIEVECHARS                                                        **
             *********************************************************************************/

			case NETMSG_RETRIEVECHARS:

				//Log("NETMSG_RETRIEVECHARS!");

//				pClientData->ClearCharacters();

				ax=RecvData->cRead();

				for(i=0;i<ax;i++)
				{

					strcpy(temp1,RecvData->pRead());// name
					strcpy(temp2,RecvData->pRead());// level
					strcpy(temp3,RecvData->pRead());// race
					strcpy(temp4,RecvData->pRead());// class
					strcpy(szTemp1,RecvData->pRead());// gender

//					strcpy(pClientData->ServerCharacter[i].t_name,temp1);
//					pClientData->ServerCharacter[i].t_level = atoi(temp2);
//					pClientData->ServerCharacter[i].t_race  = atoi(temp3);
//					pClientData->ServerCharacter[i].t_class = atoi(temp4);
//					pClientData->ServerCharacter[i].t_gender= atoi(szTemp1);

				}

				//SetGameMode(CHOOSE_CHARACTER);

				break;



            /*********************************************************************************
             ** NETMSG_RETRIEVECHARINFO                                                     **
             *********************************************************************************/

			case NETMSG_RETRIEVECHARINFO:

				// get info from server

				// if information is GOOD pass to GATHER_GAME_DATA mode

				// if information is BAD pass to CHOOSE_CHARACTER mode

				break;

            /*********************************************************************************
             ** NETMSG_LOGIN_REQUEST_REPLY                                                  **
             *********************************************************************************/

            case NETMSG_LOGIN_REQUEST_REPLY:

				//Log("Got a login request reply");

                //if(eGetGameMode()!=LOGIN_RECV) break;
                ax = RecvData->cRead();
                if((ax == GOOD_LOGIN) || (ax == NEW_ACCOUNT))
                {
					//Log(" >>> It's a good login!");

  					/*strcpy(pClientData->ServerMessage,  RecvData->pRead());
                    strcpy(pClientData->szServerVersion,RecvData->pRead()); // Extract Server's Version
                    pClientData->ID=                    RecvData->iRead();  // Extract Server Assigned SID
                    strcpy(pClientData->ServerID,       RecvData->pRead()); // Extract Server Resource ID
                    strcpy(pClientData->ServerName,     RecvData->pRead()); // Extract Server Name
                    strcpy(pClientData->ServerAuthor,   RecvData->pRead()); // Extract Server Admin
                    pClientData->Access=                RecvData->iRead();  // Extract Player's Access level
                    strcpy(pClientData->szAccessName,   RecvData->pRead()); // Extract Player's Access level name
                    pClientData->CharacterSlots=        RecvData->cRead();  // Extract Server num of character slots

                    if(pClientData->CharacterSlots>MAX_TOONS)
                        pClientData->CharacterSlots=MAX_TOONS;

                    SetGameMode(GET_SERVER_INFORMATION_START);
                    */
                    pFMGS->SetState(CON_CONNECTED);
                    //DLog("CON_CONNECTED!");
                }
                else
                {
					//Log(" >>> It's a bad name / password");
                    //SetGameMode(CHOOSE_SERVER);
                    //if(ax!=MASTER_LOGIN)
                       // pGUI->Prompt(RecvData->pRead(),"nop");
                }
                break;

            /*********************************************************************************
             ** NETMSG_HEARTBEAT                                                            **
             *********************************************************************************/

            case NETMSG_HEARTBEAT:  // just return the heartbeat... no other thing needs to be done...
            /*
                SendData.Reset();
                SendData.Write((char)NETMSG_HEARTBEAT);
                SendData.Write((int)1);
                SendNetMessage(0);
                //DLog("NET: Rx'd a Heartbeat request from server. Returned the same.");
                */
                break;


            /*********************************************************************************
             ** NETMSG_CLIENTSHUTDOWN                                                       **
             *********************************************************************************/

            case NETMSG_CLIENTSHUTDOWN:
                i=RecvData->iRead();
                strcpy(temp1,RecvData->pRead());

				//Log("FM: Server has shut down this client. Reason[%s]",temp1);

				sprintf(temp2,"Server has shut down this client. Reason:    %s",temp1);
                //pGUI->Prompt(temp2,"nop");

                // ChatBufferAdd(255,0,0,"SYSTEM","Server has shut down this client. Reason[%s]",temp1);

				NetworkSetup();
                //SetGameMode(LOGIN_SCREEN_ENTRY);
                break;

            /*********************************************************************************
             ** NETMSG_CHANGE_MODE                                                          **
             *********************************************************************************/

            case NETMSG_CHANGE_MODE:

                ax = RecvData->cRead();
                bx = RecvData->cRead();
                if(bx == false) return;
                switch(ax)
                {
                    case PLAY:
                        //DLog("Mode is now [PLAY]");
                        //pClientData->Mode=ax;
                        break;
                    case BUILD:
                        //DLog("Mode is now [BUILD]");
                        //pClientData->Mode=ax;
                        break;
                    case BUILD_GHOST:
                        //DLog("Mode is now [BUILD GHOST]");
                        //pClientData->Mode=ax;
                        break;
                    case SPECTATOR:
                        //DLog("Mode is now [SPECTATOR]");
                        //pClientData->Mode=ax;
                        break;
                    default:
                        //Log("NET: Set gameplay mode error! The value is out of range. Mode not changed.");
                        // //DLog(255,0,0,"SYSTEM","Mode not changed.");
                        break;
                }
                break;

            /*********************************************************************************
             ** NETMSG_MODIFY_MAP                                                           **
             *********************************************************************************/

            case NETMSG_MODIFY_MAP:
            /*
                if(!pOffScreenMap) { //DLog("FCOM: Map not setup yet, not changing tile..."); break; }
                cx=RecvData->cRead(); // Type of modification
                dx=RecvData->cRead(); // boolean
                ax=RecvData->iRead(); // map x
                ay=RecvData->iRead(); // map y
                az=RecvData->iRead(); // map z
                //DLog("NETMSG_MODIFY_MAP: [%d][%d][%d][%d][%d]",cx,dx,ax,ay,az);
                if(camera->in(ax-GMP_MAPSIZE,ay-GMP_MAPSIZE,az,ax+GMP_MAPSIZE,ay+GMP_MAPSIZE,az))
                {
                    //DLog("NETMSG_MODIFY_MAP: (onscreen change)");
                    pMapBuffer->SetProperty(CamCoord(ax),CamCoord(ay),cx,TOBOOL(dx));
                    ProcessConsoleCommand("savemap",0); // save the map
                }
                else
                {
                    //DLog("NETMSG_MODIFY_MAP: (offscreen change)");
                    if(!pOffScreenMap) { //Log("NETMSG_MODIFY_MAP: Can't allocate offscreen map!"); break; }
                    pOffScreenMap->Load3D(va("map%c%d-%d-%d.fmp",_PS,MapCoord(ax),MapCoord(ay),MapCoord(az)));
                    pOffScreenMap->SetProperty(CamCoord(ax),CamCoord(ay),cx,TOBOOL(dx));
                    pOffScreenMap->Save3D(va("map%c%d-%d-%d.fmp",_PS,MapCoord(ax),MapCoord(ay),MapCoord(az)));
                }
                */
                break;

            /*********************************************************************************
             ** NETMSG_SET_TILE                                                             **
             *********************************************************************************/

            case NETMSG_SET_TILE:

/*
                if(!pMapBuffer) { //Log("NETMSG_SET_TILE: Map not setup yet, not changing tile..."); break; }

                ax = RecvData->iRead(); // x
                ay = RecvData->iRead(); // y
                az = RecvData->iRead(); // z
                cx = RecvData->cRead(); // bank
                cy = RecvData->cRead(); // tile
                //DLog("NETMSG_SET_TILE: [%d][%d][%d][%d][%d]",ax,ay,az,cx,cy);
                if(camera->in(ax-GMP_MAPSIZE,ay-GMP_MAPSIZE,az,ax+GMP_MAPSIZE,ay+GMP_MAPSIZE,az))
                {
                    //DLog("NETMSG_SET_TILE: (onscreen change)");
                    pMapBuffer->SetTile(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cx); // change tile in map buffer
                    ProcessConsoleCommand("savemap",0); // save the map
                }
                else
                {
                    //DLog("NETMSG_SET_TILE: (offscreen change)");
                    if(!pOffScreenMap) { //Log("NETMSG_SET_TILE: Can't allocate offscreen map!"); break; }
                    sprintf(temp1,"map%c%d-%d-%d.fmp",_PS,MapCoord(ax),MapCoord(ay),MapCoord(ay));// load map into offscreen buffer
                    if(!pOffScreenMap->Load3D(temp1)) { pOffScreenMap->Clear(); } // change the tile
                    pOffScreenMap->SetTile(CamCoord(ax),CamCoord(ay),cx); // save the map
                    pOffScreenMap->Save3D(temp1);
                }
                */
                break;

            /*********************************************************************************
             ** NETMSG_SET_OBJECT                                                           **
             *********************************************************************************/

            case NETMSG_SET_OBJECT:
                // obsolete
                break;

            /*********************************************************************************
             ** NETMSG_SET_VERTEX                                                           **
             *********************************************************************************/

            case NETMSG_SET_VERTEX:

                /*
                if(pMapBuffer==NULL)
                {
                    //DLog("FCOM: Map not setup yet, not changing vertex...");
                    break;
                }

                ax = RecvData->iRead(); // x
                ay = RecvData->iRead(); // y
                az = RecvData->iRead(); // z
                cx = RecvData->cRead(); // height width or lighting?
                cy = RecvData->cRead(); // which vertex
                vx = RecvData->cRead(); // height / width / red element
                vy = RecvData->cRead(); // green element
                vz = RecvData->cRead(); // blue element

                dx=0;
                // if on the same screen
                if( ( ( pPlayer->GetX() ) >= ( ax - GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetX() ) <= ( ax + GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetY() ) >= ( ay - GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetY() ) <= ( ay + GMP_MAPSIZE ) ) &&
                    (   pPlayer->GetZ()   ==   az ) )
                {
                    dx=1;
                }
                // load map into offscreen buffer
                sprintf(temp1,"%s%cmap%c%d-%d-%d.fmp",pClientData->GRDir,_PS,_PS,MapCoord(ax),MapCoord(ay),MapCoord(ay));
                if(!pOffScreenMap->Load3D(temp1))
                {
                    pOffScreenMap->Clear();
                }
                switch(cx)
                {
                    case 0: // height
                        if(dx)
                            pMapBuffer->SetVertexHeight(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx);
                        else
                            pOffScreenMap->SetVertexHeight(CamCoord(ax),CamCoord(ay),cy,vx);
                        //DLog("Vertex %d height set to %d",cy,vx);//pMapBuffer->GetVertexHeight(ax+GMP_MAPSIZE*ex,ay+GMP_MAPSIZE*ey,cy));
                        break;

                    case 1: // lighting
                        if(dx)
                            pMapBuffer->SetVertexColor(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx,vy,vz);
                        else
                            pOffScreenMap->SetVertexColor(CamCoord(ax),CamCoord(ay),cy,vx,vy,vz);
                        //DLog("Vertex %d lighting set to R%d G%d B%d",cy,vx,vy,vz);
                        break;

                    case 2: // width
                        if(dx)
                            pMapBuffer->SetVertexWidth(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx);
                        else
                            pOffScreenMap->SetVertexWidth(CamCoord(ax),CamCoord(ay),cy,vx);
                        //DLog("Vertex %d width set to %d",cy,vx);
                        break;

                    case 3: // relight map
                        if(dx)
                            pMapBuffer->ClearVertexColors(vx,vy,vz);
                        else
                            pOffScreenMap->ClearVertexColors(vx,vy,vz);
                        //DLog("Map relit to %d %d %d",vx,vy,vz);
                        break;

                    default:
                        break;
                }
                // save the map
                if(dx)
                    ProcessConsoleCommand("savemap",0);
                else
                    pOffScreenMap->Save3D(temp1); */
                break;

            /*********************************************************************************
             ** NETMSG_LOCALCHAT                                                            **
             *********************************************************************************/

            case NETMSG_LOCALCHAT:  // Local Chat

				strcpy(temp1,RecvData->pRead()); // channel
				strcpy(temp2,RecvData->pRead()); // player
				strcpy(temp3,RecvData->pRead()); // time
				strcpy(temp4,RecvData->pRead()); // message

				// Add to chat channel

                break;

            /*********************************************************************************
             ** NETMSG_SYSTEMMSG                                                            **
             *********************************************************************************/

            case NETMSG_SYSTEMMSG:  // Global Chat / System Message
                strcpy(temp1,RecvData->pRead());
                // ChatBufferAdd(255,255,255,"font",temp1);
                //DLog("NETMSG_SYSTEMMSG [%s]",temp1);
                break;

            /*********************************************************************************
             ** NETMSG_MODIFY_CHARACTER                                                     **
             *********************************************************************************/

            case NETMSG_MODIFY_CHARACTER:
                    // obsolete
                break;

            /*********************************************************************************
             ** NETMSG_MOVEPLAYER                                                           **
             *********************************************************************************/

            case NETMSG_MOVEPLAYER:
                    // obsolete
                break;

            /*********************************************************************************
             ** NETMSG_VIS_MOVE                                                             **
             *********************************************************************************

            case NETMSG_VIS_REMOVE:
                i =RecvData->iRead(); // id
                ax=RecvData->iRead(); // number of path waypoints... or 0









                //DLog("NETMSG_VIS_REMOVE sid[%d]",i);
                break;


            /*********************************************************************************
             ** NETMSG_VIS_REMOVE                                                           **
             *********************************************************************************

            case NETMSG_VIS_REMOVE:
                i =RecvData->iRead(); // id
                Pop_Vis(i);
                //DLog("NETMSG_VIS_REMOVE sid[%d]",i);
                break;

            /*********************************************************************************
             ** NETMSG_VIS_UPDATE                                                           **
             *********************************************************************************

            case NETMSG_VIS_UPDATE:
                i =RecvData->iRead(); // id
                ax=RecvData->iRead(); // x
                ay=RecvData->iRead(); // y
                az=RecvData->iRead(); // z
                strcpy(temp1,RecvData->pRead()); // media
                ex=RecvData->iRead(); // media_type
                dx=RecvData->iRead(); // heading
                dy=RecvData->iRead(); // direction
                Pop_Vis(i);
                PushVis(i,ax,ay,az,temp1,ex,dx,dy);
                //DLog("NETMSG_VIS_UPDATE \
					   sid[%d] \
					   x[%d] \
					   y[%d] \
					   z[%d] \
					   media[%s] \
					   m_type[%d] \
					   head[%d] \
					   dir[%d]",i,ax,ay,az,temp1,ex,dx,dy );
                break;

            /*********************************************************************************
             ** NETMSG_SERVER_INFO                                                          **
             *********************************************************************************/

            case NETMSG_SERVER_INFO:
                switch(RecvData->cRead()) // what is updating?
                {
                    case SI_GENERIC:	// general server info -> fill in some kind
										// of server info data struct

                        ay = RecvData->cRead();
                        switch(ay)
                        {
                            case 1: // this tells the client that it is done with updates
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;

            /*********************************************************************************
             ** NETMSG_GUI                                                                  **
             *********************************************************************************/

            case NETMSG_GUI:
                dx=RecvData->cRead();
                switch(dx)
                {
                    case FM_GUI_PROGRAM_FINISH: // end of gui
                        //GAME_MODE=NEXT_GAME_MODE;
                        break;

                    case FM_GUI_STUMP_CREATE: // add gui stump (name)
                        strcpy(temp1,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_STUMP_CREATE] [%s]",temp1);
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        cx=RecvData->iRead();
                        strcpy(temp2,RecvData->pRead());
                        //DLog("add stump->[%s,%d,%d,%d,%d,%d,%s]",temp1,ax,ay,bx,by,cx,temp2);
                        //pGUI->add_stump(temp1,ax,ay,bx,by,cx,temp2);
                        break;

                    case FM_GUI_STUMP_UPDATE:
                        strcpy(temp1,RecvData->pRead());
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        cx=RecvData->iRead();
                        strcpy(temp2,RecvData->pRead());
                        //pGUI->mod_stump(temp1,ax,ay,by,by,cx,temp2);
                        //DLog("NETMSG_GUI[FM_GUI_STUMP_UPDATE] [%s]",temp1);
                        break;

                    case FM_GUI_STUMP_REMOVE: // remove gui stump
                        strcpy(temp1,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_STUMP_REMOVE] [%s]",temp1);
                        //if(pGUI->focus_stump==pGUI->get_stump(temp1)) pGUI->focus_stump=0;
                        //pGUI->del_stump(temp1);

                        break;

                    case FM_GUI_STUMP_CAPTION:
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_STUMP_CAPTION] [%s][%s]",temp1,temp2);
                        //if(pGUI)
                            //i/f(pGUI->get_stump(temp1))
                               // pGUI->get_stump(temp1)->set_caption(temp2);
                        break;

                    case FM_GUI_CONTROL_CREATE: // add gui control to a stump
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_CONTROL_CREATE] [%s][%s]",temp1,temp2);
                        cx=RecvData->iRead();
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        dx=RecvData->iRead();
                        strcpy(temp3,RecvData->pRead());
                        strcpy(temp4,RecvData->pRead());
                        //pGUI->get_stump(temp1)->add_control(temp2,cx,ax,ay,bx,by,dx,temp3,temp4);
                        //DLog("Control create!");
                        break;

                    case FM_GUI_CONTROL_REMOVE: // remove gui control from a stump
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        strcpy(temp3,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_CONTROL_REMOVE] [%s][%s][%s]",temp1,temp2,temp3);
                        break;

                    case FM_GUI_CONTROL_VALUE:
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        strcpy(temp3,RecvData->pRead());
                        //DLog("NETMSG_GUI[FM_GUI_CONTROL_VALUE]  [%s][%s][%s]",temp1,temp2,temp3);
                        //if(pGUI)
                            //if(pGUI->get_stump(temp1))
                                //if(pGUI->get_stump(temp1)->get_control(temp2))
                                   // strcpy(pGUI->get_stump(temp1)->get_control(temp2)->value,temp3);
                        break;

                    case FM_GUI_CLEAR_ALL:
                        //DLog("NETMSG_GUI[FM_GUI_CLEAR_ALL]");
                        //DEL(pGUI);
                        //pGUI=new C_FM_GUI();
                        break;

                    default:
                        break;
                }
                break;

            /*********************************************************************************
             ** NETMSG_FVM                                                                  **
             *********************************************************************************/

            case NETMSG_FVM:
                switch(RecvData->cRead())
                {
                    case FVM_GETTARGET:
                        //DLog("NETMSG_FVM(FVM_GETTARGET):%s",RecvData->pRead());
                        //pClientData->bTargeting=1;
                        break;

                    case FVM_SET_DAYLIGHT:

                        //camera->r=RecvData->fRead();
                        //camera->g=RecvData->fRead();
                        //camera->b=RecvData->fRead();

                        ////DLog("NETMSG_FVM(FVM_DAYLIGHT): Daylight changed to... r[%f] g[%f] b[%f]",camera->r,camera->g,camera->b);

                        break;

                    case FVM_MOVECAM:
						/* camera->x =RecvData->iRead();
                        camera->y =RecvData->iRead();
                        camera->z =RecvData->iRead();
                        camera->ax=RecvData->iRead();
                        camera->ay=RecvData->iRead();
                        camera->az=RecvData->iRead();
                        camera->ox=RecvData->iRead();
                        camera->oy=RecvData->iRead();
                        camera->oz=RecvData->iRead();
                        LoadMap(camera->x,camera->y,camera->z);
                        //DLog("NETMSG_FVM(FVM_MOVECAM): Camera position  x[%d]  y[%d]  z[%d]",camera->x,camera->y,camera->z);
                        //DLog("                       : Camera angle    ax[%d] ay[%d] az[%d]",camera->ax,camera->ay,camera->az);
                        //DLog("                       : Camera offset   ox[%d] oy[%d] oz[%d]",camera->ox,camera->oy,camera->oz);
                        //DLog("                       : Camera zoom     zoom[%d]",camera->zoom);
						*/
                        break;

                    case FVM_SET_MAP_DRAW:
                        //pClientData->bDrawMap=TOBOOL(RecvData->cRead());
                        ////DLog("NETMSG_FVM(FVM_SET_MAP_DRAW): [%d]",pClientData->bDrawMap);
                        break;

                    case FVM_SET_GAME_MODE: // change this to FVM_SET_PLAY_MODE
                        //pClientData->Mode=RecvData->cRead();
                        ////DLog("NETMSG_FVM(FVM_SET_GAME_MODE): [%d]",pClientData->Mode);
                        break;

                    case FVM_SET_LIMBO_STATE: // put a way to change GAME_MODE

                        //switch(RecvData->iRead())
                       // {
                        //    case 0:
                        //        GAME_MODE=GAME_ON;
                        //        break;
                       //     case 1:
                        //        GAME_MODE=GAME_LIMBO;
                       //         break;
                        //    default:
                       //         GAME_MODE=NEXT_GAME_MODE;
                       //         break;
                       // }
                        ////DLog("NETMSG_FVM(FVM_SET_LIMBO_STATE): [%d]",TOBOOL(GAME_MODE==GAME_LIMBO));
                        break;

                    default:
                        break;
                }
                break;


            /*********************************************************************************
             ** NETMSG_FILE_XFER                                                            **
             *********************************************************************************/

            case NETMSG_FILE_XFER:

//				char *pFileBuf; //char pFileBuf[NET_FILE_XFER_BLOCK_SIZE+1]; memset(pFileBuf,0,1024);
//				FILE *pFile;
//				bool bFileFail;

				switch(RecvData->cRead())
				{

				/*	// NET_FILE_RES_MEDIA
					// NET_FILE_RES_SCRIPT

					case NET_FILE_NOP:
						break;

					case NET_FILE_START:
						// filename
						// filesize
						// start file, put a temporary sequencer in temp folder for the file
						// ie; hey.bmp will create temp/hey.bmp.sequence
						strcpy(szTemp1,RecvData->pRead());
						cx=RecvData->iRead();
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
								SendData.Write((char)NETMSG_FILE_XFER);
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
							SendData.Write((char)NETMSG_FILE_XFER);
							SendData.Write((char *)szTemp1);
							SendData.Write((char)NET_FILE_ERROR);
							SendNetMessage(0);
						}

						break;

					case NET_FILE_DATA:

						// sequence number
						// filename
						// data block (1024)

						ax=RecvData->iRead(); // sequence number
						strcpy(szTemp1,RecvData->pRead()); // filename
						pFileBuf=RecvData->pRead(NET_FILE_XFER_BLOCK_SIZE);
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
								SendData.Write((char)NETMSG_FILE_XFER);
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
							SendData.Write((char)NETMSG_FILE_XFER);
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

						ax=RecvData->iRead(); // sequence number
						strcpy(szTemp1,RecvData->pRead()); // filename
						dx=RecvData->iRead();
						pFileBuf=RecvData->pRead(dx);
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
							SendData.Write((char)NETMSG_FILE_XFER);
							SendData.Write((char)NET_FILE_EOF_RESEND);
							SendData.Write((char *)szTemp1);
						}
						break;
						*/

					case NET_FILE_RESUME:
						break;

					//Log("File Xfer: %s",RecvData->pRead());

				}

                break;

            /*********************************************************************************
             ** NETMSG_PING                                                                 **
             *********************************************************************************/

            case NETMSG_PING:
                //pClientData->dwPing=dlcs_get_tickcount()-RecvData->dwRead();
                break;

            default:
                // Do nothing... (should never execute this code, but who knows...)
                //DLog("Recieved a NETMSG (%d) but could not recognize it.",pFMGS->cLastNetMessage);
                // ChatBufferAdd(255,0,0,"SYSTEM","Recieved a NETMSG (%d) but could not recognize it.",pFMGS->cLastNetMessage);
                break;
        }
    }
}

/**************************************************
 ** Find Target                                  **
 **************************************************

extern "C" void FindTarget(void)
{
    pClientData->bTargeting=0;
    SendData.Reset();
    SendData.Write((char)NETMSG_FVM);
    SendData.Write((char)FVM_GETTARGET);
    SendData.Write((char *)"TargetID");
    SendData.Write((char)0);
    SendNetMessage(0);
}

/**************************************************
 ** Connect to a Server                          **
 **************************************************/

extern "C" int NetworkConnect()
{
    int iResult;
    NetworkSetup();
    /*
    //DLog("c_netutil.cpp -> NetworkConnect() Connecting to server[%s:%s](%s,%s)",pClientData->IPAddress,pClientData->Port,pClientData->Name,pClientData->Password);
    iResult=
		pFMGS->Connect(	pClientData->IPAddress,
						pClientData->Port,
						pClientData->Name,
						pClientData->Password);
    switch(iResult)
    {
        case 1:
            //DLog("NET: Connected.");
            return true;
        default:
            NetworkSetup();

            // INPUTMODE=NORMAL;

            GAME_MODE=LOGIN_SCREEN_ENTRY;
            return false;
    }*/

    // return false;
    return true;
}
/**************************************************
 ** Initialize the Network                       **
 **************************************************/
extern "C" int NetworkSetup(void)
{
    DEL(pFMGS);
    pFMGS=new C_FMGS();
    //pClientData->dwPing=999;
    return true;
}
/**************************************************
 ** Ping the server                              **
 **************************************************/
extern "C" long Ping(void)
{
    static long dwPingTimer=dlcs_get_tickcount();
    if(!pFMGS)                              return 9999;
    if(pFMGS->eGetState()==CON_NOTCONNECT)  return 999;
    if(dlcs_get_tickcount()-dwPingTimer>5000)
    {
        dwPingTimer=dlcs_get_tickcount();
        /*
        SendData.Reset();
        SendData.Write((char)NETMSG_PING);
        SendData.Write((long)dlcs_get_tickcount());
        SendNetMessage(0);
        */
    }
    return dwPingTimer;
}
/**************************************************
 ** Get a file from server                       **
 **************************************************
extern "C" void GetFile(char *filename)
{
	if(!pFMGS) return;
	SendData.Reset();
	SendData.Write((char)NETMSG_FILE_XFER);
	SendData.Write((char)NET_FILE_START);
	SendData.Write((char *)filename);
	SendNetMessage(0);
}
*/
