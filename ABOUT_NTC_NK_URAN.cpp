//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ABOUT_NTC_NK_URAN.h"
#include "IOUtils.hpp"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox1 *AboutBox1;
//--------------------------------------------------------------------- 
__fastcall TAboutBox1::TAboutBox1(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox1::OKButtonClick(TObject *Sender)
{
	AboutBox1->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox1::FormShow(TObject *Sender)
{
	TFile *f = new TFile();
	lCreationDate->Caption = "Дата сборки: " + 	f->GetLastWriteTime(Application->ExeName);
	delete f;
}
//---------------------------------------------------------------------------

