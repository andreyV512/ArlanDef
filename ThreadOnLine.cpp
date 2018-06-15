// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ThreadOnLine.h"
#include "Math.h"
#include "Solenoid.h"
#include "Correction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ----����������� - ��������� ������� ���������� �� ����������---------------
__fastcall ThreadOnLine::ThreadOnLine(bool CreateSuspended, bool _Linear,
	// ���������� �� ����������)
	bool _Spectro, Spectroscope *_Spectroscope)

{
	Solenoid_ok = true;
	Collect = true;
	Cross = true;
	Linear = _Linear;
	Spectro = _Spectro;
	spectroscope = _Spectroscope;
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	InSpeed = ini->ReadInteger("Type_" + Globals::current_typesize,
		"InSpeed", 20);
	WorkSpeed=ini->ReadInteger("Type_" + Globals::current_typesize,
		"WorkSpeed", 35);
	OutSpeed=ini->ReadInteger("Type_" + Globals::current_typesize,
		"OutSpeed", 20);
	delay = ini->ReadInteger("OtherSettings", "OnLineCycleDelay", 10);
	pauseWorkSpeed = ini->ReadInteger("Type_" + Globals::current_typesize,
		"PauseWorkSpeed", 1050);
	pauseStop = ini->ReadInteger("Type_" + Globals::current_typesize,
		"PauseStop", 500);
	delete ini;
}

// -----������ ������ ������--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
	NameThreadForDebugging("WorkOnlineThread");
	stext1 = "����� \"������ (�)\"";
	stext2 = "������� � ��������� ���������";
	TPr::SendToProtocol("-----------");
	TPr::SendToProtocol(stext1);
	Synchronize(UpdateStatus);
	SLD->oSHIFT->Set(true);
	if (Spectro && spectroscope)
	{
		TPr::SendToProtocol("��������� ���� ������ �� ������������...");
		spectroscope->Start();
		while (!spectroscope->isDataReady())
		{
			Sleep(10); // ���� ����������� ����� ������ ������
		}
		TPr::SendToProtocol("�������� ���� ������ �� ������������...");
		spectroscope->Stop();
	}
	UnicodeString prepare_result = PrepareForWork();
	if (prepare_result != "ok")
	{
		stext1 = "����� \"������ (�)\" �� ��������!";
		stext2 = prepare_result;
		TPr::SendToProtocol("������ (�): " + stext2);
		Synchronize(UpdateStatus);

		Finally();
		cool = false;
		return;
	}

	TPr::SendToProtocol("������ (�): ���������� ������ �������");
	if (!Globals::tube_transit)
	{
		SetReturnValue(OnlineCycle());
		cool = ReturnValue;
		Finally();
	}
	else
	{
		SetReturnValue(OnlineCycleTransit());
		cool = ReturnValue;
		Finally();
	}
	return;
}

