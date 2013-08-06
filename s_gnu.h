/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#ifndef DLCSM_S_GNU_H
#define DLCSM_S_GNU_H
#ifndef DLCSM_WINDOWS
#include "dlstorm.h"
typedef unsigned char byte;
extern "C" long GetTickCount(void);
extern "C" void Sleep(int ms);
extern "C" int _kbhit(void);
extern "C" int _getch(void);
extern "C" void close_keyboard(void);
#endif // DLCSM_WINDOWS
#endif // DLCSM_S_GNU_H
