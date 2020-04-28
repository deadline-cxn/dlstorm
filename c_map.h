/* Seth's Map Class */

#ifndef _SETH_MAP_BASE2
#define _SETH_MAP_BASE2

#include "dlcs.h"

#define NOT_A_TELEPORT 65535

class CTeleport {
   public:
    CTeleport();
    virtual ~CTeleport();

    bool bLoad(char *szFileName);
    bool bSave(char *szFileName);
    bool bClear(void);

    char szName[32];

    int iSourceX;
    int iSourceY;
    int iSourceZ;

    char szDestName[32];

    int iDestinationX;
    int iDestinationY;
    int iDestinationZ;

    int iKey;

    CTeleport *pNext;
    CTeleport *pPrevious;
};

class CMapObject {
   public:
    CMapObject();
    ~CMapObject();
    // fds
    char iType;       // Type of item
    char szName[32];  // Item name
    int  SID;
    int  iGFXIndex;           // Graphics index
    int  iX;                  // X coord / also for inventory slot location if owned
    int  iY;                  // Y coord
    int  iZ;                  // Z coord
    char szDescription[256];  // Description of item, also used for sign
                              // information.
    RECT        rcMouse;
    CMapObject *pNext;
    CMapObject *pPrevious;
};

#define GMP_MAPSIZE       30
#define GMP_SCREENMAPSIZE GMP_MAPSIZE *GMP_MAPSIZE * 9
#define BANKMAX           64
#define GMP_MAX_TILES     1024
#define GMP_MAX_OBJECTS   1024
#define GMP_MAPBUFFER     0

#define GMP_NO_OBJECT     255
#define GMP_NO_ITEM       0xFFFF
#define GMP_OBJECT_LAYERS 4

#define GMP_PROPERTY_MASK 0xFF

#define GMP_PROPERTY_BLOCKED   0x01
#define GMP_PROPERTY_INDOORS   0x02
#define GMP_PROPERTY_TELEPORT  0x04  // Make a teleport list and if this is set, get the teleport from the list -> server side teleports folder
#define GMP_PROPERTY_MODIFIER  0x08  // Make a modifier list and if this is set, get the modifier from the list -> server side modifiers folder
#define GMP_PROPERTY_LIQUID    0x10  // For animation, and possibly other checking
#define GMP_PROPERTY_SOUND     0x20  // Play a sound when player steps on this tile -> put listing in servers/nos/scripts/snd#-#-#.cfg
#define GMP_PROPERTY_NPC_BLOCK 0x40  // Blocked to NPCs, but not to players
#define GMP_PROPERTY_EVENT     0x80  // Execute a script on the server that invokes other things -> server side events folder

/*
#define GMP_PROPERTY_AI_REF     256
#define GMP_PROPERTY_FUTURE_1   512
#define GMP_PROPERTY_FUTURE_2   1024
#define GMP_PROPERTY_FUTURE_3   2048
#define GMP_PROPERTY_FUTURE_4   4096
#define GMP_PROPERTY_FUTURE_5   8192
#define GMP_PROPERTY_FUTURE_6   16384
#define GMP_PROPERTY_FUTURE_7   32768
*/

struct OBJECTLIST {
    u_char cBank;    // Which object bank
    u_char cObject;  // Which object in bank
};

struct FM_VERTEX {
    float Height;
    float Width;
    float R;
    float G;
    float B;
};

struct MAPDATA {                           // BYTES
    u_short    iTile;                      // 0-1
    OBJECTLIST Object[GMP_OBJECT_LAYERS];  // 2-9

    u_char cProperties;  // 10-13
    u_char cProperties2;
    u_char cProperties3;
    u_char cProperties4;

    FM_VERTEX Vertex[4];  // 14-33
};

typedef struct iItemData {
    char        cWhat;
    CMapObject *pFirstItem;
    CMapObject *pItem;
} ITEMDATA;

class CMap {
   public:
    CMap();
    CMap(u_short x, u_short y);
    CMap(u_short x, u_short y, u_char bank, u_char tile);
    virtual ~CMap();

    //////////////////////////////////////////////////////
    // Misc stuff

    bool    Initialize(u_short x, u_short y, u_char bank,
                       u_char tile);                                     // Initializes the map
    void    CleanUp(void);                                               // Cleans up allocated data
    bool    Save3D(char *szFilename);                                    // Saves 3d fmp file
    bool    Load3D(char *szFilename);                                    // Loads 3d fmp file
    bool    SaveSector3D(char *szDir, int iMapX, int iMapY, int iMapZ);  // Saves a 3d map sector
    bool    LoadSector3D(char *szDir, int iMapX, int iMapY, int iMapZ);  // Loads a 3d map sector
    u_short GetWidth(void);                                              // Returns the u_short Width of the map
    u_short GetHeight(void);                                             // Returns the u_short Height of the map
    void    Clear(void);

