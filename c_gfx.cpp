/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gfx.cpp
 **   Class:        C_GFX
 **                 C_Camera
 **   Description:  SDL / OpenGL class wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 **   Link Libraries: OpenGL32 GLu32 GLaux SDLmain SDL
 **
 ***************************************************************/
#include "c_gfx.h"
//////////////////////////////////////////////////////////////// C_Camera CLASS CONSTRUCTOR / DESTRUCTOR
C_Camera::C_Camera() { Initialize(); }
C_Camera::~C_Camera() {  }
//////////////////////////////////////////////////////////////// C_Camera FUNCTIONS
void C_Camera::Initialize() {
    pFollowEntity=0;
    bounce=0.0f;
    loc.x=1.0f;
    loc.y=1.0f;
    loc.z=15.0f;
    rot.x=0.0f;
    rot.y=0.0f;
    rot.z=0.0f;
    scale.x=1.0f;
    scale.y=1.0f;
    scale.z=1.0f;
    angle = 0.0;
    bMovingBackward=0;
    bMovingForward1=0;
    bMovingForward2=0;
    bMovingLeft=0;
    bMovingRight=0;
    bRotatingLeft=0;
    bRotatingRight=0;
    bMovingUp=0;
    bMovingDown=0;
}
void C_Camera::Go() {
    glRotatef(rot.x,1.0f,0.0f,0.0f);
    glRotatef(rot.y,0.0f,1.0f,0.0f);
    glRotatef(rot.z,0.0f,0.0f,1.0f);
    glTranslatef(-loc.x,-loc.y,-loc.z);
}
void C_Camera::Update() {
    if(pFollowEntity) {
        loc.x=pFollowEntity->loc.x;
        loc.y=pFollowEntity->loc.y;
        loc.z=pFollowEntity->loc.z;
        // todo add camera angles
    } else {
        Move_Left();
        Move_Right();
        Move_Forward();
        Move_Backward();
        Move_Up();
        Move_Down();
        Rotate_Left();
        Rotate_Right();
    }
}
void C_Camera::Rotate_Left_Start() { bRotatingLeft=true; }
void C_Camera::Rotate_Left_Stop()  { bRotatingLeft=false; }
void C_Camera::Rotate_Left() {
    if(!bRotatingLeft) return;
    rot.y-=1.0f;
}
void C_Camera::Rotate_Right_Start() { bRotatingRight=true; }
void C_Camera::Rotate_Right_Stop()  { bRotatingRight=false; }
void C_Camera::Rotate_Right() {
    if(!bRotatingRight) return;
    rot.y+=1.0f;
}
void C_Camera::Move_Left_Start() { bMovingLeft=true; }
void C_Camera::Move_Left_Stop() { bMovingLeft=false; }
void C_Camera::Move_Left() {
    if(!bMovingLeft) return;
    float yrotrad = (rot.y / 180 * 3.141592654f);
    loc.x -= float(cos(yrotrad)) * scale.x;
    loc.z -= float(sin(yrotrad)) * scale.z;
}
void C_Camera::Move_Right_Start() { bMovingRight=true; }
void C_Camera::Move_Right_Stop() { bMovingRight=false; }
void C_Camera::Move_Right() {
    if(!bMovingRight) return;
    float yrotrad = (rot.y / 180 * 3.141592654f);
    loc.x += float(cos(yrotrad)) * scale.x;
    loc.z += float(sin(yrotrad)) * scale.z;

}
void C_Camera::Move_Forward_Start() { bMovingForward1=true; }
void C_Camera::Move_Forward_Stop() { bMovingForward1=false; }
void C_Camera::Move_Forward() {

    if((bMovingForward1) ||
       (bMovingForward2) ) {

        float xrotrad, yrotrad;
        yrotrad = (rot.y / 180 * 3.141592654f);
        xrotrad = (rot.x / 180 * 3.141592654f);
        loc.x += float(sin(yrotrad)) * scale.x;
        loc.z -= float(cos(yrotrad)) * scale.z;
        loc.y -= float(sin(xrotrad)) ;
        bounce += 0.04;
    }
}
void C_Camera::Move_Backward_Start() { bMovingBackward=true; }
void C_Camera::Move_Backward_Stop() { bMovingBackward=false; }
void C_Camera::Move_Backward() {
    if(!bMovingBackward) return;
    float xrotrad, yrotrad;
    yrotrad = (rot.y / 180 * 3.141592654f);
    xrotrad = (rot.x / 180 * 3.141592654f);
    loc.x -= float(sin(yrotrad)) * scale.x;
    loc.z += float(cos(yrotrad)) * scale.z;
    loc.y += float(sin(xrotrad));
    bounce += 0.04;
}
void C_Camera::Move_Up_Start() { bMovingUp=true; }
void C_Camera::Move_Up_Stop()  { bMovingUp=false; }
void C_Camera::Move_Up() {
    if(!bMovingUp) return;
    loc.y+=1.0f;
}
void C_Camera::Move_Down_Start() { bMovingDown=true; }
void C_Camera::Move_Down_Stop()  { bMovingDown=false; }
void C_Camera::Move_Down() {
    if(!bMovingDown) return;
    loc.y-=1.0f;
}
void C_Camera::mouseMovement(int x, int y) {
    static int lx;
    static int ly;
    int diffx=x-lx;
    int diffy=y-ly;
    diffx=0;
    if(x>lx) diffx=1;
    if(x<lx) diffx=-1;
    diffy=0;
    if(y>ly) diffy=1;
    if(y<ly) diffy=-1;
    rot.x += (float) diffy;
    rot.y += (float) diffx;
    if(rot.x < -60.0f) rot.x=-60.0f;
    if(rot.x > 60.0f)  rot.x=60.0f;
    if(rot.y < -5000.0f) rot.y=0.0f;
    lx=x;
    ly=y;
}
//////////////////////////////////////////////////////////////// C_GFX CLASS CONSTRUCTOR / DESTRUCTOR
C_GFX::C_GFX(int w, int h, int c, bool FullScreen, string wincaption, CLog *pUSELOG, CGAF *pUSEGAF) {
    InitializeGFX(w,h,c,FullScreen,wincaption,pUSELOG,pUSEGAF);
}
C_GFX::~C_GFX() {
    ShutDownGFX();
    SDL_Quit();
    pLog->_Add("SDL shut down...");
}
//////////////////////////////////////////////////////////////// GFX SYSTEM FUNCTIONS
bool C_GFX::InitializeGFX( int w, int h, int c, bool FullScreen, string wincaption,CLog *pUSELOG,CGAF *pUSEGAF) {
    bEditEntities=false;
    entities.clear();
    models.clear();
    textures.clear();
    pLog=pUSELOG;
    pGAF=pUSEGAF;
    pLog->_Add("Init SDL/OpenGL GFX Subsystem...");
    bSDLFailed=false;
#ifdef __linux__
putenv("SDL_VIDEODRIVER=x11");
#endif
    SDL_InitSubSystem(SDL_INIT_VIDEO);
/*
SDL_VIDEODRIVER=x
example:
x11
dga     (the XFree86 DGA2)
nanox   (Linux)
fbcon   (Linux)
directfb    (Linux)
ps2gs   (Playstation 2)
ggi
vgl (BSD)
svgalib (Linux)
aalib
directx (Win32)
windib  (Win32)
bwindow (BeOS)
toolbox (MacOS Classic)
DSp (MacOS Classic)
Quartz  (Mac OS X)
CGX (Amiga)
photon  (QNX)
epoc    (Epoc)
dummy

  if(SDL_VideoModeOK(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags|SDL_HWSURFACE)) {
    } else {
        pLog->_Add("SDL_HWSURFACE SDL_VideoModeOK failure");
        if(SDL_VideoModeOK(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags)) {
        }
        else {
            pLog->_Add("SDL_VideoModeOK failure");
            return false;
        }
    }  */

    pDefaultTexture=0;

    pCamera=0;
    bCreatedLog=false;
    bFullScreen =FullScreen;
    ScreenWidth =w;
    ScreenHeight=h;
    ScreenColors=c;

    VideoFlags = SDL_OPENGL|SDL_HWPALETTE|SDL_DOUBLEBUF;
    if(bFullScreen) VideoFlags |= SDL_FULLSCREEN;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,  16 );

    SDL_version ver;
    SDL_VERSION(&ver);
    pLog->_Add("SDL Version %d.%d.%d",ver.major,ver.minor,ver.patch);
        dlcsm_make_str(vdriver);
    SDL_VideoDriverName(vdriver,sizeof(vdriver));
     pLog->_Add("Video driver[%s]",vdriver);



    VideoModes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
    if(VideoModes == (SDL_Rect **)0){
    } else {
        if(VideoModes == (SDL_Rect **)-1) pLog->_DebugAdd("All resolutions available");
        else {
            pLog->_DebugAdd("Available Modes");
            for(int i=0;VideoModes[i];++i)
                pLog->_DebugAdd("  %d x %d", VideoModes[i]->w, VideoModes[i]->h);
        }
    }

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();

    if(VideoInfo) {
        pLog->_DebugAdd("VideoInfo->hw_available [%d]        ",VideoInfo->hw_available);
        pLog->_DebugAdd("VideoInfo->wm_available [%d]        ",VideoInfo->wm_available);
        pLog->_DebugAdd("VideoInfo->blit_hw [%d]             ",VideoInfo->blit_hw);
        pLog->_DebugAdd("VideoInfo->blit_hw_CC [%d]          ",VideoInfo->blit_hw_CC);
        pLog->_DebugAdd("VideoInfo->blit_hw_A [%d]           ",VideoInfo->blit_hw_A);
        pLog->_DebugAdd("VideoInfo->blit_sw [%d]             ",VideoInfo->blit_sw);
        pLog->_DebugAdd("VideoInfo->blit_sw_CC [%d]          ",VideoInfo->blit_sw_CC);
        pLog->_DebugAdd("VideoInfo->blit_sw_A [%d]           ",VideoInfo->blit_sw_A);
        pLog->_DebugAdd("VideoInfo->blit_fill [%d]           ",VideoInfo->blit_fill);
        pLog->_DebugAdd("VideoInfo->video_mem [%d]           ",VideoInfo->video_mem);
        pLog->_DebugAdd("VideoInfo->vfmt->BitsPerPixel [%d]  ",VideoInfo->vfmt->BitsPerPixel);
        pLog->_DebugAdd("VideoInfo->vfmt->BytesPerPixel [%d] ",VideoInfo->vfmt->BytesPerPixel);
        pLog->_DebugAdd("VideoInfo->vfmt->Rloss [%d]         ",VideoInfo->vfmt->Rloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Gloss [%d]         ",VideoInfo->vfmt->Gloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Bloss [%d]         ",VideoInfo->vfmt->Bloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Aloss [%d]         ",VideoInfo->vfmt->Aloss);
        pLog->_DebugAdd("VideoInfo->vfmt->Rshift [%d]        ",VideoInfo->vfmt->Rshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Gshift [%d]        ",VideoInfo->vfmt->Gshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Bshift [%d]        ",VideoInfo->vfmt->Bshift);
        pLog->_DebugAdd("VideoInfo->vfmt->Ashift [%d]        ",VideoInfo->vfmt->Ashift);
        pLog->_DebugAdd("VideoInfo->vfmt->Rmask [%d]         ",VideoInfo->vfmt->Rmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Gmask [%d]         ",VideoInfo->vfmt->Gmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Bmask [%d]         ",VideoInfo->vfmt->Bmask);
        pLog->_DebugAdd("VideoInfo->vfmt->Amask [%d]         ",VideoInfo->vfmt->Amask);
        pLog->_DebugAdd("VideoInfo->vfmt->colorkey [%d]      ",VideoInfo->vfmt->colorkey);
        pLog->_DebugAdd("VideoInfo->vfmt->alpha [%d]         ",VideoInfo->vfmt->alpha);
    } else {
        bSDLFailed=true;
        pLog->_Add("Failed getting Video Info : %s",SDL_GetError());
        return false;
    }
    if(VideoInfo->hw_available) { VideoFlags |= SDL_HWSURFACE; pLog->_Add("Hardware surfaces...");}
    else { VideoFlags |= SDL_SWSURFACE; pLog->_Add("Software surfaces..."); }
    if(VideoInfo->blit_hw) { VideoFlags |= SDL_HWACCEL; pLog->_Add("Hardware acceleration enabled!"); }

    if(SDL_VideoModeOK(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags)) {    }
    else { pLog->_Add("SDL_VideoModeOK failure");         return false;    }
    pScreen=SDL_SetVideoMode(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags);
    if(!pScreen) {
        bSDLFailed=true;
        pLog->_Add("SDL_SetVideoMode failed");
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE);
    SetWindowTitle(wincaption);

    if(InitGL(ScreenWidth, ScreenHeight)) {
        pLog->_Add("OpenGL initialized");
    }
    else {
        bSDLFailed=true;
        pLog->_Add("Can't initialize OpenGL");
        return false;
    }

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

    pCamera=new C_Camera();
    if(pCamera) {
        pCamera->Move_Left_Stop();
        pCamera->Move_Right_Stop();
        pCamera->Move_Forward_Stop();
        pCamera->Move_Backward_Stop();
        pLog->_Add("Camera initialized");
    } else {
        pLog->_Add("Can't initialize Camera");
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    // Textures
    textures.clear();
    if(LoadTextures(pGAF)) {
        pLog->_Add("[%d] base textures loaded",textures.size());
    } else {
        pLog->_Add("Can't initialize base textures");
        return false;
    }
    pDefaultTexture=GetTexture("base/testprog.jpg");
    if(!pDefaultTexture) { pLog->_Add("Can't initialize Default Texture"); return false; }

    ///////////////////////////////////////////////////////////////////////
    // Models
    models.clear();
    if(LoadModels()) {
        pLog->_Add("[%d] models loaded",models.size());
    }
    else {
        pLog->_Add("Can't initialize models");
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    // Entities

    entities.clear();
    InitializeEntities();
    pSelectedEntity=0;

    pLog->_Add("[%d] entities created",entities.size());

    pLog->_Add("GFX Initialized");
    return true;
}
void C_GFX::SetWindowTitle(string fmt, ...) {
    char ach[512];
    char f2[512];
    strcpy(f2,fmt.c_str());
    va_list va;
    va_start(va, f2);
    vsprintf(ach, f2, va);
    va_end(va);
    windowcaption.assign(ach);
    SDL_WM_SetCaption(windowcaption.c_str(),0);
}
void C_GFX::ToggleFullScreen(void) {
#ifdef __linux__
    SDL_WM_ToggleFullScreen(pScreen);
#endif
    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();
    if(VideoInfo->hw_available) { VideoFlags |= SDL_HWSURFACE; pLog->_Add("Hardware surfaces..."); }
    else                        { VideoFlags |= SDL_SWSURFACE; pLog->_Add("Software surfaces..."); }
    if(bFullScreen==true)   bFullScreen=false;
    else bFullScreen=true;
    SDL_SetVideoMode(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags^SDL_FULLSCREEN);
    InitGL(ScreenWidth,ScreenHeight);
    LoadTextures(pGAF);
}
void C_GFX::ShutDownGFX(void) {
    glFinish();
    glFlush();
    pLog->_Add("OpenGL shut down...");
    ClearEntities();
    pLog->_Add("Entities shut down...");
    dlcsm_delete(pCamera);
    pLog->_Add("Camera shut down...");
    DestroyTextures();
    pLog->_Add("Textures shut down...");
    DestroyModels();
    pLog->_Add("Models shut down...");
    SDL_FreeSurface(pScreen);
}
int C_GFX::InitGL(int x, int y) {
    float ratio = (float) x / (float) y;
    glShadeModel( GL_SMOOTH );     // Our shading model--Gouraud (smooth).
    glCullFace( GL_BACK ); // Culling.
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glClearColor(0, 0, 0, 0); // Set the clear color.
    glViewport(0, 0, x, y); // Setup our viewport.
    glMatrixMode(GL_PROJECTION);     // Change to the projection matrix and set our viewing volume.
    glLoadIdentity();
    gluPerspective(60.0, ratio, 1.0, 1024.0);     // EXERCISE:* Replace this with a call to glFrustum.
    glEnable(GL_TEXTURE_2D);
/*  glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);
    //glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    //glLoadIdentity();									// Reset The Projection Matrix
    //gluPerspective(45.0f,(GLfloat)SDL_GetVideoSurface()->w/(GLfloat)SDL_GetVideoSurface()->h,1.0f,5000.0f); // gluPerspective(45.0f,1.333f,0.1f,20000.0f);
    //glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix*/
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        pLog->_Add( "Error initializing OpenGL! %s\n", gluErrorString(error));
        return false;
    }
    return true; // Initialization Went OK
}
void C_GFX::EndScene(void) {
    FlipSurfaces();
}
void C_GFX::FlipSurfaces(void) {
    glFlush();
    SDL_GL_SwapBuffers();
}
void C_GFX::BeginScene(void) {
    GetFade(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(pCamera) pCamera->Update();
}
void C_GFX::SetScreenRes(int x,int y,int cl, bool fs) {
    //ShutDownGFX();
    //InitializeGFX(x,y,cl,fs,WindowCaption,pLog,pGAF);
}
//////////////////////////////////////////////////////////////// TEXTURES
bool C_GFX::LoadTextures(CGAF *pGAF) {
    pLog->_Add("Loading textures");
    CGLTexture *pTexture=0;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir("base");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if((!(dlcs_strcasecmp(epdf->d_name,".")) || (dlcs_strcasecmp(epdf->d_name,".."))) && (!dlcs_isdir(epdf->d_name))) {
                pTexture=new CGLTexture(pLog,va("base/%s",epdf->d_name));
                if(pTexture) {
                    if(!pTexture->glBmap) {
                        pLog->_Add("ERROR LOADING base/%s (CGLTEXTURE OBJECT DESTROYED)",epdf->d_name);
                        dlcsm_delete(pTexture);
                    }
                    else {
                        textures.push_back(pTexture);
                        pLog->_DebugAdd("Texture[%s] width[%d] height[%d] bpp[%d] GL[%d] )",
                                            pTexture->filename.c_str(),
                                            pTexture->width,
                                            pTexture->height,
                                            pTexture->bpp,
                                            pTexture->glBmap);
                    }
                }
            }
        }
    }
    closedir(dpdf);
    return true;
}
CGLTexture* C_GFX::GetTexture(string name) {
    for(vector<CGLTexture*>::iterator it = textures.begin() ; it != textures.end(); ++it) {
            if((**it).filename==name) return &(**it);
        }
    return 0;
}
CGLTexture* C_GFX::GetRandomTexture(void) {
    int x=(rand()%textures.size())+1; int y=0;
    for(vector<CGLTexture*>::iterator it = textures.begin() ; it != textures.end(); ++it) {
        if(x==y) return &(**it);
        y++;
    }
    return false;

}
bool C_GFX::DestroyTextures(void) {
    dlcsm_delete_vector(CGLTexture*,textures); /*  for(vector<CGLTexture*>::iterator it = textures.begin() ; it != textures.end(); ) {  lcsm_delete(*it);   it = textures.erase(it);    }*/
    return true;
}
//////////////////////////////////////////////////////////////// MODELS
bool C_GFX::LoadModels(void) {
    pLog->AddEntry("Loading models...\n");
    string filename;
    CGLModel *pModel;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir("models");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if( (!((dlcs_strcasecmp(epdf->d_name,".")) || (dlcs_strcasecmp(epdf->d_name,".."))))  &&
                (!dlcs_isdir(va("models/%s",epdf->d_name)))) {
                filename.assign(va("models/%s",epdf->d_name));
                pModel=new CGLModel(this,pLog);
                if(pModel->Load(filename))  models.push_back(pModel);
                else                        dlcsm_delete(pModel);
            }
        }
    }
    closedir(dpdf);
    return true;
}
CGLModel *C_GFX::GetModel(string inname) {
    for(vector<CGLModel*>::iterator it = models.begin() ; it != models.end(); ++it) {
            if((**it).name==inname) return &(**it); } return false; }
