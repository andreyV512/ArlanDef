//---------------------------------------------------------------------------
#pragma hdrstop
#include "Front.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
Front::Front(CSignal* _signal,bool _value,OnFrontDef _OnFront)
{
	signal=_signal;
	value=_value;
	OnFront=_OnFront;
}
