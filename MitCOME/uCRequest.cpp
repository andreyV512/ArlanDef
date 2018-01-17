// ---------------------------------------------------------------------------
#pragma hdrstop
#include "uCRequest.h"
#include <stdio.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
CRequest::CRequest(int _abonent,int _timeout,AnsiString _type,AnsiString _cmd)
{
	ENQ=(char)0x05;
	STX=(char)0x02;
	ETX=(char)0x03;
	ACK=(char)0x06;
	NAK=(char)0x15;
	CR=(char)0x0D;
	LF=(char)0x0A;

	abonent=_abonent;
	timeout=_timeout;
	cmd=_cmd;
	if(_type=="B")
		data_size=0;
	else if(_type=="A")
		data_size=4;
	else if(_type=="A1")
		data_size=2;
	else
		data_size=0;
	data="";
}
AnsiString CRequest::IntToHex2(int _v)
{
	char* s="00";
	sprintf(s,"%02X",_v);
	AnsiString ret=s;
	return(ret);
}
AnsiString CRequest::IntToHex(int _v)
{
	char* s="00";
	sprintf(s,"%X",_v);
	AnsiString ret=s;
	return(ret);
}

AnsiString CRequest::CRC1(AnsiString _packet)
{
	int crc=0;
	for(int i=1;i<_packet.Length();i++)
	{
		crc+=_packet[i+1];
	}
	crc&=0xFF;
	return (IntToHex2(crc));
}
AnsiString CRequest::Get()
{
	AnsiString packet;
	packet+=ENQ;
	packet+=IntToHex2(abonent);
	packet+=cmd;
	packet+=IntToHex(timeout);
	if(data_size!=0)
		packet+=data;
	packet+=CRC1(packet);
	packet+=CR;
	return (packet);
}

