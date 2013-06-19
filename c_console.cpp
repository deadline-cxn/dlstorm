/***********************************************/

#include "c_console.h"

/***********************************************/

C_EM_CONSOLE::C_EM_CONSOLE()
{
    Init();

}

/***********************************************/

C_EM_CONSOLE::C_EM_CONSOLE(CLog *pInLog)
{
    pLog=pInLog;
    Init();
}


/***********************************************/

C_EM_CONSOLE::C_EM_CONSOLE(CGAF *pInGAF, CLog *pInLog)
{
    pGAF=pInGAF;
    pLog=pInLog;
    Init();
}

/***********************************************/

C_EM_CONSOLE::~C_EM_CONSOLE()
{

}

/***********************************************/

void C_EM_CONSOLE::ConsoleScrollInput()
{
/*
    int i;
    for(i=MAX_CONSOLE_INPUT_BUFFER-1;i>0;i--)
    {
        strcpy( ConsoleCommandInputBuffer[i].Text,
                ConsoleCommandInputBuffer[i-1].Text);
    }
    strcpy(ConsoleCommandInputBuffer[0].Text,szCommand);
    iConsoleCommandInputBufferOffset=0;
*/
}

/***********************************************/

void C_EM_CONSOLE::ClearConsole(void)
{
    /*
    int i;
    for(i=0;i<MAX_CONSOLE_BUFFER;i++)
    {
    	memset(ConsoleBuffer[i].Text,0,256);
        ConsoleBuffer[i].Font=0;
    }

    for(i=0;i<MAX_CONSOLE_MULTI_COMMANDS;i++)
    {
        memset(ConsoleCommandBuffer[i].Text,0,256);
        ConsoleCommandBuffer[i].Font=0;
    }

    for(i=0;i<MAX_CONSOLE_INPUT_BUFFER;i++)
    {
        memset(ConsoleCommandInputBuffer[i].Text,0,256);
        ConsoleCommandInputBuffer[i].Font=0;
    }*/

}


/***********************************************/

bool C_EM_CONSOLE::Init(void)
{

    //pLog->_Add("C_EM_CONSOLE::Init() start");

    buf.clear();

    // DLog("Begin Console init...");

    glConsoleTexture=0;
    glConsoleTexture=new CGLTexture();

    /*

    ConsoleBuffer = NULL;
	ConsoleBuffer = new CFM_String[MAX_CONSOLE_BUFFER+1];
	ConsoleCommandBuffer = NULL;
	ConsoleCommandBuffer = new CFM_String[MAX_CONSOLE_MULTI_COMMANDS+1];
	ConsoleCommandInputBuffer = NULL;
	ConsoleCommandInputBuffer = new CFM_String[MAX_CONSOLE_INPUT_BUFFER+1];

	strcpy(ConsoleInput,"");
    ConsoleYScroll=0;
	ConsoleBufferOffset=0;
    iConsoleCommandInputBufferOffset=0;
    memset(ConsoleInput,0,sizeof(ConsoleInput));

    */

    _GlobalFunctions();
    _GlobalStrings();
    _GlobalIntegers();
    _GlobalVars();


    cvar_type_map.clear();
    cvar_type_map["CVAR_NULL"]  = CVAR_NULL;
    cvar_type_map["CVAR_BOOL"]  = CVAR_BOOL;
    cvar_type_map["CVAR_STRING"]= CVAR_STRING;
    cvar_type_map["CVAR_CHAR"]  = CVAR_CHAR;
    cvar_type_map["CVAR_UCHAR"] = CVAR_UCHAR;
    cvar_type_map["CVAR_INT"]   = CVAR_INT;
    cvar_type_map["CVAR_UINT"]  = CVAR_UINT;
    cvar_type_map["CVAR_LONG"]  = CVAR_LONG;
    cvar_type_map["CVAR_ULONG"] = CVAR_ULONG;
    cvar_type_map["CVAR_FLOAT"] = CVAR_FLOAT;

    cvar_type_format_map.clear();
    cvar_type_format_map[CVAR_NULL]   = "-null-";
    cvar_type_format_map[CVAR_BOOL]   = "%d";
    cvar_type_format_map[CVAR_STRING] = "%s";
    cvar_type_format_map[CVAR_CHAR]   = "%d";
    cvar_type_format_map[CVAR_UCHAR]  = "%d";
    cvar_type_format_map[CVAR_INT]    = "%d";
    cvar_type_format_map[CVAR_UINT]   = "%d";
    cvar_type_format_map[CVAR_LONG]   = "%d";
    cvar_type_format_map[CVAR_ULONG]  = "%d";
    cvar_type_format_map[CVAR_FLOAT]  = "%0.3f";

    if(!glConsoleTexture)
    {
        // DLog("Console initialization failure...");
        return false;
    }
   // Log("Console BMP initialized...");
    return 1;
}

