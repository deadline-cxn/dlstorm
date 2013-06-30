/********************************************************/

#include "c_gui.h"

/********************************************************/

C_FM_GUI_CTRL_DATA::C_FM_GUI_CTRL_DATA()
{
	memset(name,0,1024);
	memset(value,0,1024);
	key=0;
	selected=0;
	next=0;
}

/********************************************************/

C_FM_GUI_CTRL_DATA::~C_FM_GUI_CTRL_DATA()
{
}

/********************************************************/

C_FM_GUI_CONTROL::C_FM_GUI_CONTROL(C_FM_GUI_STUMP *pInParentStump, CLog *pInLog, C_GFX *pInGFX, C_FM_GUI *pInGUI, C_EM_CONSOLE *pInConsole)
{
    parent_stump=pInParentStump;
    pLog=pInLog;
    pGFX=pInGFX;
    pGUI=pInGUI;
    pConsole=pInConsole;
    fm_gui_control_init();
}

C_FM_GUI_CONTROL::C_FM_GUI_CONTROL(C_FM_GUI_CONTROL *pInParent)
{
    parent_control=pInParent;
    parent_stump=parent_control->parent_stump;
    pLog=parent_control->pLog;
    pGFX=parent_control->pGFX;
    pGUI=parent_control->pGUI;
    pConsole=parent_control->pConsole;
    fm_gui_control_init();
}

/********************************************************/

//C_FM_GUI_CONTROL::C_FM_GUI_CONTROL()
//{
//    fm_gui_control_init();
//}

/********************************************************/

void C_FM_GUI_CONTROL::fm_gui_control_init()
{
    props=0;
	selected=0;
	memset(name,0,1024);
	memset(value,0,1024);
	memset(value_get,0,1024);
	memset(group,0,1024);
	memset(console,0,1024);
    max_val_len=256;
    listdepth=10;
    next=NULL;
    prev=NULL;

    font=0;
    fonthighlow=1;

    new_ctrl_data.clear();

    first_control_data=0;
    control_data=0;
//    control_data_offset=0;
    control_data_total=0;
    visible=1;
    parent_control=0;
    first_child_control=0;
    child_control=0;
}

/********************************************************/

C_FM_GUI_CONTROL::~C_FM_GUI_CONTROL()
{

	clear_control_data();

	C_FM_GUI_CONTROL *dc=first_child_control;
	child_control=dc;
	while(child_control)
	{
        dc=child_control;
        child_control=child_control->next;
        DEL(dc);
	}

}

/********************************************************/

void C_FM_GUI_CONTROL::set_value(const char *val)
{
	strcpy(value,val);
}

/********************************************************/

void C_FM_GUI_CONTROL::clear_control_data(void)
{
    C_FM_GUI_CTRL_DATA *dcd=0;
    control_data=first_control_data;
    while(control_data)
    {
        dcd=control_data;
        control_data=control_data->next;
        DEL(dcd);
    }
    first_control_data=0;
//    control_data_offset=0;
    control_data_total=0;
}

/********************************************************/

void C_FM_GUI_CONTROL::control_data_unselect(void)
{
    C_FM_GUI_CTRL_DATA *gcd=0;

    gcd=first_control_data;
    while(gcd)
    {
        gcd->selected=false;
        gcd=gcd->next;
    }

}

/********************************************************/

void C_FM_GUI_CONTROL::log_control_data(void)
{
    control_data=first_control_data;
    while(control_data)
    {
        control_data=control_data->next;
    }
}

void C_FM_GUI_CONTROL::selecta(void)
{
	this->parent_stump->set_group_selecta(this);
}

/********************************************************/

char *C_FM_GUI_CONTROL::get_control_dataval(char *nname)
{
	control_data=first_control_data;
	while(control_data)
	{
		if( dscc(control_data->name, nname) )
			return  strdup(control_data->value);

		control_data=control_data->next;
	}
	return NULL;
}

/********************************************************/

C_FM_GUI_CTRL_DATA *C_FM_GUI_CONTROL::get_control_data(char *name)
{
	control_data=first_control_data;
	while(control_data)
	{
		if(dscc(control_data->name,name)) return control_data;
		control_data=control_data->next;
	}
	return NULL;
}

/********************************************************/

void C_FM_GUI_CONTROL::put_control_data(char *tnname, char *nvalue)
{
    control_data=first_control_data;
    if(control_data)
    {
        while(control_data->next)
        {
            control_data=control_data->next;
        }
        control_data->next=new C_FM_GUI_CTRL_DATA;
        if(!control_data->next) return;
        control_data=control_data->next;
    }
    else
    {
        control_data=new C_FM_GUI_CTRL_DATA;
        if(!control_data) return;
        first_control_data=control_data;
    }
	strcpy(control_data->name,tnname);
    strcpy(control_data->value,nvalue);
    control_data_total++;
    control_data->key=control_data_total;
}

/********************************************************/

void C_FM_GUI_CONTROL::put_control_data_behind(char *nvalue,char *cmp)
{
	control_data_total++;
}

/********************************************************/

void C_FM_GUI_CONTROL::put_control_data_before(char *nvalue,char *cmp)
{
	control_data_total++;
}

/********************************************************/

void C_FM_GUI_CONTROL::pop_control_data(char *ovalue)
{
	control_data_total--;
}

/********************************************************/

void C_FM_GUI_CONTROL::pop_control_data(int okey)
{
	control_data_total--;
}

/********************************************************/

void C_FM_GUI_CONTROL::sort_control_data(char method)
{
    C_FM_GUI_CTRL_DATA *a;
    C_FM_GUI_CTRL_DATA *b;
    char temp[1024];
    int i=0;
    int j=0;

    switch(method)
    {
        case FM_GSM_ALPHA_ASCEND:
            //pLog->_Add("Sort control_data ascending...");

            for(i=0;i<control_data_total;i++)
            {
                control_data=first_control_data;
                j=0;
                while(control_data)
                {
                    j++;
                    a=control_data;
                    b=control_data->next;
                    if(b)
                    {
                        if(a->value[0]>b->value[0])
                        {
                            strcpy(temp,a->value);
                            strcpy(a->value,b->value);
                            strcpy(b->value,temp);
                        }
                    }
                    control_data->key=j;
                    control_data=control_data->next;
                }
            }
            control_data_total=j;
            break;
        case FM_GSM_ALPHA_DESCEND:
            break;
        default:
            break;
    }
}

/********************************************************/

C_FM_GUI_STUMP::C_FM_GUI_STUMP(CLog *pInLog, C_GFX *pInGFX, C_FM_GUI *pInGUI, C_EM_CONSOLE *pInConsole)
{
    pLog=pInLog;
    pGFX=pInGFX;
    pGUI=pInGUI;
    pConsole=pInConsole;
    next=0;
    prev=0;

	fm_gui_stump_init();
}

/********************************************************/

C_FM_GUI_STUMP::~C_FM_GUI_STUMP()
{
    C_FM_GUI_CONTROL *del_gui_control;
    gui_control=first_gui_control;
    while(gui_control)
    {
        del_gui_control=gui_control;
        gui_control=gui_control->next;
        DEL(del_gui_control);
    }
    DEL(del_gui_control);
}

/********************************************************/

void C_FM_GUI_STUMP::fm_gui_stump_init()
{
    memset(media,0,1024);
    memset(stname,0,1024);
    strcpy(caption,"FM GUI system window");
    memset(action,0,1024);
    props=0;

    gui_control=NULL;
    first_gui_control=NULL;
	next=NULL;
}

/********************************************************/

void C_FM_GUI_STUMP::set_action(char *act)
{
	strcpy(action,act);
}

/********************************************************/

void C_FM_GUI_STUMP::set_group_selecta(C_FM_GUI_CONTROL *sgc)
{
	C_FM_GUI_CONTROL *tgc;
	tgc=first_gui_control;
	while(tgc)
	{
		if(dscc(sgc->group,tgc->group))
			tgc->selected=0;
		tgc=tgc->next;
	}
	sgc->selected=1;
}

/********************************************************/

char C_FM_GUI_STUMP::control_count(void)
{
    C_FM_GUI_CONTROL *tctrl;
    char x=0;
    tctrl=first_gui_control;
    while(tctrl)
    {
        x++;
        tctrl=tctrl->next;
    }
    return x;
}

/********************************************************/

void C_FM_GUI_STUMP::add_control(const char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value)
{
    add_control(0,name,type,x,y,x2,y2,props,media,value);
}

void C_FM_GUI_STUMP::add_control(C_FM_GUI_CONTROL *pInParentCTRL, const char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value)
{
    C_FM_GUI_CONTROL *ngc;

    if(get_control(name)) return;

    if(pInParentCTRL)
    {
        ngc=pInParentCTRL->first_child_control;
        if(ngc)
        {
            while(ngc->next)
                ngc=ngc->next;

            ngc->next = new C_FM_GUI_CONTROL(pInParentCTRL);
            ngc->next->prev=ngc;
            ngc=ngc->next;
        }
        else
        {
            ngc = new C_FM_GUI_CONTROL(pInParentCTRL);
            pInParentCTRL->first_child_control=ngc;
        }

    }
    else
    {
        ngc=first_gui_control;
        if(ngc)
        {
            while(ngc->next)
                ngc=ngc->next;

            ngc->next = new C_FM_GUI_CONTROL(this,pLog,pGFX,pGUI,pConsole);
            ngc->next->prev=ngc;
            ngc=ngc->next;
        }
        else
        {
            ngc = new C_FM_GUI_CONTROL(this,pLog,pGFX,pGUI,pConsole);
            first_gui_control=ngc;
        }
    }

    if(!ngc) return;

    ngc->parent_stump=this;
    ngc->parent_control=pInParentCTRL;

    strcpy(ngc->name,name);

    ngc->type=type;
    ngc->rect.left=x;
    ngc->rect.top=y;

    if(type==FM_GC_TICKBOX)   { x2=16; y2=16; }
    if(type==FM_GC_GROUPTICK) { x2=16; y2=16; }
    if( (type==FM_GC_TEXTBOX) || (type==FM_GC_PASSWORD) )
    {
        if(x2==0) x2=100;
        if(y2==0) y2=15;
    }

    if(x2==0) x2=100;
    if(y2==0) y2=15;

    ngc->rect.right=x2;
    ngc->rect.bottom=y2;

    strcpy(ngc->media,media);
    strcpy(ngc->value,value);
    ngc->props=props;
    if(type==FM_GC_HIDDEN)
        ngc->visible=false;


}

/********************************************************/

C_FM_GUI_CONTROL *C_FM_GUI_STUMP::get_control(const char *name)
{
    C_FM_GUI_CONTROL *childctl;
    gui_control=first_gui_control;
    while(gui_control)
    {
        if(dscc(name,gui_control->name))
            return gui_control;

        childctl=gui_control->first_child_control;
        while(childctl)
        {
            if(dscc(name,childctl->name))
                return childctl;
            childctl=childctl->next;
        }

        gui_control=gui_control->next;
    }
    return gui_control;
}

/********************************************************/

void C_FM_GUI_STUMP::size_control(char *name,int x,int y,int x2,int y2)
{
    gui_control=first_gui_control;
    while(gui_control)
    {
        if(dscc(gui_control->name,name))
        {
            gui_control->rect.left=x;
            gui_control->rect.top=y;
            gui_control->rect.right=x2;
            gui_control->rect.bottom=y2;
        }
        gui_control=gui_control->next;
    }
}
/********************************************************/

void C_FM_GUI_STUMP::del_control(char *name)
{

}

/********************************************************/

void C_FM_GUI_STUMP::pop_listbox(char *name,char *entryname,char *value)
{

}

/********************************************************/

void C_FM_GUI_STUMP::rem_listbox(char *name,char *entryname)
{

}

/********************************************************/

void C_FM_GUI_STUMP::set_caption(char *cap)
{
	strcpy(caption,cap);
}

/********************************************************/

void C_FM_GUI_STUMP::clear_grouptick(char *grp)
{
    C_FM_GUI_CONTROL *cg=0;
    cg=first_gui_control;

    char tmp[1024]; memset(tmp,0,1024);
    strcpy(tmp,grp);
    char *tok=strtok(tmp,"0123456789\n");
    if(tok)
    {
        while(cg)
        {
            if(!strncmp(tok,cg->name,strlen(tok)))
            {
                strcpy(cg->value,"off");
            }
            cg=cg->next;
        }
    }
}

/********************************************************/

void C_FM_GUI_STUMP::move_control_to_top(C_FM_GUI_CONTROL *gc)
{
//          ______     ______
//         n      \   n      \  n0

//       A          B          C

//    0p  \_______p   \______p
/*


    C_FM_GUI_CONTROL *tgc=0;

    tgc=first_gui_control;
    while(tgc)
    {
        if(tgc==gc)
        {
            if(tgc->prev)
            {
                tgc->prev->next=tgc->next;

            }
            if(tgc==first_gui_control)
            {
                first_gui_control=tgc->next;
                tgc->next->prev=0;
            }
            tgc=tgc->next;
            gc->next=0;
            gc->prev=0;
        }

        else
            tgc=tgc->next;
    }

    tgc=first_gui_control;

    while(tgc)
    {
        if(!tgc->next)
        {
            tgc->next=gc;
            gc->prev=tgc;
            gc->next=0;
            return;
        }
        tgc=tgc->next;
    }*/

}

/********************************************************/

C_FM_GUI::C_FM_GUI()
{
    pLog=new CLog("gui.log");
    pLog->On();
    pLog->LineFeedsOff();
    pLog->Restart();
    pLog->AddLineSep();
    init();
}


