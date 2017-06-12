#include "dlstorm.h"
#include "c_log.h"
#include "c_var.h"
#include "c_gfx.h"
#include "s_gnu.h"

CLog *pLog;
CVarSet *pData;
C_GFX *pGFX;

int main() {
    pLog  = new CLog("test.log");
    pLog->AddEntry("testing test.cpp");
    pData = new CVarSet();
    pData->bLoad();

    pLog->AddEntry(pData->filename.c_str());
    
    
    pGFX = new C_GFX(1024, 768, 32, 0, "Test",1, pLog,0);
    

    Sleep(25000);
    
    
    pData->bSave();
    
    
    DEL(pData);
    DEL(pGFX);
    pLog->AddEntry("Ending...");
    DEL(pLog);
    return 0;
}
