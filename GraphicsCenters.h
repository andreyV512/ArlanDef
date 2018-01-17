//---------------------------------------------------------------------------

#ifndef GraphicsCentersH
#define GraphicsCentersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <vector>
#include "Series.hpp"
#include "Chart.hpp"
#include "DataModule.h"
#include "ColorSettings.h"

//---------------------------------------------------------------------------
class TGraphicCentersForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	//сделал 10 как максимальное кол-во групп прочности
	TTabSheet *pages[10];
	TChart *chart[10];
	int PageCount;
	void DrawOnChart(String SolidGroup, int TypeSize,vector<int> coords);
	//вернет цвет которым рисовать график группы прочности
	TColor getColorSolidGroup(String solidGroup);
	//вернен номер чарта от данного типоразмера
	int getIndexChartSolidGroup(int TypeSize);

public:		// User declarations
	vector <int> IndexField;    //номера записей в запросе QEtalons для которых надо построить графики

	__fastcall TGraphicCentersForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphicCentersForm *GraphicCentersForm;
//---------------------------------------------------------------------------
#endif