/********************************************************/

C_FM_GUI::C_FM_GUI(CLog *pinLog)
{
    pLog=pinLog;
    init();
    cab_loading=false;
}

/********************************************************/

C_FM_GUI::C_FM_GUI(CGAF *pInGAF, CLog *pinLog)
{
    pGAF=pInGAF;
    pLog=pinLog;
    init();
    cab_loading=true;
}

/********************************************************/

C_FM_GUI::C_FM_GUI(C_GFX *pInGFX, CGAF *pInGAF, CLog *pinLog)
{
    pGFX=pInGFX;
    pGAF=pInGAF;
    pLog=pinLog;
    pLog->_Add("C_FM_GUI:constructor");
    init();
    cab_loading=true;
}

/********************************************************/

C_FM_GUI::~C_FM_GUI()
{
    DEL(pConsole);
    DEL(pMouse);

    clear();

    DestroyFonts();
    DestroyButtons();

	pLog->_Add("Deleted GUI");
}

/********************************************************/

void C_FM_GUI::init(void)
{

    pLog->_Add("C_FM_GUI::init()");

    GC_TYPE.clear();

    pLog->_Add("C_FM_GUI::init() > GC_TYPE.clear() ok");

    GC_TYPE["FM_GC_BUTTON"]       = FM_GC_BUTTON;
    GC_TYPE["FM_GC_SUBMIT"]       = FM_GC_SUBMIT;
    GC_TYPE["FM_GC_CANCEL"]       = FM_GC_CANCEL;
    GC_TYPE["FM_GC_INPUT"]        = FM_GC_INPUT;
    GC_TYPE["FM_GC_TEXTBOX"]      = FM_GC_TEXTBOX;
    GC_TYPE["FM_GC_PASSWORD"]     = FM_GC_PASSWORD;
    GC_TYPE["FM_GC_STATIC_TEXT"]  = FM_GC_STATIC_TEXT;
    GC_TYPE["FM_GC_LISTBOX"]      = FM_GC_LISTBOX;
    GC_TYPE["FM_GC_3D_BOX"]       = FM_GC_3D_BOX;
    GC_TYPE["FM_GC_ACTION"]       = FM_GC_ACTION;
    GC_TYPE["FM_GC_ACTION_BAR"]   = FM_GC_ACTION_BAR;
    GC_TYPE["FM_GC_MESSAGE"]      = FM_GC_MESSAGE;
    GC_TYPE["FM_GC_CHATBOX"]      = FM_GC_CHATBOX;
    GC_TYPE["FM_GC_SELECT"]       = FM_GC_SELECT;
    GC_TYPE["FM_GC_TICKBOX"]      = FM_GC_TICKBOX;
    GC_TYPE["FM_GC_HIDDEN"]       = FM_GC_HIDDEN;
    GC_TYPE["FM_GC_MENU"]         = FM_GC_MENU;
    GC_TYPE["FM_GC_DROPBOX"]      = FM_GC_DROPBOX;
    GC_TYPE["FM_GC_STATIC_IMAGE"] = FM_GC_STATIC_IMAGE;
    GC_TYPE["FM_GC_REPORTER"]     = FM_GC_REPORTER;
    GC_TYPE["FM_GC_GROUPTICK"]    = FM_GC_GROUPTICK;
    GC_TYPE["FM_GC_TEXTBUFFER"]   = FM_GC_TEXTBUFFER;
    GC_TYPE["FM_GC_SELECTA"]      = FM_GC_SELECTA;
    GC_TYPE["FM_GC_SELECTA_PROPS"]= FM_GC_SELECTA_PROPS;
    GC_TYPE["FM_GC_WILLY"]        = FM_GC_WILLY;
    GC_TYPE["FM_GC_LOCAL_DIR"]    = FM_GC_LOCAL_DIR;
    GC_TYPE["FM_GC_REMOTE_DIR"]   = FM_GC_REMOTE_DIR;
    GC_TYPE["FM_GC_H_SCROLLBAR"]  = FM_GC_H_SCROLLBAR;
    GC_TYPE["FM_GC_V_SCROLLBAR"]  = FM_GC_V_SCROLLBAR;
    GC_TYPE["FM_GC_POPUP_MENU"]   = FM_GC_POPUP_MENU;
    GC_TYPE["FM_GC_CONSOLE"]      = FM_GC_CONSOLE;
	GC_TYPE["FM_GC_FONT_SELECT"]  = FM_GC_FONT_SELECT;
	GC_TYPE["FM_GC_COLOR_PICKER"] = FM_GC_COLOR_PICKER;
	GC_TYPE["FM_GC_TAB_BOX"]      = FM_GC_TAB_BOX;

    pLog->_Add("C_FM_GUI::init() -> GC_TYPE setup");

/**/
    GC_PROP.clear();

    GC_PROP["FM_GP_ALWAYS_ON_TOP"]   = FM_GP_ALWAYS_ON_TOP;
    GC_PROP["FM_GP_SIZEABLE"]        = FM_GP_SIZEABLE;
    GC_PROP["FM_GP_CLOSEX"]          = FM_GP_CLOSEX;
    GC_PROP["FM_GP_MOVEABLE"]        = FM_GP_MOVEABLE;
    GC_PROP["FM_GP_MENU"]            = FM_GP_MENU;
    GC_PROP["FM_GP_USEBACKGROUND"]   = FM_GP_USEBACKGROUND;
    GC_PROP["FM_GP_BORDER"]          = FM_GP_BORDER;
    GC_PROP["FM_GP_MINIMIZE"]        = FM_GP_MINIMIZE;
    GC_PROP["FM_GP_CLIENT_ONLY"]     = FM_GP_CLIENT_ONLY;
    GC_PROP["FM_GP_ALWAYS_ON_BOTTOM"]= FM_GP_ALWAYS_ON_BOTTOM;
    GC_PROP["FM_GP_NO_RETURN"]       = FM_GP_NO_RETURN;
    GC_PROP["FM_GP_DO_NOT_DIE"]      = FM_GP_DO_NOT_DIE;
    GC_PROP["FM_GP_FULLSCREEN"]      = FM_GP_FULLSCREEN;
    GC_PROP["FM_GP_TOGGLE_STATE"]    = FM_GP_TOGGLE_STATE;

    pLog->_Add("C_FM_GUI::init() -> GC_PROP setup");

	pConsole = new C_EM_CONSOLE(pGAF,pLog);

	pLog->_Add("C_FM_GUI: Console created");

	ButtonTexture=0;
	B9utton=0;

	pMouse=0;
	pMouse=new C_GLMouse(pGAF,pLog);

    InitFonts();
    InitButtons();

    LoadButtons("");
    LoadFonts();

//    MOUSEMODE=MP_MENU;

    memset(szPromptMsg,0,_MAX_PATH);
    strcpy(szCommand,"nop");

    bShowFPS=false;

    /////////////////////////////////////////////////////////////////////////////////
    // Setup default key values

    KeyMap[(SDLKey)'`']="toggle console";
    KeyMap[(SDLKey)'~']="toggle console";
    KeyMap[(SDLKey)'f']="toggle fps";

    first_gui_stump=0;
	stump_count=0;
	focus_stump=0;
	focus_control=0;
	first_gui_stump=0;
	pFirstGUIData=0;
}

/********************************************************/

void C_FM_GUI::ConsoleEntry(char *fmt, ...)
{
    char ach[1024]; memset(ach,0,1024);
    va_list vaz;
    va_start(vaz,fmt);
    vsprintf(ach,fmt,vaz);
    va_end(vaz);


    if(strlen(ach))
    {
        if(pConsole)
            pConsole->Entry(ach);

        C_FM_GUI_STUMP  *gump;
        C_FM_GUI_CONTROL *ctrl;

        gump=first_gui_stump;
        while(gump)
        {
            ctrl=gump->first_gui_control;
            while(ctrl)
            {
                if(ctrl->type==FM_GC_CONSOLE)
                {
                    if(ctrl->first_child_control)
                    {
                        ctrl->put_control_data("listentry",ach);
                        ctrl->listoffset = ctrl->control_data_total - ctrl->listdepth;
                    }
                }
                ctrl=ctrl->next;
            }
            gump=gump->next;
        }
    }

}

/********************************************************/

void C_FM_GUI::clear()
{
    C_FM_GUI_STUMP *gui_stump;
    gui_stump=0;
    C_FM_GUI_STUMP *del_gui_stump;
    gui_stump=first_gui_stump;
    while(gui_stump)
    {
        del_gui_stump=gui_stump;
        gui_stump=gui_stump->next;
        DEL(del_gui_stump);
    }
    gui_stump=0;
	stump_count=0;
	focus_stump=0;
	focus_control=0;
	first_gui_stump=0;
}

/********************************************************/

void C_FM_GUI::add_stump(char *name,int x,int y,int x2,int y2,int props,char *media)
{
	bool bfree=0;

    if(stump_count>MAX_GUIS) return;

	C_FM_GUI_STUMP *stump;

	del_stump(name);

    stump=first_gui_stump;

    if(!stump)
    {
        first_gui_stump=new C_FM_GUI_STUMP(pLog,pGFX,this,pConsole);
        stump=first_gui_stump;
    }
    else
    {
        while(stump->next)
        {
            stump=stump->next;
        }
        stump->next=new C_FM_GUI_STUMP(pLog,pGFX,this,pConsole);
        stump->next->prev=stump;
        stump=stump->next;
    }


    strcpy(stump->stname,name);
    stump->rect.left=x;
    stump->rect.top=y;
    stump->rect.right=x2;
    stump->rect.bottom=y2;
    stump->mrect.left=x;
    stump->mrect.top=y;
    stump->mrect.right=x2;
    stump->mrect.bottom=y2;
    stump->props=props;
    strcpy(stump->media,media);
    stump->deleteme=0;
    stump->moving=false;

	stump_count++;

    pLog->_Add("stump added [%s] stump count [%d]",name,stump_count);
    pLog->_Add("properties: %d (%d)",stump->props,(stump->props&FM_GP_MOVEABLE));
}

/********************************************************/

void C_FM_GUI::mod_stump(char *name,int x,int y,int x2,int y2,int props,char *media)
{
    C_FM_GUI_STUMP *stump=get_stump(name);
    if(!stump) return;
    strcpy(stump->stname,name);
    stump->rect.left=x;
    stump->rect.top=y;
    stump->rect.right=x2;
    stump->rect.bottom=y2;
    stump->props=props;
    strcpy(stump->media,media);
}

/********************************************************/

void C_FM_GUI::del_stump(C_FM_GUI_STUMP *tstump)
{
    C_FM_GUI_STUMP *pstump=get_prev_stump(tstump);
    if(tstump)
    {
        stump_count--;
        if(pstump)
            pstump->next=tstump->next;

		if(first_gui_stump==tstump)
            first_gui_stump=tstump->next;

		pLog->_Add("stump deleted [%s] stump",tstump->stname);

		DEL(tstump);

		pLog->_Add("stump count [%d]",stump_count);
    }
}

/********************************************************/

void C_FM_GUI::del_stump(char *name)
{
	del_stump(get_stump(name));
}

/********************************************************/

