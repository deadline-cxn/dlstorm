/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL GUI class

****************************************************************/

#include "c_gui.h"

C_GCTRL::C_GCTRL(C_GSTMP* pInParentStump, CLog* pInLog, C_GFX* pInGFX, C_GUI* pInGUI, C_CONS* pInConsole) {
    parent_stump=pInParentStump;
    pLog=pInLog;
    pGFX=pInGFX;
    pGUI=pInGUI;
    pCons=pInConsole;
    init_ctrl();
}
C_GCTRL::C_GCTRL(C_GCTRL* pInParent) {
    parent_control=pInParent;
    parent_stump=parent_control->parent_stump;
    pLog=parent_control->pLog;
    pGFX=parent_control->pGFX;
    pGUI=parent_control->pGUI;
    pCons=parent_control->pCons;
    init_ctrl();
}
C_GCTRL::~C_GCTRL() {
    clear_control_data();
    C_GCTRL* dc=first_child_control;
    child_control=dc;
    while(child_control) {
        dc=child_control;
        child_control=child_control->next;
        dlcsm_delete(dc);
    }
}
void C_GCTRL::init_ctrl() {
    memset(name,0,1024);
    zeroize_ctrl();
    data.clear();
    buf.clear();
    visible=1;
    parent_control=0;
    first_child_control=0;
    child_control=0;
    next=0;
    prev=0;
}
void C_GCTRL::zeroize_ctrl() {
    props=0;
    selected=0;
    memset(value,0,1024);
    memset(group,0,1024);
    memset(action,0,1024);
    memset(console,0,1024);
    memset(datafill,0,1024);
    memset(datafilltarget,0,1024);
    iRelativeTo=0;
    memset(media,0,1024);
    memset(media_hover,0,1024);
    memset(media_click,0,1024);
    memset(hover_text,0,1024);
    memset(nexttabfocus,0,64);
    background_color=0;
    border_color=0;
    max_val_len=256;
    listdepth=0;
    next=NULL;
    prev=NULL;
    font=0;
    fontbank=0;
    rect.top=0;
    rect.left=0;
    rect.right=0;
    rect.bottom=0;
    mrect.top=0;
    mrect.left=0;
    mrect.right=0;
    mrect.bottom=0;
    rect_2.top=0;
    rect_2.left=0;
    rect_2.right=0;
    rect_2.bottom=0;
    visible=false;
    bMoving=false;
    bSizing=false;
    bEditing=false;
    type=0;
    listdepth=0;
    listoffset=0;
    selecting=0;
}
void C_GCTRL::set_value(const char* val) {
    strcpy(value,val);
    parent_stump->data[name]=val;
}
void C_GCTRL::get_value(char* str) {
    strcpy(str,value);
}
char* C_GCTRL::get_value(void) {
    return value;
}
void C_GCTRL::clear_control_data(void) {
    data.clear();
}
void C_GCTRL::control_data_unselect(void) {
    data_selected.clear();
}
void C_GCTRL::selecta(void) {
    this->parent_stump->set_group_selecta(this);
}
void C_GCTRL::put_control_data(const char* tnname, const char* nvalue) {
    control_data_total=data.size();
    control_data_total++;
    data[control_data_total]=nvalue;
    //data[tnname]=nvalue;
}
void C_GCTRL::sort_control_data(const char method) {
    switch(method) {
    case FM_GSM_ALPHA_ASCEND:
        break;
    case FM_GSM_ALPHA_DESCEND:
        break;
    default:
        break;
    }
}
void C_GCTRL::hide_default_children(void) {
    int i;
    switch(type) {
    case FM_GC_LOCAL_DIR:
    case FM_GC_CONSOLE:
        break;

    default:
        for(i=0; i<listdepth; i++) {
            if(parent_stump->get_control(va("%s_list_%d",name,i)))
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=false;
            if(parent_stump->get_control(va("%s_list_image_%d",name,i)))
                parent_stump->get_control(va("%s_list_image_%d",name,i))->visible=false;
        }
        if(parent_stump->get_control(va("%s_scrollbar",name)))
            parent_stump->get_control(va("%s_scrollbar",name))->visible=false;
        if(parent_stump->get_control(va("%s_input",name)))
            parent_stump->get_control(va("%s_input",name))->visible=false;

        break;
    }
}
void C_GCTRL::show_default_children(void) {
    int i;
    switch(type) {
    case FM_GC_CONSOLE:
        break;

    default:
        for(i=0; i<listdepth; i++) {
            if(parent_stump->get_control(va("%s_list_%d",name,i)))
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
            if(parent_stump->get_control(va("%s_list_image_%d",name,i)))
                parent_stump->get_control(va("%s_list_image_%d",name,i))->visible=true;
        }

        if(data.size() > listdepth)
            if(parent_stump->get_control(va("%s_scrollbar",name)))
                parent_stump->get_control(va("%s_scrollbar",name))->visible=true;

        if(parent_stump->get_control(va("%s_input",name)))
            parent_stump->get_control(va("%s_input",name))->visible=true;
        break;
    }
}
void C_GCTRL::attach_default_children(void) {
    int i,j;
    char temp[1024];
    vector <string> dirs;

    pLog->_DebugAdd("C_GCTRL::attach_default_children 1");

    ///////////////////

    if(type==FM_GC_BASE_SELECT) {
        data.clear();
        listdepth=6;

    /*  for(i=0;i<MAX_BASE_GFX;i++){
            if(pGFX->BaseTexture){
                if(pGFX->BaseTexture[i].texture){
                    if(pGFX->BaseTexture[i].texture->bmap){
                        put_control_data("what",(char* )va("%d",i));
                    }
                }
            }else{
                put_control_data("what","NULL");
            }
        }  */

        for(i=0; i<listdepth; i++) {
            parent_stump->add_control(  this,
                                        va("%s_list_%d",name,i),
                                        FM_GC_STATIC_TEXT,
                                        rect.left+64,
                                        rect.top+16+i*32+5,
                                        5,
                                        5,
                                        0, "", temp);

            if( parent_stump->get_control(va("%s_list_%d",name,i))) {
                parent_stump->get_control(va("%s_list_%d",name,i))->font=8;
                parent_stump->get_control(va("%s_list_%d",name,i))->fontbank=1;
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=false;
            }

            parent_stump->add_control(  this,
                                        va("%s_list_image_%d",name,i),
                                        FM_GC_STATIC_IMAGE,
                                        rect.left+4,
                                        rect.top+4+i*32,
                                        32,
                                        32,
                                        0,"", temp);

            if(parent_stump->get_control(va("%s_list_image_%d",name,i))) {
                parent_stump->get_control(va("%s_list_image_%d",name,i))->visible=false;
            }
        }
        control_data_total=data.size();
    }

    ///////////////////

    if(type==FM_GC_LOCAL_DIR) {
        if(pGUI->last_control_clicked) {
            pLog->_DebugAdd("[%s][%s][%s][%s][%s]",  pGUI->last_control_clicked->parent_stump->name,
                            pGUI->last_control_clicked->name,
                            pGUI->last_control_clicked->datafilltarget,
                            pGUI->last_control_clicked->datafill,
                            pGUI->last_control_clicked->action);
            strcpy(datafill,pGUI->last_control_clicked->datafill);
            strcpy(datafilltarget,pGUI->last_control_clicked->datafilltarget);
            strcpy(action,pGUI->last_control_clicked->action);
        }
        if(strlen(action)) {
            dlcs_getcwd(temp);
            // strcpy(temp,dlcs_getcwd());
            dlcs_chdir(action);
            dirs=dlcs_dir_to_vector(temp,"*.*");
            dlcs_chdir(temp);
        } else {
            dlcs_getcwd(temp);
            dirs=dlcs_dir_to_vector(temp,"*.*");
        }

        sort(dirs.begin(), dirs.end());
        data.clear();
        for(i=0; i<dirs.size(); i++)
            put_control_data("local_dir",(char*)dirs[i].c_str());
        listdepth=(rect.bottom)/16;
        listoffset=0;
        for(i=0; i<listdepth; i++) {
            parent_stump->add_control(  this,    va("%s_list_%d",name,i), FM_GC_STATIC_TEXT, 0, 16+i*16, rect.right, 16+i*16+16, 0, "", temp);
            if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                parent_stump->get_control(va("%s_list_%d",name,i))->font=8;
                parent_stump->get_control(va("%s_list_%d",name,i))->fontbank=1;
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
            }
        }
        parent_stump->add_control(  this, va("%s_scrollbar",name), FM_GC_H_SCROLLBAR, rect.left+rect.right+2, 16, 16, ((listdepth-2)*16)+16, 0, "", "");
    }

    ///////////////////

    if(type==FM_GC_DROPBOX) {
        data_selected.clear();
        parent_stump->add_control(  this,
                                    va("%s_scrollbar",name),
                                    FM_GC_H_SCROLLBAR,
                                    rect.left+rect.right+3,
                                    rect.top+17,
                                    16,
                                    ((listdepth-2)*16)+17,
                                    0,
                                    "",
                                    "");

        if(parent_stump->get_control(va("%s_scrollbar",name))) {
            parent_stump->get_control(va("%s_scrollbar",name))->visible=false;
        }
    }

    ///////////////////

    if(type==FM_GC_CHATBOX) {

        for(i=0; i<15; i++) {
            buf.push_back(va("Here is some chat! %i",i));
            put_control_data(" ",buf[i].c_str());
        }

        for(i=0; i<listdepth; i++) {
            parent_stump->add_control(  this,va("%s_list_%d",name,i),FM_GC_STATIC_TEXT,
                                        4,//rect.left+16+2,
                                        //rect.top+
                                        4+i*16,
                                        5,//rect.right,
                                        5,//i*16+32,
                                        0, "", "");

            if( parent_stump->get_control(va("%s_list_%d",name,i))) {
                parent_stump->get_control(va("%s_list_%d",name,i))->font=1;
                parent_stump->get_control(va("%s_list_%d",name,i))->fontbank=1;
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
            }

            if( parent_stump->get_control(va("%s_list_%d",name,i))) {
                parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
            }
        }



    }

    ///////////////////

    if(type==FM_GC_CONSOLE) {
        pLog->_DebugAdd("C_GCTRL::attach_default_children FM_GC_CONSOLE");

        if(pCons) {
            for(i=0; i<pCons->buf.size(); i++)
                put_control_data("listentry",(char*)pCons->buf[i].c_str());
            pLog->_DebugAdd("ADDING CONSOLE CHILDREN: filling listdepth");
            for(i=0; i<listdepth; i++) {
                strcpy(temp,(char*)pCons->buf[i].c_str());
                parent_stump->add_control(  this,va("%s_list_%d",name,i),FM_GC_STATIC_TEXT,
                                            rect.left+16+2,
                                            i*16+16,
                                            5,//rect.right,
                                            5,//i*16+32,
                                            0, "", temp);

                if( parent_stump->get_control(va("%s_list_%d",name,i))) {
                    parent_stump->get_control(va("%s_list_%d",name,i))->font=5;
                    parent_stump->get_control(va("%s_list_%d",name,i))->fontbank=1;
                    parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
                }

                if( parent_stump->get_control(va("%s_list_%d",name,i))) {
                    parent_stump->get_control(va("%s_list_%d",name,i))->visible=true;
                }
            }

            pLog->_DebugAdd("ADDING CONSOLE CHILDREN: creating scrollbar");

            if(!parent_stump->get_control(va("%s_scrollbar",name))) {
                parent_stump->add_control( this,va("%s_scrollbar",name),FM_GC_H_SCROLLBAR,
                                           rect.left,
                                           16,
                                           16,
                                           (listdepth-1)*16,
                                           0, "","");
                if(parent_stump->get_control(va("%s_scrollbar",name))) {
                    parent_stump->get_control(va("%s_scrollbar",name))->visible=true;
                    parent_stump->get_control(va("%s_scrollbar",name))->parent_control=this;
                }
            }
            pLog->_DebugAdd("ADDING CONSOLE CHILDREN: creating input (%s)",va("%s_input",name));
            if(!parent_stump->get_control(va("%s_input",name))) {
                parent_stump->add_control( this,va("%s_input",name),FM_GC_TEXTBOX,
                                           rect.left,
                                           rect.bottom+1,
                                           rect.right,
                                           16,     0, "","");
            }
            listoffset = control_data_total - listdepth;
        }
        pLog->_DebugAdd("C_GCTRL::attach_default_children 555");
    }

    ///////////////////
}
void C_GCTRL::populate_ctrl_editor(void) {
    //if(!bEditing) return;
    C_GSTMP* tstump=parent_stump;
    map <string, int>::iterator ii;
    char temp[1024];
    memset(temp,0,1024);
    if(!tstump) return;

    pGUI->setdata("guictrledit.gui","parentstumpname",tstump->name);
    pGUI->setdata("guictrledit.gui","name",name);
    pGUI->setdata("guictrledit.gui","value",get_value());


    for( ii=pGUI->GC_TYPE.begin(); ii!=pGUI->GC_TYPE.end(); ++ii)
        if( ( (*ii).second ) == (type) )
            strcpy(temp,(const char*)(*ii).first.c_str());


    pGUI->setdata("guictrledit.gui","type",temp);
    pGUI->setdata("guictrledit.gui","x",(char*)va("%d",rect.left));
    pGUI->setdata("guictrledit.gui","y",(char*)va("%d",rect.top));
    pGUI->setdata("guictrledit.gui","w",(char*)va("%d",rect.right));
    pGUI->setdata("guictrledit.gui","h",(char*)va("%d",rect.bottom));


    for( ii=pGUI->GC_PROP.begin(); ii!=pGUI->GC_PROP.end(); ++ii) {
        if((((*ii).second) & (props)) == ((*ii).second)) {
            strcpy(temp,(const char*)(*ii).first.c_str());
            pGUI->setdata("guictrledit.gui",temp,"on");
        } else {
            strcpy(temp,(const char*)(*ii).first.c_str());
            pGUI->setdata("guictrledit.gui",temp,"off");
            // (char *)va("%d",(*ii).first&props
        }
    }
    //pGUI->GC_PROP[FM_GP_BORDER].c_str();
    // (char *)va("%d",props);

}
char* C_GCTRL::convert$(char* in) {
    int i,j;
    vector <string> varswap;
    char temp[1024];
    char temp2[1024];
    char temp3[1024];
    memset(temp,0,1024);
    memset(temp2,0,1024);
    memset(temp3,0,1024);
    strcpy(temp2," ");
    varswap=dlcs_explode(" ",in);
    for(i=0; i<varswap.size(); i++) {
        strcpy(temp,varswap[i].c_str());
        if(temp[0]=='$') {
            for(j=0; j<strlen(temp); j++) temp[j]=temp[j+1];
            strcpy(temp3,this->parent_stump->data[temp].c_str());
            pLog->_DebugAdd("SWAPPED [%s] to [%s]",varswap[i].c_str(),temp3);
            strcat(temp2,temp3);
        } else
            strcat(temp2,(char*)varswap[i].c_str());
        strcat(temp2," ");
    }
    temp2[strlen(temp2)-1]=0;
    for(j=0; j<strlen(temp2); j++) temp2[j]=temp2[j+1];
    return temp2;
}

