//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "Global.h"


TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
	KeyPreview=true;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormShow(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);
	names = new TStringList();
	SelRow=1;
	SelCol=1;
// ищем типоразмеры в файле и добавляем их в ComboBox
	UpdateComboBox();
	names->Add("Diameter");
	names->Add("CrossBorder1");
	names->Add("CrossBorder2");
	names->Add("LinearBorder1");
	names->Add("LinearBorder2");
	names->Add("ThicknessBorder1");
	names->Add("ThicknessBorder2");
	names->Add("Min_Good_Length");
//	names->Add("EndCut");
	names->Add("CrossDeadZoneStart");
	names->Add("CrossDeadZoneFinish");
	names->Add("LinearDeadZoneStart");
	names->Add("LinearDeadZoneFinish");
	names->Add("InSpeed");
	names->Add("WorkSpeed");
	names->Add("OutSpeed");

	this->bDeleteTypeSize->Enabled = true;
	this->cbTypeSize->Enabled = true;
	this->bCreateTypeSize->Caption="Создать";
// создаем строки с заполненными полями:  "имя настройки" - "текущее значение"
	cbTypeSizeSelect(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::FormClose(TObject *Sender, TCloseAction &Action)
{
// при закрытии формы текущий типоразмер становится текущим для всех
	Globals::current_typesize = cbTypeSize->Text;
}
//---------------------------------------------------------------------------

void TSettingsForm::UpdateComboBox()
{
// очищаем весь список элементов ComboBox
	cbTypeSize->Items->Clear();
	TStringList *sections = new TStringList();
	ini->ReadSections(sections);
// добавляем в список элементов те элементы из ini файла, которые характеризуют типоразмер (префикс "Type_")
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1,5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6,20),NULL);

	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(Globals::current_typesize);
	sections->~TStringList();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ValueListKeyPress(TObject *Sender, wchar_t &Key)
{
// ловим нажатие на клавишу
	switch (Key)
	{
		case VK_F1:
			WinExec("hh Help.chm::Settings.htm",SW_RESTORE);
			break;
		case 27:
			SettingsForm->Close();
			break;
		case '\r':
		// нажатие на Enter только внутри объекта ValueListEditor
			if (Sender==ValueListEditor)
			{
				if ( ValueListEditor->Cells[SelCol][SelRow].IsEmpty() )
				{
					StatusBarBottom->Panels->Items[0]->Text="Введите значение!";
					break;
				}
				else
				{
					if (bCreateTypeSize->Caption=="Создать")
					{  // исправляем значения только у существующего типоразмера
						try
						{
							ValueListEditor->Cells[SelCol][SelRow].ToDouble();
						}
						catch (EConvertError &e)
						{
							e.Message="Fuck";
							StatusBarBottom->Panels->Items[0]->Text="Неправильное значение!";
							break;
						}
						ini->WriteString( "Type_" + cbTypeSize->Text,names->Strings[SelRow-1],ValueListEditor->Cells[SelCol][SelRow]);
						StatusBarBottom->Panels->Items[0]->Text="Значение изменено.";
						break;
					}
				}
			}
	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ValueListEditorSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect)
{
// сохраняем адрес выделенной ячейки в private переменных.
	SelRow=ARow;
	SelCol=ACol;
	//StatusBarBottom->Panels->Items[1]->Text="Столбец " +IntToStr(SelCol)+", Ряд "+IntToStr(SelRow);
	//this->StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::bCreateTypeSizeClick(TObject *Sender)
{
// пользователь пытается создать типоразмер
	if (bCreateTypeSize->Caption=="Создать")
	{
		ValueListEditor->InsertRow("Название типоразмера","",false);
		for (int i = 1; i < ValueListEditor->RowCount; i++)
			ValueListEditor->Cells[1][i]="";
// даём возможность выбора диаметра только из предложенных значений
		ValueListEditor->ItemProps[1]->EditStyle=esPickList;
		ValueListEditor->ItemProps[1]->PickList->Add("60");
		ValueListEditor->ItemProps[1]->PickList->Add("73");
		ValueListEditor->ItemProps[1]->PickList->Add("89");
		this->cbTypeSize->Enabled = false;
		this->bDeleteTypeSize->Enabled = false;
		bCreateTypeSize->Caption="Подтвердить";
	}

	else if (bCreateTypeSize->Caption=="Подтвердить")
	{
		String Sect = "Type_" + ValueListEditor->Cells[1][1];
		Globals::current_typesize = ValueListEditor->Cells[1][1];
		bool ok_ts=true;
// проверяем, все ли норм значения ввел юзер
		try
		{
			for(int i=0; i < names->Count; i++)
			{
				double tmp = ValueListEditor->Cells[1][i+2].ToDouble();
				ini->WriteFloat(Sect, names->Strings[i] , tmp);
			}
		}
		catch (EConvertError &e)
		{
			e.Message="Fuck";
			StatusBarBottom->Panels->Items[0]->Text="Неправильное значение!";
			ok_ts=false;
		}

		if (ok_ts)
		{
			ValueListEditor->DeleteRow(1);
			bCreateTypeSize->Caption="Создать";
			this->cbTypeSize->Enabled=true;
			this->bDeleteTypeSize->Enabled = true;
			StatusBarBottom->Panels->Items[0]->Text = "Типоразмер создан.";
			UpdateComboBox();
			cbTypeSizeSelect(Sender);
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::bDeleteTypeSizeClick(TObject *Sender)
{
// пользователь пытается удалить типоразмер
	if ( Application->MessageBoxW(L"Вы точно хотите удалить типоразмер?",L"Сообщение",MB_YESNO) == 6 )
	{
		ini->EraseSection("Type_"+cbTypeSize->Text);
		this->StatusBarBottom->Panels->Items[0]->Text="Типоразмер удален.";
		if (Globals::current_typesize == cbTypeSize->Text)
		{
			String st0 = cbTypeSize->Items->Strings[0];
			String st1 = cbTypeSize->Items->Strings[1];
			Globals::current_typesize = (cbTypeSize->ItemIndex==0) ? (st1) : (st0);
		}
		UpdateComboBox();
		cbTypeSizeSelect(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::cbTypeSizeSelect(TObject *Sender)
{
// изменился типоразмер - нужно показать его настройки. удаляем все поля и заполняем заново
	ValueListEditor->Strings->Clear();
	String sect = "Type_"+cbTypeSize->Text;
	try
	{
		ValueListEditor->InsertRow("Диаметр трубы, мм",ini->ReadInteger(sect,"Diameter",0),true);
		ValueListEditor->ItemProps[0]->ReadOnly = true;

		ValueListEditor->InsertRow("Поперечный порог брака",ini->ReadFloat(sect,"CrossBorder1",0.0),true);
		ValueListEditor->InsertRow("Поперечный порог класса 2",ini->ReadFloat(sect,"CrossBorder2",0.0),true);
		ValueListEditor->InsertRow("Продольный порог брака",ini->ReadFloat(sect,"LinearBorder1",0.0),true);
		ValueListEditor->InsertRow("Продольный порог класса 2",ini->ReadFloat(sect,"LinearBorder2",0.0),true);
		ValueListEditor->InsertRow("Толщинометрия: порог брака",ini->ReadFloat(sect,"ThicknessBorder1",0.0),true);
		ValueListEditor->InsertRow("Толщинометрия: порог класса 2",ini->ReadFloat(sect,"ThicknessBorder2",0.0),true);
		ValueListEditor->InsertRow("Минимальный годный участок",ini->ReadInteger(sect,"Min_Good_Length",0),true);
//		ValueListEditor->InsertRow("Концевой отрезок, зон",ini->ReadInteger(sect,"EndCut",0),true);

		ValueListEditor->InsertRow("Мёртвая зона поперечного, начало",ini->ReadInteger(sect,"CrossDeadZoneStart",0),true);
		ValueListEditor->InsertRow("Мёртвая зона поперечного, конец",ini->ReadInteger(sect,"CrossDeadZoneFinish",0),true);
		ValueListEditor->InsertRow("Мёртвая зона продольного, начало",ini->ReadInteger(sect,"LinearDeadZoneStart",0),true);
		ValueListEditor->InsertRow("Мёртвая зона продольного, конец",ini->ReadInteger(sect,"LinearDeadZoneFinish",0),true);
		ValueListEditor->InsertRow("Скорость продольного модуля на входе",ini->ReadInteger(sect,"InSpeed",10),true);
		ValueListEditor->InsertRow("Скорость продольного модуля в работе",ini->ReadInteger(sect,"WorkSpeed",35),true);
		ValueListEditor->InsertRow("Скорость продольного модуля на выходе",ini->ReadInteger(sect,"OutSpeed",20),true);
		StatusBarBottom->Panels->Items[0]->Text="";
	}
	catch (EConvertError &e)
	{
		e.Message="Fuck";
		StatusBarBottom->Panels->Items[0]->Text="Файл настроек поврежден";   // не работает!!!
		StatusBarBottom->Refresh();
	}
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


