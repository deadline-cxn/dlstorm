/* Seth's Map Class */

#include "c_map.h"

//////////////////////////////////////////////////////
CMap::CMap() {
    Width=1;
    Height=1;
    SIZE=Width*Height;
    pMapData    = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
}
CMap::CMap(u_short x,u_short y) {
    Width=x;
    Height=y;
    SIZE=Width*Height;
    pMapData    = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
    Initialize(x,y,0,0);
}
CMap::CMap(u_short x,u_short y,u_char bank,u_char tile) {
    Width=x;
    Height=y;
    SIZE=Width*Height;
    pMapData    = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
    Initialize(x,y,bank,tile);
}
//////////////////////////////////////////////////////
CMap::~CMap() {
    CleanUp();
}
////////////////////////////////////////////////////// Create a new map, and fill it with a certain tile
bool CMap::Initialize(u_short x, u_short y, u_char bank, u_char tile) {
    CleanUp();
    int i;
    Width  = x;
    Height = y;
    SIZE = Width * Height;
    if(pMapData) delete [] pMapData;
    pMapData = new MAPDATA[SIZE];
    if(!pMapData) return false;
    return true;
}
////////////////////////////////////////////////////// Clean up the map data, tile, and object banks
void CMap::CleanUp(void) {
    if(pMapData) {
        delete [] pMapData;
        pMapData=NULL;
    }
}
////////////////////////////////////////////////////// Load the 3D map...
bool CMap::Load3D(char *szFileName) {
    if(szFileName == NULL) return false;
    FILE *fp;
    fp = fopen(szFileName, "rb");
    if(fp == NULL) return false;
    fread(&Width, sizeof(u_short), 1, fp);
    fread(&Height, sizeof(u_short), 1, fp);
    SIZE = Width * Height;
    for(int i=0; i<SIZE; i++)
        fread(&pMapData[i], sizeof(MAPDATA), 1, fp);
    fclose(fp);
    return true;
}
////////////////////////////////////////////////////// Load in a sector
bool CMap::LoadSector3D(char *szDir, int iMapX, int iMapY, int iMapZ) {
    char szFName[1024];
    memset(szFName,0,1024);
    sprintf(szFName,"%s%c%d-%d-%d.fm2",szDir,PATH_SEP,iMapX,iMapY,iMapZ);
    if(!Load3D(szFName)) {
        Clear();
        FillRandom(0,1,1,60);
        if(!Save3D(szFName)) return false;
    }
    return true;
}
////////////////////////////////////////////////////// Save the 3D map...
bool CMap::Save3D(char *szFilename) {
    FILE *fp;
    fp = fopen(szFilename, "wb");
    if(fp == NULL) return false;
    fwrite(&Width, sizeof(u_short), 1, fp);
    fwrite(&Height, sizeof(u_short), 1, fp);
    SIZE = Width * Height;
    for(int i=0; i<SIZE; i++)
        fwrite(&pMapData[i], sizeof(MAPDATA), 1, fp);
    fclose(fp);
    return true;
}
////////////////////////////////////////////////////// Save a 3D sector
bool CMap::SaveSector3D(char *szDir, int iMapX, int iMapY, int iMapZ) {
    char szFName[1024];
    memset(szFName,0,1024);
    sp_mkdir(szDir);
    sprintf(szFName,"%s%c%d-%d-%d.fm2",szDir,PATH_SEP,iMapX,iMapY,iMapZ);
    return(Save3D(szFName));
}
////////////////////////////////////////////////////// Get the map's width
u_short CMap::GetWidth(void) {
    return Width;
}
////////////////////////////////////////////////////// Get the map's height
u_short CMap::GetHeight(void) {
    return Height;
}
////////////////////////////////////////////////////// Clear the map data
void CMap::Clear(void) {
    ClearTiles();
//    ClearObjects();
//    ClearProperties();
    ClearVertexHeights();
    ClearVertexWidths();
    ClearVertexColors(255,255,255);
}
////////////////////////////////////////////////////// Returns the tile information at (X,Y) in the format of: u_short (LOu_char = Bank / HIu_char = Tile)
int CMap::GetTile(int x, int y) {
    return GetTile(x+y*Width);
}
int CMap::GetTile(int dt) {
    if((dt<0) || (dt>SIZE)) return 0;
    return pMapData[dt].iTile;
}
////////////////////////////////////////////////////// Sets the tile data for the maptile at (X,Y)
void CMap::SetTile(int x, int y, u_char bank, u_char tile) {
    SetTile(x+y*Width,bank,tile);
}
void CMap::SetTile(int dt,u_char bank, u_char tile) {
    if(!pMapData) return;
    if((dt<0)||(dt>SIZE)) return;
    pMapData[dt].iTile=(bank<<8)+tile;
}
////////////////////////////////////////////////////// Fill the map with a certain tile
void CMap::FillTiles(u_char bank, u_char tile) {
    SIZE=Width * Height;
    u_short t=(bank<<8)+tile;
    for(int i=0; i<SIZE; i++) {
        pMapData[i].iTile=t;
    }
}
//////////////////////////////////////////////////////
void CMap::FillRandom(u_char blow,u_char bhi,u_char tlow,u_char thi) {
    for(int dt=0; dt<SIZE; dt++) SetTile(dt,rand()%bhi+blow,rand()%thi+tlow);
}
////////////////////////////////////////////////////// Clear all objects from a map tilespace
void CMap::ClearTiles(void) {
    FillTiles(0,0);
}


