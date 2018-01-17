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

	void ShowCentralizerOpened	( TBitBtn *btn, TPanel *pan );	// ����������� ������������ ���������� (������)
	void ShowCentralizerClosed	( TBitBtn *btn, TPanel *pan );	// ����������� ���������� ���������� (������)
	void ShowModuleinWork		( TBitBtn *btn, TPanel *pan );	// ����������� ������ � ������ �� (������� ���������)
	void ShowModuleinService	( TBitBtn *btn, TPanel *pan );	// ����������� ������ � ������ �� (��������� ������������)
	void ShowModuleMoving		( TBitBtn *btn, TPanel *pan );	// ����������� ������ � ��������
	void ShowModuleState		( byte state, TBitBtn *btn, TPanel *pan );	// ����������� ��������� ������
// GAN **********************
	void ShowLoaderUp	     	( TBitBtn *btn, TPanel *pan );  				// ����������� ��������� ������ ������������ ������
	void ShowLoaderDown	    	( TBitBtn *btn, TPanel *pan );  				// ����������� ��������� ������ ������������ �����
	void ShowLoaderMoving      	( TBitBtn *btn, TPanel *pan );  				// ����������� �������� ������ ������������
	void ShowLoaderState		( byte state, TBitBtn *btn, TPanel *pan ); 		// ����������� ��������� ������ ������������
	void ShowLoadPosition		( byte position, TPanel *pan1, TPanel *pan2 ); 	// ����������� ��������� �������� �������� (������ ��� �����)
	void ShowLoadPosAllowed		( TPanel *pan1, TPanel *pan2 );  // ����������� ����, ��� �������� ������������ ���������
	void ShowLoadPosDenied		( TPanel *pan1, TPanel *pan2 );  // ����������� ����, ��� �������� ������������ ���������


	bool is_pix_ok;						// ��������� �� �������� ��� ������
	bool butt_enabled;					// ���������� ������ ��� �������� �����
	TColor on, off, move; 		   	    // ����� ��� , ���� ������� � �������� ������
	String centr_on, centr_off;			// ������� ��� � ���� �����������
	String centr_close, centr_open;		// ������� ������ ��� � ���� �����������
	String mod_on, mod_off, mod_mov;	// ������� ������ � ������, � �� � � ��������
	String mod_work, mod_service, mod_dsbl;			// ������� ������ �������� � ��������� �������
// GAN *********************
	String load_Up, load_Down, load_mow, load_st, load_st1, load_work;  	// ������� �������������
	String load_offoff, load_onon;   						  	// ������� �������
	Graphics::TBitmap *load_UP, *load_Dwn, *load_mvg;  			// ������� ��� ������ �������������

	Graphics::TBitmap *centr_cl, *centr_op;						// ������� ��� ������ �����������
	Graphics::TBitmap *mod_serv, *mod_wrk,*mod_mvg;				// ������� ��� ������ c���� ��������� �������

public:		// User declarations

	__fastcall TManageForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TManageForm *ManageForm;
//---------------------------------------------------------------------------
#endif
