#ifndef _C_GUI_CHAT_ENUM
#define _C_GUI_CHAT_ENUM

#include "dlstorm.h"

typedef enum{
    CHANNEL_UNKNOWN,
    CHANNEL_SYSTEM,
    CHANNEL_CONSOLE,

    CHANNEL_LOCAL,
    CHANNEL_TRADE,
    CHANNEL_GENERAL,

    CHANNEL_WHISPER,
    CHANNEL_PARTY,
    CHANNEL_RAID,
    CHANNEL_SAY,
    CHANNEL_YELL,

    CHANNEL_CLAN,

    CHANNEL_CUSTOM,

} tChatChannels;

extern map <string, int> GUI_CHAT;
extern map <int, string> rGUI_CHAT;
extern map <int ,string> GUI_CHAT_COLOR;

extern "C" void dfGUI_CHAT(void);


#endif
