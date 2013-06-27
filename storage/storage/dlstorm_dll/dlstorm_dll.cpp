// dlstorm_dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "dlstorm_dll.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
DLSTORM_DLL_API int nDlstorm_dll=0;

// This is an example of an exported function.
DLSTORM_DLL_API int fnDlstorm_dll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see dlstorm_dll.h for the class definition
CDlstorm_dll::CDlstorm_dll()
{ 
	return; 
}