C_FM_GUI_STUMP *C_FM_GUI::get_stump(char *name)
{
    C_FM_GUI_STUMP *gui_stump=first_gui_stump;
    while(gui_stump)
    {
        if(dscc(gui_stump->stname,name)) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}

/********************************************************/

C_FM_GUI_STUMP *C_FM_GUI::get_prev_stump(char *name)
{
    C_FM_GUI_STUMP *gui_stump=first_gui_stump;
    while(gui_stump)
    {
		if(gui_stump->next)
			if(dscc(gui_stump->next->stname,name)) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}

/********************************************************/

C_FM_GUI_STUMP *C_FM_GUI::get_prev_stump(C_FM_GUI_STUMP *tstump)
{
    C_FM_GUI_STUMP *gui_stump=first_gui_stump;
    while(gui_stump)
    {
		if(gui_stump->next==tstump) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}


/********************************************************/

enum eGUILOADMODE
{
    FGL_ATTACH=0,
    FGL_GUMP,
    FGL_CTRL,
    FGL_CTRL_CHILD,
    FGL_DATA

} ;

void C_FM_GUI::call_process_file(char *fin)
{
   	std::vector <std::string> vs;
	std::vector <std::string> vin;

	static C_FM_GUI_STUMP *t;
	static C_FM_GUI_CONTROL *gc;
	static C_FM_GUI_CTRL_DATA *gcd;

    int prop=0;
	char r,g,b;
	int i;

    static char cur_gump[1024];
    static char cur_ctrl[1024];
    static eGUILOADMODE mode;
    static eGUILOADMODE lastmode;

    static bool modechange=0;

    char vsa[1024];
    memset(vsa,0,1024);
    int  vsal=0;
    char vsb[1024];
    memset(vsb,0,1024);
    char vsbl=0;
/*
	t=get_stump(cur_gump);
	if(!t)
	{
	    t=first_gui_stump;
    }
    if(t)
	{
	    gc=t->get_control(cur_gump);
        if(!gc)
            gc=t->first_gui_control;
	}
	*/

	vs=explode("\n",fin);
	if(vs.size())
        vin=explode("=",vs[0]);

    if(vin[0]!="")
    {
		pLog->_Add("%s",vin[0].c_str());

		if(vin.size()>1)
		{
			pLog->_Add("   \\=>[%s]",vin[1].c_str());
			strcpy(vsa,vin[0].c_str());
			vsal=strlen(vsa);
			strcpy(vsb,vin[1].c_str());
			vsbl=strlen(vsb);
		}

		// determine mode

		if(dscc((char *)vin[0].c_str(),"[ATTACH]"))
		{
            pLog->_Add("******> MODE ATTACH ");
            //if(mode==FGL_CTRL) if(gc) gc->attach_default_children();
            mode=FGL_ATTACH;

			return;
		}

        if(dscc((char *)vin[0].c_str(),"[GUMP]"))
        {
            pLog->_Add("******> MODE GUMP ");
            //if(mode==FGL_CTRL) if(gc) gc->attach_default_children();
            mode=FGL_GUMP;
            return;
        }

        if(dscc((char *)vin[0].c_str(),"[CTRL]"))
        {
            pLog->_Add("******> MODE CTRL ");
            //if(mode==FGL_CTRL) if(gc) gc->attach_default_children();
            mode=FGL_CTRL;
            return;
        }

        if(dscc((char *)vin[0].c_str(),"[CTRL_CHILD]"))
        {
            pLog->_Add("******> MODE CTRL_CHILD ");
            //if(mode==FGL_CTRL) if(gc) gc->attach_default_children();
            mode=FGL_CTRL_CHILD;
            return;
        }

		if(dscc((char *)vin[0].c_str(),"[DATA]"))
		{
			pLog->_Add("******> MODE DATA ");
			//if(mode==FGL_CTRL) if(gc) gc->attach_default_children();
			mode=FGL_DATA;
			return;
		}




		switch(mode)
		{

            case FGL_ATTACH:

                if(t)
                {
                    if(cab_loading)
                    {

                        cab_call((char *)vin[1].c_str());
                    }
                    else
                    {
                        if(dscc((char *)vin[0].c_str(),"name"))
                        {
                            pLog->_Add("Loading GUI file -> %s",                                va("%s%cgumps%c%s",dlcs_getcwd(),PATH_SEP,PATH_SEP,vin[1].c_str()) );
                        }
                    }
                }

                break;

            case FGL_DATA:

                if(t)
                {
                    //pLog->_Add("************** > mode:define data");

                    pLog->_Add("%s",vin[0].c_str());

                    if(vin.size()>1)
                    {
                        gc=t->get_control(cur_ctrl);
                        if(gc)
                        {
                            pLog->_Add("%s=%s",vin[0].c_str(),vin[1].c_str());
                            pLog->_Add("%s",cur_ctrl);

                            gc->put_control_data(		(char *)vin[0].c_str(), (char *)vin[1].c_str());

                            gcd=gc->get_control_data(	(char *)vin[0].c_str() );

                            if(gcd)
                                pLog->_Add("DATA[%s = %s]", (char *)vin[0].c_str(),gcd->value);

                            return;
                        }
                    }
                }
                break;

            case FGL_GUMP:

                //pLog->_Add("mode:define gump");

                if(dscc((char *)vin[0].c_str(),"name"))
                {
                    pLog->_Add("name:%s",vin[1].c_str());

                    strcpy(cur_gump,(char *)vin[1].c_str());
                    add_stump(cur_gump,810,610,10,10,0,"0");
                    t=get_stump(cur_gump);
                    return;

                }

                if(t)
                {
                    if(dscc((char *)vin[0].c_str(),"gamemode"))
                    {
                        pLog->_Add("gamemode:%s",vin[1].c_str());

                        if(dscc((char *)vin[1].c_str(),"main"))
                        {
                            pLog->_Add("This gui wants to set game mode to main");
                        }

                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"x"))
                    {
                        pLog->_Add("x:%d",atoi(vin[1].c_str()));
                        t->x=0;
                        if(dscc(vsb,"width"))
                        {
                            pLog->_Add("Setting x to %d (screen width)",SDL_GetVideoSurface()->w);
                            t->x=SDL_GetVideoSurface()->w-1;
                        }
                        if((int)vsb[vsbl-1]==(int)'*')
                        {
                            vsb[vsbl-1]=0;
                            pLog->_Add("Setting x to %d percent",vsb);
                            t->x=SDL_GetVideoSurface()->w*(atoi(vsb)/100);
                        }
                        else
                        {
                            t->x=atoi(vin[1].c_str());
                        }
                        t->rect.left=(LONG)t->x;
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"y"))
                    {
                        pLog->_Add("y:%d",atoi(vin[1].c_str()));
                        t->y=0;
                        if(dscc(vsb,"height"))
                        {
                            pLog->_Add("Setting y to %d (screen height)",SDL_GetVideoSurface()->h);
                            t->y=SDL_GetVideoSurface()->h;
                        }
                        if((int)vsb[vsbl-1]==(int)'*')
                        {
                            vsb[vsbl-1]=0;

                            t->y=SDL_GetVideoSurface()->h*(atoi(vsb)/100);
                            pLog->_Add("Setting y to %d percent (%d)",atoi(vsb),t->y);
                        }
                        else
                        {
                            t->y=atoi(vin[1].c_str());
                        }
                        t->rect.top=(LONG)t->y;
                        return;

                    }

                    if(dscc((char *)vin[0].c_str(),"w"))
                    {
                        pLog->_Add("w:%d",atoi(vin[1].c_str()));
                        t->rect.right=0;
                        if((int)vsb[vsbl-1]==(int)'*')
                        {
                            vsb[vsbl-1]=0;
                            pLog->_Add("Setting w to %d percent",vsb);
                            t->rect.right=SDL_GetVideoSurface()->w*(atoi(vsb)/100);
                        }
                        else
                        {
                            t->rect.right=atoi(vin[1].c_str());
                        }

                        if(dscc(vsb,"width"))
                        {
                            pLog->_Add("Setting w to %d (screen width)",SDL_GetVideoSurface()->w);
                            t->rect.right=SDL_GetVideoSurface()->w-1;
                        }
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"h"))
                    {
                        pLog->_Add("h:%d",atoi(vin[1].c_str()));

                        pLog->_Add(" ******************************************* ");

                        pLog->_Add("[%s]",vsb);

                        pLog->_Add(" ******************************************* ");


                        t->rect.bottom=0;
                        if(dscc(vsb,"height"))
                        {
                            pLog->_Add("Setting h to %d (screen height)",SDL_GetVideoSurface()->h);
                            t->rect.bottom=SDL_GetVideoSurface()->h;
                        }
                        if((int)vsb[vsbl-1]==(int)'*')
                        {
                            vsb[vsbl-1]=0;

                            t->rect.bottom=(SDL_GetVideoSurface()->h * ((float)(atoi(vsb)/100)));
                            pLog->_Add("Setting h to %d percent (%d) (%d)",atoi(vsb),t->rect.bottom,SDL_GetVideoSurface()->h);
                        }
                        else
                        {
                            t->rect.bottom=atoi(vin[1].c_str());
                        }
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"props"))
                    {

                        t->props=0;
                        vs=explode("|",vin[1]);
                        for(i=0;i<(int)vs.size();i++)
                        {
                            t->props+=GC_PROP[(char *)vs[i].c_str()];
                            pLog->_Add("props:%d",t->props);
                        }

                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"action"))
                    {
                        t->set_action((char *)vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"caption"))
                    {
                        t->set_caption((char *)vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"media"))
                    {
                        strcpy(t->media,vin[1].c_str());
                        return;
                    }
                }

                return;

            case FGL_CTRL_CHILD:

                if(t)
                {
                    //pLog->_Add("mode:define ctrl child");

                    if(dscc((char *)vin[0].c_str(),"name"))
                    {

                        gc=t->get_control(cur_ctrl);
                        if(!gc) return;
                        strcpy(cur_ctrl,(char *)vin[1].c_str());
                        pLog->_Add("*****> CTRL CHILD NAME = %s",cur_ctrl);
                        t->add_control(gc,cur_ctrl,0,0,0,32,32,0,"","");
                        mode=FGL_CTRL;
                        return;
                    }
                }
                return;

            case FGL_CTRL:

                if(t)
                {
                    //pLog->_Add("mode:define ctrl");

                    if(dscc((char *)vin[0].c_str(),"name"))
                    {
                        strcpy(cur_ctrl,(char *)vin[1].c_str());
                        pLog->_Add("*****> CTRL NAME = %s",cur_ctrl);
                        t->add_control(cur_ctrl,0,0,0,32,32,0,"","");
                        return;
                    }

                    gc=t->get_control(cur_ctrl);
                    if(!gc) return;


                    if(dscc((char *)vin[0].c_str(),"group"))
                    {
                        strcpy(gc->group,(char *)vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"props"))
                    {

                        gc->props=0;
                        vs=explode("|",vin[1]);
                        for(i=0;i<(int)vs.size();i++)
                        {
                            gc->props+=GC_PROP[(char *)vs[i].c_str()];
                            pLog->_Add("props:%d",gc->props);
                        }


                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"border_color"))
                    {
                        pLog->_Add("border_color %s",vin[1].c_str());
                        vs=explode(",",vin[1]);
                        if(vs.size()>2)
                        {
                            r=atoi(vs[0].c_str());
                            g=atoi(vs[1].c_str());
                            b=atoi(vs[2].c_str());
                            gc->border_color=RGB(r,g,b);
                        }
                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"background_color"))
                    {
                        pLog->_Add("background_color %s",vin[1].c_str());
                        vs=explode(",",vin[1]);
                        if(vs.size()>1)
                        {
                            r=atoi(vs[0].c_str());
                            g=atoi(vs[1].c_str());
                            b=atoi(vs[2].c_str());
                            gc->background_color=RGB(r,g,b);
                        }
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"listdepth"))
                    {
                        gc->listdepth=atoi(vin[1].c_str());
                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"console"))
                    {
                        strcpy(gc->console,(char *)vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"media"))
                    {
                        strcpy(gc->media,(char *)vin[1].c_str());
                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"media_hover"))
                    {
                        strcpy(gc->media_hover,(char *)vin[1].c_str());
                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"media_click"))
                    {
                        strcpy(gc->media_click,(char *)vin[1].c_str());
                        return;
                    }


                    if(dscc((char *)vin[0].c_str(),"x"))
                    {
                        gc->rect.left=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"y"))
                    {
                        gc->rect.top=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"h"))
                    {
                        gc->rect.bottom=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"w"))
                    {
                        if(dscc((char *)vin[1].c_str(),"width"))
                        {
                            pLog->_Add("Setting w to %d (screen width)",SDL_GetVideoSurface()->w);
                            gc->rect.right=SDL_GetVideoSurface()->w-1;
                        }
                        else
                            gc->rect.right=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"x2"))
                    {
                        gc->rect_2.left=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"y2"))
                    {
                        gc->rect_2.top=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"h2"))
                    {
                        gc->rect_2.bottom=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"w2"))
                    {
                        gc->rect_2.right=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"font"))
                    {
                        gc->font=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"fonthighlow"))
                    {
                        gc->fonthighlow=atoi(vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"type"))
                    {
                        gc->type=GC_TYPE[(char *)vin[1].c_str()];


                        if(gc->type==FM_GC_SELECTA_PROPS)
                        {
                            gc->visible=false;
                        }

                        if( (gc->type==FM_GC_TEXTBOX) || (gc->type==FM_GC_PASSWORD) )
                        {
                            gc->rect.bottom=15;
                            gc->rect.right=100;
                        }
                        if(gc->type==FM_GC_ACTION)
                        {
                            gc->rect.bottom=32;
                            gc->rect.right=32;
                        }

                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"value"))
                    {
                            if(
                                (gc->type==FM_GC_LISTBOX) ||
                                (gc->type==FM_GC_DROPBOX) )
                            {
                                gc->put_control_data(
                                    "listentry",
                                    (char *)vin[1].c_str());


                                if(( (strlen((char *)vin[1].c_str())*9) + 9) > (gc->rect.right))
                                gc->rect.right=(strlen((char *)vin[1].c_str())*9)+9;
                                pLog->_Add("SET RECT WIDTH TO:%d [%s]",gc->rect.right,(char *)vin[1].c_str());
                            }
                            else
                            {
                                gc->set_value((char *)vin[1].c_str());
                            }
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"value_get"))
                    {
                        strcpy(gc->value_get,vin[1].c_str());
                        return;
                    }

                    if(dscc((char *)vin[0].c_str(),"visible"))
                    {
                        gc->visible=false;
                        if(dscc((char *)vin[1].c_str(),"yes") )
                            gc->visible=true;
                        if(dscc((char *)vin[1].c_str(),"true") )
                            gc->visible=true;
                        if(dscc((char *)vin[1].c_str(),"1") )
                            gc->visible=true;

                    }

                    if(dscc((char *)vin[0].c_str(),"[CTRL]"))
                    {

                    }
                    else
                    {
                        return;
                    }


                default:
                break;
            }
		}
    }
}

/********************************************************/

void C_FM_GUI::call(char *file)
{
	if(cab_loading==true)
	{
		cab_call(file);
		return;
	}
}

/********************************************************/

void C_FM_GUI::cab_call(char *file)
{
	pLog->_Add("   GUI->call(gumps/%s)",file);
	std::vector <std::string> vs;
	CxMemFile *hi;
	if(!pGAF) return;
	GAF_FileBuffer nfb;
	char fin[1024];    memset(fin,0,1024);

	nfb=pGAF->GetFile((char *)va("gumps/%s",file));

    if(nfb.Size>-1) {

	hi=new CxMemFile((BYTE*)nfb.fb,nfb.Size);
	if(hi) {
	while(hi->GetS(fin,256))
	{
		vs=explode("\r",fin);
		if(vs.size()) call_process_file((char *)vs[0].c_str());
	}
	pLog->_Add("GUI [%s] loaded from GAF",file);
	DEL(hi);

	}
    }


}


