#include "dlstorm.h"
#include "c_log.h"
#include "c_var.h"
#include "c_gfx.h"
#include "s_gnu.h"
#include "c_timer.h"

CLog *pLog;
CVarSet *pData;
C_GFX *pGFX;
CTimer *pTimer;

int main() {
    pLog  = new CLog("test.log");
    pLog->AddEntry("Testing test.cpp %s (%s)", OSSTRING, CPUSTRING);
    pData = new CVarSet();
    pData->bLoad();
    pLog->AddEntry(pData->filename.c_str());
    pGFX = new C_GFX(1024, 768, 32, 0, va("Test %s (%s)",OSSTRING,CPUSTRING),1, pLog,0);
    
    pTimer=new CTimer(5000);
    int i;
    while(i < 3) {
        
        
        
        while(!pTimer->Up()) { 
                // pGFX->setpixel(rand()%1024, rand()%768, , rand()%255, rand()%255);
                
                pGFX->BeginScene();
                
                // pGFX->DrawBar(rand()%1024, rand()%768, rand()%255, rand()%255, LONGRGB(rand()%255,rand()%255,rand()%255));
                
                pGFX->DrawStarField(122);
                pGFX->EndScene();
        }
        
        pTimer->Reset();
        printf("TICK\n");
        i++;
    }

    pLog->AddEntry("Ending...");
    DEL(pTimer);
    DEL(pGFX);
    pData->bSave();
    DEL(pData);
    DEL(pLog);
    return 0;
}
