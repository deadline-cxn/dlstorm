/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         dlcs.h
 **   Description:  DL Code Storm Header
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/
#ifndef _DLCS_LIBRARY
#define _DLCS_LIBRARY

#ifndef _HOST_NAME_MAX
#define _HOST_NAME_MAX 255
#endif
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif
#define _TINYNAME_SIZE 32
#define _NAME_SIZE     64
#define _TEXTNAME_SIZE 256
#define _FILENAME_SIZE 1024

#include "dlcs_os.h"
// include OS

#include "dlcs_cpu.h"
#include "dlcs_macros.h"
#include "dlcs_standard.h"

using namespace std;


#ifdef __cplusplus
extern "C" {
#endif
const char *   va(const char *format, ...);
vector<string> dlcs_explode(const string &delimiter, const string &explodeme);
bool           dlcs_Dir2File(const char *szDir, char *szFile, char *szWildCard);
char *         dlcs_encrypt(const char *text);  // encrypt text
char *         dlcs_decrypt(const char *text);  // decrypt text
void           dlcs_suspend_power_management(void);
int            dlcs_hex_to_dec(const char *pa);
int            dlcs_bin_to_dec(const char *pa);
long           dlcs_get_tickcount(void);
char const *   dlcs_timestamp(char const *x);
char *         dlcs_get_os_version(void);
int            dlcs_mkdir(const char *szDirectoryName);
int            dlcs_chdir(const char *szDirectory);
char *         dlcs_getcwd(void);
bool           dlcs_file_exists(const char *szFile);
bool           dlcs_isdir(char const *dir);
int            dlcs_strcasecmp(const char *szOne, const char *szTwo);
char *         dlcs_strreplace(char cold, char cnew, char *strg);
char *         dlcs_get_time(void);
char *         dlcs_convert_time(struct tm *);
string         dlcs_md5_digest(string str);
string         dlcs_md5_file(string file);
// char   *dlcs_getos(void);
#ifdef __cplusplus
}
#endif

// extern "C" CExplodeList *explode(const char *delimiter, char *text);

// #define SCREEN_WIDTH  800
// #define SCREEN_HEIGHT 600
// #define SCREEN_COLORS 32

// #define MAX_CHAT_BUFFER            1024

#endif  // _DLCS_LIBRARY
