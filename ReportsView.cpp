//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReportsView.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


TFormReport *FormReport;
//---------------------------------------------------------------------------
__fastcall TFormReport::TFormReport(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::FormCreate(TObject *Sender)
{
	DateBegin->Date=IncMonth(Date(),-1);
	DateEnd->MaxDate=IncMonth(Date(),+1);
	DateEnd->Date=Date();
	KeyPreview=true;

	Rp=new Reports();
}
//---------------------------------------------------------------------------


void __fastcall TFormReport::CheckBoxResultClick(TObject *Sender)
{
	if(CheckBoxResult->Checked)
		ListBoxResult->Enabled=true;
	else
		ListBoxResult->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::FormActivate(TObject *Sender)
{
	CheckBoxResult->Checked=false;
	ListBoxResult->Enabled=false;
	CheckBoxSG->Checked=false;
	ListBoxSG->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::CheckBoxSGClick(TObject *Sender)
{
	if(CheckBoxSG->Checked)
		this->ListBoxSG->Enabled=true;
	else
		this->ListBoxSG->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::BCancelClick(TObject *Sender)
{
	FormReport->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::BCreateReportClick(TObject *Sender)
{
	UnicodeString SG="",Result="",TS="";
	bool fl=false;
	if(CheckBoxResult->Checked)
		for(int i=0; i<3; i++)
			if(ListBoxResult->Selected[i])
				{
					UnicodeString buf="";
					switch (i)
					{
						case 0: buf="Ã"; break;
						case 1: buf="Á"; break;
						case 2: buf="Ê"; break;
					}
					if(fl)
						Result+=",'"+buf+"'";
					else
					{
						Result+="'"+buf+"'";
						fl=true;
					}
				}
	fl=false;
	if(CheckBoxTS->Checked)
		for(int i=0; i<3; i++)
			if(ListBoxTS->Selected[i])
				{
					UnicodeString buf="";
					switch(i)
					{
						case 0: buf="60";
						case 1: buf="73";
						case 2: buf="89";
					}
					if(fl)
						TS+=",'"+TS+"'";
					else
					{
						TS+="'"+buf+"'";
						fl=true;
                    }
				}

	fl=false;
	if(CheckBoxSG->Checked)
		for(int i=0; i<3; i++)
			if(ListBoxSG->Selected[i])
			{
				UnicodeString buf="";
				switch (i)
				{
					case 0: buf="Ä"; break;
					case 1: buf="Ê"; break;
					case 2: buf="Å"; break;
				}

				if(fl)
					SG+=",'"+buf+"'";
				else
				{
					SG+="'"+buf+"'";
					fl=true;
				}
			}

	if(SG != "")
		SG="("+SG+")";
	if(Result != "")
		Result="("+Result+")";
	if(TS != "")
		TS="("+TS+")";
	Rp->ExportAllTubes(DateBegin->DateTime,DateEnd->DateTime,SG,Result,TS);
}
//---------------------------------------------------------------------------


void __fastcall TFormReport::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		FormReport->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::CheckBoxTSClick(TObject *Sender)
{
	if(CheckBoxTS->Checked)
		ListBoxTS->Enabled=true;
	else
		ListBoxTS->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormReport::FormDestroy(TObject *Sender)
{
	delete Rp;
}
//---------------------------------------------------------------------------

