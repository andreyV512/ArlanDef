// ---------------------------------------------------------------------------
#ifndef uCReplyH
#define uCReplyH
// ---------------------------------------------------------------------------
#include<System.hpp>

class CReply
{
private:
protected:
	AnsiString ENQ;
	AnsiString STX;
	AnsiString ETX;
	AnsiString ACK;
	AnsiString NAK;
	AnsiString CR;
	AnsiString LF;

	AnsiString CRC(AnsiString _packet);
	AnsiString IntToHex2(int _v);
	AnsiString IntToHex(int _v);

	int data_size;
	int size;

public:
	CReply(int _abonent);

	AnsiString result;
	AnsiString reply_type;
	AnsiString abonent;

	virtual bool parse(AnsiString _l)
	{
		return (false);
	}

	virtual void SetDataSize(int _data_size)
	{
		data_size = 0;
	};

	inline int GetSize(void)
	{
		return (size);
	}
};

class CReplyE : public CReply
{
public:
	CReplyE(int _abonent) : CReply(_abonent)
	{
		size = 11;
	}
	bool parse(AnsiString _l);
};

class CReplyE1 : public CReply
{
public:
	CReplyE1(int _abonent) : CReply(_abonent)
	{
		size = 9;
	}
	virtual bool parse(AnsiString _l);
};

class CReplyE2 : public CReply
{
public:
	CReplyE2(int _abonent) : CReply(_abonent)
	{
		size = 13;
	}
	virtual bool parse(AnsiString _l);
};

class CReplyE3 : public CReply
{
public:
	CReplyE3(int _abonent) : CReply(_abonent)
	{
		size = 7;
	}
	virtual bool parse(AnsiString _l);
	virtual void SetDataSize(int _data_size);
};

class CReplyC : public CReply
{
public:
	CReplyC(int _abonent) : CReply(_abonent)
	{
		size = 4;
	}
	virtual bool parse(AnsiString _l);
};

class CReplyD : public CReply
{
public:
	CReplyD(int _abonent) : CReply(_abonent)
	{
		size = 5;
	}
	virtual bool parse(AnsiString _l);
};
#endif
