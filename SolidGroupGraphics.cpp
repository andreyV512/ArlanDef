//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SolidGroupGraphics.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TSGGraphForm *SGGraphForm;
//---------------------------------------------------------------------------
__fastcall TSGGraphForm::TSGGraphForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSGGraphForm::StringGridParametersSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect)
{
	// заполняем столбцы просчитанными параметрами
	vector <double> params = Globals::tubeSG.Coordinates();
	for ( unsigned i = 0; i < Globals::tubeSG.dimension(); i++)
		StringGridParameters->Cells[0][i] =  FloatToStrF(params[i],ffGeneral, 6,0);

	// рисуем магнитный сигнал по конкретному параметру
	int current = ARow;
	vector <double> local = Globals::tubeSG.getParameters(current);
	ParameterChart->Series[0]->Clear();
	ParameterChart->Series[1]->Clear();
	ParameterChart->Series[2]->Clear();
	if (local.size())
	{
		for (unsigned i = 0; i < local.size(); i++)
			ParameterChart->Series[0]->Add(local[i]);
	// рисуем границы, которые мы отрезаем
		int max = ParameterChart->LeftAxis->Maximum;
		int min = ParameterChart->LeftAxis->Minimum;
		ParameterChart->Series[1]->AddXY(local.size()/8, max, "", clGreen);
		ParameterChart->Series[1]->AddXY(local.size()/8, min, "", clGreen);
		ParameterChart->Series[2]->AddXY(local.size()*7/8, max, "", clGreen);
		ParameterChart->Series[2]->AddXY(local.size()*7/8, min, "", clGreen);
		ParameterChart->Refresh();
	}
}
//---------------------------------------------------------------------------
void __fastcall TSGGraphForm::FormShow(TObject *Sender)
{
	bool sel = true;
	StringGridParametersSelectCell(Sender,0,0,sel);
}
//---------------------------------------------------------------------------
void __fastcall TSGGraphForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 27)
		SGGraphForm->Close();
}
//---------------------------------------------------------------------------


