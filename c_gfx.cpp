/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#include "c_gfx.h"
#define X .525731112119133606
#define Z .850650808352039932
#ifdef _WIN32
#ifndef _GL_VBO_STUFF
#define _GL_VBO_STUFF
/* PFNGLGENBUFFERSARBPROC            glGenBuffersARB = 0;             // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC            glBindBufferARB = 0;             // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC            glBufferDataARB = 0;             // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC         glBufferSubDataARB = 0;          // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC         glDeleteBuffersARB = 0;          // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC  glGetBufferParameterivARB = 0;   // return various parameters of VBO
PFNGLMAPBUFFERARBPROC             glMapBufferARB = 0;              // map VBO procedure
PFNGLUNMAPBUFFERARBPROC           glUnmapBufferARB = 0;            // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB */
#endif
#endif
// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
// vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (36 * 3 = 108).
GLfloat Cubevertices[]  =                       { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
        -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

        1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
        1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

        1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
        -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

        -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
        -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

        -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
        1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

        1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
        -1, 1,-1,   1, 1,-1,   1,-1,-1
                                                };    // v6-v5-v4
// normal array
GLfloat Cubenormals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
                            0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

                            1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
                            1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

                            0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
                            0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

                            -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
                            -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

                            0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
                            0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

                            0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
                            0, 0,-1,   0, 0,-1,   0, 0,-1
                          };    // v6-v5-v4
// color array
GLfloat Cubecolors[]    = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
                            1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

                            1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
                            0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

                            1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
                            0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

                            1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
                            0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

                            0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
                            1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

                            0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
                            0, 1, 0,   0, 1, 1,   0, 0, 1
                          };    // v6-v5-v4
//GLfloat mapvertices[1024][1024];
static GLfloat vdata[12][3] = {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};
void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d;
    a[1]/=d;
    a[2]/=d;
}