char * C_EM_CONSOLE::get_cvarformat(int t)
{
    return (char *)cvar_type_format_map[t].c_str();
}
const char * C_EM_CONSOLE::get_cvarformatted(const char *f,void *cv)
{
    return va(f,cv);
}

const char * C_EM_CONSOLE::get_cvartype_string(int t)
{
    map <string, int>::iterator ii;
    for( ii=cvar_type_map.begin(); ii!=cvar_type_map.end(); ++ii)
    {
        if( ( (*ii).second ) == (t) )
        {
            return (*ii).first.c_str();
        }
    }
    return "null";
}

int C_EM_CONSOLE::get_cvartype(const char * s)
{
    vector <string> vt;
    vt=explode("_",s);
    if(vt.size()>1)
    {
        if(dscc(vt[0].c_str(),"b"))  return CVAR_BOOL;
        if(dscc(vt[0].c_str(),"s"))  return CVAR_STRING;
        if(dscc(vt[0].c_str(),"c"))  return CVAR_CHAR;
        if(dscc(vt[0].c_str(),"uc")) return CVAR_UCHAR;
        if(dscc(vt[0].c_str(),"i"))  return CVAR_INT;
        if(dscc(vt[0].c_str(),"ui")) return CVAR_UINT;
        if(dscc(vt[0].c_str(),"l"))  return CVAR_LONG;
        if(dscc(vt[0].c_str(),"ul")) return CVAR_ULONG;
        if(dscc(vt[0].c_str(),"f"))  return CVAR_FLOAT;
    }
    return CVAR_NULL;
}
/***********************************************/

bool C_EM_CONSOLE::LoadConsoleTexture(CGAF *pGAF,char *fname)
{
	//DLog("Begin Console BMP load...");
	DestroyConsoleTexture();
    if(!Init())
		return false;

    glConsoleTexture->Transparent(1);
	glConsoleTexture->Load(pGAF,fname,1);

    if(glConsoleTexture->Loaded())
    {

		// Log("Console [%s] loaded...",fname);
    }

    else
	{
		// Log("Console [%s] load failure...",fname);
		return false;
	}

    strcpy(glConsoleTexture->name,fname);
    return TOBOOL(glConsoleTexture->bmap);
}

/***********************************************/

bool C_EM_CONSOLE::DestroyConsoleTexture(void)
{
	// DLog("Begin Console destroy...");
	DEL(glConsoleTexture);
	// DLog("Console destroyed...");
	return 1;
}

/***********************************************/