void C_GCTRL::checkMouseClicks(bool bMouseWheelUp, bool bMouseWheelDown) {
    C_GCTRL* tctrl2;
    C_GSTMP* tstump;
    C_GSTMP* tstump2;
    C_Mouse* pMouse;
    C_GCTRL* pctrl;
    tctrl2=0;
    pctrl=0;

    char temp[1024];
    memset(temp,0,1024);
    char temp2[1024];
    memset(temp2,0,1024);
    char temp3[1024];
    memset(temp3,0,1024);

    map <string, int>::iterator ii;
    tstump=parent_stump;
    pMouse=pGUI->pMouse;
    int x,y,w,h,i,j,k;
    x  = tstump->rect.left + rect.left;
    y  = tstump->rect.top + rect.top;
    switch(iRelativeTo) {
    case GUI_TOP_CENTER:
        break;

    case GUI_TOP_RIGHT:
        break;

    case GUI_MIDDLE_LEFT:
        break;

    case GUI_MIDDLE_CENTER:
        x+=parent_stump->rect.right/2;
        y+=parent_stump->rect.bottom/2;
        x-=rect.right/2;
        y-=rect.bottom/2;
        break;

    case GUI_MIDDLE_RIGHT:
        break;

    case GUI_BOTTOM_LEFT:
        break;

    case GUI_BOTTOM_CENTER:
        break;

    case GUI_BOTTOM_RIGHT:
        break;

    case GUI_TOP_LEFT:
    default:
        break;
    }

    w  = x+rect.right;
    h  = y+rect.bottom;
    if(pGUI->bDebug) pGFX->DrawRect(x-2,y-2,w+2,h+2,RGB(0,0,pGFX->GetFade(4)));
    if(bMoving) {
        bSizing=false;
        rect.left=pMouse->X()-mrect.left;
        rect.top =pMouse->Y()-mrect.top;
    }
    if(bSizing) {
        bMoving=false;
        pGFX->DrawRect(x-1,y-1,w+2,h+2, LONGRGB(155,155,pGFX->GetFade(2)));
        rect.right=pMouse->X()-mrect.right;
        rect.bottom=pMouse->Y()-mrect.bottom;
        //listdepth=(rect.top-rect.bottom)/8;

    }

    if(pGUI->pMouse->In(x,y,w,h)) {
        if(parent_stump->bEditing) {
            pGFX->DrawTransparentBar(x,y,w,h,RGB(255,255,0),RGB(255,255,0));
        }

    }



    if(strlen(hover_text)) {
        if(type==FM_GC_DROPBOX)
            if(pGUI->focus_control == this)
                if(pMouse->In(x,y ,w,y+(listdepth*16)+16))
                    pGUI->bResetToolTip=true;

        if(type==FM_GC_BASE_SELECT)
            if(pGUI->focus_control != this) {
                w=x+16;
                h=y+16;
            }

        if(pGUI->pMouse->In(x,y,w,h)) {
            if(!pGUI->bDrawToolTip) {
                pGUI->bDrawToolTip=true;
                if(pGUI->bStaticToolTip) {
                } else {
                    pGUI->rectToolTip.left   = pGUI->pMouse->X()+16;
                    pGUI->rectToolTip.top    = pGUI->pMouse->Y()+16;
                    pGUI->rectToolTip.right  = pGUI->rectToolTip.left+200;
                    pGUI->rectToolTip.bottom = pGUI->rectToolTip.top+50;
                }
                strcpy(pGUI->hover_text,hover_text);
            }
        }
    }

    tctrl2=first_child_control;
    if(tctrl2)
        while(tctrl2->next) tctrl2=tctrl2->next;
    while(tctrl2) {
        tctrl2->checkMouseClicks(bMouseWheelUp,bMouseWheelDown);
        tctrl2=tctrl2->prev;
    }

    if(pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
        if(pMouse->In(x,y,w-6,h-6)) {

            if(!pGUI->bCtrlSizing)
                if(!pGUI->bCtrlMoving)
                    if(!pGUI->bStmpMoving)
                        if(!pGUI->bStmpSizing) {
                            pGUI->bCtrlMoving=true;
                            bMoving=true;
                            bSizing=false;
                            pMouse->ClearClicks();
                            pGUI->focus_control=this;
                            pGUI->focus_stump=this->parent_stump;
                            mrect.left=(pMouse->X()-rect.left);
                            mrect.top =(pMouse->Y()-rect.top);
                        }
        }
        if(pMouse->In(w-6,h-6,w+2,h+2)) {
            if(!pGUI->bCtrlSizing)
                if(!pGUI->bCtrlMoving)
                    if(!pGUI->bStmpMoving)
                        if(!pGUI->bStmpSizing)

                        {
                            pGUI->bCtrlSizing=true;
                            bSizing=true;
                            pMouse->ClearClicks();
                            bMoving=false;
                            pGUI->focus_control=this;
                            pGUI->focus_stump=this->parent_stump;
                            mrect.right=pMouse->X()-rect.right;
                            mrect.bottom=pMouse->Y()-rect.bottom;
                        }
        }
    }

    if(!pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
        if(bMoving) {
            pGUI->bCtrlMoving=false;
            bMoving=false;
        }
        if(bSizing) {
            pGUI->bCtrlSizing=false;
            bSizing=false;
        }
    }

    if(pMouse->RightClick()) {
        if(pMouse->In(x,y,w,h))
            populate_ctrl_editor();
    }

    if(pMouse->LeftClick()) {
        pLog->_DebugAdd("CHECK MOUSE CLICKS : CLICKED (%s) (%s) (x=%d,y=%d,w=%d,h=%d) GROUP[%s] CONSOLE[%s]",tstump->name,name ,x,y,w,h, group,console);

        if(parent_stump!=pGUI->focus_stump) {
            if(pMouse->In(x,y,w,h)) {
                pGUI->set_focus_stump(parent_stump);
                pGUI->set_focus_control(parent_stump->get_control(parent_stump->default_focus_control));
                pMouse->ClearClicks();
            }
        } else
            switch(type) {
            case FM_GC_LOCAL_DIR:
                for(j=0; j<listdepth; j++) {
                    if(pMouse->In(x,y+(j*16),w,y+(j*16)+16)) {
                        pLog->_DebugAdd((char*)data[j+listoffset].c_str());
                        if(strlen(datafill)) {
                            tstump2=parent_stump;
                            if(strlen(datafilltarget)) {
                                if(pGUI->get_stump(datafilltarget)) {
                                    tstump2=pGUI->get_stump(datafilltarget);
                                }
                            }
                            if(tstump2)
                                if(tstump2->get_control(datafill)) {
                                    tstump2->get_control(datafill)->set_value((char*)data[j+listoffset].c_str());
                                }
                        }
                        if(!(parent_stump->props&FM_GP_DO_NOT_DIE)) parent_stump->bDeleteMe=true;
                    }
                }
                break;

            case FM_GC_BASE_SELECT:
                if(pGUI->focus_control==this) {
                    for(j=0; j<listdepth; j++) {
                        if(pMouse->In(x-1,y+(j*32)+4,w+1,y+(j*32)+32+4)) {
                            pGFX->DrawBar(x-1,y+(j*32)+4,w+1,y+(j*32)+32+4,LONGRGB(234,155,25));
                            if(data[j+listoffset].c_str()) {
                                pMouse->ClearClicks();
                                pGUI->last_control_clicked=this;
                                if(strlen(datafill)) {
                                    tstump2=parent_stump;
                                    if(strlen(datafilltarget)) {
                                        if(pGUI->get_stump(datafilltarget)) {
                                            tstump2=pGUI->get_stump(datafilltarget);
                                        }
                                    }
                                    if(tstump2)
                                        if(tstump2->get_control(datafill)) {
                                            tstump2->get_control(datafill)->set_value((char*)data[j+listoffset].c_str());
                                        }
                                }
                                pGUI->focus_control=0;
                                hide_default_children();
                            }
                        }
                    }
                } else {
                    if(pMouse->In(x,y,w,h)) {
                        pMouse->ClearClicks();
                        pGUI->last_control_clicked=this;
                        pGUI->focus_control=this;
                        pGUI->focus_stump=parent_stump;
                        show_default_children();
                    }
                }
                break;

            case FM_GC_TEXTBOX:
            case FM_GC_PASSWORD:
                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    pGUI->last_control_clicked=this;
                    pGUI->focus_control=this;
                    pGUI->focus_stump=parent_stump;

                }
                break;

            case FM_GC_CONSOLE:
                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    pGUI->focus_control=parent_stump->get_control(va("%s_input",name));
                    pGUI->last_control_clicked=this;
                    pGUI->focus_control=this;
                    pGUI->focus_stump=parent_stump;
                }
                break;

            case FM_GC_SELECTA:
                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    selecta();
                    pGUI->last_control_clicked=this;
                    pGUI->focus_control=this;
                    pGUI->focus_stump=parent_stump;
                }
                break;

            case FM_GC_CANCEL:
                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    pGUI->del_stump(parent_stump->name);
                    pGUI->focus_control=0;
                    pGUI->last_control_clicked=0;
                }
                break;

            case FM_GC_BUTTON:
            case FM_GC_SUBMIT:
            case FM_GC_ACTION:
                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    pLog->_DebugAdd("[%s][%s][%s][%s][%s]",parent_stump->name,name,action,datafilltarget,datafill);
                    pGUI->last_control_clicked=this;
                    pGUI->focus_control=0;
                    if(console[0]!=0) {

                        strcpy(temp,convert$(console));

                        pCons->_Execute(temp);

                    } else {
                        if(tstump->props&FM_GP_SERVER_DATA) {
                            // SEND GUI VARS TO SERVER
                        } else {
                            strcpy(temp,va("guiaction %s",tstump->action));
                            pCons->_Execute(temp);
                        }
                    }
                    if(!(tstump->props&FM_GP_DO_NOT_DIE)) {
                        pGUI->last_control_clicked=0;
                        tstump->bDeleteMe=true;
                    }
                    if(!pGUI->focus_stump) {
                        pGUI->focus_control=this;
                        pGUI->focus_stump=parent_stump;
                    }
                }
                break;

            case FM_GC_TICKBOX:
            case FM_GC_GROUPTICK:

                if(pMouse->In(x,y,w,h)) {
                    pMouse->ClearClicks();
                    if(type==FM_GC_TICKBOX) {
                        if(dlcs_strcasecmp(value,"on"))
                            set_value("off");
                        else
                            set_value("on");
                    }
                    if(type==FM_GC_GROUPTICK) {
                        if(dlcs_strcasecmp(value,"on")) {
                            parent_stump->clear_grouptick(group);
                            set_value("off");
                        } else {
                            parent_stump->clear_grouptick(group);
                            set_value("on");
                        }
                    }
                    pGUI->last_control_clicked=this;
                    pGUI->focus_control=this;
                    pGUI->focus_stump=parent_stump;
                }
                break;

            case FM_GC_DROPBOX:
                if(pGUI->focus_control!=this) {
                    if(pMouse->In(x,y,w,h)) {
                        pMouse->ClearClicks();
                        if(pGUI->focus_control==this) {
                            pGUI->last_control_clicked=this;
                            pGUI->focus_control=0;
                        } else {
                            pGUI->last_control_clicked=this;
                            pGUI->focus_control=this;
                            pGUI->focus_stump=parent_stump;
                        }
                    }
                } else {
                    if(pMouse->In(x,y ,w,y+(listdepth*16)+16)) {
                        pGUI->bDrawToolTip=false;
                        pMouse->ClearClicks();
                        pGUI->last_control_clicked=this;
                        j=1;
                        while(j<(listdepth+1)) {
                            if(pMouse->In(x-1,y+(j*16),w+1,y+(j*16)+16)) {
                                if(pGUI->bDebug) pGFX->DrawRect(x-1,y+(j*16),w+1,y+(j*16)+16,LONGRGB(234,155,25));
                                data_selected.clear();
                                data_selected[listoffset+j]=true;
                                set_value((char*)data[listoffset+j].c_str());
                                pGUI->focus_control=0;
                                listoffset=0;
                            }
                            j++;
                        }
                    }
                }

                break;

            default:
                pGUI->last_control_clicked=0;
                break;
            }
    }

    if(pMouse->ButtonDownTick(SDL_BUTTON_LEFT)) {
        if(pGUI->bDebug) pGFX->DrawRect(x,y,w,h,LONGRGB(255,0,0));
        switch(type) {
        case FM_GC_BASE_SELECT:
            for(j=0; j<listdepth; j++) {
                if(pMouse->In(x,y+(j*32)+4,w,y+(j*32)+32+4)) {
                    if(pGUI->bDebug) pGFX->DrawBar(x,y+(j*32)+4,w,y+(j*32)+32+4,LONGRGB(234,155,25));
                }
            }
            break;

        case FM_GC_H_SCROLLBAR:
        case FM_GC_V_SCROLLBAR:
            if(pMouse->In(x,y,x+16,y+16)) {
                if(parent_control) {
                    parent_control->listoffset--;
                    if(parent_control->listoffset < 0) {
                        parent_control->listoffset = 0;
                    }
                }
                pGUI->focus_control=this;
                pGUI->focus_stump=parent_stump;
            }

            if(pMouse->In(x,h,x+16,h+16)) {
                if(parent_control) {
                    parent_control->listoffset++;
                    i=parent_control->control_data_total;
                    if(parent_control->listoffset > (i-parent_control->listdepth))
                        parent_control->listoffset=i-parent_control->listdepth;
                }
                pGUI->focus_control=this;
                pGUI->focus_stump=parent_stump;
            }
            break;

        default:
            break;
        }
    }

    if(bMouseWheelUp) {
        if(pGUI->focus_control==this)
            listoffset--;
    }

    if(bMouseWheelDown) {
        if(pGUI->focus_control==this)
            listoffset++;
    }
    if(pGUI->bResetToolTip) pGUI->bDrawToolTip=false;
}
void C_GCTRL::draw(bool bMouseWheelUp, bool bMouseWheelDown) {

    pLog->_DebugAdd("C_GCTRL::draw() BEGIN");

    C_GCTRL* tctrl2=0;
    vector <string> vtmp;
    map <string, int>::iterator ii;
    int i,j,k,x,y,w,h;
    i=j=k=x=y=w=h=0;
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
    float fScrollBarRange;
    float fScrollBarPageSize;
    float fScrollBarSize;
    float fScrollBarOffset;
    int   iScrollBarThumbSize;
    int   iScrollBardrawThumbStart;
    int   iScrollBardrawThumbEnd;
    int nsp;
    x  = parent_stump->rect.left;
    y  = parent_stump->rect.top;
    if(parent_control) {
        x=parent_control->rect.left;
        y=parent_control->rect.top;
    }

    x += rect.left;
    y += rect.top;
    switch(iRelativeTo) {
    case GUI_TOP_CENTER:
        break;

    case GUI_TOP_RIGHT:
        break;

    case GUI_MIDDLE_LEFT:
        break;

    case GUI_MIDDLE_CENTER:
        x+=parent_stump->rect.right/2;
        y+=parent_stump->rect.bottom/2;
        x-=rect.right/2;
        y-=rect.bottom/2;
        break;

    case GUI_MIDDLE_RIGHT:
        break;

    case GUI_BOTTOM_LEFT:
        break;

    case GUI_BOTTOM_CENTER:
        break;

    case GUI_BOTTOM_RIGHT:
        break;

    case GUI_TOP_LEFT:
    default:
        break;
    }



    pLog->_DebugAdd("C_GCTRL::draw() 1");

    w  = x+rect.right;
    h  = y+rect.bottom;

    if(pGUI->focus_control==this) show_default_children();
    else hide_default_children();

    pLog->_DebugAdd("C_GCTRL::draw() 2");

    if(visible) {

        pLog->_DebugAdd("C_GCTRL::draw() 3");

        for( ii=pGUI->GC_TYPE.begin(); ii!=pGUI->GC_TYPE.end(); ++ii)
            if( ( (*ii).second ) == (type) )
                strcpy(temp,(const char*)(*ii).first.c_str());

        pLog->_DebugAdd("C_GCTRL::draw() visible switch(type) type=[%s]",temp);

        switch(type) {



        case FM_GC_CHATBOX:
            if(!parent_stump->get_control(va("%s_list_0",name))) attach_default_children();
            if(pGUI->pMouse->InRect(rect))
                colr=RGB(255,255,155);
            else
                colr=RGB(155,155,55);
            pGFX->DrawBar(x,y,w,h,RGB(0,0,0),RGB(0,0,0));
            pGFX->DrawRect(x,y,w,h,colr);

            if(listoffset > (control_data_total - listdepth))
                listoffset = control_data_total - listdepth;
            if(listoffset < 0)   listoffset = 0;
            for(i=0; i<listdepth; i++) {
                if(i<buf.size()) {
                    strcpy(temp,(char*)buf[i+listoffset].c_str());
                    if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                    if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;
                    if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                        parent_stump->get_control(va("%s_list_%d",name,i))->set_value(temp);
                    }
                }
            }

            break;

        case FM_GC_CONSOLE:

            if(!parent_stump->get_control(va("%s_scrollbar",name))) attach_default_children();



            if(props & FM_GP_USEBACKGROUND) pGFX->DrawBar(x,y,w,h,LONGRGB(0,0,0),LONGRGB(0,0,50));
            else                            pGFX->DrawTexture(x,y,w,h,
                        media
                        ,255,255,255 );



            if(listoffset > (control_data_total - listdepth))
                listoffset = control_data_total - listdepth;



            if(listoffset < 0)   listoffset = 0;
            for(i=0; i<listdepth; i++) {

                // pLog->_Add("C_GCTRL::draw FM_GC_CONSOLE %d",listoffset); pLog->_Add("C_GCTRL::draw FM_GC_CONSOLE %s",temp);
                strcpy(temp,(char*)pCons->buf[i+listoffset].c_str());

                if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;

                if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                    parent_stump->get_control(va("%s_list_%d",name,i))->set_value(temp);
                }

            }

            break;

        case FM_GC_H_SCROLLBAR:
        case FM_GC_V_SCROLLBAR:
            if(props & FM_GP_USEBACKGROUND) {
            } else {  // pGFX->DrawBar(x, y, x+15, h, LONGRGB(180,180,180),LONGRGB(180,180,180));
                pGFX->Draw3DBox(x-1, y+17, w-1, h+1);// , LONGRGB(180,180,180),LONGRGB(180,180,180));
            }
            pGUI->gPrint(x,y, "W", 2);
            if(parent_control) {
                fScrollBarOffset    = (float)(parent_control->listoffset);
                fScrollBarRange     = (float)(parent_control->control_data_total)*16;
                fScrollBarPageSize  = (float)(parent_control->listdepth)*16;
                fScrollBarSize      = (float)(parent_control->listdepth-3)*16; // thumb size = scroll bar size * page size / scroll bar range
                iScrollBarThumbSize = fScrollBarSize * fScrollBarPageSize  / fScrollBarRange;
                if(iScrollBarThumbSize < 15) iScrollBarThumbSize = 15;
                iScrollBardrawThumbStart = y + 16 + (((fScrollBarOffset * iScrollBarThumbSize) / fScrollBarSize) * (float)(parent_control->listdepth)) ;// - fScrollBarSize;
                iScrollBardrawThumbEnd   = iScrollBardrawThumbStart + iScrollBarThumbSize;
                pGFX->Draw3DBox( x+1, iScrollBardrawThumbStart, x+13, iScrollBardrawThumbEnd );
                if(pGUI->bDebug) pGUI->gPrint( 300, 0, va("%f %f %f %f %d %d %d",fScrollBarRange,fScrollBarSize,fScrollBarPageSize,fScrollBarOffset,iScrollBarThumbSize,iScrollBardrawThumbStart,iScrollBardrawThumbEnd), 1);
            }
            pGUI->gPrint(x,h+1, "V", 2);
            break;

        case FM_GC_DROPBOX:
            if(listoffset > (control_data_total - listdepth)) listoffset = control_data_total - listdepth;
            if(listoffset < 0) listoffset = 0;
            x=x+1;
            y=y+1;
            pGFX->DrawBar(x-1,y-1,w+1,y+16+1,0,0);
            if(!parent_stump->get_control(va("%s_scrollbar",name))) attach_default_children();
            pGFX->DrawBar(x,y,w,y+16,LONGRGB(102,102,170),LONGRGB(12,12,170));
            if(data_selected.size()==0) data_selected[1]=true;
            for(i=0; i<control_data_total; i++) {
                if(data_selected[i]==true) {
                    // pGUI->gPrint(x,y,(char*)data[i].c_str(),1);
                    pGUI->gPrint(x,y,va("^0%s",(char*)data[i].c_str()),1);
                }
            }
            if(pGUI->focus_control!=this) {
                if(pGUI->pMouse->In(x,y,w,h)) pGUI->gPrint(w-15, y, "^6Y", 2);
                else                          pGUI->gPrint(w-15,y,"^1Y", 2);
            }
            if(pGUI->focus_control==this) {
                if(parent_stump->get_control(va("%s_scrollbar",name))) {
                    pGFX->DrawBar(x-2,y+16,w+2,y+(listdepth*16)+18,0,0);
                    pGFX->DrawBar(x-1,y+17,w+1,y+(listdepth*16)+17, LONGRGB(132,132,200), LONGRGB(12,12,170));
                    j=1;
                    while( j< (listdepth+1) ) {
                        if(data_selected[j+listoffset]) {
                            r1=255;
                            g1=255;
                            b1=0;
                            r2=200;
                            g2=200;
                            b2=0;
                        } else {
                            r1=132;
                            g1=132;
                            b1=200;
                            r2=12;
                            g2=12;
                            b2=170;
                        }
                        if(pGUI->pMouse->In(x-1,y+(j*16),w+1,y+(j*16)+16)) {
                            r1=255;
                            g1=255;
                            b1=255;
                            r2=pGFX->GetFade(1);
                            g2=pGFX->GetFade(2);
                            b2=pGFX->GetFade(3);
                            r1=pGFX->GetFade(2);
                            g1=pGFX->GetFade(3);
                            b1=pGFX->GetFade(1);
                        }
                        pGFX->DrawBar(x,y+(j*16)+1,w+1,y+(j*16)+16, LONGRGB(r1,g1,b1),LONGRGB(r2,g2,b2));
                        pGUI->gPrint(x,y+(j*16),va("^0%s",(char*)data[j+listoffset].c_str()),1);
                        j++;
                    }
                }
            }
            break;

        case FM_GC_LOCAL_DIR:
            if(!parent_stump->get_control(va("%s_scrollbar",name))) {
                //rect.right=384;
                //rect.bottom=364;
                attach_default_children();
            }
            if(pGUI->pMouse->In(x,y,w,h)) {
            }
            if(props & FM_GP_USEBACKGROUND) {
                pGFX->DrawTexture(x,y,w,h,media,255,255,255 );
            } else {
                pGFX->Draw3DBox(x,y,w,h);
                pGFX->DrawBar(x+2,y+2,w-2,h-2,LONGRGB(255,255,255),LONGRGB(255,255,255));
            }
            if(listoffset > (control_data_total - listdepth)) listoffset = control_data_total - listdepth;
            if(listoffset < 0) listoffset = 0;
            for(i=0; i<listdepth; i++) {
                strcpy(temp,(char*)data[i+listoffset].c_str());
                if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;
                if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                    parent_stump->get_control(va("%s_list_%d",name,i))->set_value(va("^0 %s",temp));
                }
            }
            break;

        case FM_GC_LISTBOX:
            if(props & FM_GP_USEBACKGROUND) {
                pGFX->DrawTexture(x,y,w,h, media,255,255,255 );
            } else {
                pGFX->DrawBar(x,y,w,h,LONGRGB(180,180,180),LONGRGB(180,180,180));
            }
            pGUI->gPrint(x,y,va("^>ffffff %s",name),1);
            for(j=1; j<listdepth; j++) {
                strcpy(temp, (char*)data[j+listoffset].c_str());
                while( (strlen(temp)*9) > rect.right) {
                    temp[strlen(temp)-4]='.';
                    temp[strlen(temp)-3]='.';
                    temp[strlen(temp)-2]='.';
                    temp[strlen(temp)-1]=0;
                }
                pGUI->gPrint(x,y+16+(j*16),va("^0%s",temp),1);
                // pGUI->gPrint(x,y+16+(j*16),temp,1);
            }

            break;

        case FM_GC_GROUPTICK:
        case FM_GC_TICKBOX:
            if(props & FM_GP_USEBACKGROUND) {
                pGFX->DrawTexture(x,y,w,h,media,255,255,255 );
            } else {
                pGFX->DrawBar(x,y,w,h,LONGRGB(240,240,240),LONGRGB(240,240,240));
                pGFX->DrawBar(x+1,y+1,w,h,LONGRGB(80,80,80),LONGRGB(80,80,80));
                pGFX->DrawBar(x+2,y+2,w-2,h-2,LONGRGB(180,180,180),LONGRGB(180,180,180));
            }
            if(dlcs_strcasecmp(value,"on"))
                pGUI->gPrint(x,y,"^+H",2);
            break;

        case FM_GC_STATIC_TEXT:
            if(props&FM_GP_USEBACKGROUND)
                pGFX->DrawBar(x,y-2,x+(CGLFont_StrLen(value))*8+24,y+16,background_color,background_color);
            if(props&FM_GP_BORDER)
                pGFX->DrawRectangle(x,y-2,x+(CGLFont_StrLen(value))*8+24,y+16,border_color);
            if(font==0) font=1;
            // if(fontbank==0) fontbank=1;
            pGUI->gPrint(x,y,va("%s",value),font,fontbank);
            break;

        case FM_GC_TEXTBOX:
        case FM_GC_PASSWORD:
            if(pGUI->focus_control==this) {
                pGFX->DrawBar(x,y,w,h,LONGRGB(0,0,0),LONGRGB(0,0,0));
                pGFX->DrawBar(x+1,y+1,w-1,h-1,LONGRGB(255,255,255),LONGRGB(255,255,255));
                strcpy(temp,value);
                k=(rect.right/FONT_CHAR_WIDTH)-1;
                while(CGLFont_StrLen(temp)>k) {
                    j=0;
                    while(temp[j]!=0) {
                        temp[j]=temp[j+1];
                        j++;
                    }
                }
                if(type==FM_GC_PASSWORD) {
                    for(k=0; k<strlen(temp); k++) {
                        temp[k]='*';
                    }
                }
                pGUI->gPrint(x,y,temp,1);
                if(pGUI->flashCursor(0,0))
                    pGUI->gPrint((int)(x+(CGLFont_StrLen(temp)*8.5)),y,"^1f",2);
            } else {
                pGFX->DrawBar(x,y,w,h,LONGRGB(0,0,0),LONGRGB(0,0,0));
                pGFX->DrawBar(x+1,y+1,w-1,h-1,LONGRGB(255,255,255),LONGRGB(255,255,255));
                strcpy(temp,value);
                k=(rect.right/FONT_CHAR_WIDTH)-1;
                temp[k]=0;
                if(type==FM_GC_PASSWORD) {
                    for(k=0; k<strlen(temp); k++) {
                        temp[k]='*';
                    }
                }
                pGUI->gPrint(x,y,va("^0%s",temp),1);
                // pGUI->gPrint(x,y,temp,1);
            }
            break;

        case FM_GC_CANCEL:
        case FM_GC_SUBMIT:
        case FM_GC_BUTTON:
        case FM_GC_ACTION:
            if(strlen(media)) {
                if(pGUI->pMouse->In(x,y,w,h)) {
                    if(pGUI->pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
                        rect.right=rect.left+32;
                        rect.bottom=32;
                    }
                }
                pGFX->DrawTexture(x,y,w,h,media,255,255,255 );
            } else {
                if(props&FM_GP_USEBACKGROUND) {
                    pGUI->DrawTextureGUIButton(this,x,y);
                } else {
                    pGUI->drawSlicedGUIButton(this,x,y);
                }
            }
            break;

        case FM_GC_STATIC_IMAGE: // draw a base gfx on this spot based on media
            pGFX->DrawTexture(x,y,w,h,media,255,255,255 );
            break;

        case FM_GC_SELECTA:

            if(strlen(value)) {
                tctrl2=parent_stump->get_control(va("%s_props",group));
                r1=g1=b1=r2=g2=b2=r3=g3=b3=0;
                imedia_selected=imedia_unselected=imedia_hover=0;
                if(tctrl2) {
                    r1=atoi(tctrl2->property["selected_r"].c_str());
                    g1=atoi(tctrl2->property["selected_g"].c_str());
                    b1=atoi(tctrl2->property["selected_b"].c_str());
                    r2=atoi(tctrl2->property["unselected_r"].c_str());
                    g2=atoi(tctrl2->property["unselected_g"].c_str());
                    b2=atoi(tctrl2->property["unselected_b"].c_str());
                    r3=atoi(tctrl2->property["hover_r"].c_str());
                    g3=atoi(tctrl2->property["hover_g"].c_str());
                    b3=atoi(tctrl2->property["hover_b"].c_str());
                    imedia_selected=atoi(tctrl2->property["media_selected"].c_str());
                    imedia_unselected=atoi(tctrl2->property["media_unselected"].c_str());
                    imedia_hover=atoi(tctrl2->property["media_hover"].c_str());

                }
                if(selected) {
                    if(imedia_selected) {
                        pGFX->DrawTexture(x,y,w,h,
                                          // imedia_selected
                                          tctrl2->property["media_selected"].c_str()
                                          ,255,255,255 );
                    } else {
                        pGFX->DrawBar(x,y,w,h,RGB(r1,g1,b1),RGB(r1,g1,b1) );
                    }
                } else {
                    if(imedia_unselected) {
                        pGFX->DrawTexture(x,y,w,h,
                                          // imedia_unselected
                                          tctrl2->property["media_unselected"].c_str()
                                          ,255,255,255 );
                    } else {
                        pGFX->DrawBar(x,y,w,h,RGB(r2,g2,b2),RGB(r2,g2,b2) );
                    }
                }
                pGUI->gPrint(x+(rect.right/2)-((strlen(value)*12)/2),y-2+(rect.bottom/2)-12,value,1);
                pGUI->drawB9utton(400,400,580,580);
            }
            break;

        case FM_GC_SELECTA_PROPS:

            break;

        case FM_GC_3D_BOX:

            pGFX->Draw3DBox(x,y,w,h);
            break;

        case FM_GC_FONT_SELECT:
            if(!parent_stump->get_control(va("%s_list_1",name))) attach_default_children();
            if(pGUI->focus_control==this) {
                if(props & FM_GP_USEBACKGROUND) {
                    pGFX->DrawTexture(x,y,w,h,atoi(media),255,255,255);
                } else {
                    pGFX->Draw3DBox(x,y,w,h);
                }

                if(listoffset<0) listoffset=0;
                if(listoffset>control_data_total-listdepth) listoffset=control_data_total-listdepth;
                for(i=0; i<listdepth; i++) {
                    strcpy(temp,(char*)data[i+listoffset].c_str());
                    if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                    if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;
                    if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                        parent_stump->get_control(va("%s_list_%d",name,i))->set_value(va("^1 FONT %04d",atoi(temp)));
                        parent_stump->get_control(va("%s_list_%d",name,i))->font=i%1;
                    }
                }

            } else {
                pGUI->gPrint(x,y,"Z",2);
            }

            break;

        case FM_GC_BASE_SELECT:

            if(!parent_stump->get_control(va("%s_list_1",name))) attach_default_children();
            if(pGUI->focus_control==this) {
                if(props & FM_GP_USEBACKGROUND) {
                    pGFX->DrawTexture(x,y,w,h,media,255,255,255);
                } else {
                    pGFX->Draw3DBox(x,y,w,h);
                }
                if(listoffset<0) listoffset=0;
                if(listoffset>control_data_total-listdepth) listoffset=control_data_total-listdepth;
                for(i=0; i<listdepth; i++) {
                    if(parent_stump->get_control((va("%s_list_image_%d",name,i)))) {
                        strcpy(parent_stump->get_control(va("%s_list_image_%d",name,i))->media,
                               (char*)data[i+listoffset].c_str());
                    }
                    strcpy(temp,(char*)data[i+listoffset].c_str());
                    if(temp[strlen(temp)-1]=='\r') temp[strlen(temp)-1]=0;
                    if(temp[strlen(temp)-1]=='\n') temp[strlen(temp)-1]=0;
                    if(parent_stump->get_control(va("%s_list_%d",name,i))) {
                        parent_stump->get_control(va("%s_list_%d",name,i))->set_value(va("^1 b%04d.bmp",atoi(temp)));
                    }
                }
            } else {
                pGUI->gPrint(x,y,"Z",2);
            }
            break;

        case FM_GC_REPORTER:

            // Copies 1 control value
            // data_source, data_source_stump
            // GROUP, ACTION
            // to another control value
            // DATAFILL, DATAFILLTARGET
            // after X amount of ticks
            // CONSOLE
            // can be shown or hidden
            // VISIBLE

            break;

        case FM_GC_HIDDEN: // chillax & do nothing * 420
            break;

        case FM_GC_MESSAGE:

        case FM_GC_MENU:

        case FM_GC_WILLY:

        case FM_GC_REMOTE_DIR:

        case FM_GC_POPUP_MENU:

        case FM_GC_COLOR_PICKER:

        case FM_GC_TAB_BOX:

        default:
            strcpy(temp,"UNKNOWN TYPE!");
            for( ii=pGUI->GC_TYPE.begin(); ii!=pGUI->GC_TYPE.end(); ++ii)
                if( ( (*ii).second ) == (type) )
                    strcpy(temp,(const char*)(*ii).first.c_str());
            strcpy(temp,va("[%s][%s]", name,temp));
            pGFX->DrawBar(x,y,w,h,RGB(255,0,0) );
            pGFX->DrawRectangle(x,y,w,h,RGB(0,0,0));//x+32,y+32,RGB(0,0,0));
            pGUI->gPrint(x,y,temp,1);
            break;
        }

        tctrl2=first_child_control;
        while(tctrl2) {
            tctrl2->draw(bMouseWheelUp,bMouseWheelDown);
            tctrl2=tctrl2->next;
        }
    }

    pLog->_DebugAdd("C_GCTRL::draw() END");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C_GSTMP::C_GSTMP(CLog* pInLog, C_GFX* pInGFX, C_GUI* pInGUI, C_CONS* pInConsole) {
    pLog=pInLog;
    pGFX=pInGFX;
    pGUI=pInGUI;
    pCons=pInConsole;
    next=0;
    prev=0;
    init_stmp();
}