// ---------------------------------------------------------------------------
// -----���������� � ������ �� ������ ������ �� �������� �����----------------
UnicodeString ThreadOnLine::PrepareForWork()
{
	// ���������, ������� �� ������ ��������������� � ������ ���������
	if (SystemConst::isSolidity)
	{
		if (!SLD->iSGON->WasConst(true, 100))
			return "�������� ������ ���������������!";
	}

	SLD->oLPCHPOW->Set(true);
	// ���� ��� ����
	stext2 = "���� ������� \"����� ����\"";
	Synchronize(UpdateStatus);

	if (!SLD->iCCYCLE->Wait(true, INFINITE))
		return "�� ��������� ������� ����� ����!";
	SLD->SetAlarm(true);
	SLD->SetCrossCycle(true);
	if (Linear)
	{
		stext2 = "���� ������� \"���� ����\"";
		Synchronize(UpdateStatus);

		if (!SLD->iLCYCLE->Wait(true, INFINITE))
			return "�� ��������� ������� ���� ����!";
		SLD->SetLinearCycle(true);
	}
	SLD->oSHIFT->Set(true);

	stext2 = "���� ������  ����������";
	TPr::SendToProtocol("������ (�): " + stext2);
	Synchronize(UpdateStatus);

	if (!SLD->iREADY->Wait(true, INFINITE))
		return "�� ��������� ������� ����������!";

	SLD->oSHIFT->Set(false);
	SLD->oCSTROBE->Set(false);
	SLD->oLSTROBE->Set(false);
	SLD->oLRESULT->Set(false);
	SLD->oCRESULT->Set(false);

	Synchronize(NextTube);
	if (!LastSynchronizeReturnCode)
		return "�� �������� ����� ����� (�� ���)!";

	if (!Globals::tube_transit)
	{
		SLD->oCSOLPOW->Set(true);
		if (Linear)
		{
			TThread::CurrentThread->Sleep(600);
			SLD->oLSOLPOW->Set(true);
		}
		Sleep(500);

		AnsiString a = "�������� ����������: ";
		a += CrossSolenoid->GetUIR();
		TPr::pr(a);
		if (!CrossSolenoid->OkU())
		{
			SLD->oCSOLPOW->Set(false);
			return "���������� ����������� ��������� ��� ���������";
		}
		if (!CrossSolenoid->OkResist())
		{
			SLD->oCSOLPOW->Set(false);
			return "������������� ����������� ��������� ��������� �����";
		}
		if (Linear)
		{
			a = "�������� ����������: ";
			a += LinearSolenoid->GetUIR();
			TPr::pr(a);
			if (!LinearSolenoid->OkU())
			{
				SLD->oLSOLPOW->Set(false);
				return "���������� ����������� ��������� ��� ���������";
			}
			if (!LinearSolenoid->OkResist())
			{
				SLD->oLSOLPOW->Set(false);
				return "������������� ����������� ��������� ��������� �����";
			}
		}
		// ������ ����������
		if (Linear)
		{
			SLD->oLSCANPOW->Set(true);
			// �������� �������� ������ �� ����� � ������
			TPr::SendToProtocol(AnsiString("�������� �������� InSpeed ")+AnsiString(InSpeed));
			if (!frConverter->setParameterSpeed(Globals::defaultRotParameter,
				InSpeed))
				return "�� ������ ������ ������� �������� �������� ����������� ������";

			if (!frConverter->startRotation())
				return "�� ������� �������� �������� ����������� ������";
			if (!SLD->iLPCHRUN->Wait(false, 5000))
				return "�� ���������� �������� �������� ����������� ������!";
			SLD->SetInvA(true);
		}
		// ���������, ������� �������� ������������
		TPr::SendToProtocol("������ (�): ���������� " +
			IntToStr(Globals::current_diameter));
		Singleton::Instance()->CrossResult->sensors =
			(Globals::current_diameter == 60) ? (10) : (12);
		Singleton::Instance()->LinearResult->sensors = Globals::LinSensors;

		if (lcard->setSettingsFromIniFile())
			return "�� ������� ���������� ��������� ����� L502";
		// ���������� ������� ������ �� �������
		SLD->oCWORK->Set(true);
		if (Linear)
			SLD->oLWORK->Set(true);

		stext2 = "���� ����� � �������";
		TPr::SendToProtocol("������ (�): " + stext2);
		Synchronize(UpdateStatus);
	}
	else
	{
		SLD->oCWORK->Set(true);
		if (Linear)
			SLD->oLWORK->Set(true);
		stext2 = "���� ������� ����� � ��������";
		TPr::SendToProtocol("������ (�): " + stext2);
		Synchronize(UpdateStatus);
	}
	return "ok";
}

