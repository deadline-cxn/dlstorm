
#include <c_sdl_gfx_util.h>
#include "dlstorm.h"

CSDL_Wrap::CSDL_Wrap()
{
    w=800;
    h=600;
    b=32;
    InitSuccess=Init2D(w,h,b);
}
CSDL_Wrap::CSDL_Wrap(Uint32 nw,Uint32 nh,Uint32 nb)
{
    w=nw;
    h=nh;
    b=nb;
    InitSuccess=Init2D(nw,nh,nb);
}
CSDL_Wrap::CSDL_Wrap(Uint32 nw,Uint32 nh,Uint32 nb,bool f)
{
    w=nw;
    h=nh;
    b=nb;
    InitSuccess=Init2D(nw,nh,nb,f);
}
CSDL_Wrap::~CSDL_Wrap()
{
    SDL_Quit();
}

bool CSDL_Wrap::Init2D(int width, int height, int bpp)
{    
	return Init2D(width,height,bpp,false);
}

bool CSDL_Wrap::Init2D(int width, int height, int bpp, bool fullscreen)
{    
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    // Set a video mode 
	
    videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF;
	
	if(fullscreen==true)
	{
		videoflags|=SDL_FULLSCREEN|SDL_NOFRAME;
	}
	//else
	//	videoflags|=SDL_RESIZABLE;

    screen = CreateScreen(width, height, bpp, videoflags);
    if(!screen)
    {
        videoflags=SDL_SWSURFACE|SDL_FULLSCREEN|SDL_NOFRAME;
        screen = CreateScreen(width, height, bpp, videoflags);
        if(!screen) return false;
    }
    return true;
}

SDL_Surface *CSDL_Wrap::CreateScreen(Uint16 w, Uint16 h, Uint8 bpp, Uint32 flags)
{
    SDL_Surface *screen;
    int i;
    SDL_Color palette[NUM_COLORS];
    screen = SDL_SetVideoMode(w, h, bpp, flags);
    if(!screen) return screen;
    if (bpp==8) 
    {
        for ( i=0; i < NUM_COLORS; ++i )
        {
            palette[i].r = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
            palette[i].g = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
            palette[i].b = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
        }
        SDL_SetColors(screen, palette, 0, NUM_COLORS);
    }
    return(screen);
}


