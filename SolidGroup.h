//---------------------------------------------------------------------------

#ifndef SolidGroupH
#define SolidGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <math.h>
#include <windows.h>
#include <vector>
#include <map>
#include "TeeGDIPlus.hpp"

#include "DataModule.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSolidGroupForm : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cbSolidGroup;
	TComboBox *cbTypeSize;
	TButton *bAddInserted;
	TButton *bAddCurrentTube;
	TLabel *Label1;
	TLabel *Label2;
	TStatusBar *StatusBarBottom;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bAddInsertedClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall bAddCurrentTubeClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	vector<TEdit*> par;						// массив динамических элементов для занесения трубы
	SolidGroups::Tube *t;				// вносимая вручную труба
	SolidGroups::Tube tube_current;		// труба, которая прошла тест
	vector <double> coord;				// вектор параметров группы прочности, 8 значений
	int numCoord;
public:		// User declarations
	void Send_Tube(SolidGroups::Tube temp_tube);
	__fastcall TSolidGroupForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSolidGroupForm *SolidGroupForm;
//---------------------------------------------------------------------------
#endif