////////////////////////////////////////////////////// Get Vertex Height
float CMap::GetVertexHeight(u_char cX, u_char cY, u_char cVertex) {
    return pMapData[cX+cY*Width].Vertex[cVertex].Height;
}
float CMap::GetVertexHeight(int dt, u_char cVertex) {
    if(cVertex>3) return 0;
    if((dt<0)||(dt>SIZE)) return 0;
    return pMapData[dt].Vertex[cVertex].Height;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight) {
    SetSingleVertexHeight(cX+cY*Width,cVertex,cHeight);
}
void CMap::SetSingleVertexHeight(int dt, u_char cVertex, float cHeight) {
    if(cVertex>3) return;
    if((dt<0)||(dt>SIZE)) return;
    pMapData[dt].Vertex[cVertex].Height=cHeight;
}
////////////////////////////////////////////////////// Set Vertex Height
void CMap::SetVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight) {
    SetVertexHeight((cX+cY*Width),cVertex,cHeight);
}
void CMap::SetVertexHeight(int dt, u_char cVertex, float cHeight) {
    int dta;
    int i=dt;
    if((dt<0)||(dt>SIZE)) return;
    switch(cVertex) {
    case 0:
        dta = i;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[0].Height=cHeight;
        dta = i + Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[3].Height=cHeight;
        dta = i + 1;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[1].Height=cHeight;
        dta = i + 1 + Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[2].Height=cHeight;
        break;

    case 1:
        dta = i + Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[2].Height=cHeight;
        dta = i;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[1].Height=cHeight;
        dta = i - 1 + Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[3].Height=cHeight;
        dta = i - 1;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[0].Height=cHeight;
        break;

    case 2:
        dta = i - 1;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[3].Height=cHeight;
        dta = i - 1 - Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[0].Height=cHeight;
        dta = i;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[2].Height=cHeight;
        dta = i - Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[1].Height=cHeight;
        break;

    case 3:
        dta = i - Width + 1;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[1].Height=cHeight;
        dta = i + 1;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[2].Height=cHeight;
        dta = i - Width;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[0].Height=cHeight;
        dta = i;
        if((dta>0)&&(dta<SIZE)) pMapData[dta].Vertex[3].Height=cHeight;
        break;

    default:
        break;
    }
}
//////////////////////////////////////////////////////
void CMap::SmoothHeights(void) {
    int dta;
    float vx;
    for(int i=(Width*2); i<(SIZE-Width*2); i++) {
        for(int vertex=0; vertex < 4; vertex++) {

            switch(vertex) {
            case 0:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[0].Height;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[0].Height;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[0].Height;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[0].Height;
                    vx/=4;

                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Height=vx;
                    dta = i + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Height=vx;
                    dta = i + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Height=vx;
                    dta = i + 1 + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Height=vx;
                }
                break;

            case 1:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[1].Height;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[1].Height;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[1].Height;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[1].Height;
                    vx/=4;

                    dta = i + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Height=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Height=vx;
                    dta = i - 1 + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Height=vx;
                    dta = i - 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Height=vx;
                }
                break;

            case 2:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[2].Height;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[2].Height;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[2].Height;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[2].Height;
                    vx/=4;

                    dta = i - 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Height=vx;
                    dta = i - 1 - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Height=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Height=vx;
                    dta = i - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Height=vx;
                }
                break;

            case 3:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[3].Height;
                    dta = i - 1;
                    vx+=pMapData[dta].Vertex[3].Height;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[3].Height;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[3].Height;
                    vx/=4;

                    dta = i - Width + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Height=vx;
                    dta = i + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Height=vx;
                    dta = i - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Height=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Height=vx;
                }
                break;

            default:
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////
void CMap::SmoothWidths(void) {
    int dta;
    float vx;
    for(int i=(Width*2); i<(SIZE-Width*2); i++) {
        for(int vertex=0; vertex < 4; vertex++) {

            switch(vertex) {
            case 0:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[0].Width;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[0].Width;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[0].Width;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[0].Width;
                    vx/=4;

                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Width=vx;
                    dta = i + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Width=vx;
                    dta = i + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Width=vx;
                    dta = i + 1 + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Width=vx;
                }
                break;

            case 1:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[1].Width;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[1].Width;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[1].Width;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[1].Width;
                    vx/=4;

                    dta = i + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Width=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Width=vx;
                    dta = i - 1 + Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Width=vx;
                    dta = i - 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Width=vx;
                }
                break;

            case 2:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[2].Width;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[2].Width;
                    dta = i + 1;
                    vx+=pMapData[dta].Vertex[2].Width;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[2].Width;
                    vx/=4;

                    dta = i - 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Width=vx;
                    dta = i - 1 - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Width=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Width=vx;
                    dta = i - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Width=vx;
                }
                break;

            case 3:

                dta = i ;
                if((dta>0)&&(dta<SIZE)) {
                    vx=pMapData[dta].Vertex[3].Width;
                    dta = i - 1;
                    vx+=pMapData[dta].Vertex[3].Width;
                    dta = i - Width;
                    vx+=pMapData[dta].Vertex[3].Width;
                    dta = i + Width;
                    vx+=pMapData[dta].Vertex[3].Width;
                    vx/=4;

                    dta = i - Width + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[1].Width=vx;
                    dta = i + 1;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[2].Width=vx;
                    dta = i - Width;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[0].Width=vx;
                    dta = i;
                    if((dta>0)&&(dta<SIZE))
                        pMapData[dta].Vertex[3].Width=vx;
                }
                break;

            default:
                break;
            }
        }
    }
}

