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

//! Класс результата дефектоскопии
class Result
{
friend class ButterworthFilter;
protected:
	//! Форма для отрисовки измерений по датчику (0-сигнал по модулю, 1-исходный сигнал)
	TForm *SensorForm[2];
	//! Чарт для отрисовки измерений по датчику
	TChart *SensorChart[2];
	//! Указатель расстояния от начала трубы
	TStatusBar *SensorBar[2];

	//! массив итоговых значений по датчикам
	vector < vector <double> > sensor_data;
	//! массив всех значений - по всем датчикам , по всем зонам
	vector < vector < vector < double > > > data;
	//! кол-во измерений второй мертвой зоны
	double meas_dead_zone_finish;
	//! кол-во измерений первой мертвой зоны
	double meas_dead_zone_start;
	//! вектор усилений для датчиков
	vector < double > gain;

private:
	//! Инициализация переменных нужных как для Result,так и для потомков
	void Init();
	//! вектор отфильтрованых значений - по всем датчикам , по всем зонам [зона][датчик][измерение]
	vector < vector < vector < double > > > Filtered_Data;
	//! Вектор исходных значений - по всем датчикам , по всем зонам [зона][датчик][измерение]
	vector < vector < vector < double > > > Source_Data;
	//! Вспомогательная функция для фильтрации
	void _medianfilter(double* signal, double* result, int N);
	//! Ширина окна для медианной фильтрации
	int WidthMedianFilter;
	//! если 0 то простой просмотр, если 1 исходный
	int TypeView;
	//! зона начала вывода результата
	int shiftZone;
	//! номер датчика
	int sens;
	//! зона начала вывода результата для обсчета СтатусБара
	int startZone;
	//! Количество зон вывода результата
	int countZone;
	//! Длина зоны
	int zone_length;


public:
	//! Дефолтный конструктор для наследников
	Result();
	//! Конструктор
	Result(int sensors);
	//! Деструктор
	~Result();
	//! @brief Медианный фильтр
	//! @param signal - исходный сигнал
	//! @param result - отфильтрованый сигнал
	//! @param N - длина массива
	void medianfilter(double* signal, double* result, int N);
	//! Кол-во отображаемых зон в результате
	short zones;
	//! Кол-во отображаемых сенсоров (датчиков)
	short sensors;
	//! Кол-во измерений в 1 зоне 1 датчике
	int meas_per_zone;
	//! Мертвая зона в начале трубы(mm.)
	int dead_zone_start;
	//! Мёртвая зона в конце трубы(mm.)
	int dead_zone_finish;
	//! Общее кол-во измерений по одному датчику
	long total_meas;
	//! Цвет мертвой зоны
	TColor Dead;
	//! Цвет брака
	TColor Brack;
	//! Цвет второго класса
	TColor Class2;
	//! Цвет годной, хорошей трубы
	TColor Good;
	//! массив порогов - [0] - порог брака, [1] - порог класса 2  (%)
	DynamicArray <double> borders;
	//! массив итоговых значений по зонам
	DynamicArray <double> zone_data;
	//! массив количество измерений на датчик в зоне (ЛИРы)
	DynamicArray <int> meas_per_sensor;
	//! Распределяет данные по вектору data из буфера АЦП
	void FillData(vector < vector < double > > sourceData);
	//! считает данные, заполняя массивы "zone_data" и "sensor_data"
	virtual void ComputeZoneData(bool isLinear=false);
	//! возвращает цвет измерения/датчика/зоны
	TColor ZoneColor(double meas,int XCoord = -1);
	//! @brief отрисовывает результаты на чарте
	//! @param C - chart на который нужно вывести данные
	//! @param Owner - форма на которой лежит чарт
	virtual void PutResultOnChart(TChart *C, TComponent *Owner);
	//! @brief Выводит сырой и фильтрованный сигнал
	//! @param C - chart на который нужно вывести данные
	//! @param Owner - форма на которой лежит чарт
	//! @param zone - зона
	//! @param sensor - датчик
	void PutSourceDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);
	//! @brief Выводит результирующий сигнал
	//! @param C - chart на который нужно вывести данные
	//! @param Owner - форма на которой лежит чарт
	//! @param zone - зона
	//! @param sensor - датчик
	void PutDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);
	//! @brief Выводит сырой и фильтрованный сигнал
	//! @param C - chart на который нужно вывести данные
	//! @param Owner - форма на которой лежит чарт
	//! @param zone1 - правая зона-граница
	//! @param zone2 - левая зона-граница
	//! @param sensor - датчик
	void PutSourceDataOnChartMas(TChart *C,TComponent *Owner, int zone1, int zone2, int sensor);
	//! @brief Выводит результирующий сигнал
	//! @param C - chart на который нужно вывести данные
	//! @param Owner - форма на которой лежит чарт
	//! @param zone1 - правая зона-граница
	//! @param zone2 - левая зона-граница
	//! @param sensor - датчик
	void PutDataOnChartMas(TChart *C,TComponent *Owner, int zone1, int zone2, int sensor);
	//! @brief Разбирает собранные платой L791 значения по зонам и датчикам (старые проекты)
	//! @param _sensors - всего датчиков
	//! @param DefSensors - датчиков по дефектоскопии
	//! @param MeasOutCrossTrash - кол-во измерений которые не нужно учитывать (для поперечного)
	void PrepareData(int _sensors,int DefSensors, int MeasOutCrossTrash = 0);
	//! Двойной клик по датчику открывает окно с измерениями
	void __fastcall SeriesDblClick(TChartSeries *Sender,
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);
	//! Шифт+клик по датчику и шифт+клик по второму датчику - открывает окно с измерениями на все промежутке
	void __fastcall SeriesShiftClick(TChartSeries *Sender,
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);
	//! Рассчет расстояния от начала трубы в окне с измерениями
	void __fastcall ChartClickBackground(TCustomChart *Sender,
		  	TMouseButton Button, TShiftState Shift, int X, int Y);
	//! Обработчик события "нажал Esc - окно закрыл"
	void __fastcall EscapeKeyPress(TObject *Sender, wchar_t &Key);
	//! Сохраняем данные по трубе в файл (текстовый)
	virtual void SaveTubeToFile(UnicodeString FileName);
	//! Загрузка данных по трубе из файла (текстового)
	virtual void LoadTubeFromFile(FILE *file);
	//! Удалить все измерения
	void DeleteData();
	//! Устанавливает все усиоления датчиков
	void SetGains(vector<double> _Gain);
	//! Устанавливает усиление 1 конкртеного датчика
	void SetSensorGain(double _gain, int sensor);
	//! Высчитывает мертвые зоны
	void CalculateDeadZone(int LengthZone);
	//! Считает мертвую зону в начале
	void CalculateDeadFront(int LengthZone);
	//! Считает мертвую зону в конце
	void CalculateDeadBack(int LengthZone);
	//! Добавляет рассчитанную зону в реальном времени
	//! Вернет True если зона бракованая, иначе false
