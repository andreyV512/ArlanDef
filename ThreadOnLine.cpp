// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ThreadOnLine.h"
#include "Math.h"
#include "Solenoid.h"
#include "Correction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ----конструктор - переносим внешние переменные на внутренние---------------
__fastcall ThreadOnLine::ThreadOnLine(bool CreateSuspended, bool _Linear,
	// используем ли продольный)
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

// -----запуск потока работы--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
	NameThreadForDebugging("WorkOnlineThread");
	stext1 = "Режим \"Работа (Д)\"";
	stext2 = "Готовим к исходному положению";
	TPr::SendToProtocol("-----------");
	TPr::SendToProtocol(stext1);
	Synchronize(UpdateStatus);
	SLD->oSHIFT->Set(true);
	if (Spectro && spectroscope)
	{
		TPr::SendToProtocol("Запускаем сбор данных от спектроскопа...");
		spectroscope->Start();
		while (!spectroscope->isDataReady())
		{
			Sleep(10); // Если спектроскоп стоит первым блоком
		}
		TPr::SendToProtocol("Закончен сбор данных от спектроскопа...");
		spectroscope->Stop();
	}
	UnicodeString prepare_result = PrepareForWork();
	if (prepare_result != "ok")
	{
		stext1 = "Режим \"Работа (Д)\" не завершен!";
		stext2 = prepare_result;
		TPr::SendToProtocol("Работа (Д): " + stext2);
		Synchronize(UpdateStatus);

		Finally();
		cool = false;
		return;
	}

	TPr::SendToProtocol("Работа (Д): Подготовка прошла успешно");
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
// -----подготовка к работе от самого начала до движения трубы----------------
UnicodeString ThreadOnLine::PrepareForWork()
{
	// проверяем, включен ли модуль размагничивания и группы прочности
	if (SystemConst::isSolidity)
	{
		if (!SLD->iSGON->WasConst(true, 100))
			return "Включите модуль размагничивания!";
	}

	SLD->oLPCHPOW->Set(true);
	// ждем цех цикл
	stext2 = "Ждем сигнала \"Попер Цикл\"";
	Synchronize(UpdateStatus);

	if (!SLD->iCCYCLE->Wait(true, INFINITE))
		return "Не дождались сигнала Попер Цикл!";
	SLD->SetAlarm(true);
	SLD->SetCrossCycle(true);
	if (Linear)
	{
		stext2 = "Ждем сигнала \"Прод Цикл\"";
		Synchronize(UpdateStatus);

		if (!SLD->iLCYCLE->Wait(true, INFINITE))
			return "Не дождались сигнала Прод Цикл!";
		SLD->SetLinearCycle(true);
	}
	SLD->oSHIFT->Set(true);

	stext2 = "Ждем сигнал  Готовность";
	TPr::SendToProtocol("Работа (Д): " + stext2);
	Synchronize(UpdateStatus);

	if (!SLD->iREADY->Wait(true, INFINITE))
		return "Не дождались сигнала Готовность!";

	SLD->oSHIFT->Set(false);
	SLD->oCSTROBE->Set(false);
	SLD->oLSTROBE->Set(false);
	SLD->oLRESULT->Set(false);
	SLD->oCRESULT->Set(false);

	Synchronize(NextTube);
	if (!LastSynchronizeReturnCode)
		return "Не получили новую трубу (из АСУ)!";

	if (!Globals::tube_transit)
	{
		SLD->oCSOLPOW->Set(true);
		if (Linear)
		{
			TThread::CurrentThread->Sleep(600);
			SLD->oLSOLPOW->Set(true);
		}
		Sleep(500);

		AnsiString a = "Соленоид поперечный: ";
		a += CrossSolenoid->GetUIR();
		TPr::pr(a);
		if (!CrossSolenoid->OkU())
		{
			SLD->oCSOLPOW->Set(false);
			return "Напряжение поперечного соленоида вне диапозона";
		}
		if (!CrossSolenoid->OkResist())
		{
			SLD->oCSOLPOW->Set(false);
			return "Сопротивление поперечного соленоида превысило норму";
		}
		if (Linear)
		{
			a = "Соленоид продольный: ";
			a += LinearSolenoid->GetUIR();
			TPr::pr(a);
			if (!LinearSolenoid->OkU())
			{
				SLD->oLSOLPOW->Set(false);
				return "Напряжение продольного соленоида вне диапозона";
			}
			if (!LinearSolenoid->OkResist())
			{
				SLD->oLSOLPOW->Set(false);
				return "Сопротивление продольного соленоида превысило норму";
			}
		}
		// крутим продольный
		if (Linear)
		{
			SLD->oLSCANPOW->Set(true);
			// Выставим скорость работы на входе в модуль
			TPr::SendToProtocol(AnsiString("Скорость входящая InSpeed ")+AnsiString(InSpeed));
			if (!frConverter->setParameterSpeed(Globals::defaultRotParameter,
				InSpeed))
				return "Не смогли задать входную скорость вращения продольного модуля";

			if (!frConverter->startRotation())
				return "Не удалось включить вращение продольного модуля";
			if (!SLD->iLPCHRUN->Wait(false, 5000))
				return "Не достигнута скорость вращения продольного модуля!";
			SLD->SetInvA(true);
		}
		// посмотрим, сколько датчиков используется
		TPr::SendToProtocol("Работа (Д): типоразмер " +
			IntToStr(Globals::current_diameter));
		Singleton::Instance()->CrossResult->sensors =
			(Globals::current_diameter == 60) ? (10) : (12);
		Singleton::Instance()->LinearResult->sensors = Globals::LinSensors;

		if (lcard->setSettingsFromIniFile())
			return "Не удалось установить настройки платы L502";
		// Выставляем сигналы РАБОТА по модулям
		SLD->oCWORK->Set(true);
		if (Linear)
			SLD->oLWORK->Set(true);

		stext2 = "Ждем трубу в модулях";
		TPr::SendToProtocol("Работа (Д): " + stext2);
		Synchronize(UpdateStatus);
	}
	else
	{
		SLD->oCWORK->Set(true);
		if (Linear)
			SLD->oLWORK->Set(true);
		stext2 = "Ждем прохода трубы в транзите";
		TPr::SendToProtocol("Работа (Д): " + stext2);
		Synchronize(UpdateStatus);
	}
	return "ok";
}

// -------------------------------------------------------------------------------
// ----онлайн цикл, крутящийся бесконечно и проверяющий все события---------------
bool ThreadOnLine::OnlineCycle()
{
	TPr::SendToProtocol("Работа (Д): Режим работа");
	// флаги событий во время кругового цикла
	bool InSG = false; // труба зашла в размагничивание
	bool result = true; // результат всего цикла

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
	// Переменные для выставления сигнала на 50мс
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
			// Начало поперечного сбора
			if (!ppStarted && SLD->iCCONTROL->Get())
			{
				ppStarted = true;
				lcard->Start();
				lcard->CrossIsStarted();
				SLD->oCMEAS->Set(true);
				stext2 = "Начали сбор поперечного";
				TPr::pr(stext2);
				Synchronize(UpdateStatus);
			}
			// Обсчитываем поперечный модуль
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
				TPr::pr(String("поперечн ") + String(testCross));
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
				stext2 = "Остановили сбор с поперечного";
				TPr::pr(stext2);
				Synchronize(UpdateStatus);
			}
		} // -----------------------------------------------------------------------------------------------------------------
		if (Linear && !prIsStoped)
		{
			CycleTick = GetTickCount();
			// Начало продольного сбора
			if (!prStarted && SLD->iLCONTROL->Get())
			{
				prStarted = true;
				FirstLinerStrobeTick = CycleTick;
				SLD->oLMEAS->Set(true);
				lcard->LinearIsStarted();
				stext2 = "Начали сбор с продольного";
				Synchronize(UpdateStatus);
				TPr::pr(stext2);
			}
			if (prStarted && SLD->iLSTROBE->Get())

				// Обсчитываем продольную плату
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
				TPr::pr(String("продоль ") + String(testLong));
////test
			}
			if (!isTwoSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseStop))
			{
				isTwoSpeed = true;
				TPr::pr(String("Скорость торможения OutSpeed ") + String(OutSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, OutSpeed))
					ErrFinally("Авария: Не удалось включить торможение",
					&Collect, &result);
			}

			if (!isFreeSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseWorkSpeed))
			{
				isFreeSpeed = true;
				TPr::pr(String("Скорость в работе WorkSpeed ") + String(WorkSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, WorkSpeed))
					ErrFinally("Авария: Не удалось включить рабочую скорость",
					&Collect, &result);
			}
			if (!isOutSpeed && prStarted && !SLD->iLCONTROL->Get())
			{
				isOutSpeed = true;
				ControlOffTime = CycleTick;
				TPr::pr("Скорость разгона в конце струбы InSpeed " + String(InSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, InSpeed))
					ErrFinally("Авария: Не удалось включить конечную скорость",
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
					ErrFinally("Авария: Не удалось выключить вращение",
					&Collect, &result);
				else
				{
					stext2 = "Остановили сбор с продольного";
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
				stext2 = "Включили сбор ГП";
				Synchronize(UpdateStatus);
			}
		}
		// -----------------------------------------------------------------------------------------------------------------
		// смотрим, что труба вышла из установки
		if (SLD->iSGTUBE->WasConst(false,
			50) && ppStarted && !SLD->iCCONTROL->Get())
		{
			stext2 = "Труба вышла из установки";
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
		// смотрим, не было ли сброса
		if (Terminated)
		{
			Collect = false;
			result = false;
			TPr::SendToProtocol("Работа (Д): Зашли в Terminated");
			Finally();
		}
		// смотрим, не было ли аварии
		if (SLD->WasAlarm())
			ErrFinally("Работа (Д): Выход по аварии", &Collect, &result);
	}
	TPr::pr("Execute завершен");
	return result;
}

