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
    dlcsm_gl_delete(glBmap);

}
void CGLTexture::Initialize() {
    bMadeLog=true;
    pLog=0;
    pNext=0;
    glBmap=0;
    memset(filename,0,FILENAME_SIZE);
    memset(name,0,FILENAME_SIZE);
}
GLuint CGLTexture::Create(int x, int y) {
    width=x;
    height=y;
    dlcsm_gl_delete(glBmap);
    GLuint * data;
    data = (unsigned int*)new GLuint[((width * height)* 4 * sizeof(unsigned int))];
    memset(data,0,((width * height)* 4 * sizeof(unsigned int)));
    glGenTextures(1, &glBmap);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    delete [] data;
    if(pLog) pLog->_DebugAdd("CGLTexture::Create() -> created a texture!");
    return glBmap;
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
    glBindTexture(GL_TEXTURE_2D,glBmap);
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
    return true;
}
GLuint CGLTexture::Load(const char *file) {
    glBmap=0;
    strcpy(filename,file);
    string ext=dlcs_filetype(file);
    if (ext=="png") return LoadPNG(file);
    if (ext=="bmp") return LoadBMP(file);
    if (ext=="tga") return LoadTGA(file);
    if((ext=="jpg")||
       (ext=="jpeg")) return LoadJPG(file);
    if (ext=="tif") return LoadTIF(file);
    if (ext=="dds") return LoadDDS(file);
    if (ext=="gif") return LoadGIF(file);
    ext.clear();
    return glBmap;
}