//	bool AddZone( vector< vector < double > > new_data);
	bool AddZone( vector< vector < double > > new_data,bool isLinear = false);
	//! Автоматичекая калибровка поперечных датчиков
	void AutoCalibration(int zone);

};
//------------------------------------------------------------------------------

//! Наследник класса Result - результат по толщине
class ThicknessResult : public Result
{
public:
	//! конструктор
	ThicknessResult();
	//! Деструктор
	~ThicknessResult();
	//! создает тестовую инфу (для отладки)
	void ComputeZoneData();
	//! возвращает цвет зоны
	TColor ZoneColor(double meas);
	//! отрисовывает результат на определенном чарте
	void PutResultOnChart(TChart *Chart, TComponent *Owner);
	//! сохраняет данные в файл (только zone_data)
	void SaveTubeToFile(UnicodeString FileName);
	//! загружает данные из файла (zone_data)
	void LoadTubeFromFile(FILE*file);
};
//--------------------------------------------------------------------------------

//! Наследник класса Result - суммарный результат по всем модулям
class SummaryResult : public Result
{
public:
	//! Конструктор
	SummaryResult();
	//! Деструктор
	~SummaryResult();
	//! номер зоны реза 1 и 2
	short cut1,cut2;
	//! минимальный годный участок
	short min_good_length;
	//! отрезок с обоих концов трубы
	short end_cut;
	//! решение о трубе (Годно, Брак, Класс 2)
	String decision;
	//! Считает суммарный результат по всем модулям, заполянет массив zone_data
	void ComputeZonesData();
	//! Возвращает цвет зоны
	TColor ZoneColor(double meas);
	// Рисует результат на чарте
	void PutResultOnChart(TChart *Chart, TComponent *Owner);
	//! Принимает решение по всей трубе и вычисляет зоны отреза
	void MakeDecision();
	//! добавляет зону в реальном времени
	void AddZone();
};

//! Singleton обертка для Result и его наследников, для гарантии существования одного экземпляра объекта
class Singleton
{
private:
	static Singleton* _self;
	Singleton();
	virtual ~Singleton();

public:
	//! Объект результата поперечного модуля
	Result *CrossResult;
	//! Объект результата продольного модуля
	Result *LinearResult;
	//! Объект результата толщиномера
	ThicknessResult *ThResult;
	//! Сумарный результат
	SummaryResult *SumResult;
	//! Экземпляр класса
	static Singleton* Instance();
	//! Удаление экземпляра класса
	static void deleteInstance();
};



//---------------------------------------------------------------------------
#endif
