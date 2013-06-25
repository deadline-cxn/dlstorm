/* DLStorm SDL Mouse Wrapper */
#ifndef _DLSTORM_INPUT_UTIL
#define _DLSTORM_INPUT_UTIL

#ifdef __cplusplus
extern "C"
{
#endif
    void InitializeInput(void);
    void ShutDownInput(void);
    void RefreshMouse(void);
    bool GetMouseRelease(int iWhich);
    void SetMouseRelease(int iWhich,bool set);
    bool GetMouseDown(int iWhich);
    void SetMouseDown(int iWhich,bool set);
    bool GetMouseDblClick(int iWhich);
    void SetMouseDblClick(int iWhich,bool set);
    int  GetMouseX(void);
    void SetMouseX(int x);
    int  GetMouseY(void);
    void SetMouseY(int y);
    
	int  GetMouseWheelUp(void);
	void SetMouseWheelUp(bool m);
    int  GetMouseWheelDown(void);
	void SetMouseWheelDown(bool m);
    bool MouseInX(int x1,int x2);
    bool MouseInY(int y1,int y2);
    bool MouseIn( int x1,int y1,int x2,int y2);
//    bool MouseInXPct(float fx1,float fx2);
//   bool MouseInYPct(float fy1,float fy2);
//	bool MouseInPct( float fx1,float fy1, float fx2,float fy2);
#ifdef __cplusplus
}
#endif
#endif//_DLSTORM_INPUT_UTIL

