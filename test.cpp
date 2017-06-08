#include "dlstorm.h"
#include "c_log.h"
#include "c_data.h"
#include "c_gfx.h"

CLog *pLog;
CC_Data *pData;
C_GFX *pGFX;

int main() {
    pData = new CC_Data();
    pLog = new CLog("test.log");
    pGFX = new C_GFX(640,480,16,true,"test",pLog,0);
    int i=0;
    while(i<12) i++;
    pLog->AddEntry("testing test.cpp");
    DEL(pGFX);
    DEL(pLog);
    DEL(pData);
    return 0;
}
