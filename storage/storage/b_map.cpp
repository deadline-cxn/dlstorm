/* Seth's Map Class */

#include "b_map.h"

CMapObject::CMapObject()
{
    iType=0;            
    memset(szName,0,32);
    SID=0;
    iGFXIndex=0;
    iX=0;
    iY=0;
    iZ=0;
    memset(szDescription,0,256);
    rcMouse.bottom=0;
    rcMouse.left=0;
    rcMouse.right=0;
    rcMouse.top=0;
    pNext=NULL;
    pPrevious=NULL;
}
//////////////////////////////////////////////////////
CMapObject::~CMapObject()
{

}
//////////////////////////////////////////////////////
CMap::CMap()
{	
    Width=1;
    Height=1;
    SIZE=Width*Height;
    pMapData    = NULL;
    pItemData   = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
}
CMap::CMap(u_short x,u_short y)
{
    Width=x;
    Height=y;
    SIZE=Width*Height;
    pMapData    = NULL;
    pItemData   = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
    Initialize(x,y,0,0);
}
CMap::CMap(u_short x,u_short y,u_char bank,u_char tile)
{
    Width=x;
    Height=y;
    SIZE=Width*Height;
    pMapData    = NULL;
    pItemData   = NULL;
    pNext       = NULL;
    pPrevious   = NULL;
    Initialize(x,y,bank,tile);
}
//////////////////////////////////////////////////////
CMap::~CMap()
{ 
    CleanUp();
}
////////////////////////////////////////////////////// Create a new map, and fill it with a certain tile
bool CMap::Initialize(u_short x, u_short y, u_char bank, u_char tile)
{
    CleanUp();
    int i;
	Width  = x;
	Height = y;
    SIZE = Width * Height;
    if(pMapData) delete [] pMapData;
    pMapData = new MAPDATA[SIZE];
    if(!pMapData) return false;
    if(pItemData) delete [] pItemData;
    pItemData = new ITEMDATA[SIZE];
    if(pItemData==NULL) { delete [] pMapData; return false; }
    for(i=0;i<SIZE;i++)
    {
        pItemData[i].pItem      = NULL;
        pItemData[i].pFirstItem = NULL;
        pItemData[i].pItem      = new CMapObject;
        pItemData[i].pFirstItem = pItemData[i].pItem;
    }
    return true;
}
////////////////////////////////////////////////////// Clean up the map data, tile, and object banks
void CMap::CleanUp(void)
{	
    if(pMapData)
    {
        delete [] pMapData;
        pMapData=NULL;
    }
    CMapObject *pHi=NULL;
    CMapObject *pFirstHi=NULL;
    int j=0;
    if(pItemData)
    {
        for(int i=0;i<SIZE;i++)
        {
            if(pItemData[i].pFirstItem)
            {
                pItemData[i].pItem=pItemData[i].pFirstItem;

                while(pItemData[i].pItem)
                {
                    if(!pFirstHi)
                    {
                        pFirstHi=pItemData[i].pItem;
                        pHi=pFirstHi;
                    }
                    else
                    {
                        pHi->pNext=pItemData[i].pItem;
                        pHi=pHi->pNext;
                    }
                    pItemData[i].pItem=pItemData[i].pItem->pNext;
                    j++;
                }
            }
        }
        delete [] pItemData;
        pItemData=NULL;
        pHi=pFirstHi;
        while(pHi)
        {
            pFirstHi=pHi;
            pHi=pHi->pNext;
            DEL(pFirstHi);
        }
    }
}
////////////////////////////////////////////////////// Load the 3D map...
bool CMap::Load3D(char *szFileName)
{
	if(szFileName == NULL) return false;
	FILE *fp;	
	fp = fopen(szFileName, "rb");
	if(fp == NULL) return false;
	fread(&Width, sizeof(u_short), 1, fp);
	fread(&Height, sizeof(u_short), 1, fp);
	SIZE = Width * Height;
	for(int i=0;i<SIZE;i++)
		fread(&pMapData[i], sizeof(MAPDATA), 1, fp);
	fclose(fp);
	return true;
}
////////////////////////////////////////////////////// Load in a sector
bool CMap::LoadSector3D(char *szDir, int iMapX, int iMapY, int iMapZ)
{
    char szFName[1024];
    memset(szFName,0,1024);
    sprintf(szFName,"%s%c%d-%d-%d.fm2",szDir,PATH_SEP,iMapX,iMapY,iMapZ);
    if(!Load3D(szFName))
    {
        Clear();
        FillRandom(0,1,1,60);
        if(!Save3D(szFName)) return false;
    }
    return true;
}
////////////////////////////////////////////////////// Save the 3D map...
bool CMap::Save3D(char *szFilename)
{
	FILE *fp;
	fp = fopen(szFilename, "wb");
	if(fp == NULL) return false;
	fwrite(&Width, sizeof(u_short), 1, fp);
	fwrite(&Height, sizeof(u_short), 1, fp);
	SIZE = Width * Height;
	for(int i=0;i<SIZE;i++)
		fwrite(&pMapData[i], sizeof(MAPDATA), 1, fp);
	fclose(fp);
	return true;
}
////////////////////////////////////////////////////// Save a 3D sector
bool CMap::SaveSector3D(char *szDir, int iMapX, int iMapY, int iMapZ)
{
    char szFName[1024];
    memset(szFName,0,1024);
    sp_mkdir(szDir);
    sprintf(szFName,"%s%c%d-%d-%d.fm2",szDir,PATH_SEP,iMapX,iMapY,iMapZ);
    return(Save3D(szFName));
}
////////////////////////////////////////////////////// Get the map's width
u_short CMap::GetWidth(void)  { return Width; }
////////////////////////////////////////////////////// Get the map's height
u_short CMap::GetHeight(void) { return Height; }
////////////////////////////////////////////////////// Clear the map data
void CMap::Clear(void)
{
    ClearTiles();
    ClearObjects();
    ClearProperties();
    ClearVertexHeights();
    ClearVertexWidths();
    ClearVertexColors(255,255,255);
}
////////////////////////////////////////////////////// Clear the item data
void CMap::ClearItems(void)
{
    //int i;
    //for(i=0;i<SIZE;i++) { //pItemData[i].pItem->bClear();    }
}
////////////////////////////////////////////////////// Returns the tile information at (X,Y) in the format of: u_short (LOu_char = Bank / HIu_char = Tile)
int CMap::GetTile(int x, int y) { return GetTile(x+y*Width); }
int CMap::GetTile(int dt)       { if((dt<0) || (dt>SIZE)) return 0; return pMapData[dt].iTile; }
////////////////////////////////////////////////////// Sets the tile data for the maptile at (X,Y)
void CMap::SetTile(int x, int y, u_char bank, u_char tile) { SetTile(x+y*Width,bank,tile); }
void CMap::SetTile(int dt,u_char bank, u_char tile)
{
    if((dt<0)||(dt>SIZE)) return;
    pMapData[dt].iTile=(bank<<8)+tile;
}
////////////////////////////////////////////////////// Fill the map with a certain tile
void CMap::FillTiles(u_char bank, u_char tile)
{
	SIZE=Width * Height; u_short t=(bank<<8)+tile;
    for(int i=0; i<SIZE; i++) { pMapData[i].iTile=t; }
}
//////////////////////////////////////////////////////
void CMap::FillRandom(u_char blow,u_char bhi,u_char tlow,u_char thi) { for(int dt=0;dt<SIZE;dt++) SetTile(dt,rand()%bhi+blow,rand()%thi+tlow); }
////////////////////////////////////////////////////// Clear all objects from a map tilespace
void CMap::ClearTiles(void)
{ 
    FillTiles(0,0);
}
////////////////////////////////////////////////////// Returns the object information at (X,Y,level) in the format of:
unsigned short CMap::GetObj(int x, int y, int priority){ return GetObj(x+y*Width,priority);  }
unsigned short CMap::GetObj(int dt, int priority)
{
    if((dt<0)||(dt>SIZE)) return 0;
    return (pMapData[dt].Object[priority].cBank<<8)+(pMapData[dt].Object[priority].cObject);
}
////////////////////////////////////////////////////// Sets the object at x,y using priority 
void CMap::SetObj(int x,int y, u_char bank, u_char object, u_char priority) { SetObj(x+y*Width,bank,object,priority); }
void CMap::SetObj(int dt, u_char bank, u_char object, u_char priority)
{
    if((dt<0)||(dt>SIZE)) return;
    pMapData[dt].Object[priority].cBank=bank;
	pMapData[dt].Object[priority].cObject=object;
}
////////////////////////////////////////////////////// Clear all objects from a map tilespace
void CMap::ClearObjects(void)
{
	for(int dt=0;dt<SIZE;dt++)
    {
		for(int pp=0;pp<GMP_OBJECT_LAYERS;pp++)
		{
			SetObj(dt,GMP_NO_OBJECT,GMP_NO_OBJECT,pp);
		}
    }
}
////////////////////////////////////////////////////// Clears map properties
void CMap::ClearProperties(void)
{
	for(int dt=0;dt<SIZE;dt++)
    {
        pMapData[dt].cProperties=(u_char)NULL;
        pMapData[dt].cProperties2=(u_char)NULL;
        pMapData[dt].cProperties3=(u_char)NULL;
        pMapData[dt].cProperties4=(u_char)NULL;
    }
}
////////////////////////////////////////////////////// Get map properties
u_short CMap::cGetProperties(int x, int y) { return pMapData[y*Width+x].cProperties; }
u_short CMap::cGetProperties(int dt)
{
    if((dt<0) || (dt>SIZE)) return 0;
    return pMapData[dt].cProperties;
}
////////////////////////////////////////////////////// Set map properties (precalculated)
////////////////////////////////////////////////////// Set map properties
void CMap::SetProperties(int x, int y, u_short cProperties){ SetProperties(y*Width+x,cProperties); }
void CMap::SetProperties(int dt, u_short cProperties)
{
    if((dt<0) || (dt>SIZE)) return;
    pMapData[dt].cProperties=cProperties;
}
//////////////////////////////////////////////////////
bool CMap::GetProperty(int x, int y, u_short prp){ return GetProperty((y*Width+x),prp); }
bool CMap::GetProperty(int dt, u_short prp)
{
    if((dt<0) || (dt>SIZE)) return 0;
    return TOBOOL(pMapData[dt].cProperties&prp);
}
//////////////////////////////////////////////////////
void CMap::SetProperty(int x, int y, u_short prp, bool yesno) { SetProperty((y*Width+x),prp,yesno); }
void CMap::SetProperty(int dt, u_short prp, bool yesno)
{
    if((dt<0) || (dt>SIZE)) return;
	if(yesno) pMapData[dt].cProperties|=prp;
    else      pMapData[dt].cProperties=(pMapData[dt].cProperties & (GMP_PROPERTY_MASK - prp));
}
//////////////////////////////////////////////////////
void    CMap::FillBlocked(bool bBlocked)
{
    for(int dt=0;dt<SIZE;dt++)
    {
        SetProperty(dt,GMP_PROPERTY_BLOCKED,bBlocked);
    }
}
//////////////////////////////////////////////////////
void CMap::FillLiquid(bool bLiquid)
{
    for(int dt=0;dt<SIZE;dt++)
    {
        SetProperty(dt,GMP_PROPERTY_LIQUID,bLiquid);
    }
}
////////////////////////////////////////////////////// Get Vertex Height
float CMap::GetVertexHeight(u_char cX, u_char cY, u_char cVertex) { return pMapData[cX+cY*Width].Vertex[cVertex].Height; }
float CMap::GetVertexHeight(int dt, u_char cVertex)
{
    if(cVertex>3) return 0;
    if((dt<0)||(dt>SIZE)) return 0; return pMapData[dt].Vertex[cVertex].Height;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight){ SetSingleVertexHeight(cX+cY*Width,cVertex,cHeight); }
