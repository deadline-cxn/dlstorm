/***************************************************
 ** C_GL_MOUSE                                    **
 ***************************************************/

#ifndef C_GLMOUSE
#define C_GLMOUSE

#include "c_gaf.h"
#include "c_log.h"
#include "c_gltexture.h"

#define C_GLM_DOWNTICK_TIME 60
#define C_GLM_DOUBLECLICK_TIME 300

class C_MouseCursor
{
public:
    C_MouseCursor();
    C_MouseCursor(CLog *pInLog);
    C_MouseCursor(CGAF *pInGAF, CLog *pInLog);
    C_MouseCursor(char *fn);
    ~C_MouseCursor();

    CGLTexture      *Cursor;
    C_MouseCursor   *pNext;
    CGAF            *pGAF;
    CLog            *pLog;

    char            filename[1024];     // file name of the cursor
    unsigned char  r,g,b;
    int             x,y;
    char            x_offset,y_offset;
    char            x_hotspot,y_hotspot;

    GLvoid          reload();
    GLvoid          kill();
    GLvoid          load(char *file);
    GLvoid          draw(void);
};

class C_Mouse
{
public:

    C_Mouse();
    C_Mouse(CGAF *pInGAF, CLog *pInLog);
    ~C_Mouse();

    CLog            *pLog;
    CGAF            *pGAF;
    C_MouseCursor   *pCursor;

    void ClearClicks(void);

    bool bLeftDown;
    bool bMiddleDown;
    bool bRightDown;
    bool bLeftRelease;
    bool bMiddleRelease;
    bool bRightRelease;
    bool bLeftDblClick;
    bool bMiddleDblClick;
    bool bRightDblClick;
    bool bWheelUp;
    bool bWheelDown;
    bool bLeftDownTick;
    bool bMiddleDownTick;
    bool bRightDownTick;
    unsigned long lLeftDblClickTimer;
    unsigned long lMiddleDblClickTimer;
    unsigned long lRightDblClickTimer;
    unsigned long lLeftDownTick;
    unsigned long lMiddleDownTick;
    unsigned long lRightDownTick;
    int  ix;
    int  iy;

    void InitializeInput(void);
    void Refresh(void);

    bool LeftClick();
    bool MiddleClick();
    bool RightClick();

    bool Click(int iWhich);
    void SetClick(int iWhich,bool set);
    bool DoubleClick(int iWhich);
    void SetDoubleClick(int iWhich,bool set);

    bool ButtonDownTick(int iWhich);
    void SetDownTick(int iWhich,bool set);
    bool ButtonDown(int iWhich);
    void SetButtonDown(int iWhich,bool set);
    int  X(void);
    void SetX(int x);
    int  Y(void);
    void SetY(int y);
	int  WheelUp(void);
	void SetWheelUp(bool m);
    int  WheelDown(void);
	void SetWheelDown(bool m);
    bool InX(int x1,int x2);
    bool InY(int y1,int y2);
    bool In( int x1,int y1,int x2,int y2);
	bool InRect(RECT rc);
    bool InXPct(float fx1,float fx2);
    bool InYPct(float fy1,float fy2);
	bool InPct( float fx1,float fy1, float fx2,float fy2);
    void draw(void);

};

#endif//C_GLMOUSE
