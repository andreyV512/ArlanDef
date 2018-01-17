// ---------------------------------------------------------------------------
#ifndef A1730H
#define A1730H
#include <System.hpp>
#include "c:/Advantech/BioDAQ/Inc/BDaqCL.h"
using namespace Automation::BDaq;
class A1730
{
private:
	BDaqDevice *device;
	BDaqDio *dio;
	long startPort;
	long portCount;
	void pr(AnsiString _msg);
public:
	A1730(int _DevNum);
	~A1730(void);
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	inline bool IsOk(void)
	{
		return (device!=NULL);
	}
};

#endif
