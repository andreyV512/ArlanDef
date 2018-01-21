//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "Series.hpp"
#include <ComCtrls.hpp>
#include <windows.h>
#include <IniFiles.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include "TeeGDIPlus.hpp"
#include "Filters.h"

#include "ADCSettings.h"
#include "Protocol.h"
#include "View.h"
#include "Filters.h"
#include "Settings.h"
#include "Manage.h"
#include "Statistics.h"
#include "SolidGroup.h"
#include "SolidGroupTest.h"
#include "SolidGroupGraphics.h"
#include "ComPort.h"
#include <dir.h>
#include "ThreadOnLine.h"
#include "CommPort.h"
#include "ABOUT_NTC_NK_URAN.h"
#include <XPMan.hpp>
#include "LCARD502.h"
#include "Inverter.h"
#include "uFRHistory.h"
#include "ViewEtalons.h"
#include <Menus.hpp>
#include <Grids.hpp>
#include "CommPort.h"
#include <ComCtrls.hpp>
#include <vector>
#include "clustering.h"
#include "ThreadOnLine.h"
#include "Spectroscope.h"
#include "SignalListDef.h"
#include "InitSG.h"


class ThreadOnLine;
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *bTest;
	TButton *bWork;
	TButton *bView;
	TButton *bManage;
	TButton *bCrashForward;
	TButton *bReturnMode;
	TButton *bCancelWork;
	TButton *bStatsToNull;
	TMainMenu *MainMenu;
	TMenuItem *menuJob;
	TMenuItem *menuTube;
	TMenuItem *menuSettings;
	TMenuItem *menuCrashForward;
	TMenuItem *menuView;
	TMenuItem *menuTypeSize;
	TMenuItem *menuMeasures;
	TMenuItem *menuWork;
	TMenuItem *menuReturnMode;
	TMenuItem *menuQuit;
	TMenuItem *menuDiagnostics;
	TMenuItem *menuTest;
	TMenuItem *menuSignalsState;
	TMenuItem *menuProtocol;
	TMenuItem *menuSaveTube;
	TMenuItem *menuLoadTube;
	TMenuItem *menuClearCharts;
	TMenuItem *menuManagement;
	TMenuItem *menuTestAdvantech;
	TMenuItem *menuHelp;
	TMenuItem *menuF1;
	TMenuItem *menuColors;
	TMenuItem *menuTestASUConnection;
	TGroupBox *gbCrossDef;
	TGroupBox *gbLinearDef;
	TGroupBox *gbThickness;
	TGroupBox *gbSummary;
	TChart *CrossDefectChart;
	TChart *LinearDefectChart;
	TChart *ThicknessChart;
	TChart *SummaryChart;
	TEdit *eCrossBorder1;
	TEdit *eCrossBorder2;
	TEdit *eLinearBorder2;
	TEdit *eLinearBorder1;
	TEdit *eThicknessBorder2;
	TEdit *eThicknessBorder1;
	TEdit *eMin_Good_Length;
	TEdit *eCrossDeadZoneStart;
	TEdit *eCrossDeadZoneFinish;
	TEdit *eLinearDeadZoneFinish;
	TEdit *eLinearDeadZoneStart;
	TStatusBar *StatusBarBottom;
	TStatusBar *StatusBarTop;
	TCheckBox *cbLinear;
	TCheckBox *cbInterruptView;
	TCheckBox *cbEtalon;
	TSaveDialog *SaveToFileDialog;
	TOpenDialog *OpenDialogFromFile;
	TLabel *lCut1;
	TLabel *lCut2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label13;
	TPanel *pStatistics;
	TLabeledEdit *eValid;
	TLabeledEdit *eBrack;
	TLabeledEdit *eBrackInDaStreet;
	TComboBox *cbTypeSize;
	TButton *bGoodTube;
	TPanel *pSolidGroup;
	TApplicationEvents *ApplicationEvents;
	TButton *Button2;
	TEdit *Edit1;
	TMemo *Memo1;
	TButton *bBadTube;
	TLabel *Label14;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *Label3;
	TLabel *Label5;
	TBarSeries *Series1;
	TBarSeries *Series2;
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
	TBarSeries *Series14;
	TBarSeries *Series15;
	TBarSeries *Series16;
	TBarSeries *Series17;
	TBarSeries *Series18;
	TBarSeries *Series19;
	TBarSeries *Series20;
	TBarSeries *Series21;
	TBarSeries *Series22;
	TButton *bManualSG;
	TMenuItem *N1;
	TPopupMenu *OtherTest;
	TMenuItem *TestRotation;
	TMenuItem *SolidGroup;
	TPanel *lResult;
	TXPManifest *XPManifest1;
	TMenuItem *N2;

	TFRHistory *frHistory;
	TMenuItem *MIASMTest;
	TTimer *TimerThreadComplete;
	TLabel *lNN;
	TPanel *pNN;			// очищает все графики
	void __fastcall MainFormInit(TObject *Sender);			// выравнивает объекты на форме, работает в начале работы и при Resize