    //////////////////////////////////////////////////////
    // Item stuff

    void ClearItems(void);

    //////////////////////////////////////////////////////
    // Tile stuff

    int  GetTile(int x, int y);                            // Returns (bank*256+tile) - You MUST do a bit AND then divide  by 256 to get the bank, to get the tile, AND the LSB
    int  GetTile(int dt);                                  // Returns (bank*256+tile) - You MUST do a bit AND then divide by 256 to get the bank, to get the tile, AND the LSB
    void SetTile(int x, int y, u_char bank, u_char tile);  // Sets a tile
    void SetTile(int dt, u_char bank, u_char tile);
    void ClearTiles(void);                                                              // Clears all tiles to 0,0
    void FillTiles(u_char bank, u_char tile);                                           // Fills all tiles to bank and tile specified
    void FillRandom(u_char banklow, u_char bankhigh, u_char tilelow, u_char tilehigh);  // Fills in a random bank and tile based on banklow/bankhigh and tilelow/tilehigh

    //////////////////////////////////////////////////////
    // Object stuff

    u_short GetObj(int x, int y, int priority);                              // Returns (256*bank+object) - You MUST do a
                                                                             // bit AND then divide by 256 to get the
                                                                             // bank, to get the object, AND the LSB
    u_short GetObj(int dt, int priority);                                    // Returns (256*bank+object) - You MUST do a
                                                                             // bit AND then divide by 256 to get the
                                                                             // bank, to get the object, AND the LSB
    void SetObj(int x, int y, u_char bank, u_char object, u_char priority);  // Sets object to bank, object
    void SetObj(int dt, u_char bank, u_char object, u_char priority);        // Set object
    void ClearObjects(void);                                                 // Clears all objects from the map
    void FillObjects(u_char bank, u_char object, u_char priority);
    void FillRandomObjects(u_char banklow, u_char bankhigh, u_char objectlow, u_char objecthigh);

    //////////////////////////////////////////////////////
    // Property modification

    u_short cGetProperties(int iX, int iY);
    u_short cGetProperties(int iDT);

    void SetProperties(int iX, int iY, u_short iProperties);
    void SetProperties(int iDT, u_short iProperties);

    void ClearProperties(void);

    bool GetProperty(int x, int y, u_short prp);
    bool GetProperty(int dt, u_short prp);
    void SetProperty(int x, int y, u_short prp, bool yesno);
    void SetProperty(int st, u_short prp, bool yesno);

    void FillBlocked(bool bBlocked);
    void FillLiquid(bool bLiquid);

    //////////////////////////////////////////////////////
    // Vertex modification

    float GetVertexHeight(u_char cX, u_char cY, u_char cVertex);
    float GetVertexHeight(int dt, u_char cVertex);
    void  SetVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight);
    void  SetVertexHeight(int dt, u_char cVertex, float cHeight);
    void  SetSingleVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight);
    void  SetSingleVertexHeight(int dt, u_char cVertex, float cHeight);
    void  ClearVertexHeights(void);
    void  SmoothHeights(void);

    float GetVertexWidth(u_char cX, u_char cY, u_char cVertex);
    float GetVertexWidth(int dt, u_char cVertex);
    void  SetVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth);
    void  SetVertexWidth(int dt, u_char cVertex, float cWidth);
    void  SetSingleVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth);
    void  SetSingleVertexWidth(int dt, u_char cVertex, float cWidth);
    void  ClearVertexWidths(void);
    void  SmoothWidths(void);

    // u_long  GetVertexColor(u_char cX, u_char cY, u_char cVertex);
    float GetVertexColorR(u_char cX, u_char cY, u_char cVertex);
    float GetVertexColorG(u_char cX, u_char cY, u_char cVertex);
    float GetVertexColorB(u_char cX, u_char cY, u_char cVertex);
    void  SetVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor);
    void  SetVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue);
    void  SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor);
    void  SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue);
    void  ClearVertexColors(void);
    void  ClearVertexColors(float cRed, float cGreen, float cBlue);

    //////////////////////////////////////////////////////
    // Location stuff

    void MoveTo(int x, int y);
    void MoveTo(int x, int y, int z);
    int  GetX(void);
    void SetX(int x);
    int  GetY(void);
    void SetY(int y);
    int  GetZ(void);
    void SetZ(int z);

    //////////////////////////////////////////////////////
    //	Class members
    MAPDATA * pMapData;
    ITEMDATA *pItemData;
    int       SIZE;
    u_short   Width;
    u_short   Height;
    u_short   X;
    u_short   Y;
    u_short   Z;
    int       iKey;
    CMap *    pNext;
    CMap *    pPrevious;
};

#endif  // _EMBER_MAP_BASE
