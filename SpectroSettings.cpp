//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Global.h"
#include "SpectroSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSpectroSettingForm *SpectroSettingForm;
//---------------------------------------------------------------------------
__fastcall TSpectroSettingForm::TSpectroSettingForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TSpectroSettingForm::scanSerialPorts()
{
    cbxSerialPort->Items->Clear();

	//Both of these cases indicate that there is a COM port at that number.
	for (unsigned int i = 1; i < 25; i++)
	{
		String TestS = String().sprintf( L"\\\\.\\COM%d", i );

		//Try to open the port
		bool bSuccess = false;
		HANDLE hPort = ::CreateFile( TestS.c_str(),
									 GENERIC_READ | GENERIC_WRITE,
									 0,
									 0, OPEN_EXISTING, 0, 0);

		if (hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();

			//Check to see if the error was because some other app had the port open
			if (dwError == ERROR_ACCESS_DENIED)
				bSuccess = TRUE;
		}
		else
		{
			//The port was opened successfully
			bSuccess = TRUE;

			//Don't forget to close the port, since we are going to do nothing with it anyway
			CloseHandle(hPort);
		}

		//Add the port number to the array which will be returned
		if (bSuccess)
			cbxSerialPort->Items->Add( L"COM" + String(i) );
	}
}
void __fastcall TSpectroSettingForm::FormCreate(TObject *Sender)
{
	scanSerialPorts();
}
//---------------------------------------------------------------------------


