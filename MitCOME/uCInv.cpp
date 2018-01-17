	// ---------------------------------------------------------------------------
#pragma hdrstop
#include "uCInv.h"
#include "uCCOMPort.h"
#include "uCExchange.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
CInv::CInv()
{
	port="COM8";
	abonent=2;
	timeout=1;
	BaudRate=19200;
	ByteSize=7;
	Parity=2;
	StopBits=0;
	ReadIntervalTimeout=500;
	ReadTotalTimeoutConstant=500;
	ReadTotalTimeoutMultiplier=0;
	comport=NULL;
	OnProtocolL=NULL;

	iRUN=false;
	iSTF=false;
	iSTR=false;
	iSU=false;
	iOL=false;
	iFU=false;
	iABC=false;

	oSTF=false;
	oSTR=false;
	oRL=false;
	oRM=false;
	oRH=false;
	oRT=false;
	oMRS=false;
	max_iters=1;
}
CInv::~CInv()
{
	if(comport!=NULL)
		delete comport;
}
bool CInv::Open(void)
{
	if(comport!=NULL)
		delete comport;
	comport=CCOMPort::Create(port,
		BaudRate,
		ByteSize,
		Parity,
		StopBits,
		ReadIntervalTimeout,
		ReadTotalTimeoutConstant,
		ReadTotalTimeoutMultiplier);
	if(comport==NULL)
	{
		if(OnProtocolL!=NULL)
		{
			OnProtocolL(CCOMPort::error_msg);
			AnsiString a="CInv::Open: не могу открыть COM порт: ";
			a+=port;
			OnProtocolL(a);
		}
	}
	else
		comport->OnProtocol=OnProtocolL;
	return (comport!=NULL);
}
void CInv::Close(void)
{
	if(comport!=NULL)
		delete comport;
}
void CInv::FOnProtocolWrite(OnProtocolDef _v)
{
	OnProtocolL=_v;
	if(comport!=NULL)
		comport->OnProtocol=OnProtocolL;
}
bool CInv::StateRead(void)
{
	AnsiString reply;
	AnsiString reply_data;
	AnsiString H0="0x";
	if(!Exec("7A","B","","E1",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="E1")
	{
		error_msg="CInv::StateRead: Ответ ";
		error_msg+=reply;
		error_msg+=" , а должен быть E1";
		return (false);
	}
	byte state=(byte)StrToInt(H0+reply_data);
//	byte state=(byte)StrToInt("C3");
	iRUN=state&(1<<0);
	iSTF=state&(1<<1);
	iSTR=state&(1<<2);
	iSU=state&(1<<3);
	iOL=state&(1<<4);
	iFU=state&(1<<6);
	iABC=state&(1<<7);
	return (true);
}
bool CInv::StateWrite(void)
{
	byte state=0;
	if(oSTF)
		state|=(1<<1);
	if(oSTR)
		state|=(1<<2);
	if(oRL)
		state|=(1<<3);
	if(oRM)
		state|=(1<<4);
	if(oRH)
		state|=(1<<5);
	if(oRT)
		state|=(1<<6);
	if(oMRS)
		state|=(1<<7);
	AnsiString reply;
	AnsiString reply_data;
	AnsiString av;
	av=AnsiString::IntToHex(state,2);
	if(!Exec("FA","A1",av,"C",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="C")
	{
		error_msg="CInv::StateWrite: Ответ ";
		error_msg+=reply;
		error_msg+=" , а должен быть C, ";
		error_msg+=reply_data;
		return (false);
	}
	return (true);
}
bool CInv::Exec(
	AnsiString _code,
	AnsiString _request,
	AnsiString _request_data,
	AnsiString _reply0,
	int _reply0_size,
	AnsiString _reply1,
	int _reply1_size,
	AnsiString _reply2,
	int _reply2_size,
	AnsiString* _reply,
	AnsiString* _reply_data)
{
	if(comport==NULL)
	{
		error_msg="CInv::Exec: Не открыт COM порт";
		return (false);
	}
	CExchange* ex=new CExchange(abonent,timeout);
	CRequest* req=ex->SetRequest(_request,_code);
	req->Data(_request_data);
	if(_reply0.Length()!=0)
		ex->AddReply(_reply0)->SetDataSize(_reply0_size);
	if(_reply1.Length()!=0)
		ex->AddReply(_reply1)->SetDataSize(_reply1_size);
	if(_reply2.Length()!=0)
		ex->AddReply(_reply2)->SetDataSize(_reply2_size);
	bool ret=ex->Exec(comport,_reply,_reply_data,max_iters);
	error_msg="";
	if(!ret)
		error_msg=*_reply_data;
	delete ex;
	return (ret);
}
bool CInv::SetFrequency(AnsiString _cmd,int _frequency)
{
	AnsiString reply;
	AnsiString reply_data;
	AnsiString av=AnsiString::IntToHex(_frequency,4);
	if(!Exec(_cmd,"A",av,"C",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="C")
	{
		error_msg="CInv::SetFrequency: Ответ ";
		error_msg+=reply;
		error_msg+=" , а должен быть C ;";
		error_msg+=reply_data;

		return (false);
	}
	return(true);
}
int CInv::GetFrequency(AnsiString _cmd)
{
	AnsiString reply;
	AnsiString reply_data;
	if(!Exec(_cmd,"B","","E",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="E")
	{
		error_msg="CInv::GetFrequency: Ответ ";
		error_msg+=reply;
		error_msg+=" , а должен быть E";
		return (-1);
	}
	AnsiString H0="0x";
	return(StrToInt(H0+reply_data));
}

bool CInv::SetMode(int _mode)
{
	AnsiString reply;
	AnsiString reply_data;
	AnsiString av=AnsiString::IntToHex(_mode,4);
	if(!Exec("FB","A",av,"C",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="C")
	{
		error_msg="CInv::SetMode: Ответ ";
		error_msg+=reply;
		error_msg+=": ";
		error_msg+=reply_data;
		error_msg+=" , а должен быть C";
		return (false);
	}
	return(true);
}
int CInv::GetMode(void)
{
	AnsiString reply;
	AnsiString reply_data;
	if(!Exec("7B","B","","E",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="E")
	{
		error_msg="CInv::GetMode: Ответ ";
		error_msg+=reply;
		error_msg+=": ";
		error_msg+=reply_data;
		error_msg+=" , а должен быть E";
		return (-1);
	}
	AnsiString H0="0x";
	return(StrToInt(H0+reply_data));
}
bool CInv::Reset9696(void)
{
	AnsiString reply;
	AnsiString reply_data;
	if(!Exec("FD","A","9696","C",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="C")
	{
		error_msg="CInv::Reset9696: Ответ ";
		error_msg+=reply;
		error_msg+=": ";
		error_msg+=reply_data;
		error_msg+=" , а должен быть C";
		return (false);
	}
	return(true);
}
bool CInv::Reset9966(void)
{
	AnsiString reply;
	AnsiString reply_data;
	if(!Exec("FD","A","9966","C",0,"D",0,"",0,&reply,&reply_data))
		return (false);
	if(reply!="C")
	{
		error_msg="CInv::Reset9966: Ответ ";
		error_msg+=reply;
		error_msg+=": ";
		error_msg+=reply_data;
		error_msg+=" , а должен быть C";
		return (false);
	}
	return(true);
}


