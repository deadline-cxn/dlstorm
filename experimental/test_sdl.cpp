#include "dlstorm.h"
#include "c_log.h"
#include "c_data.h"

CLog *pLog;
CC_Data *pData;


int main() { // int, char*[]
    pData=new CC_Data();
    pLog=new CLog("test.log");
    
    pLog->AddEntry("hi");
    DEL(pLog);
    DEL(pData);
    return 0;
    
}