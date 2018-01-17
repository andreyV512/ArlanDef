// ---------------------------------------------------------------------------
#ifndef SignalListDefH
#define SignalListDefH
// ---------------------------------------------------------------------------
#include "SignalList.h"
class SignalListDef : public SignalList
{
private:
	virtual bool CheckAlarm(void);
	bool CrossCycle;
	bool LinearCycle;
	bool InvA;
public:
	SignalListDef(TIniFile* _ini);
	void SetCrossCycle(bool _v);
	void SetLinearCycle(bool _v);
	void SetInvA(bool _v);
	// Card0_IN_Digital]
	CSignal* iCC; // 0=���� ����������
	CSignal* iLPCHA; // 1=���� �� �
	CSignal* iLPCHRUN; // 2=���� �� RUN
	CSignal* iSGTUBE; // 5=�� �����
	CSignal* iCSTROBE; // 6=����� �����
	CSignal* iLSTROBE; // 7=���� �����
	CSignal* iCCYCLE; // 8=����� ����
	CSignal* iLCYCLE; // 9=���� ����
	CSignal* iREADY; // 10=����������
	CSignal* iCCONTROL; // 11=����� ��������
	CSignal* iLCONTROL; // 12=���� ��������
	CSignal* iSGON; // 15=�� �������

	// Card0_OUT_Digital]
	CSignal* oLPCHPOW; // 0=���� ��� ��
	CSignal* oLSCANPOW; // 1=���� ��� ��
	CSignal* oLSOLPOW; // 2=�������� ����
	CSignal* oCSOLPOW; // 3=�������� �����
	CSignal* oCWORK; // 7=����� ������
	CSignal* oLWORK; // 8=���� ������
	CSignal* oCMEAS; // 9=����� ���������
	CSignal* oLMEAS; // 10=���� ���������
	CSignal* oSHIFT; // 11=����������
	CSignal* oCSTROBE; // 12=����� �����
	CSignal* oLSTROBE; // 13=���� �����
	CSignal* oCRESULT; // 14=����� ���������
	CSignal* oLRESULT; // 15=���� ���������
};
extern SignalListDef* SLD;
#endif
