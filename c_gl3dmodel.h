/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson
****************************************************************/

#ifndef DL_OPEN_GL_MODEL
#define DL_OPEN_GL_MODEL

#include <string>
#include <vector>

#include "c_gfx.h"
#include "c_gltexture.h"
#include "c_log.h"

#ifdef __linux__
#include "s_gnu.h"
typedef unsigned char byte;
#endif

struct tAnimationInfo {
    char strName[255];          // This stores the name of the animation (Jump, Pain, etc..)
    int startFrame;             // This stores the first frame number for this animation
    int endFrame;               // This stores the last frame number for this animation
};

struct tMaterialInfo {
    char  strName[255];         // The texture name
    char  strFile[255];         // The texture file name (If this is set it's a texture map)
    unsigned char color[3];    // The color of the object (R, G, B)
    int   texureId;             // the texture ID
    float uTile;                // u tiling of texture  (Currently not used)
    float vTile;                // v tiling of texture  (Currently not used)
    float uOffset;              // u offset of texture  (Currently not used)
    float vOffset;              // v offset of texture  (Currently not used)
    CGLTexture  *texture;
} ;

class t3DObject {
public:
    t3DObject();
    ~t3DObject();
    int  numOfVerts;            // The number of verts in the model
    int  numOfFaces;            // The number of faces in the model
    int  numTexVertex;          // The number of texture coordinates
    int  materialID;            // The texture ID to use, which is the index into our texture array
    bool bHasTexture;           // This is TRUE if there is a texture map for this object
    char strName[255];          // The name of the object
    CVector3  *pVerts;          // The object's vertices
    CVector3  *pNormals;        // The object's normals
    CVector2  *pTexVerts;       // The texture's UV coordinates
    tFace *pFaces;              // The faces information of the object
};

class CGLModel {
public:
    CGLModel();
    CGLModel(CLog *pInLog);
    ~CGLModel();
    char        name[1024];
    char        skin[1024];
    bool        bMadeLog;
    float       elapsedTime;
    float       lastTime;
    CLog*       pLog;

    int         numOfObjects;                   // The number of objects in the model
    int         numOfMaterials;                 // The number of materials for the model
    int         numOfAnimations;                // The number of animations in this model (NEW)
    vector<tAnimationInfo>  pAnimations;// The list of animations (NEW)
    vector<tMaterialInfo>   pMaterials; // The list of material information (Textures and colors)
    CGLTexture              *texture;
    vector<t3DObject>       pObject;    // The object list for our model
    CGLModel*   pNext;
    CGLModel*   pPrev;
    CGAF*       pGAF;

    bool Load(char *filename);
    bool Load(char *filename,char *texture);
    bool Draw(void);
    bool RenderSceneDraw(void);
    void Locate(float nx, float ny, float nz);
    void Rotate(float nx, float ny, float nz);
    void Scale(float nx,float ny,float nz);
    void Color(float nr,float ng,float nb);
};

#endif
