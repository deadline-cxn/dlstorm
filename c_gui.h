/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#ifndef _C_GUI
#define _C_GUI

using namespace std;

#include "c_gui_chat_enum.h"
#include <map>
#include <algorithm>
#include "c_gfx.h"
#include "c_gaf.h"
#include "c_glfont.h"
#include "c_glmouse.h"
#include "c_gltexture.h"
#include "c_console.h"
#include "dlstorm.h"


#define FM_GP_ALWAYS_ON_TOP     1
#define FM_GP_SIZEABLE          2
#define FM_GP_CLOSEX            4
#define FM_GP_MOVEABLE          8
#define FM_GP_MENU              16
#define FM_GP_USEBACKGROUND     32
#define FM_GP_BORDER            64
#define FM_GP_MINIMIZE          128
#define FM_GP_SERVER_DATA       256
#define FM_GP_ALWAYS_ON_BOTTOM  512
#define FM_GP_NO_RETURN       	1024
#define FM_GP_DO_NOT_DIE	    2048
#define FM_GP_FULLSCREEN	    4096
#define FM_GP_TOGGLE_STATE	    8192
#define FM_GP_PERSIST           16384
#define FM_GP_FUTURE_1          32768
#define FM_GP_FUTURE_2          65536
#define FM_GSM_ALPHA_ASCEND  	1
#define FM_GSM_ALPHA_DESCEND 	2
#define MAX_BUTTONS		        128 // Button objects
#define MAX_FONTS               11
typedef void (*G_CBACK)(string whatgui);
enum eGUILOADMODE {
    FGL_ATTACH=0,
    FGL_GUMP,
    FGL_CTRL,
    FGL_CTRL_CHILD,
    FGL_PROPERTY
};
enum tGUIRelativeTo {
    GUI_NOT_RELATIVE,
    GUI_TOP_LEFT,
    GUI_TOP_CENTER,
    GUI_TOP_RIGHT,
    GUI_MIDDLE_LEFT,
    GUI_MIDDLE_CENTER,
    GUI_MIDDLE_RIGHT,
    GUI_BOTTOM_LEFT,
    GUI_BOTTOM_CENTER,
    GUI_BOTTOM_RIGHT
};
enum tGUIComponentTypes {
    FM_GC_BUTTON=1,
    FM_GC_SUBMIT,
    FM_GC_CANCEL,
    FM_GC_TEXTBOX,
    FM_GC_PASSWORD,
    FM_GC_STATIC_TEXT,
    FM_GC_LISTBOX, // Add methods to insert or remove data from listbox [NEW][DEL][MOVE UP][MOVE DOWN]
    FM_GC_3D_BOX,
    FM_GC_ACTION,
    FM_GC_MESSAGE,
    FM_GC_CHATBOX,
    FM_GC_TICKBOX,
    FM_GC_HIDDEN,
    FM_GC_MENU,
    FM_GC_DROPBOX,
    FM_GC_STATIC_IMAGE,
    FM_GC_REPORTER, // Reporter will copy one control value to another after x ticks
    FM_GC_GROUPTICK,
    FM_GC_SELECTA,
    FM_GC_SELECTA_PROPS,
    FM_GC_WILLY,
    FM_GC_LOCAL_DIR,
    FM_GC_REMOTE_DIR,
    FM_GC_H_SCROLLBAR,
    FM_GC_V_SCROLLBAR,
    FM_GC_POPUP_MENU,
    FM_GC_CONSOLE,
    FM_GC_FONT_SELECT,
    FM_GC_COLOR_PICKER,
    FM_GC_TAB_BOX,
    FM_GC_BASE_SELECT,
    FM_GC_CLICK_TO_CHANGE,
    FM_GC_FILTER,
    FM_GC_RECT_GROUP
};


class C_GUI;
class C_GSTMP;
class C_GCTRL;
class C_GCTRL {
public:
    C_GCTRL( C_GSTMP *pParentStump,CLog *pUSELog,C_GFX *pInGFX,C_GUI *pInGUI,C_CONS *pInConsole  );
    C_GCTRL(C_GCTRL *pInParentControl);
    ~C_GCTRL();

