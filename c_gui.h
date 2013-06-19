/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_GUI
#define _EMBER_GUI

#include <map>
#include <algorithm>

#include "c_gfx.h"
#include "c_gaf.h"
#include "c_glfont.h"
#include "c_glmouse.h"
#include "c_gltexture.h"
#include "c_console.h"

#include "dlstorm.h"
#include "ximage.h"



using namespace std;

/**************************************************/

#define FM_GP_ALWAYS_ON_TOP      1
#define FM_GP_SIZEABLE           2
#define FM_GP_CLOSEX             4
#define FM_GP_MOVEABLE           8
#define FM_GP_MENU              16
#define FM_GP_USEBACKGROUND     32
#define FM_GP_BORDER            64
#define FM_GP_MINIMIZE         128
#define FM_GP_CLIENT_ONLY      256
#define FM_GP_ALWAYS_ON_BOTTOM 512
#define FM_GP_NO_RETURN       1024
#define FM_GP_DO_NOT_DIE	  2048
#define FM_GP_FULLSCREEN	  4096
#define FM_GP_TOGGLE_STATE	  8192

#define FM_GSM_ALPHA_ASCEND  1
#define FM_GSM_ALPHA_DESCEND 2

#define MAX_GUIS					255		// GUI objects
#define MAX_BUTTONS					128		// Button objects
#define MAX_FONTS                   12


// GUI system default buttons
/*
#define BC_ARROW_LEFT	3
#define BC_ARROW_RIGHT	4
#define BC_ARROW_UP		2
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
#define UP				1
*/

typedef void (*FMG_CALLBACK)(string whatgui);


/**************************************************/

enum tGUIComponentTypes
{
    FM_GC_BUTTON=1,
    FM_GC_SUBMIT,
    FM_GC_CANCEL,
    FM_GC_INPUT,
    FM_GC_TEXTBOX,
	FM_GC_PASSWORD,
    FM_GC_STATIC_TEXT,
    FM_GC_LISTBOX,
    FM_GC_3D_BOX,
	FM_GC_ACTION,
	FM_GC_ACTION_BAR,
    FM_GC_MESSAGE,
    FM_GC_CHATBOX,
    FM_GC_SELECT,
    FM_GC_TICKBOX,
    FM_GC_HIDDEN,
    FM_GC_MENU,
    FM_GC_DROPBOX,
    FM_GC_STATIC_IMAGE,
    FM_GC_REPORTER,
    FM_GC_GROUPTICK,
	FM_GC_TEXTBUFFER,
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



};

/**************************************************/

enum tGUINetworkMessages
{
    FM_GUI_PROGRAM_FINISH=0,
    FM_GUI_STUMP_CREATE,
    FM_GUI_STUMP_UPDATE,
    FM_GUI_STUMP_REMOVE,
    FM_GUI_STUMP_CAPTION,
    FM_GUI_CONTROL_CREATE,
    FM_GUI_CONTROL_UPDATE,
    FM_GUI_CONTROL_REMOVE,
    FM_GUI_CONTROL_VALUE,
    FM_GUI_CLEAR_ALL
};

/**************************************************/

/*
enum INMODE
{
    NORMAL=0,
    CONSOLE1,
    CONSOLE2
};
*/

class C_FM_GUI;
class C_FM_GUI_STUMP;

/**************************************************/

class C_FM_GUI_CTRL_DATA
{
public:
    C_FM_GUI_CTRL_DATA();
    C_FM_GUI_CTRL_DATA(CLog *pLog);
    ~C_FM_GUI_CTRL_DATA();

	char name[1024];
    char value[1024];
    bool selected;
    int key;

    C_FM_GUI_CTRL_DATA* next;
};

/**************************************************/

class C_FM_GUI_CONTROL
{
public:

    //C_FM_GUI_CONTROL();

    C_FM_GUI_CONTROL(C_FM_GUI_STUMP *pParentStump, CLog *pUSELog, C_GFX *pInGFX, C_FM_GUI *pInGUI, C_EM_CONSOLE *pInConsole);
    C_FM_GUI_CONTROL(C_FM_GUI_CONTROL *pInParentControl);

    ~C_FM_GUI_CONTROL();

    CLog            *pLog;
    C_GFX           *pGFX;
    C_FM_GUI        *pGUI;
    C_EM_CONSOLE    *pConsole;