void C_EM_CONSOLE::Entry(char *fmt, ...)
{
    ////pLog->_Add("C_EM_CONSOLE::Entry() start");
    char ach[512];
    memset(ach,0,512);

    va_list vaz;
    va_start(vaz,fmt);
    vsprintf(ach,fmt,vaz);
    va_end(vaz);

    buf.push_back(ach);

    ////pLog->_Add( "TEST [%d]", buf.size() );

    /*
    if(!ConsoleBuffer) return;

    int k;



    int i;
    int j;
    int tabber;

    char ach2[512];

    memset(ach2,0,512);
    if(pClientData==NULL)
        return;
    if(ConsoleBuffer==NULL)
        return;



    j=0;
    tabber=8;
    for(i=0;i<strlen(ach);i++)
    {
        switch(ach[i])
        {
            case '\n':
            case '\r':
                if(i+1<strlen(ach))
                {
                    for(k=0;k<(MAX_CONSOLE_BUFFER-1);k++)
                        strcpy(ConsoleBuffer[k].Text,ConsoleBuffer[k+1].Text);
                    strcpy(ConsoleBuffer[MAX_CONSOLE_BUFFER-1].Text,ach2);
                    j=0;
                    memset(ach2,0,512);
                    tabber=8;
                }
                break;

            case '\t':
                while(tabber)
                {
                    ach2[j]=' ';
                    tabber--;
                    j++;
                }
                break;
            default:
                ach2[j]=ach[i];
                tabber--;
                j++;
                break;
        }
        if(j>90)
        {
            for(k=0;k<(MAX_CONSOLE_BUFFER-1);k++)
                strcpy(ConsoleBuffer[k].Text,ConsoleBuffer[k+1].Text);
            strcpy(ConsoleBuffer[MAX_CONSOLE_BUFFER-1].Text,ach2);
            j=0;


            memset(ach2,0,512);
            tabber=8;
        }
        if(tabber<0)
            tabber=8;
    }

    for(k=0;k<(MAX_CONSOLE_BUFFER-1);k++)
        strcpy(ConsoleBuffer[k].Text,ConsoleBuffer[k+1].Text);
    strcpy(ConsoleBuffer[MAX_CONSOLE_BUFFER-1].Text,ach2);
    (/*/
}

/***********************************************/

void C_EM_CONSOLE::ToggleConsole()
{

    /*
    if(INPUTMODE==NORMAL)
    {
        INPUTMODE=CONSOLE1;
        return;
    }

    if(INPUTMODE==CONSOLE1)
    {
        INPUTMODE=CONSOLE2;
        //if(GAME_MODE==GAME_ON)
         //   INPUTMODE=NORMAL;
        return;
    }

    if(INPUTMODE==CONSOLE2)
    {
        INPUTMODE=NORMAL;
        return;
    }

    INPUTMODE=CONSOLE1;
    return;
    */
}

/***********************************************/

