//---------------------------------------------------------------------------

#include "intsafe.h"
#include "DataModule.h"
#ifndef StatisticsH
#define StatisticsH
//---------------------------------------------------------------------------
//! ����� ���������� ������� ����
class Statistics
{
	public:
		//! ����������� �� ���������
		Statistics();
		//! ��������� ��������� �� �����
		bool AddTube(UnicodeString Result);
		//! �������� ��� ������
		void StatsToNull();
//		void GetValue(ULONG _Brack, ULONG _SecondClass, ULONG _Valid);   //���������� ���������� �� �����
		//! ���������� ����� ����������� ����
		ULONG GetBrack();
		//! ���������� ����� ���� ������� ������
		ULONG GetSecondClass();
		//! ���������� ����� ������ ����
		ULONG GetValid();
		//! ���������� ����� ����������� ���� � ������� �����
		ULONG GetBrackInDaStreet();
		//! ������������� ����� ����������� ���� � ������� �����
		void SetBrackInDaStreet( int BrackCount);
		//! ���������� ������ �� �����������  �� ��� �����(�� ����)
		void GetAllValue(ULONG &_Brack, ULONG &_SecondClass, ULONG &_Valid);
	private:
		//! ����� �����
		ULONG Brack;
		//! ����� ������� ������
		ULONG SecondClass;
		//! ����� ������
		ULONG Valid;
		//! ����� ���� � ������� �����
		ULONG BrackInDaStreet;
		//! ��� ����
		TIniFile *ini;

};
#endif