    C_FM_GUI_STUMP   *parent_stump;
    C_FM_GUI_CONTROL *parent_control;

    C_FM_GUI_CONTROL *first_child_control;
    C_FM_GUI_CONTROL *child_control;

    C_FM_GUI_CTRL_DATA *first_control_data;
    C_FM_GUI_CTRL_DATA *control_data;

    C_FM_GUI_CONTROL *next;
    C_FM_GUI_CONTROL *prev;

    map <int, string> new_ctrl_data;

    //////////////////////////////////////
    // gui control data

    char  name[1024];
    char  type;
    RECT  rect;
	RECT  rect_2;
    char  media[1024];
	char  media_hover[1024];
	char  media_click[1024];
    char  value[1024];
    char  value_get[1024];
	bool  selected;
	int   listdepth;
	int   listoffset;
	char  font;
	char  fonthighlow;
	RECT  upbutton;
	RECT  downbutton;
    int   key;
    int   props;
    bool  visible;
	long  background_color;
	long  border_color;
	bool  selecting;
	char  group[1024];
	char  action[1024];
    char  console[1024];
    int   max_val_len;

	// functions

    void fm_gui_control_init(void);
    void set_value(const char *val);
	void selecta(void);
   // int  control_data_offset;
    int  control_data_total;
    void control_data_unselect(void);
    void clear_control_data(void);
	char *get_control_dataval(char *nname);
	C_FM_GUI_CTRL_DATA *get_control_data(char *name);
    void put_control_data(char *nname, char *nvalue);
    void put_control_data_behind(char *nvalue,char *cmp);
    void put_control_data_before(char *nvalue,char *cmp);
    void pop_control_data(char *ovalue);
    void pop_control_data(int okey);
    void sort_control_data(char method);
    void log_control_data(void);
    void attach_default_children(void);
    void draw(void);
};

/**************************************************/

class C_FM_GUI_STUMP
{
public:

    C_FM_GUI_STUMP(CLog *pInLog, C_GFX *pInGFX, C_FM_GUI *pInGUI, C_EM_CONSOLE *pInConsole);
    ~C_FM_GUI_STUMP();

    CLog            *pLog;
    C_GFX           *pGFX;
    C_FM_GUI        *pGUI;
    C_EM_CONSOLE    *pConsole;

    char stname[1024];
    char media[1024];
    char caption[256];
    char action[1024]; // indicates local action to take

    int key;
    int props;

    float   x,y;

    RECT rect;
    RECT mrect;

    bool moving;
    bool deleteme;

    C_FM_GUI_STUMP      *next;
    C_FM_GUI_STUMP      *prev;

    C_FM_GUI_CONTROL    *first_gui_control;
    C_FM_GUI_CONTROL    *gui_control;

    void set_caption(char *name);
    void set_action(char *act);

	void set_group_selecta(C_FM_GUI_CONTROL *sgc);

    char control_count(void);

    void add_control (const char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value);
    void add_control (C_FM_GUI_CONTROL *pInParentCtrl, const char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value);

    C_FM_GUI_CONTROL *get_control(const char *name);
    void size_control(char *name,int x,int y,int x2,int y2);
    void del_control (char *name);
    void pop_listbox (char *name,char *entryname,char *value);
    void rem_listbox (char *name,char *entryname);
    void clear_grouptick(char *name);

    void fm_gui_stump_init(void);

    void move_control_to_top(C_FM_GUI_CONTROL *gc);

};

/**************************************************/



class C_FM_GUI
{
public:
    C_FM_GUI();
    C_FM_GUI(CLog *pUSELog);
    C_FM_GUI(CGAF *pUSEGAF, CLog *pUSELog);
    C_FM_GUI(C_GFX *pUSEGFX,CGAF *pUSEGAF, CLog *pUSELog);

    ~C_FM_GUI();

    map<string, int> GC_TYPE;
    map<string, int> GC_PROP;

    map<SDLKey, char *> KeyMap;

    // INMODE              INPUTMODE;

    int                 stump_count;

    C_FM_GUI_STUMP      *focus_stump;
    C_FM_GUI_STUMP      *first_gui_stump;

    C_FM_GUI_CONTROL    *focus_control;

    C_FM_GUI_CTRL_DATA  *pGUIData;
    C_FM_GUI_CTRL_DATA  *pFirstGUIData;

