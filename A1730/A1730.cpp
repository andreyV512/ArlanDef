//---------------------------------------------------------------------------
#pragma hdrstop
#include "A1730.h"
#include <SysUtils.hpp>
#include "Protocol.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
A1730::A1730(int _DevNum)
{
	startPort = 0;
	portCount = 4;
	ErrorCode errorCode;
	AccessMode AM = ModeWrite;
	device = NULL;
	errorCode = BDaqDevice::Open(_DevNum, AM, device);
	if (BioFailed(errorCode))
	{
		if (device != NULL)
			device->Close();
		device = NULL;
	}
	else if (errorCode == Success)
	{
		errorCode = device->GetModule(0, dio); // Get Dio Module
		if (errorCode == Success && ReadOnly)
		{
			if (device != NULL)
				device->Close();
			device = NULL;
		}
	}
}

A1730::~A1730(void)
{
	if (device != NULL)
		device->Close();
}

DWORD A1730::Read(void)
{
	if(device==NULL)
		return(0);
	BYTE buf[4];
	ErrorCode errorcode = dio->DiRead(startPort, portCount, buf);
	if (dio->DiRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730::ReadIn: не могу прочитать плату"));
	DWORD v = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (v);
}

DWORD A1730::ReadOut(void)
{
	if(device==NULL)
		return(0);
	BYTE buf[4];
	if (dio->DoRead(startPort, portCount, buf) != Success)
		throw(Exception("A1730::ReadOut: не могу прочитать плату"));
	DWORD bufOut = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (bufOut);
}

void A1730::Write(DWORD _v)
{
	if(device==NULL)
		return;
	BYTE buf[4];
	buf[0] = _v & 0xFF;
	buf[1] = _v >> 8 & 0xFF;
	buf[2] = _v >> 16 & 0xFF;
	buf[3] = _v >> 24 & 0xFF;
	if (dio->DoWrite(startPort, portCount, buf) != Success)
		throw(Exception("A1730::Write: не могу записать на плату"));
}
void A1730::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}
