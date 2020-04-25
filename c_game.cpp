
#include "c_game.h"

CGame::CGame()
{
	screen_width=800;
	screen_height=600;
	screen_colors=32;
}

CGame::CGame(char *APP_NAME,Uint16 iFlags) {
	screen_width=800;
	screen_height=600;
	screen_colors=32;
    Start(APP_NAME,iFlags);
}

CGame::CGame(char *APP_NAME,Uint32 iScreenWidth,Uint32 iScreenHeight,Uint32 iScreenColors,Uint16 iFlags) {
	screen_width=iScreenWidth;
	screen_height=iScreenHeight;
	screen_colors=iScreenColors;
    Start(APP_NAME,iFlags);
}

CGame::~CGame() {
    ShutDown();
}

void CGame::Start(char *APP_NAME,Uint16 iFlags) {
    G_QUIT=false;
    flags=iFlags;
    Log=0;
    SDL=0;
    // SND=0;
    Log=new CLog((char *)va("%s.log",APP_NAME));
    Log->AddEntry((char *)"------------------------------------------------------");
    Log->AddEntry((char *)va("%s! Started...",APP_NAME));
    if(flags&G_SDL) {
        SDL = new CSDL_Wrap((char *)APP_NAME,screen_width,screen_height,screen_colors,(char *)"gfx/icon.bmp");
        if(!SDL) ShutDown();
        if(!SDL->InitSuccess) {
            Log->AddEntry((char *)"SDL Subsystem Init FAILURE!");
            ShutDown();
            return;
        }
    }
    Log->AddEntry((char *)"SDL Subsystem Initialized...");
    if(flags&G_FMOD) {
        // SND = new CFMOD();
        // Log->AddEntry((char *)"SND Subsystem Initialized...");
    }
    srand(time(NULL));
    UserInit();
    Log->AddEntry((char *)"UserInit() Completed...");
}

void CGame::ShutDown() {
    Log->AddEntry((char *)"Game shutting down...");
    // dlcsm_delete(SND);
    // Log->AddEntry((char *)"Game shutting down... SND");
    dlcsm_delete(SDL);
    Log->AddEntry((char *)"Game shutting down... SDL");
    dlcsm_delete(Log);
    G_QUIT=true;
}

bool CGame::Inject(Uint16 TYPE,char *name) {
    return true;
}

bool CGame::UpdateGFXStart() {
    if(!SDL) return false;
    // SDL->ClearScreen(0);
    
    // SDL->ClearScreen(SDL_MapRGB(SDL->screen->format,0,0,0));
    // Core Engine Draws here...
    //SDL->DrawSprites();
    return false;
}

bool CGame::DrawVortex() {
    /* // CSprite     *spaceship;
    // CSprite     *grape;
    spaceship   = new CSprite();
    spaceship->source_surface = SDL->Cab->GetSurface("gfx/spaceship.bmp");
    spaceship->animated=false;
    spaceship->SetRect(0,0,0,32,32);
    spaceship->xspeed=2;
    grape       = new CSprite();
    grape->source_surface = SDL->Cab->GetSurface("gfx/grape.tga");
    if(grape->source_surface)
    grape->SetRect(0,0,0,grape->source_surface->w,grape->source_surface->h);
    //dlcsm_delete(spaceship);    //dlcsm_delete(grape);
    ///////////////////////////////////
    // Test Vars
    //    static int cx=rand()%SDL->w;
    //static int cy=rand()%SDL->h;
    // int x,y;
    //SDL->DrawMap();
    //   spaceship->x+=spaceship->xspeed;
    if(spaceship->x>SDL->w+32)
    {
    spaceship->x=-32;
    spaceship->y+=32;
    if(spaceship->y>SDL->h+32)
    {
        spaceship->y=32;
    }
    }
    spaceship->Draw();
    grape->Draw(100,100);
    cx+=rand()%5-2; if(rand()%500>450) { if(cx<GetMouseX()) cx++; if(cx>GetMouseX())cx--; }
    cy+=rand()%5-2; if(rand()%500>450) { if(cy<GetMouseY()) cy++; if(cy>GetMouseY())cy--; }
    if(cx>(SDL->w-1)) cx=SDL->w-1;
    if(cx<1) cx=1;
    if(cy>(SDL->h-1)) cy=SDL->h-1;
    if(cy<1) cy=1;
    // SDL->putpixel(rand()%SDL->w,rand()%SDL->h,RGB(0,0,rand()%256));
    for(x=-1;x<2;x++)
    for(y=-1;y<2;y++)
        SDL->putpixel(cx+x,cy+y,RGB(0,rand()%256,rand()%256));*/
    // end junk to test with
    //////////////////////////////////////////////////////////////////////////////////////////*/
    return 0;
}

bool CGame::UpdateGFXEnd() {
    if(!SDL) {
        G_QUIT=true;
        return false;
    }    
    SDL->Flip();
    return false;
}

bool CGame::UpdateInput() {
    SDL->pMouse->RefreshMouse();
    SDL_Event event;
    G_KEYDOWN=0;
    SDL_PollEvent(&event);
    switch (event.type)     {

        case SDL_MOUSEWHEEL:
            SDL->pMouse->UpdateWheel(event);
            break;

        case SDL_KEYDOWN:
            G_KEYDOWN=event.key.keysym.sym;
            break;

        // case SDL_VIDEOEXPOSE:            break;
        // case SDL_VIDEORESIZE:            break;

        case SDL_QUIT:
            G_QUIT=true;
            break;

        default:
            break;
    }
    UserKeys();
    return false;
}

bool CGame::Loop() {
    UpdateGFXStart();
    UserGFX();
    UpdateGFXEnd();
    UpdateInput();

    return G_QUIT;
}

