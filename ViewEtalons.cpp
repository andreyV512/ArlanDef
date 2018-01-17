//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewEtalons.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormViewEtalons *FormViewEtalons;
//---------------------------------------------------------------------------
__fastcall TFormViewEtalons::TFormViewEtalons(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormViewEtalons::ViewClick(TObject *Sender)
{
	String WhereStr="";

	if ( (ComboBoxSG->Text == "Все") && (ComboBoxTS->Text == "Все") )
		WhereStr="";
	else
	if ( (ComboBoxSG->Text != "Все") && (ComboBoxTS->Text == "Все") )
		WhereStr="where Name='"+ComboBoxSG->Text+"'";
	else
	if ( (ComboBoxSG->Text == "Все") && (ComboBoxTS->Text != "Все") )
		WhereStr="where TypeSize='"+ComboBoxTS->Text+"'";
	else
	if ( (ComboBoxSG->Text != "Все") && (ComboBoxTS->Text != "Все") )
		WhereStr="where ((Name='"+ComboBoxSG->Text+"') and (TypeSize='"+ComboBoxTS->Text+"'))";

	DM->QEtalons->Close();
	DM->QEtalons->Active=false;
	DM->QEtalons->SQL->Clear();
	//DM->QEtalons->SQL->Add("Select  from dbo.SGEtalon "+WhereStr);
	DM->QEtalons->SQL->Add("select e.ID,name,p1,p2,p3,p4,p5,p6,p7,p8,TypeSize,Date");

	TADODataSet *ds = new TADODataSet(NULL);
	ds->Connection = DM->ADOCon;
	ds->CommandText = "select elem from SGEtalonChem order by elem";
	ds->Open();
	ds->First();
	while(!ds->Eof){
		UnicodeString elem = ds->FieldByName("elem")->AsString;
		DM->QEtalons->SQL->Add(",sum(case elem when '"+elem+"' then value else 0 end) as "+elem);
		ds->Next();
	}
	ds->Close();
	delete ds;

	DM->QEtalons->SQL->Add("from SGEtalon e ");
	DM->QEtalons->SQL->Add("left join SGEtalonChem c on e.ID=c.id_etalon");
    DM->QEtalons->SQL->Add(WhereStr);
	DM->QEtalons->SQL->Add("group by e.ID,name,p1,p2,p3,p4,p5,p6,p7,p8,TypeSize,Date");

	DM->QEtalons->ExecSQL();
	DM->QEtalons->Active=true;

	DBGridEtalons->DataSource=DM->DataEtalons;
	DBNavigatorEtalons->DataSource = DM->DataEtalons;
}
//---------------------------------------------------------------------------
void __fastcall TFormViewEtalons::ViewAllClick(TObject *Sender)
{
	DM->QEtalons->Close();
	DM->QEtalons->Active=false;
	DM->QEtalons->SQL->Clear();
	DM->QEtalons->SQL->Add("select e.ID,name,p1,p2,p3,p4,p5,p6,p7,p8,TypeSize,Date");

	TADODataSet *ds = new TADODataSet(NULL);
	ds->Connection = DM->ADOCon;
	ds->CommandText = "select elem from SGEtalonChem order by elem";
	ds->Open();ds->First();
	while(!ds->Eof){
		UnicodeString elem = ds->FieldByName("elem")->AsString;
		DM->QEtalons->SQL->Add(",sum(case elem when '"+elem+"' then value else 0 end) as "+elem);
		ds->Next();
	}
	ds->Close();
	delete ds;
	DM->QEtalons->SQL->Add("from SGEtalon e ");
	DM->QEtalons->SQL->Add("left join SGEtalonChem c on e.ID=c.id_etalon");
	DM->QEtalons->SQL->Add("group by e.ID,name,p1,p2,p3,p4,p5,p6,p7,p8,TypeSize,Date");

	DM->QEtalons->ExecSQL();
	DM->QEtalons->Active=true;

	DBGridEtalons->DataSource=DM->DataEtalons;
	DBNavigatorEtalons->DataSource = DM->DataEtalons;
}
//---------------------------------------------------------------------------

void __fastcall TFormViewEtalons::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		FormViewEtalons->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormViewEtalons::FormActivate(TObject *Sender)
{
 this->ComboBoxSG->Clear();
 this->ComboBoxTS->Clear();

 SolidGroups::Group g;
 TStringList * gr = g.getGroupNames();
 this->ComboBoxSG->Items->Add("Все");
 this->ComboBoxSG->Items->AddStrings(gr);
 this->ComboBoxSG->ItemIndex = 0;

 SolidGroups::TubeType tt;
 TStringList * ti = tt.getTubeTypeNames();
 this->ComboBoxTS->Items->Add("Все");
 this->ComboBoxTS->Items->AddStrings(ti);
 this->ComboBoxTS->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormViewEtalons::menuExportClick(TObject *Sender)
{
	map<String, int > _map;
	_map["D"] = -1;
	_map["K"] = 0;
	_map["E"] = 1;

	if (DBGridEtalons->SelectedRows->Count > 0)
	{
		if( SaveDialog->Execute() )
		{
			FILE* file;
			file = fopen(AnsiString(SaveDialog->FileName).c_str(), "w");
			if (file == NULL)
			{
				Application->MessageBoxW(L"Не удалось открыть файл для записи данных",L"Ошибка", MB_ICONERROR | MB_OK  );
				return;
			}
			else
			{
				int number=1;
				DM->QEtalons->First();
				while(!DM->QEtalons->Eof)
				{
					if( DBGridEtalons->SelectedRows->CurrentRowSelected )
					{
						if(number > 1)
                            fprintf(file,"\n");
						fprintf(file, "%d ",number);

						for(int j=0; j<8; j++)
							fprintf(file, "%d ", DM->QEtalons->FieldByName("p"+String(j+1))->AsInteger );

						fprintf(file,"%d",_map[DM->QEtalons->FieldByName("Name")->AsString]);
						number++;


					}
				DM->QEtalons->Next();
				}
			}
			fclose(file);
		}
	}
}
//----------------------------------------------------------------------------
void __fastcall TFormViewEtalons::N1Click(TObject *Sender)
{
	vector<int> IndexSelectedField;
	if (DBGridEtalons->SelectedRows->Count > 0)
	{
		DM->QEtalons->First();
		int index=0;
		while(!DM->QEtalons->Eof)
		{
			if( DBGridEtalons->SelectedRows->CurrentRowSelected )
				IndexSelectedField.push_back(index);
			index++;
			DM->QEtalons->Next();
		}
		GraphicCentersForm->IndexField.clear();
		GraphicCentersForm->IndexField = IndexSelectedField;
		GraphicCentersForm->Show();
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormViewEtalons::SelectAllClick(TObject *Sender)
{
	DM->QEtalons->First();
	while(!DM->QEtalons->Eof)
	{
		DBGridEtalons->SelectedRows->CurrentRowSelected = true;
		DM->QEtalons->Next();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormViewEtalons::menu60Click(TObject *Sender)
{
	if( OpenDialog->Execute() )
	{
		map<int, String > _map;
		_map[-1] = "D";
		_map[0] = "K";
		_map[1] = "E";
		FILE* file;
		file = fopen(AnsiString(OpenDialog->FileName).c_str(), "r");
		if (file == NULL)
		{
			Application->MessageBoxW(L"Не удалось открыть файл для чтения ",L"Ошибка", MB_ICONERROR | MB_OK  );
			return;
		}
		else
		{
			DM->tTemp->Active = true;
			while(!feof(file))
			{
				int index;
				fscanf(file,"%d", &index);       //номер строки - мусор
				vector<int> parameters;          //8 параметров
				for(int i=0; i <8; i++)
				{
					fscanf(file,"%d", &index);
					parameters.push_back(index);
				}

				if(parameters.size() != 8)
				{
					Application->MessageBoxW(L"Ошибка", L"Не удалось считать параметры ГП", MB_OK);
					break;
				}
				else
				{
					DM->tTemp->Append();
					for(int i=1; i<=8; i++)
						DM->tTemp->FieldByName("p"+IntToStr(i))->AsString = parameters[i-1];
				}
				fscanf(file,"%d", &index);
				DM->tTemp->FieldByName("Name")->AsString = _map[index];
				DM->tTemp->FieldByName("TypeSize")->AsString = IntToStr(( (TMenuItem *)Sender)->Tag);
                DM->tTemp->FieldByName("Date")->AsString = Date();
				DM->tTemp->Post();
			}
		}
		fclose(file);
	}
}
//---------------------------------------------------------------------------

