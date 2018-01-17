// ---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include <Classes.hpp>
#include "clustering.h"
// ---------------------------------------------------------------------------
enum SGSource
{
	sgsLCard=0,sgsCOMPort=1,sgsNone=-1
};
// ! ����� ���������� ����������
class Globals
{
public:
	// ! @todo �������, ���������� ��� � ��������� ����� TypeSize, ��� � �����������
	// ! @brief ������� ���������� (����� ���� ����� ������: ��������, ������� ������)
	static String current_typesize;
	// ! ������� ������� (������ 60,73 ��� 89)
	static int current_diameter;
	// ! ���������� �� ��������
	static bool isView;
	// ! ��� � ���� � ����� � �����������
	static String IniFileName;
	// ! ��� � ���� � ����� � ��������� ���������
	static String SignalsFileName;
	// ! ���� � ����� � �������� ��� �� � ms-dos
	static String PathFile_threshold_sg;
	// ! ����� �����
	static AnsiString tube_number;
	// ! ����� �����
	static bool tube_transit;
	// ! ����� � ���������������� ������ ���������
	static SolidGroups::Tube tubeSG;

	// ! @deprecated ��������, ����� ��� ���������� �� �������
	// ! @brief ���� ���-�� ��������, �������� ��������, ����� ����������
	static const int max_sensors=16;
	// ! @deprecated ��������, ����� ��� ���������� �� �������
	// ! @brief ���� ���-�� ���, ��������, ����������
	static const int max_zones=60;
	// ! @deprecated ��������, ����� ��� ���������� �� �������
	// ! @brief ���� ���-�� ��������� � ����, ���� ����������
	static const int max_meas=10000;
	// ! ����� �������� ����������� �������� � �������
	static const int LinSensors=4;   //����� 20160427 ������ ����� 4
	// ! ������������ ����� �������� ����������� �������� � �������
	static const int Cross_sensors=12;
	// ! �������� ���������� � ������� �������� �������� �������
	static const int defaultRotParameter=4;
private:

};

String Globals::current_typesize=L"2";
int Globals::current_diameter=2;
bool Globals::isView=false;
String Globals::PathFile_threshold_sg="..\\..\\Settings\\";
String Globals::IniFileName="..\\..\\Settings\\SettingsDefectoscope.ini";
String Globals::SignalsFileName="..\\..\\Settings\\IO_Management.ini";
AnsiString Globals::tube_number="";
bool Globals::tube_transit;
SolidGroups::Tube Globals::tubeSG=SolidGroups::Tube();

// -------------------------------------------------------------------------------------------
// ! ����� ��������, ����������� � ������������ �������
class SystemConst
{
public:
	// ! ������ � ��������� ��������
	static bool isMedianFilter;
	// ! ����� � ��� ���� �� COM �����
	static bool ComWithASU;
	// ! �������� ������ ���������
	static SGSource SolidGroupSource;
	// !	������� �� �������� ������� � ���������� �������
	static bool HideInGain;
	// ! ������ � ������ ��� ���
	static bool IsOnline;
	// ! ������� ������ ��������� � ������� ����� ms-dos'������
	static bool isSolidGroupMS_DOS;
	// ! ������������ �� ������ ����������� ��� ����������� ��
	static bool isTempSensor;
	// ! ���� �� ������ ������ ���������
	static bool isSolidity;
	// ! ���� �� � ����������� �������� (�������� ����, ����������, �������������)
	static bool isLinearSolenoid;
	// ! ����� �� ����������� ������ ����������� �������� ��������
	static bool isCrossDigitalFilter;
	// ! ����� �� ����������� ������ ����������� �������� ��������
	static bool isLinearDigitalFilter;

	// ! ��������� ���������� �������������� �������
	static void SetSystemConstants();

};
bool SystemConst::isCrossDigitalFilter=true;
bool SystemConst::isLinearDigitalFilter=true;
bool SystemConst::isMedianFilter=false;
bool SystemConst::ComWithASU=false;
SGSource SystemConst::SolidGroupSource=sgsNone;
bool SystemConst::HideInGain=false;
bool SystemConst::IsOnline=true;
bool SystemConst::isSolidGroupMS_DOS=false;
bool SystemConst::isTempSensor=false;
bool SystemConst::isSolidity=false;
bool SystemConst::isLinearSolenoid=false;

// ---------------------------------------------------------------------------
#endif