C_GSTMP::~C_GSTMP() {
    C_GCTRL* del_gui_control;
    gui_control=first_gui_control;
    while(gui_control) {
        del_gui_control=gui_control;
        gui_control=gui_control->next;
        dlcsm_delete(del_gui_control);
    }
    dlcsm_delete(del_gui_control);
}

void C_GSTMP::init_stmp() {
    memset(name,0,1024);
    zeroize_stmp();
    strcpy(caption,"FM GUI system window");
    gui_control=NULL;
    first_gui_control=NULL;
    next=NULL;
}

void C_GSTMP::zeroize_stmp() {
    memset(media,0,1024);
    memset(action,0,1024);
    memset(default_focus_control,0,256);
    memset(gamemode,0,1024);
    memset(caption,0,256);
    props=0;
    iRelativeTo=0;
    x=0;
    y=0;
    bMoving=0;
    bSizing=0;
    bEditing=0;
    bDeleteMe=0;
    rect.top=0;
    rect.left=0;
    rect.bottom=0;
    rect.right=0;
    mrect.top=0;
    mrect.left=0;
    mrect.bottom=0;
    mrect.right=0;
}

void C_GSTMP::set_group_selecta(C_GCTRL* sgc) {
    C_GCTRL* tgc;
    tgc=first_gui_control;
    while(tgc) {
        if(dlcs_strcasecmp(sgc->group,tgc->group))
            tgc->selected=0;
        tgc=tgc->next;
    }
    sgc->selected=1;
}

