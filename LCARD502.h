//---------------------------------------------------------------------------

#ifndef LCARD502H
#define LCARD502H
#include <vector>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "l502api.h"
#include "vcl.h"
#include "Protocol.h"
#include "IniFiles.hpp"
#include <map>
#pragma comment(lib, "l502api.lib")
//------------------------------------------------------------------------------
using namespace std;
class L502_ProcessThread;
//! Структура настроек для одного логического канала платы LCard502
struct Channel_parameters
{
	Channel_parameters()
	{
		range = 0;
		logicalChannel =0;
		collectedMode = 0;
    }
	//! @brief Входной диапазон
	//! @li 0 Диапазон +/-10V
	//! @li 1 Диапазон +/-5V
	//! @li 2 Диапазон +/-2V
	//! @li 3 Диапазон +/-1V
	//! @li 4 Диапазон +/-0.5V
	//! @li 5 Диапазон +/-0.2V
	uint32_t range;
	//! Номер логического канала 0-31
	uint32_t logicalChannel;
	//! @brief Режим сбора
	//! @li 0 Измерение напряжения относительно общей земли
	//! @li 1 Дифференциальное измерение напряжения
	//! @li 2 Измерение собственного нуля
	uint32_t collectedMode;

};
//! Структура с параметрами АЦП LCard502
struct LCard_parameters
{
	//! ТаймАут для сбора (с каким периодом будем скидывать данные из ацп в большой буфер,мс)
	int RECV_TOUT;
	//! @brief Режим синхронизации
	//! @li 0 Внутренний сигнал
	//! @li 1 От внешнего мастера по разъему синхронизации
	//! @li 2 По фронту сигнала DI_SYN1
	//! @li 3 По фронту сигнала DI_SYN2
	//! @li 6 По спаду сигнала DI_SYN1
	//! @li 7 По спаду сигнала DI_SYN2
	uint32_t syncMode;
	//! Источник запуска синхронного ввода/вывода см. режимы @link syncMode
	uint32_t syncStartMode;
	//! Частота сбора (Гц.)
	double frequencyCollect;
	//! Частота на канал (Гц.)
	double frequencyPerChannel;
	//! Возвращает кол-во используемых каналов
	int getCountChannels(){return channels.size()+others.size();}
	//! Индивидуальные настройки для каждого канала
	vector< Channel_parameters > channels;
	//! @brief Дополнительные каналы
	//! @li 0 - Ток продольный соленоид
	//! @li 1 - Напряжение продольный соленоид
	//! @li 2 - Ток поперечный соленоид
	//! @li 3 - Напряжение поперечный соленоид
	//! @li 4 - Ток группы прочности
	//! @li 5 - Напряжение группы прочности
	//! @li 6 - Разделение по зонам
	vector< Channel_parameters > others;
	//! Конструктор
	LCard_parameters(){	others.resize(7);}
};
//! @brief Класс для работы с платой АЦП LCard502
class LCard502
{
	public:
		//! Конструктор
		LCard502();
		//! Деструктор
		~LCard502();
		//! @brief инициализирует плату АЦП
		//! @param serial_number серийный номер платы,
		//! Cписок номеров в системе можно узнать с помощью ф. @link getDeviceList();
		bool Initialization(String serial_number);
		//! @brief Старт сбора данных
		//! @param isTest - флаг тестового сбора, по умолчанию false
		//! @retval Ноль в случае успеха, иначе код ошибки
		int Start(bool isTest_ = false);
		//! Стоп сбора
		bool Stop();
		//! Показывает начат ли сбор данных с платы
		bool isStarted() {return threadRunning;}
		//! @brief Устанавливает настройки по умолчанию
		//! @retval Ноль в случае успеха, иначе код ошибки
		__int32 setSettings();
		//! @brief Устанавливает настройки
		//! @param parameters - структура с параметрами настроек LCard_parameters
		//! @retval Ноль в случае успеха, иначе код ошибки
		__int32 setSettings(LCard_parameters &parameters);
		//! Устанавливает настройки загружая их из ini-файла
		__int32 setSettingsFromIniFile();
		//! Засекает кол-во данных при старте продольного
		void LinearIsStarted();
		//! Засекает кол-во данных при старте поперечного
		void CrossIsStarted();
		//! Засекает кол-во данных при остановке продольного
		void LinearIsStopped();
		//! Засекает кол-во данных при остановке поперечного
		void CrossIsStopped();
		//! Засекает кол-во данных при старте группы прочности
		void SolidGroupIsStarted();

