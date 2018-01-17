//---------------------------------------------------------------------------

#ifndef SolidGroupGraphicsH
#define SolidGroupGraphicsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include "clustering.h"
#include "Global.h"

//---------------------------------------------------------------------------
class TSGGraphForm : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGridParameters;
	TChart *ParameterChart;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	void __fastcall StringGridParametersSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations


public:		// User declarations

	__fastcall TSGGraphForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSGGraphForm *SGGraphForm;
//---------------------------------------------------------------------------
#endif
