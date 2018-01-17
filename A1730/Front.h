//---------------------------------------------------------------------------
#ifndef FrontH
#define FrontH
#include "Signal.h"
//---------------------------------------------------------------------------
typedef void __fastcall(__closure * OnFrontDef)(unsigned int _tick);
class Front
{
public:
	CSignal* signal;
	bool value;
	OnFrontDef OnFront;
	Front(CSignal* _signal,bool _value,OnFrontDef _OnFront);
};

#endif