    // functions
    void init_ctrl(void);
    void zeroize_ctrl(void);
    void set_value(const char *val);
    void get_value(char *str);
    char *get_value(void);
    void selecta(void);

    void on_leftclick(void);
    void on_rightclick(void);
    void on_move(void);
    void on_size(void);
    void on_focus(void);
    void on_wheelup(void);
    void on_wheeldown(void);

    char *convert$(char *in);

    void populate_ctrl_editor(void);

    int  control_data_total;
    void control_data_unselect(void);
    void clear_control_data(void);
    void put_control_data(const char *nname, const char *nvalue);
    void sort_control_data(const char method);

    void set_property(char *nname, char *nvalue);

    void attach_default_children(void);
    void hide_default_children(void);
    void show_default_children(void);

    void draw(bool bMouseWheelUp, bool bMouseWheelDown);
    void checkMouseClicks(bool bMouseWheelUp, bool bMouseWheelDown);

    // members
    CLog     *pLog;
    C_GFX    *pGFX;
    C_GUI    *pGUI;
    C_CONS   *pCons;

    C_GSTMP *parent_stump;
    C_GCTRL  *parent_control;

    C_GCTRL *first_child_control;
    C_GCTRL *child_control;

    map <string, string> property;

    map <int, string>  data;
    map <int, bool>    data_selected;
    vector <string> buf;

    char  name[1024];
    char  type;
    RECT  rect;
    RECT  rect_2;
    RECT  mrect;
    //RECT  upbutton;
    //RECT  downbutton;

    int iRelativeTo;

    char  media[1024];
    char  media_hover[1024];
    char  media_click[1024];
    char  hover_text[1024];
    char  group[1024];
    char  action[1024];
    char  console[1024];
    char  datafill[1024];
    char  datafilltarget[1024];

    char  nexttabfocus[64];

    bool  selected;

    bool bMoving;
    bool bSizing;
    bool bEditing;

    int   listdepth;
    int   listoffset;
    char  font;
    char  fontbank;

    int   props;
    bool  visible;
    long  background_color;
    long  border_color;
    bool  selecting;

    int   max_val_len;

    C_GCTRL *next;
    C_GCTRL *prev;

private:
    char  value[1024];

};
class C_GSTMP {
public:
    C_GSTMP(CLog *pInLog, C_GFX *pInGFX, C_GUI *pInGUI, C_CONS *pInConsole );
    ~C_GSTMP();
    void    init_stmp(void);
    void    zeroize_stmp(void);

    char    control_count(void);
    void    add_control(const char *name, int type, int x, int y, int x2, int y2, int props, char *media, char *value);
    void    mod_control(const char *name, int type, int x, int y, int x2, int y2, int props, char *media, char *value);
    void    add_control(C_GCTRL *pInParentCtrl, const char *name, int type, int x, int y, int x2, int y2, int props, char *media, char *value);
    void    size_control(char *name,int x,int y,int x2,int y2);
    void    del_control (char *name);
    void    clear_grouptick(char *name);
    void    set_group_selecta(C_GCTRL *sgc);
    void    move_control_to_top(C_GCTRL *gc);
    void    store_stmp(void);
    void    store_stmp(char *filename);
    C_GCTRL *get_control(const char *name);

    map <string, string> data;
    CLog    *pLog;
    C_GFX   *pGFX;
    C_GUI   *pGUI;
    C_CONS  *pCons;
    char    name[1024];
    char    media[1024];
    char    caption[256];
    char    action[1024]; // indicates local action to take
    char    default_focus_control[256];
    char    gamemode[1024];
    int     iRelativeTo;
    int     props;
    float   x,y;
    RECT    rect;
    RECT    mrect;
    bool    bMoving;
    bool    bSizing;
    bool    bEditing;
    bool    bDeleteMe;
    C_GSTMP *next;
    C_GSTMP *prev;
    C_GCTRL *first_gui_control;
    C_GCTRL *gui_control;
    C_GCTRL *focus_control;

};
class C_GUI {
public:
    C_GUI();
    C_GUI(CLog *pUSELog);
    C_GUI(CGAF *pUSEGAF, CLog *pUSELog);
    C_GUI(C_GFX *pUSEGFX,CGAF *pUSEGAF, CLog *pUSELog);
    ~C_GUI();

//class C_GLAYER{public:    C_GLAYER(CLog *pInLog, C_GFX *pInGFX, C_GUI *pInGUI, C_CONS *pInConsole );    ~C_GLAYER();

