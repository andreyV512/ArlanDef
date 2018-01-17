//---------------------------------------------------------------------------

#include "intsafe.h"
#include "DataModule.h"
#ifndef StatisticsH
#define StatisticsH
//---------------------------------------------------------------------------
//! Класс статистики прогона труб
class Statistics
{
	public:
		//! Конструктор по умолчанию
		Statistics();
		//! Добавляет результат за смену
		bool AddTube(UnicodeString Result);
		//! Обнуляет все данные
		void StatsToNull();
//		void GetValue(ULONG _Brack, ULONG _SecondClass, ULONG _Valid);   //Возвращает результаты за смену
		//! Возвращает число бракованных труб
		ULONG GetBrack();
		//! Возвращает число труб второго класса
		ULONG GetSecondClass();
		//! Вовзращает число годных труб
		ULONG GetValid();
		//! Возвращает число бракованных труб в кармане брака
		ULONG GetBrackInDaStreet();
		//! Устанавливает число бракованных труб в кармане брака
		void SetBrackInDaStreet( int BrackCount);
		//! Возвращает данные по результатам  за все время(из базы)
		void GetAllValue(ULONG &_Brack, ULONG &_SecondClass, ULONG &_Valid);
	private:
		//! Число брака
		ULONG Brack;
		//! Число второго класса
		ULONG SecondClass;
		//! Число годных
		ULONG Valid;
		//! Число труб в кармане брака
		ULONG BrackInDaStreet;
		//! Ини файл
		TIniFile *ini;

};
#endif
