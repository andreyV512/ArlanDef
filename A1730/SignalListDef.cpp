#pragma hdrstop
#include "SignalListDef.h"
#pragma package(smart_init)
SignalListDef* SLD=NULL;
SignalListDef::SignalListDef(TIniFile* _ini) : SignalList(_ini)
{
	iCC=Find("���� ����������",true);
	iLPCHA=Find("���� �� �",true);
	iLPCHRUN=Find("���� �� RUN",true);
	iSGTUBE=Find("�� �����",true);
	iCSTROBE=Find("����� �����",true);
	iLSTROBE=Find("���� �����",true);
	iCCYCLE=Find("����� ����",true);
	iLCYCLE=Find("���� ����",true);
	iREADY=Find("����������",true);
	iCCONTROL=Find("����� ��������",true);
	iLCONTROL=Find("���� ��������",true);
	iSGON=Find("�� �������",true);

	oLPCHPOW=Find("���� ��� ��",false);
	oLSCANPOW=Find("���� ��� ��",false);
	oLSOLPOW=Find("�������� ����",false);
	oCSOLPOW=Find("�������� �����",false);
	oCWORK=Find("����� ������",false);
	oLWORK=Find("���� ������",false);
	oCMEAS=Find("����� ���������",false);
	oLMEAS=Find("���� ���������",false);
	oSHIFT=Find("����������",false);
	oCSTROBE=Find("����� �����",false);
	oLSTROBE=Find("���� �����",false);
	oCRESULT=Find("����� ���������",false);
	oLRESULT=Find("���� ���������",false);
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
		AlarmList->Add("�� ������� ������ ������ ���������");
		wasAlarm=true;
	}
	if(iCC->WasConst(false,50))
	{
		AlarmList->Add("��� ������� ���� ����������");
		wasAlarm=true;
	}
	if(LinearCycle)
	{
		if(iLCYCLE->WasConst(false,50))
		{
			AlarmList->Add("������ ������ ���������� ����");
			wasAlarm=true;
		}
	}
	if(CrossCycle)
	{
		if(iCCYCLE->WasConst(false,50))
		{
			AlarmList->Add("������ ������ ���������� ����");
			wasAlarm=true;
		}
	}
	if(InvA)
	{
		if(iLPCHA->WasConst(false,50))
		{
			AlarmList->Add("������: ������ ����� ���� �� �");
			wasAlarm=true;
		}
	}
	if(wasAlarm)
		prAlarm();
	return(wasAlarm);
}