// -------------------------------------------------------------------------------
// ----������ ����, ���������� ���������� � ����������� ��� �������---------------
bool ThreadOnLine::OnlineCycle()
{
	TPr::SendToProtocol("������ (�): ����� ������");
	// ����� ������� �� ����� ��������� �����
	bool InSG = false; // ����� ����� � ���������������
	bool result = true; // ��������� ����� �����

	bool ppStarted = false;
	bool ppSignaled = false;
	bool ppIsStoped = false;

	bool prStarted = false;
	bool prSignaled = false;
	bool prIsStoped = false;

	bool sgStarted = false;
	CollectedMeasurements_pp = 0;
	CollectedMeasurements_pr = 0;
	bool TubeQuit = false;
	CurrentZoneCrossResult = false;
	CurrentZoneLinearResult = false;
	// ���������� ��� ����������� ������� �� 50��
	DWORD LinearResult;
	DWORD CrossResult;
	DWORD FirstLinerStrobeTick = 0;
	DWORD CycleTick = 0;
	bool isTwoSpeed = false;
	bool isFreeSpeed = false;
	bool isOutSpeed = false;
	DWORD ControlOffTime = 0;
	int testCross = 0;
	int testLong = 0;
	while (Collect)
	{
		Sleep(delay);
		if (Cross && !ppIsStoped)
		{
			// -----------------------------------------------------------------------------------------------------------------
			// ������ ����������� �����
			if (!ppStarted && SLD->iCCONTROL->Get())
			{
				ppStarted = true;
				lcard->Start();
				lcard->CrossIsStarted();
				SLD->oCMEAS->Set(true);
				stext2 = "������ ���� �����������";
				TPr::pr(stext2);
				Synchronize(UpdateStatus);
			}
			// ����������� ���������� ������
			if (!ppSignaled && ppStarted && SLD->iCSTROBE->Get())
			{
				ppSignaled = true;
				ppData = lcard->getCrossMeasure(CollectedMeasurements_pp);
				CollectedMeasurements_pp =
					lcard->getCountCollectedMeasurements();
				Synchronize(DrawResultPP);
				CrossResult = GetTickCount();
				if (CurrentZoneCrossResult)
					SLD->oCRESULT->Set(true);
				SLD->oCSTROBE->Set(true);
			}
			if (ppSignaled && !SLD->iCSTROBE->Get())
				ppSignaled = false;

			if ((SLD->oCSTROBE->Get() || SLD->oCRESULT->Get()) &&
				((GetTickCount() - CrossResult) > 50))
			{
				SLD->oCSTROBE->Set(false);
				SLD->oCRESULT->Set(false);
////test
  ++testCross;
				TPr::pr(String("�������� ") + String(testCross));
////test
			}
			if (ppStarted && !SLD->iCCONTROL->Get())
			{
				ppIsStoped = true;
				ppData = lcard->getCrossMeasure(CollectedMeasurements_pp);
				CollectedMeasurements_pp =
					lcard->getCountCollectedMeasurements();
				Synchronize(DrawResultPP);
				CrossResult = GetTickCount();
				if (CurrentZoneCrossResult)
					SLD->oCRESULT->Set(true);
				SLD->oCSTROBE->Set(true);
				SLD->SetCrossCycle(false);
				lcard->CrossIsStopped();
				stext2 = "���������� ���� � �����������";
				TPr::pr(stext2);
				Synchronize(UpdateStatus);
			}
		} // -----------------------------------------------------------------------------------------------------------------
		if (Linear && !prIsStoped)
		{
			CycleTick = GetTickCount();
			// ������ ����������� �����
			if (!prStarted && SLD->iLCONTROL->Get())
			{
				prStarted = true;
				FirstLinerStrobeTick = CycleTick;
				SLD->oLMEAS->Set(true);
				lcard->LinearIsStarted();
				stext2 = "������ ���� � �����������";
				Synchronize(UpdateStatus);
				TPr::pr(stext2);
			}
			if (prStarted && SLD->iLSTROBE->Get())

				// ����������� ���������� �����
				if (!prSignaled && prStarted && SLD->iLSTROBE->Get())
				{
					prSignaled = true;
					prData = lcard->getLinearMeasure(CollectedMeasurements_pr);
					Synchronize(DrawResultPR);
					CollectedMeasurements_pr =
						lcard->getCountCollectedMeasurements();
					LinearResult = GetTickCount();
					if (CurrentZoneLinearResult)
						SLD->oLRESULT->Set(true);
					SLD->oLSTROBE->Set(true);
				}
			if (prSignaled && !SLD->iLSTROBE->Get())
				prSignaled = false;
			if ((SLD->oLSTROBE->Get() || SLD->oLRESULT->Get()) &&
				((GetTickCount() - LinearResult) > 50))
			{
				SLD->oLSTROBE->Set(false);
				SLD->oLRESULT->Set(false);
				////test
				++testLong;
				TPr::pr(String("������� ") + String(testLong));
////test
			}
			if (!isTwoSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseStop))
			{
				isTwoSpeed = true;
				TPr::pr(String("�������� ���������� OutSpeed ") + String(OutSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, OutSpeed))
					ErrFinally("������: �� ������� �������� ����������",
					&Collect, &result);
			}

			if (!isFreeSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseWorkSpeed))
			{
				isFreeSpeed = true;
				TPr::pr(String("�������� � ������ WorkSpeed ") + String(WorkSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, WorkSpeed))
					ErrFinally("������: �� ������� �������� ������� ��������",
					&Collect, &result);
			}
			if (!isOutSpeed && prStarted && !SLD->iLCONTROL->Get())
			{
				isOutSpeed = true;
				ControlOffTime = CycleTick;
				TPr::pr("�������� ������� � ����� ������ InSpeed " + String(InSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, InSpeed))
					ErrFinally("������: �� ������� �������� �������� ��������",
					&Collect, &result);
			}
			if (isOutSpeed && ((CycleTick - ControlOffTime) > 1000))
			{
				prIsStoped = true;
				prData = lcard->getLinearMeasure(CollectedMeasurements_pr);
				Synchronize(DrawResultPR);
				CollectedMeasurements_pr =
					lcard->getCountCollectedMeasurements();
				LinearResult = GetTickCount();
				if (CurrentZoneLinearResult)
					SLD->oLRESULT->Set(true);
				SLD->oLSTROBE->Set(true);
				SLD->SetLinearCycle(false);
				SLD->SetInvA(false);
				lcard->LinearIsStopped();
				if (!frConverter->stopRotation())
					ErrFinally("������: �� ������� ��������� ��������",
					&Collect, &result);
				else
				{
					stext2 = "���������� ���� � �����������";
					TPr::pr(stext2);
					Synchronize(UpdateStatus);
				}
			}
		}
		// -----------------------------------------------------------------------------------------------------------------
		if (!sgStarted)
		{
			if (SLD->iSGTUBE->WasConst(true, 50) && SLD->oCMEAS->Get())
			{
				sgStarted = true;
				lcard->SolidGroupIsStarted();
				stext2 = "�������� ���� ��";
				Synchronize(UpdateStatus);
			}
		}
		// -----------------------------------------------------------------------------------------------------------------
		// �������, ��� ����� ����� �� ���������
		if (SLD->iSGTUBE->WasConst(false,
			50) && ppStarted && !SLD->iCCONTROL->Get())
		{
			stext2 = "����� ����� �� ���������";
			Synchronize(UpdateStatus);
			SLD->oCSOLPOW->Set(false);
			SLD->oLSOLPOW->Set(false);
			SLD->oLSCANPOW->Set(false);

			if (SLD->iLPCHRUN->Get())
				SLD->oLPCHPOW->Set(false);

			Singleton::Instance()->CrossResult->CalculateDeadBack(200);
			Singleton::Instance()->LinearResult->CalculateDeadBack(200);
			Singleton::Instance()->SumResult->ComputeZonesData();
			Synchronize(RedrawGraphics);

			Finally();
			Collect = false;
			TPr::SendToProtocol(stext2);
		}
		// -----------------------------------------------------------------------------------------------------------------
		// �������, �� ���� �� ������
		if (Terminated)
		{
			Collect = false;
			result = false;
			TPr::SendToProtocol("������ (�): ����� � Terminated");
			Finally();
		}
		// �������, �� ���� �� ������
		if (SLD->WasAlarm())
			ErrFinally("������ (�): ����� �� ������", &Collect, &result);
	}
	TPr::pr("Execute ��������");
	return result;
}