void CMap::SetSingleVertexHeight(int dt, u_char cVertex, float cHeight)
{
    if(cVertex>3) return;
    if((dt<0)||(dt>SIZE)) return;
    pMapData[dt].Vertex[cVertex].Height=cHeight;
}
////////////////////////////////////////////////////// Set Vertex Height
void CMap::SetVertexHeight(u_char cX, u_char cY, u_char cVertex, float cHeight) { SetVertexHeight((cX+cY*Width),cVertex,cHeight); }
void CMap::SetVertexHeight(int dt, u_char cVertex, float cHeight)
{
    int dta;
    int i=dt;
    if((dt<0)||(dt>SIZE)) return;
    switch(cVertex)
    {
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
void CMap::SmoothHeights(void)
{
    int dta; float vx;
    for(int i=(Width*2);i<(SIZE-Width*2);i++)
    {
        for(int vertex=0; vertex < 4; vertex++)
        {

            switch(vertex)
            {
                case 0:
                    
                    dta = i ;
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
void CMap::SmoothWidths(void)
{
    int dta; float vx;
    for(int i=(Width*2);i<(SIZE-Width*2);i++)
    {
        for(int vertex=0; vertex < 4; vertex++)
        {

            switch(vertex)
            {
                case 0:
                    
                    dta = i ;
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
                    if((dta>0)&&(dta<SIZE))
                    {
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
float CMap::GetVertexWidth(u_char cX, u_char cY, u_char cVertex) { return GetVertexWidth(cX+cY*Width,cVertex); }
float CMap::GetVertexWidth(int dt,u_char cVertex)
{
    if((dt>0)||(dt<SIZE)) return pMapData[dt].Vertex[cVertex].Width;
    return 0.0f;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth) { SetSingleVertexWidth(cX+cY*Width,cVertex,cWidth); }
void CMap::SetSingleVertexWidth(int dt, u_char cVertex, float cWidth)
{
    if(cVertex>3) return;
    if( (dt<0) || (dt>SIZE) ) return;
    pMapData[dt].Vertex[cVertex].Width=cWidth;
}
////////////////////////////////////////////////////// Set Vertex Width
void CMap::SetVertexWidth(u_char cX, u_char cY, u_char cVertex, float cWidth) { SetVertexWidth(cX+cY*Width,cVertex,cWidth); }
void CMap::SetVertexWidth(int dt, u_char cVertex, float cWidth)
{
    if(cVertex>3) return;
    int dta;
    int i=dt;
    switch(cVertex)
    {
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
void CMap::ClearVertexHeights(void)
{
    for(int dt=0;dt<SIZE;dt++)
    {
        for(int v=0;v<4;v++)
        {
            pMapData[dt].Vertex[v].Height=0.0f;
        }
    }
}
////////////////////////////////////////////////////// Clear Vertex Widths
void CMap::ClearVertexWidths(void)
{
    for(int dt=0;dt<SIZE;dt++)
    {
        for(int v=0;v<4;v++)
        {
            pMapData[dt].Vertex[v].Width=0.0f;
        }
    }
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorR(u_char cX, u_char cY, u_char cVertex)
{
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].R;
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorG(u_char cX, u_char cY, u_char cVertex)
{
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].G;
}
//////////////////////////////////////////////////////
float CMap::GetVertexColorB(u_char cX, u_char cY, u_char cVertex)
{
    if(cVertex>3) return 0;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return 0;
    return pMapData[cX+cY*Width].Vertex[cVertex].B;
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor)
{
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    SetSingleVertexColor(cX,cY,cVertex,(float)((dwColor&255)/255),(float)(((dwColor>>8)&255)/255),(float)(((dwColor>>16)&255))/255);
}
//////////////////////////////////////////////////////
void CMap::SetSingleVertexColor(u_char cX, u_char cY, u_char cVertex, float cRed, float cGreen, float cBlue)
{
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    pMapData[cX+cY*Width].Vertex[cVertex].R=cRed;
    pMapData[cX+cY*Width].Vertex[cVertex].G=cGreen;
    pMapData[cX+cY*Width].Vertex[cVertex].B=cBlue;
}
////////////////////////////////////////////////////// Set vertex color (proper)
void CMap::SetVertexColor(u_char cX, u_char cY, u_char cVertex, float cR, float cG, float cB)
{
    if(cVertex>3) return;
    if((cX+cY*SIZE<0)||(cX+cY*Width>SIZE)) return;
    int dta;
    int i=cX+cY*Width;
    switch(cVertex)
    {
        case 0:
            dta = i;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[0].R=cR;
                pMapData[dta].Vertex[0].G=cG;
                pMapData[dta].Vertex[0].B=cB;
            }
            dta = i + Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[3].R=cR;
                pMapData[dta].Vertex[3].G=cG;
                pMapData[dta].Vertex[3].B=cB;
            }
            dta = i + 1;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[1].R=cR;
                pMapData[dta].Vertex[1].G=cG;
                pMapData[dta].Vertex[1].B=cB;
            }
            dta = i + 1 + Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[2].R=cR;
                pMapData[dta].Vertex[2].G=cG;
                pMapData[dta].Vertex[2].B=cB;
            }
            break;

        case 1:
            dta = i + Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[2].R=cR;
                pMapData[dta].Vertex[2].G=cG;
                pMapData[dta].Vertex[2].B=cB;
            }
            dta = i;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[1].R=cR;
                pMapData[dta].Vertex[1].G=cG;
                pMapData[dta].Vertex[1].B=cB;
            }
            dta = i - 1 + Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[3].R=cR;
                pMapData[dta].Vertex[3].G=cG;
                pMapData[dta].Vertex[3].B=cB;
            }
            dta = i - 1;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[0].R=cR;
                pMapData[dta].Vertex[0].G=cG;
                pMapData[dta].Vertex[0].B=cB;
            }
            break;

        case 2:
            dta = i - 1;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[3].R=cR;
                pMapData[dta].Vertex[3].G=cG;
                pMapData[dta].Vertex[3].B=cB;
            }
            dta = i - 1 - Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[0].R=cR;
                pMapData[dta].Vertex[0].G=cG;
                pMapData[dta].Vertex[0].B=cB;
            }
            dta = i;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[2].R=cR;
                pMapData[dta].Vertex[2].G=cG;
                pMapData[dta].Vertex[2].B=cB;
            }
            dta = i - Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[1].R=cR;
                pMapData[dta].Vertex[1].G=cG;
                pMapData[dta].Vertex[1].B=cB;
            }
            break;

        case 3:
            dta = i - Width + 1;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[1].R=cR;
                pMapData[dta].Vertex[1].G=cG;
                pMapData[dta].Vertex[1].B=cB;
            }
            dta = i + 1;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[2].R=cR;
                pMapData[dta].Vertex[2].G=cG;
                pMapData[dta].Vertex[2].B=cB;
            }
            dta = i - Width;
            if((dta>0)&&(dta<SIZE))
            {
                pMapData[dta].Vertex[0].R=cR;
                pMapData[dta].Vertex[0].G=cG;
                pMapData[dta].Vertex[0].B=cB;
            }
            dta = i;
            if((dta>0)&&(dta<SIZE))
            {
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
void CMap::SetVertexColor(u_char cX, u_char cY, u_char cVertex, u_long dwColor)
{
    SetVertexColor(cX,cY,cVertex,
        (float) (  (float(dwColor&255))         / 255.0f),
        (float) (  (float((dwColor>>8)&255))    / 255.0f),
        (float) (  (float((dwColor>>16)&255))   / 255.0f));
    
}
//////////////////////////////////////////////////////
void CMap::ClearVertexColors(void)
{
    ClearVertexColors(1.0f,1.0f,1.0f);
}
//////////////////////////////////////////////////////
void CMap::ClearVertexColors(float cRed, float cGreen, float cBlue)
{
    for(int x=0;x<Width;x++)
    {
        for(int y=0;y<Height;y++)
        {
            for(int v=0;v<4;v++)
            {
                pMapData[x+y*Width].Vertex[v].R=cRed;
                pMapData[x+y*Width].Vertex[v].G=cGreen;
                pMapData[x+y*Width].Vertex[v].B=cBlue;
            }
        }
    }
}
////////////////////////////////////////////////////// Move the map draw area to (X,Y)
void    CMap::MoveTo(int x, int y) { X=x; Y=y; }
////////////////////////////////////////////////////// Move the map draw area to (X,Y,Z)
void    CMap::MoveTo(int x, int y, int z) { X=x; Y=y; Z=z; }
////////////////////////////////////////////////////// Get the X coordinate of the map
int     CMap::GetX(void) { return X; }
////////////////////////////////////////////////////// Set just the X coordinate of the map
void    CMap::SetX(int x) { X = x; }
////////////////////////////////////////////////////// Get the Y coordinate of the map
int     CMap::GetY(void)  { return Y; }
////////////////////////////////////////////////////// Set just the Y coordinate of the map
void    CMap::SetY(int y) { Y = y; }
////////////////////////////////////////////////////// Get the Z coordinate of the map
int     CMap::GetZ(void) { return Z; }
////////////////////////////////////////////////////// Set the Z coordinate of the map
void    CMap::SetZ(int z) { Z = z; }
////////////////////////////////////////////////////// Teleport Class
CTeleport::CTeleport()
{ 
    bClear();
}
//////////////////////////////////////////////////////
CTeleport::~CTeleport()
{

}
//////////////////////////////////////////////////////
bool CTeleport::bClear(void)
{
    strcpy(szName,"UNK_TELEPORT");
    strcpy(szDestName,"UNK_TELEPORT");
    iSourceX=NOT_A_TELEPORT;
    iSourceY=NOT_A_TELEPORT;
    iSourceZ=NOT_A_TELEPORT;
    iDestinationX=NOT_A_TELEPORT;
    iDestinationY=NOT_A_TELEPORT;
    iDestinationZ=NOT_A_TELEPORT;
    iKey=NOT_A_TELEPORT;
    pNext=NULL;
    pPrevious=NULL;
	return true;
}
//////////////////////////////////////////////////////
bool CTeleport::bLoad(char *FileName)
{
	char *szEntry;
	char In[256];
	FILE *fp;
	fp=fopen(FileName,"rt");
	if(!fp)
		return false;

    strcpy(szName,"UNK_TELEPORT");
    strcpy(szDestName,"UNK_TELEPORT");

	while(1)
	{
		if(!fgets(In,255,fp)) break;

        szEntry=strtok(In,"=\n\r");
		if(szEntry!=NULL)
		{

            if(!strcmp(szEntry,"SRC_NAME"))
            {
                szEntry=strtok(NULL,",\n\r");
                if(szEntry)
                {
                    strcpy(szName,szEntry);
                }
            }

            if(!strcmp(szEntry,"SRC"))
            {
				szEntry=strtok(NULL,",\n\r");
				if(szEntry!=NULL)
				{
					iSourceX=atoi(szEntry);
					szEntry=strtok(NULL,",\n\r");
					if(szEntry!=NULL)
					{
						iSourceY=atoi(szEntry);
						szEntry=strtok(NULL,",\n\r");
						if(szEntry!=NULL)
						{
							iSourceZ=atoi(szEntry);
						}
					}
				}
            }

            if(!strcmp(szEntry,"DST_NAME"))
            {
                szEntry=strtok(NULL,",\n\r");
                if(szEntry)
                {
                    strcpy(szDestName,szEntry);
                }
            }

            if(!strcmp(szEntry,"DST"))
            {
				szEntry=strtok(NULL,",\n\r");
				if(szEntry!=NULL)
				{
					iDestinationX=atoi(szEntry);
					szEntry=strtok(NULL,",\n\r");
					if(szEntry!=NULL)
					{
						iDestinationY=atoi(szEntry);
						szEntry=strtok(NULL,",\n\r");
						if(szEntry!=NULL)
						{
							iDestinationZ=atoi(szEntry);

						}
					}
				}
            }
		}
	}

	fclose(fp);
	return true;
}
//////////////////////////////////////////////////////
bool CTeleport::bSave(char *szFileName)
{
	char szTemp[256];
	FILE *fp;
	fp=fopen(szFileName,"wt");

	if(!fp)
		return false;

    sprintf( szTemp,"SRC_NAME=%s",szName);
    fputs(szTemp,fp);

	sprintf( szTemp, "SRC=%d,%d,%d\n",iSourceX, iSourceY, iSourceZ);
	fputs(szTemp,fp);

    sprintf( szTemp,"DST_NAME=%s",szDestName);
    fputs(szTemp,fp);

	sprintf( szTemp, "DST=%d,%d,%d\n",iDestinationX,iDestinationY,iDestinationZ);
	fputs(szTemp,fp);

	fclose(fp);

	return true;
}
//////////////////////////////////////////////////////
