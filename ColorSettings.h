//---------------------------------------------------------------------------

#ifndef ColorSettingsH
#define ColorSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IniFiles.hpp"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormColor : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *ColorBrackAnalog;
	TLabel *LabelBrack;
	TLabel *LabelValid;
	TLabel *LabelSecondClass;
	TColorDialog *ColorDialog;
	TPanel *PanelValid;
	TPanel *PanelBrack;
	TPanel *PanelSecondClass;
	TGroupBox *GroupBoxDigital;
	TLabel *LabelInActive;
	TLabel *LabelNotInActiv;
	TLabel *LabelActiveOut;
	TPanel *NotInActive;
	TPanel *ActiveIn;
	TPanel *ActiveOut;
	TLabel *LabelNotActiveOut;
	TPanel *NotActiveOut;
	TButton *BSave;
	TButton *BClose;
	TButton *BDefault;
	TLabel *LabelSelectZone;
	TPanel *pSelectZone;
	TLabel *lDeadZone;
	TPanel *pDeadZone;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TPanel *sgD;
	TLabel *Label2;
	TPanel *sgK;
	TLabel *Label3;
	TPanel *sgE;
	void __fastcall PanelValidClick(TObject *Sender);
	void __fastcall BDefaultClick(TObject *Sender);
	void __fastcall BCloseClick(TObject *Sender);
	void __fastcall BSaveClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
	TIniFile *ini;
public:		// User declarations

	TColor SolidGroup_D;
	TColor SolidGroup_E;
	TColor SolidGroup_K;

	__fastcall TFormColor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormColor *FormColor;
//---------------------------------------------------------------------------
#endif
