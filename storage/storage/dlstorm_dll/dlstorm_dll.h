
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLSTORM_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLSTORM_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLSTORM_DLL_EXPORTS
#define DLSTORM_DLL_API __declspec(dllexport)
#else
#define DLSTORM_DLL_API __declspec(dllimport)
#endif

// This class is exported from the dlstorm_dll.dll
class DLSTORM_DLL_API CDlstorm_dll {
public:
	CDlstorm_dll(void);
	// TODO: add your methods here.
};

extern DLSTORM_DLL_API int nDlstorm_dll;

DLSTORM_DLL_API int fnDlstorm_dll(void);

