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
	// ���� Excel ������� - ������������ � ����
	try
	{
		App=Variant::GetActiveObject(L"Excel.Application");
	}
	catch(...)
	{
	//Excel �� ������� - ��������� ���
		 try
		 {
			App=Variant::CreateObject(L"Excel.Application");
		 }
		 catch (...)
		 {
		  Application->MessageBox(L"���������� ������� Microsoft Excel,�������� Excel �� ���������� �� ����������.",
																					L"������",MB_OK+MB_ICONERROR);
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
		Application->MessageBox(L"������ �������� ����� Microsoft Excel!",
										 L"������",MB_OK+MB_ICONERROR);
	}
}     */

void Reports::ExcelInit(UnicodeString File)
{
	// ���� Excel ������� - ������������ � ����
/*	try
	{
		App=Variant::GetActiveObject(L"Excel.Application");
	}
	catch(...)
	{    */
		 // Excel �� ������� - ��������� ���
		 //try
		 //{
			App=Variant::CreateObject(L"Excel.Application");
		// }
/*		 catch (...)
		 {
		  Application->MessageBox(L"���������� ������� Microsoft Excel,�������� Excel �� ���������� �� ����������.",
																					L"������",MB_OK+MB_ICONERROR);
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
		Application->MessageBox(L"������ �������� ����� Microsoft Excel!",
										 L"������",MB_OK+MB_ICONERROR);
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
	//����� ������� ������� ������ ��������� �� ����� Report, � ���������� SQL ������

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


	DM->Find->SQL->Add("ORDER BY Date");                //��������� �� ����
	DM->Find->ExecSQL();
	DM->Find->Active=true;

	if(DM->Find->RecordCount == 0)
		Application->MessageBoxW(L"������� ��������������� �������� ������ �� ����������",L"������",MB_OK+MB_ICONERROR);
	else
	{

		//����� ���������� ����������� ���� � ������� ���� ������
		ExcelInit("");

		//��������� ������ ��������
		App.OlePropertyGet("Columns").OlePropertyGet("Item",1).OlePropertySet("ColumnWidth",5);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",2).OlePropertySet("ColumnWidth",14);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",3).OlePropertySet("ColumnWidth",14);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",4).OlePropertySet("ColumnWidth",20);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",25);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",6).OlePropertySet("ColumnWidth",25);
		//App.OlePropertyGet("Columns").OlePropertyGet("Item",7).OlePropertySet("ColumnWidth",21);
	  //	App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",10);
		App.OlePropertyGet("Columns").OlePropertyGet("Item",5).OlePropertySet("ColumnWidth",18);


		//���������� ������ ������ � ����� ���� ���������
		Rang = Sh.OlePropertyGet("Range", "A1:E1");
		Rang.OleProcedure("Merge");
		StrToExcelCell(1,1,"�������������� ����� � "+DateToStr(Begin)+" �� "+DateToStr(End));

		//������ ��������� ������
		Rang.OlePropertyGet("Font").OlePropertySet("Name", "Times New Roman");
		Rang.OlePropertyGet("Font").OlePropertySet("Size",14);
		Rang.OlePropertyGet("Font").OlePropertySet("Bold", true);

		//������������� ������ 1� ������ � ������������
		Rang.OlePropertySet("RowHeight",20);
		Rang.OlePropertySet("HorizontalAlignment", 3);
		Rang.OlePropertySet("VerticalAlignment", 2);

		//����� ��������� ������
		int Rnum=3;
		StrToExcelCell(Rnum,1,"�");
		StrToExcelCell(Rnum,2,"����/�����");
		StrToExcelCell(Rnum,3,"����������");
		StrToExcelCell(Rnum,4,"������ ���������");
	  //	StrToExcelCell(Rnum,5,"���������� ���������");
	  //	StrToExcelCell(Rnum,6,"���������� ���������");
	  //	StrToExcelCell(Rnum,7,"��������� �������");
		//StrToExcelCell(Rnum,5,"�����,��");
		StrToExcelCell(Rnum,5,"���������");


		//����������� ���������
		Rang = Sh.OlePropertyGet("Range", "A3:E3");
		Rang.OlePropertySet("HorizontalAlignment", 3);
		Rang.OlePropertyGet("Font").OlePropertySet("Name", "Times New Roman");
		Rang.OlePropertyGet("Font").OlePropertySet("Size",12);
		Rang.OlePropertyGet("Font").OlePropertySet("Bold", true);

		//���������� �����
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("LineStyle", 1);
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("Weight", 3);

		//����� ����� ���� �� ini-�����
		Ini = new TIniFile(Globals::IniFileName);
		int LengthZone=Ini->ReadInteger("Size", "ZoneLength",200);
		delete Ini;

		//��������� �������������� �����
		Rnum=4;

		long Brack=0;
		long Valid=0;
		long SecondClass=0;
		//������ ������� ������
		for(int i=0; i < DM->Find->RecordCount; i++ )
		{
			//������������� ������ ����� ������� (�������� ���������)
			UnicodeString Res;
			if(DM->Find->FieldByName("Result")->AsString == "� ")
				{Valid++; Res="������ �����";}
			else
			if(DM->Find->FieldByName("Result")->AsString == "� ")
				{Brack++; Res="����";}
			else
			if(DM->Find->FieldByName("Result")->AsString == "� ")
				{SecondClass++; Res="������ �����";}

			ToExcelCell(Rnum+i, 1,Variant(i+1));                                        //����� �� �������
			ToExcelCell(Rnum+i, 2,Variant(DM->Find->FieldByName("Date")->Value));
			StrToExcelCell(Rnum+i, 3,DM->Find->FieldByName("TypeSize")->Value);
			StrToExcelCell(Rnum+i, 4,String(DM->Find->FieldByName("SolidGroup")->AsString));
		 //	StrToExcelCell(Rnum+i, 5,AntiGovno(DM->Find->FieldByName("LineResult")->AsString));
		 //	StrToExcelCell(Rnum+i, 6,AntiGovno(DM->Find->FieldByName("Singleton::Instance()->CrossResult")->AsString) );
		 //	StrToExcelCell(Rnum+i, 7,AntiGovno(DM->Find->FieldByName("ThicknessResult")->AsString));
		 //	ToExcelCell(Rnum+i, 5,Variant(DM->Find->FieldByName("Length")->Value*LengthZone));
			StrToExcelCell(Rnum+i, 5,Res);               								//�������� ���������
			DM->Find->Next();
		}


		//����������� ������ � ������� �� ������ �������
		AnsiString str = "A4:E"+IntToStr(Rnum-1+DM->Find->RecordCount);

		Rang = Sh.OlePropertyGet("Range", str.c_str()  ); 		//Embarcadero �� ����������
		Rang.OlePropertySet("HorizontalAlignment", 3);

		//Border - ���������� ������ - 1 �����, 2 ������, 3 ������, 4 �����
		//������������ ����� ������� 2
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("LineStyle", 1);
		Rang.OlePropertyGet("Borders").OlePropertyGet("Item",4 ).OlePropertySet("Weight", 2);

		//������� ������
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

		StrToExcelCell(Rnum,1,"����� ����:  "+UnicodeString(DM->Find->RecordCount));
		StrToExcelCell(Rnum+1,1,"������ �����:  "+UnicodeString(Valid));
		StrToExcelCell(Rnum+2,1,"����:  "+UnicodeString(Brack));
		StrToExcelCell(Rnum+3,1,"������ �����:  "+UnicodeString(SecondClass));


		//����� �������� ����� � �������
		Sh = App.OlePropertyGet("Worksheets").OlePropertyGet("Item", 1);
		Sh.OlePropertySet("Name", "����� �1");

		//������ �������� �������
		if(!App.Empty())
			App.OlePropertySet("Visible",true);

		Opened=true;         //�������� ������-���� ��� ����������� �������� ���������.
	}

}
//------------------------------------------------------------------------------
String Reports::AntiGovno(String Res)
{
	if(Res == "� ")
		return "������ �����";
	else
	if(Res == "� ")
		return "����";
	else
	if(Res == "� ")
		return "������ �����";

	return "";
}


