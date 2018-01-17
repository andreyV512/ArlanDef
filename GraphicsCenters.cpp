//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GraphicsCenters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGraphicCentersForm *GraphicCentersForm;
//---------------------------------------------------------------------------
__fastcall TGraphicCentersForm::TGraphicCentersForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGraphicCentersForm::FormCreate(TObject *Sender)
{
//	������� ���������� ������������
	DM->Find->Close();
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add("Select DISTINCT TypeSize From dbo.SGEtalon");
	DM->Find->ExecSQL();
	DM->Find->Active = true;
	//�������� ������� �������/������ ������� ��������� ����� ��������� ���� � ���� ������
	PageCount = DM->Find->RecordCount;
	for(int i=0; i < DM->Find->RecordCount; i++)
	{
		pages[i] = new TTabSheet(this);
		pages[i]->Parent = this;
		pages[i]->PageControl = PageControl;
		pages[i]->Name = "page_"+IntToStr(DM->Find->FieldByName("TypeSize")->AsInteger);
		pages[i]->Caption = DM->Find->FieldByName("TypeSize")->AsString;

		chart[i] = new TChart(pages[i]);
		chart[i]->Parent = pages[i];
		chart[i]->Name = "chart_"+IntToStr(DM->Find->FieldByName("TypeSize")->AsInteger);
		chart[i]->Tag = DM->Find->FieldByName("TypeSize")->AsInteger;
		chart[i]->Align = alClient;
		chart[i]->Legend->Visible = false;
		chart[i]->BottomAxis->Minimum = 0;
		chart[i]->BottomAxis->Maximum = 80;
		chart[i]->View3D = false;
		chart[i]->Color = clWhite;
		DM->Find->Next();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGraphicCentersForm::FormShow(TObject *Sender)
{
	for(int i=0; i < PageCount; i++)
	{
		for(int j=0; j < chart[i]->SeriesCount(); j++)
			{
				chart[i]->Series[j]->Clear();
				chart[i]->Series[j]->RefreshSeries();
			}
		pages[i]->Refresh();
	}
	int size = IndexField.size();
	//� query ������ ������� � ���������� ������, ��� ��� ��� ����� �������
	for(int i=0; i < IndexField.size(); i++)         //������� ������ � �������� ������� ������� ������� ������������
	{
    	int currentRecord = 0;
		DM->QEtalons->First();
		while(!DM->QEtalons->Eof)
		{
			int number = IndexField[i];
			if( IndexField[i] == currentRecord )    //������ �� � �������� �� �������
			{
				vector<int> coords;
				for(int p=0; p < 8; p++)
				{
					int par = DM->QEtalons->FieldByName("p"+IntToStr(p+1))->AsInteger;
					coords.push_back(DM->QEtalons->FieldByName("p"+IntToStr(p+1))->AsInteger);
				}
				String sg = DM->QEtalons->FieldByName("Name")->AsString;
				int ts = DM->QEtalons->FieldByName("TypeSize")->AsInteger;
				DrawOnChart(sg, ts , coords);

				DM->QEtalons->Next();
				break;
			}
			else
			DM->QEtalons->Next();

		currentRecord++;
		}
	}
}
//---------------------------------------------------------------------------
void TGraphicCentersForm::DrawOnChart(String SolidGroup, int TypeSize, vector<int> coords)
{
	int indexChart = getIndexChartSolidGroup(TypeSize);     //��������� �� ����� �� ������ ����� ��������

	chart[indexChart]->AddSeries(new TLineSeries(Owner));  //�������� ����� �� ���� �����
	int seriesIndex = chart[indexChart]->SeriesCount()-1;  //��������� ������� ����� ��� ������� �� ������� �����
	//�������� ������ �� ������ ��������� ��������������� ������
	for(int i = 0; i < coords.size(); i++)
		((TLineSeries*)chart[indexChart]->Series[seriesIndex])->AddXY(i, coords[i],L"", getColorSolidGroup(SolidGroup));
}
//---------------------------------------------------------------------------
TColor TGraphicCentersForm::getColorSolidGroup(String solidGroup)
{
	//���� ��� �� ���������� ������ ������� ����� ���������� ������, � �� ����� �� �������
	if( (solidGroup == "K") || (solidGroup == "�") )
		return FormColor->SolidGroup_K;
	else
	if( (solidGroup == "D") || (solidGroup == "�") )
		return FormColor->SolidGroup_D;
	else
	if( (solidGroup == "E") || (solidGroup == "�") )
		return FormColor->SolidGroup_E;
	else
		return clBlack;
}
//---------------------------------------------------------------------------
int TGraphicCentersForm::getIndexChartSolidGroup(int TypeSize)
{
	for(int i=0; i < PageCount; i++)
		if( chart[i]->Tag == TypeSize )
			return i;

	return 0;
}
//---------------------------------------------------------------------------
void __fastcall TGraphicCentersForm::FormDestroy(TObject *Sender)
{
	for(int i=0; i < PageCount; i++)
	{
		for(int j=0; j < chart[i]->SeriesCount(); j++)
			delete chart[i]->Series[j];
		delete chart[i];
		delete pages[i];
	}
}
//---------------------------------------------------------------------------

