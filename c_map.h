/* Seth's Map Class */

#ifndef _SETH_MAP_BASE2
#define _SETH_MAP_BASE2

#include "dlstorm.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "SDL.h"

#define GMP_MAPSIZE				30
#define GMP_SCREENMAPSIZE       GMP_MAPSIZE*GMP_MAPSIZE*9

struct FM_VERTEX {
    float  Height;
    float  Width;
    float  R;
    float  G;
    float  B;
};

struct MAPDATA {
    FM_VERTEX   Vertex[4];
    long         iTile;
};

class CMap {
public:
    CMap();
    CMap(u_short x,u_short y);
    CMap(u_short x,u_short y, u_char bank, u_char tile);
    virtual ~CMap();

    //////////////////////////////////////////////////////
    // Misc stuff

    bool	Initialize(u_short x, u_short y, u_char bank, u_char tile); // Initializes the map
    void	CleanUp(void);              // Cleans up allocated data
    bool	Save3D(char *szFilename);   // Saves 3d fmp file
    bool	Load3D(char *szFilename);   // Loads 3d fmp file
    bool    SaveSector3D(char *szDir, int iMapX, int iMapY, int iMapZ); // Saves a 3d map sector
    bool    LoadSector3D(char *szDir, int iMapX, int iMapY, int iMapZ); // Loads a 3d map sector
    u_short GetWidth(void);             // Returns the u_short Width of the map
    u_short GetHeight(void);            // Returns the u_short Height of the map
    void    Clear(void);

    //////////////////////////////////////////////////////
    // Tile stuff

    int		GetTile(int x, int y); // Returns (bank*256+tile) - You MUST do a bit AND then divide by 256 to get the bank, to get the tile, AND the LSB
    int		GetTile(int dt); // Returns (bank*256+tile) - You MUST do a bit AND then divide by 256 to get the bank, to get the tile, AND the LSB
    void	SetTile(int x, int y, u_char bank, u_char tile); // Sets a tile
    void    SetTile(int dt, u_char bank, u_char tile);
    void    ClearTiles(void); // Clears all tiles to 0,0
    void	FillTiles(u_char bank, u_char tile); // Fills all tiles to bank and tile specified
    void	FillRandom(u_char banklow , u_char bankhigh, u_char tilelow, u_char tilehigh); // Fills in a random bank and tile based on banklow/bankhigh and tilelow/tilehigh

    //////////////////////////////////////////////////////
    // Vertex modification

    float   GetVertexHeight(u_char cX, u_char cY, u_char cVertex);
    float   GetVertexHeight(int dt, u_char cVertex);
    void    SetVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight);
    void    SetVertexHeight(int dt, u_char cVertex,float cHeight);
    void    SetSingleVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight);
    void    SetSingleVertexHeight(int dt, u_char cVertex, float cHeight);
    void    ClearVertexHeights(void);
    void    SmoothHeights(void);

    float   GetVertexWidth(u_char cX, u_char cY, u_char cVertex);
    float   GetVertexWidth(int dt, u_char cVertex);
    void    SetVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth);
    void    SetVertexWidth(int dt, u_char cVertex, float cWidth);
    void    SetSingleVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth);
    void    SetSingleVertexWidth(int dt, u_char cVertex, float cWidth);
    void    ClearVertexWidths(void);
    void    SmoothWidths(void);

    //u_long  GetVertexColor(u_char cX, u_char cY, u_char cVertex);
    float   GetVertexColorR(u_char cX, u_char cY, u_char cVertex);
    float   GetVertexColorG(u_char cX, u_char cY, u_char cVertex);
    float   GetVertexColorB(u_char cX, u_char cY, u_char cVertex);
    void    SetVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor);
    void    SetVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue);
    void    SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor);
    void    SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue);
    void    ClearVertexColors(void);
    void    ClearVertexColors(float cRed, float cGreen, float cBlue);

    //////////////////////////////////////////////////////
    // Location stuff

    void	MoveTo(int x, int y);
    void	MoveTo(int x, int y, int z);
    int		GetX(void);
    void	SetX(int x);
    int		GetY(void);
    void	SetY(int y);
    int		GetZ(void);
    void	SetZ(int z);

    void    Render(void);

    //////////////////////////////////////////////////////
    //	Class members
    MAPDATA     *pMapData;

    int			SIZE;
    u_short		Width;
    u_short		Height;
    u_short		X;
    u_short		Y;
    u_short		Z;
    int         iKey;
    CMap        *pNext;
    CMap        *pPrevious;
};


#endif // _EMBER_MAP_BASE
