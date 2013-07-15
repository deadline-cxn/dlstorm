

#include "c_entity.h"

CEntity::CEntity()
{
    set_defaults();
    Log=new CLog("Entity.log");
}

CEntity::CEntity(char *nm)
{
    set_defaults();
    strcpy(name,nm);
    Log=new CLog(va("%s.log",name));
}

CEntity::~CEntity()
{
    DEL(Log);
}

