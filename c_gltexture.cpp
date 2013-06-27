
#include "c_gltexture.h"
#include "glerrors.h"
#include "SDL.h"

CGLTexture::CGLTexture()
{
    next=0;
    bmap=0;
    mask=0;
    usemask=0;
    memset(tfilename,0,1024);
    memset(name,0,1024);
}

CGLTexture::CGLTexture(CGAF *pGAF,char *fname)
{
    next=0;
    bmap=0;
    mask=0;
    usemask=0;
    memset(tfilename,0,1024);
    memset(name,0,1024);
    Load(pGAF,fname,0);

}
CGLTexture::CGLTexture(CGAF *pGAF,char *fname, bool fmask)
{
    next=0;
    bmap=0;
    mask=0;
    usemask=fmask;
    memset(tfilename,0,1024);
    memset(name,0,1024);
    Load(pGAF,fname,0);
}
/****************************************************************************************************/
CGLTexture::~CGLTexture()
{
    glDEL(bmap);
    glDEL(mask);
}
bool CGLTexture::Create(int x, int y)
{
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
    //D//Log("CGLTexture::Create() -> created a texture!");
    return 1;
}
bool CGLTexture::Clear(u_char R,u_char G,u_char B)
{
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

bool CGLTexture::ClearMask(u_char R,u_char G,u_char B)
{
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

bool CGLTexture::Transparent(bool trans)
{
    if(trans)
    {
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
        //D//Log("CGLTexture::Transparent() set texture to use transparency...");
    }
    else
    {
        glDEL(mask);
        usemask=0;
        //D//Log("CGLTexture::Transparent() set texture to not use transparency...");
    }

    return 1;
}

/****************************************************************************************************/

bool   CGLTexture::Loaded(void)
{
    if(bmap)
    {
        if(usemask) if(!mask) return 0;
        return 1;
    }
    return 0;
}

/****************************************************************************************************/

#pragma comment(lib,"cximage.lib")
#pragma comment(lib,"Tiff.lib" )
#pragma comment(lib,"jasper.lib" )
#pragma comment(lib,"libdcr.lib" )
#pragma comment(lib,"Jpeg.lib" )
#pragma comment(lib,"mng.lib" )
#pragma comment(lib,"png.lib" )
#pragma comment(lib,"zlib.lib" )

#include "ximage.h"

extern GAF_SCANCALLBACK what(GAFFile_ElmHeader *ElmInfo,LPSTR FullPat);

/****************************************************************************************************/
GLuint CGLTexture::Load(CGAF *pGAF, char *filename,bool which){
    glDEL(bmap);
    glDEL(mask);

	if(!strlen(filename)) return 0;
	if(!pGAF) return 0;
    char filename2[1024]; memset(filename2,0,1024);
    char maskfile[1024];  memset(maskfile,0,1024);
    char ax,ay,az;
	//long size1;
	//long size2;
	//unsigned char *fb1;
	//unsigned char *fb2;
	CxImage *himage=0;
	CxImage *himage2=0;

	GAF_FileBuffer nfbuf1;
	GAF_FileBuffer nfbuf2;

    ax=filename[strlen(filename)-3];
    ay=filename[strlen(filename)-2];
    az=filename[strlen(filename)-1];
    strcpy(filename2,filename);
    filename2[strlen(filename2)-4]=0;
    sprintf(maskfile,"%smask.%c%c%c",filename2,ax,ay,az);

	nfbuf1  = pGAF->GetFile((LPSTR)filename);

	//size1	= nfbuf1.Size;
	//fb1		= nfbuf1.fb;
	////Log("%s %s",filename,maskfile);
    //if(!fb1) return 0;

	himage=new CxImage((BYTE*)nfbuf1.fb,nfbuf1.Size,CXIMAGE_FORMAT_BMP);

	if(himage) {

		himage->IncreaseBpp(24);
		himage->SwapRGB2BGR();
		himage->SetXDPI(72);
		himage->SetYDPI(72);

	    glGenTextures(1, &bmap);
	    glBindTexture(GL_TEXTURE_2D, bmap);

		glTexImage2D    (	GL_TEXTURE_2D,
						0,
						3,
						himage->GetWidth(),
						himage->GetHeight(),
						0,
						GL_RGB,
						GL_UNSIGNED_BYTE,
						himage->GetBits());

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFlush();
		himage->Destroy();
		DEL(himage);
	}

	nfbuf2  = pGAF->GetFile(maskfile);
	//size2   = nfbuf2.Size;
	//fb2	    = nfbuf2.fb;

//	if(fb2)
	//{
		himage2=new CxImage((BYTE*)nfbuf2.fb,nfbuf2.Size,CXIMAGE_FORMAT_BMP);

		if(himage2)
		{
			himage2->IncreaseBpp(24);
			himage2->SwapRGB2BGR();
			himage2->SetXDPI(72);
			himage2->SetYDPI(72);

			glGenTextures   (1, &mask);
			glBindTexture   (GL_TEXTURE_2D, mask);

			glTexImage2D    (	GL_TEXTURE_2D,
								0,
								3,
								himage2->GetWidth(),
								himage2->GetHeight(),
								0,
								GL_RGB,
								GL_UNSIGNED_BYTE,
								himage2->GetBits());


			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Log("                    \\--> Mask found: %s mask(%d)",maskfile,mask);
			usemask=1;
			himage2->Destroy();
			DEL(himage2);
		}

		//free(fb2);
	//}

	//free(fb1);

    return 1;

}
/****************************************************************************************************/
bool CGLTexture::ReLoad(void)
{
    if(!strlen(tfilename))
    {
        //D//Log("CGLTexture::ReLoad() (could not produce tfilename)");
        return 0;
    }
    return (Load(pWhichGAF,tfilename,0)?0:1);
}

bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b)
{
    return Draw(x,y,x2,y2,r,g,b,255,255,255);
}

bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2)//Draw(int x,int y,int x2,int y2,long color)
{
    if(!bmap) { Load(pWhichGAF,tfilename,1); return 0; }
    if(usemask) if(!mask) { Load(pWhichGAF,tfilename,1); return 0; }

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
