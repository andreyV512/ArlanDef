//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "Statistics.h"
#include "IniFiles.hpp"
#include "Global.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
Statistics::Statistics()
{
	ini = new TIniFile(Globals::IniFileName);
	Valid = ini->ReadInteger("Statistics","Valid",0);
	SecondClass = ini->ReadInteger("Statistics","SecondClass",0);
	Brack = ini->ReadInteger("Statistics","Brack",0);
	BrackInDaStreet = ini->ReadInteger("Statistics","BrackInDaStreet",0);
	delete ini;
}
//---------------------------------------------------------------------------
bool Statistics::AddTube(UnicodeString Result)
{
	if(Result=="Ã")
		{
			Valid ++;
			ini = new TIniFile(Globals::IniFileName);
			ini->WriteInteger("Statistics","Valid",Valid);
			delete ini;
			return true;
		}
	else
	if(Result=="Á")
		{
			Brack ++;
			BrackInDaStreet ++;
			ini = new TIniFile(Globals::IniFileName);
			ini->WriteInteger("Statistics","Brack",Brack);
			ini->WriteInteger("Statistics","BrackInDaStreet",BrackInDaStreet);
			delete ini;
			return true;
		}
	else
	if(Result=="Ê")
		{
			SecondClass ++;
			ini = new TIniFile(Globals::IniFileName);
			ini->WriteInteger("Statistics","SecondClass",SecondClass);
			delete ini;
			return true;
		}
	else
		return false;
}
//------------------------------------------------------------------------------
/*
void Statistics::GetValue(ULONG _Brack, ULONG _SecondClass, ULONG _Valid)
{
	_Brack=Brack;
	_SecondClass=SecondClass;
	_Valid=Valid;
}
*/
//------------------------------------------------------------------------------
ULONG Statistics::GetBrack()
{
	return Brack;
}
ULONG Statistics::GetSecondClass()
{
	return SecondClass;
}
ULONG Statistics::GetValid()
{
	return Valid;
}
ULONG Statistics::GetBrackInDaStreet()
{
	return BrackInDaStreet;
}
void Statistics::SetBrackInDaStreet( int BrackCount )
{
	BrackInDaStreet = BrackCount;
}

//------------------------------------------------------------------------------
void Statistics::StatsToNull()
{
	Brack = 0;
	SecondClass = 0;
	Valid = 0;
	ini = new TIniFile(Globals::IniFileName);
	ini->WriteInteger("Statistics","Valid",Valid);
	ini->WriteInteger("Statistics","SecondClass",SecondClass);
	ini->WriteInteger("Statistics","Brack",Brack);
	delete ini;
}
//------------------------------------------------------------------------------

void Statistics::GetAllValue(ULONG &_Brack, ULONG &_SecondClass, ULONG &_Valid)
{
	DM->Find->Close();
	DM->Find->Active=false;
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add("SELECT Result FROM dbo.InfoTube WHERE (Result = 'Á')");
	DM->Find->ExecSQL();
	DM->Find->Open();
	_Brack=DM->Find->RecordCount;

	DM->Find->Close();
	DM->Find->Active=false;
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add("SELECT Result FROM dbo.InfoTube WHERE (Result = 'Ê')");
	DM->Find->ExecSQL();
	DM->Find->Open();
	_SecondClass=DM->Find->RecordCount;

	DM->Find->Close();
	DM->Find->Active=false;
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add("SELECT Result FROM dbo.InfoTube WHERE (Result = 'Ã')");
	DM->Find->ExecSQL();
	DM->Find->Open();
	_Valid=DM->Find->RecordCount;
}
//------------------------------------------------------------------------------

