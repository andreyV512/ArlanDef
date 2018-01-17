//---------------------------------------------------------------------------

#ifndef ThreadOnLineH
#define ThreadOnLineH

#include <vector>
#include "IniFiles.hpp"

#include "Global.h"
#include "Main.h"
#include "Result.h"
#include "ComPort.h"
#include "Protocol.h"
#include "Spectroscope.h"

//---------------------------------------------------------------------------
class Advantech1784;
//! @brief ����� ��� ������ ������ � �������� �������
//! ��� �������� �� ����������� ����� ����� ������ �������������� � �����
class ThreadOnLine: public TThread
{
	private:
		int pauseWorkSpeed;
		int pauseStop;
		void __fastcall Execute();
		//! ���������� �� ���������� (������ ����������)
		bool Cross;
		//! ���������� �� ����������
		bool Linear;
		//! ���������� �� �����������
		bool Spectro;
		//! ������ ���������� ���������
		void __fastcall DrawResultPP();
		//! ������ ���������� ���������
		void __fastcall DrawResultPR();
		//! �������������� ��� ������� ������� �����
		void __fastcall RedrawGraphics();
		//! @brief ��� ���������, ��������� �� ������ (�-����)
		//! ������ �������� �������� �� ��������
		vector< vector < double > > ppData;
		//! ������ �������� �������� �� ��������
		vector< vector < double > > prData;
		//! ���-�� ��������� ������ �� �����������
		__int64 CollectedMeasurements_pp;
		//! ���-�� ��������� ������ �� �����������
		__int64 CollectedMeasurements_pr;
		//! ����� ��� ������� � ���������� � ������ 1
		String stext1;
		//! ����� ��� ������� � ���������� � ������ 2
		String stext2;
		//! ���������� �������� ��������� ������� ����� �������
		UnicodeString PrepareForWork( );
		//! ��������� ����� (���������� �������� � �.�.)
		void Finally();
		void ErrFinally(AnsiString _msg,bool* _Collect,bool* _result);
		//! ��������� � �����
		bool Solenoid_ok;
		//! ��������� �� ������� ���� �����������
		bool CurrentZoneCrossResult;
		//! ��������� �� ������� ���� �����������
		bool CurrentZoneLinearResult;
		//�������� �������� ����, ������, �����
//vector<int> speeds;
		int InSpeed;
		int WorkSpeed;
		int OutSpeed;
		//! ��������� ���������� ������� �����, ������������ �������
		void __fastcall UpdateStatus();
//		void __fastcall GetTubeNumber();
//		void __fastcall ClearCharts();
		void __fastcall NextTube(void);
		//! ����� ��������� ���� ���� ����������
		bool OnlineCycle(void);
		bool OnlineCycleTransit(void);
		int delay;
	public:
		//! �����������
		//! @param *_Counter ����� Advantech1784
		//! @param *St ��������� �� ���������, ������������ ������� ��������
		//! @param _Thick ���������� �� �������������
		//! @param _Linear ���������� �� ����������
		//! @param _Spectro ���������� �� �����������
		//! @param &alth ����� ����� ������
		__fastcall ThreadOnLine(bool CreateSuspended,
								bool _Linear,
								bool _Spectro,
								Spectroscope *_Spectroscope);
		//! ������� ��������� ������
		bool Collect;
		//! ������ Return Value, ������� �� ��������
		bool cool;
		Spectroscope *spectroscope;
};
//---------------------------------------------------------------------------

#endif