////////////////////////////////////////////////////// Get Vertex Width
float CMap::GetVertexWidth(u_char cX, u_char cY, u_char cVertex) {
    return GetVertexWidth(cX+cY*Width,cVertex);
}
float CMap::GetVertexWidth(int dt,u_char cVertex) {
    if((dt>0)||(dt<SIZE)) return pMapData[dt].Vertex[cVertex].Width;
    return 0.0f;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth) {
    SetSingleVertexWidth(cX+cY*Width,cVertex,cWidth);
}
void CMap::SetSingleVertexWidth(int dt, u_char cVertex, float cWidth) {
    if(cVertex>3) return;
    if( (dt<0) || (dt>SIZE) ) return;
    pMapData[dt].Vertex[cVertex].Width=cWidth;
}
////////////////////////////////////////////////////// Set Vertex Width
void CMap::SetVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth) {
    SetVertexWidth(cX+cY*Width,cVertex,cWidth);
}
void CMap::SetVertexWidth(int dt, u_char cVertex, float cWidth) {
    if(cVertex>3) return;
    int dta;
    int i=dt;
    switch(cVertex) {
    case 0:
        dta = i;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[0].Width=cWidth;
        dta = i + Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[3].Width=cWidth;
        dta = i + 1;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[1].Width=cWidth;
        dta = i + 1 + Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[2].Width=cWidth;
        break;

    case 1:
        dta = i + Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[2].Width=cWidth;
        dta = i;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[1].Width=cWidth;
        dta = i - 1 + Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[3].Width=cWidth;
        dta = i - 1;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[0].Width=cWidth;
        break;

    case 2:
        dta = i - 1;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[3].Width=cWidth;
        dta = i - 1 - Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[0].Width=cWidth;
        dta = i;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[2].Width=cWidth;
        dta = i - Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[1].Width=cWidth;
        break;

    case 3:
        dta = i - Width + 1;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[1].Width=cWidth;
        dta = i + 1;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[2].Width=cWidth;
        dta = i - Width;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[0].Width=cWidth;
        dta = i;
        if((dta>0)&&(dta<SIZE))
            pMapData[dta].Vertex[3].Width=cWidth;
        break;

    default:
        break;
    }
}
////////////////////////////////////////////////////// Clear Vertex Heights
void CMap::ClearVertexHeights(void) {
    for(int dt=0; dt<SIZE; dt++) {
        for(int v=0; v<4; v++) {
            pMapData[dt].Vertex[v].Height=0.0f;
        }
    }
}
////////////////////////////////////////////////////// Clear Vertex Widths
void CMap::ClearVertexWidths(void) {
    for(int dt=0; dt<SIZE; dt++) {
        for(int v=0; v<4; v++) {
            pMapData[dt].Vertex[v].Width=0.0f;
        }
    }
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorR(u_char cX, u_char cY, u_char cVertex) {
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].R;
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorG(u_char cX, u_char cY, u_char cVertex) {
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].G;
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorB(u_char cX, u_char cY, u_char cVertex) {
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].B;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor) {
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    SetSingleVertexColor(cX,cY,cVertex,(float)((dwColor&255)/255),(float)(((dwColor>>8)&255)/255),(float)(((dwColor>>16)&255))/255);
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue) {
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    pMapData[cX+cY*Width].Vertex[cVertex].R=cRed;
    pMapData[cX+cY*Width].Vertex[cVertex].G=cGreen;
    pMapData[cX+cY*Width].Vertex[cVertex].B=cBlue;
}
////////////////////////////////////////////////////// Set vertex color (proper)
void CMap::SetVertexColor(u_char cX, u_char cY, u_char cVertex, float cR, float cG, float cB) {
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    int dta;
    int i=cX+cY*Width;
    switch(cVertex) {
    case 0:
        dta = i;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[0].R=cR;
            pMapData[dta].Vertex[0].G=cG;
            pMapData[dta].Vertex[0].B=cB;
        }
        dta = i + Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[3].R=cR;
            pMapData[dta].Vertex[3].G=cG;
            pMapData[dta].Vertex[3].B=cB;
        }
        dta = i + 1;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[1].R=cR;
            pMapData[dta].Vertex[1].G=cG;
            pMapData[dta].Vertex[1].B=cB;
        }
        dta = i + 1 + Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[2].R=cR;
            pMapData[dta].Vertex[2].G=cG;
            pMapData[dta].Vertex[2].B=cB;
        }
        break;

    case 1:
        dta = i + Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[2].R=cR;
            pMapData[dta].Vertex[2].G=cG;
            pMapData[dta].Vertex[2].B=cB;
        }
        dta = i;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[1].R=cR;
            pMapData[dta].Vertex[1].G=cG;
            pMapData[dta].Vertex[1].B=cB;
        }
        dta = i - 1 + Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[3].R=cR;
            pMapData[dta].Vertex[3].G=cG;
            pMapData[dta].Vertex[3].B=cB;
        }
        dta = i - 1;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[0].R=cR;
            pMapData[dta].Vertex[0].G=cG;
            pMapData[dta].Vertex[0].B=cB;
        }
        break;

    case 2:
        dta = i - 1;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[3].R=cR;
            pMapData[dta].Vertex[3].G=cG;
            pMapData[dta].Vertex[3].B=cB;
        }
        dta = i - 1 - Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[0].R=cR;
            pMapData[dta].Vertex[0].G=cG;
            pMapData[dta].Vertex[0].B=cB;
        }
        dta = i;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[2].R=cR;
            pMapData[dta].Vertex[2].G=cG;
            pMapData[dta].Vertex[2].B=cB;
        }
        dta = i - Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[1].R=cR;
            pMapData[dta].Vertex[1].G=cG;
            pMapData[dta].Vertex[1].B=cB;
        }
        break;

    case 3:
        dta = i - Width + 1;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[1].R=cR;
            pMapData[dta].Vertex[1].G=cG;
            pMapData[dta].Vertex[1].B=cB;
        }
        dta = i + 1;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[2].R=cR;
            pMapData[dta].Vertex[2].G=cG;
            pMapData[dta].Vertex[2].B=cB;
        }
        dta = i - Width;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[0].R=cR;
            pMapData[dta].Vertex[0].G=cG;
            pMapData[dta].Vertex[0].B=cB;
        }
        dta = i;
        if((dta>0)&&(dta<SIZE)) {
            pMapData[dta].Vertex[3].R=cR;
            pMapData[dta].Vertex[3].G=cG;
            pMapData[dta].Vertex[3].B=cB;
        }
        break;

    default:
        break;
    }
}
//////////////////////////////////////////////////////
void CMap::SetVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor) {
    SetVertexColor(cX,cY,cVertex,
                   (float) (  (float(dwColor&255))         / 255.0f),
                   (float) (  (float((dwColor>>8)&255))    / 255.0f),
                   (float) (  (float((dwColor>>16)&255))   / 255.0f));

}
//////////////////////////////////////////////////////
void CMap::ClearVertexColors(void) {
    ClearVertexColors(1.0f,1.0f,1.0f);
}
//////////////////////////////////////////////////////
void CMap::ClearVertexColors(float cRed, float cGreen, float cBlue) {
    for(int x=0; x<Width; x++) {
        for(int y=0; y<Height; y++) {
            for(int v=0; v<4; v++) {
                pMapData[x+y*Width].Vertex[v].R=cRed;
                pMapData[x+y*Width].Vertex[v].G=cGreen;
                pMapData[x+y*Width].Vertex[v].B=cBlue;
            }
        }
    }
}
////////////////////////////////////////////////////// Move the map draw area to (X,Y)
void    CMap::MoveTo(int x, int y) {
    X=x;
    Y=y;
}
////////////////////////////////////////////////////// Move the map draw area to (X,Y,Z)
void    CMap::MoveTo(int x, int y, int z) {
    X=x;
    Y=y;
    Z=z;
}
////////////////////////////////////////////////////// Get the X coordinate of the map
int     CMap::GetX(void) {
    return X;
}
////////////////////////////////////////////////////// Set just the X coordinate of the map
void    CMap::SetX(int x) {
    X = x;
}
////////////////////////////////////////////////////// Get the Y coordinate of the map
int     CMap::GetY(void) {
    return Y;
}
////////////////////////////////////////////////////// Set just the Y coordinate of the map
void    CMap::SetY(int y) {
    Y = y;
}
////////////////////////////////////////////////////// Get the Z coordinate of the map
int     CMap::GetZ(void) {
    return Z;
}
////////////////////////////////////////////////////// Set the Z coordinate of the map
void    CMap::SetZ(int z) {
    Z = z;
}