C_Camera::C_Camera() {
    Initialize();
}
C_Camera::~C_Camera() {  }
void C_Camera::Initialize() {
    pFollowEntity=0;
    cScale = 1.0;
    bounce = 0.0;
    xpos  = 0.0f;
    ypos  = 0.0f;
    zpos  = 15.0f;
    xrot  = 0;
    yrot  = 0;
    angle = 0.0;
    bMovingBackward=0;
    bMovingForward=0;
    bMovingLeft=0;
    bMovingRight=0;
}
void C_Camera::Go() {
    glRotatef(xrot,1.0,0.0,0.0);
    glRotatef(yrot,0.0,1.0,0.0);
    glTranslated(-xpos,-ypos,-zpos);
}
void C_Camera::Update() {
    if(pFollowEntity) {
        xpos=pFollowEntity->Pos.x;
        ypos=pFollowEntity->Pos.y;
        zpos=pFollowEntity->Pos.z;
        // todo add camera angles
    } else {
        Move_Left();
        Move_Right();
        Move_Forward();
        Move_Backward();
    }
}
void C_Camera::Rotate_Left() { }
void C_Camera::Rotate_Right() { }
void C_Camera::Move_Left_Start() {
    bMovingLeft=true;
}
void C_Camera::Move_Left_Stop() {
    bMovingLeft=false;
}
void C_Camera::Move_Left() {
    if(!bMovingLeft) return;
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= float(cos(yrotrad)) * cScale;
    zpos -= float(sin(yrotrad)) * cScale;
}
void C_Camera::Move_Right_Start() {
    bMovingRight=true;
}
void C_Camera::Move_Right_Stop() {
    bMovingRight=false;
}
void C_Camera::Move_Right() {
    if(!bMovingRight) return;
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += float(cos(yrotrad)) * cScale;
    zpos += float(sin(yrotrad)) * cScale;

}
void C_Camera::Move_Forward_Start() {
    bMovingForward=true;
}
void C_Camera::Move_Forward_Stop() {
    bMovingForward=false;
}
void C_Camera::Move_Forward() {
    if(!bMovingForward) return;
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos += float(sin(yrotrad)) * cScale;
    zpos -= float(cos(yrotrad)) * cScale;
    ypos -= float(sin(xrotrad)) ;
    bounce += 0.04;
}
void C_Camera::Move_Backward_Start() {
    bMovingBackward=true;
}
void C_Camera::Move_Backward_Stop() {
    bMovingBackward=false;
}
void C_Camera::Move_Backward() {
    if(!bMovingBackward) return;
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos -= float(sin(yrotrad)) * cScale;
    zpos += float(cos(yrotrad)) * cScale;
    ypos += float(sin(xrotrad));
    bounce += 0.04;
}
void C_Camera::mouseMovement(int x, int y) {
    int diffx=x-lastx;
    int diffy=y-lasty;
    //diffx=0;
    //if(x>lastx) diffx=1;
    //if(x<lastx) diffx=-1;
    //diffy=0;
    //if(y>lasty) diffy=1;
    //if(y<lasty) diffy=-1;
    lastx=x;
    lasty=y;
    xrot += (float) diffy;
    yrot += (float) diffx;
    if(xrot < -60.0f) xrot=-60.0f;
    if(xrot > 60.0f) xrot=60.0f;
    if(yrot < -5000.0f) yrot=0.0f;
}
// C_GFX::C_GFX() { pCamera = 0; VideoFlags=0; }
C_GFX::C_GFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG, CGAF *pUSEGAF) {
    pDefaultTexture=0;
    pFirstTexture=0;
    pFirstModel=0;
    pMap=0;
    pFirstMapModel=0;

    pCamera=0;
    InitializeGFX(w,h,c,FullScreen,wincaption,pUSELOG,pUSEGAF);
}
C_GFX::~C_GFX() {
    ShutDownGFX();
    SDL_Quit();
}
bool C_GFX::InitializeGFX(int w, int h, int c, bool FullScreen, char *wincaption,CLog *pUSELOG,CGAF *pUSEGAF) {
#ifdef __linux__
    putenv("SDL_VIDEODRIVER=dga");
#endif
    pLog=pUSELOG;
    pGAF=pUSEGAF;
    bFullScreen =FullScreen;
    ScreenWidth =w;
    ScreenHeight=h;
    ScreenColors=c;
    pLog->_Add("Init SDL/OpenGL GFX Subsystem...");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    VideoFlags = SDL_OPENGL|SDL_HWPALETTE|SDL_DOUBLEBUF;
    if(bFullScreen) VideoFlags |= SDL_FULLSCREEN;
    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware
    if(VideoInfo) {
    } else {
        pLog->_Add("Failed getting Video Info : %s",SDL_GetError());
        return false;
    }
    if(VideoInfo->hw_available) {
        VideoFlags |= SDL_HWSURFACE;
        pLog->_Add("Hardware surfaces...");
    } else {
        VideoFlags |= SDL_SWSURFACE;
        pLog->_Add("Software surfaces...");
    }
    if(VideoInfo->blit_hw) {
        VideoFlags |= SDL_HWACCEL;
        pLog->_Add("Hardware acceleration enabled!");
    }
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,  16 );
    if(SDL_VideoModeOK(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags)) {
    } else {
        pLog->_Add("SDL_VideoModeOK failure");
        return false;
    }
    pScreen = SDL_SetVideoMode(w,h,c,VideoFlags);
    if(pScreen) {
    } else {
        pLog->_Add("Can't set up pScreen! ErroR!");
        return false;
    }
    SDL_ShowCursor(SDL_DISABLE);
    SetWindowTitle(wincaption);
    pLog->_Add("SDL initialized (Video memory:[%d])",VideoInfo->video_mem);
    if(InitGL()) {
        pLog->_Add("OpenGL initialized");

    } else {
        pLog->_Add("Can't initialize OpenGL");
        return false;
    }
#ifdef _WIN32
    /*  glGenBuffersARB     = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
    	glBindBufferARB     = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
    	glBufferDataARB     = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
    	glDeleteBuffersARB  = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB"); */
