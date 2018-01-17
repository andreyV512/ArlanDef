// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

// using Microsoft::ManagementConsole::Advanced;

#include "Manage.h"
#include "Global.h"
#include "Solenoid.h"
#include "Inverter.h"
#include "SignalListDef.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageForm *ManageForm;
// ---------------------------------------------------------------------------
__fastcall TManageForm::TManageForm(TComponent* Owner)
	: TForm(Owner)
{
	// ������ ����� ��� ������ � ���������, �������� � �����
	ManageForm->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::FormShow(TObject *Sender)
{
	// ������ ����� �� �����
	TIniFile *ini=new TIniFile(Globals::IniFileName);

	on=ini->ReadInteger("Color","ActiveIn",0);
	off=ini->ReadInteger("Color","NotActiveIn",0);
	move=ini->ReadInteger("Color","NotActiveOut",0);

	delete ini;

	eFriquencyRot->Text=IntToStr(frConverter->getParameterSpeed(Globals::defaultRotParameter));

	bRotation->Caption="�������� ��������";
	// �������� ������, ������������� ��������� ������ � �������
	butt_enabled=true;
	this->StatusBarBottom->Panels->Items[0]->Text="";
	this->StatusBarBottom->Refresh();
	Timer->Enabled=true;
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormClose(TObject *Sender,TCloseAction &Action)
{
	// ��������� ������, ������������� ��������� ������ � �������
	Timer->Enabled=false;
	// ���������� �������, ���� ������������ ����� �� ����
	SLD->oCSOLPOW->Set(false);
	SLD->oLSOLPOW->Set(false);
	bLinearSolenoid->Caption="�������� ��������� ����";
	bCrossSolenoid->Caption="�������� ��������� ����";
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormKeyPress(TObject *Sender,wchar_t &Key)
{
	if(Key==27)
		ManageForm->Close();
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::bLinearSolenoidClick(TObject *Sender)
{
	if(!SLD->oLSOLPOW->Get())
	{
		SLD->oLSOLPOW->Set(true);
		bLinearSolenoid->Caption="��������� ��������� ����";
	}
	else if(SLD->oLSOLPOW->Get())
	{
		SLD->oLSOLPOW->Set(false);
		bLinearSolenoid->Caption="�������� ��������� ����";
	}
	StatusBarBottom->Panels->Items[0]->Text="";
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bCrossSolenoidClick(TObject *Sender)
{
	if(!SLD->oCSOLPOW->Get())
	{
		bCrossSolenoid->Caption="��������� ��������� ����";
		SLD->oCSOLPOW->Set(true);
	}
	else if(SLD->oCSOLPOW->Get())
	{
		bCrossSolenoid->Caption="�������� ��������� ����";
		SLD->oCSOLPOW->Set(false);
	}
	StatusBarBottom->Panels->Items[0]->Text="";
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bRotationClick(TObject *Sender)
{
	if(SLD->iLPCHRUN->Get())
	{
		if(!frConverter->stopRotation())
		{
			StatusBarBottom->Panels->Items[0]->Text="�� ������� ���������� ��������";
			StatusBarBottom->Refresh();
		}
		else
		{
			bRotation->Caption="�������� ��������";
			StatusBarBottom->Panels->Items[0]->Text="���������� ��������";
			StatusBarBottom->Refresh();
		}
	}
	else
	{
		if(!frConverter->setParameterSpeed(Globals::defaultRotParameter,StrToInt(eFriquencyRot->Text)))
		{
			StatusBarBottom->Panels->Items[0]->Text="�� ������� ��������� ��������";
			StatusBarBottom->Refresh();
			return;
		}

		if(!frConverter->startRotation())
			StatusBarBottom->Panels->Items[0]->Text="�� ������� �������� ��������";
		else
		{
			StatusBarBottom->Panels->Items[0]->Text="�������� ��������";
			bRotation->Caption="��������� ��������";
		}

		StatusBarBottom->Refresh();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::TimerTimer(TObject *Sender)
{
	if(SLD->oCSOLPOW->Get() && !CrossSolenoid->OkResist())
	{
		SLD->oCSOLPOW->Set(false);
		bCrossSolenoid->Caption="�������� ��������� ����";
		StatusBarBottom->Panels->Items[0]->Text="��������� ����������� ���������";
		StatusBarBottom->Refresh();
	}
	if(SLD->oLSOLPOW->Get() && !LinearSolenoid->OkResist())
	{
		SLD->oLSOLPOW->Set(false);
		bLinearSolenoid->Caption="�������� ��������� ����";
		StatusBarBottom->Panels->Items[0]->Text="��������� ����������� ���������";
		StatusBarBottom->Refresh();
	}
	if(SLD->oLSOLPOW->Get())
	{
//		TPr::pr("������ ������ �����������");
		eLinearAmperage->Text=FloatToStrF(LinearSolenoid->getAmperage(),ffNumber,8,1);
		eLinearVoltage->Text=FloatToStrF(LinearSolenoid->getVoltage(),ffNumber,8,0);
		eLinearTemperature->Text=FloatToStrF(LinearSolenoid->getResist(),ffNumber,8,0);
	}
	else
	{
		eLinearAmperage->Text=" ";
		eLinearVoltage->Text=" ";
		eLinearTemperature->Text=" ";
	}
	if(SLD->oCSOLPOW->Get())
	{
		eCrossAmperage->Text=FloatToStrF(CrossSolenoid->getAmperage(),ffNumber,8,1);
		eCrossVoltage->Text=FloatToStrF(CrossSolenoid->getVoltage(),ffNumber,8,0);
		eCrossTemperature->Text=FloatToStrF(CrossSolenoid->getResist(),ffNumber,8,0);
	}
	else
	{
		eCrossAmperage->Text=" ";
		eCrossVoltage->Text=" ";
		eCrossTemperature->Text=" ";
	}
}
// ---------------------------------------------------------------------------

