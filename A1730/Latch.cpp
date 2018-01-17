//---------------------------------------------------------------------------
#pragma hdrstop
#include "Latch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
Latch::Latch(bool _value,  CSignal* _signal,TEvent* _event)
{
	value=_value;
	signal=_signal;
	event=_event;
	terminate=false;
}