void C_EM_CONSOLE::draw_console()
{
    int i;
    int z;
    char szTemp1[1024]; memset(szTemp1,0,1024);

    ////////////////////////////////////////////////////////////////////////////////
    // Scroll System Messages

    static unsigned long scrollme  = dlcs_get_tickcount();
    static char scrolledlines=0;


   // if(dlcs_get_tickcount()-scrollme>300)
   // {
       // scrollme = dlcs_get_tickcount();

       // scrolledlines--;

        //if(scrolledlines<-10)
        //{
           // for(i=0; i<MAX_CHAT_BUFFER-1; i++)
           // {
                //strcpy(ChatBuffer[i].Text,ChatBuffer[i+1].Text);
                //ChatBuffer[i].Font = ChatBuffer[i+1].Font;
           // }
            // memset(ChatBuffer[MAX_CHAT_BUFFER-1].Text,0,256);
            //scrolledlines=0;
        //}
    //}


   // if(pClientData)
  //  {
        //for(i=0; i<MAX_CHAT_BUFFER; i++)
        //    WriteText(0,i*11+scrolledlines,ChatBuffer[i].Text,0);

        //if(bShowFPS) // Draw FPS (if it is toggled on)
            // DrawFPS(pClientData->ScreenWidth-100,0);

        //if( (bShowPING) &&
               // (pFMGS->eGetState()!=CON_NOTCONNECT) ) // Draw PING info (if it is toggled on)

            // WriteText(pClientData->ScreenWidth-100,12,va("^5PING^c[^+^5%d^c^+]",Ping()),0);

//        if(Mode==BUILD) // BUILD MODE INDICATOR
 //           WriteText(pClientData->ScreenWidth-150,24,"^+^1[^+^4BUILD MODE^1^+]",0); // Indicate player is in build mode

        /////////////////////////////////////////////////////////////////////////////////
        // Draw DEBUG info (if it is toggled on)
/*
        if(INPUTMODE != CONSOLE2)
        {
            if(pClientData->cDebug)
            {
                DrawBar(
                    0,							pClientData->ScreenHeight-112 ,
                    pClientData->ScreenWidth,	pClientData->ScreenHeight-110 ,LONGRGB(65,0,0),LONGRGB(180,0,0));

                DrawTransparentBar(
                    0,							pClientData->ScreenHeight-110 ,
                    pClientData->ScreenWidth,	pClientData->ScreenHeight     ,LONGRGB(55,55,0),LONGRGB(180,180,0));

                DrawBar(0,							pClientData->ScreenHeight-110 ,
                        pClientData->ScreenWidth,	pClientData->ScreenHeight-95  ,LONGRGB(180,0,0),LONGRGB(65,0,0));

                DrawBar(0,							pClientData->ScreenHeight-95  ,
                        pClientData->ScreenWidth,	pClientData->ScreenHeight-93  ,LONGRGB(65,0,0),LONGRGB(180,0,0));



                //pGUI->
                WriteText(0,pClientData->ScreenHeight-110,"^1^+Debug Window",0);
                //if(pFMGS)
                  //  pGUI->WriteText(8,pClientData->ScreenHeight-92,va("^4NET^1: ^4LNM^1=^4%d NWS^1=^4%d",cLastNetMessage,pFMGS->eGetState()),0);

                //pGUI->
                WriteText(8,pClientData->ScreenHeight-81,va("^4MOUSE^1: ^4MX ^1=^4%3d MY ^1=^4%3d MLB^1=^4%1d MMB^1=^4%1d MRB^1=^4%1d MM ^1=^4%2d",
                          GetMouseX(),
                          GetMouseY(),
                          GetMouseDown(SDL_BUTTON_LEFT),
                          GetMouseDown(SDL_BUTTON_MIDDLE),
                          GetMouseDown(SDL_BUTTON_RIGHT),
                          MOUSEMODE),0);

                if(camera)
                {
                    pGUI->WriteText(0,460, va("CX  [%04.2f] CY  [%04.2f] CZ  [%04.2f]",camera->x,camera->y,camera->z)		,0);
                    pGUI->WriteText(0,480, va("CPX [%04.2f] CPY [%04.2f] CPZ [%04.2f]",camera->px,camera->py,camera->pz)	,0);
                    pGUI->WriteText(0,500, va("CVX [%04.2f] CVY [%04.2f] CVZ [%04.2f]",camera->vx,camera->vy,camera->vz)	,0);
                    pGUI->WriteText(0,520, va("CUX [%04.2f] CUY [%04.2f] CUZ [%04.2f]",camera->ux,camera->uy,camera->uz)	,0);
                    pGUI->WriteText(0,540, va("CRX [%04.2f] CRY [%04.2f] CRZ [%04.2f]",camera->rx,camera->ry,camera->rz)	,0);
                    pGUI->WriteText(0,560, va("CTX [%04.2f] CTY [%04.2f] CTZ [%04.2f]",camera->tx,camera->ty,camera->tz)	,0);
                }


           // }
        //}

        ////////////////////////////////////////////////////////////////////////////////
        // Do stuff for current input mode here



        switch(INPUTMODE)
        {
        case CONSOLE1:

            if(ConsoleYScroll != 287)
                ConsoleYScroll = 287;

            DrawConsoleBMP(0,0,pClientData->ScreenWidth,ConsoleYScroll+3);
            DrawBar(0,ConsoleYScroll+3,pClientData->ScreenWidth,ConsoleYScroll+5,LONGRGB(42,42,62),LONGRGB(130,130,180));

            for(i=MAX_CONSOLE_BUFFER-23-ConsoleBufferOffset; i<MAX_CONSOLE_BUFFER-ConsoleBufferOffset; i++)

                WriteText(  11,
                            (i-MAX_CONSOLE_BUFFER-1+ConsoleBufferOffset)*12 + (ConsoleYScroll),
                            ConsoleBuffer[i].Text,
                            5);

            WriteText(1,(ConsoleYScroll)-12,">",5);

            sprintf(szTemp1,"%s",ConsoleInput);
            if(
               FlashCursor(1,1)) strcat(szTemp1,"_");
            WriteText(11,(ConsoleYScroll)-12,szTemp1,5);

            DrawGUIResource(BC_BLANK_BUTTON,1,1,11,11);
            DrawGUIResourceC(BC_ARROW_UP,2,2,10,10,0,0,0);

            if( (GetMouseX()<11) &&
                    (GetMouseY()<11) )

            {
                if(GetMouseDown(SDL_BUTTON_LEFT))
                {
                    DrawGUIResourceC(BC_ARROW_UP,2,2,10,10,255,255,255);
                    ConsoleBufferOffset++;
                    if(ConsoleBufferOffset>MAX_CONSOLE_BUFFER-23)
                        ConsoleBufferOffset=MAX_CONSOLE_BUFFER-23;
                }
            }

            DrawGUIResource(BC_BLANK_BUTTON,1,ConsoleYScroll-16,10,ConsoleYScroll-6);
            DrawGUIResourceC(BC_ARROW_DOWN,2,ConsoleYScroll-15,9,ConsoleYScroll-7,0,0,0);

            if( (GetMouseX()<11) &&
                    (GetMouseY()>ConsoleYScroll-16)  &&
                    (GetMouseY()<ConsoleYScroll))
            {
                if(GetMouseDown(SDL_BUTTON_LEFT))
                {
                    DrawGUIResourceC(BC_ARROW_DOWN,2,ConsoleYScroll-14,9,ConsoleYScroll-7,255,255,255);
                    // DrawGUIResource(BC_ARROW_DOWN,2,ConsoleYScroll-14,8,ConsoleYScroll-8);
                    ConsoleBufferOffset--;
                    if(ConsoleBufferOffset<0)
                        ConsoleBufferOffset=0;
                }
            }

            DrawGUIResource(BC_SCROLLBAR_UD,0,11,10,ConsoleYScroll-15); // updown slide bar

            z= ConsoleYScroll-18-(2*((ConsoleBufferOffset)%(MAX_CONSOLE_BUFFER)/8)+10);

            //z=((MAX_CONSOLE_BUFFER) % (ConsoleBufferOffset)) %
            //  ;
            //  ((ConsoleYScroll-18)-12));
            // z=(((ConsoleYScroll-18)-12))-z;

            DrawGUIResourceC(BC_SCROLL_SLIDE,3,z,8,z+8,255,20,155);
            if(MouseIn(3,z,8,z+8))
            {
                if(GetMouseDown(SDL_BUTTON_LEFT))
                {
                    WriteText(1,z,"Slider not working yet...",5);
                }
            }

            break;

        default:
            ConsoleYScroll=0;
            break;
        }
        */
   // }
}


/****************************************************************************************************/

void C_EM_CONSOLE::DrawConsoleBMP(int iX1,int iY1,int iX2,int iY2)
{
	if(!glConsoleTexture)
	{
		glConsoleTexture=new CGLTexture();
		return;
	}

    if(!glConsoleTexture->Loaded())
	{
		glConsoleTexture->Transparent(1);
		glConsoleTexture->Load(pGAF,"base/console.bmp",1);
		return;
	}
    glConsoleTexture->Draw(iX1,iY1,iX2,iY2,255,255,255);
}







