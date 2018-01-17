//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFASMTest.h"
#include "ComPort.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFASMTest *FASMTest;
//---------------------------------------------------------------------------
__fastcall TFASMTest::TFASMTest(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFASMTest::FormCreate(TObject *Sender)
{
//	myComPort=new MyComPort("ASMTest");
	MyCom->OnStatus=Status;
//	myComPort->OnStatus=Status;
}
//---------------------------------------------------------------------------
void __fastcall TFASMTest::FormDestroy(TObject *Sender)
{
//	delete myComPort;
}
//---------------------------------------------------------------------------
void TFASMTest::Status(AnsiString _msg)
{
	StatusBar1->SimpleText=_msg;
}

void __fastcall TFASMTest::Button1Click(TObject *Sender)
{
	TButton* b=(TButton*)Sender;
	b->Enabled=false;
//	myComPort->TestConnection();
	MyCom->TestConnection();
	b->Enabled=true;
}
//---------------------------------------------------------------------------



