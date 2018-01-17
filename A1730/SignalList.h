// ---------------------------------------------------------------------------
#ifndef SignalListH
#define SignalListH
#include<SyncObjs.hpp>
#include "Signal.h"
#include "uCRListT.h"
#include "Latch.h"
#include "A1730.h"
#include "Front.h"
#include <IniFiles.hpp>
class SignalList : private TThread
{
private:
	void __fastcall Execute();
	CRListT <CSignal> S;
	CRListT <Latch> L;
	CRListT <Front> F;
	void WriteSignals(void);
	void FlushSignals(DWORD _buf,DWORD _tick);
	bool Wait(bool _value,CSignal* _signal,DWORD _tm);
	A1730* dev;
	int period;
	void LatchesTerminate0(void);
	void CheckFront(DWORD _tick);
	void CheckWait(DWORD _tick);
	bool IsAlarm;
protected:
	CSignal* Find(AnsiString _name,bool _in);
	virtual bool CheckAlarm(void);
	TCriticalSection* cs;
	bool wasAlarm;
	TStringList* AlarmList;
	void prAlarm(void);
	void pr(AnsiString _msg);
public:
	SignalList(TIniFile* _ini);
	virtual __fastcall ~SignalList(void);
	void SetPeriod(int _period);
	inline int SignalCount(void)
	{
		return (S.Count());
	}
	inline CSignal* GetSignal(int _index)
	{
		return (S[_index]);
	}
	void LatchesTerminate(void);
	Front* SetFront(CSignal* _signal,bool _value,OnFrontDef _OnFront);
	void ClearFront(Front* _front);
	void ClearFronts(void);
	void SetAlarm(bool _on);
	bool WasAlarm(void);
	void GetAlarmList(TStringList* _alarmList);
};
#endif
