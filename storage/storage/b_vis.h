/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_BASE_VIS
#define _EMBER_BASE_VIS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "b_base.h"

#define FM_VIS_MODEL        0
#define FM_VIS_SPRITE       1
#define FM_VIS_MULTI_SPRITE 2

#define FM_VIS_HIDDEN       0
#define FM_VIS_VISIBLE      1

class CVisPath
{
public:
    CVisPath()  { x=0;y=0;z=0; next=0; prev=0; };
    CVisPath(int px,int py,int pz) { x=px; y=py; z=pz; next=0; prev=0; };
    ~CVisPath() { };
    int x;
    int y;
    int z;
    CVisPath *next;
    CVisPath *prev;
};

class CGLModel;
class CGLTexture;
class C_FM_VIS : public C_FM_BASE
{
public:
    C_FM_VIS();
    virtual ~C_FM_VIS();
    float   x,y,z;
    float   mx,my,mz;
    float   r,g,b;
    char    heading;
    char    direction;
    bool    visible;
    char    type;
    char    media_type;
    char    media[1024]; // name of media file
    bool    moving;
    bool    stopped;

    void        path_push(float x, float y, float z);
    void        path_pop(void);
    CVisPath*   path_get(void);
    void        path_clear(void);

    CVisPath *path;
    CVisPath *firstpath;

#ifndef _EMBER_SERVER
    CGLModel    *model;
    CGLTexture  *texture;
#endif
    C_FM_VIS    *next;

    void fm_vis_init(void);

    bool    in(float cx,float cy,float cz,float cx2,float cy2,float cz2);
};

#endif // _EMBER_BASE_VIS
