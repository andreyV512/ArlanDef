#ifndef INIT_SG_H
#define INIT_SG_H

#include <Windows.h>


extern "C"
{
	class ISG_DLL
	{
	public:
		virtual int WINAPI OptionsWindow() = 0;
		virtual void WINAPI Compute2(wchar_t *typeSize, int channelSamplingRate, double *current, double *voltage, int count, wchar_t *groupName, double *result, unsigned *color) = 0;
		virtual void WINAPI Compute1(wchar_t *typeSize, int channelSamplingRate, double *data, int count, wchar_t *groupName, double *result, unsigned *color) = 0;
	};
}

ISG_DLL *InitSolid();
void DestroySolid(ISG_DLL **);
#endif