//------------------------------------------------------------------------------


#pragma hdrstop

#include "Inverter.h"
#include <IniFiles.hpp>
#include "Global.h"

//------------------------------------------------------------------------------

#pragma package(smart_init)
Inverter *frConverter;
Inverter::Inverter()
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	AnsiString ini_section="Inverter";
	if(!ini->ValueExists(ini_section,"PortName"))
		ini->WriteString(ini_section,"PortName","COM3");
	if(!ini->ValueExists(ini_section,"Abonent"))
		ini->WriteInteger(ini_section,"Abonent",2);
	if(!ini->ValueExists(ini_section,"timeout"))
		ini->WriteInteger(ini_section,"timeout",1);
	if(!ini->ValueExists(ini_section,"BaudRate"))
		ini->WriteInteger(ini_section,"BaudRate",19200);
	if(!ini->ValueExists(ini_section,"DataBits"))
		ini->WriteInteger(ini_section,"DataBits",7);
	if(!ini->ValueExists(ini_section,"Parity"))
		ini->WriteInteger(ini_section,"Parity",2);
	if(!ini->ValueExists(ini_section,"StopBits"))
		ini->WriteInteger(ini_section,"StopBits",0);
	if(!ini->ValueExists(ini_section,"ReadIntervalTimeout"))
		ini->WriteInteger(ini_section,"ReadIntervalTimeout",500);
	if(!ini->ValueExists(ini_section,"ReadTotalTimeoutConstant"))
		ini->WriteInteger(ini_section,"ReadTotalTimeoutConstant",500);
	if(!ini->ValueExists(ini_section,"ReadTotalTimeoutMultiplier"))
		ini->WriteInteger(ini_section,"ReadTotalTimeoutMultiplier",0);
	if(!ini->ValueExists(ini_section,"MaxIters"))
		ini->WriteInteger(ini_section,"MaxIters",1);

	inverter = new CInv();
	inverter->port=ini->ReadString(ini_section,"PortName","COM3");
	inverter->abonent=ini->ReadInteger(ini_section,"Abonent",2);
	inverter->timeout=ini->ReadInteger(ini_section,"timeout",1);
	inverter->BaudRate=ini->ReadInteger(ini_section,"BaudRate",19200);
	inverter->ByteSize = ini->ReadInteger(ini_section,"DataBits",7);
	inverter->Parity = ini->ReadInteger(ini_section,"Parity",2);
	inverter->StopBits = ini->ReadInteger(ini_section,"StopBits",0);
	inverter->ReadIntervalTimeout = ini->ReadInteger(ini_section,"ReadIntervalTimeout",500);
	inverter->ReadTotalTimeoutConstant = ini->ReadInteger(ini_section,"ReadTotalTimeoutConstant",500);
	inverter->ReadTotalTimeoutMultiplier = ini->ReadInteger(ini_section,"ReadTotalTimeoutMultiplier",0);
	inverter->max_iters = ini->ReadInteger(ini_section,"MaxIters",1);
	delete ini;
	inverter->Open();
}
//------------------------------------------------------------------------------
bool Inverter::setParameterSpeed(int number, int value)
{
	if( (number < 4) || (number > 6) )
		return false;
	bool ret=inverter->SetFrequency(IntToStr(80+number),value*100);
	if(!ret)
		TPr::pr("Inverter::setParameterSpeed: ошибка:  "+inverter->error_msg);
	return ret;
}
//------------------------------------------------------------------------------
int Inverter::getParameterSpeed(int number)
{
	return (int)inverter->GetFrequency("0"+IntToStr(number))/100;
}
//------------------------------------------------------------------------------
bool Inverter::startRotation()
{
	ResetErrors();

	inverter->oSTF = true;
	//Вот тут сменить на oRL - 6пункт, oRM - 5пункт
	inverter->oRH  = true;
	inverter->oMRS  = true;
	if(!inverter->StateWrite())
		return false;

	if(inverter->error_msg.Length()!=0)
		TPr::SendToProtocol("Inverter::startRotation: ошибка  "+inverter->error_msg);
	if(testThread == NULL)
	{
		testThread = new rotationThread(true,this);
		testThread->FreeOnTerminate = false;
		//!Todo подумать над Start();
		testThread->Resume();
//		testThread->Start();
	}

	return true;
}
//------------------------------------------------------------------------------
bool Inverter::stopRotation()
{
	testThread->Stop();
	inverter->oSTF = false;
	inverter->oRH  = false;
//    inverter->oMRS = true;
	if(!inverter->StateWrite())
		return false;

	if(testThread)
	{
		delete testThread;
		testThread = NULL;
	}
	return true;
}
//------------------------------------------------------------------------------
bool Inverter::stateRead()
{
	return inverter->StateRead();
}
//------------------------------------------------------------------------------
bool Inverter::NETManage()
{
	return inverter->SetMode(2);
}
//------------------------------------------------------------------------------
bool Inverter::ResetErrors()
{
	return inverter->Reset9696();
}
//------------------------------------------------------------------------------
Inverter::~Inverter()
{
	delete inverter;
}
//******************************************************************************
__fastcall rotationThread::rotationThread(bool CreateSuspended, Inverter *inv)
{
	inverter = inv;
	mutex = new TMutex(false);
}
//------------------------------------------------------------------------------
void rotationThread::Stop()
{
	mutex->Acquire();
	isStop = false;
	mutex->Release();
}
//------------------------------------------------------------------------------
void __fastcall rotationThread::Execute()
{
	isStop = true;
	while(isStop)
	{
		inverter->stateRead();
		TThread::CurrentThread->Sleep( 500 );
	}
}
//------------------------------------------------------------------------------
__fastcall rotationThread::~rotationThread()
{
	delete mutex;
}
//------------------------------------------------------------------------------
