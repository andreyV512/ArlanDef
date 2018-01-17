// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Protocol.h"

#include "stdio.h"
#include "uFunctions.h"
#include <IniFiles.hpp>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPr* TPr::Instance=NULL;
// ---------------------------------------------------------------------------
void TPr::SendToProtocol(AnsiString _msg)
{
	if(Instance==NULL)
		Instance=new TPr();
	Instance->Add(_msg);
}
void TPr::pr(AnsiString _msg)
{
	if(Instance==NULL)
		Instance=new TPr();
	Instance->Add(_msg);
}
void TPr::Clear(void)
{
	if(Instance!=NULL)
		Instance->Clear0();
}
void TPr::Dispose(void)
{
	if(Instance!=NULL)
	{
		delete Instance;
		Instance=NULL;
	}
}
void TPr::Show(void)
{
	if(Instance==NULL)
		Instance=new TPr();
	Instance->Show0();
}

// ---------------------------------------------------------------------------
TPr::TPr(void) : TForm((TComponent*)0)
{
}

// ---------------------------------------------------------------------------
void __fastcall TPr::FormCreate(TObject *Sender)
{
	cs=new TCriticalSection();
	L=new TStringList();
	TIniFile *ini=new TIniFile("..\\..\\Settings\\SettingsDefectoscope.ini");
	Timer1->Interval=ini->ReadInteger("Default","ProtocolPeriod",400);
	LoadFormPos(this,ini);
	cbProtocolToFile->Checked=ini->ReadBool("Default","ProtocolToFile",0);
	delete ini;
	FileName="..\\..\\Protocols\\Protocol_"+DateToStr(Date())+".txt";
	file=fopen(FileName.c_str(),"a");
	if(file==NULL)
	{
		CreateDirectoryW(L"..\\..\\Protocols",0);
		file=fopen(FileName.c_str(),"a");
	}
	mProtocol->Perform(WM_VSCROLL,SB_BOTTOM,0);
	L->Add("Открыли протокол: "+TimeToStr(Time()));
}

// ---------------------------------------------------------------------------
void __fastcall TPr::FormDestroy(TObject *Sender)
{
	fclose(file);
	TIniFile *ini=new TIniFile("..\\..\\Settings\\SettingsDefectoscope.ini");
	ini->WriteBool("Default","ProtocolToFile",cbProtocolToFile->Checked);
	SaveFormPos(this,ini);
	delete ini;
	delete L;
	delete cs;
}

void TPr::Add(AnsiString _msg)
{
	cs->Enter();
	{
		L->Add(_msg);
	} cs->Leave();
}

// ---------------------------------------------------------------------------

void __fastcall TPr::FormKeyDown(TObject *Sender,WORD &Key,
	TShiftState Shift)
{
	if(Key==27)
	cs->Enter();
	{
		Close();
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void TPr::Clear0()
{
	cs->Enter();
	{
		L->Clear();
		mProtocol->Clear();
		mProtocol->Lines->Clear();
	} cs->Leave();
}
void TPr::Show0(void)
{
	cs->Enter();
	{
		TForm::Show();
	} cs->Leave();
}
bool TPr::Visible0(void)
{
	bool ret;
	cs->Enter();
	{
		ret=TForm::Visible;
	} cs->Leave();
	return(ret);
}
bool TPr::Visible(void)
{
	if(Instance==NULL)
		return(false);
	return(Instance->Visible0());
}

void __fastcall TPr::FormResize(TObject *Sender)
{
	cs->Enter();
	{
		mProtocol->Width=ClientWidth-mProtocol->Left*2;
		mProtocol->Height=ClientHeight-mProtocol->Top-mProtocol->Left;
	} cs->Leave();
}
// ---------------------------------------------------------------------------
void __fastcall TPr::Timer1Timer(TObject *Sender)
{
	cs->Enter();
	{
		for(int i=0;i<L->Count;i++)
		{
			AnsiString a=L->Strings[i];
			if(Visible)
				mProtocol->Lines->Add(a);
			if(cbProtocolToFile->Checked)
			{
				fprintf(file,a.c_str());
				fprintf(file,"\n");
			}
		}
		if(cbProtocolToFile->Checked&&L->Count>0)
			fflush(file);
		L->Clear();
	} cs->Leave();
}
// ---------------------------------------------------------------------------
