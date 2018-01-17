// ---------------------------------------------------------------------------

#pragma hdrstop

#include "FSignalsState.h"
#include "Global.h"
#include "uFRSigPanel.h"
#include "uFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFSignalsStateF *FSignalsStateF;

// ---------------------------------------------------------------------------
__fastcall TFSignalsStateF::TFSignalsStateF(TComponent* Owner,
	TIniFile* _ini) : TForm(Owner)
{
	ini = _ini;
}
// ---------------------------------------------------------------------------

void __fastcall TFSignalsStateF::FormShow(TObject *Sender)
{
	InOutTimer->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFSignalsStateF::InOutTimerTimer(TObject *Sender)
{
	TTimer* t = (TTimer*)Sender;
	t->Enabled = false;
	CheckSignals();
	t->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFSignalsStateF::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	InOutTimer->Enabled = false;
}
// ---------------------------------------------------------------------------

void TFSignalsStateF::CheckSignals(void)
{
	for (int i = 0; i < ControlCount; i++)
	{
		TControl* p = Controls[i];
		if (p->ClassName() != "TFRSigPanel")
			continue;
		TFRSigPanel* pp = (TFRSigPanel*)p;
		pp->CheckSignal();
	}
}

void __fastcall TFSignalsStateF::FormHide(TObject *Sender)
{
	InOutTimer->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFSignalsStateF::FormCreate(TObject *Sender)
{
	LoadFormPos(this, ini);
	int space = 5;

	int ltop0 = space;
	int lwidth;
	for (int i = 0; i < a1730->SignalCount(); i++)
	{
		CSignal* sig = a1730->GetSignal(i);
		if (!sig->GetIn())
			continue;
		TFRSigPanel *p = new TFRSigPanel(this, sig);
		p->Parent = this;
		AnsiString a = "In";
		a += i;
		p->Name = a;
		p->Top = ltop0;
		p->Left = space;
		lwidth = p->Width;
		ltop0 += p->Height + space;
	}
	int ltop1 = space;
	for (int i = 0; i < a1730->SignalCount(); i++)
	{
		CSignal* sig = a1730->GetSignal(i);
		if (sig->GetIn())
			continue;
		TFRSigPanel *p = new TFRSigPanel(this, sig);
		p->Parent = this;
		AnsiString a = "Out";
		a += i;
		p->Name = a;
		p->Top = ltop1;
		p->Left = space + lwidth + space;
		ltop1 += p->Height + space;
	}
	Width = space + lwidth + space + lwidth + space + (Width - ClientWidth);
	if (ltop0 < ltop1)
		ltop0 = ltop1;
	Height = ltop0 + (Height - ClientHeight);

}
// ---------------------------------------------------------------------------

void __fastcall TFSignalsStateF::FormDestroy(TObject *Sender)
{
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------
