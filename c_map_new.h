/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/

#include "dlstorm.h"
#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"
#include "s_gnu.h"

#define MESH_RESOLUTION  1.0f									// Pixels Per Vertex
#define MESH_HEIGHTSCALE 1.0f									// Mesh Height Scale

class CVert {   												// Vertex Class;
public:
    float x;													// X Component
    float y;													// Y Component
    float z;													// Z Component
};
typedef CVert CVec;												// The Definitions Are Synonymous

class CTexCoord { 												// Texture Coordinate Class;
public:
    float u;													// U Component
    float v;													// V Component
};

class CTex {
public:
    int t;
};

class CMesh {
public:
    CMesh();													// Mesh Constructor
    CMesh(CLog *pInLog, CGAF *pInGAF);
    CMesh(CLog *pInLog, CGAF * pInGAF, CGLTexture *pInTexture);	// Mesh Constructor
    ~CMesh();	     											// Mesh Deconstructor

    int			m_nVertexCount;								// Vertex Count
    CVert*		m_pVertices;								// Vertex Data
    CTexCoord*	m_pTexCoords;								// Texture Coordinates
    CTex*       m_pTex;
    bool        bMadeLog;

    int         x;
    int         y;

    CVert       pOffset;

    float       res;

    CGLTexture* pTexture;
    CLog*       pLog;
    CGAF*       pGAF;

    void        SetPointHeight(int nX,int nZ,float fHeight);
    void        Initialize(void);
    void        SetPointTexture(int nX, int nZ,CGLTexture* pTex);
    void        Draw(void);


    // bool        LoadHeightmap( char* szPath, float flHeightScale, float flResolution ); // Heightmap Loader
    // float       PtHeight( int nX, int nY ); // Single Point Height
};
