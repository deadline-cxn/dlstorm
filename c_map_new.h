#include "dlstorm.h"
#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"
#include "s_gnu.h"

#define MESH_RESOLUTION 1.0f									// Pixels Per Vertex
#define MESH_HEIGHTSCALE 1.0f									// Mesh Height Scale

class CVert														// Vertex Class
{
public:
	float x;													// X Component
	float y;													// Y Component
	float z;													// Z Component
};
typedef CVert CVec;												// The Definitions Are Synonymous

class CTexCoord													// Texture Coordinate Class
{
public:
	float u;													// U Component
	float v;													// V Component
};

class CTex
{
public:
    int t;
};

class CMesh
{
public:
	CMesh();													// Mesh Constructor
	CMesh(CLog *pInLog, CGAF *pInGAF);
	CMesh(CLog *pInLog, CGAF * pInGAF, CGLTexture *pInTexture);				// Mesh Constructor
	~CMesh();	     											// Mesh Deconstructor

	void            Initialize(void);
	int				m_nVertexCount;								// Vertex Count
	CVert*			m_pVertices;								// Vertex Data
	CTexCoord*		m_pTexCoords;								// Texture Coordinates
	CTex*            m_pTex;
	bool            bMadeLog;

	int     x;
	int     y;
	float   res;


	//bool LoadHeightmap( char* szPath, float flHeightScale, float flResolution ); // Heightmap Loader
	//float PtHeight( int nX, int nY ); // Single Point Height

	void SetPointHeight(int nX,int nY,float fHeight);

	CGLTexture *pTexture;
	CLog        *pLog;
	CGAF        *pGAF;

//    CxImage *himage;
};





typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;


class CMantraMap
{
public:
    CMantraMap();
    ~CMantraMap();

    SECTOR sector1;				// Our Model Goes Here:

    void Draw(void);
};


/*
class Terrain
{
public:

    Terrain( float heightScale = 500.0f, float blockScale = 2.0f );
    virtual ~Terrain();

    void Terminate();
    bool LoadHeightmap( const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height );
    bool LoadTexture( const std::string& filename, unsigned int textureStage = 0 );

    // Get the height of the terrain at a position in world space
    float GetHeightAt( const glm::vec3& position );

    void Render();
    // In debug builds, the terrain normals will be rendered.
    void DebugRender();

protected:
    void GenerateIndexBuffer();
    void GenerateNormals();
    // Generates the vertex buffer objects from the
    // position, normal, texture, and color buffers
    void GenerateVertexBuffers();

    void RenderNormals();

private:
    typedef std::vector<glm::vec3>  PositionBuffer;
    typedef std::vector<glm::vec4>  ColorBuffer;
    typedef std::vector<glm::vec3>  NormalBuffer;
    typedef std::vector<glm::vec2>  TexCoordBuffer;
    typedef std::vector<GLuint>     IndexBuffer;

    PositionBuffer  m_PositionBuffer;
    ColorBuffer     m_ColorBuffer;
    NormalBuffer    m_NormalBuffer;
    TexCoordBuffer  m_Tex0Buffer;
    IndexBuffer     m_IndexBuffer;

    // ID's for the VBO's
    GLuint  m_GLVertexBuffer;
    GLuint  m_GLNormalBuffer;
    GLuint  m_GLColorBuffer;
    GLuint  m_GLTex0Buffer;
    GLuint  m_GLTex1Buffer;
    GLuint  m_GLTex2Buffer;
    GLuint  m_GLIndexBuffer;

    static const unsigned int m_uiNumTextures = 3;
    GLuint  m_GLTextures[m_uiNumTextures];

    glm::mat4x4 m_LocalToWorldMatrix;

    // The dimensions of the heightmap texture
    glm::uivec2 m_HeightmapDimensions;

    // The height-map value will be multiplied by this value
    // before it is assigned to the vertex's Y-coordinate
    float   m_fHeightScale;
    // The vertex's X and Z coordinates will be multiplied by this
    // for each step when building the terrain
    float   m_fBlockScale;
};
*/
