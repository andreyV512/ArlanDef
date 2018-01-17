#pragma hdrstop
#include "SignalListDef.h"
#pragma package(smart_init)
SignalListDef* SLD=NULL;
SignalListDef::SignalListDef(TIniFile* _ini) : SignalList(_ini)
{
	iCC=Find("Цепи Управления",true);
	iLPCHA=Find("Прод ПЧ А",true);
	iLPCHRUN=Find("Прод ПЧ RUN",true);
	iSGTUBE=Find("ГП Труба",true);
	iCSTROBE=Find("Попер строб",true);
	iLSTROBE=Find("Прод строб",true);
	iCCYCLE=Find("Попер цикл",true);
	iLCYCLE=Find("Прод цикл",true);
	iREADY=Find("Готовность",true);
	iCCONTROL=Find("попер контроль",true);
	iLCONTROL=Find("прод контроль",true);
	iSGON=Find("ГП Включен",true);

	oLPCHPOW=Find("Прод Пит ПЧ",false);
	oLSCANPOW=Find("Прод Пит СУ",false);
	oLSOLPOW=Find("Соленоид Прод",false);
	oCSOLPOW=Find("Соленоид Попер",false);
	oCWORK=Find("Попер Работа",false);
	oLWORK=Find("Прод Работа",false);
	oCMEAS=Find("Попер измерение",false);
	oLMEAS=Find("Прод измерение",false);
	oSHIFT=Find("Перекладка",false);
	oCSTROBE=Find("Попер строб",false);
	oLSTROBE=Find("Прод строб",false);
	oCRESULT=Find("Попер результат",false);
	oLRESULT=Find("Прод результат",false);
	CrossCycle=false;
	LinearCycle=false;
	InvA=false;
}
void SignalListDef::SetCrossCycle(bool _v)
{
	cs->Enter();
	{
		CrossCycle=_v;
	} cs->Leave();
}
void SignalListDef::SetLinearCycle(bool _v)
{
	cs->Enter();
	{
		LinearCycle=_v;
	} cs->Leave();
}
void SignalListDef::SetInvA(bool _v)
{
	cs->Enter();
	{
		InvA=_v;
	} cs->Leave();
}
bool SignalListDef::CheckAlarm(void)
{
	if(iSGON->WasConst(false,50))
	{
		AlarmList->Add("Не включен модуль группы прочности");
		wasAlarm=true;
	}
	if(iCC->WasConst(false,50))
	{
		AlarmList->Add("Нет сигнала цепи управления");
		wasAlarm=true;
	}
	if(LinearCycle)
	{
		if(iLCYCLE->WasConst(false,50))
		{
			AlarmList->Add("Пропал сигнал Продольный Цикл");
			wasAlarm=true;
		}
	}
	if(CrossCycle)
	{
		if(iCCYCLE->WasConst(false,50))
		{
			AlarmList->Add("Пропал сигнал Поперечный цикл");
			wasAlarm=true;
		}
	}
	if(InvA)
	{
		if(iLPCHA->WasConst(false,50))
		{
			AlarmList->Add("Авария: пропал синал Прод ПЧ А");
			wasAlarm=true;
		}
	}
	if(wasAlarm)
		prAlarm();
	return(wasAlarm);
}


