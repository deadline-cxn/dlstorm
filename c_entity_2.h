/* Seth's Game Entity Class */

#ifndef SETH_GAME_ENTITY
#define SETH_GAME_ENTITY

#include "dlstorm.h"
#include "c_log.h"


class CEntity;


class CEntity
{
public:
    CEntity();
    CEntity(char *nm);
    ~CEntity();

    CLog    *Log;



};












#endif // SETH_ENTITY_CLASS