#endif
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
    if(LoadBaseGFX(pGAF)) {
        pLog->_Add("Base Textures initialized");
    } else {
        pLog->_Add("Can't initialize Base Textures");
        return false;
    }

    pDefaultTexture=GetTexture("base/default.png");
    if(!pDefaultTexture) {
        pDefaultTexture=new CGLTexture;
        if(pDefaultTexture) {
            pDefaultTexture->LoadPNG("base/default.png");
            pLog->_Add("Default Texture initialized");
        }
    }
    if(!pDefaultTexture) {
        pLog->_Add("Can't initialize Default Texture");
        return false;
    }

    pMap = new CMesh(pLog,pGAF);
    // pMap = new CMesh(pLog, pGAF, pDefaultTexture);
    if(pMap) {
        pLog->_Add("Map mesh initialized");
    } else {
        pLog->_Add("Can't initialize Map mesh");
        return false;
    }

    pMap->pOffset.x=5.0f;
    pMap->pOffset.z=15.0f;
    pMap->pTexture=pDefaultTexture;//GetTexture("base/b0113.png");

    if(!LoadModels()) return false;

    pLog->_Add("GFX Initialized");
    return true;
}
void C_GFX::SetWindowTitle(char *fmt, ...) {
    char ach[512];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    strcpy(WindowCaption, ach);
    SDL_WM_SetCaption(WindowCaption,0);
}
void C_GFX::ToggleFullScreen(void) {
    // SDL_WM_ToggleFullScreen(pScreen);
    /*  const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();
        if(VideoInfo->hw_available) { VideoFlags |= SDL_HWSURFACE; pLog->_Add("Hardware surfaces..."); }
        else                        { VideoFlags |= SDL_SWSURFACE; pLog->_Add("Software surfaces..."); }
    	if(bFullScreen==true) bFullScreen=false;
    	else bFullScreen=true;
    	pScreen = SDL_SetVideoMode(ScreenWidth,ScreenHeight,ScreenColors,VideoFlags^SDL_FULLSCREEN);
    	InitGL();	*/
}
void C_GFX::ShutDownGFX(void) {
    pLog->_DebugAdd("Shutting down SDL/OpenGL GFX subsystem...");
    DEL(pCamera);
    DEL(pMap);

    DestroyBaseGFX();
    DEL(pDefaultTexture);
    DestroyModels();
    glFinish();
    glFlush();
    SDL_FreeSurface(pScreen);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    pLog->_Add("SDL/OpenGL GFX subsystem shutdown...");
}
int C_GFX::InitGL() { // All Setup For OpenGL Goes Here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
    gluPerspective(45.0f,(GLfloat)SDL_GetVideoSurface()->w/(GLfloat)SDL_GetVideoSurface()->h,1.0f,5000.0f); // gluPerspective(45.0f,1.333f,0.1f,20000.0f);
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
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
void C_GFX::StarField(int iDir) {
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
        DrawBaseGFX( (int)(star[y].x),
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
void C_GFX::SetScreenRes(int x,int y,int cl, bool fs) {
    //ShutDownGFX();
    //InitializeGFX(x,y,cl,fs,WindowCaption,pLog,pGAF);
}
bool C_GFX::LoadBaseGFX(CGAF *pGAF) { // Load in GFX Base
    CGLTexture *pTexture;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir("base");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if( (dlcs_strcasecmp(epdf->d_name,".")) ||
                    (dlcs_strcasecmp(epdf->d_name,"..")) ) {
            } else {
                if(sp_isdir(epdf->d_name)) {

                } else {
                    if (strcmp (".png", epdf->d_name + strlen (epdf->d_name) - 4) == 0) {
                        pLog->AddEntry("Found base texture: base/%s\n",epdf->d_name);
                        pTexture=pFirstTexture;
                        if(pTexture) {
                            while(pTexture->pNext) {
                                pTexture=pTexture->pNext;
                            }
                            pTexture->pNext=new CGLTexture;
                            pTexture=pTexture->pNext;
                        }
                        else {
                            pFirstTexture=new CGLTexture;
                            pTexture=pFirstTexture;
                        }
                        pTexture->LoadPNG(va("base/%s",epdf->d_name));
                        if(!pTexture->bmap) {
                            pLog->AddEntry("ERROR LOADING base/%s (CGLTEXTURE OBJECT DESTROYED)\n",epdf->d_name);
                            DEL(pTexture);
                        }
                        else {
                            pLog->AddEntry("LOAD %s SUCCESS (OPENGL[%d]) \n",pTexture->tfilename,pTexture->bmap);
                        }
                    }
                }
            }
        }
    }
    closedir(dpdf);
    return true;
}
CGLTexture* C_GFX::GetTexture(char * name) {
    CGLTexture* pTexture;
    pTexture=pFirstTexture;
    while(pTexture) {
        if(dlcs_strcasecmp(pTexture->tfilename,name)) return pTexture;
        pTexture=pTexture->pNext;
    }
    // TODO: Attempt to load texture
    return 0;
}
int C_GFX::GetTotalTextures(void) {
    int n=0;
    CGLTexture* pTexture;
    pTexture=pFirstTexture;
    while(pTexture) {
        n++;
        pTexture=pTexture->pNext;
    }
    return n;
}
CGLTexture* C_GFX::GetRandomTexture(void) {
    int n=GetTotalTextures();
    int x=0;
    int r=(rand()%n)+1;
    CGLTexture* pTexture;
    pTexture=pFirstTexture;
    while(pTexture) {
        x++;
        if(x>n) return pTexture;
        if(x==r) return pTexture;
        pTexture=pTexture->pNext;
    }
    return 0;
}
bool C_GFX::DestroyBaseGFX(void) {
    CGLTexture * pTexture;
    pTexture=pFirstTexture;
    while(pTexture) {
        pFirstTexture=pTexture;
        pTexture=pTexture->pNext;
        DEL(pFirstTexture);
    }
}
bool C_GFX::LoadModels(void) {
    pLog->AddEntry("Loading models...\n");

    char szModelFilename[1024];
    memset(szModelFilename,0,1024);
    CGLModel *pModel;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir("models");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if( (dlcs_strcasecmp(epdf->d_name,".")) ||
                    (dlcs_strcasecmp(epdf->d_name,"..")) ) {
            } else {

                if(sp_isdir(va("models/%s",epdf->d_name))) {

                    strcpy(szModelFilename,va("models/%s",epdf->d_name));

                    pLog->AddEntry("Found model: %s\n",szModelFilename);

/*
                    pModel=pFirstModel;
                    if(pModel) {
                        while(pModel->pNext) {
                            pModel=pModel->pNext;
                        }
                        pModel->pNext=new CGLModel;
                        pModel=pModel->pNext;
                    }
                    else {
                        pFirstModel=new CGLModel;
                        pModel=pFirstModel;
                    }
                    pModel->Load(szModelFilename);
                    */
                }
            }
        }
    }

    closedir(dpdf);

    memset(szModelFilename,0,1024);
    C_MapModel *pMapModel;

    dpdf = opendir("map");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if( (dlcs_strcasecmp(epdf->d_name,".")) ||
                    (dlcs_strcasecmp(epdf->d_name,"..")) ) {
            } else {

                if (strcmp (".fmo", epdf->d_name + strlen (epdf->d_name) - 4) == 0) {
                    strcpy(szModelFilename,va("map/%s",epdf->d_name));
                    pLog->AddEntry("Found map model: %s\n",szModelFilename);

                    pMapModel=pFirstMapModel;
                    if(pMapModel) {
                        while(pMapModel->pNext) pMapModel=pMapModel->pNext;
                        pMapModel->pNext=new C_MapModel();
                        pMapModel=pMapModel->pNext;
                    }
                    else {
                        pFirstMapModel=new C_MapModel();
                        pMapModel=pFirstMapModel;
                    }
                    if(pMapModel) {
                        pMapModel->Load(szModelFilename);
                    }
                }
            }
        }
    }
    closedir(dpdf);
    return true;
}
CGLModel *C_GFX::GetModel(char *name) {
    CGLModel* pModel=pFirstModel;
    while(pModel) {
        if(dlcs_strcasecmp(pModel->name,name))
            return pModel;
        pModel=pModel->pNext;
    }
    return false;
}
bool C_GFX::DestroyModels(void) {
    pLog->_DebugAdd("Begin Models destroy...");
    CGLModel *pModel;
    pModel=pFirstModel;
    while(pModel) {
        pFirstModel=pModel;
        pModel=pModel->pNext;
        DEL(pFirstModel);
    }

    C_MapModel* pMapModel;
    pMapModel=pFirstMapModel;
    while(pMapModel) {
        pFirstMapModel=pMapModel;
        pMapModel=pMapModel->pNext;
        DEL(pFirstMapModel);
    }
    pLog->_DebugAdd("Models destroyed...");
    return true;
}
void C_GFX::RenderScene(void) { // Render the game scene Frame
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();											// Reset The Modelview Matrix
/*  glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glDisable (GL_BLEND);
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    */

/*  GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat density = 0.02; //set the density to 0.3 which isacctually quite thick
    glEnable (GL_FOG);              //enable this for fog
    glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
    glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to our color chosen above
    glFogf (GL_FOG_DENSITY, density); //set the density to the value above
    glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the nicest, may slow down on older cards
    //glFogf (GL_FOG_START, 10.0f);
    //glFogf (GL_FOG_END, 680.0f);   // Enable Pointers
    //glFogi (GL_FOG_MODE, GL_LINEAR); */



    StarField(1);
    // DrawSkyBox();

    if(pCamera) pCamera->Go();

    pMap->Draw();
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
    pCamera->Go();
    glTranslatef(0.0f,ypos,zpos);
    CGLTexture *pTexture=0;
    pTexture=GetTexture("base/sun.png");
    if(pTexture) glBindTexture(GL_TEXTURE_2D, pTexture->bmap);
    glDisable(GL_BLEND);
    DrawSphere(3,160.0f,1.0f,1.0f,1.0f);
}
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
void C_GFX::DrawBaseGFX(int x,int y,int x2,int y2,char * name,u_char r,u_char g,u_char b) {
    CGLTexture *pTexture;
    pTexture=0;
    pTexture=GetTexture(name);
    if(pTexture)
        pTexture->Draw2d(x,y,x2,y2,r,g,b);
}
/****************************************************************************************************
void C_GFX::DrawBit4ge(int x,int y,int x2,int y2,bool bsin) {
    if(!cgltBit4ge) {
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
    if(bsin) {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y-y3+ 5*sin(fB4/12)),    1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3+ 5*sin(fB4/15)),  float(y-y3+ 5*sin(fB4/13)),    1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3+ 5*sin(fB4/14)),  float(y+ 5*sin(fB4/25)),    1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x+ 5*sin(fB4/22)),   float(y+ 5*sin(fB4/12)),    1.0f);
    }
    else {
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
} */
/****************************************************************************************************
void C_GFX::DrawModels(void) {
    static float x,y,z;
    x=320.4f;
    y=90.5f;
    Model=FirstModel;
    if(Model) {
        Model->Rotate(x,y,0);
        Model->Scale(1.7f,1.7f,1.7f);
        Model->Locate(-160,-90,-10);
        glDisable(GL_BLEND);
        Model->Draw();
        Model=Model->next;
        if(Model) {
            Model->Rotate(x,y,0);
            Model->Scale(1.7f,1.7f,1.7f);
            Model->Locate(160,-90,-10);
            glDisable(GL_BLEND);
            Model->Draw();
            Model=Model->next;

            if(Model) {
                glEnable(GL_BLEND);
                Model->Rotate(x,y,0);
                Model->Scale(1.7f,1.7f,1.7f);
                Model->Draw();
            }
        }
    }
} */
void C_GFX::UpdatePickRay(GLfloat x,GLfloat y) {
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
void C_GFX::draw_3d_box(int x, int y, int x2, int y2) {
    RECT r;
    r.left=x;
    r.top=y;
    r.right=(x2-x);
    r.bottom=(y2-y);
    draw_3d_box(r);
}
void C_GFX::draw_3d_box(RECT rect) {
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
void C_GFX::DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r,float cr,float cg,float cb) {
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
}
void C_GFX::DrawSphere(int ndiv, float radius, float cr,float cg,float cb) {
    glBegin(GL_TRIANGLES);
    for (int i=0; i<20; i++)
        DrawTri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius,cr,cg,cb);
    glEnd();
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

  // Store the current matrix
     glPushMatrix();
    // Reset and transform the matrix.
    //glLoadIdentity();
  /*gluLookAt(
        0,0,0,
        pCamera->xpos,
        pCamera->zpos,
        pCamera->ypos,


        // camera->x(),camera->y(),camera->z(),
        0,1,0);
        */

    // Enable/Disable features
    //glPushAttrib(GL_ENABLE_BIT);
    //glEnable(GL_TEXTURE_2D);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);

    // Render the front quad

    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.front.png")->bmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  1.0f, -1.0f, -1.0f );
        glTexCoord2f(1, 0); glVertex3f( -1.0f, -1.0f, -1.0f );
        glTexCoord2f(1, 1); glVertex3f( -1.0f,  1.0f, -1.0f );
        glTexCoord2f(0, 1); glVertex3f(  1.0f,  1.0f, -1.0f );
    glEnd();

    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.left.png")->bmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  1.0f, -1.0f,  1.0f );
        glTexCoord2f(1, 0); glVertex3f(  1.0f, -1.0f, -1.0f );
        glTexCoord2f(1, 1); glVertex3f(  1.0f,  1.0f, -1.0f );
        glTexCoord2f(0, 1); glVertex3f(  1.0f,  1.0f,  1.0f );
    glEnd();

    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.back.png")->bmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -1.0f, -1.0f,  1.0f );
        glTexCoord2f(1, 0); glVertex3f(  1.0f, -1.0f,  1.0f );
        glTexCoord2f(1, 1); glVertex3f(  1.0f,  1.0f,  1.0f );
        glTexCoord2f(0, 1); glVertex3f( -1.0f,  1.0f,  1.0f );

    glEnd();

    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.right.png")->bmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -1.0f, -1.0f, -1.0f );
        glTexCoord2f(1, 0); glVertex3f( -1.0f, -1.0f,  1.0f );
        glTexCoord2f(1, 1); glVertex3f( -1.0f,  1.0f,  1.0f );
        glTexCoord2f(0, 1); glVertex3f( -1.0f,  1.0f, -1.0f );
    glEnd();

    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.top.png")->bmap);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -1.0f,  1.0f, -1.0f );
        glTexCoord2f(0, 0); glVertex3f( -1.0f,  1.0f,  1.0f );
        glTexCoord2f(1, 0); glVertex3f(  1.0f,  1.0f,  1.0f );
        glTexCoord2f(1, 1); glVertex3f(  1.0f,  1.0f, -1.0f );
    glEnd();

    // Render the bottom quad
    if(GetTexture("base/sb1.bottom.png")) {
        glBindTexture(GL_TEXTURE_2D, GetTexture("base/sb1.bottom.png")->bmap);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f( -1.0f, -1.0f, -1.0f );
            glTexCoord2f(0, 1); glVertex3f( -1.0f, -1.0f,  1.0f );
            glTexCoord2f(1, 1); glVertex3f(  1.0f, -1.0f,  1.0f );
            glTexCoord2f(1, 0); glVertex3f(  1.0f, -1.0f, -1.0f );
        glEnd();
    }

    // Restore enable bits and matrix
    //glPopAttrib();
    glPopMatrix();
}
