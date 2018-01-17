//---------------------------------------------------------------------------

#ifndef uFASMTestH
#define uFASMTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ComPort.h"
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFASMTest : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TButton *Button1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
//	MyComPort* myComPort;
	void Status(AnsiString _msg);
public:		// User declarations
	__fastcall TFASMTest(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFASMTest *FASMTest;
//---------------------------------------------------------------------------
#endif