/********************************************************/

void C_FM_GUI::bcall(char *file)
{
	pLog->_Add("   GUI->bcall(gumps/%s)",file);
	// std::vector <std::string> vs;
	CxMemFile *hi;
	if(!pGAF) return;
	GAF_FileBuffer nfb;
	char fin[1024];    memset(fin,0,1024);
	nfb=pGAF->GetFile((char *)va("gumps/%s",file));
	hi=new CxMemFile((BYTE*)nfb.fb,nfb.Size);
	pLog->_Add("end bcall");
	DEL(hi);
}

/********************************************************/

void C_FM_GUI::bstore(char *file)
{

    pGAF->AddFileFromMem((char *)va("gumps/new_%s",file),
                         (unsigned char *)&first_gui_stump,sizeof(&first_gui_stump));

    pGAF->ExtractFile_ToFile((LPSTR)va("gumps/new_%s",file),(LPSTR)va("gumps/new_%s",(LPSTR)file));

}

/********************************************************/

void C_FM_GUI::draw()
{

    //

    draw_ctrls();

    DrawPrompt();

    pMouse->draw();

    // DrawMouse(pMouse->X(),pMouse->Y(),MOUSEMODE);

    /*    if(pClientData->bTargeting)
    {
        if(!LeftClick )
        {
            if(pMouse->LeftClick())
            {
                FindTarget();
            }
        }
    }
    */
}


/********************************************************/

void C_FM_GUI_CONTROL::attach_default_children(void)
{
    int i;
    char temp[1024];
    vector <string> dirs;


    if(type==FM_GC_LOCAL_DIR)
    {
        dirs=Dir2Vector(dlcs_getcwd(),"*.*");
        sort(dirs.begin(), dirs.end());
        new_ctrl_data.clear();

        for(i=0;i<dirs.size();i++)
        {
            new_ctrl_data[i]=dirs[i].c_str();
        }

        listdepth=15;
        control_data_total=new_ctrl_data.size();
        listoffset=0;

        for(i=0;i<listdepth;i++)
        {

           parent_stump->add_control(  this,
                                        va("%s_list_%d",name,i),
                                        FM_GC_STATIC_TEXT,
                                        0,
                                        i*16+16,
                                        rect.right,
                                        i*16+32,
                                        0, "", temp);

                if(parent_stump->get_control(va("%s_list_%d",name,i)))
                {
                    parent_stump->get_control(va("%s_list_%d",name,i))->font=8;
                    parent_stump->get_control(va("%s_list_%d",name,i))->fonthighlow=1;
                    parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
                }
        }


        parent_stump->add_control(  this,
                                    va("%s_scrollbar",name),
                                    FM_GC_H_SCROLLBAR,
                                    rect.right+1,
                                    0,
                                    rect.right+1+16,
                                    (listdepth-1)*16,
                                    0,
                                    "",
                                    "");

    }


    if(type==FM_GC_DROPBOX)
    {
        parent_stump->add_control(this,va("%s_scrollbar",name),FM_GC_H_SCROLLBAR,
                        rect.right+1,
                        16,
                        rect.right+1+16,
                        (listdepth-1)*16,
                        0, "","");


        if(parent_stump->get_control(va("%s_scrollbar",name)))
        {
            parent_stump->get_control(va("%s_scrollbar",name))->visible=false;
        }
    }


    if(type==FM_GC_CONSOLE)
    {

        if(pConsole)
        {

           // pLog->_Add("ADDING CONSOLE CHILDREN: filling control data (listentry)");

            for(i=0;i<pConsole->buf.size();i++)
                put_control_data("listentry",(char *)pConsole->buf[i].c_str());


            //pLog->_Add("ADDING CONSOLE CHILDREN: filling listdepth");

            for(i=0;i<listdepth;i++)
            {
                strcpy(temp,(char *)pConsole->buf[i].c_str());
                parent_stump->add_control(  0,va("%s_list_%d",name,i),FM_GC_STATIC_TEXT,
                                            rect.left+16+2,
                                            i*16+16,
                                            rect.right,
                                            i*16+32,
                                            0, "", temp);

                if(parent_stump->get_control(va("%s_list_%d",name,i)))
                {
                    parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
                }
            }

            //pLog->_Add("ADDING CONSOLE CHILDREN: creating scrollbar");

            if(!parent_stump->get_control(va("%s_scrollbar",name)))
            {

                parent_stump->add_control( 0,va("%s_scrollbar",name),FM_GC_H_SCROLLBAR,
                                rect.left,
                                16,
                                rect.left+16,
                                (listdepth-1)*16,
                                0, "","");

                if(parent_stump->get_control(va("%s_scrollbar",name)))
                {
                    parent_stump->get_control(va("%s_scrollbar",name))->visible=true;
                    parent_stump->get_control(va("%s_scrollbar",name))->parent_control=this;
                }
            }

            pLog->_Add("ADDING CONSOLE CHILDREN: creating input (%s)",va("%s_input",name));

            if(!parent_stump->get_control(va("%s_input",name)))
            {

                parent_stump->add_control( 0,va("%s_input",name),FM_GC_TEXTBOX,
                            rect.left,
                            rect.bottom+1,
                            rect.right,
                            16,     0, "","");
            }


            listoffset = control_data_total - listdepth;
        }




    }

}

/********************************************************/

