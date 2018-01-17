//---------------------------------------------------------------------------

#ifndef SolidGroupTestH
#define SolidGroupTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "Main.h"
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <Dialogs.hpp>
#include <dir.h>
#include "clustering.h"
#include "LCARD502.h"
#include "BubbleCh.hpp"
#include "TeeFunci.hpp"
//---------------------------------------------------------------------------
class TSGForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *pTop;
	TStatusBar *StatusBar;
	TButton *bTest;
	TButton *bOpenTube;
	TButton *bSaveTube;
	TChart *SignalChart;
	TLabel *lTestTime;
	TEdit *eTestTime;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TButton *bShowCurrentTube;
	TPanel *BottomPanel;
	TPopupMenu *PopupMenuTestSG;
	TMenuItem *N1;
	TScrollBar *scbChartDragger;
	TPanel *pThresholds;
	TMenuItem *N2;
	TMenuItem *AddParameters;
	TLineSeries *Series;
	TLineSeries *Series1;
	TPointSeries *Series2;
	TPointSeries *Series3;
	TLabel *lMinScrollbar;
	TLabel *lMaxScrollbar;
	TCustomTeeFunction *TeeFunction1;
	TAverageTeeFunction *TeeFunction2;
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bOpenTubeClick(TObject *Sender);
	void __fastcall bSaveTubeClick(TObject *Sender);
	void __fastcall bShowCurrentTubeClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall tbZoomChange(TObject *Sender);
	void __fastcall scbChartDraggerChange(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
 //	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall AddParametersClick(TObject *Sender);
	void __fastcall SignalChartClick(TObject *Sender);
	void __fastcall SignalChartScroll(TObject *Sender);
private:	// User declarations
	static const int quantity_thresholds = 8;
	vector<double> buffer;
	TEdit *eThresholds[quantity_thresholds];    //Поля для отображения порогов
	void DrawOnChart();							//рисует на чарте
	void DrawThresholds();						// рисует пороги на чарте
	TLineSeries *thresholds[8];					// массив серий с порогами
	vector <double> Signal;						//Вектор с сигналом
	void SaveToFile(UnicodeString FileName);	//сохраняет трубу
	SolidGroups::Tube tube_sg;					// труба с характеристиками группы прочности
	long curPos;
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles) END_MESSAGE_MAP(TForm);
public:		// User declarations
	void LoadFromFile(UnicodeString FileName);	//открывает трубу
	__fastcall TSGForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSGForm *SGForm;
//---------------------------------------------------------------------------
#endif
