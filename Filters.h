//---------------------------------------------------------------------------

#ifndef FiltersH
#define FiltersH


#include <vector>
#include "Result.h"

//! @brief Тип для функции фильтрации по Чебышеву
//! @param data - массив с сигналом
//! @param N - размер массива
//! @param order - порядок фильтра
//! @param sampleRate - частота сбора
//! @param cutoffFrequency - частота среза
//! @param centerFrequency частота центра фильтра
//! @param widthFrequency ширина фильтра
//! @param rippleDb пульсации в полосе пропускания
//! @param type - тип фильтра 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
typedef void (*_ChebyshevI)(double *data,int N,
						   int order,
						   double sampleRate,
						   double cutoffFrequency,
						   double centerFrequency,
						   double widthFrequency,
						   double rippleDb,
						   int type);

//! @brief Тип для функции фильтрации по Баттерворту
//! @param data - массив с сигналом
//! @param N - размер массива
//! @param order - порядок фильтра
//! @param sampleRate - Частота сбора
//! @param cutoffFrequency - частота среза
//! @param centerFrequency - частота центра фильтра
//! @param widthFrequency - ширина фильтра
//! @param type - тип фильтра 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
typedef void (*_Butterworth)(double *data,
							 int N,
							 int order,
							 double sampleRate,
							 double cutoffFrequency,
							 double centerFrequency,
							 double widthFrequency,
							 int type);


//! @brief Тип для функции фильтрации по Баттерворту
//! @param data - массив с сигналом
//! @param N - размер массива
//! @param order - порядок фильтра
//! @param sampleRate - Частота сбора
//! @param cutoffFrequency - частота среза
//! @param centerFrequency - Частота центра фильтра
//! @param widthFrequency - ширина фильтра
//! @param rippleDb - Пульсации в полосе пропускания
//! @param rolloff - пульсации в полосе подавления
//! @param type - тип фильтра 0 - LowPass,1 - HighPass,2 - BandPass,3 - BandStop
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
		//! Конструктор
		Filters();
		//! @brief Загружает настройки из файла
		//! @param type "Pp" или "Pr"
		void setSettingsFromIniFile(String type);
		//! Фильтрует полученный сигнал
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


