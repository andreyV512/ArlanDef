//---------------------------------------------------------------------------

#ifndef ManageH
#define ManageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
class TManageForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gbLinear;
	TGroupBox *gbCross;
	TStatusBar *StatusBarBottom;
	TBitBtn *bRotation;
	TTimer *TemperatureTimer;
	TPanel *pCrossSolenoidNew;
	TLabel *lCrossAmperage;
	TLabel *lCrossVoltage;
	TLabel *lCrossResistance;
	TEdit *eCrossAmperage;
	TEdit *eCrossVoltage;
	TEdit *eCrossTemperature;
	TPanel *pLinearSolenoid;
	TLabel *lLinearAmperage;
	TLabel *lLinearVoltage;
	TLabel *lLinearResistance;
	TEdit *eLinearAmperage;
	TEdit *eLinearVoltage;
	TEdit *eLinearTemperature;
	TLabel *Label1;
	TEdit *eFriquencyRot;
	TTimer *Timer;
	TBitBtn *bCrossSolenoid;
	TBitBtn *bLinearSolenoid;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bLinearSolenoidClick(TObject *Sender);
	void __fastcall bCrossSolenoidClick(TObject *Sender);
	void __fastcall bRotationClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);


private:	// User declarations

	void ShowCentralizerOpened	( TBitBtn *btn, TPanel *pan );	// отображение разомкнутого центратора (схвата)
	void ShowCentralizerClosed	( TBitBtn *btn, TPanel *pan );	// отображение замкнутого центратора (схвата)
	void ShowModuleinWork		( TBitBtn *btn, TPanel *pan );	// отображение модуля в режиме РП (рабочее положение)
	void ShowModuleinService	( TBitBtn *btn, TPanel *pan );	// отображение модуля в режиме ПО (положение обслуживания)
	void ShowModuleMoving		( TBitBtn *btn, TPanel *pan );	// отображение модуля в движении
	void ShowModuleState		( byte state, TBitBtn *btn, TPanel *pan );	// отображение состояния модуля
// GAN **********************
	void ShowLoaderUp	     	( TBitBtn *btn, TPanel *pan );  				// отображение положения ЛЮБОГО перекладчика вверху
	void ShowLoaderDown	    	( TBitBtn *btn, TPanel *pan );  				// отображение положения ЛЮБОГО перекладчика внизу
	void ShowLoaderMoving      	( TBitBtn *btn, TPanel *pan );  				// отображение движения ЛЮБОГО перекладчика
	void ShowLoaderState		( byte state, TBitBtn *btn, TPanel *pan ); 		// отображение состояния ЛЮБОГО перекладчика
	void ShowLoadPosition		( byte position, TPanel *pan1, TPanel *pan2 ); 	// отображение состояния концевых датчиков (начало или конец)
	void ShowLoadPosAllowed		( TPanel *pan1, TPanel *pan2 );  // отображение того, что движение перекладчика разрешено
	void ShowLoadPosDenied		( TPanel *pan1, TPanel *pan2 );  // отображение того, что движение перекладчика запрещено


	bool is_pix_ok;						// загружены ли картинки для кнопок
	bool butt_enabled;					// блокировка кнопок при движении трубы
	TColor on, off, move; 		   	    // цвета вкл , выкл сигнала и движения модуля
	String centr_on, centr_off;			// надписи вкл и выкл центраторов
	String centr_close, centr_open;		// надписи кнопок вкл и выкл центраторов
	String mod_on, mod_off, mod_mov;	// надписи модуль в работе, в по и в движении
	String mod_work, mod_service, mod_dsbl;			// надписи кнопок поднятия и опускания модулей
// GAN *********************
	String load_Up, load_Down, load_mow, load_st, load_st1, load_work;  	// надписи перекладчиков
	String load_offoff, load_onon;   						  	// надписи панелей
	Graphics::TBitmap *load_UP, *load_Dwn, *load_mvg;  			// рисунки для кнопок перекладчиков

	Graphics::TBitmap *centr_cl, *centr_op;						// рисунки для кнопок центраторов
	Graphics::TBitmap *mod_serv, *mod_wrk,*mod_mvg;				// рисунки для кнопок cмены состояния модулей

public:		// User declarations

	__fastcall TManageForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TManageForm *ManageForm;
//---------------------------------------------------------------------------
#endif