		//! Возвращает структуру с настройками
		LCard_parameters getSettings(){return Parameters;}
		//! Возвращает вектор обнаруженных плат
		vector < String > getDeviceList();
		//! Возвращает кол-во собранных данных
		__int64 getCountCollectedMeasurements();
		//! Возвращает размер выделенного буфера под данные
		__int32 getMaxBufferSize();
		//! @brief Вставляет данные в "большой" вектор
		//! @param data - вектор в котором содержатся данные
		//! @param size - кол-во элементов которые нужно скопировать (не всегда вектор заполнен полностью)
		void insertData(const vector<double> &data,__int32 size);
		//! Возвращает измерения для продольного модуля
		vector < vector < double> >  getLinearMeasure();
		//! Возвращает измерения для поперечного модуля
		vector < vector < double > > getCrossMeasure();
		//! @brief Возвращает измерения для продольного модуля с указанием начала и конца массива
		//! @param oldPosition - номер последнего измерения предыдущей зоны
		vector < vector < double> >  getLinearMeasure( __int64 oldPosition );
		//! @brief Возвращает измерения для поперечного модуля с указанием начала и конца массива
		//! @param oldPosition - номер последнего измерения предыдущей зоны
		vector < vector < double > > getCrossMeasure ( __int64 oldPosition );
		//! Возвращает значение определенного канала платы АЦП (одиночное измерение)
		vector<double> getFrameValue();
		//! Возвращает массив токов и напряжений для группы прочностей
		vector<double> getSolidGroupSignal();

	private:
		//! кол-во собранных измерений при старте продольного
		__int64 CountMeasurementsStartLinear;
		//! кол-во собранных измерений при старте поперечного
		__int64 CountMeasurementsStartCross;
		//! кол-во собранных измерений при остановке поперечного
		__int64 CountMeasurementsStoppedCross;
		//! кол-во собранных измерений при остановке продольного
		__int64 CountMeasurementsStoppedLinear;
		//! кол-во собранных измерений при старте группы прочности
		__int64 CountMeasurementsStartSolidGroup;
		//! Сколько данных собрали
		__int64 CountCollectedMeasurements;
		//! Карта для приведения вольт к процентам
		map<int,int> voltToPercent;
		//! тестовый сбор
		bool isTest;
		//! Сбор данных запущен
		bool threadRunning;
		//! Хендл платы
		t_l502_hnd handle;
		//! Поток сбора
		L502_ProcessThread *thread;
		//! Вектор данных в вольтах
		vector<double> SourceData;
		//! Настройки платы
		LCard_parameters Parameters;
		//! Выполняется по завершению потока сбора
		__fastcall void OnThreadTerminate(TObject *obj);
		//! @brief Возвращает вектор с данными
		//! @param firstSensorPosition - позиция 1го датчика модуля
		//! @param lastSensorPosition - позиция последнего датчика модуля
		//! @param StartPosition - начало отсчета
		//! @param StopPosition - конец  отсчета
		vector < vector < double > > getMeasure(int firstSensorPosition,
												int lastSensorPosition,
												__int64 StartPosition,
												__int64 StopPosition);
};
extern LCard502 *lcard;

//------------------------------------------------------------------------------
//! Наследник TThread поток  в котором собираются данные в LСard502
class L502_ProcessThread : public TThread
{
	protected:
		void __fastcall Execute();

	public:
		  //! Возвращает текст ошибки
		  String GetError();
		  //! остановит поток если stop = true
		  bool stop;
		  //! конструктор класса
		__fastcall L502_ProcessThread(bool CreateSuspended, t_l502_hnd hnd_);

	private:
	 //! Время сбора данных, если время истекло вернет столько сколько успел собрать
	 int RECV_TOUT;
	 //! Код ошибки при выполнении потока сбора
	 int err;
	 //! Размер буфера выделеного под данные
	 uint32_t bufferSize;
	 //! Хендл на плату LCard 502
	 t_l502_hnd hnd;
	 //! номер первого логического канала в буфере АЦП
	 uint32_t firstLch;
};

#endif
