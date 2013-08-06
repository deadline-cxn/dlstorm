/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/
#ifndef _DEADLINE_LIBRARY
#define _DEADLINE_LIBRARY
#include "dlstorm_lib.h"
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DL_INCLUDE_LOG
#include "c_log.h"
class CLog;
extern CLog* p_Log;
extern "C" void dLog(const char *format, ...);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
extern char _vx[1024];
extern "C" const char *va(const char *format, ...);
#ifdef __cplusplus
extern "C" {
#endif
vector <string> dlcs_explode(const string &delimiter, const string &explodeme);
vector  <string> dlcs_dir_to_vector(char *szDir, char *szWildCard);
bool	dlcs_dir_to_file(char *szDir,char *szFile,char *szWildCard);

void    dlcs_md5_digest(char *str, char *text); // md5 digest of the text
char*   dlcs_encrypt(char *text);
char*   dlcs_decrypt(char *text);
bool    dlcs_istrue(char *text);
void    dlcs_suspend_power_management(void);
int     dlcs_hex_to_dec(char *pa);
int     dlcs_bin_to_dec(char *pa);
long    dlcs_get_tickcount(void);
char*   dlcs_get_os_version(char *x);
char*   dlcs_get_hostname(char *x);
char*   dlcs_get_ipaddress(char *x);
bool    dlcs_isdir(char *dir);
int     dlcs_mkdir(char *szDirectoryName);
int     dlcs_chdir(char *szDirectory);
char*   dlcs_getcwd(char *x);
int     dlcs_strcasecmp(const char *szOne,const char *szTwo);
int     dlcs_str2mem(char *string, char &mem);
char*   dlcs_charreplace(char *str, char cold,char cnew);
char*   dlcs_strreplace(char *str, const char* what, const char* to);
char*   dlcs_get_time(char *x);
char*   dlcs_convert_time(char *x,struct tm*);
char*   dlcs_timestamp(char *x);
char*   dlcs_readable_timestamp(char *x,char *in);
char*   dlcs_get_filetype(char*x,char*in);
#ifdef __cplusplus
}
#endif

/* MD5 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org) based on: md5.h and md5.c
   reference implementation of RFC 1321
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.
License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.
License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.
RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.
A small class for calculating MD5 hashes of strings or byte arrays
it is not meant to be fast or secure
usage: 1) feed it blocks of uchars with update()
       2) finalize()
       3) get hexdigest() string
       or
       MD5(std::string).hexdigest()
assumes that char is 8 bit and int is 32 bit */

class MD5 {
public:
    typedef unsigned int size_type; // must be 32bit
    MD5();
    MD5(const std::string& text);
    void update(const unsigned char *buf, size_type length);
    void update(const char *buf, size_type length);
    MD5& finalize();
    std::string hexdigest() const;
    friend std::ostream& operator<<(std::ostream&, MD5 md5);
private:
    void init();
    typedef unsigned char uint1; //  8bit
    typedef unsigned int uint4;  // 32bit
    enum {blocksize = 64}; // VC6 won't eat a const static int here
    void transform(const uint1 block[blocksize]);
    static void decode(uint4 output[], const uint1 input[], size_type len);
    static void encode(uint1 output[], const uint4 input[], size_type len);
    bool finalized;
    uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
    uint4 count[2];   // 64bit counter for number of bits (lo, hi)
    uint4 state[4];   // digest so far
    uint1 digest[16]; // the result
    // low level logic operations
    static inline uint4 F(uint4 x, uint4 y, uint4 z);
    static inline uint4 G(uint4 x, uint4 y, uint4 z);
    static inline uint4 H(uint4 x, uint4 y, uint4 z);
    static inline uint4 I(uint4 x, uint4 y, uint4 z);
    static inline uint4 rotate_left(uint4 x, int n);
    static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};
std::string md5(const std::string str);

#endif // _DEADLINE_LIBRARY
