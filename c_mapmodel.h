/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    OpenGL Map Model class

****************************************************************/

#include "dlstorm.h"
#include "c_gl3dmodel.h"
#include "c_gltexture.h"

class C_MapModelList {
public:
    C_MapModelList();
    ~C_MapModelList();
    CVector3    loc;
    CVector3    rot;
    CVector3    scale;
    CColor3     color;
    float       transparency;
};

typedef struct tagVERTEX {
    float x, y, z;
    float u, v;
} VERTEX;

typedef struct tagTRIANGLE {
    VERTEX  vertex[3];
} TRIANGLE;

typedef struct tagSECTOR {
    int         numtriangles;
    TRIANGLE*   triangle;
} SECTOR;

class C_MapModel {
public:
    C_MapModel();
    ~C_MapModel();

    C_MapModel* pNext;

    SECTOR  sector1;
    char    name[255];
    char    author[255];
    int     polycount;
    char    texture[1024];
    float   transparency;

    void    Load(char* filename);
    void    Save(char* filename);
    void    Draw(void);


};
