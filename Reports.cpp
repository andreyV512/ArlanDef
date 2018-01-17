//------------------------------------------------------------------------------


#pragma hdrstop

#include "Reports.h"
//------------------------------------------------------------------------------

#pragma package(smart_init)

Reports::Reports()
{
	Opened=false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*void Reports::ExcelInit(UnicodeString File)
{
	// если Excel запущен - подключиться к нему
	try
	{
		App=Variant::GetActiveObject(L"Excel.Application");
	}
	catch(...)
	{
	//Excel не запущен - запустить его
		 try
		 {
			App=Variant::CreateObject(L"Excel.Application");
		 }
		 catch (...)
		 {
		  Application->MessageBox(L"Невозможно открыть Microsoft Excel,возможно Excel не установлен на компьютере.",
																					L"Ошибка",MB_OK+MB_ICONERROR);
		 }
	}

	try
	{
		if(File!="")
			App.OlePropertyGet("WorkBooks").OleProcedure("Open",File.c_str());
		else
			App.OlePropertyGet("WorkBooks").OleProcedure("add");

		Sh=App.OlePropertyGet("WorkSheets",1);
	}

	catch(...)
	{
		Application->MessageBox(L"Ошибка открытия книги Microsoft Excel!",
										 L"Ошибка",MB_OK+MB_ICONERROR);
	}
}     */

void Reports::ExcelInit(UnicodeString File)
{
	// если Excel запущен - подключиться к нему
/*	try
	{
		App=Variant::GetActiveObject(L"Excel.Application");
	}
	catch(...)
	{    */
		 // Excel не запущен - запустить его
		 //try
		 //{
			App=Variant::CreateObject(L"Excel.Application");
		// }
/*		 catch (...)
		 {
		  Application->MessageBox(L"Невозможно открыть Microsoft Excel,возможно Excel не установлен на компьютере.",
																					L"Ошибка",MB_OK+MB_ICONERROR);
		 }
	}  */

//	try
//	{
		if(File!="")
			App.OlePropertyGet("WorkBooks").OleProcedure("Open",File.c_str());
		else
			App.OlePropertyGet("WorkBooks").OleProcedure("add");

		Sh=App.OlePropertyGet("WorkSheets",1);
//	}
	/*
	catch(...)
	{
		Application->MessageBox(L"Ошибка открытия книги Microsoft Excel!",
										 L"Ошибка",MB_OK+MB_ICONERROR);
	}                     */
}
//------------------------------------------------------------------------------
Reports::~Reports()
{
	if(Opened)
		App.OleProcedure("Quit");
	App.Clear();
	Sh.Clear();
	Rang.Clear();
}
//------------------------------------------------------------------------------
void Reports::ToExcelCell(int Row,int Column, Variant data)
{
  try
  {
	Variant  cur = Sh.OlePropertyGet("Cells", Row,Column);
	cur.OlePropertySet("Value", data);
  }
	catch(...) { ; }
}
//------------------------------------------------------------------------------
void Reports::StrToExcelCell(int Row, int Column, AnsiString data)
{
  try
  {
	Variant  cur = Sh.OlePropertyGet("Cells", Row, Column);
	cur.OlePropertySet( "Value", data.c_str() );
  }
	catch(...) { ; }
}
//------------------------------------------------------------------------------
void Reports::ExportAllTubes(TDateTime Begin, TDateTime End,UnicodeString SG ,UnicodeString Result,UnicodeString TS)
{
	//Здесь смотрим условия отбора пришедшие из формы Report, и составляем SQL запрос

	DM->Find->Active=false;
	DM->Find->Close();
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add("Select * from dbo.InfoTube");
   	DM->Find->SQL->Add("where (Date Between :Begin and :End)");
	//DM->Find->SQL->Add("where ((Date > :Begin) and (Date < :End))");
	DM->Find->Parameters->ParamByName("Begin")->Value=Begin;
	DM->Find->Parameters->ParamByName("End")->Value=End;

	if( SG != "" )
		DM->Find->SQL->Add("AND (SolidGroup in "+SG+")");

	if( Result != "" )
		DM->Find->SQL->Add("AND (Result in"+Result+")");

	if( TS != "" )
		DM->Find->SQL->Add("AND (TypeSize in"+TS+")");


	DM->Find->SQL->Add("ORDER BY Date");                //сортируем по дате
	DM->Find->ExecSQL();
	DM->Find->Active=true;

	if(DM->Find->RecordCount == 0)
		Application->MessageBoxW(L"Записей удовлетворяющих условиям отбора не обнаружено",L"Ошибка",MB_OK+MB_ICONERROR);
	else
	{

		//Здесь собственно форматируем лист и выводим туда данные
		ExcelInit("");

		//Установим ширину столбцов
		App.OlePropertyGet("Columns").OlePropertyGet("Item",1).OlePropertySet("ColumnWidth",5);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",2).OlePropertySet("ColumnWidth",14);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",3).OlePropertySet("ColumnWidth",14);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",4).OlePropertySet("ColumnWidth",20);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",25);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",6).OlePropertySet("ColumnWidth",25);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",7).OlePropertySet("ColumnWidth",21);
	  //	App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",10);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",18);


		//Объединяем первую строку и пишем туда заголовок
		Rang = Sh.OlePropertyGet("Range", "A1:E1");
		Rang.OleProcedure("Merge");
		StrToExcelCell(1,1,"Отсканированые трубы с "+DateToStr(Begin)+" по "+DateToStr(End));

		//Делаем настройки шрифта
		Rang.OlePropertyGet("Font").OlePropertySet("Name", "Times New Roman");
		Rang.OlePropertyGet("Font").OlePropertySet("Size",14);
		Rang.OlePropertyGet("Font").OlePropertySet("Bold", true);

		//Устанавливаем ширину 1й строки и выравнивание
		Rang.OlePropertySet("RowHeight",20);
		Rang.OlePropertySet("HorizontalAlignment", 3);
		Rang.OlePropertySet("VerticalAlignment", 2);

		//Пишем заголовки отчета
		int Rnum=3;
		StrToExcelCell(Rnum,1,"№");
		StrToExcelCell(Rnum,2,"Дата/Время");
		StrToExcelCell(Rnum,3,"Типоразмер");
		StrToExcelCell(Rnum,4,"Группа прочности");
	  //	StrToExcelCell(Rnum,5,"Продольный результат");
	  //	StrToExcelCell(Rnum,6,"Поперечный результат");
	  //	StrToExcelCell(Rnum,7,"Результат толщины");
		//StrToExcelCell(Rnum,5,"Длина,мм");
		StrToExcelCell(Rnum,5,"Результат");


		//Выравниваем заголовки
		Rang = Sh.OlePropertyGet("Range", "A3:E3");
		Rang.OlePropertySet("HorizontalAlignment", 3);
		Rang.OlePropertyGet("Font").OlePropertySet("Name", "Times New Roman");
		Rang.OlePropertyGet("Font").OlePropertySet("Size",12);
		Rang.OlePropertyGet("Font").OlePropertySet("Bold", true);

		//Подчеркнем снизу
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("LineStyle", 1);
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("Weight", 3);

		//Берем длину зоны из ini-файла
		Ini = new TIniFile(Globals::IniFileName);
		int LengthZone=Ini->ReadInteger("Size", "ZoneLength",200);
		delete Ini;

		//закончили форматирование листа
		Rnum=4;

		long Brack=0;
		long Valid=0;
		long SecondClass=0;
		//теперь выводим данные
		for(int i=0; i < DM->Find->RecordCount; i++ )
		{
			//Прихорашиваем данные перед выводом (Конечный результат)
			UnicodeString Res;
			if(DM->Find->FieldByName("Result")->AsString == "Г ")
				{Valid++; Res="Первый класс";}
			else
			if(DM->Find->FieldByName("Result")->AsString == "Б ")
				{Brack++; Res="Брак";}
			else
			if(DM->Find->FieldByName("Result")->AsString == "К ")
				{SecondClass++; Res="Второй класс";}

			ToExcelCell(Rnum+i, 1,Variant(i+1));                                        //номер по порядку
			ToExcelCell(Rnum+i, 2,Variant(DM->Find->FieldByName("Date")->Value));
			StrToExcelCell(Rnum+i, 3,DM->Find->FieldByName("TypeSize")->Value);
			StrToExcelCell(Rnum+i, 4,String(DM->Find->FieldByName("SolidGroup")->AsString));
		 //	StrToExcelCell(Rnum+i, 5,AntiGovno(DM->Find->FieldByName("LineResult")->AsString));
		 //	StrToExcelCell(Rnum+i, 6,AntiGovno(DM->Find->FieldByName("Singleton::Instance()->CrossResult")->AsString) );
		 //	StrToExcelCell(Rnum+i, 7,AntiGovno(DM->Find->FieldByName("ThicknessResult")->AsString));
		 //	ToExcelCell(Rnum+i, 5,Variant(DM->Find->FieldByName("Length")->Value*LengthZone));
			StrToExcelCell(Rnum+i, 5,Res);               								//Конечный результат
			DM->Find->Next();
		}


		//Выравниваем данные в таблице по центру столбца
		AnsiString str = "A4:E"+IntToStr(Rnum-1+DM->Find->RecordCount);

		Rang = Sh.OlePropertyGet("Range", str.c_str()  ); 		//Embarcadero ты беспощаден
		Rang.OlePropertySet("HorizontalAlignment", 3);

		//Border - определяет бордюр - 1 слева, 2 справа, 3 сверху, 4 снизу
		//Подчеркиваем снизу толщина 2
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("LineStyle", 1);
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("Weight", 2);

		//Обводим справа
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",2 ).OlePropertySet("LineStyle", 1);
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",2 ).OlePropertySet("Weight", 2);

		Rnum+=DM->Find->RecordCount+1;

		for(int i=0; i<=3; i++)
			{
				AnsiString Str="A"+IntToStr(Rnum+i)+":B"+IntToStr(Rnum+i);

				Rang = Sh.OlePropertyGet("Range", Str.c_str());
				Rang.OleProcedure("Merge");
				Rang.OlePropertySet("HorizontalAlignment", 4);

				Rang.OlePropertyGet("Font").OlePropertySet("Name", "Times New Roman");
				Rang.OlePropertyGet("Font").OlePropertySet("Size",12);
				Rang.OlePropertyGet("Font").OlePropertySet("Bold", true);

				Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("LineStyle", 1);
				Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("Weight", 3);

			}

		StrToExcelCell(Rnum,1,"Всего труб:  "+UnicodeString(DM->Find->RecordCount));
		StrToExcelCell(Rnum+1,1,"Первый класс:  "+UnicodeString(Valid));
		StrToExcelCell(Rnum+2,1,"Брак:  "+UnicodeString(Brack));
		StrToExcelCell(Rnum+3,1,"Второй класс:  "+UnicodeString(SecondClass));


		//Дадим название листу с отчетом
		Sh = App.OlePropertyGet("Worksheets").OlePropertyGet("Item", 1);
		Sh.OlePropertySet("Name", "Отчет №1");

		//делаем документ видимым
		if(!App.Empty())
			App.OlePropertySet("Visible",true);

		Opened=true;         //документ открыт-флаг для дальнейшего закрытия документа.
	}

}
//------------------------------------------------------------------------------
String Reports::AntiGovno(String Res)
{
	if(Res == "Г ")
		return "Первый класс";
	else
	if(Res == "Б ")
		return "Брак";
	else
	if(Res == "К ")
		return "Второй класс";

	return "";
}


