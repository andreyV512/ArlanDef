// ---------------------------------------------------------------------------
#ifndef uCCOMPortH
#define uCCOMPortH
// ---------------------------------------------------------------------------
#include<system.hpp>
// ---------------------------------------------------------------------------
typedef void (__closure *OnProtocolDef)(AnsiString _msg);
class CCOMPort
{
private:
	AnsiString COM;
	HANDLE handleCom;
	bool verbal;
	CCOMPort(){};
public:
	static CCOMPort* Create(AnsiString _port,
		int _BaudRate,
		int _ByteSize,
		int _Parity,
		int _StopBits,
		int _ReadIntervalTimeout,
		int _ReadTotalTimeoutConstant,
		int _ReadTotalTimeoutMultiplier
		);
	~CCOMPort(void);
	bool read_s(unsigned char* _s,int _n);
	bool write_s(unsigned char* _s,int _n);
	OnProtocolDef OnProtocol;
	static AnsiString error_msg;
	void ClearBuffer(void);
};
// ---------------------------------------------------------------------------
#endif
