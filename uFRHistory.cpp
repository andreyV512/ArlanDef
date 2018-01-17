// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFRHistory.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uFRTubeHistory"
#pragma resource "*.dfm"
TFRHistory *FRHistory;

// ---------------------------------------------------------------------------
__fastcall TFRHistory::TFRHistory(TComponent* Owner) : TFrame(Owner)
{
}
// ---------------------------------------------------------------------------

void __fastcall TFRHistory::FrameResize(TObject *Sender)
{
	int space = 3;
	FRTubeHistory1->Top = 0;
	FRTubeHistory1->Height = ClientHeight;
	FRTubeHistory2->Top = 0;
	FRTubeHistory2->Height = ClientHeight;
	FRTubeHistory3->Top = 0;
	FRTubeHistory3->Height = ClientHeight;
	FRTubeHistory1->Width = (ClientWidth - space * 2) / 3;
	FRTubeHistory2->Width = FRTubeHistory1->Width;
	FRTubeHistory3->Width = FRTubeHistory1->Width;

	FRTubeHistory1->Left = 0;
	FRTubeHistory2->Left = FRTubeHistory1->Left + FRTubeHistory1->Width + space;
	FRTubeHistory3->Left = FRTubeHistory2->Left + FRTubeHistory2->Width + space;
}

// ---------------------------------------------------------------------------
void TFRHistory::Add(AnsiString _TubeNumber, AnsiString _SolidGroup,
	TColor _SolidGroupColor, AnsiString _Result, TColor _ResultColor,
	AnsiString _cut1, AnsiString _cut2)
{
	FRTubeHistory3->CopyFrom(FRTubeHistory2);
	FRTubeHistory2->CopyFrom(FRTubeHistory1);
	FRTubeHistory1->Set(_TubeNumber, _SolidGroup, _SolidGroupColor, _Result,
		_ResultColor, _cut1, _cut2);
}

void TFRHistory::Clear()
{
	FRTubeHistory1->Clear();
	FRTubeHistory2->Clear();
	FRTubeHistory3->Clear();
}
