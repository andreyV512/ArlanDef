//---------------------------------------------------------------------------

#ifndef ReportsH
#define ReportsH

#include "DataModule.h"
#include "Comobjec.h"
#include "Global.h"
#include <vector>

//! ����� ���������� �� ����� ������ � Excel
class Reports
{
	public:
		//! �����������
		Reports();
		//! ����������
		~Reports();
		//! @brief ������� ������ �� ������ �������� �������� ������
		//! @param begin - ���� ������
		//! @param End - ���� ���������
		//! @param SG - ������ ���������
		//! @param Result - ��������� (����,�����,����� 2)
		//! @param TS - ����������
		void ExportAllTubes(TDateTime Begin, TDateTime End,UnicodeString SG ,UnicodeString Result,UnicodeString TS);
		void ExportTube(ULONG ID);                            	//��������� ����� �����
		//! ������ Application
		Variant App;
		//!������ ����
		Variant Sh;
		//! ������ ������(������� �����)
		Variant Rang;
	private:
		void DrawExcelAll();
		void DrawExcelZone();
		void ExcelInit(UnicodeString File);                  		//�������������� ��������
		void ToExcelCell(int Row, int Column, Variant data);        //����� ������ � ������
		void StrToExcelCell(int Row, int Column, AnsiString data);  //����� ������ � ������
		TIniFile *Ini;                      						//���� ��������
		bool Opened;                                                //���� ��� �������� �����
		String AntiGovno(String Res);
};

#endif
