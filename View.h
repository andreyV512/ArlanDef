//---------------------------------------------------------------------------

#ifndef ViewH
#define ViewH
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
#include <AppEvnts.hpp>

#include "Result.h"
#include "Global.h"
#include <Menus.hpp>

//---------------------------------------------------------------------------

class TViewForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *pCrossZone;
	TPanel *pLinearZone;
	TGroupBox *gbCross;
	TGroupBox *gbLinear;
	TChart *ViewLinearChart;
	TChart *ViewCrossChart;
	TBarSeries *BarSeries1;
	TLineSeries *Series2;
	TChart *Chart1;
	TApplicationEvents *ApplicationEvents;
	TBarSeries *Series14;
	TBarSeries *Series15;
	TBarSeries *Series16;
	TBarSeries *Series17;
	TBarSeries *Series18;
	TBarSeries *Series19;
	TBarSeries *Series20;
	TBarSeries *Series1;
	TBarSeries *Series3;
	TBarSeries *Series4;
	TBarSeries *Series5;
	TBarSeries *Series6;
	TBarSeries *Series7;
	TBarSeries *Series8;
	TBarSeries *Series9;
	TBarSeries *Series10;
	TBarSeries *Series11;
	TBarSeries *Series12;
	TBarSeries *Series13;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ViewCrossChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ViewLinearChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ChartClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall ViewCrossChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ViewLinearChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);


private:	// User declarations
	TChart * arc[Globals::max_sensors];	// ������ ������ ��� ����������� ���� �� ��������
	void  ViewFormInit();				// ������������� �����
	short total_charts;					// ����� ���-�� ������������ ������
	TEdit *arGain[Globals::max_sensors];// ������ ������ � ���������� �� ������ (�������)
	TIniFile *ini;
	bool repaint;						// ���� ������� �������� - ����������
	int CrossPos;						//��������� ������� �����������
	int LinearPos;						//��������� ������� �����������
	int SelectedColor;                  //���� ��������� ����
	bool GainEnable;
public:		// User declarations
	__fastcall TViewForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TViewForm *ViewForm;
//---------------------------------------------------------------------------
#endif


