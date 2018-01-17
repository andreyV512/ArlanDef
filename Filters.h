//---------------------------------------------------------------------------

#ifndef FiltersH
#define FiltersH


#include <vector>
#include "Result.h"

//! @brief ��� ��� ������� ���������� �� ��������
//! @param data - ������ � ��������
//! @param N - ������ �������
//! @param order - ������� �������
//! @param sampleRate - ������� �����
//! @param cutoffFrequency - ������� �����
//! @param centerFrequency ������� ������ �������
//! @param widthFrequency ������ �������
//! @param rippleDb ��������� � ������ �����������
//! @param type - ��� ������� 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
typedef void (*_ChebyshevI)(double *data,int N,
						   int order,
						   double sampleRate,
						   double cutoffFrequency,
						   double centerFrequency,
						   double widthFrequency,
						   double rippleDb,
						   int type);

//! @brief ��� ��� ������� ���������� �� �����������
//! @param data - ������ � ��������
//! @param N - ������ �������
//! @param order - ������� �������
//! @param sampleRate - ������� �����
//! @param cutoffFrequency - ������� �����
//! @param centerFrequency - ������� ������ �������
//! @param widthFrequency - ������ �������
//! @param type - ��� ������� 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
typedef void (*_Butterworth)(double *data,
							 int N,
							 int order,
							 double sampleRate,
							 double cutoffFrequency,
							 double centerFrequency,
							 double widthFrequency,
							 int type);


//! @brief ��� ��� ������� ���������� �� �����������
//! @param data - ������ � ��������
//! @param N - ������ �������
//! @param order - ������� �������
//! @param sampleRate - ������� �����
//! @param cutoffFrequency - ������� �����
//! @param centerFrequency - ������� ������ �������
//! @param widthFrequency - ������ �������
//! @param rippleDb - ��������� � ������ �����������
//! @param rolloff - ��������� � ������ ����������
//! @param type - ��� ������� 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
typedef void (*_Elliptic)(double *data,
						  int N,
						  int order,
						  double sampleRate,
						  double cutoffFrequency,
						  double centerFrequency,
						  double widthFrequency,
						  double rippleDb,
						  double rolloff,
						  int type);
enum filterTypes {Butterworth = 0, Chebyshev = 1, Elliptic = 2};
enum filterSubTypes {lowPass = 0, highPass = 1, bandPass = 2, bandStop = 3};

struct filterSettings
{
	filterTypes CurrentType;
	filterSubTypes CurrentSubType;
	int order;
	double sampleRate;
	double cutoffFrequency;
	double centerFrequency;
	double widthFrequency;
	double rippleDb;
	double rolloff;
};

class Filters
{
	public:
		//! �����������
		Filters();
		//! @brief ��������� ��������� �� �����
		//! @param type "Pp" ��� "Pr"
		void setSettingsFromIniFile(String type);
		//! ��������� ���������� ������
		void toFilter(double *data, int N);
	private:
		filterSettings settings;
		HINSTANCE 	dllInstance;
		_ChebyshevI  filter_chebyshev;
		_Elliptic    filter_elliptic;
		_Butterworth filter_butterworth;

};
extern Filters *CrossFilter;
extern Filters *LinearFilter;

#endif


