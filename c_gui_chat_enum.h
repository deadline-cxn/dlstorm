#ifndef _C_GUI_CHAT_ENUM
#define _C_GUI_CHAT_ENUM

#include <map>
#include <string>
using std::map;
using std::string;

#include "dlstorm.h"

typedef enum {
    CHANNEL_UNKNOWN,
    CHANNEL_SERVER,
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
    CHANNEL_FRIENDS,
    CHANNEL_TEAM,
    CHANNEL_MATCH,
    CHANNEL_ARENA,
    CHANNEL_CUSTOM,
} tChatChannels;

extern map<string, int> GUI_CHAT;
extern map<int, string> rGUI_CHAT;
extern map<int, string> GUI_CHAT_COLOR;

extern "C" void dfGUI_CHAT(void);

#endif
