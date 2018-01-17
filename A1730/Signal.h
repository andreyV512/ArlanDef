// ---------------------------------------------------------------------------
#ifndef SignalH
#define SignalH
#include<System.hpp>
#include<SyncObjs.hpp>

// ---------------------------------------------------------------------------
class CSignal
{
	friend class SignalList;
public:
	typedef void(__closure * tagOnSet)(void);
	typedef bool(__closure * tagOnWait)(bool _value, CSignal * _signal,
		DWORD _tm);
private:
	bool value;
	bool value_prev;
	AnsiString name;
	bool in;
	int index;
	tagOnSet OnSet;
	tagOnWait OnWait;
	TCriticalSection* cs;
	DWORD last_changed;

	bool WasConst0(bool _value, DWORD _period);

public:
	CSignal(AnsiString _name, bool _in, int _index, tagOnSet _OnSet,
		TCriticalSection* _cs, tagOnWait _OnWait);
	bool Get(void);
	bool WasConst(bool _value, DWORD _period);
	void Set(bool _value);
	bool Wait(bool _value, DWORD _tm);

	inline bool IsInput()
	{
		return (in);
	}

	inline int GetIndex()
	{
		return (index);
	}

	inline AnsiString GetName()
	{
		return (name);
	}
};
#endif
