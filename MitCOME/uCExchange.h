// ---------------------------------------------------------------------------
#ifndef uCExchangeH
#define uCExchangeH
#include "uCRequest.h"
#include "uCReply.h"
#include "uCRListT.h"
#include "uCCOMPort.h"
// ---------------------------------------------------------------------------
class CExchange
{
private:
	int abonent;
	int timeout;
	CRequest* request;
	CRListT<CReply> Lreply;
	bool Exec0(CCOMPort* _comport,AnsiString* _reply_type,AnsiString* _result);
public:
	CExchange(int _abonent, int _timeout);
	~CExchange();
	void Clear();
	CRequest* SetRequest(AnsiString _type,AnsiString _cmd);
	CReply* AddReply(AnsiString _name);
	bool Exec(CCOMPort* _comport,AnsiString* _reply_type,AnsiString* _result,int _iters);
};
#endif
