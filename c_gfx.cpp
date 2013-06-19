/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_gfx.h"

#define X .525731112119133606
#define Z .850650808352039932


static GLfloat vdata[12][3] = {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}

void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb) {
    if (div<=0) {
        glNormal3fv(a); glColor3f(cr,cg,cb); glVertex3f(a[0]*r, a[1]*r, a[2]*r);
        glNormal3fv(b); glColor3f(cr,cg,cb); glVertex3f(b[0]*r, b[1]*r, b[2]*r);
        glNormal3fv(c); glColor3f(cr,cg,cb); glVertex3f(c[0]*r, c[1]*r, c[2]*r);
    } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0;i<3;i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab); normalize(ac); normalize(bc);
        drawtri(a, ab, ac, div-1, r,cr,cg,cb);
        drawtri(b, bc, ab, div-1, r,cr,cg,cb);
        drawtri(c, ac, bc, div-1, r,cr,cg,cb);
        drawtri(ab, bc, ac, div-1, r,cr,cg,cb);  //<--Comment this line and sphere looks really cool!
    }
}

void drawsphere(int ndiv, float radius, float cr,float cg,float cb) {
    glBegin(GL_TRIANGLES);
    for (int i=0;i<20;i++)
        drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius,cr,cg,cb);
    glEnd();
}

/****************************************************************************************************/

C_GFX::C_GFX()
{
    VideoFlags=0;
}

/****************************************************************************************************/

C_GFX::C_GFX(int w, int h, int c, bool FullScreen, const char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF)
{

    InitializeGFX(w,h,c,FullScreen,wincaption,pUSELOG,pUSEGAF);

}

/****************************************************************************************************/

C_GFX::~C_GFX()
{
    ShutDownGFX();
    SDL_Quit();
}

/****************************************************************************************************/

bool C_GFX::InitializeGFX(int w, int h, int c, bool FullScreen, const char *wincaption,CLog *pUSELOG,CGAF *pUSEGAF)
{
    pLog=pUSELOG;
    pGAF=pUSEGAF;

    pLog->_Add("Init SDL/OpenGL GFX Subsystem...");

#ifdef __linux__
	putenv("SDL_VIDEODRIVER=dga");
#endif

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    VideoFlags = SDL_OPENGL|SDL_HWPALETTE|SDL_DOUBLEBUF;

    if(FullScreen) VideoFlags |= SDL_FULLSCREEN;

    const
    SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware

    if(!VideoInfo)
    {
        pLog->_Add("Failed getting Video Info : %s",SDL_GetError());
        return false;
    }
    if(VideoInfo->hw_available)
    {
        VideoFlags |= SDL_HWSURFACE;
        pLog->_Add("Hardware surfaces...");
    }
    else
    {
        VideoFlags |= SDL_SWSURFACE;
        pLog->_Add("Software surfaces...");
    }

    if(VideoInfo->blit_hw)
    {
        VideoFlags |= SDL_HWACCEL;
        pLog->_Add("Hardware acceleration enabled!");
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,  16 );

    pLog->_Add("Video memory:[%d]",VideoInfo->video_mem);
/*
    pClientData->screen_res_640x480x16   = SDL_VideoModeOK(600,480,16, VideoFlags);
    pClientData->screen_res_800x600x16   = SDL_VideoModeOK(800,600,16, VideoFlags);
    pClientData->screen_res_1024x768x16  = SDL_VideoModeOK(1024,768,16, VideoFlags);
    pClientData->screen_res_1280x1024x16 = SDL_VideoModeOK(1280,1024,16, VideoFlags);
    pClientData->screen_res_640x480x32   = SDL_VideoModeOK(600,480,32, VideoFlags);
    pClientData->screen_res_800x600x32   = SDL_VideoModeOK(800,600,32, VideoFlags);
    pClientData->screen_res_1024x768x32  = SDL_VideoModeOK(1024,768,32, VideoFlags);
    pClientData->screen_res_1280x1024x32 = SDL_VideoModeOK(1280,1024,32, VideoFlags);
    */


    if(!SDL_VideoModeOK(w,h,c,VideoFlags))
    {
        pLog->_Add("SDL_VideoModeOK failure");
    }
    // if(!pClientData->ScreenColors)
  //  {
         //pClientData->ScreenColors=16;
    //     return false;
    //}

    pScreen = SDL_SetVideoMode(w,h,c,VideoFlags);
    if(!pScreen)
    {
        pLog->_Add("Can't set up pScreen! ErroR!");
        return false;
    }

    pLog->_Add("Attempting OpenGL initialization");

    if(!InitGL())       return false;

    //strcpy(WindowCaption,wincaption);
    //SDL_WM_SetCaption(WindowCaption,"icon");

    SDL_ShowCursor(SDL_DISABLE);


    return true;
}

