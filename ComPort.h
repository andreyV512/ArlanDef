//---------------------------------------------------------------------------

#ifndef ComPortH
#define ComPortH

#include "Global.h"
#include "clustering.h"
#include "Result.h"
#include "Main.h"

#include "slycomm\CommPort.h"
#pragma link "CommPort"

//---------------------------------------------------------------------------
//! ������� ��� COM �����, ����������� ����� ���������� ������� �������������� �������
typedef void (__closure *OnStatusDef)(AnsiString _msg);
class MyComPort
{
private:

// ��������� ��������
	//! ��� ����� (COM1, COM2 ...)
	String PortName;
	//! ��� ���� (��������)
	int BaudRate;
	//! ���� ������
	int DataBits;
	//! �������� ����
	int StopBits;
	//! ��������
	int Parity;

// ���������� ����������
	//! ����� �������� (��� ������ ��������)
	int ASUMode;
	//! ����� ������� �� ��� (5 ��� 17)
	int length;
	//! ����� �������� � ���� (������ const)
	int abonent;
	//! ������� ������	(1, 2 ��� 5)
	int function;
	//! �������� ������ � ����
	unsigned char data[50];
	//! ����� �������� ������
	int data_size;
	//! ������� - ������ ��������
	TEvent* DataReceived;
	//! ������� ��� ����� :)
	TCommPort *CommPort;

// ���������� �������
	//! ���������� ��������� COM �����
	void SetComPortSettings();
	//! ������ �� ������� ���������	(����-��)
	AnsiString GetTubeNumber_2(bool* _transit);
	//! ��������� ���������� ������ (������ 2-�� �������� )
	bool CheckSentMessage();

// ������ ����������� �� ����������, �����
	//! ��������� ���������� �������� �� ����������� �������
	unsigned char EliseevRes(int zone);
	//! ���� ���� �� ��������������
	int plant_cut;
//	Result pp2;
//	Result pr2;
//	ThicknessResult th2;
//	SummaryResult sum2;
	//! ������� ��� ���������� �����������
//	TColor Good;
//	TColor Class2;
//	TColor Brack;

	//! ����� �������� ������ �����
	UINT TubeNumTimeout;
	//! ����� �������� ����������
	UINT ResultTimeout;
	//! ����� �������� �������� �������
	UINT TestTimeout;
	//! ����� �������� ������� � ������� ���������
	UINT SolidGroupTimeout;

	void Create();
	MyComPort();
	void Status(AnsiString _msg);
public:
	//! ����������� ������ ������
	MyComPort(AnsiString _ini_section);
	//! ����������
	~MyComPort();

	//! ������� - ������ �������
	void __fastcall MyComPortDataReceived(TObject *Sender, DWORD Count);
	//! ������ ����������� ����� �� 1 ���������
	unsigned char countCRC16_1(unsigned char* buff, unsigned int size);
	//! ������ ����������� ����� �� 2 ���������
	unsigned short countCRC16_2(unsigned char* bufData, unsigned int sizeData);
	//! ���� ������ �����
	bool TestConnection();
	//! ������ ������ ����� (���� �����)
	AnsiString GetTubeNumber(bool* _transit);
	//! @brief ������� ���������� (���� �����)
	//! @param res ����� ��������� (����� / ����)
	//!	@param c1,c2 ���� ���� 1, 2 (������ � ����� �����)
	//! @param len ����� ����� � �����
	//! @param sgg ������ ��������� (1 ����� ���������)
	bool SendResultToASU(short res, short c1, short c2, short len, SolidGroups::Group sgg);		// 1 - ���������
//	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg);	// 2 - ����-��
	//! @brief ������� ���������� (���� �����)
	//! @param solid_num ������ ���������, ���������� ������
	bool SendResultToASU(int solid_num);	// 2 - ����-��
	//! ������� ������ ��������� (���� �����)
	SolidGroups::Tube GetSolidGroup();
	//! ���� ���� �� ��������������
	inline int GetPlantCut() { return plant_cut;}
	OnStatusDef OnStatus;
	inline bool IsOpened(void){return(CommPort->Open);}
	bool SendToProtocol();
};

//---------------------------------------------------------------------------
// ���������� ���������, ����������� ��������� ��������� �� ����� ����� ���������.
extern MyComPort *MyCom;
//---------------------------------------------------------------------------

#endif