// стандартные функции
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormActivate(TObject *Sender);
// нажатие пунктов главного меню
	void __fastcall menuTypeSizeClick(TObject *Sender);
	void __fastcall menuMeasuresClick(TObject *Sender);
	void __fastcall menuQuitClick(TObject *Sender);
	void __fastcall menuSignalsStateClick(TObject *Sender);
	void __fastcall menuSaveTubeClick(TObject *Sender);
	void __fastcall menuLoadTubeClick(TObject *Sender);
	void __fastcall menuSolidGroupClick(TObject *Sender);
	void __fastcall menuTestAdvantechClick(TObject *Sender);
	void __fastcall menuF1Click(TObject *Sender);
	void __fastcall menuAboutClick(TObject *Sender);
	void __fastcall menuViewEtalonsClick(TObject *Sender);
	void __fastcall menuProtocolClick(TObject *Sender);
	void __fastcall menuColorsClick(TObject *Sender);
	void __fastcall menuSGTestClick(TObject *Sender);
	void __fastcall menuTestASUConnectionClick(TObject *Sender);
	void __fastcall menuGraphicsSGClick(TObject *Sender);
// нажатие кнопок на форме
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bViewClick(TObject *Sender);
	void __fastcall bWorkClick(TObject *Sender);
	void __fastcall bCancelWorkClick(TObject *Sender);
	void __fastcall bManageClick(TObject *Sender);
	void __fastcall bGoodTubeClick(TObject *Sender);
	void __fastcall bStatsToNullClick(TObject *Sender);
	void __fastcall ChartClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall cbEtalonClick(TObject *Sender);
	void __fastcall eSettingsChange(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall ReportViewClick(TObject *Sender);
	void __fastcall eBrackInDaStreetChange(TObject *Sender);
	void __fastcall bBadTubeClick(TObject *Sender);
	void __fastcall bManualSGClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall TestRotationClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall MIASMTestClick(TObject *Sender);
	void __fastcall menuTestSpectroscopeClick(TObject *Sender);
	void __fastcall menuSpectroSettingsClick(TObject *Sender);
	void __fastcall cbSpectrotestClick(TObject *Sender);
	void __fastcall TimerThreadCompleteTimer(TObject *Sender);
	void __fastcall SolidGroupClick(TObject *Sender);
	void __fastcall pSolidGroupClick(TObject *Sender);

private:	// User declarations
   ISG_DLL *sg;
	// настройки типоразмеров
	void LoadSettings(TObject *Sender);		// читает настройки из ini файла, заполняет едиты, устанавли пороги, мертвые зоны
	TIniFile *ini;							// ини файл с настройками
	void UpdateComboBox();					// обновляет ComboBox с типоразмерами (после добавления или удаления, в начале работы)

	// работа с группой прочности
	int tempSGbegin;						// температура модуля ГП в начале цикла работы
	void SetSolidGroup(TObject *Sender);	// читает эталоны определенного типоразмера из БД и инициализирует их
	SolidGroups::Analyzer anal;				// анализатор группы прочности
	// ЛИР
	ThreadOnLine *workonline;				// тред отрисовки в реальном времени
	// блокировка
	void SetAbleButtons(bool state , TButton *exc1 = NULL, TMenuItem *exc2 = NULL,
					TButton *exc3 = NULL, TButton *exc4 = NULL);	// блокировка/разблокировка клавиш
	void SetProjectSpecialty();				// устанавливает характеристики проекта
	bool GoodTube;							// оператор меняет результат принудительно на Годно
	bool BadTube;							// оператор меняет результат на Брак
	Statistics *stats;						// экземпляр статистики прогона труб
	void UpdateStats();						// обновить статистику
	vector<double> Linear_Signal;			//убрать после отладки фильтра
	// костыли ГП Пыть-ях
	int manual_num;
	String manual_name;
	void ReadFromFile(UnicodeString path);
			//для реализации Drag & Drop
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles) END_MESSAGE_MAP(TForm);
	bool IsSendResultToProtocol;

public:		// User declarations
	void IdentifySolidGroup();				// определяет группу прочности прошедешей трубы
	SolidGroups::Tube tubeSG;				// труба с параметрами группы прочности
	__fastcall TMainForm(TComponent* Owner);

	void __fastcall CompleteWork(TObject *Sender);		// событие OnTerminate для треда работы
	void UpdateStatus(String _text1,String _text2);
	void GetTubeNumber(void);
	void ClearCharts(void);
	void SendResultToASM(void);
	// Спектроскоп
  //	Spectroscope *spectroscope;
	void NextTube(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
extern bool LastSynchronizeReturnCode;
//---------------------------------------------------------------------------
#endif

