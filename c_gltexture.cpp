
#include "c_gltexture.h"
#include "glerrors.h"
#include "SDL.h"
#include "ximage.h"

CGLTexture::CGLTexture() {   Initialize(); }

CGLTexture::CGLTexture(CLog *pInLog) {     Initialize();     pLog=pInLog; }

CGLTexture::CGLTexture(CGAF *pGAF,char *fname) {
    Initialize();
    LoadBMP(pGAF,fname,0);
}

CGLTexture::CGLTexture(CLog *pInLog, CGAF *pGAF,char *fname) {
    Initialize();
    pLog=pInLog;
    LoadBMP(pGAF,fname,0);
}

CGLTexture::CGLTexture(CGAF *pGAF,char *fname, bool fmask) {
    Initialize();
    usemask=fmask;
    LoadBMP(pGAF,fname,0);
}

CGLTexture::CGLTexture(CLog *pInLog, CGAF *pGAF,char *fname, bool fmask) {
    Initialize();
    pLog=pInLog;
    usemask=fmask;
    LoadBMP(pGAF,fname,0);
}

/****************************************************************************************************/
CGLTexture::~CGLTexture() {
    if(bMadeLog) DEL(pLog);
    glDEL(bmap);
    glDEL(mask);
}

void CGLTexture::Initialize() {
    pLog=0;
    bMadeLog=0;
    next=0;
    bmap=0;
    mask=0;
    usemask=0;
    memset(tfilename,0,1024);
    memset(name,0,1024);
}

bool CGLTexture::Create(int x, int y) {
    width=x;
    height=y;
    glDEL(bmap);
    bmap=0;
    unsigned int* data;
    data = (unsigned int*)new GLuint[((width * height)* 4 * sizeof(unsigned int))];
    memset(data,((width * height)* 4 * sizeof(unsigned int)),0);
    glGenTextures(1, &bmap);
    glBindTexture(GL_TEXTURE_2D, bmap);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    delete [] data;
    if(pLog) pLog->_DebugAdd("CGLTexture::Create() -> created a texture!");
    return 1;
}
bool CGLTexture::Clear(u_char R,u_char G,u_char B) {
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,bmap);
    glBegin(GL_QUADS);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(0),      float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(width),  float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(width),  float(height), 1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(0),      float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}

bool CGLTexture::ClearMask(u_char R,u_char G,u_char B) {
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,mask);
    glBegin(GL_QUADS);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(0),      float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(width),  float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(width),  float(height), 1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(0),      float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}

bool CGLTexture::Transparent(bool trans) {
    if(trans) {
        if((width==0)||(height==0)) {width=1024;height=1024;}
        mask=0;
        unsigned int* data;
        data = (unsigned int*)new GLuint[((width * height)* 4 * sizeof(unsigned int))];
        memset(data,((width * height)* 4 * sizeof(unsigned int)),0);
        glGenTextures(1, &mask);
        glBindTexture(GL_TEXTURE_2D, mask);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        delete [] data;
        usemask=1;
        if(pLog) pLog->_DebugAdd("CGLTexture::Transparent() set texture to use transparency...");
    }
    else {
        glDEL(mask);
        usemask=0;
        if(pLog) pLog->_DebugAdd("CGLTexture::Transparent() set texture to not use transparency...");
    }
    return 1;
}

/****************************************************************************************************/

bool   CGLTexture::Loaded(void) {
    if(bmap) {
        if(usemask) if(!mask) return 0;
        return 1;
    }
    return 0;
}

extern GAF_SCANCALLBACK what(GAFFile_ElmHeader *ElmInfo,LPSTR FullPat);