char C_GSTMP::control_count(void) {
    C_GCTRL* tctrl;
    char x=0;
    tctrl=first_gui_control;
    while(tctrl) {
        x++;
        tctrl=tctrl->next;
    }
    return x;
}

void C_GSTMP::add_control(const char* name,int type,int x,int y,int x2,int y2,int props,char* media,char* value) {
    add_control(0,name,type,x,y,x2,y2,props,media,value);
}

void C_GSTMP::add_control(C_GCTRL* pInParentCTRL, const char* name,int type,int x,int y,int x2,int y2,int props,char* media,char* value) {
    C_GCTRL* ngc;
    if(get_control(name)) {
        mod_control(name,type,x,y,x2,y2,props,media,value);
        return;
    }
    if(pInParentCTRL) {
        ngc=pInParentCTRL->first_child_control;
        if(ngc) {
            while(ngc->next)
                ngc=ngc->next;
            ngc->next = new C_GCTRL(pInParentCTRL);
            ngc->next->prev=ngc;
            ngc=ngc->next;
        } else {
            ngc = new C_GCTRL(pInParentCTRL);
            pInParentCTRL->first_child_control=ngc;
        }
    } else {
        ngc=first_gui_control;
        if(ngc) {
            while(ngc->next)
                ngc=ngc->next;
            ngc->next = new C_GCTRL(this,pLog,pGFX,pGUI,pCons);
            ngc->next->prev=ngc;
            ngc=ngc->next;
        } else {
            ngc = new C_GCTRL(this,pLog,pGFX,pGUI,pCons);
            first_gui_control=ngc;
        }
    }
    if(!ngc) return;
    ngc->parent_stump=this;
    ngc->parent_control=pInParentCTRL;
    ngc->data.clear();
    ngc->buf.clear();
    strcpy(ngc->name,name);
    mod_control(name,type,x,y,x2,y2,props,media,value);
}

void C_GSTMP::mod_control(const char* name,int type,int x,int y,int x2,int y2,int props,char* media,char* value) {
    pLog->_DebugAdd("mod_control: %s,%d,%d,%d,%d,%d,%d,%s,%s",name,type,x,y,x2,y2,props,media,value);
    C_GCTRL* ngc;
    ngc=get_control(name);
    if(!ngc) return;
    ngc->type=type;
    ngc->rect.left=x;
    ngc->rect.top=y;
    if(type==FM_GC_TICKBOX)   {
        x2=16;
        y2=16;
        ngc->set_value("off");
    }
    if(type==FM_GC_GROUPTICK) {
        x2=16;
        y2=16;
        ngc->set_value("off");
    }
    if(x2==0) x2=100;
    if(y2==0) y2=15;
    ngc->rect.right=x2;
    ngc->rect.bottom=y2;
    strcpy(ngc->media,media);
    ngc->set_value(value);
    ngc->props=props;
    if(type==FM_GC_HIDDEN)
        ngc->visible=false;
}

C_GCTRL* C_GSTMP::get_control(const char* name) {
    C_GCTRL* childctl;
    gui_control=first_gui_control;
    while(gui_control) {
        if(dlcs_strcasecmp(name,gui_control->name))
            return gui_control;
        childctl=gui_control->first_child_control;
        while(childctl) {
            if(dlcs_strcasecmp(name,childctl->name))
                return childctl;
            childctl=childctl->next;
        }
        gui_control=gui_control->next;
    }
    return gui_control;
}

void C_GSTMP::size_control(char* name,int x,int y,int x2,int y2) {
    gui_control=first_gui_control;
    while(gui_control) {
        if(dlcs_strcasecmp(gui_control->name,name)) {
            gui_control->rect.left=x;
            gui_control->rect.top=y;
            gui_control->rect.right=x2;
            gui_control->rect.bottom=y2;
        }
        gui_control=gui_control->next;
    }
}

void C_GSTMP::del_control(char* name) {
    C_GCTRL* pcontrol=0;
    pcontrol=get_control(name);
    if(!pcontrol) return;
    if(pcontrol->next)
        pcontrol->next->prev=pcontrol->prev;
    if(pcontrol->prev)
        pcontrol->prev->next=pcontrol->next;
    dlcsm_delete(pcontrol);
}

void C_GSTMP::clear_grouptick(char* grp) {
    C_GCTRL* cg=0;
    cg=first_gui_control;
    while(cg) {
        if(dlcs_strcasecmp(cg->group,grp)) {
            if(cg->type==FM_GC_GROUPTICK) {
                cg->set_value("off");
                pLog->_DebugAdd("Control [%s] a member of group [%s] was turned off",cg->name,grp);
            }
        }
        cg=cg->next;
    }
}