/****************************************************************************************************/

void C_GFX::ShutDownGFX(void)
{
    pLog->_Add("Shutting down SDL/OpenGL GFX subsystem...");

    DestroyBaseGFX();
    //DestroyModels();

    glFinish();
    glFlush();

    SDL_FreeSurface(pScreen);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    pLog->_Add("SDL/OpenGL GFX subsystem shutdown...");
}

/****************************************************************************************************/

int C_GFX::InitGL() // All Setup For OpenGL Goes Here
{
	glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)SDL_GetVideoSurface()->w/(GLfloat)SDL_GetVideoSurface()->h,0.1f,200.0f);
	// gluPerspective(45.0f,1.333f,0.1f,20000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);									// Depth Buffer Setup

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

	glEnable(GL_TEXTURE_2D);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    BaseTexture=0;

    if(!InitBaseGFX())  return false;
    //if(!InitModels())   return false;

    pLog->_Add("OpenGL Initialized...");

    return true; // Initialization Went OK
}

/****************************************************************************************************/
void C_GFX::EndScene(void)
{
    FlipSurfaces();
}

void C_GFX::FlipSurfaces(void)
{
	glFlush();
	SDL_GL_SwapBuffers();
}

/****************************************************************************************************/

void C_GFX::BeginScene(void)
{
    GetFade(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
    static stra star[500];
    static bool bstars;
    static float ffy;
    static float fff;
    static float dir=-.405f;
    int y;
    glClearColor( (0.2f),(0.4f),(1.0f),0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // change to draw sky here
    DrawBaseGFX(0,0,SDL_GetVideoSurface()->w,SDL_GetVideoSurface()->h,10,155,155,155);

    if(GAME_MODE==GAME_ON) { return; }
    if(!Generic)
    {
        if(!bstars)
        {
            for(y=0;y<500;y++)
            {
                star[y].x=(rand()%900-100);
                star[y].y= rand()%900-150;
                star[y].speed=(rand()%24)*0.48f+1.3f;
                star[y].gfx=rand()%2+12;
            }
            bstars=1;
        }
        fff+=dir;
        if(fff>34)  dir =(-0.405f);
        if(fff<-34) dir =( 0.405f);
        for(y=0;y<500;y++)
        {
            if(GAME_MODE!=GAME_ON)
            {
                DrawBaseGFX( (int)(star[y].x),
                             (int)(600-star[y].y-30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                             (int)(star[y].x+2+star[y].speed ),
                             (int)(600-star[y].y+1+star[y].speed -30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                             star[y].gfx,
                             GetFade(3),
                             GetFade(2),
                             GetFade(1) );
            }
            star[y].x+=star[y].speed;
            if(star[y].x> SDL_GetVideoSurface()->w+100) star[y].x=0-(50*star[y].speed);
        }
        y=GetFade(2);
        ffy -= 3.4;
        if(ffy<(-100)) ffy=SDL_GetVideoSurface()->w+100;
        if(GAME_MODE!=RETRO_INTRO_PLAY)
        {
            DrawBar(0,147+y,SDL_GetVideoSurface()->w,168+y,LONGRGB(0,0,0),LONGRGB(255,255,255));
            DrawBar(0,148+y,SDL_GetVideoSurface()->w,167+y,LONGRGB(255,255,255),LONGRGB(0,0,0));
            DrawBar(0,149+y,SDL_GetVideoSurface()->w,166+y,LONGRGB(0,0,0),LONGRGB(255,255,255));
            WriteText(ffy   ,149+y+18*sin(ffy/23)     ,"^+^&^1S",7);
            WriteText((int)ffy+8 ,(int)149+y+18*sin((ffy+8)/23) ,"^+^&^ct",7);
            WriteText((int)ffy+16,(int)149+y+18*sin((ffy+16)/23),"^+^&^ca",7);
            WriteText((int)ffy+24,(int)149+y+18*sin((ffy+24)/23),"^+^&^dn",7);
            WriteText((int)ffy+32,(int)149+y+18*sin((ffy+32)/23),"^+^&^dd",7);
            WriteText((int)ffy+40,(int)149+y+18*sin((ffy+40)/23),"^+^&^eb",7);
            WriteText((int)ffy+48,(int)149+y+18*sin((ffy+48)/23),"^+^&^ey",7);
        }
        else
        {
            if(!cgltSDL)
            {
                cgltSDL=new CGLTexture();
                cgltSDL->Load(va("%s%cdata%csdl.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltSDL->Draw(80,SDL_GetVideoSurface()->h-96,80+128,SDL_GetVideoSurface()->h+22,255,255,255,255,255,255);
            }

            if(!cgltFMOD)
            {
                cgltFMOD=new CGLTexture();
                cgltFMOD->Load(va("%s%cdata%cfmod.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltFMOD->Draw(218,SDL_GetVideoSurface()->h-74,282,SDL_GetVideoSurface()->h-10,255,255,255,255,255,255);
            }

            if(!cgltOpenGL)
            {
                cgltOpenGL=new CGLTexture();
                cgltOpenGL->Load(va("%s%cdata%copengl.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
            }
            else
            {
                cgltOpenGL->Draw(292,SDL_GetVideoSurface()->h-108,292+128,SDL_GetVideoSurface()->h+28,255,255,255,255,255,255);
            }

			if(!cgltBit4ge)
			{
				cgltBit4ge=new CGLTexture();
				cgltBit4ge->Load(va("%s%cdata%cdlstorm.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP),0);
			}
			else
			{
				cgltBit4ge->Draw(512,SDL_GetVideoSurface()->h-168,768,SDL_GetVideoSurface()->h+88,255,255,255,255,255,255);
				//DrawBit4ge(10,SDL_GetVideoSurface()->h-74,74,SDL_GetVideoSurface()->h-10,0);
			}

        }
    }
		*/
}

/****************************************************************************************************/

void C_GFX::ToggleFullScreen(void)
{
    // const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();

    // if(VideoInfo->hw_available) { VideoFlags |= SDL_HWSURFACE; pLog->_Add("Hardware surfaces..."); }
    // else                        { VideoFlags |= SDL_SWSURFACE; pLog->_Add("Software surfaces..."); }

    // SDL_FULLSCREEN

	//if(pClientData->bFullScreen==false)
	//{

	 //   pClientData->bFullScreen=true;
	//}
	//else
	//{
	//    pClientData->bFullScreen=false;
	//}

#ifdef _WIN32

 	//ShutDownGFX();
 	//InitializeGFX(pClientData->bFullScreen);

#else

	if(!pScreen)
	{
	    pLog->_Add("FATAL ERROR: no pScreen!");
	    return;
    }

	if(SDL_WM_ToggleFullScreen(pScreen)!=1)
	{
		pLog->_Add("Unable to toggle fullscreen: [%s]\n", SDL_GetError() );
	}

	// 7034549721

#endif
}

/****************************************************************************************************/

void C_GFX::SetScreenRes(int x,int y,int cl, bool fs)
{
	ShutDownGFX();
 	InitializeGFX(x,y,cl,fs,WindowCaption,pLog,pGAF);
}

/****************************************************************************************************/ // BASE GFX

bool C_GFX::InitBaseGFX()
{
    BaseTexture=0;
	BaseTexture=new CGLTextureList[MAX_BASE_GFX];
	if(!BaseTexture)
	{
		pLog->_Add("BaseTexture initialization failure...");
		return false;
	}
    for(int i=0;i<MAX_BASE_GFX;i++)
    {


		BaseTexture[i].texture=0;



    }
    pLog->_Add("BaseGFX initialized...");
	return 1;
}

/****************************************************************************************************/

bool C_GFX::LoadBaseGFX(CGAF *pGAF) // Load in GFX Base
{
    pLog->_Add("Begin BaseTexture load...");
    DestroyBaseGFX();
	if(!InitBaseGFX())
	{
		pLog->_Add("BaseTexture load failure...");
		return false;
	}

    for(int i=0;i<MAX_BASE_GFX;i++)
	{
		Load1BaseGFX(pGAF,i);
	}
	return 1;
}

/****************************************************************************************************/

bool C_GFX::Load1BaseGFX(CGAF *pGAF,int which)
{
    char fname[1024];
    sprintf(fname,"base/b%04d.bmp",which);
	if(!BaseTexture[which].texture)
	{
		BaseTexture[which].texture=new CGLTexture();
        BaseTexture[which].texture->usemask=false;
        BaseTexture[which].texture->Load(pGAF,fname,1);
        strcpy(BaseTexture[which].texture->name,va("%d",which));
		return false;
	}
    if(!BaseTexture[which].texture)
	{
		pLog->_Add("ERROR: texture not valid");
		return false;
	}

	BaseTexture[which].texture->usemask=false;
   	BaseTexture[which].texture->Load(pGAF,fname,1);
    strcpy(BaseTexture[which].texture->name,va("%d",which));
    return 1;
}

/****************************************************************************************************/

CGLTexture *C_GFX::GetBaseGFX(char *name)
{
    if(!BaseTexture) return false;
    for(int i=0;i<MAX_BASE_GFX;i++)
        if(BaseTexture[i].texture)
            if(dlcs_strcasecmp(BaseTexture[i].texture->name,name))
                return BaseTexture[i].texture;
    return false;
}

/****************************************************************************************************/

bool C_GFX::DestroyBaseGFX(void)
{
    pLog->_Add("Begin BaseTexture destroy...");
	if(!BaseTexture) return 0;
    for(int i=0;i<MAX_BASE_GFX;i++) DEL(BaseTexture[i].texture);
    delete [] BaseTexture; BaseTexture=0;
    pLog->_Add("BaseTexture destroyed..."); return false;
}

/****************************************************************************************************/ // MODELS

bool C_GFX::InitModels()
{
	//pLog->_Add("Begin Models init...");
	//Model=new CGLModel();
	//FirstModel=Model;
	//pLog->_Add("Models initialized...");
	return 1;
}

/****************************************************************************************************/

bool C_GFX::LoadModels(void)
{
    /*
	int i=0;
	pLog->_Add("Begin Models load...");
	Model=FirstModel;
	if(!Model)
		Model=new CGLModel();
	FirstModel=Model;
	if(Model)
	{
		while(i<MAX_MODELS)
		{
			if(Load1Model(i))
			{
				Model->next=new CGLModel(CGAF *pGAF);
				Model=Model->next;
				pLog->_Add("Found model [%d]",i);
			}
			i++;
		}
	}
	pLog->_Add("Models loaded...");
	*/
	return 1;
}

/****************************************************************************************************/

bool C_GFX::Load1Model(int i)
{
    /*
	if(!Model)
		return false;
	if(
		!Model->Load(
		va("models/%04d/tris.md2",i),
		va("models/%04d/skin.bmp",i)
		)
		)
		return false;
	strcpy(Model->name,va("%d",i));
	*/
	return 1;
}

/****************************************************************************************************/

CGLModel *C_GFX::GetModel(char *name)
{
    /*
	Model=FirstModel;
	while(Model)
	{
		if(atoi(Model->name)==atoi(name))
		return Model; Model=Model->next;
	}
	*/
	return false;
}

/****************************************************************************************************/

bool C_GFX::DestroyModels(void)
{
    /*
	pLog->_Add("Begin Models destroy...");
	CGLModel *del;
	Model=FirstModel;
	while(Model)
	{
		del=Model;
		Model=Model->next;
		DEL(del);
	}
	pLog->_Add("Models destroyed...");
	*/
	return 1;
}

/****************************************************************************************************/

void C_GFX::RenderScene(void) // Render the game scene Frame
{
    //if(!camera)      return;

    static float x,y,z;

	static float lxp=0; lxp=lxp+.000021; if(lxp>360) lxp=0;
	static float lyp=0; lyp=lyp+.000021; if(lyp>360) lyp=0;
	static float lzp=0; lzp=lzp+.000021; if(lyp>360) lyp=0;

	static float sxp;
	static float syp;
	static float szp;

	sxp=(sin(lxp)/12)*1232;
	syp=(cos(lyp)/132)*1232;
	szp=(cos(lzp)/12)*1232;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    /*
    ---------------------------
    |
    |
    |
    |
    |
    |
    |
    |
    |
    |
    ___________________________


                x <->      y |      z (zoom)
                             |
    */

    static float tx,ty,tz,rx,ry,rz;

    glTranslatef((0.0f)+tx,(0.0f)+ty,(0.0f)+tz);

    glRotatef(rx,1.0f,0,0);
	glRotatef(rz,0,0,1.0f);



	// glRotatef(camera->ry,0,1.0f,0);


    /*
    glTranslatef(-1.5f,0.0f,-6.0f);

    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();

    glTranslatef(4.0f,0.0f,0.0f);

    glBegin(GL_QUADS);                      // Draw A Quad
        glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
        glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
    glEnd();
    */


	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(camera->x,camera->y,camera->z);
	//glTranslatef(syp,sxp,szp);
	//glDisable(GL_BLEND);

	//glEnable(GL_TEXTURE_2D);
    //if(SunTex) glBindTexture(GL_TEXTURE_2D, SunTex->bmap);

	// drawsphere(2,3.0f,1.0f,1.0f,.40f);


	// drawtri(6.0f,6.0f,1.0f,4,2,255,255,255);


	//glTranslatef(-syp,-sxp,-szp);
	//glTranslatef(0,0,10.0f);
	//glTranslatef(camera->ux,camera->uy,camera->uz);
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();

}

/****************************************************************************************************/
void C_GFX::DrawRect(RECT rc, long color)
{
    DrawRectangle(rc.left,rc.top,rc.left+rc.right,rc.top+rc.bottom,color);
}
/****************************************************************************************************/
void C_GFX::DrawRectangle(int iX,int iY,int iX2,int iY2,long color)
{
    DrawBar(iX,iY,iX+1,iY2,color,color);
    DrawBar(iX,iY,iX2,iY+1,color,color);
    DrawBar(iX2,iY,iX2+1,iY2,color,color);
    DrawBar(iX,iY2,iX2,iY2+1,color,color);
}
/****************************************************************************************************/
void C_GFX::DrawBar(RECT rc, long color)
{
    rc.right=rc.right+rc.left;
    rc.bottom=rc.bottom+rc.top;
    DrawBar(rc.left, rc.top, rc.right, rc.bottom, color, color);
}
/****************************************************************************************************/
void C_GFX::DrawBar(int iX,int iY,int iX2,int iY2,long color)
{
    DrawBar(iX,iY,iX2,iY2, color, color);
}
/****************************************************************************************************/
void C_GFX::DrawBar(RECT rc, long color1, long color2)
{
    rc.right=rc.right+rc.left;
    rc.bottom=rc.bottom+rc.top;
    DrawBar(rc.left, rc.top, rc.right, rc.bottom, color1, color2);
}
/****************************************************************************************************/
void C_GFX::DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2)
{
    iY=SDL_GetVideoSurface()->h-iY;
    iY2=SDL_GetVideoSurface()->h-iY2;
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
/****************************************************************************************************/
void C_GFX::DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2)
{
    iY=SDL_GetVideoSurface()->h-iY;
    iY2=SDL_GetVideoSurface()->h-iY2;
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}



/****************************************************************************************************/
void C_GFX::DrawVertice(int x, int y)
{
    DrawBar(x,y,x+2,y+2,LONGRGB(255,0,0),LONGRGB(0,0,255));
}

/****************************************************************************************************/

void C_GFX::DrawBaseGFX(int x,int y,int x2,int y2,int which,u_char r,u_char g,u_char b)
{
    if(!BaseTexture)
	{
		LoadBaseGFX(pGAF);
		return;
	}

    if(!BaseTexture[which].texture)
	{
		BaseTexture[which].texture = new CGLTexture();
		return;
	}

    if(!BaseTexture[which].texture->Loaded()) // 		!TOBOOL(BaseTexture[which].texture->bmap))
	{
		Load1BaseGFX(pGAF,which);
		return;
	}

    BaseTexture[which].texture->Draw(x,y,x2,y2,r,g,b);
}

/****************************************************************************************************
void C_GFX::DrawBit4ge(int x,int y,int x2,int y2,bool bsin)
{
    if(!cgltBit4ge)
    {
        cgltBit4ge=new CGLTexture();
        cgltBit4ge->Load("data/dlstorm.bmp",0);
        return;
    }



    static float fB4;
    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);

    fB4 +=.5;

	// cgltBit4ge->Draw(x,y,x3,y3,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f);

	// *

    // glLoadIdentity();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D,cgltBit4ge->bmap);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS );
    if(bsin)
    {


        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y-y3+ 5*sin(fB4/12)),    1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3+ 5*sin(fB4/15)),  float(y-y3+ 5*sin(fB4/13)),    1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3+ 5*sin(fB4/14)),  float(y+ 5*sin(fB4/25)),    1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y+ 5*sin(fB4/12)),    1.0f);
    }
    else
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),   float(y-y3),    1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),  float(y-y3),    1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),  float(y),    1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),   float(y),    1.0f);

    }
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
		*/
// }

/****************************************************************************************************/
void C_GFX::DrawModels(void)
{
    /*
    static float x,y,z;

    x=320.4f;
    y=90.5f;

    Model=FirstModel;
    if(Model)
    {
        Model->Rotate(x,y,0);
        Model->Scale(1.7f,1.7f,1.7f);
        Model->Locate(-160,-90,-10);
        glDisable(GL_BLEND);
        Model->Draw();
        Model=Model->next;

        if(Model)
        {
            Model->Rotate(x,y,0);
            Model->Scale(1.7f,1.7f,1.7f);
            Model->Locate(160,-90,-10);
            glDisable(GL_BLEND);
            Model->Draw();
            Model=Model->next;

            if(Model)
            {
                glEnable(GL_BLEND);
                Model->Rotate(x,y,0);
                Model->Scale(1.7f,1.7f,1.7f);
                Model->Draw();
            }
        }
    }
    */
}

/****************************************************************************************************/
void C_GFX::UpdatePickRay(GLfloat x,GLfloat y)
{
    GLfloat ray_pnt[4];
    GLfloat ray_vec[4];
    /*
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	glGetDoublev(  GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev(  GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	winX = x; winY = (GLfloat)viewport[3] - y;
	glReadPixels( x, GLint(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);
    gluUnProject( winX, winY, 1, modelview, projection, viewport, &posX2, &posY2, &posZ2);
    */
    GLint   mouse_x=x;//GetMouseX();//+x_offset;
    GLint   mouse_y=y;//GetMouseY();//+y_offset;
    GLfloat near_height,zNear,zFar,near_distance;
    GLint   window_width  = SDL_GetVideoSurface()->w;
    GLint   window_height = SDL_GetVideoSurface()->h;
    GLfloat aspect = double(window_width)/double(window_height);
    near_height=1.0f;
    near_distance=2.0f;
    zNear=0.0f;
    zFar=1.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_height * aspect, near_height * aspect, -near_height, near_height, zNear, zFar );
    // you can build your pick ray vector like this:
    GLint     window_y    = (window_height - mouse_y) - window_height/2;
    GLdouble  norm_y      = double(window_y)/double(window_height/2);
    GLint     window_x    = mouse_x - window_width/2;
    GLdouble  norm_x      = double(window_x)/double(window_width/2);
    // (Note that most window systems place the mouse coordinate origin in the upper left of the window instead of the lower left. That's why window_y is calculated the way it is in the above code. When using a glViewport() that doesn't match the window height, the viewport height and viewport Y are used to determine the values for window_y and norm_y.)
    // The variables norm_x and norm_y are scaled between -1.0 and 1.0. Use them to find the mouse location on your zNear clipping plane like so:
    //GLfloat y = near_height * norm_y;
    //GLfloat x = near_height * aspect * norm_x;
    // Now your pick ray vector is (x, y, -zNear).
    // To transform this eye coordinate pick ray into object coordinates,
    // multiply it by the inverse of the ModelView matrix in use when the
    // scene was rendered. When performing this multiplication, remember
    // that the pick ray is made up of a vector and a point, and that
    // vectors and points transform differently. You can translate and
    // rotate points, but vectors only rotate. The way to guarantee that
    // this is working correctly is to define your point and vector as
    // four-element arrays, as the following pseudo-code shows:
    //PickRayNear.x = ray_pnt[0] = 0.0f;
    //PickRayNear.y = ray_pnt[1] = 0.0f;
    //PickRayNear.z = ray_pnt[2] = 0.0f;
    ray_pnt[3] = 1.0f;
    //PickRayFar.x = ray_vec[0] = x;
    //PickRayFar.y =ray_vec[1] = y;
    //PickRayFar.z =ray_vec[2] = -near_distance;
    ray_vec[3] = 0.0f;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    // GLdouble gl_z = 100.0 ;  gl_x = mX + ((mZoom - gl_z) * (gl_x - mX) / (mZoom - gl_z - z))) ;  gl_y = mY + ((mZoom - gl_z) * (gl_y - mY) / (mZoom - gl_z - z))) ;

}


/****************************************************************************************************/
u_char C_GFX::GetFade(char cWhichFade)
{
    static u_char  cFader1=0;
    static unsigned long dwFadeTimer1=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime1=4;
    static char  cFadeDir1=1;
    static u_char  cFader2=0;
    static unsigned long dwFadeTimer2=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime2=3;
    static char  cFadeDir2=1;
    static u_char  cFader3=0;
    static unsigned long dwFadeTimer3=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime3=1;
    static char  cFadeDir3=1;

    static unsigned long dwFadeTimer4=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime4=1;
    static u_char  cFader4=0;

    if(dlcs_get_tickcount()-dwFadeTimer1 > dwFadeChangeTime1)
    {
        dwFadeTimer1=dlcs_get_tickcount();
        if(cFader1==255)
            cFadeDir1=-1;
        if(cFader1==0)
            cFadeDir1=1;
        cFader1+=cFadeDir1;
    }
    if(dlcs_get_tickcount()-dwFadeTimer2 > dwFadeChangeTime2)
    {
        dwFadeTimer2=dlcs_get_tickcount();
        if(cFader2==255)
            cFadeDir2=-1;
        if(cFader2==0)
            cFadeDir2=1;
        cFader2+=cFadeDir2;
    }
    if(dlcs_get_tickcount()-dwFadeTimer3 > dwFadeChangeTime3)
    {
        dwFadeTimer3=dlcs_get_tickcount();
        if(cFader3>=252)
            cFadeDir3=-3;
        if(cFader3<=3)
            cFadeDir3=3;
        cFader3+=cFadeDir3;
    }
    if(dlcs_get_tickcount()-dwFadeTimer4 > dwFadeChangeTime4)
    {
        cFader4++;
    }
    if(cWhichFade==1) return cFader1;
    if(cWhichFade==2) return cFader2;
    if(cWhichFade==3) return cFader3;
    if(cWhichFade==4) return cFader4;
    return 0;
}