    C_GSTMP *focus_stump;
    C_GSTMP *first_gui_stump;
    C_GCTRL *focus_control;

    void    clear(void); // clear

    void    setdata(char * stump, char* ctrl, char *value);
    char *  getdata(char * ctrlname);
    void    getdata(char * pString, char * ctrlname);
    int     getctrltype(char *);

    void    edit_stump(char *file);
    void    remove_control(char *stump, char *control);

    // GUI Creation / Delete functions

    void    set_focus_stump(C_GSTMP *pStump);
    void    set_focus_control(C_GCTRL *pCtrl);
    void    moveStumpToTop(C_GSTMP *pStump);
    void    add_stump(char *name,int x,int y,int x2,int y2,int props,char *media);
    void    mod_stump(char *name,int x,int y,int x2,int y2,int props,char *media);
    void    del_stump(C_GSTMP *tstump);
    void    del_stump(char *name);
    void    del_stumps(void);

    bool    isAStumpMoving(void);
    bool    isAControlMoving(void);
    bool    isAStumpSizing(void);
    bool    isAControlSizing(void);

    bool    bCtrlMoving;
    bool    bStmpMoving;
    bool    bCtrlSizing;
    bool    bStmpSizing;

    void    draw(void);
    void    draw_ctrls(void);

    C_GSTMP *get_stump(char *name);
    C_GSTMP *get_prev_stump(char *name);
    C_GSTMP *get_prev_stump(C_GSTMP *tstump);

    int     stump_count;

    // GUI Initialization / destruction functions

    void    init(void); //  initialize basic data
    bool    initFonts(void); // initialize fonts
    bool    initButtons(void); // initialize buttons
    bool    loadFonts(void);
    bool    load1Font(void);
    bool    loadButtons(char *szFilename);
    bool    destroyFonts(void);
    bool    destroyButtons(void);

    // G_CBACK _gcdatafill(string whatgui, string whatdata);
    // GUI Draw functions