SDL_Surface * LoadMemBMP(unsigned char *fb, int Size)
{    
   
    if(!fb) return 0;

	long fp_offset;
    fp_offset=0;
	int bmpPitch, i, pad;
	SDL_Surface *surface;
	Uint32 Rmask, Gmask, Bmask;
	SDL_Palette *palette;
	Uint8 *bits;
	int ExpandBMP, cursor;

	char   magic[2];
	Uint32 bfSize;
	Uint16 bfReserved1;
	Uint16 bfReserved2;
	Uint32 bfOffBits;

	Uint32 biSize;
	Sint32 biWidth;
	Sint32 biHeight;
	Uint16 biPlanes;
	Uint16 biBitCount;
	Uint32 biCompression;
	Uint32 biSizeImage;
	Sint32 biXPelsPerMeter;
	Sint32 biYPelsPerMeter;
	Uint32 biClrUsed;
	Uint32 biClrImportant;

	surface = NULL;

    magic[0]=fb[0];
    magic[1]=fb[1];
    magic[2]=0;

    bfSize      = s_MAKEDWORD( fb[2],fb[3],fb[4],fb[5]);
    bfReserved1 = s_MAKEWORD(  fb[6],fb[7]);
    bfReserved2 = s_MAKEWORD(  fb[8],fb[9]);
    bfOffBits   = s_MAKEDWORD( fb[10],fb[11],fb[12],fb[13]);
    biSize      = s_MAKEDWORD( fb[14],fb[15],fb[16],fb[17]);
    biWidth     = s_MAKEDWORD( fb[18],fb[19],fb[20],fb[21]);
    biHeight    = s_MAKEDWORD( fb[22],fb[23],fb[24],fb[25]);
    biPlanes    = s_MAKEWORD(  fb[26],fb[27]);
    biBitCount  = s_MAKEWORD(  fb[28],fb[29]);
   
    
    //Log->AddEntry(va("CCab->GetBMP(%s)=[%s] FileSize[%d] Res1[%d] Res2[%d] OffBits[%d] Size[%d] Width[%d] Height[%d] Planes[%d] BPP[%d]",nm,magic,bfSize,bfReserved1,bfReserved2,bfOffBits,biSize,biWidth,biHeight,biPlanes,biBitCount));

    if( biSize == 12)
    {
		biCompression	= BI_RGB;
		biSizeImage	    = 0;
		biXPelsPerMeter	= 0;
		biYPelsPerMeter	= 0;
		biClrUsed	    = 0;
		biClrImportant	= 0;
    }
    else
    {
		biCompression	= s_MAKEDWORD(  fb[30],fb[31],fb[32],fb[33]);
		biSizeImage	    = s_MAKEDWORD(  fb[34],fb[35],fb[36],fb[37]);
		biXPelsPerMeter	= s_MAKEDWORD(  fb[38],fb[39],fb[40],fb[41]);
		biYPelsPerMeter	= s_MAKEDWORD(  fb[42],fb[43],fb[44],fb[45]);
		biClrUsed	    = s_MAKEDWORD(  fb[46],fb[47],fb[48],fb[49]);
		biClrImportant	= s_MAKEDWORD(  fb[50],fb[51],fb[52],fb[53]);
    }

	switch (biBitCount) 
    {
		case 1:
		case 4:
			ExpandBMP = biBitCount;
			biBitCount = 8;
			break;
		default:
			ExpandBMP = 0;
			break;
	}
	Rmask = Gmask = Bmask = 0;
	switch (biCompression)
    {
		case BI_RGB:
			if ( bfOffBits == (14+biSize) )
            {
				switch (biBitCount)
                {
					case 15:
					case 16:
						Rmask = 0x7C00;
						Gmask = 0x03E0;
						Bmask = 0x001F;
						break;
					case 24:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					    Rmask = 0x000000FF;
					    Gmask = 0x0000FF00;
					    Bmask = 0x00FF0000;
						break;
#endif
					case 32:
						Rmask = 0x00FF0000;
						Gmask = 0x0000FF00;
						Bmask = 0x000000FF;
						break;
					default:
						break;
				}
				break;
			}

		case BI_BITFIELDS:
			switch (biBitCount)
            {
				case 15:
				case 16:
				case 32:
					Rmask = s_MAKEDWORD(  fb[54],fb[55],fb[56],fb[57]);
					Gmask = s_MAKEDWORD(  fb[58],fb[59],fb[60],fb[61]);
					Bmask = s_MAKEDWORD(  fb[62],fb[63],fb[64],fb[65]);
					break;
				default:
					break;
			}
			break;
		default:			
			return 0;
            break;
	}

	// Create a compatible surface, note that the colors are RGB ordered 
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, biWidth, biHeight, biBitCount, Rmask, Gmask, Bmask, 0);
    if(surface==NULL) return 0;

    //Log->AddEntry(va("Surface created... [%d]",surface));

    cursor=66;

	palette = (surface->format)->palette;
	if ( palette ) 
    {
		if ( biClrUsed == 0 ) 
        {
			biClrUsed = 1 << biBitCount;
		}
		if ( biSize == 12 ) 
        {
			for ( i = 0; i < (int)biClrUsed; ++i )
            {
                palette->colors[i].b=fb[cursor++];
                palette->colors[i].g=fb[cursor++];
                palette->colors[i].r=fb[cursor++];
				palette->colors[i].unused = 0;
            }
		}
        else 
        {
			for ( i = 0; i < (int)biClrUsed; ++i )
            {
                palette->colors[i].b=fb[cursor++];
                palette->colors[i].g=fb[cursor++];
                palette->colors[i].r=fb[cursor++];
				palette->colors[i].unused=fb[cursor++];
            }
		}
		palette->ncolors = biClrUsed;
	}
    else
    {
        //Log->AddEntry(va("Palette not used for %s",nm));
    }


    cursor=fp_offset+bfOffBits;

    //Log->AddEntry(va(">>> Cursor to: %d",cursor));

	bits=(Uint8 *)surface->pixels+(surface->h*surface->pitch);

    //Log->AddEntry(va("Pitch: %d",surface->pitch));

	switch(ExpandBMP)
    {
		case 1:
			bmpPitch = (biWidth + 7) >> 3;
			pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
			break;
		case 4:
			bmpPitch = (biWidth + 1) >> 1;
			pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
			break;
		default:
			pad  = ((surface->pitch%4) ?
					(4-(surface->pitch%4)) : 0);
			break;
	}

	while(bits > (Uint8 *)surface->pixels)
    {
		bits -= surface->pitch;
		switch(ExpandBMP)
        {
			case 1:
            case 4:
                {
			        Uint8 pixel = 0;
			        int   shift = (8-ExpandBMP);
			        for ( i=0; i<surface->w; ++i )
                    {
				        if ( i%(8/ExpandBMP) == 0 )
                        {
                            pixel=fb[cursor++];
				        }
				        *(bits+i) = (pixel>>shift);
				        pixel <<= ExpandBMP;
			        }
                }
			break;

			default:
                for(int j=0;j<surface->pitch;j++) bits[j]=fb[cursor++];
                
			  

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			// Byte-swap the pixels if needed. Note that the 24bpp case has already been taken care of above.
			    switch(biBitCount)
                {
				    case 15:
				    case 16:
                    
				        Uint16 *pix = (Uint16 *)bits;
					    for(i = 0; i < surface->w; i++)
					        pix[i] = SDL_Swap16(pix[i]);
					    break;
				    

				    case 32: 
				        Uint32 *pix = (Uint32 *)bits;
					    for(i = 0; i < surface->w; i++)
					        pix[i] = SDL_Swap32(pix[i]);
					    break;
			    }
#endif
			break;
		}

		if(pad)
        {
		    Uint8 padbyte;
            for(i=0;i<pad;++i)
            {
                padbyte=fb[cursor++];
			}
		}
	}
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY,SDL_MapRGB(surface->format, 0, 0, 0));
	return surface;
}