/****************************************************************************************************/
GLuint CGLTexture::LoadBMP(CGAF *pGAF,const char *filename,bool which) {
    pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> Start");
	if(!strlen(filename))   return 0;
	if(!pGAF)               return 0;
    char filename2[1024]; memset(filename2,0,1024);
    char maskfile[1024];  memset(maskfile,0,1024);
    char ax,ay,az;
    int x, y;
	GAF_FileBuffer nfbuf1;
	nfbuf1.fb=0;
	nfbuf1.Size=0;
	GAF_FileBuffer nfbuf2;
    nfbuf2.fb=0;
	nfbuf2.Size=0;
    glDEL(bmap);
    glDEL(mask);
    ax=filename[strlen(filename)-3];
    ay=filename[strlen(filename)-2];
    az=filename[strlen(filename)-1];
    strcpy(filename2,filename);
    filename2[strlen(filename2)-4]=0;
    sprintf(maskfile,"%smask.%c%c%c",filename2,ax,ay,az);
	nfbuf1=pGAF->GetFile((LPSTR)filename);
	if(nfbuf1.fb) {
        pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) nfbuf1.fb[%d] nfbuf1.Size[%d]",nfbuf1.fb,nfbuf1.Size);
        pLog->_DebugAdd("Filename   = %s\n",filename);
        pLog->_DebugAdd("Maskfile   = %s\n",maskfile);
        pLog->_DebugAdd("nfbuf1.fb  = %d\n",nfbuf1.fb);
        pLog->_DebugAdd("nfbuf1.Size= %d\n",nfbuf1.Size);
        x=nfbuf1.fb[18]+nfbuf1.fb[19]*256+nfbuf1.fb[20]*512+nfbuf1.fb[21]*1024;
        y=nfbuf1.fb[22]+nfbuf1.fb[23]*256+nfbuf1.fb[24]*512+nfbuf1.fb[25]*1024;
        pLog->_DebugAdd(" X: %d  Y: %d ",x,y);
        glGenTextures(1, &bmap);
        glBindTexture(GL_TEXTURE_2D, bmap);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, nfbuf1.fb+54);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFlush();
	}
	nfbuf2=pGAF->GetFile(maskfile);
    if(nfbuf2.fb) {
        x=nfbuf2.fb[18]+nfbuf2.fb[19]*256+nfbuf2.fb[20]*512+nfbuf2.fb[22]*1024;
        y=nfbuf2.fb[22]+nfbuf2.fb[23]*256+nfbuf2.fb[24]*512+nfbuf2.fb[25]*1024;
        glGenTextures(1, &mask);
        glBindTexture(GL_TEXTURE_2D, mask);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, nfbuf2.fb+54);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if(pLog) pLog->_DebugAdd("                    \\--> Mask found: %s mask(%d)",maskfile,mask);
        usemask=1;
    }
    return 1;
}
/****************************************************************************************************/
bool CGLTexture::ReLoad(void) {
    if(!strlen(tfilename)) {
        if(pLog) pLog->_DebugAdd("CGLTexture::ReLoad() (could not produce tfilename)");
        return 0;
    }
    return (LoadBMP(pGAF,tfilename,0)?0:1);
}

bool CGLTexture::Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b){
    return Draw2d(x,y,x2,y2,r,g,b,255,255,255);
}

bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b){
    return Draw(x,y,x2,y2,r,g,b,255,255,255);
}

bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2){
    if(!bmap) { LoadBMP(pGAF,tfilename,1); return 0; }
    if(usemask) if(!mask) { LoadBMP(pGAF,tfilename,1); return 0; }
    int x3=(x2-x);
    int y3=(y2-y);
    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);
    if(usemask) {
        ////////////////////////////////////////////////////////
        // draw mask

        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR,GL_ZERO);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();
        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,mask);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r2,g2,b2);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();

        ////////////////////////////////////////////////////////
        // draw bmap

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r,g,b);


        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();

        ////////////////////////////////////////////////////////
        // pop matrices

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);


    }
    else
    {
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glColor3ub(r,g,b);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);
    }

    return 1;
}


bool CGLTexture::Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2)//Draw(int x,int y,int x2,int y2,long color)
{
    if(!bmap) { LoadBMP(pGAF,tfilename,1); return 0; }
    if(usemask) if(!mask) { LoadBMP(pGAF,tfilename,1); return 0; }

    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);

    if(usemask)
    {
        ////////////////////////////////////////////////////////
        // draw mask

        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR,GL_ZERO);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();
        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,mask);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r2,g2,b2);

        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex3d(x,y-y3,1);
        glTexCoord2d(1, 0); glVertex3d(x+x3,y-y3,1);
        glTexCoord2d(1, 1); glVertex3d(x+x3,y,1);
        glTexCoord2d(0, 1); glVertex3d(x,y,1);
        glEnd();

        ////////////////////////////////////////////////////////
        // draw bmap

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r,g,b);


        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex3d(x,y-y3,1);
        glTexCoord2d(1, 0); glVertex3d(x+x3,y-y3,1);
        glTexCoord2d(1, 1); glVertex3d(x+x3,y,1);
        glTexCoord2d(0, 1); glVertex3d(x,y,1);
        glEnd();

        ////////////////////////////////////////////////////////
        // pop matrices

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);


    }
    else
    {
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        gluOrtho2D(0,SDL_GetVideoSurface()->w,0,SDL_GetVideoSurface()->h);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glColor3ub(r,g,b);

        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex3d(x,y-y3,1);
        glTexCoord2d(1, 0); glVertex3d(x+x3,y-y3,1);
        glTexCoord2d(1, 1); glVertex3d(x+x3,y,1);
        glTexCoord2d(0, 1); glVertex3d(x,y,1);
        glEnd();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);
    }

    return 1;
}

bool CGLTexture::DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2)
{
    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
    glEnd();

    return 1;

}
