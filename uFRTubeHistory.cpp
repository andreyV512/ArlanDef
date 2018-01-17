// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFRTubeHistory.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRTubeHistory *FRTubeHistory;

// ---------------------------------------------------------------------------
__fastcall TFRTubeHistory::TFRTubeHistory(TComponent* Owner) : TFrame(Owner) {
	Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TFRTubeHistory::FrameResize(TObject *Sender) {
	int space = 2;

	lNN->Top = space;
	lNN->Height = Height - space * 2;
	pNN->Top = lNN->Top;
	pNN->Height = lNN->Height;
	pSolidGroup->Top = lNN->Top;
	pSolidGroup->Height = lNN->Height;
	lCut1->Top = lNN->Top;
	lCut1->Height = lNN->Height;
	lCut2->Top = lNN->Top;
	lCut1->Height = lNN->Height;
	lResult->Top = lNN->Top;
	lResult->Height = lNN->Height;

	lNN->Left = space+2;
	pNN->Left = lNN->Left + lNN->Width + space;
	pSolidGroup->Left = pNN->Left + pNN->Width + space;
	lCut1->Left = pSolidGroup->Left + pSolidGroup->Width + space;
	lCut2->Left = Width - lCut2->Width - space;
	lResult->Left = lCut1->Left + lCut1->Width + space;
	lResult->Width = lCut2->Left - lResult->Left - space;
}

// ---------------------------------------------------------------------------
void TFRTubeHistory::Set(AnsiString _TubeNumber,AnsiString _SolidGroup, TColor _SolidGroupColor,
	AnsiString _Result, TColor _ResultColor, AnsiString _cut1, AnsiString _cut2)
{
	pNN->Caption = _TubeNumber;
	pSolidGroup->Caption = _SolidGroup;
	pSolidGroup->Color = _SolidGroupColor;
	lResult->Caption = _Result;
	lResult->Color = _ResultColor;
	lCut1->Caption = _cut1;
	lCut2->Caption = _cut2;
}

void TFRTubeHistory::Clear() {
	pNN->Caption = "";
	pSolidGroup->Caption = "";
	pSolidGroup->Color = clBtnFace;
	lResult->Caption = "";
	lResult->Color = clBtnFace;
	lCut1->Caption = "";
	lCut2->Caption = "";
}

void TFRTubeHistory::CopyFrom(TFRTubeHistory* _th) {
	pNN->Caption = _th->pNN->Caption;
	pSolidGroup->Caption = _th->pSolidGroup->Caption;
	pSolidGroup->Color = _th->pSolidGroup->Color;
	lResult->Caption = _th->lResult->Caption;
	lResult->Color = _th->lResult->Color;
	lCut1->Caption = _th->lCut1->Caption;
	lCut2->Caption = _th->lCut2->Caption;
}
AnsiString TFRTubeHistory::GetTubeNumber(void)
{
	return(pNN->Caption);
}

