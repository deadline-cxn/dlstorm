#include "dlstorm.h"
#include "c_log.h"
#include "c_var.h"

CLog *pLog;
CVarSet *pData;


int main() {
    pLog  = new CLog("test.log");
    pLog->AddEntry("testing test.cpp");
    
    pData = new CVarSet();
    pData->bLoad();
    
    
    
    
    
    pLog->AddEntry(pData->filename.c_str());
    
        
    
    
    
    
    pData->bSave();    
    DEL(pData);
    DEL(pLog);
    return 0;
}
