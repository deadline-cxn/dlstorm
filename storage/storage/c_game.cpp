
#include "c_game.h"

CGame::CGame()
{
	screen_width=800;
	screen_height=600;
	screen_colors=32;
}

CGame::CGame(char *APP_NAME,Uint16 FLAGS)
{
	screen_width=800;
	screen_height=600;
	screen_colors=32;
    Start(APP_NAME,FLAGS);
}

CGame::CGame(char *APP_NAME,Uint32 SCREEN_WIDTH,Uint32 SCREEN_HEIGHT,Uint32 SCREEN_COLORS,Uint16 FLAGS)
{
	screen_width=SCREEN_WIDTH;
	screen_height=SCREEN_HEIGHT;
	screen_colors=SCREEN_COLORS;
    Start(APP_NAME,FLAGS);
}

CGame::~CGame()
{
    ShutDown();
}

void CGame::Start(char *APP_NAME,Uint16 _flags)
{
    flags=_flags;

    G_QUIT=false;

    Log=0;
    GFX=0;
    SND=0;

    Log=new CLog(va("%s.log",APP_NAME));
    Log->AddEntry("------------------------------------------------------");
    Log->AddEntry(va("%s! Started...",APP_NAME));

    if(flags&G_SDL)
    {
        GFX = new CSDL_Wrap(APP_NAME,screen_width,screen_height,screen_colors, flags, "gfx/icon.bmp");
        if(!GFX) ShutDown();
        if(!GFX->InitSuccess) ShutDown();
        Log->AddEntry("GFX Subsystem Initialized...");
    }

    if(flags&G_FMOD)
    {
        SND = new CFMOD();
        Log->AddEntry("SND Subsystem Initialized...");
    }

    srand(time(NULL));

    UserInit();
    Log->AddEntry("UserInit() Completed...");

}

void CGame::ShutDown()
{
    Log->AddEntry("Game shutting down...");
    DEL(SND);
    Log->AddEntry("Game shutting down... SND");
    DEL(GFX);
    Log->AddEntry("Game shutting down... GFX");
    DEL(Log);
    G_QUIT=true;
}

bool CGame::Inject(Uint16 TYPE,char *name)
{

    return true;
}

bool CGame::UpdateGFXStart()
{
    if(!GFX) return false;
    GFX->ClearScreen(SDL_MapRGB(GFX->screen->format,0,0,0));

    // Core Engine Draws here...
    //GFX->DrawSprites();

    return false;
}

bool CGame::DrawVortex()
{

        /* // CSprite     *spaceship;
        // CSprite     *grape;
        spaceship   = new CSprite();
        spaceship->source_surface = GFX->Cab->GetSurface("gfx/spaceship.bmp");
        spaceship->animated=false;
        spaceship->SetRect(0,0,0,32,32);
        spaceship->xspeed=2;
        grape       = new CSprite();
        grape->source_surface = GFX->Cab->GetSurface("gfx/grape.tga");
        if(grape->source_surface)
        grape->SetRect(0,0,0,grape->source_surface->w,grape->source_surface->h);
        //DEL(spaceship);    //DEL(grape);
        ///////////////////////////////////
        // Test Vars
        //    static int cx=rand()%GFX->w;
        //static int cy=rand()%GFX->h;
        // int x,y;
        //GFX->DrawMap();
        /*    spaceship->x+=spaceship->xspeed;
        if(spaceship->x>GFX->w+32)
        {
        spaceship->x=-32;
        spaceship->y+=32;
        if(spaceship->y>GFX->h+32)
        {
            spaceship->y=32;
        }
        }
        spaceship->Draw();
        grape->Draw(100,100);
        cx+=rand()%5-2; if(rand()%500>450) { if(cx<GetMouseX()) cx++; if(cx>GetMouseX())cx--; }
        cy+=rand()%5-2; if(rand()%500>450) { if(cy<GetMouseY()) cy++; if(cy>GetMouseY())cy--; }
        /*if(cx>(GFX->w-1)) cx=GFX->w-1;
        if(cx<1) cx=1;
        if(cy>(GFX->h-1)) cy=GFX->h-1;
        if(cy<1) cy=1;
        // GFX->putpixel(rand()%GFX->w,rand()%GFX->h,RGB(0,0,rand()%256));
        for(x=-1;x<2;x++)
        for(y=-1;y<2;y++)
            GFX->putpixel(cx+x,cy+y,RGB(0,rand()%256,rand()%256));*/
        // end junk to test with
        //////////////////////////////////////////////////////////////////////////////////////////*/
    return 0;
}

bool CGame::UpdateGFXEnd()
{
    if(!GFX) return false;G_QUIT=true;
    GFX->Flip();
    return false;
}

bool CGame::UpdateInput()
{
    SDL_Event   event;

    G_KEYDOWN=0;

    RefreshMouse();
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_KEYDOWN:
            G_KEYDOWN=event.key.keysym.sym;
            break;

        case SDL_VIDEOEXPOSE:
            break;

        case SDL_VIDEORESIZE:
            break;

        case SDL_QUIT:
            G_QUIT=true;
            break;

        default:
            break;
    }

    UserKeys();

    return false;
}

bool CGame::Loop()
{
    UpdateGFXStart();
    UserGFX();
    UpdateGFXEnd();
    UpdateInput();


    return G_QUIT;
}

