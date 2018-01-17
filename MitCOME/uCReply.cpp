// ---------------------------------------------------------------------------

#pragma hdrstop

#include "uCReply.h"
#include <stdio.h>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

CReply::CReply(int _abonent)
{
	ENQ = (char)0x05;
	STX = (char)0x02;
	ETX = (char)0x03;
	ACK = (char)0x06;
	NAK = (char)0x15;
	CR = (char)0x0D;
	LF = (char)0x0A;

	size = 0;
	data_size = 0;
	abonent = IntToHex2(_abonent);
}

AnsiString CReply::IntToHex2(int _v)
{
	char* s = "00";
	sprintf(s, "%02X", _v);
	AnsiString ret = s;
	return (ret);
}

AnsiString CReply::IntToHex(int _v)
{
	char* s = "00";
	sprintf(s, "%X", _v);
	AnsiString ret = s;
	return (ret);
}

AnsiString CReply::CRC(AnsiString _packet)
{
	int crc = 0;
	for (int i = 0; i < _packet.Length(); i++)
	{
		crc += _packet[i + 1];
	}
	crc &= 0xFF;
	return (AnsiString::IntToHex(crc, 2));
}

/*
 bool CReply::parse(AnsiString _l)
 {
 return (false);
 }
 */
bool CReplyE::parse(AnsiString _l)
{
	if (_l.Length() < 11)
		return (false);
	int pos = _l.Pos(STX);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 11)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	x += 4;
	if (l[x] != ETX[1])
		return (false);
	x += 1;
	if (l.SubString(x, 2) != CRC(l.SubString(2, 2 + 4)))
		return (false);
	x += 2;
	if (l[x] != CR[1])
		return (false);
	result = l.SubString(4, 4);
	reply_type = "E";
	return (true);
}

bool CReplyE1::parse(AnsiString _l)
{
	if (_l.Length() < 9)
		return (false);
	int pos = _l.Pos(STX);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 9)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	x += 2;
	if (l[x] != ETX[1])
		return (false);
	x += 1;
	if (l.SubString(x, 2) != CRC(l.SubString(2, 2 + 2)))
		return (false);
	x += 2;
	if (l[x] != CR[1])
		return (false);
	result = l.SubString(4, 2);
	reply_type = "E1";
	return (true);
}

bool CReplyE2::parse(AnsiString _l)
{
	if (_l.Length() < 13)
		return (false);
	int pos = _l.Pos(STX);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 13)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	x += 6;
	if (l[x] != ETX[1])
		return (false);
	x += 1;
	if (l.SubString(x, 2) != CRC(l.SubString(2, 2 + 6)))
		return (false);
	x += 2;
	if (l[x] != CR[1])
		return (false);
	result = l.SubString(4, 6);
	reply_type = "E2";
	return (true);
}

bool CReplyE3::parse(AnsiString _l)
{
	if (_l.Length() < 7 + data_size)
		return (false);
	int pos = _l.Pos(STX);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 7 + data_size)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	x += data_size;
	if (l[x] != ETX[1])
		return (false);
	x += 1;
	if (l.SubString(x, 2) != CRC(l.SubString(2, 2 + data_size)))
		return (false);
	x += 2;
	result = l.SubString(4, data_size);
	reply_type = "E3";
	if (l[x] != CR[1])
	{
		return (false);
	}
	return (true);
}

void CReplyE3::SetDataSize(int _data_size)
{
	data_size = _data_size;
	if (data_size < 0)
		data_size = 0;
	size=7+data_size;
}

bool CReplyC::parse(AnsiString _l)
{
	if (_l.Length() < 4)
		return (false);
	int pos = _l.Pos(ACK);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 4)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	if (l[x] != CR[1])
		return (false);
	result = "ACK";
	reply_type = "C";
	return (true);
}

bool CReplyD::parse(AnsiString _l)
{
	if (_l.Length() < 5)
		return (false);
	int pos = _l.Pos(NAK);
	if (pos <= 0)
		return (false);
	AnsiString l = _l.Delete(1, pos - 1);
	if (l.Length() < 5)
		return (false);
	int x = 1;
	x += 1;
	if (l.SubString(x, 2) != abonent)
		return (false);
	x += 2;
	x += 1;
	if (l[x] != CR[1])
		return (false);
	result = l.SubString(4, 1);
	reply_type = "D";
	return (true);
}