GLuint CGLTexture::LoadFromMem(const char *file){ // TODO
    return 0;
}
GLuint CGLTexture::LoadBMPFromMem(unsigned char* fb, GLuint* idata){// TODO
    return 0;
}
GLuint CGLTexture::LoadTextureFromMem(unsigned char *fb){ // TODO
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
        glDEL(glBmap);
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

    	    glGenTextures(1, &glBmap);
    	    glBindTexture(GL_TEXTURE_2D, glBmap);


    		if(pLog) pLog->_DebugAdd("CGLTexture::Load(pGAF,filename,which) -> %s %d %d size(%d) glBmap(%d)",filename,himage->Gewidth(),himage->Geheight(),himage->GetSize(),glBmap);

    		glTexImage2D    ( GL_TEXTURE_2D,
    						0,
    						3,
    						himage->Gewidth(),
    						himage->Geheight(),
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
    								himage2->Gewidth(),
    								himage2->Geheight(),
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
    dlcsm_gl_delete(glBmap);
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
        pLog->_DebugAdd("IMAGE   : Filename   = %s     OPENGL[%d] WIDTH: %d  HEIGHT: %d \n",filename,glBmap,x,y);
        glGenTextures(1, &glBmap);
        glBindTexture(GL_TEXTURE_2D, glBmap);
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
    } */
    return 0;
}
GLuint CGLTexture::LoadPNG(const char *file) {
    glBmap=0;
    strcpy(filename,file);
    pLog->_Add("Loading PNG file [%s]",filename);
    png_byte header[8];
    FILE *fp = fopen(filename, "rb");
    if (fp == 0) { perror(filename); return 0; }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) { pLog->_Add("error: %s is not a PNG.\n", filename); fclose(fp); return 0;}

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) { pLog->_Add("error: png_create_info_struct returned 0"); fclose(fp); return 0; }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) { pLog->_Add("error: png_create_info_struct returned 0"); png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL); fclose(fp); return 0;    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) { pLog->_Add("error: png_create_info_struct returned 0"); png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL); fclose(fp); return 0;    }

    if (setjmp(png_jmpbuf(png_ptr))) { pLog->_Add("error from libpng"); png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);fclose(fp); return 0;}

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    int color_type;



    png_get_IHDR(
        png_ptr,
        info_ptr,
        &width,
        &height,
        &BPP,
        &color_type,
        NULL,
        NULL,
        NULL);

    pLog->_Add(" %d %d %d",width,height,BPP);
    png_read_update_info(png_ptr, info_ptr);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    rowbytes += 3 - ((rowbytes-1) % 4);

    png_byte * image_data;
    image_data=malloc(rowbytes * height * sizeof(png_byte)+15);
    if(image_data == NULL) { pLog->_Add("error: could not allocate memory for PNG image data"); png_destroy_read_struct(&png_ptr, &info_ptr, &end_info); fclose(fp); return 0; }

    png_bytep * row_pointers = malloc(height * sizeof(png_bytep));
    if(row_pointers == NULL) { pLog->_Add("error: could not allocate memory for PNG row pointers"); png_destroy_read_struct(&png_ptr, &info_ptr, &end_info); dlcsm_free(image_data); fclose(fp); return 0; }

    for(int i = 0; i < height; i++) row_pointers[height - 1 - i] = (png_bytep )image_data + i * rowbytes;

    png_read_image(png_ptr, row_pointers);

    glGenTextures(1, &glBmap);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)    format=GL_RGBA;
    else                                        format=GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format,       width,      height, 0, format, GL_UNSIGNED_BYTE, image_data);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return glBmap;
}
GLuint CGLTexture::LoadBMP(const char *file) {
    strcpy(filename,file);
    pLog->_Add("Loading BMP file [%s]",filename);
    glBmap=0;
    SDL_Surface *loadSurface;
    loadSurface = SDL_LoadBMP(filename);
    if(loadSurface) {
        BPP = loadSurface->format->BytesPerPixel;
        if (BPP == 4) {
            if (loadSurface->format->Rmask == 0x000000ff)   format = GL_RGBA;
            else                                            format = GL_BGRA;
        }
        else
        if (BPP == 3) {
            if(loadSurface->format->Rmask == 0x000000ff)   format = GL_RGB;
            else                                            format = GL_BGR;
        } else {
            pLog->_Add("Invalid bitmap format");
            SDL_FreeSurface(loadSurface);
            glBmap=0;
            return false;
        }
        glGenTextures( 1, &glBmap);
        glBindTexture( GL_TEXTURE_2D, glBmap );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        width  = loadSurface->w;
        height = loadSurface->h;
        glTexImage2D(   GL_TEXTURE_2D, 0, 4,
                        width,
                        height,
                        0,
                        format,
                        GL_UNSIGNED_BYTE,
                        loadSurface->pixels);
        SDL_FreeSurface(loadSurface);
    }
    return glBmap;
}
GLuint CGLTexture::LoadTGA(const char* file){
/////////////////////////////// TGA Stuff
    typedef struct { GLubyte Header[12]; } TGAHeader;
    typedef struct {
        GLubyte		header[6];
        GLuint		bytesPerPixel;
        GLuint		imageSize;
        GLuint		temp;
        GLuint		type;
        GLuint		Height;
        GLuint		Width;
        GLuint		Bpp; } TGA;

    TGAHeader tgaheader;									// TGA header
    TGA tga;												// TGA image data
    GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
    GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

    GLuint* data;

    pLog->_Add("Loading TGA file [%s] [%d]",file,sizeof(TGAHeader));
    glBmap=0;

    bool bCompressed=0;
    strcpy(filename,file);
	FILE* fp;
	fp = fopen(filename,"rb");
	if(!fp) return false;

	if(fread(&tgaheader, sizeof(tgaheader), 1, fp) == 0) {
		if(fp) fclose(fp);
        return false;
	}
	pLog->_Add("Loading TGA file [%s] 2",file);
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)         bCompressed=false;
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)    bCompressed=true;
	else {
        fclose(fp);
        return false;
	}
	pLog->_Add("Loading TGA file [%s] 3",file);
    if(bCompressed==false) {
        if(fread(tga.header, sizeof(tga.header), 1, fp) == 0) {
            if(fp) fclose(fp);
            return false;
        }
        width    = tga.header[1] * 256 + tga.header[0];
        height   = tga.header[3] * 256 + tga.header[2];
        BPP	    = tga.header[4];
        tga.Width		= width;
        tga.Height		= height;
        tga.Bpp			= BPP;
        if((width <= 0) || (height <= 0) || ((BPP != 24) && (BPP !=32))) {
            if(fp) fclose(fp);
            return false;
        }
        if(BPP == 24)  format = GL_RGB;
        else                    format = GL_RGBA;
        tga.bytesPerPixel	= (tga.Bpp / 8);
        tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
        data	= (GLuint *)malloc(tga.imageSize);
        if(data == NULL) {
            fclose(fp);
            return false;
        }
        if(fread(data, 1, tga.imageSize, fp) != tga.imageSize) {
            if(data) dlcsm_free(data);
            fclose(fp);
            return false;
        }
        for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel) {
            data[cswap] ^= data[cswap+2] ^=
            data[cswap] ^= data[cswap+2];
        }
        fclose(fp);
        return true;
    }

    if(bCompressed==true) {
        pLog->_Add("Loading TGA file [%s] 4",file);
        if(fread(tga.header, sizeof(tga.header), 1, fp) == 0){
            if(fp) fclose(fp);
            return false;
        }

        pLog->_Add("Loading TGA file [%s] 5",file);

        width  = tga.header[1] * 256 + tga.header[0];
        pLog->_Add("Loading TGA file [%s] 5.1",file);
        height = tga.header[3] * 256 + tga.header[2];
        pLog->_Add("Loading TGA file [%s] 5.2",file);
        BPP	= tga.header[4];
        pLog->_Add("Loading TGA file [%s] 5.3",file);
        tga.Width		= width;
        pLog->_Add("Loading TGA file [%s] 5.4",file);
        tga.Height		= height;
        pLog->_Add("Loading TGA file [%s] 5.5",file);
        tga.Bpp			= BPP;
        pLog->_Add("Loading TGA file [%s] 5.6",file);

        pLog->_Add("Loading TGA file [%s] 6",file);

        if((width <= 0) || (height <= 0) || ((BPP != 24) && (BPP !=32))){
            if(fp) fclose(fp);
            return false;
        }

        pLog->_Add("Loading TGA file [%s] 7",file);

        if(BPP == 24)  format = GL_RGB;
        else                    format = GL_RGBA;
        tga.bytesPerPixel	= (tga.Bpp / 8);
        tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
        data	= (GLuint *)malloc(tga.imageSize);
        if(data == NULL) {
            fclose(fp);
            return false;
        }

        pLog->_Add("Loading TGA file [%s] 8",file);

        GLuint pixelcount	= tga.Height * tga.Width;
        GLuint currentpixel	= 0;
        GLuint currentbyte	= 0;
        GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);
        pLog->_Add("Loading TGA file [%s] 9",file);
        do {
            GLubyte chunkheader = 0;
            if(fread(&chunkheader, sizeof(GLubyte), 1, fp) == 0) {
                if(fp) fclose(fp);
                if(data) dlcsm_free(data);
                return false;
            }
            if(chunkheader < 128) {
                chunkheader++;
                for(short counter = 0; counter < chunkheader; counter++) {
                    if(fread(colorbuffer, 1, tga.bytesPerPixel, fp) != tga.bytesPerPixel) {
                        if(fp) fclose(fp);
                        if(colorbuffer) dlcsm_free(colorbuffer);
                        if(data) dlcsm_free(data);
                        return false;
                    }
                    data[currentbyte		] = colorbuffer[2];
                    data[currentbyte + 1	] = colorbuffer[1];
                    data[currentbyte + 2	] = colorbuffer[0];
                    if(tga.bytesPerPixel == 4) data[currentbyte + 3] = colorbuffer[3];
                    currentbyte += tga.bytesPerPixel;
                    currentpixel++;
                    if(currentpixel > pixelcount) {
                        if(fp) fclose(fp);
                        if(colorbuffer) dlcsm_free(colorbuffer);
                        if(data) dlcsm_free(data);
                        return false;
                    }
                }
            }
            else {
                chunkheader -= 127;
                if(fread(colorbuffer, 1, tga.bytesPerPixel, fp) != tga.bytesPerPixel) {
                    if(fp) fclose(fp);
                    if(colorbuffer) dlcsm_free(colorbuffer);
                    if(data) dlcsm_free(data);
                    return false;
                }
                for(short counter = 0; counter < chunkheader; counter++) {																			// by the header
                    data[currentbyte		] = colorbuffer[2];
                    data[currentbyte + 1	] = colorbuffer[1];
                    data[currentbyte + 2	] = colorbuffer[0];
                    if(tga.bytesPerPixel == 4) data[currentbyte + 3] = colorbuffer[3];
                    currentbyte += tga.bytesPerPixel;
                    currentpixel++;
                    if(currentpixel > pixelcount) {
                        if(fp) fclose(fp);
                        if(colorbuffer) dlcsm_free(colorbuffer);
                        if(data) dlcsm_free(data);
                        return false;
                    }
                }
            }
        }
        while(currentpixel < pixelcount);

        pLog->_Add("Loading TGA file [%s] 10",file);
        fclose(fp);
    }

    pLog->_Add("Loading TGA file [%s] 11",file);
    glGenTextures( 1, &glBmap);
    glBindTexture(GL_TEXTURE_2D, glBmap );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexImage2D(   GL_TEXTURE_2D,
                    0,
                    4,
                    width,
                    height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    data);

    pLog->_Add("Loading TGA file [%s] 12",file);
    dlcsm_free(data);
	return glBmap;
}
GLuint CGLTexture::LoadJPG(const char* file){
    glBmap=0;
#ifdef DLCSM_WINDOWS
    strcpy(filename,file);
    pLog->_Add("Loading JPG file [%s]",filename);
    bool Fast;
    unsigned long sz;
    FILE* fp = fopen(filename, "rb");
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;
    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info);

    if(!fp) {
        pLog->_Add("Error reading JPEG file %s!!!", filename);
        return false;
    }
    jpeg_stdio_src(&info, fp);    //tell the jpeg lib the file we'er reading
    jpeg_read_header(&info, TRUE);   //tell it to start reading it
    if(Fast)
      info.do_fancy_upsampling = FALSE;
    jpeg_start_decompress(&info);    //decompress the file
    width = info.output_width;
    height = info.output_height;
    sz = width * height * 3;

    info.buffered_image = new BYTE[sz];
    //read the scan lines
    BYTE*  p1 = info.buffered_image;
    BYTE** p2 = &p1;
    int numlines = 0;
    while(info.output_scanline < info.output_height) {
      numlines = jpeg_read_scanlines(info.buffered_image, p2, 1);
      *p2 += numlines * 3 * info.output_width;
    }
    jpeg_finish_decompress(info.buffered_image);
    fclose(fp);

    glGenTextures(1, &glBmap);
    glBindTexture(GL_TEXTURE_2D, glBmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(   GL_TEXTURE_2D,
                    0,
                    GL_RGB8,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    info.buffered_image);

    jpeg_destroy_decompress(info.buffered_image);
#endif // DLCSM_WINDOWS
    return glBmap;
}
GLuint CGLTexture::LoadTIF(const char* file) { // TODO
}
GLuint CGLTexture::LoadDDS(const char* file) { // TODO
}
GLuint CGLTexture::LoadGIF(const char* file) { // TODO
}
bool CGLTexture::Draw2d(int x,int y,int x2,int y2,u_char r,u_char g,u_char b) { return Draw2d(x,y,x2,y2,r,g,b,255,255,255);}
bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b) {   return Draw(x,y,x2,y2,r,g,b,255,255,255); }
bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2) {
    if(!glBmap) { Load(filename); return false; }
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
    glBindTexture(GL_TEXTURE_2D,glBmap);
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
    if(!glBmap) { Load(filename); return 0; }
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
    glBindTexture(GL_TEXTURE_2D,glBmap);
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

