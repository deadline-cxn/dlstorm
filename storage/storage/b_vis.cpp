/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "b_vis.h"
#include "global.h"
#include <string.h>

//////////////////////////////////////////////////////
C_FM_VIS::C_FM_VIS() { fm_vis_init(); }
//////////////////////////////////////////////////////
void C_FM_VIS::fm_vis_init(void)
{
    fm_base_init();
    x=y=z=0;
    r=g=b=255;
    heading=direction=0;
    visible=FM_VIS_VISIBLE;
    type=0;
    media_type=FM_VIS_MODEL;
    memset(media,0,1024);
    model=0;
    texture=0;
    next=0;
    path=0;
    firstpath=0;
    stopped=true;
    moving=false;
}
//////////////////////////////////////////////////////
C_FM_VIS::~C_FM_VIS() { path_clear(); }
//////////////////////////////////////////////////////
void C_FM_VIS::path_push(float px, float py, float pz)
{
    CVisPath *newpath;
    newpath=new CVisPath(px,py,pz);
    if(!firstpath) { firstpath=newpath; return; }
    newpath->next=firstpath;
    newpath->next->prev=newpath;
    firstpath=newpath;
}
//////////////////////////////////////////////////////
void C_FM_VIS::path_pop(void)
{
    if(!firstpath) return;
    path=firstpath;
    if(firstpath->next) { firstpath=firstpath->next; firstpath->prev=0; }
    if(path==firstpath) { DEL(firstpath); }
    DEL(path);
}
//////////////////////////////////////////////////////
CVisPath *C_FM_VIS::path_get(void) { return firstpath; }
//////////////////////////////////////////////////////
void C_FM_VIS::path_clear(void) { while(firstpath) { path_pop(); } }
//////////////////////////////////////////////////////
bool C_FM_VIS::in(float cx,float cy,float cz,float cx2,float cy2,float cz2) 
{
	if((x>=cx)&&(x<=cx2)&&(y>=cy)&&(y<=cy2)&&(z>=cz)&&(z<=cz2))
		return true;
	return false;
}
//////////////////////////////////////////////////////



