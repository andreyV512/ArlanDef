//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataModule.h"
#include "Global.h"
#include "Main.h"

#define QZone 61
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDM *DM;
//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
	: TDataModule(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TDM::DataModuleCreate(TObject *Sender)//Коннектимся к БД коннект из udl файла
{
	UnicodeString path = ParamStr(0);
	int pos2 = path.Pos("Defectoscope.exe");
	SetCurrentDir( path.SubString(1, pos2-1) );

	Connect=false;
	try
	{
		ADOCon->Connected=false;
		ADOCon->ConnectionString="FILE NAME=..\\..\\Settings\\Connection.udl";
		ADOCon->Open();
		Connect=true;
	}
	catch (...)
	{
		Application->MessageBoxW(L"Не удалось подключиться к базе данных, проверьте настройки подключения",L"ОШИБКА",MB_OK|MB_ICONERROR);
		Connect=false;
	}
	if(Connect)
	{
		TubeTable->Active=true;
		DefectTable->Active=true;
		TEtalons->Active=true;
		NumTube=0;
		CreateTable();
	}
}
//---------------------------------------------------------------------------
void TDM::SaveTube( DynamicArray <double> CrossDef , DynamicArray <double> LineDef ,
					int lengthArray , String Result , String SolidGroup,
					int TypeSize,bool isEtalon ) //
{
	if(Connect)
	{
		TubeTable->Append();
		TubeTable->FieldByName("Date")->AsDateTime = Date()+Time();
		TubeTable->FieldByName("Result")->Value = Result;
		TubeTable->FieldByName("SolidGroup")->AsString = SolidGroup;
		TubeTable->FieldByName("TypeSize")->AsInteger=TypeSize;
		TubeTable->FieldByName("length")->AsInteger=Singleton::Instance()->SumResult->zones * 200.0;
		TubeTable->FieldByName("NumberTube")->AsString=Globals::tube_number;
		TubeTable->Post();
	 }
}
//------------------------------------------------------------------------------
void TDM::CreateTable()
{
	if(Connect)
	{
		String SQL_database_name = "Buran";
		Find->Close();
		Find->Active = false;
		Find->SQL->Clear();
		Find->SQL->Add("IF OBJECT_ID('" + SQL_database_name + ".[dbo].[Uran_NKT]') IS NULL");
		Find->SQL->Add("BEGIN");
		Find->SQL->Add("SET ANSI_NULLS ON");
		Find->SQL->Add("SET QUOTED_IDENTIFIER ON");
		Find->SQL->Add("SET ANSI_PADDING ON");
		Find->SQL->Add("CREATE TABLE [dbo].[Uran_NKT]("
						 "[AI] [int] IDENTITY(1,1) NOT NULL,"
						 "[N_TR_URAN] [int] NOT NULL,"
						 "[DAT_T] [smalldatetime] NOT NULL,"
						 "[DAT_TIM] [datetime] NULL DEFAULT GETDATE(),"
						 "[PBR_1] [smallint] NOT NULL,"
						 "[P2C_1] [smallint] NOT NULL,"
						 "[PBR_2] [smallint] NOT NULL,"
						 "[P2C_2] [smallint] NOT NULL,"
						 "[PBR_3] [smallint] NOT NULL,"
						 "[P2C_3] [smallint] NOT NULL,"
						 "[L_URAN] [smallint] NOT NULL,"
						 "[REZ1] [smallint] NOT NULL,"
						 "[REZ2] [smallint] NOT NULL,"
						 "[PRIZN_URAN] [smallint] NOT NULL,"
						 "[PRIZN_URAN_MUFTA] [smallint] NOT NULL,"
						 "[PR_GROUP_URAN] [char](3) COLLATE Cyrillic_General_CI_AS NOT NULL,"
						 "[PR_GROUP_MUFTA] [char](3) COLLATE Cyrillic_General_CI_AS NOT NULL,");
		for(int i=0; i<QZone; i++)
		{
			Find->SQL->Add("[DEF_" + IntToStr(i * 2) + "] [smallint] NOT NULL,");
			Find->SQL->Add("[DEF_" + IntToStr(i * 2 + 1) + "]  [smallint] NOT NULL");
			if( i < (QZone) )
				Find->SQL->Add(", ");
		}
		Find->SQL->Add(") ON [PRIMARY]");
		Find->SQL->Add("SET ANSI_PADDING OFF");
		Find->SQL->Add("END");

		Find->ExecSQL();
	}
}
//------------------------------------------------------------------------------
void TDM::SaveTube()
{
	if(Connect)
	{
		Find->Close();
		Find->Active = false;
		Find->SQL->Clear();
		Find->SQL->Add("delete from dbo.Uran_NKT");
		Find->ExecSQL();

		tableASU->Active = true;
		tableASU->Insert();
		// Номер трубы
		tableASU->FieldByName("N_TR_URAN")->AsInteger = 0;
		// Дата текущая
		tableASU->FieldByName("DAT_T")->AsDateTime = Date();
		// Дата + время
		tableASU->FieldByName("DAT_TIM")->AsDateTime = Date() +Time();

		//Порог брака поперечный
		tableASU->FieldByName("PBR_1")->AsInteger = (int)Singleton::Instance()->CrossResult->borders[0];
		int border;
		if(Singleton::Instance()->CrossResult->borders.Length == 2)
			border = (int)Singleton::Instance()->CrossResult->borders[1];
		else
			border = 0;
		//Порог второго класса поперечный
		tableASU->FieldByName("P2C_1")->AsInteger = border;

		//Порог брака продольный
		tableASU->FieldByName("PBR_2")->AsInteger = (int)Singleton::Instance()->LinearResult->borders[0];
		if(Singleton::Instance()->LinearResult->borders.Length == 2)
			border = (int)Singleton::Instance()->LinearResult->borders[1];
		else
			border = 0;
		//Порог второго класса поперечный
		tableASU->FieldByName("P2C_2")->AsInteger = border;

		//Порог брака толщины
		tableASU->FieldByName("PBR_3")->AsInteger = (int)(Math::RoundTo(Singleton::Instance()->ThResult->borders[0],-2)*10);
		if(Singleton::Instance()->ThResult->borders.Length==2)
			border = (int)(Singleton::Instance()->ThResult->borders[1]*10);
		else
			border = 0;
		//Порог второго класса толщины
		tableASU->FieldByName("P2C_3")->AsInteger = border;
		//Фактическое кол-во зон
		tableASU->FieldByName("L_URAN")->AsInteger =Singleton::Instance()->SumResult->zones;
		//Рез 1
		tableASU->FieldByName("REZ1")->AsInteger = Singleton::Instance()->SumResult->cut1;
		//Рез 2
		tableASU->FieldByName("REZ2")->AsInteger = Singleton::Instance()->SumResult->cut2;
		int result=0;
		if( Singleton::Instance()->SumResult->decision == "Годно" )
			result = 1;
		else if( Singleton::Instance()->SumResult->decision == "Класс 2" )
			result = 2;
		else
			result = 3;
		//Результат по трубе
		tableASU->FieldByName("PRIZN_URAN")->AsInteger = result;
		//Результат муфты
		tableASU->FieldByName("PRIZN_URAN_MUFTA")->AsInteger = 255;
		//Группа прочности трубы
		tableASU->FieldByName("PR_GROUP_URAN")->AsString = Globals::tubeSG.solidGroup().GroupToEnglish();
		//Группа прочности муфты
		tableASU->FieldByName("PR_GROUP_MUFTA")->AsString = "X";

		//Заполняем все зоны нолями
		for(int i=0; i<(QZone*2); i++)
		{
			tableASU->FieldByName("DEF_"+IntToStr(i))->AsInteger = resultZone(i);
		}

		tableASU->Post();
	}
}
int16_t TDM::resultZone(int numberZone)
{
//	int maxZones = Singleton::Instance()->CrossResult->zones;
//	if( maxZones < Singleton::Instance()->LinearResult->zones )
//		maxZones = Singleton::Instance()->LinearResult->zones;
//	else
//	if( maxZones < Singleton::Instance()->ThResult->zones )
//		maxZones = Singleton::Instance()->ThResult->zones;

	if( ( Singleton::Instance()->SumResult->zones-1 ) < numberZone)
		return 0;

	double crossResult  = Singleton::Instance()->CrossResult->zone_data[numberZone];
	double linearResult = Singleton::Instance()->LinearResult->zone_data[numberZone];
	double thickResult  = Singleton::Instance()->ThResult->zone_data[numberZone];
	double sumResult    = Singleton::Instance()->SumResult->zone_data[numberZone];

	TColor good 		= Singleton::Instance()->CrossResult->Good;
	TColor brack        = Singleton::Instance()->CrossResult->Brack;
	TColor class2		= Singleton::Instance()->CrossResult->Class2;
	TColor dead			= Singleton::Instance()->CrossResult->Dead;

	int16_t result=0;

	//Если серая зона
	if( thickResult == 10.0 )
		result = 255;
	else
	{
		result = (int)(Math::RoundTo(thickResult,-2)*10);
	}

	//Результат будем выяснять по цвету т.к. более удобного функционала не было предусмотрено
	//Биты 8 - 9  Поперечный модуль
	//Если зона существует
	if(crossResult > 0)
	{
		if(Singleton::Instance()->CrossResult->ZoneColor(crossResult) == good)
			result |= 256;
		else if(Singleton::Instance()->CrossResult->ZoneColor(crossResult) == class2)
			result |= 512;
		else
			result |= 768;
	}
	//Биты 10-11 Продольный модуль
	if(linearResult > 0)
	{
		if(Singleton::Instance()->LinearResult->ZoneColor(linearResult) == good)
			result |= 1024;
		else if(Singleton::Instance()->LinearResult->ZoneColor(linearResult) == class2)
			result |= 2048;
		else
			result |= 3072;
	}
	//Биты 12 - 13 Модуль толщиномера
	if( thickResult > 0 )
	{
		if( Singleton::Instance()->ThResult->ZoneColor(thickResult) == good)
			result |= 4096;
		else if( Singleton::Instance()->ThResult->ZoneColor(thickResult) == class2)
			result |= 8192;
		else
			result |= 12288;
	}
	//Биты 14 - 15 Общий результат
	if( Singleton::Instance()->SumResult->ZoneColor(sumResult) == good)
		result |= 16384;
	else if(Singleton::Instance()->SumResult->ZoneColor(sumResult) == class2)
		result |= -32768;
	else
		result |= -16384;
	return result;
}
//------------------------------------------------------------------------------
int TDM::GetThick(DynamicArray <double> Thick, long ID)
{
	if(Connect)
	{
		Find->Close();
		Find->Active=false;
		Find->SQL->Clear();
		Find->SQL->Add("Select Max(ID_Tube) AS maximum from dbo.InfoTube");
		Find->ExecSQL();
		Find->Active=true;
		ULONG NumberTube=DM->Find->FieldByName("maximum")->AsInteger;
		ID=NumberTube;
		TubeTable->Last();

			//Запрос на выборку данных из таблицы с толщиной
			QueryThick->Close();
			QueryThick->Active=false;
			QueryThick->SQL->Clear();
			QueryThick->SQL->Add("Select * from dbo.ThickDef where ID_Tube=:Par ORDER BY zone" );
			QueryThick->Parameters->ParamByName("par")->Value=NumberTube;
			QueryThick->ExecSQL();
			QueryThick->Active=true;
			TPr::SendToProtocol("кол-во записей (зон)  "+IntToStr(QueryThick->RecordCount));
			//пишем в массив выбранные данные
			for (int i = 0; i < QueryThick->RecordCount; i++)
			{
				Thick[i]=QueryThick->FieldByName("MinThickness")->AsFloat;
				QueryThick->Next();
			}
			TPr::SendToProtocol("Получили результат по  толщине из базы");
			return QueryThick->RecordCount;
	}
	return 0;
}

//------------------------------------------------------------------------------
void TDM::SaveSG(SolidGroups::Tube Tube, UnicodeString TableName)
{
if(Connect)
	{
	//Получаем название таблицы и пишем в нее, если название неправельное то посылаем в message
	if ( (TableName == "SGCenter") || (TableName == "SGDataBase") || (TableName == "SGEtalon" ) )
		{
			//Типа инициализация ADOTable по имени таблицы
			Table->Active=false;
			Table->Connection=ADOCon;
			Table->TableName=TableName;
			Table->Active=true;
			//пишем инфу в таблицу
			Table->Insert();
			Table->FieldByName("TypeSize")->AsInteger = (int) Tube.type();
			Table->FieldByName("Date")->AsDateTime = Date()+Time();
			Table->FieldByName("Name")->AsString = (String) Tube.solidGroup();
			Table->FieldByName("Date")->AsDateTime = Date() + Time();
			vector<double> coords=Tube.Coordinates();
			for(int i=0; i<8;i++)
			{
				Table->FieldByName("p"+UnicodeString(i+1))->Value=(int)coords[i];
			}
			Table->Post();
			//Если в векторе координат есть данные по химсоставу, то запишем их
			if(coords.size()>8)Spectroscope::WriteSpectroDataToDB(coords,ADOCon);

		} else
				Application->MessageBoxW(L"Не верное название таблицы",L"Ошибка",MB_OK);
	}
}
//----------------------------------------------------------------------------------
vector<SolidGroups::Tube> TDM::ReadEtalon(int TypeSize)	// ,UnicodeString SG
{
	vector <SolidGroups::Tube> array;
	//Пишем запрос для поиска нужной нам записи
	Find->Close();
	Find->Active=false;
	Find->SQL->Clear();
	Find->SQL->Add("Select * from dbo.SGEtalon");
	Find->SQL->Add("where  (TypeSize=:par1)");	//(Name=:par) and
//	Find->Parameters->ParamByName("par")->Value=SG;
	Find->Parameters->ParamByName("par1")->Value=TypeSize;
	Find->ExecSQL();
	Find->Active=true;
	TPr::SendToProtocol("Эталонов ГП найдено: "+IntToStr(Find->RecordCount));
	Spectroscope::initElemIndexes();
	//Проверяем сколько записей нашлось
	if (Find->RecordCount == 0 )
		{
			Application->MessageBoxW(L"Запрашиваемого эталона группы прочности не обнаружено\n Ошибка 300",L"Ошибка",MB_ICONERROR);
			return array;
		}
	else
	{
		SolidGroups::Tube buf; 			//объект "трубы"
		Find->First();

		for (int i = 0;  i < Find->RecordCount; i++)
		{
			vector <double> param;
			for(int j=1; j<=8; j++)     //Пихаем данные в произвольный буфер
				param.push_back( Find->FieldByName("p"+UnicodeString(j))->AsFloat );

			vector<double> chemData = Spectroscope::AddSpectroDataToSGEtalon(Find->FieldByName("ID")->AsInteger,ADOCon);
			param.insert(param.end(),chemData.begin(),chemData.end());

			buf.setCoordinates(param); //буфер пихаем в "Трубу"
			buf.setTubeType( SolidGroups::TubeType( TypeSize ) );
			buf.setSolidGroup( SolidGroups::Group ( Find->FieldByName("Name")->AsString ) );
			array.push_back(buf);      //Трубу пихаем в массив труб
			Find->Next();
		}
	}


	//Возвращаем параметры в массиве объектов
	return array;
}
//------------------------------------------------------------------------------
SolidGroups::Tube TDM::ReadCenter(int TypeSize,UnicodeString SG)
{   //Пишем запрос для поиска нужной нам записи
	SolidGroups::Tube tube;
	Find->Close();
	Find->Active=false;
	Find->SQL->Clear();
	Find->SQL->Add("Select * from dbo.SGCenter");
	Find->SQL->Add("where ((Name=:par) and (TypeSize=:par1))");
	Find->Parameters->ParamByName("par")->Value=SG;
	Find->Parameters->ParamByName("par1")->Value=TypeSize;
	Find->ExecSQL();
	Find->Active=true;
	//Проверяем сколько записей нашлось
	if (Find->RecordCount == 0 )
		{
			Application->MessageBoxW(L"Запрашиваемой записи не обнаружено",L"Ошибка",MB_OK);
			//tube.setTubeType( SolidGroupTube::tsUnknown );
			tube.setTubeType( SolidGroups::TubeType::tsUnknown );
			return tube;
		}
	else
	if (Find->RecordCount > 1 )
		{
			Application->MessageBoxW(L"Обнаруженно более одной записи координат центров",L"Ошибка",MB_OK);
			//tube.setTypeSize(SolidGroupTube::tsUnknown);
			tube.setTubeType( SolidGroups::TubeType::tsUnknown );
			return tube;
		}
	else
	{
		vector<double> param;
		for (int i = 1; i <=8; i++)                    //Вносим значения центров из таблицы в вектор
			param.push_back(Find->FieldByName("p"+(UnicodeString)i)->AsFloat);


		tube.setCoordinates(param);             //Вектор пихаем в объект
		tube.setTubeType( SolidGroups::TubeType(TypeSize) );
		tube.setSolidGroup( SolidGroups::Group( SG ) );

		return tube;
	}
}
//------------------------------------------------------------------------------
ULONG TDM::AddTube(String result)
{
	if( !Connect)
		Application->MessageBoxW(L"Не удалось подключиться к базе данных, проверьте настройки подключения",L"ОШИБКА",MB_OK|MB_ICONERROR);
	else
		{
			TubeTable->Append();
			TubeTable->FieldByName("ThicknessResult")->AsString=result;
			TubeTable->Post();


			ULONG ID=TubeTable->FieldByName("ID_Tube")->Value;
			return ID;
		}
	return 0;
}


