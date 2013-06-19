
/**************************************************/

#ifndef _EMBER_CONSOLE

#define _EMBER_CONSOLE
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

#include "c_gltexture.h"
#include "c_log.h"
#include "c_gaf.h"
#include <map>

/**************************************************/

#define MAX_CONSOLE_BUFFER			1024	// Lines in console scrollback buffer
#define MAX_CONSOLE_MULTI_COMMANDS	32		// Number of console commands on a single line seperated by ;
#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again

    // b_  = bool
    // s_  = string
    // c_  = char
    // uc_ = unsigned char
    // i_  = int
    // ui_ = unsigned int
    // l_  = long
    // ul_ = unsigned long
    // f_  = float

enum ember_convar_types {
CVAR_NULL=0,
CVAR_BOOL,
CVAR_STRING,
CVAR_CHAR,
CVAR_UCHAR,
CVAR_INT,
CVAR_UINT,
CVAR_LONG,
CVAR_ULONG,
CVAR_FLOAT,
};

class C_EM_CONSOLE
{
public:
    C_EM_CONSOLE();
    C_EM_CONSOLE(CLog *pInLog);
    C_EM_CONSOLE(CGAF *pInGAF, CLog *pInLog);
    ~C_EM_CONSOLE();

    CLog                *pLog;
    CGAF                *pGAF;

    bool                Init(void);

    CGLTexture          *glConsoleTexture;

    //char			    ConsoleInput[1024];
	//int				    ConsoleYScroll;
	//int				    ConsoleBufferOffset;
    //int                 iConsoleCommandInputBufferOffset;

    void                ToggleConsole(void);
    void                Entry(char *fmt, ...);
    void                DrawConsoleBMP(int iX1,int iY1,int iX2,int iY2);
    bool                LoadConsoleTexture(CGAF *pGAF, char *szFilename);
    bool                DestroyConsoleTexture(void);
    void                ClearConsole(void);
	void                ConsoleScrollInput(void);
	void                draw_console(void);



    vector<string>      buf;

    void                _Execute(const char *cmd);

    int                 get_cvartype(const char *s);
    const char *       get_cvartype_string(int t);
    const char *       get_cvarformatted(const char *f,void *cv);
    map <string, int>   cvar_type_map;

    char *              get_cvarformat(int t);
    map <int, string>   cvar_type_format_map;

    void                _GlobalStrings(void);
    map <string, string> stringmap;

    void                _GlobalIntegers(void);
    map <string, int>    intmap;

    void                _GlobalFunctions(void);
    typedef void strfunc_t(const string&);
    typedef map<string, strfunc_t*> strfmap_t;
    strfmap_t funcmap;

    void                _GlobalVars(void);
    typedef void *strvar_t;
    typedef map<string, strvar_t> strvarmap_t;
    map <string, strvar_t>::iterator svm_i;
    strvarmap_t varmap;

};

#endif // _EMBER_CONSOLE