    void    drawButton(int which, int updown, int x, int y);
    void    drawButton(int which, int updown, int x, int y, int w, int h);
    void    drawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,unsigned char r, unsigned char g, unsigned char b);
    void    drawGUIButton(int x, int y,int whichslice,int size);
    void    drawGUIResource(int w,int x,int y,int x2,int y2);
    int     drawSlicedGUIButton(C_GCTRL *gui_control,int x,int y); //int x, int y, int width);
    int     DrawTextureGUIButton(C_GCTRL *gui_control,int x,int y);
    void    drawB9utton(int x,int y, int w, int h);
    void    drawMouse(int iX,int iY,int iMouseMode);
    void    drawStatBar(int iX,int iY, int iW, int iT,int iV); // iT is total value, iV is current value, will draw percentage bar
    void    drawCStatBar(int iX,int iY, int iW, int iT,int iV,long Color1,long Color2); // iT is total value, iV is current value, will draw percentage bar
    bool    flashCursor(int iX, int iY);
    void    gPrintPct(float fX, float fY,const char *szText,int fnt);
    void    gPrint(int iX,int iY,const char *szText,const char *fnt);
    void    gPrint(int iX,int iY,const char *szText,const char *fnt,int wset);
    void    gPrint(int iX,int iY,const char *szText,int font);
    void    gPrint(int iX,int iY,const char *szText,int font, int wset);
    void    drawOSIcon(int iX,int iY,char *szOS);
    void    drawPrompt(void);
    void    drawFileXferStatus(void);
    void    drawFPS(int iX,int iY);//,long color);
    void    drawCPUSTRING(int iX, int iY);
    void    prompt(char *szPrompt, char *szCommand);
    bool    bIsPrompt(void);
    void    drawToolTip(void);

    // GUI Load / Store functions

    bool	cab_loading;
    //void	cab_call_process_file(char *file);
    void	cab_call(char *file);  // load a fm gui file
    void	cab_store(char *file); // save a fm gui to a file
    void	call_do_line(char *line);
    void	call(char *file);  // load a fm gui file
    void    call_file(char *file);
    void	store(void); // save all stumps
    void	store(char *name); // save a fm gui to a file
    void    bcall(char *file);
    void    bstore(char *file);

    // GUI Keyboard / Mouse functions

    int     doInput(void);
    int     processKeyboard(void);
    void    checkMouseClicks(void);
    void    clearKeys(void);
    bool    bSaveBinds(char *szFilename);

    // GUI members

    char	szPromptMsg[1024];
    bool    bClosePrompt;
    char	szCommand[1024];
    bool    bShowFPS;  // Show frames per second on or off
    bool    bMadeLog;  // Made the log file on or off

    CGAF    *pGAF;
    C_GFX   *pGFX;
    CLog    *pLog;
    C_Mouse *pMouse;

    map<string, int>    GC_TYPE;
    map<string, int>    GC_PROP;
    map<string, int>    GC_RELATIVE;


    map<SDLKey, char *> KeyMap;
    map<SDLKey, bool>   KeyDown;
    long KeyRepeatTimer;

    CGLTextureList      *ButtonTexture;
    CGLTextureList	    *B9utton;


    CGLFontList*    font;

    CGLFont*            pFirstFont;
    CGLFont*            GetFont(char* szWhichFont);
    CGLFont*            GetFont(int iWhich);

    bool    bStaticToolTip;
    bool    bDrawToolTip;
    bool    bResetToolTip;
    RECT    rectToolTip;
    char    hover_text[1024];
    bool    bDebug;

    C_GCTRL *last_control_clicked;

    C_GCTRL *highest_control_clicked;
    C_GSTMP *highest_stump_clicked;

    // GUI Console stuff

    C_CONS  *pCons;
    void    consEntry(char *fmt, ...);
    void _consExecute(char *cmd);

    // CHAT System stuff

    void    addChat(int channel, char * user,  char * msg);

    map<string, int>    GUI_CHAT;
    map<int, string>    GUI_CHAT_COLOR;

};
#endif //_C_GUI

