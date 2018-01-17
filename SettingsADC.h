//---------------------------------------------------------------------------

#ifndef SettingsADCH
#define SettingsADCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include "IniFiles.hpp"
#include <ActnCtrls.hpp>
#include <ToolWin.hpp>
#include "cspin.h"
#include <AppEvnts.hpp>
#include "Main.h"
//---------------------------------------------------------------------------
class TFormSettADC : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *LSizeBuf;
	TEdit *EditSizeBuf;
	TLabel *LRate;
	TEdit *EditRate;
	TLabel *Label13;
	TEdit *EditKadr;
	TButton *bSave;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TComboBox *ComboBox3;
	TComboBox *ComboBox4;
	TComboBox *ComboBox5;
	TComboBox *ComboBox6;
	TComboBox *ComboBox7;
	TComboBox *ComboBox8;
	TComboBox *ComboBox9;
	TComboBox *ComboBox10;
	TComboBox *ComboBox11;
	TComboBox *ComboBox12;
	TCSpinEdit *CSpinEdit1;
	TCSpinEdit *CSpinEdit2;
	TCSpinEdit *CSpinEdit3;
	TCSpinEdit *CSpinEdit4;
	TCSpinEdit *CSpinEdit5;
	TCSpinEdit *CSpinEdit6;
	TCSpinEdit *CSpinEdit7;
	TCSpinEdit *CSpinEdit8;
	TCSpinEdit *CSpinEdit9;
	TCSpinEdit *CSpinEdit10;
	TCSpinEdit *CSpinEdit11;
	TCSpinEdit *CSpinEdit12;
	TGroupBox *GroupBox2;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TComboBox *ComboBox13;
	TComboBox *ComboBox14;
	TComboBox *ComboBox15;
	TComboBox *ComboBox16;
	TCSpinEdit *CSpinEdit13;
	TCSpinEdit *CSpinEdit14;
	TCSpinEdit *CSpinEdit15;
	TCSpinEdit *CSpinEdit16;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TButton *Button1;
	TButton *Button2;
	TApplicationEvents *ApplicationEvents1;
	TButton *Button3;
	TTabSheet *TabSheet3;
	TLabel *LabelTemp1;
	TLabel *LabelTemp2;
	TLabel *NumberCh;
	TCSpinEdit *SpinTemp1;
	TCSpinEdit *SpinTemp2;
	TLabel *LabelMagnetic;
	TCSpinEdit *SpinMagnetic;
	TButton *Button4;
	TButton *BExit;
	TLabel *LabelKatphot1;
	TLabel *LabelKataphot2;
	TLabel *LabelSG;
	TLabel *LabelMag;
	TCSpinEdit *SpinKataphot1;
	TCSpinEdit *SpinKataphot2;
	TCSpinEdit *SpinSG;
	TCSpinEdit *SpinCurr;
	TLabel *Label18;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall EditSizeBufKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);



private:	// User declarations

void __fastcall WriteNum(TCSpinEdit* CS,UnicodeString Type);
void __fastcall ReadNum(TCSpinEdit*  CS,UnicodeString Type);
void __fastcall Write(TComboBox* t,	    UnicodeString Type);
void __fastcall Read(TComboBox* t,      UnicodeString Type);

public:		// User declarations
	__fastcall TFormSettADC(TComponent* Owner);
		TIniFile *Ini;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSettADC *FormSettADC;
//---------------------------------------------------------------------------
#endif
