//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include "IniFiles.hpp"


//---------------------------------------------------------------------------
class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *ValueListEditor;
	TStatusBar *StatusBarBottom;
	TComboBox *cbTypeSize;
	TButton *bCreateTypeSize;
	TButton *bDeleteTypeSize;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ValueListKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall ValueListEditorSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall bCreateTypeSizeClick(TObject *Sender);
	void __fastcall bDeleteTypeSizeClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations
	short SelRow;					// выделенный р€д
	short SelCol;					// выделенный столбец
	TIniFile *ini;					// ini файл с настройками типоразмеров
	TStringList *names;				// имена настроек, которые необходимо и писать в ini файл
	void UpdateComboBox();			// функци€, обновл€юща€ список типоразмеров в ComboBox'e
public:		// User declarations

	__fastcall TSettingsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
