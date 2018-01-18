#pragma hdrstop
#include "SignalList.h"
#include<IniFiles.hpp>
#include "Protocol.h"
#include <Dialogs.hpp>
#pragma package(smart_init)
SignalList::SignalList(TIniFile* _ini)
{
	cs=new TCriticalSection();
	period=_ini->ReadInteger("OtherSettings","SignalListPeriod",200);
	AnsiString fname=_ini->ReadString("OtherSettings","SignalListFile","..\\..\\Settings\\IO_Management.ini");
	TIniFile *ini=new TIniFile(fname);
	TStringList *sections=new TStringList();
	TStringList *keys=new TStringList();
	ini->ReadSections(sections);
	if(sections->Count==0)
	{
		AnsiString a="SignalList::SignalList: файл ";
		a+=fname;
		a+=" либо не существует, либо пуст";
		throw(Exception(a));
	}
	for(int i=0;i<sections->Count;i++)
	{
		AnsiString Sect=sections->Strings[i];
		bool in=Sect.SubString(8,3)=="IN_";
		if(Sect.SubString(1,5)=="ACard")
		{
			ini->ReadSection(Sect,keys);
			for(int j=0;j<keys->Count;j++)
			{
				AnsiString Key=keys->Strings[j];
				AnsiString MapKey=ini->ReadString(Sect,Key,"");
				S.Add(new CSignal(MapKey,in,keys->Strings[j].ToInt(),WriteSignals,cs,Wait));
				AnsiString a=MapKey;
				a+="[";
				a+=keys->Strings[j];
				a+="] ";
				if(in)
					a+="IN";
				else
					a+="OUT";
				TPr::pr(a);
			}
		}
	}
	delete keys;
	delete sections;
	delete ini;
	dev=new A1730(_ini->ReadInteger("OtherSettings","SignalListDevice",0));
#ifndef _DEBUG
	if(!dev->IsOk())
		throw(Exception("SignalList::SignalList: не смогли открыть плату Advantech1730"));
#else
	MessageDlg("¬ключЄн макрос _DEBUG", mtError, TMsgDlgButtons() << mbOK,NULL);
#endif
	IsAlarm=false;
	wasAlarm=false;
	AlarmList=new TStringList();
}
__fastcall SignalList::~SignalList(void)
{
	delete dev;
	delete cs;
	delete AlarmList;
}
void SignalList::FlushSignals(DWORD _buf,DWORD _tick)
{
	for(int i=0;i<S.Count();i++)
	{
		CSignal* p=S[i];
		if(!p->in)
			continue;
		bool value=(_buf&(((DWORD)1)<<p->index))!=0;
		p->value_prev=p->value;
		if(value!=p->value)
		{
			p->value=value;
			p->last_changed=_tick;
/*
			AnsiString a=p->name;
			if(p->value)
				a+="=true";
			else
				a+="=false";
			TPr::pr(a);
*/
		}
	}
}
void _fastcall SignalList::Execute()
{
	while(true)
	{
		if(Terminated)
			break;
		cs->Enter();
		{
			DWORD v=dev->Read();
			DWORD tick=GetTickCount();
			FlushSignals(v,tick);
			if(IsAlarm)
			{
				if(!wasAlarm)
				{
					if(CheckAlarm())
						LatchesTerminate0();
				}
			}
			CheckFront(tick);
			CheckWait(tick);
		} cs->Leave();
		Sleep(period);
		if(Terminated)
			break;
	}
}
bool SignalList::Wait(bool _value,CSignal* _signal,DWORD _tm)
{
	TEvent* event=new TEvent(true);
	Latch* lp=new Latch(_value,_signal,event);
	cs->Enter();
	{
		L.Add(lp);
	} cs->Leave();
	bool ret=event->WaitFor(_tm)==wrSignaled;
	delete event;
	cs->Enter();
	{
		if(lp->terminate)
			ret=false;
		L.Remove(lp);
	} cs->Leave();
	return (ret);
}
void SignalList::WriteSignals(void)
{
	DWORD buf=dev->ReadOut();
	CSignal* p;
	for(int i=0;i<S.Count();i++)
	{
		p=S[i];
		if(p->in)
			continue;
		if(p->value)
			buf|=((DWORD)1)<<p->index;
		else
			buf&=~(((DWORD)1)<<p->index);
	}
	dev->Write(buf);
}
CSignal* SignalList::Find(AnsiString _name,bool _in)
{
	for(int i=0;i<S.Count();i++)
	{
		if(S[i]->name==_name&&S[i]->in==_in)
			return (S[i]);
	}
	AnsiString a="Signals::Find: сигнал ";
	a+=_name;
	a+=" не найден";
	throw(Exception(a));
	return (NULL);
}
void SignalList::SetPeriod(int _period)
{
	cs->Enter();
	{
		period=_period;
	} cs->Leave();
}
bool SignalList::CheckAlarm(void)
{
	return (false);
}
void SignalList::LatchesTerminate0(void)
{
	for(int i=0;i<L.Count();i++)
	{
		Latch* lp=L[i];
		lp->terminate=true;
		lp->event->SetEvent();
	}
}
Front* SignalList::SetFront(CSignal* _signal,bool _value,OnFrontDef _OnFront)
{
	Front* front;
	cs->Enter();
	{
		front=F.Add(new Front(_signal,_value,_OnFront));
	} cs->Leave();
	return (front);
}
void SignalList::ClearFront(Front* _front)
{
	cs->Enter();
	{
		F.Remove(_front);
	} cs->Leave();
}
void SignalList::ClearFronts(void)
{
	cs->Enter();
	{
		F.Clear();
	} cs->Leave();
}
void SignalList::CheckFront(DWORD _tick)
{
	for(int i=0;i<F.Count();i++)
	{
		Front* f=F[i];
		if(f->signal->value==f->value&&f->signal->value_prev!=f->value)
			f->OnFront(_tick);
	}
}
void SignalList::CheckWait(DWORD _tick)
{
	Latch* lp;
	for(int i=0;i<L.Count();i++)
	{
		Latch* lp=L[i];
		if(lp->signal->value==lp->value)
			lp->event->SetEvent();
	}
}
void SignalList::LatchesTerminate(void)
{
	cs->Enter();
	{
		LatchesTerminate0();
	} cs->Leave();
}
void SignalList::SetAlarm(bool _on)
{
	cs->Enter();
	{
		IsAlarm=_on;
		wasAlarm=false;
		AlarmList->Clear();
	} cs->Leave();
}
bool SignalList::WasAlarm(void)
{
	bool ret;
	cs->Enter();
	{
		ret=wasAlarm;
	} cs->Leave();
	return (ret);
}
void SignalList::GetAlarmList(TStringList* _alarmList)
{
	cs->Enter();
	{
		_alarmList->Clear();
		for(int i=0;i<AlarmList->Count;i++)
			_alarmList->Add(AlarmList->Strings[i]);
	} cs->Leave();
}
void SignalList::prAlarm(void)
{
	for(int i=0;i<AlarmList->Count;i++)
		pr(AlarmList->Strings[i]);
}
void SignalList::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}

