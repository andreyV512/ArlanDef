//---------------------------------------------------------------------------

#ifndef InverterH
#define InverterH
#include "MitCome/uCInv.h"
#include "Protocol.h"
#include "vcl.h"

//------------------------------------------------------------------------------
class rotationThread;
class Inverter
{
	public:
		Inverter();
		~Inverter();
		//! @brief Устанавливает скорость в один из параметров
		//! @param number - номер пункта в частотнике ( 4,5,6 )
		//! @param value  - значение в Гц
		bool setParameterSpeed(int number, int value);
		//! @param number номер пункта пч (4,5,6)
		int getParameterSpeed(int number);
		//! Включает вращение
		bool startRotation();
		//! Останавливает вращение
		bool stopRotation();
		//! Считывает все значения, нужно для проверки в отдельном потоке
		bool stateRead();
		//! Переводит ПЧ в режим NET
		bool NETManage();
		//! Сбрасывает ошибки с частотника
		bool ResetErrors();
	private:
		//
		CInv* inverter;


		bool isConnect;
		//! Поток который будет запрашивать состояние частоника каждые 500мс
		rotationThread *testThread;
};
//------------------------------------------------------------------------------
class rotationThread: public TThread
{
	protected:
		void __fastcall Execute();
	public:
		__fastcall rotationThread(bool CreateSuspended, Inverter *inv);
		__fastcall ~rotationThread();
		void Stop();
	private:
		TMutex *mutex;
		Inverter *inverter;
		bool isStop;
};
extern Inverter *frConverter;
//------------------------------------------------------------------------------

#endif