void C_FM_GUI_CONTROL::draw(void)
{
    C_FM_GUI_CONTROL   *tctrl2=0;
    C_FM_GUI_CTRL_DATA *tctrldata;

    vector <string> vtmp;

    int i,j,k,x,y,x2,y2,w,h,w2,h2;
    int x3,y3;
	i=j=k=x=y=x2=y2=w=h=w2=h2=0;

    char temp[1024];
	memset(temp,0,1024);

	long colr;

	int imedia_out;

	int imedia_selected;
	int imedia_unselected;
	int imedia_hover;

	RECT offrect;

	bool bcheck;

	int r1,g1,b1;
	int r2,g2,b2;
	int r3,g3,b3;

	float FONT_CHAR_WIDTH=8.5;

	float fPercent;
	float iV;
	float iT;
	float iW;

    x   = parent_stump->rect.left;
    y   = parent_stump->rect.top;
    x2  = parent_stump->rect.right;
    y2  = parent_stump->rect.bottom;

    tctrl2=parent_control;
    while(tctrl2)
    {
        x +=tctrl2->rect.left;
        y +=tctrl2->rect.top;
        x2+=tctrl2->rect.right;
        y2+=tctrl2->rect.bottom;
        tctrl2=tctrl2->parent_control;
    }

    x += rect.left;
    y += rect.top;
    x2+= rect.right;
    y2+= rect.bottom;

    x3 = x+rect.right;
    y3 = y+rect.bottom;


    if(visible)
    {

            if(pGUI->pMouse->In(x,y,x3,y3))
            {
                if(pGUI->pMouse->LeftClick())
                {
                    pLog->_Add("CLICKED IN [%s]",name);

                    //pGUI->focus_control=parent_stump->get_control(va("%s_input",name));

                    //if(!pGUI->focus_control)
                      //  pGUI->focus_control=this;

                    //pGUI->focus_stump=this->parent_stump;
                }
                if(pGUI->pMouse->WheelDown())
                {
                    listoffset++;
                }
                if(pGUI->pMouse->WheelUp())
                {
                    listoffset--;
                }
            }


        switch(type)
        {

            case FM_GC_CONSOLE:

                if(!parent_stump->get_control(va("%s_scrollbar",name)))
                {
                    attach_default_children();
                }

                if(pGUI->pMouse->In(x,y,x3,y3))
                {

                    if(pGUI->pMouse->LeftClick())
                    {
                        pGUI->focus_control=parent_stump->get_control(va("%s_input",name));
                        if(!pGUI->focus_control)
                            pGUI->focus_control=this;

                        pGUI->focus_stump=this->parent_stump;
                    }
                    if(pGUI->pMouse->WheelDown())
                    {
                        listoffset++;
                    }
                    if(pGUI->pMouse->WheelUp())
                    {
                        listoffset--;
                    }
                }
                if(props & FM_GP_USEBACKGROUND)
                {
                    pGFX->DrawBar(x,y,x3,y3,LONGRGB(0,0,0),LONGRGB(0,0,50));
                }
                else
                {
                    pGFX->DrawBaseGFX(x,y,x3,y3,atoi(media),255,255,255 );
                }
                if(listoffset > (control_data_total - listdepth))
                    listoffset = control_data_total - listdepth;
                if(listoffset < 0)
                    listoffset = 0;
                for(i=0;i<listdepth;i++)
                {
                    strcpy(temp,(char *)pConsole->buf[i+listoffset].c_str());
                    if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                    if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;
                    if(parent_stump->get_control(va("%s_list_%d",name,i)))
                    {
                        strcpy(parent_stump->get_control(va("%s_list_%d",name,i))->value,va("^>00FF00%s",temp));
                    }
                }
                break;

            case FM_GC_H_SCROLLBAR:
            case FM_GC_V_SCROLLBAR:

                    if(props & FM_GP_USEBACKGROUND)
                    {

                    }

                    else
                    {
                        pGFX->DrawBar(x   , y  , x+15   , y3+17    ,LONGRGB(180,180,180),LONGRGB(180,180,180));
                    }

                    pGUI->WriteText(x,y, "W", 2);

                    if(pGUI->pMouse->In(x,y,x+16,y+16))
                    {
                        if(pGUI->pMouse->ButtonDownTick(SDL_BUTTON_LEFT))
                        {
                            if(parent_control)
                            {
                                parent_control->listoffset--;
                                if(parent_control->listoffset < 0)
                                {
                                    parent_control->listoffset = 0;
                                }
                            }
                        }
                    }

                    if(parent_control)
                    {

                        iV=(float)parent_control->listoffset;
                        iT=(float)parent_control->control_data_total;
                        h=(y3-9)-(y+16);
                        iW=float(h);
                        fPercent=(((parent_control->listoffset)/iT)*iW);
                        pGUI->WriteText(x , y+16+fPercent, "f", 2);
                    }


                    pGUI->WriteText(x,y3, "V", 2);

                    if(pGUI->pMouse->In(x,y3,x+16,y3+16))
                    {
                        if(pGUI->pMouse->ButtonDownTick(SDL_BUTTON_LEFT))
                        {
                            if(parent_control)
                            {
                                parent_control->listoffset++;
                                i=parent_control->control_data_total;
                                if(parent_control->listoffset > (i-parent_control->listdepth))
                                    parent_control->listoffset=i-parent_control->listdepth;
                            }
                        }
                    }

                break;

            case FM_GC_DROPBOX:

                    if(!parent_stump->get_control(va("%s_scrollbar",name)))
                    {
                        attach_default_children();
                    }

                    pGFX->DrawBar(x,y,x3,y+16,LONGRGB(102,102,170),LONGRGB(12,12,170));

                    bcheck=0;

                    tctrldata=first_control_data;
                    while(tctrldata)
                    {
                        if(tctrldata->selected)
                        {
                            bcheck=1;
                        }
                        tctrldata=tctrldata->next;
                    }
                    if(!bcheck)
                    {
                        if(first_control_data)
                            first_control_data->selected=1;
                    }

                    tctrldata=first_control_data;
                    while(tctrldata)
                    {
                        if(tctrldata->selected)
                        {
                            pGUI->WriteText(x,y,va("^>FFFFFF%s",tctrldata->value),0);
                        }
                        tctrldata=tctrldata->next;
                    }

                    if(pGUI->focus_control!=this)
                    {
                        if(pGUI->pMouse->In(x,y,x3,y3))
                        {
                            pGUI->WriteText(x3-15, y, "^6Y", 2);

                            if(pGUI->pMouse->LeftClick())
                            {
                                if(pGUI->focus_control==this)
                                {
                                    pGUI->focus_control=0;
                                }
                                else
                                {
                                    pGUI->focus_control=this;
                                    pGUI->focus_stump=this->parent_stump;
                                }
                            }
                        }

                        else
                            pGUI->WriteText(x3-15,y,"^1Y", 2);
                    }


                    if(pGUI->focus_control==this)
                    {
                        if(parent_stump->get_control(va("%s_scrollbar",name)))
                        {
                            parent_stump->get_control(va("%s_scrollbar",name))->visible=true;
                            pGFX->DrawBar(x-1,y+17,x3+1,y+(listdepth*16)+17, LONGRGB(132,132,200), LONGRGB(12,12,170));
                            i=control_data_total;
                            tctrldata=first_control_data;
                            k=0;
                            while(k<listoffset)
                            {
                                if(tctrldata)
                                {
                                    if(tctrldata->next)
                                    {
                                        tctrldata=tctrldata->next;
                                    }
                                }
                                k++;
                            }
                            j=1;

                            while( (tctrldata) &&
                                    (j<(listdepth+1)) )
                            {

                                if(tctrldata->selected)
                                {
                                    r1=255; g1=255; b1=0;
                                    r2=200; g2=200; b2=0;

                                }
                                else
                                {
                                    r1=132; g1=132; b1=200;
                                    r2=12;  g2=12;  b2=170;

                                }

                                if(pGUI->pMouse->In(x-1,y+(j*16),x3+1,y+(j*16)+16))
                                {
                                    r1=255; g1=255; b1=255;

                                    r2=pGFX->GetFade(1);
                                    g2=pGFX->GetFade(2);
                                    b2=pGFX->GetFade(3);

                                    r1=pGFX->GetFade(2);
                                    g1=pGFX->GetFade(3);
                                    b1=pGFX->GetFade(1);

                                    if(pGUI->pMouse->LeftClick())
                                    {
                                        control_data_unselect();
                                        tctrldata->selected=true;
                                        pGUI->focus_control=0;
                                        listoffset=0;
                                    }

                                    if(pGUI->pMouse->WheelDown())
                                    {
                                        listoffset++;
                                        if(listoffset> (i-listdepth))
                                            listoffset=i-listdepth;
                                    }

                                    if(pGUI->pMouse->WheelUp())
                                    {
                                        listoffset--;
                                        if(listoffset<0)
                                            listoffset=0;
                                    }


                                }

                                pGFX->DrawBar(x-1,y+(j*16),x3+1,y+(j*16)+16, LONGRGB(r1,g1,b1),LONGRGB(r2,g2,b2));
                                pGUI->WriteText(x,y+(j*16),va("^>FFFFFF%s",tctrldata->value),0);
                                j++;
                                tctrldata=tctrldata->next;
                            }
                        }
                    }

                    else
                    {
                        if(parent_stump->get_control(va("%s_scrollbar",name)))
                            parent_stump->get_control(va("%s_scrollbar",name))->visible=false;
                    }

                break;

            case FM_GC_LOCAL_DIR:

                    if(!parent_stump->get_control(va("%s_scrollbar",name)))
                    {
                        rect.right=384;
                        rect.bottom=364;
                        attach_default_children();
                    }

                    if(pGUI->pMouse->In(x,y,x3,y3))
                    {

                    }

                    if(props & FM_GP_USEBACKGROUND)
                    {
                        pGFX->DrawBaseGFX(x,y,x3,y3,atoi(media),255,255,255 );

                    }
                    else
                    {
                        pGUI->draw_3d_box(x,y,x3,y3);
                        pGFX->DrawBar(x+2,y+2,x3-2,y3-2,LONGRGB(255,255,255),LONGRGB(255,255,255));
                    }

                    if(listoffset > (control_data_total - listdepth))
                        listoffset = control_data_total - listdepth;

                    if(listoffset < 0)
                        listoffset = 0;

                    for(i=0;i<listdepth;i++)
                    {

                        strcpy(temp,(char *)new_ctrl_data[i+listoffset].c_str());

                        if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                        if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;

                        if(parent_stump->get_control(va("%s_list_%d",name,i)))
                        {



                            strcpy(parent_stump->get_control(va("%s_list_%d",name,i))->value,va("^0 %s",temp));
                        }
                    }




                break;

            case FM_GC_LISTBOX:

                    pGFX->DrawBar(x,y,x3,y3,LONGRGB(180,180,180),LONGRGB(180,180,180));
                    pGUI->WriteText(x,y,va("^>ffffff%s",name),0);

                    tctrldata=first_control_data;

                    while(tctrldata)
                    {
                        if(y+16>(y3-12)) break;
                        strcpy(temp,tctrldata->value);
                        while( (strlen(temp)*9) > rect.right)
                        {
                            temp[strlen(temp)-4]='.';
                            temp[strlen(temp)-3]='.';
                            temp[strlen(temp)-2]='.';
                            temp[strlen(temp)-1]=0;
                        }

                        pGUI->WriteText(x,y+16,temp,0);
                        y+=12;
                        tctrldata=tctrldata->next;
                    }

                    break;

            case FM_GC_GROUPTICK:
            case FM_GC_TICKBOX:

                if(props & FM_GP_USEBACKGROUND)
                {
                    pGFX->DrawBaseGFX(x,y,x3,y3,atoi(media),255,255,255 );
                }
                else
                {
                    pGFX->DrawBar(x,y,x3,y3,LONGRGB(240,240,240),LONGRGB(240,240,240));
                    pGFX->DrawBar(x+1,y+1,x3,y3,LONGRGB(80,80,80),LONGRGB(80,80,80));
                    pGFX->DrawBar(x+2,y+2,x3-2,y3-2,LONGRGB(180,180,180),LONGRGB(180,180,180));
                }

                if(dscc(value,"1")) strcpy(value,"on");

                if(dscc(value,"on"))
                {
                    pGUI->WriteText(x,y,"^+H",2);
                }
                else
                {
                    strcpy(value,"off");
                }

                if(pGUI->pMouse->In(x,y,x3,y3))
                {
                    if(pGUI->pMouse->LeftClick())
                    {
                        if(dscc(value,"on"))
                        {
                            this->parent_stump->clear_grouptick(name);
                            strcpy(value,"off");
                        }
                        else
                        {
                            this->parent_stump->clear_grouptick(name);
                            strcpy(value,"on");
                        }

                        pGUI->focus_control=this;
                    }
                }

                break;

            case FM_GC_STATIC_TEXT:

                if(props&FM_GP_USEBACKGROUND)
                    pGFX->DrawBar(x,y-2,x+(CGLFont_StrLen(value))*8+24,y+16,background_color,background_color);

                if(props&FM_GP_BORDER)
                    pGFX->DrawRectangle(x,y-2,x+(CGLFont_StrLen(value))*8+24,y+16,border_color);

                pGUI->WriteText(x,y,va("^>5555ff%s",value),font,fonthighlow);

                break;

            case FM_GC_TEXTBOX:
            case FM_GC_PASSWORD:

                if(pGUI->focus_control==this)
                {
                    //pGFX->DrawBar(x+1,y-1,x3+1,y3+1,LONGRGB(pGFX->GetFade(3),pGFX->GetFade(3),255),LONGRGB(pGFX->GetFade(3),pGFX->GetFade(3),255));
                    //pGFX->DrawBar(x+8,y,x3,y3,LONGRGB(255,255,255),LONGRGB(255,255,255));

                    pGFX->DrawBar(x,y,x3,y3,LONGRGB(0,0,0),LONGRGB(0,0,0));
                    pGFX->DrawBar(x+1,y+1,x3-1,y3-1,LONGRGB(255,255,255),LONGRGB(255,255,255));

                    strcpy(temp,value);
                    k=(rect.right/FONT_CHAR_WIDTH)-1;
                    while(CGLFont_StrLen(temp)>k)
                    {
                        j=0;
                        while(temp[j]!=0)
                        {
                            temp[j]=temp[j+1];
                            j++;
                        }
                    }

                    if(type==FM_GC_PASSWORD)
                    {
                        for(k=0;k<strlen(temp);k++)
                        {
                            temp[k]='*';
                        }
                    }

                    pGUI->WriteText(x,y,va("^>00FF00%s",temp),0);

                    if(pGUI->FlashCursor(0,0))
                        pGUI->WriteText((int)(x+(CGLFont_StrLen(temp)*8.5)),y,"^1f",2);

                }
                else
                {
                    pGFX->DrawBar(x,y,x3,y3,LONGRGB(0,0,0),LONGRGB(0,0,0));
                    pGFX->DrawBar(x+1,y+1,x3-1,y3-1,LONGRGB(255,255,255),LONGRGB(255,255,255));

                    strcpy(temp,value);
                    k=(rect.right/FONT_CHAR_WIDTH)-1;
                    temp[k]=0;

                    if(type==FM_GC_PASSWORD)
                    {
                        for(k=0;k<strlen(temp);k++)
                        {
                            temp[k]='*';
                        }
                    }

                    pGUI->WriteText(x,y,va("^>00FF00%s",temp),0);

                    if(pGUI->pMouse->In(x,y,x3,y3))
                    {
                        if(pGUI->pMouse->LeftClick())
                        {
                            pLog->_Add("Mouse clicked in gui_control [%s]",name);
                            pGUI->focus_control=this;
                            pGUI->focus_stump=this->parent_stump;

                        }
                    }
                }




                break;



            case FM_GC_CANCEL:

                if(pGUI->pMouse->In(x,y,x3,y3))
                {
                    if(pGUI->pMouse->LeftClick())
                    {
                        pGUI->del_stump(this->parent_stump->stname);
                    }
                }


            case FM_GC_SUBMIT:
            case FM_GC_BUTTON:
            case FM_GC_ACTION:

                imedia_out=atoi(media);

                if(imedia_out)
                {
                    if(pGUI->pMouse->In(x,y,x+32,y+32))
                    {
                                 imedia_out=atoi(media_hover);
                                 if(pGUI->pMouse->ButtonDown(SDL_BUTTON_LEFT))
                                     imedia_out=atoi(media_click);
                    }
                    pGFX->DrawBaseGFX(x,y,x+32,y+32,imedia_out,255,255,255 );
                }
                else
                {

                    if(props&FM_GP_USEBACKGROUND)
                    {
                        pGUI->DrawBaseGFXGUIButton(this,x,y);
                    }
                    else
                    {
                        pGUI->DrawSlicedGUIButton(this,x,y);
                    }


                }

                if(pGUI->pMouse->In(x,y,x3,y3))
                {
                    if(pGUI->pMouse->LeftClick())
                    {
                        pLog->_Add("-----------------[%d]--------------",pConsole->buf.size());
                        pLog->_Add("Clicked: %s:%s (x=%d,y=%d,w=%d,h=%d) GROUP[%s]",name,value,x,y,x3,y3,group);
                        pLog->_Add("check for console var");

                        strcpy(temp,"guigo ");
                        strcat(temp,va("|%s=%s",name,value) );

                        tctrl2=parent_stump->first_gui_control;
                        while(tctrl2)
                        {
                            if(
                                (tctrl2->type!=FM_GC_BUTTON) &&
                                (tctrl2->type!=FM_GC_STATIC_TEXT) &&
                                (tctrl2->type!=FM_GC_SELECTA_PROPS)
                                )
                            {

                                if(tctrl2->type==FM_GC_SELECTA)
                                {
                                    if(tctrl2->selected)
                                        strcat(temp,va("|%s=%s",tctrl2->group,tctrl2->value));
                                }
                                else
                                {
                                    strcat(temp,va("|%s",tctrl2->name));
                                    //if(type==FM_GC_PASSWORD)
                                    //	strcat(temp,va("=%s",encrypt(value)));
                                    //else
                                    strcat(temp,va("=%s",tctrl2->value));
                                }
                            }
                            tctrl2=tctrl2->next;
                        }
                        tctrl2=0;


                        if(console[0]!=0)
                        {
                            pLog->_Add("console var");
                            pConsole->_Execute(temp);
                            pGUI->_ConsoleExecute(console);
                        }
                        else
                        {
                            pLog->_Add("no console var");
                            if(parent_stump->props&FM_GP_CLIENT_ONLY)
                            {
                                // process client side gump
                                pLog->_Add("FM_GP_CLIENT_ONLY");

                                strcat(temp,va("|action=%s",parent_stump->action));
                                pConsole->_Execute(temp);

                                if(!(parent_stump->props&FM_GP_DO_NOT_DIE))
                                    pGUI->del_stump(parent_stump->stname);
                            }

                            /*

                            else
                            {
                                pLog->_Add("SEND GUI VARS TO SERVER");

                                SendData.Reset();
                                SendData.Write((char)NETMSG_GUI);
                                SendData.Write((char *)tstump->name);

                                j=0;
                                tctrl2=tstump->first_gui_control;
                                while(tctrl2)
                                {
                                    if(!(tctrl2->props&FM_GP_NO_RETURN))
                                    {
                                        j++;
                                    }
                                    tctrl2=tctrl2->next;
                                }

                                SendData.Write((char)j);
                                tctrl2=tstump->first_gui_control;
                                while(tctrl2)
                                {
                                    if(!(tctrl2->props&FM_GP_NO_RETURN))
                                    {
                                        SendData.Write((char *)tctrl2->name);
                                        SendData.Write((char *)tctrl2->value);
                                    }
                                    tctrl2=tctrl2->next;
                                }
                                tctrl=0;
                                SendNetMessage(0);
                                pLog->_Add("Send submit for stump[%s] with %d controls",tstump->name,j);
                                pGUI->focus_control=0;
                                SetMouseRelease(SDL_BUTTON_LEFT,0);
                            }*/
                        }
                    }
                }

                break;


            case FM_GC_HIDDEN: // chillax & do nothing * 420
                break;


            case FM_GC_STATIC_IMAGE: // draw a base gfx on this spot based on media

                    pGFX->DrawBaseGFX(x,y,x3,y3,atoi(media),255,255,255 );

                break;

            case FM_GC_CHATBOX:


                if(pGUI->pMouse->InRect(rect))
                    colr=RGB(255,255,155);
                else
                    colr=RGB(155,155,55);

                pGFX->DrawBar(x,y,x3,y3,RGB(0,0,0),RGB(0,0,0));
                pGFX->DrawRect(rect,colr);

                break;


            case FM_GC_SELECTA:

                if(strlen(value))
                {
                    tctrl2=parent_stump->get_control(va("%s_props",group));

                    r1=g1=b1=r2=g2=b2=r3=g3=b3=0;
                    imedia_selected=imedia_unselected=imedia_hover=0;

                    if(tctrl2)
                    {
                        tctrldata=tctrl2->first_control_data;
                        while(tctrldata)
                        {
                            if(dscc(tctrldata->name,"selected_r")) r1=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"selected_g")) g1=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"selected_b")) b1=atoi(tctrldata->value);

                            if(dscc(tctrldata->name,"unselected_r")) r2=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"unselected_g")) g2=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"unselected_b")) b2=atoi(tctrldata->value);

                            if(dscc(tctrldata->name,"hover_r")) r3=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"hover_g")) g3=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"hover_b")) b3=atoi(tctrldata->value);

                            if(dscc(tctrldata->name,"media_selected")) imedia_selected=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"media_unselected")) imedia_unselected=atoi(tctrldata->value);
                            if(dscc(tctrldata->name,"media_hover")) imedia_hover=atoi(tctrldata->value);

                            tctrldata=tctrldata->next;
                        }
                    }

                    if(selected)
                    {
                        if(imedia_selected)
                        {
                            pGFX->DrawBaseGFX(x,y,x3,y3,imedia_selected,255,255,255 );
                        }
                        else
                        {
                            pGFX->DrawBar(x,y,x3,y3,RGB(r1,g1,b1),RGB(r1,g1,b1) );
                        }
                    }
                    else
                    {
                        if(imedia_unselected)
                        {
                            pGFX->DrawBaseGFX(x,y,x3,y3,imedia_unselected,255,255,255 );
                        }
                        else
                        {
                            pGFX->DrawBar(x,y,x3,y3,RGB(r2,g2,b2),RGB(r2,g2,b2) );
                        }
                    }

                    if(pGUI->pMouse->InRect(rect))
                    {
                        pGFX->DrawBar(x,y,x3,y3,RGB(r3,g3,b3),RGB(r3,g3,b3));

                        if(pGUI->pMouse->LeftClick())
                            selecta();
                    }

                    pGUI->WriteText(x+(rect.right/2)-((strlen(value)*12)/2),y-2+(rect.bottom/2)-12,value,0);
                    pGUI->DrawB9utton(400,400,580,580);
                }

                break;

            default:

                strcpy(temp,"UNKNOWN TYPE!");
                map <string, int>::iterator ii;
                for( ii=pGUI->GC_TYPE.begin(); ii!=pGUI->GC_TYPE.end(); ++ii)
                {
                    if( ( (*ii).second ) == (type) )
                    {
                        strcpy(temp,(const char *)(*ii).first.c_str());
                    }
                }
                strcpy(temp,va("[%s][%s]", name,temp));
                x3=x3+(strlen(temp)* 12)+2;
                pGFX->DrawBar(x,y,x3,y3,RGB(255,0,0) );
                pGFX->DrawRectangle(x,y,x+32,y+32,RGB(0,0,0));
                pGUI->WriteText(x,y,temp,0);
                if(pGUI->pMouse->In(x,y,x3,y3))
                {



                }
                /**/
                break;
        }


        tctrl2=first_child_control;
        while(tctrl2)
        {
            tctrl2->draw();
            tctrl2=tctrl2->next;
        }
    }
}
void C_FM_GUI::draw_3d_box(int x, int y, int x2, int y2)
{
    RECT r;
    r.left=x;
    r.top=y;
    r.right=(x2-x);
    r.bottom=(y2-y);
    draw_3d_box(r);
}
void C_FM_GUI::draw_3d_box(RECT rect)
{
    pGFX->DrawBar(  rect.left,
                    rect.top,
                    rect.left+rect.right-2,
                    rect.top+rect.bottom-2,
                    LONGRGB(240,240,240),
                    LONGRGB(240,240,240));

    pGFX->DrawBar(  rect.left+2,
                    rect.top+2,
                    rect.left+rect.right,
                    rect.top+rect.bottom,
                    LONGRGB(80,80,80),
                    LONGRGB(80,80,80));

    pGFX->DrawBar(  rect.left+2,
                    rect.top+2,
                    rect.left+rect.right-2,
                    rect.top+rect.bottom-2,
                    LONGRGB(180,180,180),
                    LONGRGB(180,180,180));
}

