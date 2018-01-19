#include "InitSG.h"

HMODULE hinstLib = NULL;
ISG_DLL *InitSolid()
{
	ISG_DLL *ret = NULL;
	hinstLib = LoadLibrary(TEXT("SG_DLL.dll"));
	if (NULL != hinstLib)
	{
		typedef ISG_DLL *(WINAPI *TSG_Init)();
		TSG_Init init = (TSG_Init) GetProcAddress(hinstLib, "_SG_Init@0");

		if (NULL != init)
		{
			ret = init();
		}
	}
	return ret;
}

void DestroySolid(ISG_DLL **o)
{
	if (NULL != hinstLib)
	{
		typedef ISG_DLL *(WINAPI *TSG_Destroy)(ISG_DLL **);
		TSG_Destroy destroy = (TSG_Destroy) GetProcAddress(hinstLib, "_SG_Destroy@4");
		if (NULL != destroy)
		{
			if(NULL != *o)destroy(o);
		}
		FreeLibrary(hinstLib);
		hinstLib = NULL;
	}
}