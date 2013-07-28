/***************************************************************
 **      EMBER                                                **
 ***************************************************************/
#ifndef _WINDOWS

#include "dlstorm.h"

typedef unsigned char byte;

extern "C" long GetTickCount(void);
extern "C" void Sleep(int ms);
extern "C" int _kbhit(void);
extern "C" int _getch(void);
extern "C" void close_keyboard(void);



#endif
