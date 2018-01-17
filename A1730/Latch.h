// ---------------------------------------------------------------------------

#ifndef LatchH
#define LatchH
#include<Classes.hpp>
#include "Signal.h"

// ---------------------------------------------------------------------------
class Latch
{
public:
	bool value;
	CSignal* signal;
	TEvent* event;
	bool terminate;
	Latch(bool _value, CSignal* _signal,TEvent* _event);
};
#endif
