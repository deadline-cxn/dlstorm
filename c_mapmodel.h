/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL Map Model class

****************************************************************/

#ifndef _DLSTORM_MAP_MODEL
#define _DLSTORM_MAP_MODEL

#include "dlstorm.h"
#include "c_gfx.h"
#include "c_gl3dmodel.h"
#include "c_gltexture.h"

class C_MapModel {
public:
    C_MapModel();
    ~C_MapModel();
    void        Load(char* filename);
    void        Save(char* filename);
    void        Draw(void);

    C_MapModel* pNext;
    SECTOR      sector1;
    char        name[255];
    char        author[255];
    int         polycount;
    char        texture[1024];
    float       transparency;
};

class C_MapModelList {
public:
    C_MapModelList();
    ~C_MapModelList();
    C_MapModelList* pNext;
    C_MapModel* pMapModel;
    void        Draw(void);
    CVector3    loc;
    CVector3    rot;
    CVector3    scale;
    CColor3     color;
    float       transparency;
};

#endif // _DLSTORM_MAP_MODEL
