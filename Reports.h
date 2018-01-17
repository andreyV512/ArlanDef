//---------------------------------------------------------------------------

#ifndef ReportsH
#define ReportsH

#include "DataModule.h"
#include "Comobjec.h"
#include "Global.h"
#include <vector>

//! Класс отвечающий за вывод данных в Excel
class Reports
{
	public:
		//! Конструктор
		Reports();
		//! Деструктор
		~Reports();
		//! @brief Выводит данные по трубам учитывая критерии отбора
		//! @param begin - Дата начала
		//! @param End - Дата окончания
		//! @param SG - группа прочности
		//! @param Result - результат (брак,годно,класс 2)
		//! @param TS - типоразмер
		void ExportAllTubes(TDateTime Begin, TDateTime End,UnicodeString SG ,UnicodeString Result,UnicodeString TS);
		void ExportTube(ULONG ID);                            	//Принимает номер трубы
		//! Объект Application
		Variant App;
		//!Объект Лист
		Variant Sh;
		//! Объект ячейка(область ячеек)
		Variant Rang;
	private:
		void DrawExcelAll();
		void DrawExcelZone();
		void ExcelInit(UnicodeString File);                  		//Инициализируем документ
		void ToExcelCell(int Row, int Column, Variant data);        //Пишет данные в ячейку
		void StrToExcelCell(int Row, int Column, AnsiString data);  //Пишет строку в ячейку
		TIniFile *Ini;                      						//Файл настроек
		bool Opened;                                                //Флаг для закрытия файла
		String AntiGovno(String Res);
};

#endif