void C_FM_GUI::draw_ctrls(void) // Draw and check any active GUIs
{
    C_FM_GUI_STUMP          *tstump=0;
    C_FM_GUI_STUMP          *bstump=0;
    C_FM_GUI_CONTROL        *tctrl=0;
	C_FM_GUI_CONTROL        *tctrl2=0;
	C_FM_GUI_CTRL_DATA      *tctrldata=0;

    float FONT_CHAR_WIDTH=8.5;

    int i,j,k,x,y,x2,y2,w,h,w2,h2;
	i=j=k=x=y=x2=y2=w=h=w2=h2=0;

	long colr;
	int media_out;

	int r1,g1,b1;
	int r2,g2,b2;
	int r3,g3,b3;

    char temp[1024];
	memset(temp,0,1024);

	int media_selected;
	int media_unselected;
	int media_hover;

	RECT offrect;

	bool bcheck;

    if(focus_stump)
	{
        if(focus_control)
		{
            if(focus_control->parent_stump!=focus_stump)
			{
               focus_control=0;
			}
		}
	}

	if(focus_control)
        focus_stump->move_control_to_top(focus_control);

    tstump=first_gui_stump;

    while(tstump)
    {

        if(tstump->props & FM_GP_FULLSCREEN)
        {
            tstump->rect.left	= 0;
            tstump->rect.top	= 0;
            tstump->rect.bottom	= SDL_GetVideoSurface()->h;
            tstump->rect.right	= SDL_GetVideoSurface()->w;
        }
        else
        {


        }

        if(tstump->moving==true)
        {
            pGFX->DrawTransparentBar(

                                tstump->rect.left-4,
                                tstump->rect.top-4,
                                tstump->rect.left+tstump->rect.right+4,
                                tstump->rect.top+tstump->rect.bottom+4,
                                LONGRGB(240,240,240),
                                LONGRGB(240,240,240));
        }

        if(tstump->props & FM_GP_USEBACKGROUND)
        {
            pGFX->DrawBaseGFX(

                                tstump->rect.left,
                                tstump->rect.top,
                                tstump->rect.left+tstump->rect.right,
                                tstump->rect.top+tstump->rect.bottom,
                                atoi(tstump->media),
                                 255,255,255 );
        }
        else
        {
            draw_3d_box(tstump->rect);
        }

        /////////////////////////////////////////////
        // loop through controls of this gui stump

        tctrl=tstump->first_gui_control;

        while(tctrl)
        {
            if(tctrl!=focus_control)
                tctrl->draw();
            tctrl=tctrl->next;
        }


        if(tstump->props & FM_GP_MENU)
        {
            if(focus_stump==tstump)
            {

                pGFX->DrawBar(  tstump->rect.left+1,
                                tstump->rect.top+1,
                                tstump->rect.left+tstump->rect.right-1,
                                tstump->rect.top+18-1,
                                LONGRGB(0,60,160),
                                LONGRGB(60,0,60));
            }
            else
            {
                pGFX->DrawBar(  tstump->rect.left+1,
                                tstump->rect.top+1,
                                tstump->rect.left+tstump->rect.right-1,
                                tstump->rect.top+18-1,
                                LONGRGB(60,60,60),
                                LONGRGB(10,10,10));
            }

            WriteText(	tstump->rect.left+16,
                        tstump->rect.top+1,
                        va("^1%s",tstump->caption), 0);

            WriteText(	tstump->rect.left,
                        tstump->rect.top+1,
                        "R", 2);
        }

        if(tstump->props & FM_GP_CLOSEX)
        {
            if(pMouse->In(

                        tstump->rect.left+tstump->rect.right-16,
                        tstump->rect.top+1,
                        tstump->rect.left+tstump->rect.right,
                        tstump->rect.top+16))
            {
                WriteText(
                            tstump->rect.left+tstump->rect.right-16,
                            tstump->rect.top+1,

                            "^6X",2);

                if(pMouse->LeftClick())
                {
                    tstump->deleteme=1;
                    if(tstump==0) continue;
                }
            }
            else
            {

                WriteText(
                            tstump->rect.left+tstump->rect.right-16,
                            tstump->rect.top+1,

                            "^1X", 2);
            }
        }

        if(tstump->props & FM_GP_MINIMIZE)
        {
            DrawButton(79,0,(tstump->rect.left+tstump->rect.right)-17-16 ,tstump->rect.top+1);
        }

        if((tstump->props & FM_GP_MOVEABLE)>0)
        {
            if(tstump->moving==false)
            {
                if(pMouse->In(  tstump->rect.left,
                                tstump->rect.top,
                                tstump->rect.left + tstump->rect.right,
                                tstump->rect.top  + tstump->rect.bottom ) )
                {
                    if(pMouse->RightClick())
                    {
                        tstump->moving=true;
                        focus_stump=tstump;
                        tstump->mrect.left=(pMouse->X()-tstump->rect.left);
                        tstump->mrect.top =(pMouse->Y()-tstump->rect.top);
                    }
                }
            }
            else
            {
                tstump->rect.left=pMouse->X()-tstump->mrect.left;
                tstump->rect.top =pMouse->Y()-tstump->mrect.top;
                if(pMouse->RightClick())
                {
                    tstump->moving=false;
                }
            }
        }


        if(pMouse->In(  tstump->rect.left,
                        tstump->rect.top,
                        tstump->rect.left+tstump->rect.right,
                        tstump->rect.top+tstump->rect.bottom ) )
        {
            if(pMouse->LeftClick())
            {
                if(focus_stump==tstump)
                {
                    //return;
                }
                else
                {

                    focus_stump=tstump;
                    //return;
                }
            }
        }

        //FM_GP_NOPROPS
        //FM_GP_ALWAYS_ON_TOP
        //FM_GP_SIZEABLE
        //FM_GP_CLOSEX
        //FM_GP_MOVEABLE
        //FM_GP_BORDER
        //FM_GP_CLIENT_ONLY
        //FM_GP_ALWAYS_ON_BOTTOM
        //FM_GP_NO_RETURN
        //FM_GP_DO_NOT_DIE
        //FM_GP_TOGGLE_STATE



        tstump=tstump->next;
    }

    if(focus_control)
        focus_control->draw();


    tstump=first_gui_stump;

    while(tstump)
    {

        if(tstump->deleteme)
        {
            if(focus_stump==tstump) focus_stump=0;

            if(tstump->prev)
                tstump->prev->next=tstump->next;

            del_stump(tstump);
            return;
        }
        tstump=tstump->next;
    }

    // if(focus_control) focus_control->draw();
}

/********************************************************/

void C_FM_GUI::cleardata(void)
{
	C_FM_GUI_CTRL_DATA *pDelGUIData=0;
	pGUIData=pFirstGUIData;
	while(pGUIData)
	{
		pDelGUIData=pGUIData;
		pGUIData=pGUIData->next;
		DEL(pDelGUIData);
	}
	DEL(pDelGUIData);
	DEL(pFirstGUIData);
	pFirstGUIData=0;
}

/********************************************************/

void C_FM_GUI::parse(char *text)
{
    pLog->_Add("C_FM_GUI::parse start");

    cleardata();

    pGUIData=pFirstGUIData;
    if(!pGUIData)
    {
        pGUIData=new C_FM_GUI_CTRL_DATA();
        pFirstGUIData=pGUIData;
    }

	vector <string> break1;
	vector <string> break2;
	break1=explode("|",text);

	int i;
	for(i=0;i<break1.size();i++)
	{
        break2=explode("=",break1[i].c_str());

        if(break2.size()>1)
        {
			strcpy(pGUIData->name,  break2[0].c_str());
			strcpy(pGUIData->value, break2[1].c_str());
            pLog->_Add(" ParseGUIVars()-N-[%s] -V-[%s]",pGUIData->name,pGUIData->value);
			pGUIData->next=new C_FM_GUI_CTRL_DATA();
			pGUIData=pGUIData->next;
        }
	}

    pGUIData=pFirstGUIData;
    while(pGUIData)
    {
        pLog->_Add("   \\-name[%s]  value[%s]",pGUIData->name,pGUIData->value);
        pGUIData=pGUIData->next;
    }
}

/********************************************************/

char * C_FM_GUI::getdata(char *ctrlname)
{
    pGUIData=pFirstGUIData;
    while(pGUIData)
    {
        if(dscc(pGUIData->name,ctrlname))
        {
            return strdup(pGUIData->value);
        }
        pGUIData=pGUIData->next;
    }
    return strdup("NULL");
}

/********************************************************/

void C_FM_GUI::setdata(const char *x, const char *y)
{
    if(first_gui_stump)
        first_gui_stump->get_control(x)->set_value(y);
}

/********************************************************/

void C_FM_GUI::DrawB9utton(int x,int y, int w, int h)
{
	int i,row,col;
	int cx,cy;
	row=0;
	col=0;

	if(!B9utton)
	{
	    B9utton=new CGLTextureList[9];
	    if(!B9utton)		 return;
	}

	for(i=0;i<9;i++)
	{
		if(!B9utton[i].texture)
		{
			B9utton[i].texture=new CGLTexture();

		}
		if(!B9utton[i].texture->Loaded())
		{
			B9utton[i].texture->usemask=0;
			B9utton[i].texture->Load(pGAF,va("buttons/b9_%03d.bmp",i),0);
			return;
		}

		cx=x + (col*8);
		cy=y + (row*8);

		B9utton[i].texture->Draw(cx,cy,cx+8,cy+8,255,255,255);
		col++;
		if(col>2) { col=0; row++; }
	}
}

/********************************************************/

