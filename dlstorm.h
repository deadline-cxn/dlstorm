/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2017
 **   File:         dlstorm.h
 **   Description:  DLCODESTORM Namespace, where the magic happens
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef DLCS_DEADLINE_CODE_STORM_LIB
#define DLCS_DEADLINE_CODE_STORM_LIB

#if !defined(_DLCS_CONSOLE) && !defined(_DLCS_SDL)

Read_line_24_of_dlstorm_h

Deliberate syntax error...
You have to define what type of application.

Current application types
_DLCS_CONSOLE
OR
_DLCS_SDL

Additionally, you must define the following
_DLCS_OPENGL
_DLCS_DEVIL

#endif // DEFINE TEST

#ifdef _DLCS_DEBUG
#define _DLCS_DEBUG_GUI 1
#define _DLCS_DEBUG_LOG
#define _DLCS_DEBUG_GFX 1
#define _DLCS_DEBUG_SND 1
#endif

#include "os.h"
#include "cpu.h"


// Other common defines

#define MAX_TEXT_LEN 1024

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// DL Code Storm Library functions and definitions
namespace DLCODESTORM {
    const char *va(const char *format, ...);
    vector<string> dlcs_explode(const string &delimiter, const string &explodeme);
    vector<string> dlcs_dir_to_vector(char const * szDir, char const * szWildCard);
    bool    dlcs_dir_to_file(char const * szDir,char const * szFile,char const * szWildCard);
    string  dlcs_md5_digest(string str);
    string  dlcs_md5_file(string file);
    string  dlcs_encrypt(string text);
    string  dlcs_decrypt(string text);
    bool    dlcs_istrue(string text);
    void    dlcs_suspend_power_management(void);
    int     dlcs_hex_to_dec(char const * pa);
    int     dlcs_bin_to_dec(char const * pa);
    long    dlcs_get_tickcount(void);
    string  dlcs_get_os_version(void);
    string  dlcs_get_hostname(void);
    string  dlcs_get_ipaddress(void);
    string  dlcs_tolower(string);
    bool    dlcs_isdir(char const * dir);
    int     dlcs_get_filesize(string file);
    string  dlcs_filetype(string pathName);
    int     dlcs_count_words(string s,string x);

    int     dlcs_mkdir(char const * szDirectoryName);
    int     dlcs_chdir(char const * szDirectory);
    string  dlcs_getcwd(void);
    int     dlcs_strcasecmp(char const * szOne,char const * szTwo);
    char * dlcs_trim_lf(char * x);
    char*   dlcs_charreplace(char const * str, char cold,char cnew);
    char*   dlcs_strreplace(char const * str, char const * what, char const * to);
    char const * dlcs_get_time(char const * x);
    char const * dlcs_convert_time(char const *x,struct tm*);
    char const * dlcs_timestamp(char const * x);
    char const * dlcs_readable_timestamp(char const * x,char const * in);
    char const * dlcs_get_filetype(char const * x,char const * in);
    string  dlcs_get_webpage(string url);
    void    dlcs_get_webpage2(string url,char const * x);
    string  dlcs_dns_lookup(string hostname);
    string  dlcs_inet_aton(string ip);
    string  dlcs_inet_ntoa(string ip);
};


using namespace DLCODESTORM;

#endif // DLCS_DEADLINE_CODE_STORM_LIB

