//---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH

#include "Classes.hpp"
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "Chart.hpp"
#include "LCard502.h"
#include "Filters.h"
#include "Protocol.h"


using namespace std;

//! ����� ���������� �������������
class Result
{
friend class ButterworthFilter;
protected:
	//! ����� ��� ��������� ��������� �� ������� (0-������ �� ������, 1-�������� ������)
	TForm *SensorForm[2];
	//! ���� ��� ��������� ��������� �� �������
	TChart *SensorChart[2];
	//! ��������� ���������� �� ������ �����
	TStatusBar *SensorBar[2];

	//! ������ �������� �������� �� ��������
	vector < vector <double> > sensor_data;
	//! ������ ���� �������� - �� ���� �������� , �� ���� �����
	vector < vector < vector < double > > > data;
	//! ���-�� ��������� ������ ������� ����
	double meas_dead_zone_finish;
	//! ���-�� ��������� ������ ������� ����
	double meas_dead_zone_start;
	//! ������ �������� ��� ��������
	vector < double > gain;

private:
	//! ������������� ���������� ������ ��� ��� Result,��� � ��� ��������
	void Init();
	//! ������ �������������� �������� - �� ���� �������� , �� ���� ����� [����][������][���������]
	vector < vector < vector < double > > > Filtered_Data;
	//! ������ �������� �������� - �� ���� �������� , �� ���� ����� [����][������][���������]
	vector < vector < vector < double > > > Source_Data;
	//! ��������������� ������� ��� ����������
	void _medianfilter(double* signal, double* result, int N);
	//! ������ ���� ��� ��������� ����������
	int WidthMedianFilter;
	//! ���� 0 �� ������� ��������, ���� 1 ��������
	int TypeView;
	//! ���� ������ ������ ����������
	int shiftZone;
	//! ����� �������
	int sens;
	//! ���� ������ ������ ���������� ��� ������� ����������
	int startZone;
	//! ���������� ��� ������ ����������
	int countZone;
	//! ����� ����
	int zone_length;


public:
	//! ��������� ����������� ��� �����������
	Result();
	//! �����������
	Result(int sensors);
	//! ����������
	~Result();
	//! @brief ��������� ������
	//! @param signal - �������� ������
	//! @param result - �������������� ������
	//! @param N - ����� �������
	void medianfilter(double* signal, double* result, int N);
	//! ���-�� ������������ ��� � ����������
	short zones;
	//! ���-�� ������������ �������� (��������)
	short sensors;
	//! ���-�� ��������� � 1 ���� 1 �������
	int meas_per_zone;
	//! ������� ���� � ������ �����(mm.)
	int dead_zone_start;
	//! ̸����� ���� � ����� �����(mm.)
	int dead_zone_finish;
	//! ����� ���-�� ��������� �� ������ �������
	long total_meas;
	//! ���� ������� ����
	TColor Dead;
	//! ���� �����
	TColor Brack;
	//! ���� ������� ������
	TColor Class2;
	//! ���� ������, ������� �����
	TColor Good;
	//! ������ ������� - [0] - ����� �����, [1] - ����� ������ 2  (%)
	DynamicArray <double> borders;
	//! ������ �������� �������� �� �����
	DynamicArray <double> zone_data;
	//! ������ ���������� ��������� �� ������ � ���� (����)
	DynamicArray <int> meas_per_sensor;
	//! ������������ ������ �� ������� data �� ������ ���
	void FillData(vector < vector < double > > sourceData);
	//! ������� ������, �������� ������� "zone_data" � "sensor_data"
	virtual void ComputeZoneData(bool isLinear=false);
	//! ���������� ���� ���������/�������/����
	TColor ZoneColor(double meas,int XCoord = -1);
	//! @brief ������������ ���������� �� �����
	//! @param C - chart �� ������� ����� ������� ������
	//! @param Owner - ����� �� ������� ����� ����
	virtual void PutResultOnChart(TChart *C, TComponent *Owner);
	//! @brief ������� ����� � ������������� ������
	//! @param C - chart �� ������� ����� ������� ������
	//! @param Owner - ����� �� ������� ����� ����
	//! @param zone - ����
	//! @param sensor - ������
	void PutSourceDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);
	//! @brief ������� �������������� ������
	//! @param C - chart �� ������� ����� ������� ������
	//! @param Owner - ����� �� ������� ����� ����
	//! @param zone - ����
	//! @param sensor - ������
	void PutDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);
	//! @brief ������� ����� � ������������� ������
	//! @param C - chart �� ������� ����� ������� ������
	//! @param Owner - ����� �� ������� ����� ����
	//! @param zone1 - ������ ����-�������
	//! @param zone2 - ����� ����-�������
	//! @param sensor - ������
	void PutSourceDataOnChartMas(TChart *C,TComponent *Owner, int zone1, int zone2, int sensor);
	//! @brief ������� �������������� ������
	//! @param C - chart �� ������� ����� ������� ������
	//! @param Owner - ����� �� ������� ����� ����
	//! @param zone1 - ������ ����-�������
	//! @param zone2 - ����� ����-�������
	//! @param sensor - ������
	void PutDataOnChartMas(TChart *C,TComponent *Owner, int zone1, int zone2, int sensor);
	//! @brief ��������� ��������� ������ L791 �������� �� ����� � �������� (������ �������)
	//! @param _sensors - ����� ��������
	//! @param DefSensors - �������� �� �������������
	//! @param MeasOutCrossTrash - ���-�� ��������� ������� �� ����� ��������� (��� �����������)
	void PrepareData(int _sensors,int DefSensors, int MeasOutCrossTrash = 0);
	//! ������� ���� �� ������� ��������� ���� � �����������
	void __fastcall SeriesDblClick(TChartSeries *Sender,
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);
	//! ����+���� �� ������� � ����+���� �� ������� ������� - ��������� ���� � ����������� �� ��� ����������
	void __fastcall SeriesShiftClick(TChartSeries *Sender,
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);
	//! ������� ���������� �� ������ ����� � ���� � �����������
	void __fastcall ChartClickBackground(TCustomChart *Sender,
		  	TMouseButton Button, TShiftState Shift, int X, int Y);
	//! ���������� ������� "����� Esc - ���� ������"
	void __fastcall EscapeKeyPress(TObject *Sender, wchar_t &Key);
	//! ��������� ������ �� ����� � ���� (���������)
	virtual void SaveTubeToFile(UnicodeString FileName);
	//! �������� ������ �� ����� �� ����� (����������)
	virtual void LoadTubeFromFile(FILE *file);
	//! ������� ��� ���������
	void DeleteData();
	//! ������������� ��� ��������� ��������
	void SetGains(vector<double> _Gain);
	//! ������������� �������� 1 ����������� �������
	void SetSensorGain(double _gain, int sensor);
	//! ����������� ������� ����
	void CalculateDeadZone(int LengthZone);
	//! ������� ������� ���� � ������
	void CalculateDeadFront(int LengthZone);
	//! ������� ������� ���� � �����
	void CalculateDeadBack(int LengthZone);
	//! ��������� ������������ ���� � �������� �������
	//! ������ True ���� ���� ����������, ����� false
