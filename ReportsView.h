//---------------------------------------------------------------------------

#ifndef ReportsViewH
#define ReportsViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "Reports.h"
//---------------------------------------------------------------------------
class Reports;
class TFormReport : public TForm
{
__published:	// IDE-managed Components
	TPanel *AllTube;
	TGroupBox *GroupDate;
	TDateTimePicker *DateBegin;
	TLabel *LabelBeginDate;
	TLabel *LabelEndDate;
	TDateTimePicker *DateEnd;
	TButton *BCreateReport;
	TButton *BCancel;
	TCheckBox *CheckBoxResult;
	TListBox *ListBoxResult;
	TListBox *ListBoxSG;
	TCheckBox *CheckBoxSG;
	TCheckBox *CheckBoxTS;
	TListBox *ListBoxTS;
	void __fastcall CheckBoxResultClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall CheckBoxSGClick(TObject *Sender);
	void __fastcall BCancelClick(TObject *Sender);
	void __fastcall BCreateReportClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall CheckBoxTSClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

	Reports *Rp;

public:		// User declarations
	__fastcall TFormReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReport *FormReport;
//---------------------------------------------------------------------------
#endif