int C_GFX::GetTotalModels(void) { return models.size(); }
CGLModel* C_GFX::GetRandomModel(void) {
    int x=(rand()%models.size())+1; int y=0;
    for(vector<CGLModel*>::iterator it = models.begin() ; it != models.end(); ++it) {
        if(x==y) return &(**it);
        y++;
    }
    return false;
}
bool C_GFX::DestroyModels(void) { dlcsm_delete_vector(CGLModel*,models); return true; }
//////////////////////////////////////////////////////////////// RENDER SCENE
void C_GFX::RenderScene(int mx, int my) {
    glRenderMode(_glRendermode);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawFog();
    DrawSkyBox(); // TODO: Fix Skybox
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    pCamera->Go();
    DrawEntities();
}
//////////////////////////////////////////////////////////////// MISC 2D DRAW FUNCTIONS
void C_GFX::DrawRect(RECT rc, long color) {
    DrawRectangle(rc.left,rc.top,rc.left+rc.right,rc.top+rc.bottom,color);
}
void C_GFX::DrawRect(int iX, int iY, int iX2, int iY2, long color) {
    DrawRectangle(iX,iY,iX2,iY2,color);
}
void C_GFX::DrawRectangle(int iX,int iY,int iX2,int iY2,long color) {
    DrawBar(iX,iY,iX+1,iY2,color,color);
    DrawBar(iX,iY,iX2,iY+1,color,color);
    DrawBar(iX2,iY,iX2+1,iY2,color,color);
    DrawBar(iX,iY2,iX2,iY2+1,color,color);
}
void C_GFX::DrawBar(RECT rc, long color) {
    rc.right=rc.right+rc.left;
    rc.bottom=rc.bottom+rc.top;
    DrawBar(rc.left, rc.top, rc.right, rc.bottom, color, color);
}
void C_GFX::DrawBar(int iX,int iY,int iX2,int iY2,long color) {
    DrawBar(iX,iY,iX2,iY2, color, color);
}
void C_GFX::DrawBar(RECT rc, long color1, long color2) {
    rc.right=rc.right+rc.left;
    rc.bottom=rc.bottom+rc.top;
    DrawBar(rc.left, rc.top, rc.right, rc.bottom, color1, color2);
}
void C_GFX::DrawBar(int iX,int iY,int iX2,int iY2,long color1,long color2) {
    iY=SDL_GetVideoSurface()->h-iY;
    iY2=SDL_GetVideoSurface()->h-iY2;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0.375,0.375,0);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