void C_FM_GUI::DrawGUIButton(int x, int y, int which, int size) // Draw a button at x,y in updown state
{
    if(!ButtonTexture)
    {
        return;
    }
    if(!ButtonTexture[which].texture)
    {
		ButtonTexture[which].texture=new CGLTexture();
        return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
		ButtonTexture[which].texture->usemask=1;
		ButtonTexture[which].texture->Load(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }

    ButtonTexture[which].texture->Draw(x,y,x+size,y+size,255,255,255);
}

/********************************************************/

void C_FM_GUI::DrawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,u_char r, u_char g, u_char b) // Draw a GUI resource at x,y
{
    if(!ButtonTexture)
    {
        return;
    }
    if(!ButtonTexture[which].texture)
    {
		ButtonTexture[which].texture=new CGLTexture();
        return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
		ButtonTexture[which].texture->usemask=1;
		ButtonTexture[which].texture->Load(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }
    ButtonTexture[which].texture->Draw(iX,iY,iX2,iY2,r,g,b);
}
/****************************************************************************************************/

void C_FM_GUI::DrawOSIcon(int iX,int iY,char *szOS)
{
    switch(szOS[0])
    {
        case 'W': //win32
            WriteText(iX,iY,"U",2);
            break;
        case 'L': //linux
            WriteText(iX,iY,"T",2);
            break;
        case 'F': // freebsd
            WriteText(iX,iY,"S",2);
            break;
        default: // what the?
            WriteText(iX,iY,"?",2);
            break;
    }
}


/********************************************************/

void C_FM_GUI::DrawButton(int which, int updown, int x, int y) // Draw a button at x,y in updown state
{
    DrawButton(which,updown,x,y,16,16);
}

void C_FM_GUI::DrawButton(int which, int updown, int x, int y,int w, int h)
{
    if(!ButtonTexture)
    {
        return;
    }
    if(!ButtonTexture[which].texture)
    {
		ButtonTexture[which].texture=new CGLTexture();
		return;
    }

    if(!ButtonTexture[which].texture->Loaded())
    {
		ButtonTexture[which].texture->usemask=0;
		ButtonTexture[which].texture->Load(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }

    ButtonTexture[which].texture->Draw(x,y,
		x+w,
		y+h,
		255,255,255);
}

/********************************************************/

int C_FM_GUI::DrawSlicedGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y) // Draw a button at x,y in updown state
{
    int j=0;
    int width=CGLFont_StrLen(gui_control->value)*8;
    DrawGUIButton(x,y,51,32);
    j+=32;
    while(width>16)
    {
		DrawGUIButton(x+j,y,52,32);
        j+=32;
        width-=32;
    }

    DrawGUIButton(x+j,y,53,32);

    gui_control->rect.right=x+j+32;
	gui_control->rect.bottom=32;

    if(pMouse->In(x,y,x+j,y+32) ) //Rect(gui_control->rect))
    {
        WriteText(x+12, y+6, va("^&^>aaaaff%s",gui_control->value),1);
    }
	else
    {
        WriteText(x+12, y+6, va("^&^>6666aa%s",gui_control->value),1);
    }
    return false;
}

/********************************************************/

int C_FM_GUI::DrawBaseGFXGUIButton(C_FM_GUI_CONTROL *gui_control,int x,int y)
{
    int j=0;
    pGFX->DrawBaseGFX(  x,
                        y,
                        gui_control->rect.right  +x,
                        gui_control->rect.bottom +y,
                        atoi(gui_control->media)   ,
                        255,255,255 );
    WriteText(gui_control->rect.left+8+x,gui_control->rect.top+8+y,gui_control->value,0);
    if(pMouse->In(gui_control->rect.left+x,gui_control->rect.top+y,gui_control->rect.right+x,gui_control->rect.bottom+y))
    {
        WriteText(gui_control->rect.left+8+x,gui_control->rect.top+8+y,va("^g%s",gui_control->value),0);
    }
    return false;
}

/********************************************************/

void C_FM_GUI::Prompt(char *szPrompt, char *szCommand)
{
    // ProcessConsoleCommand("play prompt.wav",0);
    strcpy(szPromptMsg,szPrompt);
    strcpy(szCommand,szCommand);
}

/********************************************************/

void C_FM_GUI::DrawPrompt(void)
{
    if(!CGLFont_StrLen(szPromptMsg)) return;
    pGFX->DrawBaseGFX(250,200,550,500,9,255,255,255);
    int i,j,k;
    char szTemp[1024];
    // TODO: if the message is larger than the window, wrap it somehow here, maybe add a scrollbar if necessary
    memset(szTemp,0,1024); j=0; k=0;
    for(i=0;i<CGLFont_StrLen(szPromptMsg);i++)
    {
        szTemp[j]=szPromptMsg[i];
        j++;
        if(j>25)
        {
            k++; j=0;
            WriteText(278,225+k*12,szTemp,0);
            memset(szTemp,0,1024);
        }
    }
    if(j!=0) { k++; WriteText(278,225+k*12,szTemp,0); }
    DrawButton(14,1,271,360,80,80);
    if(pMouse->In(271,360,318,380))
    {
        if(pMouse->LeftClick())
        {
            //// ProcessConsoleCommand(PlayMouseLB,0);
            memset(szPromptMsg,0,_MAX_PATH);
            return;
        }
    }
    DrawButton(13,1,482,360,80,80);
    if(pMouse->In(482,360,530,380))
    {
        if(pMouse->LeftClick())
        {
            //// ProcessConsoleCommand(PlayMouseLB,0);
            // ProcessConsoleCommand(szCommand,0);
            memset(szPromptMsg,0,_MAX_PATH);
            return;
        }
    }
 //   if(pMouse->In(250,200,550,400)) MOUSEMODE=MP_NORTHWEST;
}

/********************************************************/

bool C_FM_GUI::bIsPrompt(void)
{
    if(strlen(szPromptMsg))
        return true;
    return false;
}

/********************************************************/
bool C_FM_GUI::FlashCursor(int xi, int yi) // Get cursor time
{
    static bool bCursor;
    static unsigned long dwCursorTime=dlcs_get_tickcount();
    if((dlcs_get_tickcount()-dwCursorTime)>400)
    {
        dwCursorTime=dlcs_get_tickcount();
        if(bCursor) bCursor=false;
        else        bCursor=true;
    }
    return bCursor;
}


/********************************************************/

bool C_FM_GUI::InitButtons()
{
	int i;
	pLog->_Add("Begin ButtonTexture init...");

	B9utton=new CGLTextureList[9];
	if(!B9utton)
	{
		pLog->_Add("B9utton inititialization failure...");
		return false;
	}

	ButtonTexture=new CGLTextureList[MAX_BUTTONS];
	if(!ButtonTexture)
	{
		pLog->_Add("ButtonTexture initalization failure...");
		return false;
	}

	for(i=0;i<MAX_BUTTONS;i++)
		ButtonTexture[i].texture=0;

	for(i=0;i<9;i++)
		B9utton[i].texture=0;

	pLog->_Add("ButtonGFX initialized...");
	return 1;
}

/********************************************************/
bool C_FM_GUI::LoadButtons(char *szFilename)
{
	pLog->_Add("Begin ButtonTexture load...");
	DestroyButtons();
	if(!InitButtons())
	{
		pLog->_Add("ButtonTexture load failure...");
		return false;
	}
	pLog->_Add("ButtonTexture loaded...");
	return 1;
}
/********************************************************/

bool C_FM_GUI::DestroyButtons()
{
	int i;

	pLog->_Add("Begin ButtonTexture destroy...");

	for(i=0;i<9;i++)
	{
		DEL(B9utton[i].texture);
	}
	delete [] B9utton;
	B9utton=0;

	for(i=0;i<MAX_BUTTONS;i++)
	{
		DEL(ButtonTexture[i].texture);
	}
	delete [] ButtonTexture;
	ButtonTexture=0;

	pLog->_Add("Button Texures destroyed...");
	return 1;
}

/********************************************************/

bool C_FM_GUI::InitFonts()
{
    pLog->_Add("Begin Fonts init...");
    font=0;
    font=new CGLFontList[MAX_FONTS];
    if(!font)
    {
       pLog->_Add("Fonts initialization failure...");
        return false;
    }
    for(int i=0;i<MAX_FONTS;i++)
        font[i].ob=0;
    pLog->_Add("Fonts initialized...");
    return 1;
}

/********************************************************/

bool C_FM_GUI::LoadFonts(void)
{
    pLog->_Add("Begin Fonts load...");
    DestroyFonts();
    if(!InitFonts()) return false;
    bool loaded=1;

    for(int i=0;i<MAX_FONTS;i++)
    {

        if(!font[i].ob)
		{
			font[i].ob=new CGLFont(pGAF, pLog);
		}
        if(font[i].ob)
		{
            if(!font[i].ob->Load(va("fonts/fnt%02d",i)))
			{
                loaded=0;
			}
		}
    }

    if(font[2].ob)
	{
		font[2].ob->r=255;
		font[2].ob->g=255;
		font[2].ob->b=255;
	}

    if(loaded)
    {
        pLog->_Add("Fonts loaded...");
    }
    else
    {
        pLog->_Add("Fonts load failure...");
    }
    return loaded;
}

/********************************************************/

bool C_FM_GUI::DestroyFonts(void)
{
    pLog->_Add("Begin Fonts destroy...");
	if(!font) return 0;
    for(int i=0;i<MAX_FONTS;i++)
	{
		DEL(font[i].ob);
	}
    delete [] font; font=0;
    pLog->_Add("Fonts destroyed...");
    return 1;
}

/********************************************************/

//void WriteTextPct(float fX, float fY, char *szText,int fnt)//{ //	WriteText(  SDL_GetVideoSurface()->w*fX,//				SDL_GetVideoSurface()->h*fY,//				szText,//				fnt);//}

void C_FM_GUI::WriteText(int iX,int iY,const char *szText,int fnt)
{
    WriteText(iX,iY,szText,fnt,1);
}

/********************************************************/

void C_FM_GUI::WriteText(int iX,int iY,const char *szText,int fnt, int set)
{
    if(fnt<0) return;
	if(fnt>MAX_FONTS) return;

	if(!font) return;
	if(!font[fnt].ob) return;
	if(! (font[fnt].ob->pFontTex->Loaded()) )
	{
		font[fnt].ob->Load(va("fonts/fnt%02d",fnt));
		return;
	}
	font[fnt].ob->Print(iX-8.5,iY,szText,set); //fx,fy,szText,0); //
}

/********************************************************/

void C_FM_GUI::DrawFPS(int iX,int iY) //,long color)
{
    static unsigned long   FPS;
    static unsigned long   LastShowFPS;
    static unsigned long   FrameCount;
    if(dlcs_get_tickcount()-LastShowFPS>1000)
    {
        FPS=(long)FrameCount*1000/(dlcs_get_tickcount()-LastShowFPS);
        LastShowFPS=dlcs_get_tickcount();
        FrameCount=0;
    }
    FrameCount++;
    WriteText(iX,iY,va("^5FPS ^1[^+^5%d^1^+]",FPS),5);
}

/********************************************************/

void C_FM_GUI::DrawFileXferStatus(void) // Draw download stats at bottom of screen
{
    /*
    if(!pPlayer) return;
    if(!pPlayer->DownloadInfo.bIsDownloading) return;
    WriteText(10,310,va("Recieving file[%s]",pPlayer->DownloadInfo.szLocalXferName),LONGRGB(0,255,0));
    WriteText(10,320,va("File size[%d] u_chars",pPlayer->DownloadInfo.iFileSize),LONGRGB(0,255,0));
    WriteText(10,330,va("Progress[%d] u_chars",pPlayer->DownloadInfo.iTotalBytesRead),LONGRGB(0,255,0));
    // WriteText(10,340,va("Transfer Rate :%d u_chars per second",pPlayer->iBytesPerSecond),LONGRGB(0,255,0));
    DrawStatBar(10,345,150,(int)pPlayer->DownloadInfo.iTotalBytesRead,(int)pPlayer->DownloadInfo.iFileSize);
    */
}

/********************************************************/
int C_FM_GUI::DoInput()
{
    return ProcessKeyboard();
}
int C_FM_GUI::ProcessKeyboard()
{
    SDL_Event event;
    SDLMod modstate;

    bool bDone=0;

    SDLKey ikey;

    unsigned __int8 *keystate;

    /* Check for events */

    pMouse->Refresh();

    while ( SDL_PollEvent (&event) )
    {
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                pMouse->SetWheelUp(0);
                pMouse->SetWheelDown(0);
                if(event.button.button==SDL_BUTTON_WHEELUP)
                {
                    pMouse->SetWheelUp(1);
                }
                pMouse->SetWheelDown(0);
                if(event.button.button==SDL_BUTTON_WHEELDOWN)
                {
                    pMouse->SetWheelDown(1);
                }
                break;

            case SDL_KEYUP:

                keystate=SDL_GetKeyState(NULL);
                modstate=SDL_GetModState();
                ikey=event.key.keysym.sym;

                if(!focus_control)
                {
                    switch(ikey)
                    {
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_PAGEDOWN:
                        break;
                    case SDLK_PAGEUP:
                        break;
                    default:
                        return ikey;
                        break;
                    }
                }
                break;

            case SDL_KEYDOWN:

                keystate=SDL_GetKeyState(NULL);
                modstate=SDL_GetModState();
                ikey=event.key.keysym.sym;

                if( (ikey>='a') &&
                    (ikey<='z') &&
                    (modstate & KMOD_SHIFT) )
                    ikey=(SDLKey)(ikey-32);

                if(modstate & KMOD_SHIFT)
                {
                    switch(ikey)
                    {
                    case '1':
                        ikey=(SDLKey)'!';
                        break;
                    case '2':
                        ikey=(SDLKey)'@';
                        break;
                    case '3':
                        ikey=(SDLKey)'#';
                        break;
                    case '4':
                        ikey=(SDLKey)'$';
                        break;
                    case '5':
                        ikey=(SDLKey)'%';
                        break;
                    case '6':
                        ikey=(SDLKey)'^';
                        break;
                    case '7':
                        ikey=(SDLKey)'&';
                        break;
                    case '8':
                        ikey=(SDLKey)'*';
                        break;
                    case '9':
                        ikey=(SDLKey)'(';
                        break;
                    case '0':
                        ikey=(SDLKey)')';
                        break;
                    case ',':
                        ikey=(SDLKey)'<';
                        break;
                    case '.':
                        ikey=(SDLKey)'>';
                        break;
                    case ';':
                        ikey=(SDLKey)':';
                        break;
                    case '\'':
                        ikey=(SDLKey)'"';
                        break;
                    case '/':
                        ikey=(SDLKey)'?';
                        break;
                    case '[':
                        ikey=(SDLKey)'{';
                        break;
                    case ']':
                        ikey=(SDLKey)'}';
                        break;
                    case '-':
                        ikey=(SDLKey)'_';
                        break;
                    case '=':
                        ikey=(SDLKey)'+';
                        break;
                    case '\\':
                        ikey=(SDLKey)'|';
                        break;
                    }
                }

                if(modstate & KMOD_ALT)
                {
                    if(keystate[SDLK_RETURN])
                    {
                        pGFX->ToggleFullScreen();
                        break;
                    }
                }

                if(focus_control)
                {
                    if( (focus_control->type==FM_GC_TEXTBOX) ||
                        (focus_control->type==FM_GC_PASSWORD) )
                    {

                        if((ikey=='u') && (modstate & KMOD_CTRL))
                        {
                            memset(focus_control->value,0,1024);
                            return 0;
                        }

                        switch(ikey)
                        {
                        case '\t':
                            if(modstate & KMOD_SHIFT)
                            {
                                focus_control=focus_control->prev;
                            }
                            else
                            {
                                focus_control=focus_control->next;
                            }
                            break;

                        case 27:
                            focus_control=focus_control->next;
                            break;

                        case 13:

                            if(focus_control->parent_control)
                            {
                                if(focus_control->parent_control->type==FM_GC_CONSOLE)
                                {
                                    _ConsoleExecute(focus_control->value);
                                    memset(focus_control->value,0,1024);
                                    break;
                                }
                            }

                            focus_control=focus_control->next;

                            break;

                        case 8:
                            if(!CGLFont_StrLen(focus_control->value))
                                break;
                            focus_control->value[strlen(focus_control->value)-1]=0;
                            break;

                        default:
                            if( (ikey>=' ') &&
                                (ikey<='~') &&
                                (CGLFont_StrLen(focus_control->value)<focus_control->max_val_len) )
                            {
                                if(strlen(focus_control->value)>focus_control->max_val_len)
                                    break;
                                focus_control->value[strlen(focus_control->value)+1]=0;
                                focus_control->value[strlen(focus_control->value)]=ikey;
                            }
                            break;
                        }
                    }
                }

                    break;

        case SDL_QUIT:
            bDone = 1;
            break;
        default:
            break;
        }
    }
    if((event.type) == SDL_KEYUP)
        return ikey;
        /* */
    return 0;
}