bool ThreadOnLine::OnlineCycleTransit()
{
	TPr::SendToProtocol("������ (�): ����� �������");
	bool result = true;
	// ��������� ����� �����
	bool inUnut = false;
	while (Collect)
	{
		if (!inUnut && SLD->iSGTUBE->WasConst(true, 1000))
		{
			inUnut = true;
		}
		if (inUnut && !SLD->iSGTUBE->WasConst(true, 1000))
		{
			Finally();
			stext2 = "����� ����� �� ���������";
			Synchronize(UpdateStatus);
			Collect = false;
			result = false;
			TPr::SendToProtocol(stext2);
		}
		// -----------------------------------------------------------------------------------------------------------------
		// �������, �� ���� �� ������
		if (SLD->WasAlarm())
		{
			Finally();
			Collect = false;
			result = false;
			TPr::pr("������ (�): ����� �� ������");
		}
		if (Terminated)
		{
			Finally();
			Collect = false;
			result = false;
			TPr::pr("������ (�): ����� � Terminated");
		}
		Sleep(delay);
	}
	TPr::pr("Execute ��������");
	return result;
}

void ThreadOnLine::Finally()
{
	TPr::pr("Finally() ������ ��������");
	lcard->Stop();
	SLD->SetCrossCycle(false);
	SLD->SetLinearCycle(false);
	SLD->oLWORK->Set(false);
	SLD->oLMEAS->Set(false);
	SLD->oCWORK->Set(false);
	SLD->oCMEAS->Set(false);
	SLD->oLSOLPOW->Set(false);
	SLD->oCSOLPOW->Set(false);
	SLD->oLSCANPOW->Set(false);
	SLD->oSHIFT->Set(false);
}

