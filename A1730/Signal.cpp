// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Signal.h"
#include<SysUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CSignal::CSignal(AnsiString _name,bool _in,int _index,tagOnSet _OnSet,
	TCriticalSection* _cs,tagOnWait _OnWait)
{
	name=_name;
	in=_in;
	index=_index;
	value=false;
	value_prev=false;
	OnSet=_OnSet;
	OnWait=_OnWait;
	cs=_cs;
	last_changed=GetTickCount();
}

bool CSignal::Get(void)
{
	bool ret;
	cs->Enter();
	{
		ret=value;
	} cs->Leave();
	return (ret);
}
bool CSignal::WasConst(bool _value,DWORD _period)
{
	bool ret;
	DWORD last_changed1;
	cs->Enter();
	{
		ret=value;
		last_changed1=last_changed;
	} cs->Leave();
	if(last_changed1+_period<GetTickCount())
	{
		if(ret==_value)
			return (true);
	}
	return (false);
}
bool CSignal::WasConst0(bool _value,DWORD _period)
{
	if(last_changed+_period>GetTickCount())
	{
		if(value==_value)
			return (true);
	}
	return (false);
}

void CSignal::Set(bool _value)
{
	if(in)
	{
		AnsiString a="Signal::Set: попытка выставить входной сигнал: ";
		a+=name;
		throw(Exception(a));
	}
	cs->Enter();
	{
		value_prev=value;
		value=_value;
		OnSet();
	} cs->Leave();
}

bool CSignal::Wait(bool _value,DWORD _tm)
{
	if(!in)
	{
		AnsiString a="Signal::Wait: попытка подождать выходной сигнал: ";
		a+=name;
		throw(Exception(a));
	}
	if(Get()==_value)
		return (true);
	return (OnWait(_value,this,_tm));
}