//	bool AddZone( vector< vector < double > > new_data);
	bool AddZone( vector< vector < double > > new_data,bool isLinear = false);
	//! ������������� ���������� ���������� ��������
	void AutoCalibration(int zone);

};
//------------------------------------------------------------------------------

//! ��������� ������ Result - ��������� �� �������
class ThicknessResult : public Result
{
public:
	//! �����������
	ThicknessResult();
	//! ����������
	~ThicknessResult();
	//! ������� �������� ���� (��� �������)
	void ComputeZoneData();
	//! ���������� ���� ����
	TColor ZoneColor(double meas);
	//! ������������ ��������� �� ������������ �����
	void PutResultOnChart(TChart *Chart, TComponent *Owner);
	//! ��������� ������ � ���� (������ zone_data)
	void SaveTubeToFile(UnicodeString FileName);
	//! ��������� ������ �� ����� (zone_data)
	void LoadTubeFromFile(FILE*file);
};
//--------------------------------------------------------------------------------

//! ��������� ������ Result - ��������� ��������� �� ���� �������
class SummaryResult : public Result
{
public:
	//! �����������
	SummaryResult();
	//! ����������
	~SummaryResult();
	//! ����� ���� ���� 1 � 2
	short cut1,cut2;
	//! ����������� ������ �������
	short min_good_length;
	//! ������� � ����� ������ �����
	short end_cut;
	//! ������� � ����� (�����, ����, ����� 2)
	String decision;
	//! ������� ��������� ��������� �� ���� �������, ��������� ������ zone_data
	void ComputeZonesData();
	//! ���������� ���� ����
	TColor ZoneColor(double meas);
	// ������ ��������� �� �����
	void PutResultOnChart(TChart *Chart, TComponent *Owner);
	//! ��������� ������� �� ���� ����� � ��������� ���� ������
	void MakeDecision();
	//! ��������� ���� � �������� �������
	void AddZone();
};

//! Singleton ������� ��� Result � ��� �����������, ��� �������� ������������� ������ ���������� �������
class Singleton
{
private:
	static Singleton* _self;
	Singleton();
	virtual ~Singleton();

public:
	//! ������ ���������� ����������� ������
	Result *CrossResult;
	//! ������ ���������� ����������� ������
	Result *LinearResult;
	//! ������ ���������� �����������
	ThicknessResult *ThResult;
	//! �������� ���������
	SummaryResult *SumResult;
	//! ��������� ������
	static Singleton* Instance();
	//! �������� ���������� ������
	static void deleteInstance();
};



//---------------------------------------------------------------------------
#endif