void ThreadOnLine::ErrFinally(AnsiString _msg, bool* _Collect, bool* _result)
{
	SLD->oLPCHPOW->Set(false);
	Finally();
	*_Collect = false;
	*_result = false;
	TPr::SendToProtocol(_msg);
	stext2 = _msg;
	Synchronize(UpdateStatus);
}

// ---------------------------------------------------------------------------
void __fastcall ThreadOnLine::UpdateStatus()
{
	MainForm->UpdateStatus(stext1, stext2);
}

// -----------------------------------------------------------------------------
void __fastcall ThreadOnLine::NextTube(void)
{
	MainForm->NextTube();
}

// -----------------------------------------------------------------------------
void __fastcall ThreadOnLine::DrawResultPP()
{
	// ������ � ����� ������ ��
	CurrentZoneCrossResult = Singleton::Instance()->CrossResult->AddZone
		(ppData);
	// Singleton::Instance()->CrossResult->ComputeZoneData();
	Singleton::Instance()->CrossResult->PutResultOnChart
		(MainForm->CrossDefectChart, MainForm);

	if (Linear)
	{
		Singleton::Instance()->SumResult->AddZone();
		Singleton::Instance()->SumResult->ComputeZonesData();
		Singleton::Instance()->SumResult->PutResultOnChart
			(MainForm->SummaryChart, MainForm);
	}
}

// ---------------------------------------------------------------------------
void __fastcall ThreadOnLine::DrawResultPR()
{
	// ������ � ����� ������ ���
	CurrentZoneLinearResult = Singleton::Instance()->LinearResult->AddZone
		(prData, true);
	// Singleton::Instance()->LinearResult->ComputeZoneData(true);
	Singleton::Instance()->LinearResult->PutResultOnChart
		(MainForm->LinearDefectChart, MainForm);

	// ������ ������ � ����� ���������� ����������
	if (Linear)
	{
		Singleton::Instance()->SumResult->AddZone();
		Singleton::Instance()->SumResult->ComputeZonesData();
		Singleton::Instance()->SumResult->PutResultOnChart
			(MainForm->SummaryChart, MainForm);
	}
}

// ---------------------------------------------------------------------------
void __fastcall ThreadOnLine::RedrawGraphics()
{
	Singleton::Instance()->CrossResult->PutResultOnChart
		(MainForm->CrossDefectChart, MainForm);
	Singleton::Instance()->LinearResult->PutResultOnChart
		(MainForm->LinearDefectChart, MainForm);
	Singleton::Instance()->ThResult->PutResultOnChart(MainForm->ThicknessChart,
		MainForm);
	Singleton::Instance()->SumResult->PutResultOnChart(MainForm->SummaryChart,
		MainForm);
}
// ---------------------------------------------------------------------------
