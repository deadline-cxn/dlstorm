/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_gl3dmodel.h
 **   Class:        CGLModel
 **   Description:  Import 3d models
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_CGLMODEL
#define _DLCS_CGLMODEL
#include "dlstorm.h"
#include "c_gfx.h"
#include "c_gltexture.h"
#include "c_log.h"
#include "s_gnu.h"
#ifdef DLCS_WINDOWS
#include "../SDK/assimp/include/assimp/cimport.h"
#include "../SDK/assimp/include/assimp/scene.h"
#include "../SDK/assimp/include/assimp/postprocess.h"
#else
#include <assimp/assimp.h>
#include <assimp/aiScene.h>
#include <assimp/aiTypes.h>
#include <assimp/aiPostProcess.h>
#endif
class C_GFX;
/////////////////////////////// CGLMaterial class
class CGLMaterial {
public:
    CGLMaterial();
    ~CGLMaterial();
    string  name;
    int     iMaterialIndex;
    string DiffuseTexture;
    //aiString NormalTexture;
    //aiString HeightTexture;
    //aiString OpacityTexture;
    //aiString ShininessTexture;
    //aiString  SpecularTexture;
    void    Initialize(void);
};
/////////////////////////////// CGLMesh class
class CGLMesh {
public:
    CGLMesh();
    ~CGLMesh();
    string  name;
    int     iMeshIndex;
    int     iMaterialIndex;
    int     numTriangles;
    int     numUvCoords;
    float*  vertexArray;
    float*  normalArray;
    float*  uvArray;
    void    Initialize(void);

};
/////////////////////////////// CGLModel class
class CGLModel {
public:
    CGLModel();
    CGLModel(C_GFX* pGFX, CLog *pInLog);
    ~CGLModel();
    string                  name;
    vector<CGLMesh*>        meshes;
    vector<CGLMaterial*>    materials;
    int                     numMeshes;
    int                     numMaterials;
    void                    Initialize(void);
    bool                    Load(string filename);
    bool                    Draw(CGLTexture* pTexture);
    CGLMaterial*            GetMaterial(int x);
    CGLMaterial*            GetMaterial(string diffuse_tex);
    CGLMesh*                GetMesh(int x);


    bool                    bMadeLog;
    CLog*                   pLog;
    CGAF*                   pGAF;
    C_GFX*                  pGFX;
};

#endif // _DLCS_CGLMODEL
