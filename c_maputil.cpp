/***************************************************************
 ** EMBER                                                     **
 ***************************************************************/

#include "c_maputil.h"
#include "fm_util.h"
#include "global.h"
#include "c_map.h"
#include "c_console.h"

CEmMap                *pFragMap;                              // 9 Map Sectors
CEmMap                *pFirstFragMap;
CEmMap                *pOffScreenMap;                         // pOffScreenMap used for tile and object setting operations

CC_Map              *pMapBuffer;                            // All 9 maps in on, it is the rendered map

bool InitializeMap(void)
{
    DLog("Enter InitializeMap()...");
    if(!CreateOffScreenMap()) return 0;
    if(!CreateFragMaps())     return 0;
    if(!CreateMapBuffer())    return 0;
    DLog("Exit InitializeMap()...");
    return 1;
}

void ShutDownMap(void)
{
    DLog("Enter ShutDownMap()...");
    ClearFragMaps();
    ClearMapBuffer();
    ClearOffScreenMap();
    DLog("Exit ShutDownMap()...");
};

/**************************************************
 ** Clear the Frag Maps                          **
 **************************************************/

void ClearFragMaps(void)
{
    CEmMap *pDeleteFragMap=NULL;
    pFragMap=pFirstFragMap;
    while(pFragMap)
    {
        pDeleteFragMap=pFragMap;
        pFragMap=pFragMap->pNext;
        DEL(pDeleteFragMap);
    }
    pFragMap=NULL;
    pFirstFragMap=NULL;
}

bool CreateFragMaps(void)
{
    ClearFragMaps();
    pFragMap = new CEmMap;
    pFragMap->pMapData=NULL;
    pFirstFragMap=pFragMap; 
    for(int i=0;i<10;i++)
    {
        if(!pFragMap)
        {
            Log("Can't create FragMap buffers!");
            return 0;
        }
        if(!pFragMap->Initialize(GMP_MAPSIZE,GMP_MAPSIZE,0,5))
        {
            Log("Can't init FragMap buffers!");
            return 0;
        }
        pFragMap->iKey=i;
        pFragMap->pNext=new CEmMap;
        pFragMap=pFragMap->pNext;
    }
    pFragMap=pFirstFragMap;
    return 1;
}

void ClearOffScreenMap(void)
{
    DEL(pOffScreenMap);
}

bool CreateOffScreenMap(void)
{
    ClearOffScreenMap();
    pOffScreenMap = new CEmMap;
    if(pOffScreenMap == NULL)
    {
        Log("Can't create OffScreenMap!");
        return 0;
    }
    pOffScreenMap->Initialize(GMP_MAPSIZE,GMP_MAPSIZE,0,5);
    return 1;
}

void ClearMapBuffer(void)
{
    DEL(pMapBuffer);
}

bool CreateMapBuffer(void)
{
    ClearMapBuffer();
    pMapBuffer = new CC_Map;
    if(!pMapBuffer->bInitialize(GMP_MAPSIZE*3,GMP_MAPSIZE*3,0,0))
    {
            Log("Can't initialize map data! Not enough memory. Disconnecting.");
            ProcessConsoleCommand("disconnect",0);
            DLog("Exit InitializeMap()...");
            return 0;
    }
    pMapBuffer->SetMapDraw(-84,-114,14,25);
    pMapBuffer->MoveTo((GMP_MAPSIZE*3)/2,(GMP_MAPSIZE*3)/2);
    //ProcessConsoleCommand("refresh mapgfx",0);
    pMapBuffer->TileWidth=64;
    pMapBuffer->TileHeight=64;
    pMapBuffer->FillRandom(0,1,0,3);

    return 1;
}

/**************************************************
 ** Make the map seem seamless                   **
 **************************************************/

//void SeamlessMap(void)
//{
//}

/**************************************************
 ** Shift the Item Data when new map is loaded   **
 **************************************************/

void ShiftItems(int iDir)
{
    DLog("Enter ShiftItems()...");
    /*

    switch(iDir)
    {
        case NORTH:

            // clear bottom section

            for(x=0;x<pMapBuffer->Width;x++)
                for(y=(GMP_MAPSIZE*2);y<pMapBuffer->Height;y++)
                    pMapBuffer->pItemData[x+y*GMP_SIZE*3].pItem->bClear();

            for(x=0;x<pMapBuffer->Width;x++)
            {
                for(y=0;y<GMP_MAPSIZE*2;y++)
                {

                    // move middle to bottom

                    pMapBuffer->pItemData[x+(y+GMP_MAPSIZE*2)*GMP_SIZE*3].pItem->bCopy(pMapBuffer->pItemData[x+((y+GMP_SIZE)*GMP_SIZE*3)].pItem);
                    pMapBuffer->pItemData[x+((y+GMP_SIZE)*GMP_SIZE*3)].pItem->bClear();
                    
                    // move top to middle

                    pMapBuffer->pItemData[x+(y+GMP_MAPSIZE)*GMP_SIZE*3].pItem->bCopy(pMapBuffer->pItemData[x+(y*GMP_SIZE*3)].pItem);
                    pMapBuffer->pItemData[x+(y*GMP_SIZE*3)].pItem->bClear();
                }
            }

            break;

        case SOUTH:

            // clear top section

            for(x=0;x<pMapBuffer->Width;x++)
                for(y=0;y<GMP_MAPSIZE;y++)
                    pMapBuffer->pItemData[x+y*GMP_SIZE*3].pItem->bClear();

            // move middle section to top

            for(x=0;x<pMapBuffer->Width;x++)
            {
                for(y=GMP_MAPSIZE;y<pMapBuffer->Height;y++)
                {
                    pMapBuffer->pItemData[x+(y+GMP_MAPSIZE*2)*GMP_SIZE*3].pItem->bCopy(pMapBuffer->pItemData[x+((y+GMP_SIZE)*GMP_SIZE*3)].pItem);
                    pMapBuffer->pItemData[x+((y+GMP_SIZE)*GMP_SIZE*3)].pItem->bClear();
                    
                    pMapBuffer->pItemData[x+(y+GMP_MAPSIZE)*GMP_SIZE*3].pItem->bCopy(pMapBuffer->pItemData[x+(y*GMP_SIZE*3)].pItem);
                    pMapBuffer->pItemData[x+(y*GMP_SIZE*3)].pItem->bClear();
                }
            }

            break;

        case WEST:

            // clear right section

            // move middle section right
            
            // move left section to middle

            break;

        case EAST:

            // clear left section

            // move middle section left

            // move right section to middle

            break;

        default:
            break;

    }
/*


            // Copy Item Data (Then destroy it from the old section)
            
            pFragMap=pFirstFragMap;

            pMapBuffer->pItemData[x+y*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
                        
            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+GMP_MAPSIZE+y*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE+y*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;
            
            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+y*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+y*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;
            
            pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bClear();
            pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem->bCopy(pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem);
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            // Copy Item Data (Then destroy it from the old section)
            
            pFragMap=pFirstFragMap;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+y*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
                        
            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE+y*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE*2+y*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

            pFragMap=pFragMap->pNext;

            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();
            pFragMap->pItemData[x+y*GMP_MAPSIZE].pItem->bCopy(pMapBuffer->pItemData[x+GMP_MAPSIZE*2+(y+GMP_MAPSIZE*2)*GMP_MAPSIZE*3].pItem);
            pMapBuffer->pItemData[x+y*GMP_MAPSIZE].pItem->bClear();

  */

    DLog("Exit ShiftItems()...");
}


