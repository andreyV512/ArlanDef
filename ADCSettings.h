//---------------------------------------------------------------------------

#ifndef ADCSettingsH
#define ADCSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "cspin.h"
#include "IniFiles.hpp"
#include <AppEvnts.hpp>
#include "Global.h"
#include "Filters.h"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <vector>
#include <Registry.hpp>
//---------------------------------------------------------------------------
class TADCSettForm : public TForm
{
__published:	// IDE-managed Components
	TApplicationEvents *ApplicationEvents;
	TPageControl *PageControl;
	TTabSheet *ppPage;
	TGroupBox *gbPP;
	TGroupBox *gbCross;
	TLabel *lnumberChannel;
	TLabel *lamperage;
	TLabel *lTemp;
	TCSpinEdit *spCrossAmperage;
	TCSpinEdit *spCrossVoltage;
	TTabSheet *OtherPage;
	TCSpinEdit *SpinCurr;
	TCSpinEdit *SpinSG;
	TCheckBox *cbCalcSolidGroup;
	TPopupMenu *CopyValueCrossChannel;
	TMenuItem *CopyAllSensor;
	TPopupMenu *CopyValueLinearChannel;
	TMenuItem *CopyAllSensorLinear;
	TTabSheet *FilterPAge;
	TCheckBox *cbMedianFilter;
	TLabel *lWidthMedianFilter;
	TComboBox *ComboBoxWidthMF;
	TTabSheet *prPage;
	TPanel *pBottom;
	TButton *bSave;
	TButton *bExit;
	TEdit *eFriquency;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label2;
	TEdit *ppRate;
	TEdit *ppKadr;
	TGroupBox *gbPR;
	TGroupBox *gbLinear;
	TLabel *lnumberchannelLinear;
	TLabel *lLinearAmperage;
	TLabel *lLinearTemp;
	TCSpinEdit *spLinearAmperage;
	TCSpinEdit *spLinearVoltage;
	TGroupBox *GroupBox4;
	TLabel *Label10;
	TLabel *Label14;
	TComboBox *cbbSyncMode;
	TComboBox *cbbSyncStartMode;
	TLabel *Label15;
	TLabel *Label16;
	TRadioGroup *RadioGroup1;
	TCheckBox *cbIsFiltersPr;
	TGroupBox *gbButterworthPr;
	TLabel *Label11;
	TCSpinEdit *csOrderPr;
	TLabel *Label12;
	TEdit *eCutoffFreqPr;
	TLabel *Label13;
	TEdit *eCenterFreqPr;
	TLabel *Label17;
	TEdit *eWidthFreqPr;
	TRadioGroup *rgFilterSubTypePr;
	TLabel *Label18;
	TEdit *eRippleDbPr;
	TLabel *Label19;
	TEdit *eRolloffPr;
	TRadioGroup *rgFilterTypePr;
	TRadioGroup *RadioGroup2;
	TRadioGroup *rgFilterTypePp;
	TRadioGroup *rgFilterSubTypePp;
	TGroupBox *gbButterworthPp;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TCSpinEdit *csOrderPp;
	TEdit *eCutoffFreqPp;
	TEdit *eCenterFreqPp;
	TEdit *eWidthFreqPp;
	TEdit *eRippleDbPp;
	TEdit *eRolloffPp;
	TCheckBox *cbIsFiltersPp;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *eFriquencyRot;
	TLabel *Label7;
	TComboBox *cbSolidGroup;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall bExitClick(TObject *Sender);
	void __fastcall ppSizeBufferKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall eThresholdResistKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall CopyAllSensorClick(TObject *Sender);
	void __fastcall ContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall CopyAllSensorLinearClick(TObject *Sender);
	void __fastcall rgFilterTypePrClick(TObject *Sender);
	void __fastcall rgFilterSubTypePrClick(TObject *Sender);
	void __fastcall rgFilterTypePpClick(TObject *Sender);
	void __fastcall rgFilterSubTypePpClick(TObject *Sender);

private:
	// User declarations
	static const int lin_sensors = Globals::LinSensors;
	static const int Cross_sensors = Globals::Cross_sensors;
	TEdit		 *arGainPP		        [Cross_sensors];
	TComboBox 	 *arComboBoxPp	        [Cross_sensors];
	TCSpinEdit	 *arSpinEditPp	        [Cross_sensors];
	TLabel		 *arLabelPp		        [Cross_sensors];
	TComboBox 	 *arCollectedModePp		[Cross_sensors];
	TEdit 		 *arGainPR		        [lin_sensors];
	TComboBox 	 *arComboBoxPr	        [lin_sensors];
	TLabel		 *arLabelPr		        [lin_sensors];
	TCSpinEdit	 *arSpinEditPr	        [lin_sensors];
	TComboBox 	 *arCollectedModePr		[lin_sensors];
	int LinearItemIndex;
	int CrossItemIndex;
	TIniFile *ini;
	void block();
	void ChooseFilterPr();
	void BlockFilterParamPr(bool mask[]);
	void ChooseFilterPp();
	void BlockFilterParamPp(bool mask[]);
	bool fl;

	void FillAllControls();

public:		// User declarations
	__fastcall TADCSettForm(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TADCSettForm *ADCSettForm;
//---------------------------------------------------------------------------
#endif
