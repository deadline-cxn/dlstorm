/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_MAP_UTIL
#define _EMBER_MAP_UTIL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C"
{
#endif
    bool InitializeMap(void);
    //void SeamlessMap(void);
    void ShutDownMap(void);
    void ShiftItems(int iDir);
    void ClearFragMaps(void);
    bool CreateFragMaps(void);
    void ClearOffScreenMap(void);
    bool CreateOffScreenMap(void);
    void ClearMapBuffer(void);
    bool CreateMapBuffer(void);

#ifdef __cplusplus
}
#endif

class CEmMap;
class CC_Map;

extern CEmMap               *pFragMap;                              // 9 Map Sectors
extern CEmMap               *pFirstFragMap;
extern CEmMap               *pDeleteFragMap;
extern CEmMap               *pOffScreenMap;                         // pOffScreenMap used for tile and object setting operations
extern CC_Map              *pMapBuffer;                            // All 9 maps in on


#endif
