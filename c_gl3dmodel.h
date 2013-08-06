/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/

#ifndef _DLSTORM_OPENGL_MODEL
#define _DLSTORM_OPENGL_MODEL

#include "dlstorm.h"
#include "c_gfx.h"
#include "c_gltexture.h"
#include "c_log.h"
#include "s_gnu.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"

class C_GFX;

class CGLMaterial {
public:
    CGLMaterial();
    ~CGLMaterial();
    CGLMaterial* pNext;
    char    name[FILENAME_SIZE];
    int     iMaterialIndex;
    char    DiffuseTexture[FILENAME_SIZE];
    char    NormalTexture[FILENAME_SIZE];
    char    HeightTexture[FILENAME_SIZE];
    char    OpacityTexture[FILENAME_SIZE];
    char    ShininessTexture[FILENAME_SIZE];
    char    SpecularTexture[FILENAME_SIZE];
    void    Initialize(void);
};

class CGLMesh {
public:
    CGLMesh();
    ~CGLMesh();
    CGLMesh* pNext;
    char    name[FILENAME_SIZE];
    int     iMeshIndex;
    int     iMaterialIndex;
    int     numTriangles;
    int     numUvCoords;
    float*  vertexArray;
    float*  normalArray;
    float*  uvArray;
    void    Initialize(void);

};

class CGLModel {
public:
    CGLModel();
    CGLModel(C_GFX* pGFX, CLog *pInLog);
    ~CGLModel();
    char        name[FILENAME_SIZE];
    bool        bMadeLog;
    CLog*       pLog;
    CGLModel*   pNext;
    CGLModel*   pPrev;
    CGAF*       pGAF;
    C_GFX*      pGFX;

    CGLMesh*     pFirstMesh;
    CGLMaterial* pFirstMaterial;

    int         numMeshes;
    int         numMaterials;

    void         Initialize(void);
    bool         Load(char* filename);
    bool         Draw(CGLTexture* pTexture);
    CGLMesh*     GetMesh(int x);
    CGLMaterial* GetMaterial(int x);
    CGLMaterial* GetMaterial(char* inDiffuseTex);
};

#endif
