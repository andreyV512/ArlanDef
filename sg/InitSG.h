#ifndef INIT_SG_H
#define INIT_SG_H

#include <Windows.h>

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

extern "C"
{
	class ISG_DLL
	{
	public:
		virtual int WINAPI OptionsWindow() = 0;
		virtual void WINAPI Compute(wchar_t *typeSize, int channelSamplingRate, double *current, double *voltage, int count, wchar_t *groupName, double *result, unsigned *color) = 0;
		virtual void WINAPI Compute(wchar_t *typeSize, int channelSamplingRate, double *data, int count, wchar_t *groupName, double *result, unsigned *color) = 0;
	};
	
	EXPORT ISG_DLL *WINAPI SG_Init();
	EXPORT void WINAPI SG_Destroy(ISG_DLL **);
}

ISG_DLL *InitSolid();
void DestroySolid(ISG_DLL **);
#endif