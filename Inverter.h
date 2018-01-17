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
		//! @brief ������������� �������� � ���� �� ����������
		//! @param number - ����� ������ � ���������� ( 4,5,6 )
		//! @param value  - �������� � ��
		bool setParameterSpeed(int number, int value);
		//! @param number ����� ������ �� (4,5,6)
		int getParameterSpeed(int number);
		//! �������� ��������
		bool startRotation();
		//! ������������� ��������
		bool stopRotation();
		//! ��������� ��� ��������, ����� ��� �������� � ��������� ������
		bool stateRead();
		//! ��������� �� � ����� NET
		bool NETManage();
		//! ���������� ������ � ����������
		bool ResetErrors();
	private:
		//
		CInv* inverter;


		bool isConnect;
		//! ����� ������� ����� ����������� ��������� ��������� ������ 500��
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


