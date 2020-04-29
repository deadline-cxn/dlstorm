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
bool           dlcs_istrue(const char *szBoolCheck);
vector<string> dlcs_dir_to_vector(const char *szDir, const char *szWildCard);
// char   *dlcs_getos(void);
#ifdef __cplusplus
}
#endif

/*
TODO: ADD THIS STUFF:

char* DLCODESTORM::dlcs_get_filetype(char* x, char* in) {
    vector <string> ft;
    ft=dlcs_explode(".",in);
    ft.size();
    strcpy(x,ft[ft.size()].c_str());
    return x;
}

////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_tolower(string z) {
    dlcsm_make_filename(x);
    strcpy(x,z.c_str());
    int i=0;
    while (x[i]) { x[i] = tolower(x[i]); i++; }
    z.assign(x);
    return z;
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool DLCODESTORM::dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard) {
    FILE *fp;
    fp=fopen(szFile,"wt");
    if(!fp) return false;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(szDir);
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if(!((dlcs_strcasecmp(epdf->d_name,".")) || (dlcs_strcasecmp(epdf->d_name,".."))))
               if(!dlcs_isdir(epdf->d_name))
                fputs(epdf->d_name,fp);
        }
    }
    closedir(dpdf);
    fclose(fp);
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char* DLCODESTORM::dlcs_trim_lf(char* x) {
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
    if((x[strlen(x)-1]=='\n') || (x[strlen(x)-1]=='\r')) x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_md5_digest(string str) { // return a md5 digest of text
    return md5(str);
}
string DLCODESTORM::dlcs_md5_file(string file) { // return md5 digest of file
    string in_md5;
    string out_md5;
                        ////////
                                (int file_descript;
                                char* file_buffer;
                                file_descript = open(file.c_str(), O_RDONLY);
                                if(file_descript < 0) out_md5="(ERROR)";
                                else {
                                    struct stat statbuf;
                                    if(fstat(file_descript, &statbuf) < 0) out_md5="(ERROR)";
                                    else {
                                        file_buffer =

                                        // mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, file_descript, 0);

                                        for(i=0; i <MD5_DIGEST_LENGTH; i++) {
                                            printf("%02x",md[i]);
                                        }
                                        MD5((unsigned char*) file_buffer, file_size, result);
                                        print_md5_sum(result);
                                        // printf("  %s\n", argv[1]);
                                        }
                                    in_md5.assign(file_buffer)
                                    out_md5=md5(in_md5);
                                }
    //
    return out_md5;

}
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_encrypt(string text) { return text; } // TODO
////////////////////////////////////////////////////////////////////////////////////////////////
string DLCODESTORM::dlcs_decrypt(string text) { return (text); } // TODO
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_get_time(char *x) {
    struct tm *dc;
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_convert_time(char *x,struct tm* dc) {
    time_t td;
    time(&td);
    dc=localtime(&td);
    strcpy(x,asctime(dc));
    x[strlen(x)-1]=0;
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_timestamp(char *x) {
    time_t td;
    time( &td );
    strcpy(x,va("%ld",td));
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////
char *DLCODESTORM::dlcs_readable_timestamp(char *x,char *in) {
    long wtf;
    wtf=atoi(in);
    strcpy(x,ctime(&wtf));
    return x;
}


*/

// extern "C" CExplodeList *explode(const char *delimiter, char *text);
// #define SCREEN_WIDTH  800
// #define SCREEN_HEIGHT 600
// #define SCREEN_COLORS 32
// #define MAX_CHAT_BUFFER            1024

#endif  // _DLCS_LIBRARY