void C_GSTMP::move_control_to_top(C_GCTRL* gc) {
//      	    ______   ______   ______
//	           n      \ n      \ n      \
//  	0	  A        B        C        0
//       \_______p \______p \______p
    /*


        C_GCTRL* tgc=0;

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

void C_GSTMP::store_stmp(void) {
    store_stmp(name);
}

void C_GSTMP::store_stmp(char* filename) {
    pLog->_DebugAdd("Storing GUI stump [%s]",filename);
    remove(va("gumps%c%s.bak",PATH_SEP,filename));
    rename(va("gumps%c%s",PATH_SEP,filename),va("gumps%c%s.bak",PATH_SEP,filename));
    map <string, int>::iterator ii;
    int i;
    char temp[1024];
    memset(temp,0,1024);
    FILE* fp;
    fp=fopen(va("gumps%c%s",PATH_SEP,filename),"wt");
    fputs("[GUMP]\n",fp);
    fputs(va("name=%s\n",name),fp);
    if(strlen(caption)) fputs(va("caption=%s\n",caption),fp);
    if(strlen(action))   fputs(va("action=%s\n",action),fp);
    if(strlen(gamemode)) fputs(va("gamemode=%s\n",gamemode),fp);
    strcpy(temp,"props=");
    for( ii=pGUI->GC_PROP.begin(); ii!=pGUI->GC_PROP.end(); ++ii) {
        if((((*ii).second ) & (props)) == ((*ii).second)) {
            strcat(temp,(const char*)(*ii).first.c_str());
            strcat(temp,"|");
        }
    }
    temp[strlen(temp)-1]='\n';
    fputs( temp, fp);
    fputs(va("x=%d\n",rect.left),fp);
    fputs(va("y=%d\n",rect.top),fp);
    if(rect.right) fputs(va("w=%d\n",rect.right),fp);
    if(rect.bottom) fputs(va("h=%d\n",rect.bottom),fp);
    if(iRelativeTo) {
        memset(temp,0,1024);
        for( ii=pGUI->GC_RELATIVE.begin(); ii!=pGUI->GC_RELATIVE.end(); ++ii) {
            if( ((*ii).second) == iRelativeTo ) {
                strcpy(temp,va("relativeto=%s\n",(const char*)(*ii).first.c_str()));
                break;
            }
        }
        fputs( temp, fp);
    }
    fputs(va("media=%s\n",media),fp);
    if(strlen(default_focus_control)) fputs(va("default_focus_control=%s\n",default_focus_control),fp);
    fputs("\n",fp);
    C_GCTRL* tgc=0;
    tgc=first_gui_control;
    while(tgc) {
        fputs("[CTRL]\n",fp);
        fputs(va("name=%s\n",tgc->name),fp);
        if(strlen(tgc->datafill)) fputs(va("datafill=%s\n",tgc->datafill),fp);
        if(strlen(tgc->datafilltarget)) fputs(va("datafilltarget=%s\n",tgc->datafilltarget),fp);
        if(strlen(tgc->media))       fputs(va("media=%s\n",tgc->media),fp);
        if(strlen(tgc->media_hover)) fputs(va("media_hover=%s\n",tgc->media_hover),fp);
        if(strlen(tgc->media_click)) fputs(va("media_click=%s\n",tgc->media_click),fp);
        if(strlen(tgc->hover_text))  fputs(va("hover_text=%s\n",tgc->hover_text),fp);
        fputs(va("x=%d\n",tgc->rect.left),fp);
        fputs(va("y=%d\n",tgc->rect.top),fp);
        if(tgc->rect.right) fputs(va("w=%d\n",tgc->rect.right),fp);
        if(tgc->rect.bottom) fputs(va("h=%d\n",tgc->rect.bottom),fp);
        if(tgc->iRelativeTo) {
            memset(temp,0,1024);
            for( ii=pGUI->GC_RELATIVE.begin(); ii!=pGUI->GC_RELATIVE.end(); ++ii) {
                if( ((*ii).second) == tgc->iRelativeTo ) {
                    strcpy(temp,va("relativeto=%s\n",(const char*)(*ii).first.c_str()));
                    break;
                }
            }
            fputs( temp, fp);
        }
        for( ii=pGUI->GC_TYPE.begin(); ii!=pGUI->GC_TYPE.end(); ++ii) {
            if( ((*ii).second) == tgc->type ) {
                strcpy(temp,va("type=%s\n",(const char*)(*ii).first.c_str()));
            }
        }
        fputs( temp, fp);
        if(tgc->props) {
            strcpy(temp,"props=");
            for( ii=pGUI->GC_PROP.begin(); ii!=pGUI->GC_PROP.end(); ++ii) {
                if((((*ii).second ) & (tgc->props)) == ((*ii).second)) {
                    strcat(temp,(const char*)(*ii).first.c_str());
                    strcat(temp,"|");
                }
            }
            temp[strlen(temp)-1]='\n';
            fputs( temp, fp);
        }
        if(tgc->type!=FM_GC_DROPBOX) {
            tgc->get_value(temp);
            if(strlen(temp))         fputs(va("value=%s\n",temp),fp);
        }
        if(strlen(tgc->console)) fputs(va("console=%s\n",tgc->console),fp);
        if(strlen(tgc->group)) fputs(va("group=%s\n",tgc->group),fp);
        if(strlen(tgc->action)) fputs(va("action=%s\n",tgc->action),fp);
        if(tgc->font) fputs(va("font=%d\n",tgc->font),fp);
        if(tgc->fontbank) fputs(va("fontbank=%d\n",tgc->fontbank),fp);
        if(tgc->listdepth) fputs(va("listdepth=%d\n",tgc->listdepth),fp);
        if(tgc->background_color) {
            fputs(va("background_color=%d,%d,%d\n",
                     GetRValue(tgc->background_color),
                     GetGValue(tgc->background_color),
                     GetBValue(tgc->background_color)), fp);

        }
        if(tgc->border_color) {
            fputs(va("border_color=%d,%d,%d\n",
                     GetRValue(tgc->border_color),
                     GetGValue(tgc->border_color),
                     GetBValue(tgc->border_color)), fp);
        }

        if(strlen(tgc->nexttabfocus))  fputs(va("nexttabfocus=%s\n",tgc->nexttabfocus),fp);

        if(     (tgc->type!=FM_GC_CONSOLE)
                &&  (tgc->type!=FM_GC_BASE_SELECT)
          ) {
            for(i=0; i<tgc->data.size(); i++) {
                if(strlen((char*)tgc->data[i].c_str()))
                    fputs(va("value=%s\n",tgc->data[i].c_str()),fp);
            }
        }
        fputs("\n",fp);
        tgc=tgc->next;
    }
    fclose(fp);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

C_GUI::C_GUI() {
    pLog=new CLog("gui.log");
    pLog->On();
    pLog->LineFeedsOff();
    pLog->Restart();
    pLog->AddLineSep();
    init();
    bMadeLog=true;
}

C_GUI::C_GUI(CLog* pinLog) {
    pLog=pinLog;
    init();
    cab_loading=false;
}

C_GUI::C_GUI(CGAF* pInGAF, CLog* pinLog) {
    pGAF=pInGAF;
    pLog=pinLog;
    init();
    cab_loading=true;
}

C_GUI::C_GUI(C_GFX* pInGFX, CGAF* pInGAF, CLog* pinLog) {
    pGFX=pInGFX;
    pGAF=pInGAF;
    pLog=pinLog;
    pLog->_DebugAdd("C_GUI:C_GUI(C_GFX* pInGFX, CGAF* pInGAF, CLog* pinLog)");
    init();
    cab_loading=true;
}

C_GUI::~C_GUI() {
    pLog->_DebugAdd("C_GUI:~C_GUI()");
    if(bMadeLog) dlcsm_delete(pLog);
    dlcsm_delete(pCons);
    dlcsm_delete(pMouse);
    clear();
    destroyFonts();
    destroyButtons();

}

void C_GUI::init(void) {


    pFirstFont=0;
    bCtrlMoving=0;
    bStmpMoving=0;
    bCtrlSizing=0;
    bStmpSizing=0;
    bDebug=0;
    SDL_Init(SDL_INIT_EVENTTHREAD);
    pLog->_DebugAdd("C_GUI::init()");
    GC_TYPE.clear();
    GC_TYPE["FM_GC_BUTTON"]       = FM_GC_BUTTON;
    GC_TYPE["FM_GC_SUBMIT"]       = FM_GC_SUBMIT;
    GC_TYPE["FM_GC_CANCEL"]       = FM_GC_CANCEL;
    GC_TYPE["FM_GC_TEXTBOX"]      = FM_GC_TEXTBOX;
    GC_TYPE["FM_GC_PASSWORD"]     = FM_GC_PASSWORD;
    GC_TYPE["FM_GC_STATIC_TEXT"]  = FM_GC_STATIC_TEXT;
    GC_TYPE["FM_GC_LISTBOX"]      = FM_GC_LISTBOX;
    GC_TYPE["FM_GC_3D_BOX"]       = FM_GC_3D_BOX;
    GC_TYPE["FM_GC_ACTION"]       = FM_GC_ACTION;
    GC_TYPE["FM_GC_MESSAGE"]      = FM_GC_MESSAGE;
    GC_TYPE["FM_GC_CHATBOX"]      = FM_GC_CHATBOX;
    GC_TYPE["FM_GC_TICKBOX"]      = FM_GC_TICKBOX;
    GC_TYPE["FM_GC_HIDDEN"]       = FM_GC_HIDDEN;
    GC_TYPE["FM_GC_MENU"]         = FM_GC_MENU;
    GC_TYPE["FM_GC_DROPBOX"]      = FM_GC_DROPBOX;
    GC_TYPE["FM_GC_STATIC_IMAGE"] = FM_GC_STATIC_IMAGE;
    GC_TYPE["FM_GC_REPORTER"]     = FM_GC_REPORTER;
    GC_TYPE["FM_GC_GROUPTICK"]    = FM_GC_GROUPTICK;
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
    GC_TYPE["FM_GC_BASE_SELECT"]  = FM_GC_BASE_SELECT;
    pLog->_DebugAdd("C_GUI::init() -> GC_TYPE setup");
    GC_PROP.clear();
    GC_PROP["FM_GP_ALWAYS_ON_TOP"]   = FM_GP_ALWAYS_ON_TOP;
    GC_PROP["FM_GP_SIZEABLE"]        = FM_GP_SIZEABLE;
    GC_PROP["FM_GP_CLOSEX"]          = FM_GP_CLOSEX;
    GC_PROP["FM_GP_MOVEABLE"]        = FM_GP_MOVEABLE;
    GC_PROP["FM_GP_MENU"]            = FM_GP_MENU;
    GC_PROP["FM_GP_USEBACKGROUND"]   = FM_GP_USEBACKGROUND;
    GC_PROP["FM_GP_BORDER"]          = FM_GP_BORDER;
    GC_PROP["FM_GP_MINIMIZE"]        = FM_GP_MINIMIZE;
    GC_PROP["FM_GP_SERVER_DATA"]     = FM_GP_SERVER_DATA;
    GC_PROP["FM_GP_ALWAYS_ON_BOTTOM"]= FM_GP_ALWAYS_ON_BOTTOM;
    GC_PROP["FM_GP_NO_RETURN"]       = FM_GP_NO_RETURN;
    GC_PROP["FM_GP_DO_NOT_DIE"]      = FM_GP_DO_NOT_DIE;
    GC_PROP["FM_GP_FULLSCREEN"]      = FM_GP_FULLSCREEN;
    GC_PROP["FM_GP_TOGGLE_STATE"]    = FM_GP_TOGGLE_STATE;
    GC_PROP["FM_GP_PERSIST"]         = FM_GP_PERSIST;
    pLog->_DebugAdd("C_GUI::init() -> GC_PROP setup");
    GC_RELATIVE.clear();
    GC_RELATIVE["GUI_TOP_LEFT"]     = GUI_TOP_LEFT;
    GC_RELATIVE["GUI_TOP_CENTER"]   = GUI_TOP_CENTER;
    GC_RELATIVE["GUI_TOP_RIGHT"]    = GUI_TOP_RIGHT;
    GC_RELATIVE["GUI_MIDDLE_LEFT"]  = GUI_MIDDLE_LEFT;
    GC_RELATIVE["GUI_MIDDLE_CENTER"]= GUI_MIDDLE_CENTER;
    GC_RELATIVE["GUI_MIDDLE_RIGHT"] = GUI_MIDDLE_RIGHT;
    GC_RELATIVE["GUI_BOTTOM_LEFT"]  = GUI_BOTTOM_LEFT;
    GC_RELATIVE["GUI_BOTTOM_CENTER"]= GUI_BOTTOM_CENTER;
    GC_RELATIVE["GUI_BOTTOM_RIGHT"] = GUI_BOTTOM_RIGHT;
    pLog->_DebugAdd("C_GUI::init() -> GC_RELATIVE setup");

    pCons = new C_CONS(pGAF,pLog);
    pLog->_DebugAdd("C_GUI: Console created");
    ButtonTexture=0;
    B9utton=0;
    pMouse=0;
    pMouse=new C_Mouse(pLog);// (pGAF,pLog);
    //pMouse=new C_Mouse(pLog);// (pGAF,pLog);
    initFonts();
    initButtons();
    loadButtons("");
//     loadFonts();
//    MOUSEMODE=MP_MENU;
    memset(szPromptMsg,0,_MAX_PATH);
    strcpy(szCommand,"nop");
    bShowFPS=false;

    /////////////////////////////////////////////////////
    // Setup default key values
    KeyDown.clear();

    KeyMap[(SDLKey)'`']="toggle console";
    KeyMap[(SDLKey)'~']="toggle console";
    KeyMap[(SDLKey)'f']="toggle fps";

    KeyRepeatTimer=dlcs_get_tickcount();

    stump_count=0;
    focus_stump=0;
    focus_control=0;
    last_control_clicked=0;
    first_gui_stump=0;
    bMadeLog=0;
    bStaticToolTip=0;
    memset(hover_text,0,1024);
    bDrawToolTip=false;
}

void C_GUI::consEntry(char* fmt, ...) {
    C_GSTMP* stump;
    C_GCTRL* ctrl;
    char ach[1024];
    memset(ach,0,1024);
    va_list vaz;
    va_start(vaz,fmt);
    vsprintf(ach,fmt,vaz);
    va_end(vaz);
    if(strlen(ach)) {
        if(pCons)
            pCons->Entry(ach);
        stump=first_gui_stump;
        while(stump) {
            ctrl=stump->first_gui_control;
            while(ctrl) {
                if(ctrl->type==FM_GC_CONSOLE) {
                    if(ctrl->first_child_control) {
                        ctrl->put_control_data("listentry",ach);
                        ctrl->listoffset = ctrl->control_data_total - ctrl->listdepth;
                    }
                }
                ctrl=ctrl->next;
            }
            stump=stump->next;
        }
    }
}

void C_GUI::clear() {
    C_GSTMP* gui_stump;
    gui_stump=0;
    C_GSTMP* del_gui_stump;
    gui_stump=first_gui_stump;
    while(gui_stump) {
        del_gui_stump=gui_stump;
        gui_stump=gui_stump->next;
        dlcsm_delete(del_gui_stump);
    }
    gui_stump=0;
    stump_count=0;
    focus_stump=0;
    focus_control=0;
    first_gui_stump=0;
}

void C_GUI::add_stump(char* name,int x,int y,int x2,int y2,int props,char* media) {
    bool bfree=0;
    if(get_stump(name)) {
        mod_stump(name,x,y,x2,y2,props,media);
        return;
    }
    C_GSTMP* stump;
    del_stump(name);
    stump=first_gui_stump;
    if(!stump) {
        first_gui_stump=new C_GSTMP(pLog,pGFX,this,pCons);
        stump=first_gui_stump;
    } else {
        while(stump->next) {
            stump=stump->next;
        }
        stump->next=new C_GSTMP(pLog,pGFX,this,pCons);
        stump->next->prev=stump;
        stump=stump->next;
    }
    if(stump) {
        strcpy(stump->name,name);
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
        stump->bDeleteMe=0;
        stump->bMoving=false;
        stump->bSizing=false;
        stump_count++;
        pLog->_DebugAdd("stump added [%s] stump count [%d]",name,stump_count);
    }
}

void C_GUI::mod_stump(char* name,int x,int y,int x2,int y2,int props,char* media) {
    C_GSTMP* stump=get_stump(name);
    if(!stump) return;
    strcpy(stump->name,name);
    stump->rect.left=x;
    stump->rect.top=y;
    stump->rect.right=x2;
    stump->rect.bottom=y2;
    stump->props=props;
    strcpy(stump->media,media);
}

void C_GUI::del_stump(C_GSTMP* tstump) {
    char temp[1024];
    memset(temp,0,1024);
    if(tstump) {
        stump_count--;
        if(tstump->prev) {
            tstump->prev->next=tstump->next;
            if(tstump->next) {
                tstump->next->prev=tstump->prev;
            }
        }
        if(first_gui_stump==tstump) {
            first_gui_stump=tstump->next;
            first_gui_stump->prev=0;
        }
        if(focus_stump=tstump) {
            focus_stump=0;
            focus_control=0;
        }
        strcpy(temp,va("C_GUI::del_stump(C_GSTMP* tstump) -> stump deleted [%s] stump",tstump->name));
        dlcsm_delete(tstump);
        pLog->_DebugAdd("%s (stump count [%d])",temp,stump_count);
    }
}

void C_GUI::del_stump(char* name) {
    del_stump(get_stump(name));
}

void C_GUI::del_stumps(void) {
    C_GSTMP* tstump;
    C_GSTMP* dstmp;
    tstump=first_gui_stump;
    while(tstump) {
        if(tstump->bDeleteMe) {
            dstmp=tstump;
            tstump=tstump->next;
            // if(tstump->prev) tstump->prev->next=tstump->next;
            del_stump(dstmp);
        } else
            tstump=tstump->next;
    }
}

C_GSTMP* C_GUI::get_stump(char* name) {
    C_GSTMP* gui_stump=first_gui_stump;
    while(gui_stump) {
        if(dlcs_strcasecmp(gui_stump->name,name)) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}

C_GSTMP* C_GUI::get_prev_stump(char* name) {
    C_GSTMP* gui_stump=first_gui_stump;
    while(gui_stump) {
        if(gui_stump->next)
            if(dlcs_strcasecmp(gui_stump->next->name,name)) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}

C_GSTMP* C_GUI::get_prev_stump(C_GSTMP* tstump) {
    C_GSTMP* gui_stump=first_gui_stump;
    while(gui_stump) {
        if(gui_stump->next==tstump) break;
        gui_stump=gui_stump->next;
    }
    return gui_stump;
}

void C_GUI::call_do_line(char* line) {

    pLog->_DebugAdd("   GUI->call_do_line");

    char temp[1024];
    memset(temp,0,1024);
    std::vector <std::string> vs;
    std::vector <std::string> vin;
    static C_GSTMP* stump;
    static C_GCTRL* ctrl;
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

    vs=dlcs_explode("\n",line);
    if(vs.size())
        vin=dlcs_explode("=",vs[0]);

    if(vin[0]!="") {
        if(vin.size()>1) {
            strcpy(vsa,vin[0].c_str());
            vsal=strlen(vsa);
            strcpy(vsb,vin[1].c_str());
            vsbl=strlen(vsb);
        }

        // determine mode

        if(dlcs_strcasecmp((char*)vin[0].c_str(),"[ATTACH]")) {
            mode=FGL_ATTACH;
            return;
        }

        if(dlcs_strcasecmp((char*)vin[0].c_str(),"[GUMP]")) {
            mode=FGL_GUMP;
            return;
        }

        if(dlcs_strcasecmp((char*)vin[0].c_str(),"[CTRL]")) {
            mode=FGL_CTRL;
            return;
        }

        if(dlcs_strcasecmp((char*)vin[0].c_str(),"[CTRL_CHILD]")) {
            mode=FGL_CTRL_CHILD;
            return;
        }

        if(dlcs_strcasecmp((char*)vin[0].c_str(),"[PROPERTY]")) {
            mode=FGL_PROPERTY;
            return;
        }


        switch(mode) {
        case FGL_ATTACH:
            if(stump) {
                if(cab_loading) {
                    cab_call((char*)vin[1].c_str());
                } else {
                    if(dlcs_strcasecmp((char*)vin[0].c_str(),"name")) {
                        dlcs_getcwd(temp);
                        pLog->_DebugAdd("Loading GUI file -> %s",
                                        va("%s%cgumps%c%s",temp,PATH_SEP,PATH_SEP,vin[1].c_str()) );
                    }
                }
            }
            break;

        case FGL_PROPERTY:
            if(stump) {
                if(vin.size()>1) {
                    ctrl=stump->get_control(cur_ctrl);
                    if(ctrl) {
                        ctrl->property[(char*)vin[0].c_str()]=(char*)vin[1].c_str();
                        return;
                    }
                }
            }
            break;

        case FGL_GUMP:
            pLog->_DebugAdd("mode: FGL_GUMP define gump");
            if(dlcs_strcasecmp((char*)vin[0].c_str(),"name")) {
                pLog->_DebugAdd("name:%s",vin[1].c_str());
                strcpy(cur_gump,(char*)vin[1].c_str());
                add_stump(cur_gump,810,610,10,10,0,"0");
                stump=get_stump(cur_gump);
                return;
            }

            if(stump) {
                if(dlcs_strcasecmp((char*)vin[0].c_str(),"gamemode")) {
                    strcpy(stump->gamemode,vin[1].c_str());
                    return;
                }
                if(dlcs_strcasecmp((char*)vin[0].c_str(),"relativeto")) {
                    stump->iRelativeTo=GC_RELATIVE[(char*)vin[1].c_str()];
                    switch(stump->iRelativeTo) {
                    case GUI_TOP_CENTER:
                        break;

                    case GUI_TOP_RIGHT:
                        break;

                    case GUI_MIDDLE_LEFT:
                        break;

                    case GUI_MIDDLE_CENTER:
                        stump->x+=pGFX->ScreenWidth/2;
                        stump->y+=pGFX->ScreenHeight/2;
                        stump->x-=stump->rect.right/2;
                        stump->y-=stump->rect.bottom/2;
                        break;

                    case GUI_MIDDLE_RIGHT:
                        break;

                    case GUI_BOTTOM_LEFT:
                        break;

                    case GUI_BOTTOM_CENTER:
                        stump->x+=pGFX->ScreenWidth/2;
                        stump->y+=pGFX->ScreenHeight-(stump->rect.bottom);
                        stump->x-=stump->rect.right/2;
                        stump->y-=stump->rect.bottom;
                        break;

                    case GUI_BOTTOM_RIGHT:
                        break;

                    case GUI_TOP_LEFT:
                    default:
                        break;
                    }
                    stump->rect.left=(LONG)stump->x;
                    stump->rect.top=(LONG)stump->y;
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"x")) {
                    stump->x=0;
                    if(dlcs_strcasecmp(vsb,"width")) {
                        stump->x=SDL_GetVideoSurface()->w-1;
                    }
                    if((int)vsb[vsbl-1]==(int)'*') {
                        vsb[vsbl-1]=0;
                        stump->x=SDL_GetVideoSurface()->w*(atoi(vsb)/100);
                    } else {
                        stump->x=atoi(vin[1].c_str());
                    }
                    stump->rect.left=(LONG)stump->x;
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"y")) {
                    stump->y=0;
                    if(dlcs_strcasecmp(vsb,"height")) {
                        stump->y=SDL_GetVideoSurface()->h;
                    }
                    if((int)vsb[vsbl-1]==(int)'*') {
                        vsb[vsbl-1]=0;
                        stump->y=SDL_GetVideoSurface()->h*(atoi(vsb)/100);
                    } else {
                        stump->y=atoi(vin[1].c_str());
                    }
                    stump->rect.top=(LONG)stump->y;
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"w")) {
                    stump->rect.right=0;
                    if((int)vsb[vsbl-1]==(int)'*') {
                        vsb[vsbl-1]=0;
                        stump->rect.right=SDL_GetVideoSurface()->w*(atoi(vsb)/100);
                    } else {
                        stump->rect.right=atoi(vin[1].c_str());
                    }
                    if(dlcs_strcasecmp(vsb,"width")) {
                        stump->rect.right=SDL_GetVideoSurface()->w-1;
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"h")) {
                    stump->rect.bottom=0;
                    if(dlcs_strcasecmp(vsb,"height")) {
                        stump->rect.bottom=SDL_GetVideoSurface()->h;
                    }
                    if((int)vsb[vsbl-1]==(int)'*') {
                        vsb[vsbl-1]=0;
                        stump->rect.bottom=(SDL_GetVideoSurface()->h * ((float)(atoi(vsb)/100)));
                    } else {
                        stump->rect.bottom=atoi(vin[1].c_str());
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"props")) {
                    stump->props=0;
                    vs=dlcs_explode("|",vin[1]);
                    for(i=0; i<(int)vs.size(); i++) {
                        stump->props+=GC_PROP[(char*)vs[i].c_str()];
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"action")) {
                    strcpy(stump->action,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"caption")) {
                    strcpy(stump->caption,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"media")) {
                    strcpy(stump->media,vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"default_focus_control")) {
                    strcpy(stump->default_focus_control,vin[1].c_str());
                    return;
                }
            }

            return;

        case FGL_CTRL_CHILD:
            if(stump) {
                if(dlcs_strcasecmp((char*)vin[0].c_str(),"name")) {
                    ctrl=stump->get_control(cur_ctrl);
                    if(!ctrl) return;
                    strcpy(cur_ctrl,(char*)vin[1].c_str());
                    stump->add_control(ctrl,cur_ctrl,0,0,0,32,32,0,"","");
                    mode=FGL_CTRL;
                    return;
                }
            }
            return;

        case FGL_CTRL:
            if(stump) {
                if(dlcs_strcasecmp((char*)vin[0].c_str(),"name")) {
                    strcpy(cur_ctrl,(char*)vin[1].c_str());
                    stump->add_control(cur_ctrl,0,0,0,32,32,0,"","");
                    return;
                }
                ctrl=stump->get_control(cur_ctrl);
                if(!ctrl) return;
                if(dlcs_strcasecmp((char*)vin[0].c_str(),"value")) {

                    if( (ctrl->type==FM_GC_LISTBOX) ||
                            (ctrl->type==FM_GC_DROPBOX) ) {
                        ctrl->put_control_data("listentry",(char*)vin[1].c_str());
                        if( ((CGLFont_StrLen((char*)vin[1].c_str())*9.5)+9) > (ctrl->rect.right))
                            ctrl->rect.right=(CGLFont_StrLen((char*)vin[1].c_str())*9.5)+9;
                    } else {
                        ctrl->set_value((char*)vin[1].c_str());
                    }
                    if( ctrl->type==FM_GC_STATIC_TEXT ) {
                        ctrl->rect.bottom=15;
                        ctrl->rect.right=CGLFont_StrLen((char*)vin[1].c_str())*9.5;
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"group")) {
                    strcpy(ctrl->group,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"props")) {
                    ctrl->props=0;
                    vs=dlcs_explode("|",vin[1]);
                    for(i=0; i<(int)vs.size(); i++) {
                        ctrl->props+=GC_PROP[(char*)vs[i].c_str()];
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"border_color")) {
                    vs=dlcs_explode(",",vin[1]);
                    if(vs.size()>2) {
                        r=atoi(vs[0].c_str());
                        g=atoi(vs[1].c_str());
                        b=atoi(vs[2].c_str());
                        ctrl->border_color=RGB(r,g,b);
                    } else {

                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"background_color")) {
                    vs=dlcs_explode(",",vin[1]);
                    if(vs.size()>1) {
                        r=atoi(vs[0].c_str());
                        g=atoi(vs[1].c_str());
                        b=atoi(vs[2].c_str());
                        ctrl->background_color=RGB(r,g,b);
                    } else {

                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"listdepth")) {
                    ctrl->listdepth=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"nexttabfocus")) {
                    strcpy(ctrl->nexttabfocus,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"console")) {
                    strcpy(ctrl->console,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"action")) {
                    strcpy(ctrl->action,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"datafill")) {
                    strcpy(ctrl->datafill,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"datafilltarget")) {
                    strcpy(ctrl->datafilltarget,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"media")) {
                    strcpy(ctrl->media,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"media_hover")) {
                    strcpy(ctrl->media_hover,(char*)vin[1].c_str());
                    return;
                }


                if(dlcs_strcasecmp((char*)vin[0].c_str(),"media_click")) {
                    strcpy(ctrl->media_click,(char*)vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"hover_text")) {
                    strcpy(ctrl->hover_text,vin[1].c_str());
                    //strcpy(ctrl->hover_text,ctrl->name);
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"x")) {
                    ctrl->rect.left=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"y")) {
                    ctrl->rect.top=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"w")) {
                    if(dlcs_strcasecmp((char*)vin[1].c_str(),"width")) {
                        ctrl->rect.right=SDL_GetVideoSurface()->w-1;
                    } else {
                        ctrl->rect.right=atoi(vin[1].c_str());
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"h")) {
                    ctrl->rect.bottom=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"relativeto")) {
                    ctrl->iRelativeTo=GC_RELATIVE[(char*)vin[1].c_str()];
                    pLog->_DebugAdd("GUI CONTROL: %s relative to %s (%d) (%d)",ctrl->name,vin[1].c_str(),ctrl->iRelativeTo,GC_RELATIVE[(char*)vin[1].c_str()]);
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"font")) {
                    ctrl->font=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"fontbank")) {
                    ctrl->fontbank=atoi(vin[1].c_str());
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"type")) {
                    ctrl->type=GC_TYPE[(char*)vin[1].c_str()];
                    if(ctrl->type==FM_GC_SELECTA_PROPS) {
                        ctrl->visible=false;
                    }

                    if( (ctrl->type==FM_GC_TEXTBOX) || (ctrl->type==FM_GC_PASSWORD) ) {
                        ctrl->rect.bottom=15;
                    }
                    if(ctrl->type==FM_GC_ACTION) {
                        ctrl->rect.bottom=32;
                        ctrl->rect.right=32;
                    }
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"visible")) {
                    ctrl->visible=false;
                    if(dlcs_strcasecmp((char*)vin[1].c_str(),"yes") )
                        ctrl->visible=true;
                    if(dlcs_strcasecmp((char*)vin[1].c_str(),"true") )
                        ctrl->visible=true;
                    if(dlcs_strcasecmp((char*)vin[1].c_str(),"1") )
                        ctrl->visible=true;
                    return;
                }

                if(dlcs_strcasecmp((char*)vin[0].c_str(),"[CTRL]")) {
                } else {
                    return;
                }

            default:
                break;
            }
        }
    }
}

void C_GUI::call(char*file) {
    if(cab_loading==true) {
        cab_call(file);
    } else {
        call_file(file);
    }
    if(get_stump(file)) {
        focus_stump=get_stump(file);
        focus_control=focus_stump->get_control(focus_stump->default_focus_control);
    }
}

void C_GUI::call_file(char*file) {
    FILE* fp;
    char fin[1024];
    memset(fin,0,1024);
    std::vector <std::string> vs;
    fp=fopen(va("gumps/%s",file),"rt");
    if(fp) {
        while(fgets(fin,256,fp)) {
            vs=dlcs_explode("\r",fin);
            if(vs.size()) call_do_line((char*)vs[0].c_str());
        }
        fclose(fp);
    }
}

void C_GUI::cab_call(char* file) {
    pLog->_DebugAdd("   GUI->call(gumps/%s)",file);
    std::vector <std::string> vs;

    // CxMemFile* hi;

    if(!pGAF) return;

    GAF_FileBuffer nfb;
    int nfbindex;
    nfb.fb=0;
    nfb.Size=0;
    nfbindex=0;

    char fline[1024];
    memset(fline,0,1024);
    int lindex;
    lindex=0;

    nfb = pGAF->GetFile((LPSTR)va("gumps/%s",file));

    pLog->_DebugAdd(" nfb.Size=%d nfb.fb=%d(address) ",nfb.Size, nfb.fb);


    if(nfb.Size) {

        for(nfbindex=0; nfbindex<nfb.Size; nfbindex++) {

            fline[lindex]=nfb.fb[nfbindex];

            if( (nfb.fb[nfbindex]=='\r') ||
                    (nfb.fb[nfbindex]=='\n') ) {
                if(lindex) {
                    fline[lindex]=0;
                    lindex=0;
                    call_do_line(fline);
                }
                memset(fline,0,1024);
            } else {
                lindex++;
            }
        }

        /* hi=new CxMemFile((BYTE*)nfb.fb,nfb.Size);
        while(hi->GetS(fin,256)) {
            vs = dlcs_explode("\r",fin);
            if(vs.size()) call_do_line((char *)vs[0].c_str());
        }
        dlcsm_delete(hi); */
    }
    pLog->_DebugAdd("   GUI->call(gumps/%s) end",file);
}