bool CSDL_Wrap::ToggleFullScreen()
{
    int videoflags;
    videoflags = screen->flags;
    videoflags ^= SDL_FULLSCREEN;
    screen = CreateScreen( screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
    if(screen == NULL ) return false;
    return true;
}

// Return the pixel value at (x, y) NOTE: The surface must be locked before calling this!
Uint32 CSDL_Wrap::getpixel(int x, int y)
{
    if(x>(screen->w-1)) return 0;
    if(x<0) return 0;
    if(y>(screen->h-1)) return 0;
    if(y<0) return 0;

    int bpp = screen->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;
    }
}
// Set the pixel at (x, y) to the given value NOTE: The surface must be locked before calling this!
void CSDL_Wrap::putpixel(int x, int y, Uint32 pixel)
{
    if(x>(screen->w-1)) return;
    if(x<0) return;
    if(y>(screen->h-1)) return;
    if(y<0) return;
    int bpp=screen->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set 
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    switch(bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if(SDL_BYTEORDER==SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } 
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void CSDL_Wrap::ClearScreen(Uint32 color)
{
    SDL_Rect barea2; barea2.x=0; barea2.y=0; barea2.w=screen->w;barea2.h=screen->h;
    SDL_FillRect(screen, &barea2, color);
}

void CSDL_Wrap::Flip()
{
    if(videoflags&&SDL_DOUBLEBUF)
    SDL_Flip(screen);
}