//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


//---------------------------------------------------------------------------
USEFORM("ReportsView.cpp", FormReport);
USEFORM("Settings.cpp", SettingsForm);
USEFORM("Protocol.cpp", Pr);
USEFORM("SolidGroup.cpp", SolidGroupForm);
USEFORM("uFRTubeHistory.cpp", FRTubeHistory); /* TFrame: File Type */
USEFORM("uFRHistory.cpp", FRHistory); /* TFrame: File Type */
USEFORM("uFASMTest.cpp", FASMTest);
USEFORM("ViewEtalons.cpp", FormViewEtalons);
USEFORM("View.cpp", ViewForm);
USEFORM("SolidGroupTest.cpp", SGForm);
USEFORM("SolidGroupGraphics.cpp", SGGraphForm);
USEFORM("SpectroSettings.cpp", SpectroSettingForm);
USEFORM("A1730\uFSignalsState.cpp", FSignalsState);
USEFORM("A1730\uFRSigPanel.cpp", FRSigPanel); /* TFrame: File Type */
USEFORM("ADCSettings.cpp", ADCSettForm);
USEFORM("ABOUT_NTC_NK_URAN.cpp", AboutBox1);
USEFORM("ColorSettings.cpp", FormColor);
USEFORM("GraphicsCenters.cpp", GraphicCentersForm);
USEFORM("Manage.cpp", ManageForm);
USEFORM("Main.cpp", MainForm);
USEFORM("DataModule.cpp", DM); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

	try
	{
        // Пытаемся открыть мьютекс.
		HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
        if(!hMutex)
          // Мьютекса не существует. То есть,
		  // это первый экземпляр,
          // создаем мьютекс.
		  hMutex = CreateMutex(0, 0, L"MyApp1.0");
		else
		{
		  // Мьютекс существует , то есть , запущен
		  // второй экземпляр, говорим юзеру что он олень и закрываем прогу.
		  Application->MessageBoxW(L"Приложение уже запущено!!!",L"Ошибка",MB_OK|MB_ICONERROR);
		  return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		Application->CreateForm(__classid(TDM), &DM);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TViewForm), &ViewForm);
		Application->CreateForm(__classid(TSettingsForm), &SettingsForm);
		Application->CreateForm(__classid(TManageForm), &ManageForm);
		Application->CreateForm(__classid(TFormViewEtalons), &FormViewEtalons);
		Application->CreateForm(__classid(TFormColor), &FormColor);
		Application->CreateForm(__classid(TFormReport), &FormReport);
		Application->CreateForm(__classid(TADCSettForm), &ADCSettForm);
		Application->CreateForm(__classid(TSGForm), &SGForm);
		Application->CreateForm(__classid(TGraphicCentersForm), &GraphicCentersForm);
		Application->CreateForm(__classid(TAboutBox1), &AboutBox1);
		Application->CreateForm(__classid(TSGGraphForm), &SGGraphForm);
		Application->CreateForm(__classid(TSolidGroupForm), &SolidGroupForm);
		Application->Run();

		// Приложение закрывается ,
		// освобождаем мьютекс.
		ReleaseMutex(hMutex);
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
