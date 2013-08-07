/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gltexture.cpp
 **   Class:        CGLTexture
 **   Description:  SDL/OpenGL Texture manager class
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_gltexture.h"
extern GAF_SCANCALLBACK what(GAFFile_ElmHeader *ElmInfo,LPSTR FullPat);
/////////////////////////////// CGLTexture class
CGLTexture::CGLTexture() {
    Initialize();
    pLog=new CLog("texture.log");
    bMadeLog=true;
}
CGLTexture::CGLTexture(const char* file) {
    Initialize();
    pLog=new CLog("texture.log");
    bMadeLog=true;
    Load(file);
}
CGLTexture::CGLTexture(CLog *pInLog) {
    Initialize();
    bMadeLog=false;
    pLog=pInLog;
}
CGLTexture::CGLTexture(CLog *pInLog, const char *file) {
    Initialize();
    bMadeLog=false;
    pLog=pInLog;
    Load(file);
}
CGLTexture::~CGLTexture() {
    if(bMadeLog)
        dlcsm_delete(pLog);
    dlcsm_gl_delete(bmap);
}
void CGLTexture::Initialize() {
    bMadeLog=true;
    pLog=0;
    pNext=0;
    bmap=0;
    dlcsm_zero(filename);
    dlcsm_zero(name);
}
GLuint CGLTexture::Create(int x, int y) {
    width=x;
    height=y;
    dlcsm_gl_delete(bmap);
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
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(0),      float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(width),  float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(width),  float(height), 1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(0),      float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}
GLuint CGLTexture::Load(const char *file) {
    strcpy(filename,file);
    string ext=dlcs_filetype(file);
    if(ext=="png") return LoadPNG(file);
    if(ext=="bmp") return LoadBMP(file);
    if(ext=="tga") return LoadTGA(file);
    if((ext=="jpg")||
       (ext=="jpeg")) return LoadJPG(file);
    if(ext=="tif") return LoadTIF(file);
    if(ext=="dds") return LoadDDS(file);
    if(ext=="gif") return LoadGIF(file);

    ext.clear();
    return false;
}
// TODO:
GLuint CGLTexture::LoadFromMem(const char *file){
    return 0;
}
// TODO:
GLuint CGLTexture::LoadBMPFromMem(unsigned char* fb, Image *image){
    return 0;
}
// TODO:
GLuint CGLTexture::LoadTextureFromMem(unsigned char *fb){
    /*  pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> Start");
    	if(!strlen(filename)) return 0;
    	if(!pGAF) return 0;
        char filename2[1024]; memset(filename2,0,1024);
        char maskfile[1024];  memset(maskfile,0,1024);
        char ax,ay,az;
    	long size1;
    	long size2;
    	unsigned char *fb1;
    	unsigned char *fb2;
    	CxImage *himage;
    	CxImage *himage2;
    	GAF_FileBuffer nfbuf1;
    	GAF_FileBuffer nfbuf2;
        glDEL(bmap);
        glDEL(mask);
        ax=filename[strlen(filename)-3];
        ay=filename[strlen(filename)-2];
        az=filename[strlen(filename)-1];
        strcpy(filename2,filename);
        filename2[strlen(filename2)-4]=0;
        sprintf(maskfile,"%smask.%c%c%c",filename2,ax,ay,az);

    	if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> %s %s",filename,maskfile);

    	nfbuf1  = pGAF->GetFile((LPSTR)filename);

    	if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> 11111");

    	size1	= nfbuf1.Size;
    	fb1		= nfbuf1.fb;

    	if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> 22222");

    	if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> nbuf size = %d",size1);

        if(!size1) return 0;

    	if(!fb1) return 0;

    	himage=new CxImage((BYTE*)fb1,size1,CXIMAGE_FORMAT_BMP);

    	pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> ZZZZZ");

    	if(himage)
    	{

    		himage->IncreaseBpp(24);
    		himage->SwapRGB2BGR();
    		himage->SetXDPI(72);
    		himage->SetYDPI(72);

    	    glGenTextures(1, &bmap);
    	    glBindTexture(GL_TEXTURE_2D, bmap);


    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> %s %d %d size(%d) bmap(%d)",filename,himage->GetWidth(),himage->GetHeight(),himage->GetSize(),bmap);

    		glTexImage2D    ( GL_TEXTURE_2D,
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

    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> AAAAA");

    		glFlush();

    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> BBBBB");

    		himage->Destroy();

    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> CCCCC");

    		DEL(himage);

    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> DDDDD");

    	}

    	pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> YYYYY");

    	nfbuf2  = pGAF->GetFile(maskfile);
    	size2   = nfbuf2.Size;
    	fb2	    = nfbuf2.fb;

    	pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> XXXXX");

    	if(fb2)
    	{
    		himage2=new CxImage((BYTE*)fb2,size2,CXIMAGE_FORMAT_BMP);

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

    			if(pLog) pLog->_DebugAdd("                    \\--> Mask found: %s mask(%d)",maskfile,mask);
    			usemask=1;
    			himage2->Destroy();
    			DEL(himage2);
    		}

    		free(fb2);
    	}

    	free(fb1);

    	pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> End");

        return 1;


    pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> Start");
    if(!strlen(filename))   return 0;
    if(!pGAF)               return 0;

    dlcsm_make_filename(filename2);
    char ax,ay,az;
    int x, y;
    GAF_FileBuffer nfbuf1;
    dlcsm_zero(nfbuf1.fb);
    dlcsm_zero(nfbuf1.Size);
    dlcsm_gl_delete(bmap);
    ax=filename[strlen(filename)-3];
    ay=filename[strlen(filename)-2];
    az=filename[strlen(filename)-1];
    strcpy(filename2,filename);
    filename2[strlen(filename2)-4]=0;
    // sprintf(maskfile,"%smask.%c%c%c",filename2,ax,ay,az);
    nfbuf1=pGAF->GetFile((LPSTR)filename);
    if(nfbuf1.fb) {
        x=nfbuf1.fb[18]+nfbuf1.fb[19]*256+nfbuf1.fb[20]*512+nfbuf1.fb[21]*1024;
        y=nfbuf1.fb[22]+nfbuf1.fb[23]*256+nfbuf1.fb[24]*512+nfbuf1.fb[25]*1024;
        pLog->_DebugAdd("GAF INFO: nfbuf1.Size= %d nfbuf1.fb  = %d(address)\n",nfbuf1.Size,nfbuf1.fb);
        pLog->_DebugAdd("IMAGE   : Filename   = %s     OPENGL[%d] WIDTH: %d  HEIGHT: %d \n",filename,bmap,x,y);
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
        if(pLog) pLog->_DebugAdd("      \\--> Mask found: %s OPENGL[%d] WIDTH:%d HEIGHT:%d",maskfile,mask,x,y);
        usemask=1;
    }
    */

    return 0;
}
GLuint CGLTexture::LoadPNG(const char *file) {
    strcpy(filename,file);
    int width;
    int height;
    png_byte header[8];
    FILE *fp = fopen(filename, "rb");
    if (fp == 0) {
        perror(filename);
        return 0;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "error: %s is not a PNG.\n", filename);
        fclose(fp);
        return 0;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,NULL, NULL, NULL);
    width = temp_width;
    height = temp_height;
    png_read_update_info(png_ptr, info_ptr);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    rowbytes += 3 - ((rowbytes-1) % 4);
    png_byte * image_data;
    image_data = malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    png_bytep * row_pointers = malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }
    int i;
    for (i = 0; i < temp_height; i++) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }
    png_read_image(png_ptr, row_pointers);
    glGenTextures(1, &bmap);
    glBindTexture(GL_TEXTURE_2D, bmap);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return bmap;
}
GLuint CGLTexture::LoadBMP(const char *file) {
    strcpy(filename,file);
    bmap=0;
    SDL_Surface *loadSurface;
    loadSurface = SDL_LoadBMP(filename);
    if(loadSurface) {
        glGenTextures( 1, &bmap);
        glBindTexture( GL_TEXTURE_2D, bmap );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        width  = loadSurface->w;
        height = loadSurface->h;
        glTexImage2D( GL_TEXTURE_2D, 0, 4, loadSurface->w,loadSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadSurface->pixels );
        SDL_FreeSurface( loadSurface );
    }
    return bmap;
}
// TODO:
GLuint CGLTexture::LoadTGA(const char* file){
    return 0;
}
// TODO:
GLuint CGLTexture::LoadJPG(const char* file){
    return 0;
}
// TODO:
GLuint CGLTexture::LoadTIF(const char* file) {
}
// TODO:
GLuint CGLTexture::LoadDDS(const char* file) {
}
// TODO:
GLuint CGLTexture::LoadGIF(const char* file) {
}
bool CGLTexture::Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b) { return Draw2d(x,y,x2,y2,r,g,b,255,255,255);}
bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b) {   return Draw(x,y,x2,y2,r,g,b,255,255,255); }
bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2) {
    if(!bmap) { Load(filename); return false; }
    int x3=(x2-x);
    int y3=(y2-y);
    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);

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
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(x),      float(y-y3),    1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(x+x3),   float(y-y3),    1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(x+x3),   float(y),       1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(x),      float(y),       1);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}
bool CGLTexture::Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2) {
    if(!bmap) { Load(filename); return 0; }
    int x3=(x2-x);
    int y3=(y2-y);
    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glTexCoord2d(0, 0);
    glVertex3d(x,y-y3,1);
    glTexCoord2d(1, 0);
    glVertex3d(x+x3,y-y3,1);
    glTexCoord2d(1, 1);
    glVertex3d(x+x3,y,1);
    glTexCoord2d(0, 1);
    glVertex3d(x,y,1);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}
bool CGLTexture::DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2) {
    int x3=(x2-x);
    int y3=(y2-y);
    x=x/2;
    y=(-y/2)+(SDL_GetVideoSurface()->h/2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(float(x),      float(y-y3),    1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(float(x+x3),   float(y-y3),    1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(float(x+x3),   float(y),       1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(float(x),      float(y),       1);
    glEnd();
    return 1;
}