//////////////////////////////////////////////////////
bool C_FM_GUI::bSaveBinds(char *szFilename)
{

    FILE *fout;
    int ikey;
    char szTemp[_MAX_PATH];

    fout=fopen(szFilename,"wt");
    if(!fout)
        return false;

    fputs("## Ember key bindings file.\n",fout);

    for(ikey=0;ikey<SDLK_LAST;ikey++)
    {
        strcpy(szTemp,"bind ");
        if(ikey == SDLK_ESCAPE) strcat(szTemp,"esc ");
        if(ikey == SDLK_1) strcat(szTemp,"1 ");
        if(ikey == SDLK_2) strcat(szTemp,"2 ");
        if(ikey == SDLK_3) strcat(szTemp,"3 ");
        if(ikey == SDLK_4) strcat(szTemp,"4 ");
        if(ikey == SDLK_5) strcat(szTemp,"5 ");
        if(ikey == SDLK_6) strcat(szTemp,"6 ");
        if(ikey == SDLK_7) strcat(szTemp,"7 ");
        if(ikey == SDLK_8) strcat(szTemp,"8 ");
        if(ikey == SDLK_9) strcat(szTemp,"9 ");
        if(ikey == SDLK_0) strcat(szTemp,"0 ");
        if(ikey == SDLK_MINUS) strcat(szTemp,"- ");
        if(ikey == SDLK_EQUALS) strcat(szTemp,"= ");
        if(ikey == SDLK_BACKSPACE) strcat(szTemp,"backspace ");
        if(ikey == SDLK_TAB) strcat(szTemp,"tab ");
        if(ikey == SDLK_q) strcat(szTemp,"q ");
        if(ikey == SDLK_w) strcat(szTemp,"w ");
        if(ikey == SDLK_e) strcat(szTemp,"e ");
        if(ikey == SDLK_r) strcat(szTemp,"r ");
        if(ikey == SDLK_t) strcat(szTemp,"t ");
        if(ikey == SDLK_y) strcat(szTemp,"y ");
        if(ikey == SDLK_u) strcat(szTemp,"u ");
        if(ikey == SDLK_i) strcat(szTemp,"i ");
        if(ikey == SDLK_o) strcat(szTemp,"o ");
        if(ikey == SDLK_p) strcat(szTemp,"p ");
        if(ikey == SDLK_LEFTBRACKET) strcat(szTemp,"[ ");
        if(ikey == SDLK_RIGHTBRACKET) strcat(szTemp,"] ");
        if(ikey == SDLK_RETURN) strcat(szTemp,"enter ");
        if(ikey == SDLK_LCTRL) strcat(szTemp,"l_control ");
        if(ikey == SDLK_a) strcat(szTemp,"a ");
        if(ikey == SDLK_s) strcat(szTemp,"s ");
        if(ikey == SDLK_d) strcat(szTemp,"d ");
        if(ikey == SDLK_f) strcat(szTemp,"f ");
        if(ikey == SDLK_g) strcat(szTemp,"g ");
        if(ikey == SDLK_h) strcat(szTemp,"h ");
        if(ikey == SDLK_j) strcat(szTemp,"j ");
        if(ikey == SDLK_k) strcat(szTemp,"k ");
        if(ikey == SDLK_l) strcat(szTemp,"l ");
        if(ikey == SDLK_SEMICOLON) strcat(szTemp,"; ");
        if(ikey == SDLK_QUOTE) strcat(szTemp,"' ");
        //if(ikey == SDLK_BACKQUOTE) strcat(szTemp,"` ");
        if(ikey == SDLK_LSHIFT) strcat(szTemp,"l_shift ");
        if(ikey == SDLK_BACKSLASH) strcat(szTemp,"\\ ");
        if(ikey == SDLK_z) strcat(szTemp,"z ");
        if(ikey == SDLK_x) strcat(szTemp,"x ");
        if(ikey == SDLK_c) strcat(szTemp,"c ");
        if(ikey == SDLK_v) strcat(szTemp,"v ");
        if(ikey == SDLK_b) strcat(szTemp,"b ");
        if(ikey == SDLK_n) strcat(szTemp,"n ");
        if(ikey == SDLK_m) strcat(szTemp,"m ");
        if(ikey == SDLK_COMMA) strcat(szTemp,", ");
        if(ikey == SDLK_PERIOD) strcat(szTemp,". ");
        if(ikey == SDLK_SLASH) strcat(szTemp,"/ ");
        if(ikey == SDLK_RSHIFT) strcat(szTemp,"r_shift ");
        if(ikey == SDLK_KP_MULTIPLY) strcat(szTemp,"* ");
        if(ikey == SDLK_LALT) strcat(szTemp,"l_alt ");
        if(ikey == SDLK_SPACE) strcat(szTemp,"space ");
        if(ikey == SDLK_CAPSLOCK) strcat(szTemp,"capslock ");
        if(ikey == SDLK_F1) strcat(szTemp,"f1 ");
        if(ikey == SDLK_F2) strcat(szTemp,"f2 ");
        if(ikey == SDLK_F3) strcat(szTemp,"f3 ");
        if(ikey == SDLK_F4) strcat(szTemp,"f4 ");
        if(ikey == SDLK_F5) strcat(szTemp,"f5 ");
        if(ikey == SDLK_F6) strcat(szTemp,"f6 ");
        if(ikey == SDLK_F7) strcat(szTemp,"f7 ");
        if(ikey == SDLK_F8) strcat(szTemp,"f8 ");
        if(ikey == SDLK_F9) strcat(szTemp,"f9 ");
        if(ikey == SDLK_F10) strcat(szTemp,"f10 ");
        if(ikey == SDLK_F11) strcat(szTemp,"f11 ");
        if(ikey == SDLK_F12) strcat(szTemp,"f12 ");
        if(ikey == SDLK_F13) strcat(szTemp,"f13 ");
        if(ikey == SDLK_F14) strcat(szTemp,"f14 ");
        if(ikey == SDLK_F15) strcat(szTemp,"f15 ");
        if(ikey == SDLK_NUMLOCK) strcat(szTemp,"numlock ");
        if(ikey == SDLK_SCROLLOCK) strcat(szTemp,"scroll ");
        if(ikey == SDLK_KP7) strcat(szTemp,"numpad_7 ");
        if(ikey == SDLK_KP8) strcat(szTemp,"numpad_8 ");
        if(ikey == SDLK_KP9) strcat(szTemp,"numpad_9 ");
        if(ikey == SDLK_KP_MINUS) strcat(szTemp,"- ");
        if(ikey == SDLK_KP4) strcat(szTemp,"numpad_4 ");
        if(ikey == SDLK_KP5) strcat(szTemp,"numpad_5 ");
        if(ikey == SDLK_KP6) strcat(szTemp,"numpad_6 ");
        if(ikey == SDLK_KP_PLUS) strcat(szTemp,"+ ");
        if(ikey == SDLK_KP1) strcat(szTemp,"numpad_1 ");
        if(ikey == SDLK_KP2) strcat(szTemp,"numpad_2 ");
        if(ikey == SDLK_KP3) strcat(szTemp,"numpad_3 ");
        if(ikey == SDLK_KP0) strcat(szTemp,"numpad_0 ");
        if(ikey == SDLK_KP_PERIOD) strcat(szTemp,"numpad_decimal ");
        if(ikey == SDLK_KP_EQUALS) strcat(szTemp,"numpad_equals ");
        if(ikey == SDLK_AT) strcat(szTemp,"at ");
        if(ikey == SDLK_COLON) strcat(szTemp,"colon ");
        if(ikey == SDLK_UNDERSCORE) strcat(szTemp,"underscore ");
        if(ikey == SDLK_KP_ENTER) strcat(szTemp,"numpad_enter ");
        if(ikey == SDLK_RCTRL) strcat(szTemp,"r_control ");
        if(ikey == SDLK_KP_DIVIDE) strcat(szTemp,"numpad_divide ");
        if(ikey == SDLK_RALT) strcat(szTemp,"r_alt ");
        if(ikey == SDLK_PAUSE) strcat(szTemp,"pause ");
        if(ikey == SDLK_HOME) strcat(szTemp,"home ");
        if(ikey == SDLK_UP) strcat(szTemp,"up ");
        if(ikey == SDLK_PAGEUP) strcat(szTemp,"pageup ");
        if(ikey == SDLK_LEFT) strcat(szTemp,"left ");
        if(ikey == SDLK_RIGHT) strcat(szTemp,"right ");
        if(ikey == SDLK_END) strcat(szTemp,"end ");
        if(ikey == SDLK_DOWN) strcat(szTemp,"down ");
        if(ikey == SDLK_PAGEDOWN) strcat(szTemp,"pagedown ");
        if(ikey == SDLK_INSERT) strcat(szTemp,"insert ");
        if(ikey == SDLK_DELETE) strcat(szTemp,"delete ");
        if(ikey == SDLK_LSUPER) strcat(szTemp,"l_windows ");
        if(ikey == SDLK_RSUPER) strcat(szTemp,"r_windows ");
        if(ikey == SDLK_POWER) strcat(szTemp,"power ");

        if(strcmp(szTemp,"bind "))
        {
            //if(strlen(Key[ikey].Command))
            //{
                //strcat(szTemp,Key[ikey].Command);
                //strcat(szTemp,"\n");
                //fputs(szTemp,fout);
                //pLog->_Add("%d %s",ikey,szTemp);
            //}
        }
    }

    fclose(fout);
    return true;
}



/****************************************************************************************************/

void C_FM_GUI::DrawStatBar(int iX,int iY,int iW, int iT,int iV) // iT is total value, iV is current value, will draw percentage bar
{
    pGFX->DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    pGFX->DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    pGFX->DrawBar(iX,iY,iX+(int)fPercent,iY+3,LONGRGB(0,100,0),LONGRGB(0,100,0));
}

/****************************************************************************************************/

void C_FM_GUI::DrawCStatBar(int iX,int iY,int iW, int iT,int iV,long Color1,long Color2) // iT is total value, iV is current value, will draw percentage bar
{
    pGFX->DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    pGFX->DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    pGFX->DrawBar(iX,iY,iX+(int)fPercent,iY+3,Color1,Color2);
}


/***********************************************/

void C_FM_GUI::ClearKeys(void)
{
    int i;
    for(i=0;i<SDLK_LAST;i++)
    {
//        memset(Key[i].Command,0,256);
    }

}

void C_FM_GUI::_ConsoleExecute(char *cmd)
{
        pConsole->_Execute(cmd);
}

