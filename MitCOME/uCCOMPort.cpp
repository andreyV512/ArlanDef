// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include "uCCOMPort.h"
#pragma package(smart_init)
// ---------------------------------------------------------------------------

CCOMPort* CCOMPort::Create(
	AnsiString _port,
	int _BaudRate,
	int _ByteSize,
	int _Parity,
	int _StopBits,
	int _ReadIntervalTimeout,
	int _ReadTotalTimeoutConstant,
	int _ReadTotalTimeoutMultiplier)
{
	WideString w="\\\\.\\";
	w+=_port;
	CCOMPort* P=new CCOMPort();
	P->handleCom=CreateFile(w.c_bstr(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
	if(P->handleCom==INVALID_HANDLE_VALUE)
	{
		delete P;
		error_msg="CCOMPort::Create: Не могу открыть порт: ";
		error_msg+=_port;
		return (NULL);
	}
	DCB dcb;
	GetCommState(P->handleCom,&dcb);
	AnsiString a;
	dcb.BaudRate=_BaudRate;
	dcb.Parity=(BYTE)_Parity;
	dcb.StopBits=(BYTE)_StopBits;
	dcb.ByteSize=(BYTE)_ByteSize;
	if(SetCommState(P->handleCom,&dcb)==0)
	{
		delete P;
		error_msg="CCOMPort::Create: ";
		error_msg+=_port;
		error_msg+=": не могу выставить состояние";
		return (NULL);
	}
	COMMTIMEOUTS t;
	GetCommTimeouts(P->handleCom,&t);
	t.ReadIntervalTimeout=_ReadIntervalTimeout;
	t.ReadTotalTimeoutMultiplier=_ReadTotalTimeoutMultiplier;
	t.ReadTotalTimeoutConstant=_ReadTotalTimeoutConstant;
	if(SetCommTimeouts(P->handleCom,&t)==0)
	{
		delete P;
		error_msg="CCOMPort::Create: ";
		error_msg+=_port;
		error_msg+=": не могу выставить задержки";
		return (NULL);
	}
	if(SetCommMask(P->handleCom,0)==0)
	{
		delete P;
		error_msg="CCOMPort::Create: ";
		error_msg+=_port;
		error_msg+=": не могу выставить маску событий";
		return (NULL);
	}
	P->OnProtocol=NULL;
	error_msg="";
	return (P);
}
// ---------------------------------------------------------------------------
CCOMPort::~CCOMPort(void)
{
	CloseHandle(handleCom);
}
// ---------------------------------------------------------------------------
bool CCOMPort::read_s(unsigned char* _s,int _n)
{
	unsigned long ln=_n;
	ReadFile(handleCom,_s,_n,&ln,NULL);
	if(OnProtocol!=NULL)
	{
		AnsiString a;
		for(unsigned long i=0;i<ln;i++)
		{
			a.printf("r[%d]: %X |%c|",(int)i,_s[i],_s[i]);
			OnProtocol(a);
		}
	}
	if(ln<(unsigned long)_n)
		return (false);
	return (true);
}
// ------------------------------------------------------------------------------
bool CCOMPort::write_s(unsigned char* _s,int _n)
{
	unsigned long ln=_n;
	WriteFile(handleCom,_s,_n,&ln,NULL);
	if(OnProtocol!=NULL)
	{
		AnsiString a;
		for(unsigned long i=0;i<ln;i++)
		{
			a.printf("w[%d]: %X |%c|",(int)i,_s[i],_s[i]);
			OnProtocol(a);
		}
	}
	if(ln<(unsigned long)_n)
		return (false);
	return (true);
}
// ------------------------------------------------------------------------------
void CCOMPort::ClearBuffer(void)
{
	PurgeComm(handleCom,PURGE_RXCLEAR);
}
AnsiString CCOMPort::error_msg;

