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
	CSignal* iCC; // 0=Цепи Управления
	CSignal* iLPCHA; // 1=Прод ПЧ А
	CSignal* iLPCHRUN; // 2=Прод ПЧ RUN
	CSignal* iSGTUBE; // 5=ГП Труба
	CSignal* iCSTROBE; // 6=Попер строб
	CSignal* iLSTROBE; // 7=Прод строб
	CSignal* iCCYCLE; // 8=Попер цикл
	CSignal* iLCYCLE; // 9=Прод цикл
	CSignal* iREADY; // 10=Готовность
	CSignal* iCCONTROL; // 11=попер контроль
	CSignal* iLCONTROL; // 12=прод контроль
	CSignal* iSGON; // 15=ГП Включен

	// Card0_OUT_Digital]
	CSignal* oLPCHPOW; // 0=Прод Пит ПЧ
	CSignal* oLSCANPOW; // 1=Прод Пит СУ
	CSignal* oLSOLPOW; // 2=Соленоид Прод
	CSignal* oCSOLPOW; // 3=Соленоид Попер
	CSignal* oCWORK; // 7=Попер Работа
	CSignal* oLWORK; // 8=Прод Работа
	CSignal* oCMEAS; // 9=Попер измерение
	CSignal* oLMEAS; // 10=Прод измерение
	CSignal* oSHIFT; // 11=Перекладка
	CSignal* oCSTROBE; // 12=Попер строб
	CSignal* oLSTROBE; // 13=Прод строб
	CSignal* oCRESULT; // 14=Попер результат
	CSignal* oLRESULT; // 15=Прод результат
};
extern SignalListDef* SLD;
#endif
