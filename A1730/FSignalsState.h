//---------------------------------------------------------------------------

#ifndef FSignalsStateH
#define FSignalsStateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <vcl.h>


#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFSignalsStateF : public TForm
{
__published:	// IDE-managed Components
	TTimer *InOutTimer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall InOutTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	void CheckSignals(void);
	TIniFile *ini;
public:		// User declarations
	__fastcall TFSignalsStateF(TComponent* Owner,TIniFile* _ini);

};
//---------------------------------------------------------------------------
extern PACKAGE TFSignalsStateF *FSignalsStateF;
//---------------------------------------------------------------------------
#endif