void C_GUI::bcall(char* file) {
    pLog->_DebugAdd(" GUI->bcall(gumps/%s)",file);
//	CxMemFile *hi;
    if(!pGAF) return;
    GAF_FileBuffer nfb;
    char fin[1024];
    memset(fin,0,1024);
    nfb=pGAF->GetFile((char*)va("gumps/%s",file));
    //hi=new CxMemFile((BYTE*)nfb.fb,nfb.Size);	dlcsm_delete(hi);
}

void C_GUI::bstore(char* file) {
    pGAF->AddFileFromMem((char*)va("gumps/new_%s",file),
                         (unsigned char*)&first_gui_stump,sizeof(&first_gui_stump));
    pGAF->ExtractFile_ToFile((LPSTR)va("gumps/new_%s",file),(LPSTR)va("gumps/new_%s",(LPSTR)file));
}

void C_GUI::store(char* stump_name) {
    if(get_stump(stump_name)) get_stump(stump_name)->store_stmp();
}

void C_GUI::store(void) {
    C_GSTMP* tstump;
    tstump=first_gui_stump;
    while(tstump) {
        tstump->store_stmp();
        tstump=tstump->next;
    }
}

void C_GUI::draw() {
    draw_ctrls();
    checkMouseClicks();
    drawPrompt();
    drawToolTip();
    pMouse->draw();
    // TODO: Add MOUSE INPUT MODE
}

void C_GUI::moveStumpToTop(C_GSTMP* pStump) {
    C_GSTMP* pTempStump;
    if(pStump) {
        if(pStump->prev)
            pStump->prev->next=pStump->next;

        if(pStump->next)
            pStump->next->prev=pStump->prev;

        if(first_gui_stump==pStump) {
            if(pStump->next) {
                first_gui_stump=pStump->next;
                first_gui_stump->prev=0;
            } else return;
        }

        pTempStump=first_gui_stump;
        if(pTempStump) {
            while(pTempStump->next) {
                pTempStump=pTempStump->next;
            }

            pTempStump->next=pStump;
            pStump->prev=pTempStump;
            pStump->next=0;
        } else {
            first_gui_stump=pStump;
            pStump->prev=0;
            pStump->next=0;
        }

        if(focus_stump==pStump) {
            focus_stump=pStump;
            if(focus_control)
                if(focus_control->parent_stump!=pStump)
                    focus_control=0;
        }
    }
}

bool C_GUI::isAStumpMoving(void) {
    C_GSTMP* pTempStump;
    pTempStump = first_gui_stump;
    while(pTempStump) {
        if(pTempStump->bMoving)
            return true;
        pTempStump=pTempStump->next;
    }
    return false;
}

void C_GUI::checkMouseClicks(void) {
    int i,j,k,x,y,w,h;
    C_GSTMP* tstump=0;
    C_GCTRL* tctrl=0;
    C_GCTRL* tctrl2=0;
    char temp[1024];
    memset(temp,0,1024);
    char console[1024];
    memset(console,0,1024);
    bool bMouseWheelUp, bMouseWheelDown;
    bMouseWheelUp   = pMouse->WheelUp();
    bMouseWheelDown = pMouse->WheelDown();
    moveStumpToTop(focus_stump);
    if(focus_control) focus_control->checkMouseClicks(bMouseWheelUp,bMouseWheelDown);
    tstump=first_gui_stump;
    if(tstump)
        while(tstump->next) tstump=tstump->next;
    while(tstump) {
        x=tstump->rect.left;
        y=tstump->rect.top;
        w=x+tstump->rect.right;
        h=y+tstump->rect.bottom;

        if(bDebug) pGFX->DrawRect(x-2,y-2,w+2,h+2,LONGRGB(pGFX->GetFade(4),234,225));

        tctrl=tstump->first_gui_control;
        if(tctrl)
            while(tctrl->next) tctrl=tctrl->next;
        while(tctrl) {
            tctrl->checkMouseClicks(bMouseWheelUp,bMouseWheelDown);
            tctrl=tctrl->prev;
        }

        if(tstump->props & FM_GP_MOVEABLE) {
            if(!tstump->bMoving) {
                if(!bCtrlMoving)
                    if(!bCtrlSizing)
                        if(!bStmpSizing)
                            if(!bStmpMoving)
                                if(!isAStumpMoving()) {
                                    if(pMouse->In(x,y,w-32,h-32)) {
                                        if(pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
                                            bStmpMoving=true;
                                            pMouse->ClearClicks();
                                            tstump->bMoving=true;
                                            focus_stump=tstump;
                                            tstump->mrect.left=(pMouse->X()-tstump->rect.left);
                                            tstump->mrect.top =(pMouse->Y()-tstump->rect.top);
                                        }
                                    }
                                }
            } else {
                tstump->rect.left=pMouse->X()-tstump->mrect.left;
                tstump->rect.top =pMouse->Y()-tstump->mrect.top;
                setdata("guistumpedit.gui","x",(char*)va("%d",tstump->rect.left));
                setdata("guistumpedit.gui","y",(char*)va("%d",tstump->rect.top));
                setdata("guistumpedit.gui","w",(char*)va("%d",tstump->rect.right));
                setdata("guistumpedit.gui","h",(char*)va("%d",tstump->rect.bottom));
                if(!pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
                    pMouse->ClearClicks();
                    tstump->bMoving=false;
                    bStmpMoving=false;
                }
            }
        }

        if(tstump->props & FM_GP_SIZEABLE) {
            if(!tstump->bSizing) {
                if(!bCtrlMoving)
                    if(!bCtrlSizing)
                        if(!bStmpSizing)
                            if(!bStmpMoving)
                                if(pMouse->In(w-32,h-32,w+2,h+2)) {
                                    if(pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
                                        bStmpSizing=true;
                                        pMouse->ClearClicks();
                                        tstump->bSizing=true;
                                        tstump->bMoving=false;
                                        focus_stump=tstump;
                                        pGFX->DrawRect(w-32,h-32,w+2,h+2, LONGRGB(155,pGFX->GetFade(1),255));
                                        tstump->mrect.right=pMouse->X()-tstump->rect.right;
                                        tstump->mrect.bottom=pMouse->Y()-tstump->rect.bottom;
                                    }
                                }
            } else {
                pGFX->DrawRect(x-1,y-1,w+2,h+2, LONGRGB(155,155,pGFX->GetFade(2)));
                tstump->rect.right=pMouse->X()-tstump->mrect.right;
                tstump->rect.bottom=pMouse->Y()-tstump->mrect.bottom;
                setdata("guistumpedit.gui","x",(char*)va("%d",tstump->rect.left));
                setdata("guistumpedit.gui","y",(char*)va("%d",tstump->rect.top));
                setdata("guistumpedit.gui","w",(char*)va("%d",tstump->rect.right));
                setdata("guistumpedit.gui","h",(char*)va("%d",tstump->rect.bottom));
                if(!pMouse->ButtonDown(SDL_BUTTON_LEFT)) {
                    pMouse->ClearClicks();
                    tstump->bSizing=false;
                    tstump->bMoving=false;
                    bStmpSizing=false;
                }
            }
        }
        if(tstump->props & FM_GP_CLOSEX) {
            if(pMouse->In(w-16,y+1,w,y+16)) {
                gPrint(w-16,y+1,"^6X",2);
                if(pMouse->LeftClick()) {
                    tstump->bDeleteMe=1;
                }
            }
        }
        tstump=tstump->prev;
    }
    tstump=first_gui_stump;
    if(tstump)
        while(tstump->next) tstump=tstump->next;
    while(tstump) {
        x=tstump->rect.left;
        y=tstump->rect.top;
        w=x+tstump->rect.right;
        h=y+tstump->rect.bottom;
        if(pMouse->In(x,y,w,h)) {
            if(pMouse->LeftClick()) {
                pMouse->ClearClicks();
                focus_stump=tstump;
                focus_control=tstump->get_control(tstump->default_focus_control);
                setdata("guictrledit.gui","parentstumpname",focus_stump->name);
            }
        }
        tstump=tstump->prev;
    }
}