bool ThreadOnLine::OnlineCycleTransit()
{
	TPr::SendToProtocol("Работа (Д): Режим транзит");
	bool result = true;
	// результат всего цикла
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
			stext2 = "Труба вышла из установки";
			Synchronize(UpdateStatus);
			Collect = false;
			result = false;
			TPr::SendToProtocol(stext2);
		}
		// -----------------------------------------------------------------------------------------------------------------
		// смотрим, не было ли сброса
		if (SLD->WasAlarm())
		{
			Finally();
			Collect = false;
			result = false;
			TPr::pr("Работа (Д): Выход по аварии");
		}
		if (Terminated)
		{
			Finally();
			Collect = false;
			result = false;
			TPr::pr("Работа (Д): Зашли в Terminated");
		}
		Sleep(delay);
	}
	TPr::pr("Execute завершен");
	return result;
}

void ThreadOnLine::Finally()
{
	TPr::pr("Finally() снятие сигналов");
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
	// Расчет и вывод данных ПП
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
	// Расчет и вывод данных ПрП
	CurrentZoneLinearResult = Singleton::Instance()->LinearResult->AddZone
		(prData, true);
	// Singleton::Instance()->LinearResult->ComputeZoneData(true);
	Singleton::Instance()->LinearResult->PutResultOnChart
		(MainForm->LinearDefectChart, MainForm);

	// расчет данных и вывод суммарного результата
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
