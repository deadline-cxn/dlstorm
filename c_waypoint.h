
#ifndef _C_WAYPOINT_
#define _C_WAYPOINT_

#include "c_gl3dmodel.h"

class CWayPoint {
public:
    CWayPoint();
    ~CWayPoint();
    CVector3 Pos;
    CWayPoint *pNext;
    CWayPoint *pPrevious;
};


#endif // _C_WAYPOINT_

