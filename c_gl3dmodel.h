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

// This stores the speed of the animation between each key frame
#define kAnimationSpeed         5.0f
#define MD2_MAX_TRIANGLES       4096
#define MD2_MAX_VERTICES        2048
#define MD2_MAX_TEXCOORDS       2048
#define MD2_MAX_FRAMES          512
#define MD2_MAX_SKINS           32
#define MD2_MAX_FRAMESIZE       (MD2_MAX_VERTICES * 4 + 128)


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

// This holds our information for each animation of the Quake model.
// A STL vector list of this structure is created in our t3DModel structure below.
struct tAnimationInfo {
    char strName[255];          // This stores the name of the animation (Jump, Pain, etc..)
    int startFrame;             // This stores the first frame number for this animation
    int endFrame;               // This stores the last frame number for this animation
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.

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

// We added 4 new variables to our model structure.  These will help us handle
// the current animation.  As of now, the current animation will continue to loop
// from it's start from to it's end frame until we right click and change animations.
class t3DModel {
public:
    t3DModel();
    ~t3DModel();
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
    int numOfAnimations;                // The number of animations in this model (NEW)
    int currentAnim;                    // The current index into pAnimations list (NEW)
    int currentFrame;                   // The current frame of the current animation (NEW)
    CVector3    loc;
    CVector3    rot;
    CVector3    scale;
    CColor3     color;
    float       transparency;
    vector<tAnimationInfo>  pAnimations;// The list of animations (NEW)
    vector<tMaterialInfo>   pMaterials; // The list of material information (Textures and colors)
    CGLTexture              *texture;
    vector<t3DObject>       pObject;    // The object list for our model
};

struct tMd2Header {
    int magic;                   // This is used to identify the file
    int version;                 // The version number of the file (Must be 8)
    int skinWidth;               // The skin width in pixels
    int skinHeight;              // The skin height in pixels
    int frameSize;               // The size in bytes the frames are
    int numSkins;                // The number of skins associated with the model
    int numVertices;             // The number of vertices (constant for each frame)
    int numTexCoords;            // The number of texture coordinates
    int numTriangles;            // The number of faces (polygons)
    int numGlCommands;           // The number of gl commands
    int numFrames;               // The number of animation frames
    int offsetSkins;             // The offset in the file for the skin data
    int offsetTexCoords;         // The offset in the file for the texture data
    int offsetTriangles;         // The offset in the file for the face data
    int offsetFrames;            // The offset in the file for the frames data
    int offsetGlCommands;        // The offset in the file for the gl commands data
    int offsetEnd;               // The end of the file offset
};

struct tMd2AliasTriangle {
    byte vertex[3];
    byte lightNormalIndex;
};

struct tMd2Triangle {
    float vertex[3];
    float normal[3];
};

struct tMd2Face {
    short vertexIndices[3];
    short textureIndices[3];
};

struct tMd2AliasFrame {
    float scale[3];
    float translate[3];
    char name[16];
    tMd2AliasTriangle aliasVertices[1];
};

struct tMd2Frame {
    char strName[16];
    tMd2Triangle *pVertices;
};

typedef char tMd2Skin[64];

class CLoadMD2 {
public:
    CLoadMD2(CLog *pInLog);
    ~CLoadMD2() {} ;
    CLog *pLog;
    bool ImportMD2(t3DModel *pModel, char *strFileName, char *strTexture);
    void ReadMD2Data();
    void ParseAnimations(t3DModel *pModel);
    void ConvertDataStructures(t3DModel *pModel);
    void CleanUp();
    CGAF*           pGAF;
    FILE*           m_FilePointer;
    tMd2Header      m_Header;           // The header data
    tMd2Skin*       m_pSkins;          // The skin data
    CTexCoord*      m_pTexCoords;      // The texture coordinates
    tMd2Face*       m_pTriangles;      // Face index information
    tMd2Frame*      m_pFrames;         // The frames of animation (vertices)
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
    CLoadMD2*   MD2;
    t3DModel*   Model;
    CGLModel*   pNext;
    CGLModel*   pPrev;
    CGAF*       pGAF;
    bool Load(char *filename);
    bool Load(char *filename,char *texture);
    bool Draw(void);
    bool RenderSceneDraw(void);
    void Locate(float nx, float ny, float nz) {
        if(!Model) return;
        Model->loc.x=nx;
        Model->loc.y=ny;
        Model->loc.z=nz;
    }
    void Rotate(float nx, float ny, float nz) {
        if(!Model) return;
        Model->rot.x=nx;
        Model->rot.y=ny;
        Model->rot.z=nz;
    }
    void Scale(float nx,float ny,float nz)    {
        if(!Model) return;
        Model->scale.x=nx;
        Model->scale.y=ny;
        Model->scale.z=nz;
    }
    void Color(float nr,float ng,float nb)    {
        if(!Model) return;
        Model->color.r=nr;
        Model->color.g=ng;
        Model->color.b=nb;
    }
    float ReturnCurrentTime(t3DModel *pModel, int nextFrame);
};

#endif
