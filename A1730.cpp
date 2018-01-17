// ---------------------------------------------------------------------------
#pragma hdrstop
#include "A1730.h"
#include <Forms.hpp>
#include "Protocol1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

A1730* A1730::Create(int _DevNum, AnsiString _file_name, bool _ReadOnly)
{
	A1730* ret = new A1730(_DevNum, _file_name, _ReadOnly);
	if (ret->device == NULL)
	{
		delete ret;
		return (NULL);
	}
	return (ret);
}

A1730::A1730(int _DevNum, AnsiString _file_name, bool _ReadOnly)
	: TThread(false)
{
	period = 50;
	startPort = 0;
	portCount = 4;
	cs = new TCriticalSection();
	ErrorCode errorCode;
	AccessMode AM = (_ReadOnly) ? (ModeRead) : (ModeWrite);
	device = NULL;
	errorCode = BDaqDevice::Open(_DevNum, AM, device);
	if (BioFailed(errorCode))
	{
		if (device != NULL)
			device->Close();
		device = NULL;
	}
	else if (errorCode == Success)
	{
		errorCode = device->GetModule(0, dio); // Get Dio Module
		if (errorCode == Success && ReadOnly)
		{
			if (device != NULL)
				device->Close();
			device = NULL;
		}
	}
	TIniFile *ini = new TIniFile(_file_name);
	TStringList *sections = new TStringList();
	TStringList *keys = new TStringList();
	ini->ReadSections(sections);
	if (sections->Count == 0)
	{
		AnsiString a = "Signals::Signals: файл ";
		a += _file_name;
		a += " либо не существует, либо пуст";
		throw(Exception(a));
	}
	for (int i = 0; i < sections->Count; i++)
	{
		AnsiString Sect = sections->Strings[i];
		bool in = Sect.SubString(8, 3) == "IN_";
		if (Sect.SubString(1, 5) == "ACard")
		{
			ini->ReadSection(Sect, keys);
			for (int j = 0; j < keys->Count; j++)
			{
				AnsiString Key = keys->Strings[j];
				AnsiString MapKey = ini->ReadString(Sect, Key, "");
				M.Add(new CSignal(MapKey, in, keys->Strings[j].ToInt(),
					WriteSignals, cs, Wait));
			}
		}
	}
	delete keys;
	delete sections;
	delete ini;
	iPCHA = Find("ПЧ А");
	iCC = Find("Цепи управления");
	iPCHRUN = Find("ПЧ RUN");
	iSTROBE = Find("Строб Зоны");
	iCYCLE = Find("Цикл");
	iREADY = Find("Готовность");
	iCONTROL = Find("Контроль");

	oPCHPOW = Find("Питание ПЧ");
	oSCANPOW = Find("Питание СУ");
	oSOLPOW = Find("Питание соленоид");
	oWORK = Find("Работа");
	oMEAS = Find("Измерение");
	oSHIFT = Find("Перекладка");
	oSTROBE = Find("Строб");
	oRESULT = Find("Результат");
	alarmCycleOn = false;
	reason = "";
	OnFront = NULL;
	IsAlarm = false;
}

__fastcall A1730::~A1730(void)
{
	TThread::Terminate();
	TThread::WaitFor();
	if (device != NULL)
		device->Close();
	delete cs;
}

void _fastcall A1730::Execute()
{
	while (true)
	{
		if (Terminated)
			break;
		ReadSignals();
		Sleep(period);
		if (Terminated)
			break;
	}
}

