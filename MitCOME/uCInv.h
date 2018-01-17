// ---------------------------------------------------------------------------
#ifndef uCInvH
#define uCInvH
// ---------------------------------------------------------------------------
#include<System.hpp>
#include "uCCOMPort.h"
class CInv
{
private:
	CCOMPort* comport;
	void FOnProtocolWrite(OnProtocolDef _v);
	OnProtocolDef OnProtocolL;

public:
	CInv(void);
	~CInv(void);
	bool Open(void);
	void Close(void);

	AnsiString port;
	int abonent;
	int timeout;
	int BaudRate;
	int ByteSize;
	int Parity;
	int StopBits;
	int ReadIntervalTimeout;
	int ReadTotalTimeoutConstant;
	int ReadTotalTimeoutMultiplier;
	__property OnProtocolDef OnProtocol=
	{write=FOnProtocolWrite};
	bool StateRead(void);
	bool StateWrite(void);
	AnsiString error_msg;

	bool iRUN;
	bool iSTF;
	bool iSTR;
	bool iSU;
	bool iOL;
	bool iFU;
	bool iABC;

	bool oSTF;
	bool oSTR;
	bool oRL;
	bool oRM;
	bool oRH;
	bool oRT;
	bool oMRS;
	int max_iters;

	bool Exec(
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
		AnsiString* _reply_data);
	int GetFrequency(AnsiString _cmd);
	bool SetFrequency(AnsiString _cmd,int _frequency);
	inline bool SetFrequencyRH(int _frequency){return(SetFrequency("84",_frequency));};
	inline bool SetFrequencyRM(int _frequency){return(SetFrequency("85",_frequency));};
	inline bool SetFrequencyRL(int _frequency){return(SetFrequency("86",_frequency));};
	inline int  GetFrequencyRH(void){return(GetFrequency("04"));}; // -1 ошибка
	inline int  GetFrequencyRM(void){return(GetFrequency("05"));};
	inline int  GetFrequencyRL(void){return(GetFrequency("06"));};
	bool SetMode(int _mode);
	int GetMode(void);
	bool Reset9696(void);
	bool Reset9966(void);

};
#endif