/*  GUI system default buttons
#define BC_ARROW_LEFT	3
#define BC_ARROW_RIGHT	4
#define BC_ARROW_UP	2
#define BC_ARROW_DOWN	1
#define BC_SCROLLBAR_UD 5
#define BC_SCROLL_SLIDE 6
#define BC_TICKBOX      7
#define BC_FREEBSD      8
#define BC_LINUX        9
#define BC_WIN32        10
#define BC_SCROLLBAR_LR 11
#define BC_NEXT			12
#define BC_OK			13
#define BC_CANCEL		14
#define BC_BACK			15
#define BC_SAVE			17
#define BC_BLANK_BUTTON 18
#define BC_MINUS		19
#define BC_PLUS			20
#define BC_VERTEX       21
#define BC_CLOSE        22
#define BC_MINIMIZE     23
#define BC_DISCARD		24
#define DOWN			0
#define UP				1 */
/*
void C_CONS::_GlobalIntegers(){
    intmap.clear();




    intmap["f1"]    = SDLK_F1;
    intmap["f2"]    = SDLK_F2;
    intmap["f3"]    = SDLK_F3;
    intmap["f4"]    = SDLK_F4;
    intmap["f5"]    = SDLK_F5;
    intmap["f6"]    = SDLK_F6;
    intmap["f7"]    = SDLK_F7;
    intmap["f8"]    = SDLK_F8;
    intmap["f9"]    = SDLK_F9;
    intmap["f10"]   = SDLK_F10;
    intmap["f11"]   = SDLK_F11;
    intmap["f12"]   = SDLK_F12;
    intmap["f13"]   = SDLK_F13;
    intmap["f14"]   = SDLK_F14;
    intmap["f15"]   = SDLK_F15;

    intmap["numlock"]   = SDLK_NUMLOCK;
    intmap["scroll"]    = SDLK_SCROLLOCK;
    intmap["numpad_7"]  = SDLK_KP7;
    intmap["numpad_8"]  = SDLK_KP8;
    intmap["numpad_9"]  = SDLK_KP9;
    intmap["-"]         = SDLK_KP_MINUS;
    intmap["numpad_4"]  = SDLK_KP4;
    intmap["numpad_5"]  = SDLK_KP5;
    intmap["numpad_6"]  = SDLK_KP6;
    intmap["+"]         = SDLK_KP_PLUS;
    intmap["numpad_1"]  = SDLK_KP1;
    intmap["numpad_2"]  = SDLK_KP2;
    intmap["numpad_3"]  = SDLK_KP3;
    intmap["numpad_0"]  = SDLK_KP0;

    intmap["numpad_decimal"] = SDLK_KP_PERIOD;
//    intmap["oem_102"]   = SDLK_OEM_102;

    intmap["esc"] = SDLK_ESCAPE;
    intmap["1"] = SDLK_1;
    intmap["2"] = SDLK_2;
    intmap["3"] = SDLK_3;
    intmap["4"] = SDLK_4;
    intmap["5"] = SDLK_5;
    intmap["6"] = SDLK_6;
    intmap["7"] = SDLK_7;
    intmap["8"] = SDLK_8;
    intmap["9"] = SDLK_9;
    intmap["0"] = SDLK_0;
    intmap["-"] = SDLK_MINUS;
    intmap["="] = SDLK_EQUALS;
    intmap["backspace"] = SDLK_BACKSPACE;
    intmap["tab"] = SDLK_TAB;
    intmap["q"] = SDLK_q;
    intmap["w"] = SDLK_w;
    intmap["e"] = SDLK_e;
    intmap["r"] = SDLK_r;
    intmap["t"] = SDLK_t;
    intmap["y"] = SDLK_y;
    intmap["u"] = SDLK_u;
    intmap["i"] = SDLK_i;
    intmap["o"] = SDLK_o;
    intmap["p"] = SDLK_p;
    intmap["["] = SDLK_LEFTBRACKET;
    intmap["]"] = SDLK_RIGHTBRACKET;
    intmap["enter"] = SDLK_RETURN;
    intmap["l_control"] = SDLK_LCTRL;
    intmap["a"] = SDLK_a;
    intmap["s"] = SDLK_s;
    intmap["d"] = SDLK_d;
    intmap["f"] = SDLK_f;
    intmap["g"] = SDLK_g;
    intmap["h"] = SDLK_h;
    intmap["j"] = SDLK_j;
    intmap["k"] = SDLK_k;
    intmap["l"] = SDLK_l;
    intmap[";"] = SDLK_SEMICOLON;
    intmap["'"] = SDLK_QUOTE;
    intmap["`"] = SDLK_BACKQUOTE;
    intmap["l_shift"] = SDLK_LSHIFT;
    intmap["\\"] = SDLK_BACKSLASH;
    intmap["z"] = SDLK_z;
    intmap["x"] = SDLK_x;
    intmap["c"] = SDLK_c;
    intmap["v"] = SDLK_v;
    intmap["b"] = SDLK_b;
    intmap["n"] = SDLK_n;
    intmap["m"] = SDLK_m;
    intmap[","] = SDLK_COMMA;
    intmap["."] = SDLK_PERIOD;
    intmap["/"] = SDLK_SLASH;
    intmap["r_shift"] = SDLK_RSHIFT;
    intmap["*"] = SDLK_KP_MULTIPLY;
    intmap["l_alt"] = SDLK_LALT;
    intmap["space"] = SDLK_SPACE;
    intmap["capslock"] = SDLK_CAPSLOCK;
//    intmap["kana"] = SDLK_KANA;
//    intmap["abnt_c1"] = SDLK_ABNT_C1;
//    intmap["convert"] = SDLK_CONVERT;
//    intmap["noconvert"] = SDLK_NOCONVERT;
//    intmap["yen"] = SDLK_YEN;
//    intmap["abnt_c2"] = SDLK_ABNT_C2; // Numpad . on Portugese (Brazilian) keyboards
    intmap["numpad_equals"] = SDLK_KP_EQUALS;
//    intmap["circumflex"] = SDLK_CIRCUMFLEX;
    intmap["at"] = SDLK_AT;
    intmap["colon"] = SDLK_COLON;
    intmap["_"] = SDLK_UNDERSCORE;
//    intmap["kanji"] = SDLK_KANJI;
//    intmap["stop"] = SDLK_STOP;
//    intmap["ax"] = SDLK_AX;
//    intmap["unlabeled"] = SDLK_UNLABELED;
//    intmap["nexttrack"] = SDLK_NEXTTRACK; // Next Track Key?
    intmap["numpad_enter"] = SDLK_KP_ENTER;
    intmap["r_control"] = SDLK_RCTRL;
//    intmap["mute"] = SDLK_MUTE; // Mute Key?
//    intmap["calculator"] = SDLK_CALCULATOR; // Calculator Key?
//    intmap["playpause"] = SDLK_PLAYPAUSE; // Play / Pause Key?
//    intmap["mediastop"] = SDLK_MEDIASTOP; // Media Stop Key?
//    intmap["volumedown"] = SDLK_VOLUMEDOWN; // Volume - Key?
//    intmap["volumeup"] = SDLK_VOLUMEUP; // Volume + Key?
//    intmap["webhome"] = SDLK_WEBHOME; // Web home Key?
//    intmap["numpad_comma"] = SDLK_NUMPADCOMMA;
    intmap["/"] = SDLK_KP_DIVIDE;
//    intmap["sysrq"] = SDLK_SYSRQ;
    intmap["r_alt"] = SDLK_RALT;
    intmap["pause"] = SDLK_PAUSE;
    intmap["home"] = SDLK_HOME;
    intmap["up"] = SDLK_UP;
    intmap["pageup"] = SDLK_PAGEUP;
    intmap["left"] = SDLK_LEFT;
    intmap["right"] = SDLK_RIGHT;
    intmap["end"] = SDLK_END;
    intmap["down"] = SDLK_DOWN;
    intmap["pagedown"] = SDLK_PAGEDOWN;
    intmap["insert"] = SDLK_INSERT;
    intmap["delete"] = SDLK_DELETE;
    intmap["l_windows"] = SDLK_LSUPER;
    intmap["r_windows"] = SDLK_RSUPER;
//    intmap["start"] = SDLK_START;
    intmap["power"] = SDLK_POWER;
//    intmap["sleep"] = SDLK_SLEEP;
//    intmap["wake"] = SDLK_WAKE;
//    intmap["websearch"] = SDLK_WEBSEARCH;
//    intmap["webfavorites"] = SDLK_WEBFAVORITES;
//    intmap["webrefresh"] = SDLK_WEBREFRESH;
//    intmap["webstop"] = SDLK_WEBSTOP;
//    intmap["webforward"] = SDLK_WEBFORWARD;
//    intmap["webback"] = SDLK_WEBBACK;
//    intmap["mycomputer"] = SDLK_MYCOMPUTER;
//    intmap["mail"] = SDLK_MAIL;
//    intmap["mediaselect"] = SDLK_MEDIASELECT;
}


*/

//G_CBACK C_GUI::_gcdatafill(string whatgui, string whatdata){//    pLog->_Add("_gcdatafill whatgui[%s] whatdata[%s]",(char *)whatgui.c_str(),(char *)whatdata.c_str());}