void C_GUI::draw_ctrls(void) {
    C_GSTMP* tstump=0;
    C_GSTMP* bstump=0;
    C_GCTRL* tctrl=0;
    C_GCTRL* tctrl2=0;
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
    bool bMouseWheelUp, bMouseWheelDown;
    if(focus_stump) {
        if(focus_control) {
            if(focus_control->parent_stump!=focus_stump) {
                focus_control=0;
            }
        }
    }
    if(focus_control)
        focus_stump->move_control_to_top(focus_control);
    tstump=first_gui_stump;
    while(tstump) {
        x=tstump->rect.left;
        y=tstump->rect.top;
        w=x+tstump->rect.right;
        h=y+tstump->rect.bottom;
        if(tstump->props & FM_GP_FULLSCREEN) {
            tstump->rect.left	= 0;
            tstump->rect.top	= 0;
            tstump->rect.bottom	= SDL_GetVideoSurface()->h;
            tstump->rect.right	= SDL_GetVideoSurface()->w;
        } else {

        }

        if(tstump->props & FM_GP_MOVEABLE)
            if(tstump->bMoving) {
                pGFX->DrawTransparentBar(   tstump->rect.left-4,
                                            tstump->rect.top-4,
                                            tstump->rect.left+tstump->rect.right+4,
                                            tstump->rect.top+tstump->rect.bottom+4,
                                            LONGRGB(240,240,240),
                                            LONGRGB(240,240,240));
            }
        if(tstump->props & FM_GP_SIZEABLE)
            if(tstump->bSizing) {
                pGFX->DrawBar(w-32,h-32,w+2,h+2, LONGRGB(155,155,pGFX->GetFade(0)));
            } else {
                if(pMouse->In(w-32,h-32,w+2,h+2)) {
                    pGFX->DrawBar(w-32,h-32,w+2,h+2, LONGRGB(155,pGFX->GetFade(1),255));
                }
            }

        if(tstump->props & FM_GP_USEBACKGROUND) {
            pGFX->DrawTexture(  tstump->rect.left,
                                tstump->rect.top,
                                tstump->rect.left+tstump->rect.right,
                                tstump->rect.top+tstump->rect.bottom,
                                //atoi(
                                tstump->media,
                                255,255,255 );
        } else {
            pGFX->Draw3DBox(tstump->rect);
        }
        //FM_GP_ALWAYS_ON_TOP
        //FM_GP_ALWAYS_ON_BOTTOM
        //FM_GP_SIZEABLE
        //FM_GP_BORDER
        //FM_GP_CLIENT_ONLY
        //FM_GP_NO_RETURN
        //FM_GP_DO_NOT_DIE
        //FM_GP_TOGGLE_STATE

        tctrl=tstump->first_gui_control;
        while(tctrl) {
            if(tctrl!=focus_control) tctrl->draw(bMouseWheelUp,bMouseWheelDown);
            tctrl=tctrl->next;
        }

        if(tstump->props & FM_GP_MENU) {
            if(focus_stump==tstump) pGFX->DrawBar(tstump->rect.left+1, tstump->rect.top+1, tstump->rect.left+tstump->rect.right-1, tstump->rect.top+18-1, LONGRGB(0,60,160), LONGRGB(60,0,60));
            else                    pGFX->DrawBar(tstump->rect.left+1, tstump->rect.top+1, tstump->rect.left+tstump->rect.right-1, tstump->rect.top+18-1, LONGRGB(60,60,60), LONGRGB(10,10,10));
            gPrint(	tstump->rect.left+16, tstump->rect.top+1, va("^1%s",tstump->caption), 1);
            gPrint(	tstump->rect.left, tstump->rect.top+1, "R", 2);
        }
        if(tstump->props & FM_GP_MINIMIZE)  drawButton(79,0,(tstump->rect.left+tstump->rect.right)-17-16 ,tstump->rect.top+1);
        if(tstump->props & FM_GP_CLOSEX)    gPrint(w-16,y+1,"^1X",2);
        tstump=tstump->next;
    }
    if(focus_control) focus_control->draw(bMouseWheelUp,bMouseWheelDown);
    del_stumps();
}

void C_GUI::set_focus_stump(C_GSTMP* stump) {
    if(stump) {
        focus_stump=stump;
    }
}

void C_GUI::set_focus_control(C_GCTRL* ctrl) {
    if(ctrl) {
        focus_control=ctrl;
        set_focus_stump(ctrl->parent_stump);
    }
}

void C_GUI::drawToolTip(void) {
    if(bDrawToolTip) {
        pGFX->DrawRect(rectToolTip.left-1,rectToolTip.top-1,rectToolTip.right+1,rectToolTip.bottom+1,RGB(0,0,0));
        pGFX->DrawBar(rectToolTip.left,rectToolTip.top,rectToolTip.right,rectToolTip.bottom,RGB(255,255,0));
        gPrint(rectToolTip.left+4,rectToolTip.top+4,hover_text,1);
    }
    bDrawToolTip=false;
    bResetToolTip=false;
}

char* C_GUI::getdata(char* ctrlname) {
    if(!last_control_clicked) return("null");
    return (char*)last_control_clicked->parent_stump->data[ctrlname].c_str();
}

void C_GUI::getdata(char* pString, char* ctrlname) {
    // hash this shit into a table then be able to call it like $name
    strcpy(pString,getdata(ctrlname));
}

void C_GUI::setdata(char* stump, char* ctrl, char* value) {
    if(get_stump(stump))
        if(get_stump(stump)->get_control(ctrl))
            get_stump(stump)->get_control(ctrl)->set_value(value);
}

void C_GUI::edit_stump(char* file) {
    char temp[1024];
    map <string, int>::iterator ii;
    if(!get_stump("guistumpedit.gui")) call("guistumpedit.gui");
    C_GSTMP* guistumpedit=get_stump("guistumpedit.gui");
    //if(!get_stump(file))
    call(file);
    C_GSTMP* editstump=get_stump(file);
    if(!guistumpedit) return;
    if(!editstump) return;
    memset(temp,0,1024);
    if(editstump) {
        editstump->bEditing=true;
        pLog->_DebugAdd("con_guiedit: loaded %s",file);
    }
    setdata(guistumpedit->name,"name",editstump->name);
    setdata(guistumpedit->name,"caption",editstump->caption);
    setdata(guistumpedit->name,"media",editstump->media);
    setdata(guistumpedit->name,"action",editstump->action);
    setdata(guistumpedit->name,"default_focus_control",editstump->default_focus_control);
    setdata(guistumpedit->name,"x",(char*)va("%d",editstump->rect.left));
    setdata(guistumpedit->name,"y",(char*)va("%d",editstump->rect.top));
    setdata(guistumpedit->name,"w",(char*)va("%d",editstump->rect.right));
    setdata(guistumpedit->name,"h",(char*)va("%d",editstump->rect.bottom));
    for( ii=GC_PROP.begin(); ii!=GC_PROP.end(); ++ii) {
        strcpy(temp,(const char*)(*ii).first.c_str());
        if( ( ( (*ii).second ) & (editstump->props) ) == ((*ii).second) ) {
            setdata("guistumpedit.gui",temp,"on");
        } else {
            setdata("guistumpedit.gui",temp,"off");
        }
    }
    for( ii=GC_RELATIVE.begin(); ii!=GC_RELATIVE.end(); ++ii) {
        strcpy(temp,(const char*)(*ii).first.c_str());
        if( (*ii).second == editstump->iRelativeTo) {
            setdata("guistumpedit.gui","relativeto",temp);
        }
    }
}

void C_GUI::remove_control(char* stump,char* control) {
    C_GSTMP* pstump;
    pstump=get_stump(stump);
    if(!pstump) return;
    pstump->del_control(control);
}

void C_GUI::drawB9utton(int x,int y, int w, int h) {
    int i,row,col;
    int cx,cy;
    row=0;
    col=0;
    if(!B9utton) {
        B9utton=new CGLTextureList[9];
        if(!B9utton)		 return;
    }
    for(i=0; i<9; i++) {
        if(!B9utton[i].texture) {
            B9utton[i].texture=new CGLTexture(pLog);
            B9utton[i].texture->pGAF=pGAF;
        }
        if(!B9utton[i].texture->Loaded()) {
            // B9utton[i].texture->usemask=0;
            B9utton[i].texture->LoadBMP(pGAF,va("buttons/b9_%03d.bmp",i),0);
            return;
        }
        cx=x + (col*8);
        cy=y + (row*8);
        B9utton[i].texture->Draw(cx,cy,cx+8,cy+8,255,255,255);
        col++;
        if(col>2) {
            col=0;
            row++;
        }
    }
}

void C_GUI::drawGUIButton(int x, int y, int which, int size) {
    // draw a button at x,y in updown state
    if(!ButtonTexture) return;
    if(!ButtonTexture[which].texture) {
        ButtonTexture[which].texture=new CGLTexture(pLog);
        ButtonTexture[which].texture->pGAF=pGAF;
        return;
    }
    if(!ButtonTexture[which].texture->Loaded()) {
        // ButtonTexture[which].texture->usemask=1;
        ButtonTexture[which].texture->LoadBMP(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }
    ButtonTexture[which].texture->Draw(x,y,x+size,y+size,255,255,255);
}

void C_GUI::drawGUIResource(int w,int x,int y,int x2,int y2) {
    drawGUIResourceC(w,x,y,x2,y2,255,255,255);
}

void C_GUI::drawGUIResourceC(int which,int iX,int iY,int iX2,int iY2,u_char r, u_char g, u_char b) {
    // draw a GUI resource at x,y
    if(!ButtonTexture) return;
    if(!ButtonTexture[which].texture) {
        ButtonTexture[which].texture=new CGLTexture(pLog);
        ButtonTexture[which].texture->pGAF=pGAF;
        return;
    }
    if(!ButtonTexture[which].texture->Loaded()) {
        // ButtonTexture[which].texture->usemask=1;
        ButtonTexture[which].texture->LoadBMP(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }
    ButtonTexture[which].texture->Draw(iX,iY,iX2,iY2,r,g,b);
}

void C_GUI::drawOSIcon(int iX,int iY,char* szOS) {
    switch(szOS[0]) {
    case 'W': //win32
        gPrint(iX,iY,"U",2);
        break;
    case 'L': //linux
        gPrint(iX,iY,"T",2);
        break;
    case 'F': // freebsd
        gPrint(iX,iY,"S",2);
        break;
    default: // what the?
        gPrint(iX,iY,"?",2);
        break;
    }
}

void C_GUI::drawButton(int which, int updown, int x, int y) {
    // draw a button at x,y in updown state
    drawButton(which,updown,x,y,16,16);
}

void C_GUI::drawButton(int which, int updown, int x, int y,int w, int h) {
    if(!ButtonTexture) return;
    if(!ButtonTexture[which].texture) {
        ButtonTexture[which].texture=new CGLTexture(pLog);
        ButtonTexture[which].texture->pGAF=pGAF;
        return;
    }
    if(!ButtonTexture[which].texture->Loaded()) {
        // ButtonTexture[which].texture->usemask=0;
        ButtonTexture[which].texture->LoadBMP(pGAF,va("buttons/%03d.bmp",which),0);
        return;
    }
    ButtonTexture[which].texture->Draw(x,y,x+w,y+h,255,255,255);
}

int  C_GUI::drawSlicedGUIButton(C_GCTRL* gui_control,int x,int y) {
    // draw a button at x,y in updown state
    int j=0;
    int width=CGLFont_StrLen(gui_control->get_value())*8;
    drawGUIButton(x,y,51,32);
    j+=32;
    while(width>16) {
        drawGUIButton(x+j,y,52,32);
        j+=32;
        width-=32;
    }
    drawGUIButton(x+j,y,53,32);
    gui_control->rect.right=j+32;
    gui_control->rect.bottom=32;
    if(pMouse->In(x,y,x+j,y+32)) { //Rect(gui_control->rect))
        gPrint(x+12, y+6, va("^&^>aaaaff%s",gui_control->get_value()),1);
    } else {
        gPrint(x+12, y+6, va("^&^>6666aa%s",gui_control->get_value()),1);
    }
    return false;
}

int  C_GUI::DrawTextureGUIButton(C_GCTRL* gui_control,int x,int y) {
    int j=0;
    pGFX->DrawTexture(x,y,gui_control->rect.right+x, gui_control->rect.bottom+y,
                      //atoi(
                      gui_control->media,255,255,255 );
    gPrint(gui_control->rect.left+8+x,gui_control->rect.top+8+y,gui_control->get_value(),1);
    if(pMouse->In(gui_control->rect.left+x,gui_control->rect.top+y,gui_control->rect.right+x,gui_control->rect.bottom+y)) {
        gPrint(gui_control->rect.left+8+x,gui_control->rect.top+8+y,va("^g%s",gui_control->get_value()),1);
    }
    return false;
}

void C_GUI::prompt(char* szPrompt, char* szCommand) {
    // ProcessConsoleCommand("play prompt.wav",0);
    strcpy(szPromptMsg,szPrompt);
    strcpy(szCommand,szCommand);
    bClosePrompt=false;
}

void C_GUI::drawCPUSTRING(int iX, int iY) {
    drawOSIcon(iX,iY,CPUSTRING);
    gPrint(iX+10,iY,va("^+^1(^5%s^1)",CPUSTRING),1);
}

void C_GUI::drawPrompt(void) {
    if(bClosePrompt==true) {
        memset(szPromptMsg,0,1024);
        del_stump("prompt.gui");
        return;
    }
    if(get_stump("prompt.gui")) {
        focus_stump=get_stump("prompt.gui");
        return;
    }
    if(!CGLFont_StrLen(szPromptMsg)) return;
    call("prompt.gui");
    setdata("prompt.gui","message",szPromptMsg);
}

bool C_GUI::bIsPrompt(void) {
    if(strlen(szPromptMsg)) return true;
    return false;
}

bool C_GUI::flashCursor(int xi, int yi) {
    static bool bCursor; // Get cursor time
    static unsigned long dwCursorTime=dlcs_get_tickcount();
    if((dlcs_get_tickcount()-dwCursorTime)>400) {
        dwCursorTime=dlcs_get_tickcount();
        if(bCursor) bCursor=false;
        else        bCursor=true;
    }
    return bCursor;
}

bool C_GUI::initButtons() {
    int i;
    B9utton=new CGLTextureList[9];
    if(!B9utton) return false;
    ButtonTexture=new CGLTextureList[MAX_BUTTONS];
    if(!ButtonTexture)return false;
    for(i=0; i<MAX_BUTTONS; i++)
        ButtonTexture[i].texture=0;
    for(i=0; i<9; i++)
        B9utton[i].texture=0;
    return true;
}

bool C_GUI::loadButtons(char* szFilename) {
    destroyButtons();
    if(!initButtons()) return false;
    return true;
}

bool C_GUI::destroyButtons() {
    int i;
    for(i=0; i<9; i++)
        dlcsm_delete(B9utton[i].texture);
    delete [] B9utton;
    B9utton=0;
    for(i=0; i<MAX_BUTTONS; i++)
        dlcsm_delete(ButtonTexture[i].texture);
    delete [] ButtonTexture;
    ButtonTexture=0;
    return true;
}

bool C_GUI::initFonts() {
    pLog->_DebugAdd("Begin Fonts init...");
    loadFonts();
    return 1;
}

bool C_GUI::loadFonts(void) {
    pLog->_DebugAdd("Begin Fonts load...");
    CGLFont* pFont;
    int x=1;
    DIR* dpdf;
    struct dirent* epdf;
    dpdf = opendir("fonts");
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            if( (dlcs_strcasecmp(epdf->d_name,".")) ||
                    (dlcs_strcasecmp(epdf->d_name,"..")) ) {
            } else {
                if(dlcs_isdir(epdf->d_name)) {

                } else {
                    // pLog->AddEntry("Found font texture: font/%s\n",epdf->d_name);
                    pFont=pFirstFont;
                    if(pFont) {
                        while(pFont->pNext) {
                            pFont=pFont->pNext;
                        }
                        pFont->pNext=new CGLFont(pGAF, pLog);
                        pFont=pFont->pNext;
                    } else {
                        pFirstFont=new CGLFont(pGAF, pLog);
                        pFont=pFirstFont;
                    }
                    pFont->Load(va("fonts/%s",epdf->d_name));
                    pFont->iWhich=x;
                    x++;
                    if(!pFont->pFontTex->bmap) {
                        pLog->AddEntry("ERROR LOADING base/%s (CGLTEXTURE OBJECT DESTROYED)\n",epdf->d_name);
                        dlcsm_delete(pFont);
                    } else {
                        pLog->AddEntry("Font texture %s loaded (OPENGL[%d]) \n",pFont->pFontTex->tfilename,pFont->pFontTex->bmap);
                    }
                }
            }
        }
    }
    closedir(dpdf);
}