void C_GFX::DrawTransparentBar(int iX,int iY,int iX2,int iY2,long color1,long color2) {
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
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(iX ),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(iX2),  float(iY),    1.0f);
    glColor3ub( ((color2) & 0xff), ((color2>>8 ) & 0xff), ((color2>>16) & 0xff) );
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(iX2),  float(iY2),   1.0f);
    glColor3ub( ((color1) & 0xff), ((color1>>8 ) & 0xff), ((color1>>16) & 0xff) );
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(iX),  float(iY2),   1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
void C_GFX::DrawVertice(int x, int y) {
    DrawBar(x,y,x+2,y+2,LONGRGB(255,0,0),LONGRGB(0,0,255));
}
void C_GFX::DrawTexture(int x,int y,int x2,int y2,string name,u_char r,u_char g,u_char b) {
    CGLTexture *pTexture;
    pTexture=0;
    pTexture=GetTexture(name);
    if(pTexture)
        pTexture->Draw2d(x,y,x2,y2,r,g,b);
}
void C_GFX::Draw3DBox(int x, int y, int x2, int y2) {
    RECT r;
    r.left=x;
    r.top=y;
    r.right=(x2-x);
    r.bottom=(y2-y);
    Draw3DBox(r);
}
void C_GFX::Draw3DBox(RECT rect) {
    DrawBar(  rect.left,
              rect.top,
              rect.left+rect.right-2,
              rect.top+rect.bottom-2,
              LONGRGB(240,240,240),
              LONGRGB(240,240,240));

    DrawBar(  rect.left+2,
              rect.top+2,
              rect.left+rect.right,
              rect.top+rect.bottom,
              LONGRGB(80,80,80),
              LONGRGB(80,80,80));

    DrawBar(  rect.left+2,
              rect.top+2,
              rect.left+rect.right-2,
              rect.top+rect.bottom-2,
              LONGRGB(180,180,180),
              LONGRGB(180,180,180));
}
//////////////////////////////////////////////////////////////// MISC 3D DRAW FUNCTIONS
void C_GFX::DrawFog(void) {
    /*  GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat density = 0.02; //set the density to 0.3 which isacctually quite thick
    glEnable (GL_FOG);              //enable this for fog
    glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
    glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to our color chosen above
    glFogf (GL_FOG_DENSITY, density); //set the density to the value above
    glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the nicest, may slow down on older cards
    glFogf (GL_FOG_START, 10.0f);
    glFogf (GL_FOG_END, 680.0f);   // Enable Pointers
    //glFogi (GL_FOG_MODE, GL_LINEAR); */
}
void C_GFX::DrawSun(void) {
    static float der;
    static float zpos;
    static float ypos;
    float skycolor_r;
    float skycolor_g;
    float skycolor_b;
    der+=.0002;
    ypos = sin(der) * 1550;
    zpos = cos(der) * 1550;
    skycolor_r=((ypos+300)/1549);
    skycolor_g=((ypos+300)/1549);
    skycolor_b=((ypos+300)/1549);
    if(skycolor_r > 0.4f) skycolor_r=0.4f;
    if(skycolor_r < 0.0f) skycolor_r=0.0f;
    if(skycolor_g > 0.2f) skycolor_g=0.2f;
    if(skycolor_g < 0.0f) skycolor_g=0.0f;
    if(skycolor_b > 1.0f) skycolor_b=1.0f;
    if(skycolor_b < 0.0f) skycolor_b=0.0f;
    glClearColor( skycolor_r , skycolor_g, skycolor_b ,0);

    glLoadIdentity();

    glTranslatef(0.0f,ypos,zpos);
    CGLTexture *pTexture=0;
    pTexture=GetTexture("base/sun.png");
    if(pTexture) glBindTexture(GL_TEXTURE_2D, pTexture->glBmap);
    glDisable(GL_BLEND);
    DrawSphere(3,160.0f,1.0f,1.0f,1.0f);
}
void C_GFX::DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb) {
/*
    if (div<=0) {
        glNormal3fv(a);
        glColor3f(cr,cg,cb);
        glVertex3f(a[0]*r, a[1]*r, a[2]*r);
        glNormal3fv(b);
        glColor3f(cr,cg,cb);
        glVertex3f(b[0]*r, b[1]*r, b[2]*r);
        glNormal3fv(c);
        glColor3f(cr,cg,cb);
        glVertex3f(c[0]*r, c[1]*r, c[2]*r);
    } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0; i<3; i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab);
        normalize(ac);
        normalize(bc);
        DrawTri(a, ab, ac, div-1, r,cr,cg,cb);
        DrawTri(b, bc, ab, div-1, r,cr,cg,cb);
        DrawTri(c, ac, bc, div-1, r,cr,cg,cb);
        DrawTri(ab, bc, ac, div-1, r,cr,cg,cb);  //<--Comment this line and sphere looks really cool!
    }
    */
}
void C_GFX::DrawSphere(int ndiv, float radius, float cr,float cg,float cb) {
    /*
    glBegin(GL_TRIANGLES);
    for (int i=0; i<20; i++)
        DrawTri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius,cr,cg,cb);
    glEnd();
    */
}
void C_GFX::DrawCube() {
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    // Right face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glEnd();
}
void C_GFX::DrawSkyBox(void) {

    return;
    glPushMatrix();

    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1.0f,1.0f,1.0f,1.0f);

    glTranslatef(pCamera->loc.x, pCamera->loc.y, pCamera->loc.z);
    glScalef(1200.0f,1200.0f,1200.0f);


    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.front.png")->glBmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();


    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.back.png")->glBmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
    glEnd();

    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.left.png")->glBmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
    glEnd();

    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.right.png")->glBmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glEnd();

    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.top.png")->glBmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();

    // Render the bottom quad
    if(GetTexture("base/sb1.bottom.png")) {
        glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.bottom.png")->glBmap);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glEnd();
    }

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}
void C_GFX::DrawStarField(int iDir) {
    struct stra {
        float x;
        float y;
        float speed;
        char gfx[1024];
    };
    static stra star[500];
    static bool bstars;
    static float ffy;
    static float fff;
    static float dir=-.405f;
    int y;
    // glClearColor( (0.2f),(0.4f),(1.0f),0);    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // DrawBaseGFX(0,0,SDL_GetVideoSurface()->w,SDL_GetVideoSurface()->h,"base/b0121.png",155,155,155);
    if(!bstars){
        for(y=0;y<500;y++)  {
            star[y].x=(rand()%
                       SDL_GetVideoSurface()->w
                       -100);
            star[y].y= rand()%
            SDL_GetVideoSurface()->h;//            -150;
            star[y].speed=(rand()%24)*0.48f+1.3f;
            strcpy(star[y].gfx,"base/star1.png");
            if((rand()%100)>50)
                strcpy(star[y].gfx,"base/star2.png");
        }
        bstars=1;
    }
    fff+=dir;
    if(fff>34)  dir =(-0.405f);
    if(fff<-34) dir =( 0.405f);
    for(y=0;y<500;y++){
        DrawTexture( (int)(star[y].x),
                     (int)(600-star[y].y-30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                     (int)(star[y].x+2+star[y].speed ),
                     (int)(600-star[y].y+1+star[y].speed -30+180*sin(fff/223)+180*cos((star[y].x-130)/380)),
                     star[y].gfx,
                     255,255,255);
                     //                                          GetFade(3),                     GetFade(2),                     GetFade(1) );
        star[y].x+=star[y].speed;
        if(star[y].x > SDL_GetVideoSurface()->w) star[y].x=0;//-(50*star[y].speed);
    }
    //y=GetFade(2);
    //ffy -= 3.4;    if(ffy<(-100)) ffy=SDL_GetVideoSurface()->w+100;

}
//////////////////////////////////////////////////////////////// OTHER GFX FUNCTIONS
u_char C_GFX::GetFade(char cWhichFade) {
    static u_char  cFader1=0;
    static unsigned long dwFadeTimer1=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime1=400;
    static char  cFadeDir1=1;
    static u_char  cFader2=0;
    static unsigned long dwFadeTimer2=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime2=300;
    static char  cFadeDir2=1;
    static u_char  cFader3=0;
    static unsigned long dwFadeTimer3=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime3=100;
    static char  cFadeDir3=1;

    static unsigned long dwFadeTimer4=dlcs_get_tickcount();
    static unsigned long dwFadeChangeTime4=100;
    static u_char  cFader4=0;

    if(dlcs_get_tickcount()-dwFadeTimer1 > dwFadeChangeTime1) {
        dwFadeTimer1=dlcs_get_tickcount();
        if(cFader1==255)
            cFadeDir1=-1;
        if(cFader1==0)
            cFadeDir1=1;
        cFader1+=cFadeDir1;
    }
    if(dlcs_get_tickcount()-dwFadeTimer2 > dwFadeChangeTime2) {
        dwFadeTimer2=dlcs_get_tickcount();
        if(cFader2==255)
            cFadeDir2=-1;
        if(cFader2==0)
            cFadeDir2=1;
        cFader2+=cFadeDir2;
    }
    if(dlcs_get_tickcount()-dwFadeTimer3 > dwFadeChangeTime3) {
        dwFadeTimer3=dlcs_get_tickcount();
        if(cFader3>=252)
            cFadeDir3=-3;
        if(cFader3<=3)
            cFadeDir3=3;
        cFader3+=cFadeDir3;
    }
    if(dlcs_get_tickcount()-dwFadeTimer4 > dwFadeChangeTime4) {
        cFader4++;
    }
    if(cWhichFade==1) return cFader1;
    if(cWhichFade==2) return cFader2;
    if(cWhichFade==3) return cFader3;
    if(cWhichFade==4) return cFader4;
    return 0;
}
//////////////////////////////////////////////////////////////// ENTITY FUNCTIONS
void C_GFX::InitializeEntities(void) {
    pLog->_Add(" GOING NUTS WITH ENTITIES!!!!!!!!!!!! ");
    ClearEntities();
    int i;
    int numntt;
    numntt=300;
    string nout;
    for(i=0;i<numntt;i++){
        nout.assign(va("Entity %d",i));
        MakeEntity(nout,0,0,0);
    }
}
void C_GFX::MakeEntity(string name,float x, float y, float z) {
    C_Entity *pNTT = new C_Entity(pLog,pGAF,this,0);
    if(pNTT) {
        pNTT->name=name;
        pLog->_DebugAdd("ENTITY IS NAMED [%s]",pNTT->name.c_str());

        if(x) pNTT->loc.x=x;
        else  pNTT->loc.x     = (((float)rand()/(float)RAND_MAX)*1250)-625;
        if(y) pNTT->loc.y=y;
        else  pNTT->loc.y     = (((float)rand()/(float)RAND_MAX)*500)+20;
        if(z) pNTT->loc.z=z;
        else  pNTT->loc.z     = (((float)rand()/(float)RAND_MAX)*1250)-625;

        pNTT->rot.x     = ( (float)rand()/(float)RAND_MAX)*360;
        pNTT->rot.y     = ( (float)rand()/(float)RAND_MAX)*360;
        pNTT->rot.z     = ( (float)rand()/(float)RAND_MAX)*360;
        pNTT->autorot.x = ( (float)rand()/(float)RAND_MAX)*1.5f;
        pNTT->autorot.y = ( (float)rand()/(float)RAND_MAX)*1.5f;
        pNTT->autorot.z = ( (float)rand()/(float)RAND_MAX)*1.5f;
        pNTT->scale.x   = (((float)rand()/(float)RAND_MAX)*5.0f)+1;
        pNTT->scale.y   = (((float)rand()/(float)RAND_MAX)*5.0f)+1;
        pNTT->scale.z   = (((float)rand()/(float)RAND_MAX)*5.0f)+1;
        pNTT->color.r   = 0.8f;
        pNTT->color.g   = 0.8f;
        pNTT->color.b   = 0.8f;

        pNTT->pTexture=GetRandomTexture();

        pNTT->type  = ENTITY_STATIC;
        if((rand()%100)>90) pNTT->type=ENTITY_LIGHT;
        if((rand()%100)>90) pNTT->type=ENTITY_SOUND;
        if((rand()%100)>90) pNTT->type=ENTITY_AURA;
        if((rand()%100)>90) pNTT->type=ENTITY_NPC;

        switch(pNTT->type) {
            //case ENTITY_PLAYER:
            //case ENTITY_NPC:
            //case ENTITY_NPC_SPAWN:
            //case ENTITY_NPC_GENERATOR:
            case ENTITY_PLAYER_SPAWN:
                pNTT->pTexture=GetTexture("base/ntt.ankh.png");
                break;
            case ENTITY_LIGHT:
                pNTT->pTexture=GetTexture("base/ntt.light.png");
                break;
            case ENTITY_SOUND:
                pNTT->pTexture=GetTexture("base/ntt.audio.png");
                break;
            case ENTITY_AURA:
                pNTT->pTexture=GetTexture("base/ntt.aura.png");
                break;
            default:
                break;
        }

        if(!pNTT->pTexture) pNTT->pTexture=pDefaultTexture;

        if( (rand()%100)> 50 ) {
            pNTT->pModel=GetRandomModel();
            pNTT->pTexture=0;
            if(pNTT->pModel)
                pLog->_DebugAdd("Entity :%s", pNTT->pModel->name.c_str());
            //pNTT->loc.x = 0.0f;
            pNTT->loc.y = 0.0f;
            //pNTT->loc.z = 0.0f;
            pNTT->scale.x = 0.3f;
            pNTT->scale.y = 0.3f;
            pNTT->scale.z = 0.3f;
            pNTT->rot.x = 0.0f;
            // pNTT->rot.y = 0.0f;
            pNTT->rot.z = 0.0f;
            pNTT->autorot.x = 0.0f;
            pNTT->autorot.y = 0.0f;
            pNTT->autorot.z = 0.0f;
        }
        if(pNTT->pTexture) if(pNTT->pTexture->glBmap)
        pLog->_DebugAdd("Entity texture set to [%s] GL[%d]", pNTT->pTexture->filename.c_str(), pNTT->pTexture->glBmap);
        entities.push_back(pNTT);
    }
}
void C_GFX::DrawEntities(void) {
    for(vector<C_Entity*>::iterator it=entities.begin(); it<entities.end(); it++ )
        (*it)->Draw(bEditEntities);
}
C_Entity* C_GFX::GetSelectedEntity(void) { return pSelectedEntity; }
void C_GFX::ClearSelectEntity(void) {
    for(vector<C_Entity*>::iterator it=entities.begin(); it<entities.end(); it++ ){
        (*it)->bSelected=false;
    }
    pSelectedEntity=0;
}
void C_GFX::SelectEntity(C_Entity* pEntity) {
    ClearSelectEntity();
    pSelectedEntity=pEntity;
    pSelectedEntity->bSelected=true;
}
void C_GFX::SelectClosestEntity(void) {
    ClearSelectEntity();
    dlcs_V3 thisloc;
    dlcs_V3 v;
    thisloc=pCamera->loc;
    float dist;
    float closestDist = 1000000000.0f;
    pSelectedEntity=0;
    for(vector<C_Entity*>::iterator it=entities.begin(); it<entities.end(); it++ ){
        v.x=(*it)->loc.x-thisloc.x;
        v.y=(*it)->loc.y-thisloc.y;
        v.z=(*it)->loc.z-thisloc.z;
        dist = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
        if (dist < closestDist) {
            closestDist = dist;
            if(pSelectedEntity) pSelectedEntity->bSelected=false;
            pSelectedEntity=(*it);
            pSelectedEntity->bSelected=true;
            return;
        }
    }
}
void C_GFX::DeleteEntity(C_Entity* pEntity) {
    for(vector<C_Entity*>::iterator it=entities.begin(); it<entities.end(); it++){
        if((*it)==pEntity) {
            dlcsm_delete(*it);
            it=entities.erase(it);
            return;
        }
    }
}
void C_GFX::ClearEntities(void) { dlcsm_delete_vector(C_Entity*,entities); entities.clear(); }
void C_GFX::LoadEntities(dlcs_V3 WhichSector) { }
void C_GFX::SaveEntities(dlcs_V3 WhichSector) { }



