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
//! @brief Класс для режима работы в реальном времени
//! Все действия по прохождению трубы через модули обрабатываются в цикле
class ThreadOnLine: public TThread
{
	private:
		int pauseWorkSpeed;
		int pauseStop;
		void __fastcall Execute();
		//! Используем ли поперечный (всегда используем)
		bool Cross;
		//! Используем ли продольный
		bool Linear;
		//! Используем ли Спектроскоп
		bool Spectro;
		//! Рисует поперечный результат
		void __fastcall DrawResultPP();
		//! Рисует продольный результат
		void __fastcall DrawResultPR();
		//! Перерисовывает все графики главной формы
		void __fastcall RedrawGraphics();
		//! @brief все параметры, связанные со сбором (Л-Кард)
		//! массив итоговых значений по датчикам
		vector< vector < double > > ppData;
		//! массив итоговых значений по датчикам
		vector< vector < double > > prData;
		//! Кол-во собранных данных по поперечному
		__int64 CollectedMeasurements_pp;
		//! Кол-во собранных данных по продольному
		__int64 CollectedMeasurements_pr;
		//! текст для надписи в статусбаре в панели 1
		String stext1;
		//! Текст для надписи в статусбаре в панели 2
		String stext2;
		//! Выставляем исходное положение модулей перед работой
		UnicodeString PrepareForWork( );
		//! Финальные штуки (отключение сигналов и т.д.)
		void Finally();
		void ErrFinally(AnsiString _msg,bool* _Collect,bool* _result);
		//! Соленоиды в норме
		bool Solenoid_ok;
		//! Результат по текущей зоне поперечного
		bool CurrentZoneCrossResult;
		//! Результат по текущей зоне продольного
		bool CurrentZoneLinearResult;
		//Скорости вращения вход, работа, выход
//vector<int> speeds;
		int InSpeed;
		int WorkSpeed;
		int OutSpeed;
		//! Обновляет компоненты главной формы, обрабатывает события
		void __fastcall UpdateStatus();
//		void __fastcall GetTubeNumber();
//		void __fastcall ClearCharts();
		void __fastcall NextTube(void);
		//! Поток обработки всей кучи информации
		bool OnlineCycle(void);
		bool OnlineCycleTransit(void);
		int delay;
	public:
		//! Конструктор
		//! @param *_Counter Плата Advantech1784
		//! @param *St Указатель на статусбар, показывающий течение процесса
		//! @param _Thick Используем ли толщинометрию
		//! @param _Linear Используем ли продольный
		//! @param _Spectro Используем ли спектроскоп
		//! @param &alth Адрес треда аварии
		__fastcall ThreadOnLine(bool CreateSuspended,
								bool _Linear,
								bool _Spectro,
								Spectroscope *_Spectroscope);
		//! Признак остановки потока
		bool Collect;
		//! Вместо Return Value, который не откопать
		bool cool;
		Spectroscope *spectroscope;
};
//---------------------------------------------------------------------------

#endif