bool C_GUI::destroyFonts(void) {
    CGLFont* pFont;
    pFont=pFirstFont;
    while(pFont) {
        pFirstFont=pFont;
        pFont=pFont->pNext;
        dlcsm_delete(pFirstFont);
    }
}

CGLFont* C_GUI::GetFont(char* szFont) {
    CGLFont* pFont;
    pFont=pFirstFont;
    while(pFont) {
        if(dlcs_strcasecmp(pFont->pFontTex->tfilename,szFont)) {
            return pFont;
        }
        pFont=pFont->pNext;
    }
    return 0;
}
CGLFont* C_GUI::GetFont(int iWhich) {
    CGLFont* pFont;
    pFont=pFirstFont;
    while(pFont) {
        if(pFont->iWhich==iWhich) {
            return pFont;
        }
        pFont=pFont->pNext;
    }
    return 0;
}

void C_GUI::gPrint(int iX,int iY,const char* szText,const char* fnt) {
    gPrint(iX,iY,szText,fnt,1);
}
void C_GUI::gPrint(int iX,int iY,const char* szText,const char* fnt,int wset) {
    CGLFont* pFont;
    pFont=GetFont((char*)fnt);
    if(pFont) {
        pFont->Print(iX-8.5,iY,szText,wset);
    }
}
void C_GUI::gPrint(int iX,int iY,const char* szText,int fnt) {
    gPrint(iX,iY,szText,fnt,1);
}
void C_GUI::gPrint(int iX,int iY,const char* szText,int fnt, int wset) {
    CGLFont* pFont;
    pFont=GetFont(fnt);
    if(pFont) {
        pFont->Print(iX-8.5,iY,szText,wset);
    }
}

void C_GUI::drawFPS(int iX,int iY) {
    static unsigned long   FPS;
    static unsigned long   LastShowFPS;
    static unsigned long   FrameCount;
    if(dlcs_get_tickcount()-LastShowFPS>1000) {
        FPS=(long)FrameCount*1000/(dlcs_get_tickcount()-LastShowFPS);
        LastShowFPS=dlcs_get_tickcount();
        FrameCount=0;
    }
    FrameCount++;
    if((iX==0) && (iY==0)) {
        iX=pGFX->ScreenWidth-80;
        iY=pGFX->ScreenHeight-32;
    }
    gPrint(iX,iY,va("^5FPS ^1[^+^5%d^1^+]",FPS),5);
}

void C_GUI::drawFileXferStatus(void) {
    // draw download stats at bottom of screen
    /*  if(!pPlayer) return;
    if(!pPlayer->DownloadInfo.bIsDownloading) return;
    gPrint(10,310,va("Recieving file[%s]",pPlayer->DownloadInfo.szLocalXferName),LONGRGB(0,255,0));
    gPrint(10,320,va("File size[%d] u_chars",pPlayer->DownloadInfo.iFileSize),LONGRGB(0,255,0));
    gPrint(10,330,va("Progress[%d] u_chars",pPlayer->DownloadInfo.iTotalBytesRead),LONGRGB(0,255,0));
    // gPrint(10,340,va("Transfer Rate :%d u_chars per second",pPlayer->iBytesPerSecond),LONGRGB(0,255,0));
    drawStatBar(10,345,150,(int)pPlayer->DownloadInfo.iTotalBytesRead,(int)pPlayer->DownloadInfo.iFileSize);*/
}

int  C_GUI::doInput() {
    return processKeyboard();
}

int  C_GUI::processKeyboard() {
    char temp[1024];
    memset(temp,0,1024);

    bool bDone=0;

    iKeyDown=0;
    iKeyUp=0;
    ikey=0;

    static bool bWarped;
    static int ilmx;
    static int ilmy;

    if(!pMouse->bRightDown) {
        ilmx=pMouse->X();
        ilmy=pMouse->Y();
    }

    pMouse->Refresh();

    while ( SDL_PollEvent (&event) ) {
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            pMouse->SetWheelUp(0);
            pMouse->SetWheelDown(0);
            if(event.button.button==SDL_BUTTON_WHEELUP) {
                pMouse->SetWheelUp(1);
            }
            pMouse->SetWheelDown(0);
            if(event.button.button==SDL_BUTTON_WHEELDOWN) {
                pMouse->SetWheelDown(1);
            }
            break;

        case SDL_KEYUP:

            keystate=SDL_GetKeyState(NULL);
            modstate=SDL_GetModState();
            ikey=event.key.keysym.sym;
            iKeyUp=ikey;

            if( (ikey>='a') &&
                    (ikey<='z') &&
                    (modstate & KMOD_SHIFT) )
                ikey=(SDLKey)(ikey-32);

            if(modstate & KMOD_SHIFT) {
                switch(ikey) {
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

            KeyDown[(SDLKey)ikey]=false;

            if(!focus_control) {
                switch(ikey) {
                case SDLK_F5:
                    break;
                case SDLK_UP:
                    pGFX->pCamera->bMovingForward1=false; //Move_Forward_Stop();
                    break;
                case SDLK_DOWN:
                    pGFX->pCamera->Move_Backward_Stop();
                    break;
                case SDLK_LEFT:
                    pGFX->pCamera->Move_Left_Stop();
                    break;
                case SDLK_RIGHT:
                    pGFX->pCamera->Move_Right_Stop();
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
            iKeyDown=ikey;

            if( (ikey>='a') &&
                    (ikey<='z') &&
                    (modstate & KMOD_SHIFT) )
                ikey=(SDLKey)(ikey-32);

            if(modstate & KMOD_SHIFT) {
                switch(ikey) {
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

            if(modstate & KMOD_ALT) {
                if(keystate[SDLK_RETURN]) {
                    pGFX->ToggleFullScreen();
                    break;
                }
            }

            KeyDown[(SDLKey)ikey]=true;
            KeyRepeatTimer=0;

            if(focus_control) {
                if( (focus_control->type==FM_GC_TEXTBOX) ||
                        (focus_control->type==FM_GC_PASSWORD) ) {

                    if((ikey=='u') && (modstate & KMOD_CTRL)) {

                        focus_control->set_value("");
                        return 0;
                    }

                    switch(ikey) {
                    case '\t':
                        if(modstate & KMOD_SHIFT) {
                            focus_control=focus_control->prev;
                        } else {
                            focus_control=focus_control->next;
                        }
                        break;

                    case 27:

                        focus_control=focus_control->next;
                        break;

                    case 13:

                        if(focus_control->parent_control) {
                            if(focus_control->parent_control->type==FM_GC_CONSOLE) {
                                _consExecute(focus_control->get_value());
                                //pLog->_DebugAdd("[%s]",temp);
                                strcpy(temp,focus_control->get_value());
                                pCons->_Execute(temp);
                                focus_control->set_value("");
                                break;
                            }
                        }
                        focus_control=focus_control->next;
                        break;

                    default:
                        break;
                    }
                }
            }


            if(!focus_control) {
                switch(ikey) {
                case SDLK_UP:
                    pGFX->pCamera->bMovingForward1=true; // Move_Forward_Start();
                    break;
                case SDLK_DOWN:
                    pGFX->pCamera->Move_Backward_Start();
                    break;
                case SDLK_LEFT:
                    pGFX->pCamera->Move_Left_Start();
                    break;
                case SDLK_RIGHT:
                    pGFX->pCamera->Move_Right_Start();
                    break;
                case SDLK_PAGEDOWN:
                    break;
                case SDLK_PAGEUP:
                    break;

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




    if(pMouse->bRightDown) {
        pMouse->bDraw=0;
        bWarped=0;
        if( (ilmx!=pMouse->ix) ||
            (ilmy!=pMouse->iy) ) {
            pGFX->pCamera->mouseMovement(pMouse->ix,pMouse->iy);
        }

         if(pMouse->bLeftDown) {
                pGFX->pCamera->bMovingForward2=true; // Move_Forward_Start();
         }
         else {
            pGFX->pCamera->bMovingForward2=false; //Move_Forward_Stop();
         }

    } else {
        pMouse->bDraw=1;
        if(!bWarped) {
            bWarped=true;
            SDL_WarpMouse(ilmx,ilmy);
        }
    }

    if((dlcs_get_tickcount()-KeyRepeatTimer)>600) {
        KeyRepeatTimer=dlcs_get_tickcount();
        for(int tkey=0; tkey<255; tkey++) {
            if(KeyDown[(SDLKey)tkey]==true) {
                switch(tkey) {
                case 8:
                    focus_control->get_value(temp);
                    if(strlen(temp)) {
                        temp[strlen(temp)-1]=0;
                        focus_control->set_value(temp);
                    }
                    break;

                default:
                    memset(temp,0,1024);
                    if(focus_control) {
                        focus_control->get_value(temp);
                        if( (tkey>=' ') &&
                                (tkey<='~') &&
                                (CGLFont_StrLen(temp) < focus_control->max_val_len) ) {
                            if(strlen(temp) > focus_control->max_val_len) break;
                            temp[strlen(temp)+1]=0;
                            temp[strlen(temp)]=tkey;
                            focus_control->set_value(temp);
                        }
                    }
                    break;
                }
            }
        }
    }
    if((event.type) == SDL_KEYUP)
        return ikey;
    return 0;
}
bool C_GUI::bSaveBinds(char* szFilename) {
    FILE* fout;
    int ikey;
    char szTemp[_MAX_PATH];
    fout=fopen(szFilename,"wt");
    if(!fout)
        return false;
    fputs("## Ember key bindings file.\n",fout);
    for(ikey=0; ikey<SDLK_LAST; ikey++) {
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
        if(strlen(KeyMap[(SDLKey)ikey])) {
            strcat(szTemp,KeyMap[(SDLKey)ikey]);
            strcat(szTemp,"\n");
            fputs(szTemp,fout);
            pLog->_DebugAdd("%d %s",ikey,szTemp);
        }
    }
    fclose(fout);
    return true;
}

void C_GUI::drawStatBar(int iX,int iY,int iW, int iT,int iV) {
    // iT is total value, iV is current value, will draw percentage bar
    pGFX->DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    pGFX->DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    pGFX->DrawBar(iX,iY,iX+(int)fPercent,iY+3,LONGRGB(0,100,0),LONGRGB(0,100,0));
}

void C_GUI::drawCStatBar(int iX,int iY,int iW, int iT,int iV,long Color1,long Color2) {
    // iT is total value, iV is current value, will draw percentage bar
    pGFX->DrawBar(iX-1,iY-1,iX+iW+1,iY+4,LONGRGB(0,0,0),LONGRGB(0,0,0));
    pGFX->DrawBar(iX,iY,iX+iW,iY+3,LONGRGB(0,0,100),LONGRGB(0,0,100));
    float fPercent=( ((float)iV/(float)iT) * iW);
    pGFX->DrawBar(iX,iY,iX+(int)fPercent,iY+3,Color1,Color2);
}

void C_GUI::clearKeys(void) {
    KeyMap.clear();
}

void C_GUI::_consExecute(char *cmd){
    char temp[1024]; memset(temp,0,1024);
    strcpy(temp,cmd);
    pCons->_Execute(temp);
}

void C_GUI::addChat(int channel, char* user,  char* msg) {
    C_GCTRL* c;
    C_GSTMP* s;
    vector <string> v;
    int i;
    s=first_gui_stump;
    while(s) {
        c=s->first_gui_control;
        while(c) {
            if(c->type==FM_GC_CHATBOX) {

                v=dlcs_explode(",",c->property["channel"].c_str());

                if(v.size()) {

                    for(i=0; i<v.size(); i++) {

                        // pLog->_Add("%s%s",GUI_CHAT_COLOR[GUI_CHAT[v[i].c_str()]].c_str(),v[i].c_str());

                        if(GUI_CHAT[v[i]]==channel) {
                            c->put_control_data("",va("%s>%s",user,msg));
                            c->listoffset = c->control_data_total - c->listdepth;
                            c->buf.push_back(va("%s%s>%s",GUI_CHAT_COLOR[GUI_CHAT[v[i].c_str()]].c_str(),user,msg));
                        }
                    }
                }
            }
            c=c->next;
        }
        s=s->next;
    }
}

