//---------------------------------------------------------------------------

#ifndef SpectroSettingsH
#define SpectroSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
class TSpectroSettingForm : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOK;
	TButton *btnCancel;
	TGroupBox *gbSerialPort;
	TComboBox *cbxParity;
	TComboBox *cbxStopBits;
	TComboBox *cbxBaudrate;
	TComboBox *cbxSerialPort;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall FormCreate(TObject *Sender);
private:
	void __fastcall scanSerialPorts();
public:		// User declarations
	__fastcall TSpectroSettingForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSpectroSettingForm *SpectroSettingForm;
//---------------------------------------------------------------------------
#endif
