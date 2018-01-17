//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFRSigPanel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRSigPanel *FRSigPanel;
//---------------------------------------------------------------------------
__fastcall TFRSigPanel::TFRSigPanel(TComponent* Owner,CSignal* _signal)
	: TFrame(Owner)
{
	signal=_signal;
	AnsiString a="  ";
	a+=signal->GetIndex();
	a+=" ";
	a+=signal->GetName();
	Panel1->Caption=a;
}
//---------------------------------------------------------------------------
void TFRSigPanel::CheckSignal(void)
{
	if(signal->IsInput())
	{
		if(signal->Get())
			Panel1->Color=clGreen;
		else
			Panel1->Color=clBtnFace;
	}
	else
	{
		if(signal->Get())
			Panel1->Color=clRed;
		else
			Panel1->Color=clBtnFace;
	}
//	Panel1->Refresh();
//	Panel1->Repaint();
}