    void                init(void);

    void                clear(void);
    void                cleardata(void);

    char                *getdata(char *);
    void                setdata(const char*, const char *);

    void                parse(char *text);

	bool				cab_loading;

	void				cab_call_process_file(char *file);
    void				cab_call(char *file);  // load a fm gui file
    void				cab_store(char *file); // save a fm gui to a file

	void				call_process_file(char *file);

    void				call(char *file);  // load a fm gui file
    void				store(char *file); // save a fm gui to a file

    void                bcall(char *file);
    void                bstore(char *file);

    void				add_stump(char *name,int x,int y,int x2,int y2,int props,char *media);
    void				mod_stump(char *name,int x,int y,int x2,int y2,int props,char *media);
    void				del_stump(C_FM_GUI_STUMP *tstump);
    void				del_stump(char *name);

    void                draw(void);
    void                draw_ctrls(void);

    CGLTextureList      *ButtonTexture;
    CGLTextureList	    *B9utton;
    CGLFontList         *font;

    void                DrawButton(int which, int updown, int x, int y);
    void                DrawButton(int which, int updown, int x, int y, int w, int h);

    void                DrawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,unsigned char r, unsigned char g, unsigned char b);
    void                DrawGUIButton(int x, int y,int whichslice,int size);
    void                DrawGUIResource(int w,int x,int y,int x2,int y2) {DrawGUIResourceC(w,x,y,x2,y2,255,255,255);}

    int                 DrawSlicedGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y); //int x, int y, int width);
    int                 DrawBaseGFXGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y);
    void                DrawB9utton(int x,int y, int w, int h);
    void                DrawMouse(int iX,int iY,int iMouseMode);
    void                DrawStatBar(int iX,int iY, int iW, int iT,int iV); // iT is total value, iV is current value, will draw percentage bar
    void                DrawCStatBar(int iX,int iY, int iW, int iT,int iV,long Color1,long Color2); // iT is total value, iV is current value, will draw percentage bar


    void                draw_3d_box(RECT rect);
    void                draw_3d_box(int x, int y, int x2, int y2);

    bool                InitFonts(void);

    bool                InitButtons(void);

    bool                FlashCursor(int iX, int iY);
	void                WriteTextPct(float fX, float fY,const char *szText,int fnt);
    void                WriteText(int iX,int iY,const char *szText,int font);
    void                WriteText(int iX,int iY,const char *szText,int font, int set);


    FMG_CALLBACK        _gcdatafill(string whatgui, string whatdata);

    bool                LoadFonts(void);
    bool                Load1Font(void);

    bool                LoadButtons(char *szFilename);


    bool                DestroyFonts(void);

    bool                DestroyButtons(void);

    void                DrawOSIcon(int iX,int iY,char *szOS);
    void                DrawPrompt(void);
    void                DrawFileXferStatus(void);
    void                DrawFPS(int iX,int iY);//,long color);

    void                Prompt(char *szPrompt, char *szCommand);
    bool                bIsPrompt(void);

    int                 DoInput(void);
    int                 ProcessKeyboard(void);

    C_FM_GUI_STUMP		*get_stump(char *name);
	C_FM_GUI_STUMP		*get_prev_stump(char *name);
	C_FM_GUI_STUMP		*get_prev_stump(C_FM_GUI_STUMP *tstump);

	char			    szPromptMsg[1024];
	char			    szCommand[1024];

    void                ClearKeys(void);
    bool                bSaveBinds(char *szFilename);

	bool                bShowFPS;			    		// On or Off show frames per second

	C_EM_CONSOLE        *pConsole;
	void                ConsoleEntry(char *fmt, ...);
	void                _ConsoleExecute	(char *cmd);

	CGAF                *pGAF;
	C_GFX               *pGFX;
	C_GLMouse           *pMouse;
	CLog                *pLog;


};

/**************************************************/

//int C_FM_GUI::*pgi_stump_count = &C_FM_GUI::stump_count;

// int C_FM_GUI::*pgi_stump_count = &C_FM_GUI::stump_count;
// Its type is "int (*)(char,float)" if an ordinary function
// Its type is "int (Fred::*)(char,float)" if a non-static member function of class Fred

// typedef ;
// typedef int (C_FM_GUI::*FredMemFn)(char x, float y);

#endif

