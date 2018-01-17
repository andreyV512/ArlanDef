// ---------------------------------------------------------------------------
#ifndef A1730H
#define A1730H
#include "c:/Advantech/BioDAQ/Inc/BDaqCL.h"
#include<Classes.hpp>
#include<SyncObjs.hpp>
#include "Signal.h"
#include "uCRListT.h"
#include "Latch.h"
#include<IniFiles.hpp>

using namespace Automation::BDaq;
typedef void __fastcall(__closure * OnEventDef)(void);
typedef void __fastcall(__closure * OnFrontDef)(unsigned int _tick);

// ---------------------------------------------------------------------------
class A1730 : private TThread
{
private:
	A1730(int _DevNum, AnsiString _file_name, bool _ReadOnly);

	BDaqDevice *device;
	BDaqDio *dio;
	int period;
	TCriticalSection* cs;
	long startPort;
	long portCount;
	CRListT<CSignal>M;
	CRListT<Latch>L;

	void __fastcall Execute();
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	bool GetIn(int _index);
	bool GetOut(int _index);
	void Set(int _index, bool _value);
	void WriteSignals(void);
	void ReadSignals(void);
	CSignal* Find(AnsiString _name);
	bool Wait(bool _value, CSignal* _signal, DWORD _tm, TThread* _thread);
	void Alarm(void);
	bool alarmCycleOn;
	AnsiString reason;
	OnFrontDef OnFront;

	void SendFront(unsigned int _tick);
	void Latchterminate0(void);
	bool IsAlarm;
public:
	static A1730* Create(int _DevNum, AnsiString _file_name,
		bool _ReadOnly = false);
	__fastcall ~A1730(void);
	void SetPeriod(int _period);

	CSignal* iPCHA; // �� �
	CSignal* iCC; // ���� ����������
	CSignal* iPCHRUN; // �� RUN
	CSignal* iSTROBE; // ����� ����
	CSignal* iCYCLE; // ����
	CSignal* iREADY; // ����������
	CSignal* iCONTROL; // ��������

	CSignal* oPCHPOW; // ������� ��
	CSignal* oSCANPOW; // ������� ��
	CSignal* oSOLPOW; // ������� ��������
	CSignal* oWORK; // ������
	CSignal* oMEAS; // ���������
	CSignal* oSHIFT; // ����������
	CSignal* oSTROBE; // �����
	CSignal* oRESULT; // ���������

	inline int SignalCount(void)
	{
		return (M.Count());
	}

	inline CSignal* GetSignal(int _index)
	{
		return (M[_index]);
	}
	void AlarmCycleOn(bool _v);
	void SetOnFront(OnFrontDef _OnFront);
	AnsiString GetReason(void);
	void Latchterminate(void);
	void SetAlarm(bool _IsAlarm);
	void pr(AnsiString _msg);
};
#endif