DWORD A1730::Read(void)
{
	BYTE buf[4];
	ErrorCode errorcode = dio->DiRead(startPort, portCount, buf);
	if (dio->DiRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730::ReadIn: не могу прочитать плату"));
	DWORD v = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (v);
}

DWORD A1730::ReadOut(void)
{
	BYTE buf[4];
	if (dio->DoRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730::ReadOut: не могу прочитать плату"));
	DWORD bufOut = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (bufOut);
}

void A1730::Write(DWORD _v)
{
	BYTE buf[4];
	buf[0] = _v & 0xFF;
	buf[1] = _v >> 8 & 0xFF;
	buf[2] = _v >> 16 & 0xFF;
	buf[3] = _v >> 24 & 0xFF;
	if (dio->DoWrite(startPort, portCount, buf) != Success)
		throw(Exception("A1730::Write: не могу записать на плату"));
}

void A1730::SetPeriod(int _period)
{
	cs->Enter();
	period = _period;
	cs->Leave();
}

CSignal* A1730::Find(AnsiString _name)
{
	for (int i = 0; i < M.Count(); i++)
	{
		if (M[i]->name == _name)
			return (M[i]);
	}
	AnsiString a = "Signals::Find: сигнал ";
	a += _name;
	a += " не найден";
	throw(Exception(a));
	return (NULL);
}

void A1730::ReadSignals(void)
{
	cs->Enter();
	{
		DWORD buf = Read();
		CSignal* p;
		bool value;
		DWORD tick = GetTickCount();
		for (int i = 0; i < M.Count(); i++)
		{
			p = M[i];
			if (!p->in)
				continue;
			value = (buf & (((DWORD)1) << p->index)) != 0;
			p->value_prev = p->value;
			if (value != p->value)
			{
				p->value = value;
				p->last_changed = tick;
			}
		}
		Alarm();
		SendFront(tick);
		Latch* lp;
		for (int i = 0; i < L.Count(); i++)
		{
			Latch* lp = L[i];
			if (lp->signal->value == lp->value)
				lp->signal->event->SetEvent();
		}
	} cs->Leave();
}

void A1730::WriteSignals(void)
{
	cs->Enter();
	{
		DWORD buf = ReadOut();
		CSignal* p;
		for (int i = 0; i < M.Count(); i++)
		{
			p = M[i];
			if (p->in)
				continue;
			if (p->value)
				buf |= ((DWORD)1) << p->index;
			else
				buf &= ~(((DWORD)1) << p->index);
		}
		Write(buf);
	} cs->Leave();
}

bool A1730::Wait(bool _value, CSignal* _signal, DWORD _tm, TThread* _thread)
{
	Latch* lp;
	cs->Enter();
	{
		lp = L.Add(new Latch(_value, _signal, _thread));
	} cs->Leave();
	bool ret = _signal->event->WaitFor(_tm) == wrSignaled;
	cs->Enter();
	{
		_signal->event->ResetEvent();
		if (lp->terminate)
			ret = false;
		L.Remove(lp);
	} cs->Leave();
	return (ret);
}

void A1730::Alarm(void)
{
	if (!IsAlarm)
		return;
	reason = "";
	if (iPCHA->WasConst0(false, 50))
	{
		reason = "Ошибка частотника! Пропал сигнал ПЧ А";
		pr(reason);
		Latchterminate0();
		IsAlarm=false;
		return;
	}
	if (iCC->WasConst0(false, 50))
	{
		reason = "Нет сигнала цепи управления!";
		pr(reason);
		Latchterminate0();
		IsAlarm=false;
		return;
	}
	if (alarmCycleOn)
	{
		if (!iCYCLE->value)
		{
			reason = "Пропал сигнал Цикл";
			pr(reason);
			Latchterminate0();
			IsAlarm=false;
			return;
		}
	}
	// Цепи управления», «Работа», «ПЧ А», отсутствие сигнала «ПЧ RUN», воздух, ПЧ В
}

void A1730::AlarmCycleOn(bool _v)
{
	cs->Enter();
	{
		alarmCycleOn = _v;
	} cs->Leave();
}

AnsiString A1730::GetReason(void)
{
	AnsiString ret;
	cs->Enter();
	{
		ret = reason;
		ret.Unique();
	} cs->Leave();
	return (ret);
}

void A1730::SendFront(unsigned int _tick)
{
	if (OnFront == NULL)
		return;
	if (iSTROBE->value == true && iSTROBE->value_prev == false)
		OnFront(_tick);
}

void A1730::SetOnFront(OnFrontDef _OnFront)
{
	cs->Enter();
	{
		OnFront = _OnFront;
	} cs->Leave();
}

void A1730::Latchterminate(void)
{
	cs->Enter();
	{
		Latchterminate0();
	} cs->Leave();
}

void A1730::Latchterminate0(void)
{
	for (int i = 0; i < L.Count(); i++)
	{
		Latch* lp = L[i];
		lp->terminate = true;
		lp->signal->event->SetEvent();
	}
}

void A1730::SetAlarm(bool _IsAlarm)
{
	cs->Enter();
	{
		IsAlarm = _IsAlarm;
	} cs->Leave();
}

void A1730::pr(AnsiString _msg)
{
	ProtocolForm->pr(_msg);

}
