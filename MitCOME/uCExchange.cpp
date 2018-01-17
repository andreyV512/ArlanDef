// ---------------------------------------------------------------------------
#pragma hdrstop
#include "uCExchange.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CExchange::CExchange(int _abonent, int _timeout)
{
	abonent = _abonent;
	timeout = _timeout;
	request = NULL;
}

CExchange::~CExchange()
{
	Clear();
}

void CExchange::Clear()
{
	Lreply.Clear();
	if (request != NULL)
		delete request;
	request = NULL;
}

CRequest* CExchange::SetRequest(AnsiString _type, AnsiString _cmd)
{
	request = new CRequest(abonent, timeout, _type, _cmd);
	return (request);
}

CReply* CExchange::AddReply(AnsiString _name)
{
	CReply* p;
	if (_name == "C")
		p = new CReplyC(abonent);
	else if (_name == "D")
		p = new CReplyD(abonent);
	else if (_name == "E")
		p = new CReplyE(abonent);
	else if (_name == "E1")
		p = new CReplyE1(abonent);
	else if (_name == "E2")
		p = new CReplyE2(abonent);
	else if (_name == "E3")
		p = new CReplyE3(abonent);
	else
		p = new CReply(abonent);
	Lreply.Add(p);
	return (p);
}

bool CExchange::Exec0(CCOMPort* _comport, AnsiString* _reply_type,
	AnsiString* _result)
{
	*_result = "";
	*_reply_type = "";
	AnsiString req = request->Get();
	_comport->ClearBuffer();
	if (!_comport->write_s(req.c_str(), req.Length()))
	{
		*_result = "Не смогли послать запрос";
		return (false);
	}
	AnsiString packet;
	char* s = "12345";
	CReply* p;
	int max_size=0;
	for (int i = 0; i < Lreply.Count(); i++)
	{
		p = Lreply[i];
		if(max_size<p->GetSize())
			max_size=p->GetSize();
	}
	for (; ;)
	{
		if (!_comport->read_s(s, 1))
		{
			if (packet.Length() == 0)
				* _result = "Нет ответа";
			else
				*_result = "Ответ не разобран - короткий";
			return (false);
		}
		packet += s[0];
		for (int i = 0; i < Lreply.Count(); i++)
		{

			p = Lreply[i];
			if (p->parse(packet))
			{
				*_reply_type = p->reply_type;
				*_result = p->result;
				return (true);
			}
		}
		if(packet.Length()>=max_size)
		{
			*_result = "Ответ не разобран - синтаксис";
			return (false);
		}
	}
}
bool CExchange::Exec(CCOMPort* _comport,AnsiString* _reply_type,AnsiString* _result,int _iters)
{
	if(_iters<=0)
		_iters=1;
	for(int i=0;i<_iters;i++)
	{
		if(Exec0(_comport, _reply_type,_result))
			return(true);
	}
	return(false);
}

