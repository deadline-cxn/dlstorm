/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2013
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

Read_line_23_of_dlstorm_h

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

#include "pre/os.h"
#include "pre/cpu.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// DL Code Storm Library functions and definitions
namespace DLCODESTORM {
    const char *va(const char *format, ...);
    vector<string> dlcs_explode(const string &delimiter, const string &explodeme);
    vector<string> dlcs_dir_to_vector(char *szDir, char *szWildCard);
    bool    dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard);
    string  dlcs_md5_digest(string str);
    string  dlcs_md5_file(string file);
    string  dlcs_encrypt(string text);
    string  dlcs_decrypt(string text);
    bool    dlcs_istrue(string text);
    void    dlcs_suspend_power_management(void);
    int     dlcs_hex_to_dec(char *pa);
    int     dlcs_bin_to_dec(char *pa);
    long    dlcs_get_tickcount(void);
    string  dlcs_get_os_version(void);
    string  dlcs_get_hostname(void);
    string  dlcs_get_ipaddress(void);
    string  dlcs_tolower(string);
    bool    dlcs_isdir(char* dir);
    int     dlcs_get_filesize(string file);
    string  dlcs_filetype(string pathName);
    int     dlcs_count_words(string s,string x);

    int     dlcs_mkdir(char *szDirectoryName);
    int     dlcs_chdir(char *szDirectory);
    string  dlcs_getcwd(void);
    int     dlcs_strcasecmp(const char* szOne,const char* szTwo);
    char*   dlcs_trim_lf(char* x);
    char*   dlcs_charreplace(char* str, char cold,char cnew);
    char*   dlcs_strreplace(char* str, const char* what, const char* to);
    char*   dlcs_get_time(char* x);
    char*   dlcs_convert_time(char *x,struct tm*);
    char*   dlcs_timestamp(char* x);
    char*   dlcs_readable_timestamp(char* x,char* in);
    char*   dlcs_get_filetype(char* x,char* in);
    string  dlcs_get_webpage(string url);
    string  dlcs_dns_lookup(string hostname);
    string  dlcs_inet_aton(string ip);
    string  dlcs_inet_ntoa(string ip);
};


using namespace DLCODESTORM;

#endif // DLCS_DEADLINE_CODE_STORM_LIB

