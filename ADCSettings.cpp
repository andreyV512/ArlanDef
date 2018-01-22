//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Filters.h"
#include "ADCSettings.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TADCSettForm *ADCSettForm;
//---------------------------------------------------------------------------
__fastcall TADCSettForm::TADCSettForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview=true;
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);

	int vmar = 40, hmar = 55;
	int height = 15, width = 60;
	for (int i = 0; i < Cross_sensors; i++)
	{
// создаем элементы для настроек поперечной платы
		arLabelPp[i] = new  TLabel( ADCSettForm->ppPage );
		arLabelPp[i] -> Parent = gbPP;
		arLabelPp[i] -> SetBounds(16, vmar + i*(height+20),width,height);
		arLabelPp[i] -> Visible = true;
		arLabelPp[i] -> Caption = "Датчик " + IntToStr(i+1);

		arComboBoxPp[i] = new TComboBox ( ADCSettForm->ppPage );
		arComboBoxPp[i] -> Parent = gbPP;
		arComboBoxPp[i] -> SetBounds(150, vmar + i*(height+20),width,height);
		arComboBoxPp[i] -> Name = L"Range" + IntToStr(i);
		arComboBoxPp[i] -> Items->Add(L"±10");
		arComboBoxPp[i] -> Items->Add(L"±5");
		arComboBoxPp[i] -> Items->Add(L"±2");
		arComboBoxPp[i] -> Items->Add(L"±1");
		arComboBoxPp[i] -> Visible = true;
		arComboBoxPp[i] -> Style=csDropDownList;
		arComboBoxPp[i] -> Font->Size=10;
		arComboBoxPp[i] -> ItemIndex = 0;
		arComboBoxPp[i] -> PopupMenu = CopyValueCrossChannel;
		arComboBoxPp[i] -> OnContextPopup = TContextPopupEvent(&ContextPopup);
		arComboBoxPp[i] -> Tag = 0;		//пригодится чтобы отделить продольный от поперечного в ContextPopup

		arSpinEditPp[i] = new  TCSpinEdit( ADCSettForm->ppPage );
		arSpinEditPp[i] -> Parent = gbPP;
		arSpinEditPp[i] -> SetBounds(100, vmar + i*(height+20),width,height);
		arSpinEditPp[i] -> Name = L"Channel" + IntToStr(i);
		arSpinEditPp[i] -> MinValue = 0;
		arSpinEditPp[i] -> MaxValue = 31;
		arSpinEditPp[i] -> Width = 35;
		arSpinEditPp[i] -> Font->Size=10;
		arSpinEditPp[i] -> AutoSize=false;
		arSpinEditPp[i] -> Visible = true;

		arGainPP[i] = new TEdit( ADCSettForm->ppPage );
		arGainPP[i] -> Parent = gbPP;
		arGainPP[i] -> SetBounds(220, vmar + i*(height+20),width,height+8);
		arGainPP[i] -> AutoSize=false;
		arGainPP[i] -> Name = L"ppGain" + IntToStr(i);
		arGainPP[i] -> Clear();
		arGainPP[i] -> OnKeyPress=ppSizeBufferKeyPress;
		arGainPP[i] -> Refresh();

		arCollectedModePp[i] = new TComboBox(ADCSettForm->ppPage);
		arCollectedModePp[i]->Parent = gbPP;
		arCollectedModePp[i]->SetBounds(290,vmar + i*(height+20),width+60,height+8);
		arCollectedModePp[i]->Name=L"CrossCollectedMode"+String(i);
		arCollectedModePp[i]->Items->Add("С общей землей");
		arCollectedModePp[i]->Items->Add("Дифференциальный");
		arCollectedModePp[i]->Items->Add("Измерение нуля");
		arCollectedModePp[i]->Visible = true;
		arCollectedModePp[i]->Style=csDropDownList;
		arCollectedModePp[i]->Font->Size=10;
		arCollectedModePp[i]->ItemIndex = 0;
		arCollectedModePp[i]->Tag =0;


	}
	for (int i = 0; i < lin_sensors; i++)
	{
// создаем элементы для настроек продольной платы
		arLabelPr[i] = new  TLabel( ADCSettForm->prPage );
		arLabelPr[i] -> Parent = gbPR;
		arLabelPr[i] -> SetBounds(16, vmar + i*(height+20),width,height);
		arLabelPr[i] -> Visible = true;
		arLabelPr[i] -> Caption = "Датчик " + IntToStr(i+1);

		arComboBoxPr[i] = new TComboBox ( ADCSettForm->prPage );
		arComboBoxPr[i] -> Parent = gbPR;
		arComboBoxPr[i] -> SetBounds(150, vmar + i*(height+20),width,height);
		arComboBoxPr[i] -> Name = L"prRange" + IntToStr(i);
		arComboBoxPr[i] -> Items->Add(L"±10");
		arComboBoxPr[i] -> Items->Add(L"±5");
		arComboBoxPr[i] -> Items->Add(L"±2");
		arComboBoxPr[i] -> Items->Add(L"±1");
		arComboBoxPr[i] -> Visible = true;
		arComboBoxPr[i] -> Style=csDropDownList;
		arComboBoxPr[i] -> Font->Size=10;
		arComboBoxPr[i] -> ItemIndex = 0;
		arComboBoxPr[i] -> PopupMenu = CopyValueLinearChannel;
		arComboBoxPr[i] -> OnContextPopup = TContextPopupEvent(&ContextPopup);
		arComboBoxPr[i] -> Tag = 1;


		arSpinEditPr[i] = new  TCSpinEdit( ADCSettForm->prPage );
		arSpinEditPr[i] -> Parent = gbPR;
		arSpinEditPr[i] -> SetBounds(100, vmar + i*(height+20),width,height);
		arSpinEditPr[i] -> Name = L"prChannel" + IntToStr(i);
		arSpinEditPr[i] -> MinValue = 0;
		arSpinEditPr[i] -> MaxValue = 31;
		arSpinEditPr[i] -> Width = 35;
		arSpinEditPr[i] -> Font->Size=10;
		arSpinEditPr[i] -> AutoSize=false;
		arSpinEditPr[i] -> Visible = true;

		arGainPR[i] = new TEdit( ADCSettForm->prPage );
		arGainPR[i] -> Parent = gbPR;
		arGainPR[i] -> SetBounds(220, vmar + i*(height+20),width,height+8);
		arGainPR[i] -> Name = L"prGain" + IntToStr(i);
		arGainPR[i] -> AutoSize=false;
		arGainPR[i] -> Clear();
		arGainPR[i] -> OnKeyPress=ppSizeBufferKeyPress;

		arCollectedModePr[i] = new TComboBox(ADCSettForm->prPage);
		arCollectedModePr[i]->Parent = gbPR;
		arCollectedModePr[i]->SetBounds(290,vmar + i*(height+20),width+60,height+8);
		arCollectedModePr[i]->Name=L"LinearCollectedMode"+String(i);
		arCollectedModePr[i]->Items->Add("С общей землей");
		arCollectedModePr[i]->Items->Add("Дифференциальный");
		arCollectedModePr[i]->Items->Add("Измерение нуля");
		arCollectedModePr[i]->Visible = true;
		arCollectedModePr[i]->Style=csDropDownList;
		arCollectedModePr[i]->Font->Size=10;
		arCollectedModePr[i]->ItemIndex = 0;
		arCollectedModePr[i]->Tag =0;


	}

	FillAllControls();
}
//---------------------------------------------------------------------------
void TADCSettForm::FillAllControls()
{
// читаем все настройки по датчикам из ini файла
	String gain_str = "Gain "+ IntToStr(Globals::current_diameter);
	for (int i = 0; i < Cross_sensors; i++)
	{
		arComboBoxPp[i] -> ItemIndex = (ini->ReadInteger("Range_PP_"+String(Globals::current_diameter),"Range"+IntToStr(i),1));
		arCollectedModePp[i] -> ItemIndex = ini->ReadInteger("CollectedMode_PP_"+String(Globals::current_diameter),"Mode"+IntToStr(i),1);
		arSpinEditPp[i] -> Value=ini->ReadInteger("PP","Number"+UnicodeString(i),i);
		arGainPP[i] -> Text = ini->ReadFloat( gain_str , "Gain"+IntToStr(i) , 10);

		Singleton::Instance()->CrossResult->SetSensorGain( StrToFloat(arGainPP[i]->Text) , i);
	}
	for (int i = 0; i < lin_sensors; i++)
	{
		arComboBoxPr[i] -> ItemIndex = ini->ReadInteger("Range_PR_"+String(Globals::current_diameter),"Range" + IntToStr(i),1);
		arCollectedModePr[i] -> ItemIndex = (ini->ReadInteger("CollectedMode_PR_"+String(Globals::current_diameter),"Mode"+IntToStr(i),1));
		arSpinEditPr[i] -> Value=StrToInt(ini->ReadInteger("PR","Number"+UnicodeString(i),i));
		arGainPR[i] -> Text=ini->ReadFloat("PR","Gain"+IntToStr(i),1);
		Singleton::Instance()->LinearResult->SetSensorGain( StrToFloat(arGainPR[i]->Text) , i);
	}
	this->ppKadr->Text=FloatToStr(ini->ReadFloat("LCard", "Kadr",0));
	this->ppRate->Text=FloatToStr(ini->ReadFloat("LCard", "Rate",1));
	this->eFriquency->Text = FloatToStr(ini->ReadFloat("LCard", "Friquency",1.0));
	cbbSyncStartMode->ItemIndex	= ini->ReadInteger("LCard", "SyncStartMode",0);

	cbSolidGroup->ItemIndex = cbSolidGroup->Items->IndexOf(ini->ReadString("OtherSettings","StandartSolidGroup","K"));
	//датчики группы прочности
	SpinSG->Value   = ini->ReadInteger("OtherSettings","SensorSG",16);
	SpinCurr->Value = ini->ReadInteger("OtherSettings","SensorCurr",17);
	//Вариант расчета ГП
	cbCalcSolidGroup->Checked = ini->ReadBool("OtherSettings","IsCalcSolid", 1);

	eFriquencyRot->Text = ini->ReadInteger("Default" , "SpeedInTest" , 10);

	cbMedianFilter->Checked = ini->ReadBool("OtherSettings","isMedianFilter", 0);
	ComboBoxWidthMF->ItemIndex = ComboBoxWidthMF->Items->IndexOf(ini->ReadInteger("OtherSettings","WidthMedianFilter",3));

	//заполняем настройки соленоидов для БУРАН 5000
	spCrossAmperage->Value  = ini->ReadInteger("PP", "amperageChannel", 3);
	spCrossVoltage->Value 	= ini->ReadInteger("PP", "voltageChannel",  4);

	spLinearAmperage->Value = ini->ReadInteger("PR", "amperageChannel", 5);
	spLinearVoltage->Value 	= ini->ReadInteger("PR", "voltageChannel",  6);

	//Настройки фильтров продольного
	cbIsFiltersPr->Checked 			= ini->ReadBool("Filters","isFilterPr", 0);
	rgFilterTypePr->ItemIndex 		= ini->ReadInteger("Filters","FilterTypePr", 0);
	rgFilterSubTypePr->ItemIndex 	= ini->ReadInteger("Filters","FilterSubTypePr", 0);
	csOrderPr->Value				= ini->ReadInteger("Filters","orderPr", 2);
	eCutoffFreqPr->Text 			= FloatToStr(ini->ReadFloat("Filters","cutoffFrequencyPr", 300.0));
	eWidthFreqPr->Text 	            = FloatToStr(ini->ReadFloat("Filters","widthFrequencyPr", 300.0));
	eCenterFreqPr->Text             = FloatToStr(ini->ReadFloat("Filters","centerFrequencyPr", 300.0));
	eRippleDbPr->Text	            = FloatToStr(ini->ReadFloat("Filters","rippleDbPr", 0.3));
	eRolloffPr->Text 				= FloatToStr(ini->ReadFloat("Filters","rolloffPr", 0.3));

	//Настройки фильтров поперечного
	cbIsFiltersPp->Checked 			= ini->ReadBool("Filters","isFilterPp", 0);
	rgFilterTypePp->ItemIndex 		= ini->ReadInteger("Filters","FilterTypePp", 0);
	rgFilterSubTypePp->ItemIndex 	= ini->ReadInteger("Filters","FilterSubTypePp", 0);
	csOrderPp->Value				= ini->ReadInteger("Filters","orderPp", 2);
	eCutoffFreqPp->Text 			= FloatToStr(ini->ReadFloat("Filters","cutoffFrequencyPp", 300.0));
	eWidthFreqPp->Text 	            = FloatToStr(ini->ReadFloat("Filters","widthFrequencyPp", 300.0));
	eCenterFreqPp->Text             = FloatToStr(ini->ReadFloat("Filters","centerFrequencyPp", 300.0));
	eRippleDbPp->Text	            = FloatToStr(ini->ReadFloat("Filters","rippleDbPp", 0.3));
	eRolloffPp->Text 				= FloatToStr(ini->ReadFloat("Filters","rolloffPp", 0.3));

}
//---------------------------------------------------------------------------

void TADCSettForm::block()
{
	gbCross->Enabled = fl;
	gbLinear->Enabled = fl;
	cbCalcSolidGroup->Enabled=fl;
	ComboBoxWidthMF->Enabled=fl;
	ppKadr->Enabled=fl;
	ppRate->Enabled=fl;
	SpinSG->Enabled=fl;
	SpinCurr->Enabled=fl;
	cbMedianFilter->Enabled = fl;
	for(int i=0; i<Cross_sensors; i++)
	{
		arSpinEditPp[i]->Enabled=fl;
		arGainPP[i] -> Enabled=fl;
	}
	for(int i=0; i<lin_sensors; i++)
	{
		arComboBoxPr[i]->Enabled=fl;
		arSpinEditPr[i]->Enabled=fl;
		arGainPR[i] -> Enabled=fl;
	}
}
//------------------------------------------------------------------------------
void __fastcall TADCSettForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
		//Проверяем на нажатие сочетания клавиш Ctrl+Alt+Enter
	if (Msg.message == WM_HOTKEY) // сообщение наше
		   if (Msg.wParam == 0x00F) // идентификатор наш
				if(fl) fl=false;
				else fl=true;
				block();
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::FormActivate(TObject *Sender)
{
	bool RHKret = RegisterHotKey(ADCSettForm->Handle,         // Handle окна, которому отправлять сообщения WM_HOTKEY
								0x00F,                 // УСЛОВНЫЙ идентификатор горячего ключа
								MOD_ALT + MOD_CONTROL, // модификаторы
								VK_RETURN              // код клавиши
								);
	FillAllControls();
	fl=false;
	block();
}
//------------------------------------------------------------------------------
void __fastcall TADCSettForm::bSaveClick(TObject *Sender)
{
	vector<double> ppGain,prGain;
	LCard_parameters param;
	String gain_str = "Gain "+ IntToStr(Globals::current_diameter);
	//Сохраняем данные по датчикам
	for(int i=0; i<Cross_sensors; i++)
	{
		Channel_parameters ch_param;
		ini->WriteInteger("Range_PP_"+String(Globals::current_diameter),"Range"+UnicodeString(i),StrToInt(arComboBoxPp[i] -> ItemIndex) );
		ini->WriteInteger("CollectedMode_PP_"+String(Globals::current_diameter),"Mode"+UnicodeString(i),StrToInt(arCollectedModePp[i] -> ItemIndex) );
		ini->WriteInteger("PP","Number"+UnicodeString(i),arSpinEditPp[i]->Value);
		ini->WriteFloat( gain_str , "Gain"+IntToStr(i) , StrToFloat(arGainPP[i]->Text) );
		ppGain.push_back(StrToFloat(arGainPP[i]->Text));

		ch_param.range = arComboBoxPp[i]->ItemIndex;
		ch_param.logicalChannel = arSpinEditPp[i]->Value;
		ch_param.collectedMode = arCollectedModePp[i] -> ItemIndex;
		param.channels.push_back(ch_param);
	}
	for(int i=0; i<lin_sensors; i++)
	{
		Channel_parameters ch_param;
		ini->WriteInteger("Range_PR_"+String(Globals::current_diameter),"Range"+UnicodeString(i),StrToInt(arComboBoxPr[i] -> ItemIndex) );
		ini->WriteInteger("CollectedMode_PR_"+String(Globals::current_diameter),"Mode"+UnicodeString(i),StrToInt(arCollectedModePr[i] -> ItemIndex) );
		ini->WriteInteger("PR","Number"+UnicodeString(i),arSpinEditPr[i]->Value);
		ini->WriteFloat("PR","Gain"+IntToStr(i),StrToFloat(arGainPR[i]->Text) );
		prGain.push_back(StrToFloat(arGainPR[i]->Text));

		ch_param.range = arComboBoxPr[i]->ItemIndex;
		ch_param.logicalChannel = arSpinEditPr[i]->Value;
		ch_param.collectedMode = arCollectedModePr[i]->ItemIndex;
		param.channels.push_back(ch_param);
	}
	Singleton::Instance()->CrossResult->SetGains(ppGain);
	Singleton::Instance()->LinearResult->SetGains(prGain);

	//сохраняем прочие настройки

	ini->WriteInteger("OtherSettings","SensorSG",     SpinSG->Value);
	ini->WriteInteger("OtherSettings","SensorCurr",   SpinCurr->Value);
	ini->WriteBool   ("OtherSettings","IsCalcSolid", cbCalcSolidGroup->Checked);
	Globals::defaultGroupCheck = cbCalcSolidGroup->Checked;

	ini->WriteBool("OtherSettings","isMedianFilter", cbMedianFilter->Checked);
	ini->WriteString("OtherSettings","StandartSolidGroup",cbSolidGroup->Text);
	ini->WriteInteger("OtherSettings","WidthMedianFilter",ComboBoxWidthMF->Text.ToInt());
	//Данные по соленоидам
	ini->WriteInteger("PP", "amperageChannel", spCrossAmperage->Value);
	ini->WriteInteger("PP", "voltageChannel",  spCrossVoltage->Value);

	ini->WriteInteger("PR", "amperageChannel", spLinearAmperage->Value);
	ini->WriteInteger("PR", "voltageChannel",  spLinearVoltage->Value);

	param.others[0].logicalChannel = spLinearAmperage->Value;
	param.others[1].logicalChannel = spLinearVoltage->Value;

	param.others[2].logicalChannel = spLinearAmperage->Value;
	param.others[3].logicalChannel = spLinearVoltage->Value;

	param.others[4].logicalChannel = SpinCurr->Value;
	param.others[5].logicalChannel = SpinSG->Value;

	ini->WriteFloat  ("LCard", "Kadr",StrToFloat(this->ppKadr->Text));
	ini->WriteFloat  ("LCard", "Rate",StrToFloat(this->ppRate->Text));
	ini->WriteFloat  ("LCard", "Friquency",StrToFloat(this->eFriquency->Text));
	ini->WriteInteger("LCard", "SyncMode",cbbSyncMode->ItemIndex);
	ini->WriteInteger("LCard", "SyncStartMode",cbbSyncStartMode->ItemIndex);

	param.frequencyPerChannel = StrToFloat(this->ppRate->Text);
	param.frequencyCollect = StrToFloat(this->eFriquency->Text);
	param.syncMode = cbbSyncMode->ItemIndex;
	param.syncStartMode = cbbSyncStartMode->ItemIndex;

	lcard->setSettings(param);

	param = lcard->getSettings();
	this->ppRate->Text = FloatToStrF(param.frequencyPerChannel,ffFixed,10,2);

	this->eFriquency->Text = FloatToStrF(param.frequencyCollect,ffFixed,10,2);

	ini->WriteBool("Filters","isFilterPr",cbIsFiltersPr->Checked);
	ini->WriteInteger("Filters","orderPr", csOrderPr->Value);
	ini->WriteFloat("Filters","cutoffFrequencyPr", StrToFloat(eCutoffFreqPr->Text));
	ini->WriteFloat("Filters","widthFrequencyPr", StrToFloat(eWidthFreqPr->Text));
	ini->WriteFloat("Filters","centerFrequencyPr", StrToFloat(eCenterFreqPr->Text));
	ini->WriteFloat("Filters","rippleDbPr", StrToFloat(eRippleDbPr->Text));
	ini->WriteFloat("Filters","rolloffPr", StrToFloat(eRolloffPr->Text));
	ini->WriteInteger("Filters","FilterTypePr", rgFilterTypePr->ItemIndex);
	ini->WriteInteger("Filters","FilterSubTypePr", rgFilterSubTypePr->ItemIndex);

	ini->WriteBool("Filters","isFilterPp",cbIsFiltersPp->Checked);
	ini->WriteInteger("Filters","orderPp", csOrderPp->Value);
	ini->WriteFloat("Filters","cutoffFrequencyPp", StrToFloat(eCutoffFreqPp->Text));
	ini->WriteFloat("Filters","widthFrequencyPp", StrToFloat(eWidthFreqPp->Text));
	ini->WriteFloat("Filters","centerFrequencyP", StrToFloat(eCenterFreqPp->Text));
	ini->WriteFloat("Filters","rippleDbPp", StrToFloat(eRippleDbPp->Text));
	ini->WriteFloat("Filters","rolloffPp", StrToFloat(eRolloffPp->Text));
	ini->WriteInteger("Filters","FilterTypePp", rgFilterTypePp->ItemIndex);
	ini->WriteInteger("Filters","FilterSubTypePp", rgFilterSubTypePp->ItemIndex);

	SystemConst::isCrossDigitalFilter = cbIsFiltersPp->Checked;
	SystemConst::isLinearDigitalFilter = cbIsFiltersPr->Checked;
	CrossFilter->setSettingsFromIniFile("Pp");
	LinearFilter->setSettingsFromIniFile("Pr");

	Singleton::Instance()->LinearResult->ComputeZoneData(true);
	Singleton::Instance()->LinearResult->PutResultOnChart(MainForm->LinearDefectChart, MainForm);

	Singleton::Instance()->CrossResult->ComputeZoneData();
	Singleton::Instance()->CrossResult->PutResultOnChart(MainForm->CrossDefectChart, MainForm);

	Singleton::Instance()->SumResult->ComputeZonesData();
	Singleton::Instance()->SumResult->PutResultOnChart(MainForm->SummaryChart, MainForm);
	Singleton::Instance()->SumResult->MakeDecision();
	MainForm->lResult->Caption = Singleton::Instance()->SumResult->decision;
	MainForm->lCut1->Caption   = Singleton::Instance()->SumResult->cut1;
	MainForm->lCut2->Caption   = Singleton::Instance()->SumResult->cut2;

	SystemConst::SetSystemConstants();
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::bExitClick(TObject *Sender)
{
	ADCSettForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::ppSizeBufferKeyPress(TObject *Sender, wchar_t &Key)
{
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ','))) Key = 0x00;
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormDestroy(TObject *Sender)
{
	for (int i = 0; i < Cross_sensors; i++)
	{
		delete arLabelPp[i];
		delete arGainPP[i];
		delete arComboBoxPp[i];
		delete arSpinEditPp[i];
	}
	for (int i = 0; i < lin_sensors; i++)
	{
		delete arGainPR[i];
		delete arComboBoxPr[i];
		delete arSpinEditPr[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::eThresholdResistKeyPress(TObject *Sender, wchar_t &Key)
{
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ','))) Key = 0x00;
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::CopyAllSensorClick(TObject *Sender)
{
	for(int i=0; i < Cross_sensors; i++)
		arComboBoxPp[i]->ItemIndex = CrossItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::ContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled)
{
	if( ((TComboBox *)Sender)->Tag == 0 )
		CrossItemIndex = ((TComboBox *)Sender)->ItemIndex;
	else
		LinearItemIndex = ((TComboBox *)Sender)->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::CopyAllSensorLinearClick(TObject *Sender)
{
	for(int i=0; i < lin_sensors; i++)
		arComboBoxPr[i]->ItemIndex = LinearItemIndex;
}
//---------------------------------------------------------------------------
void TADCSettForm::ChooseFilterPr()
{
	bool m[7] = {0,0,0,0,0,0};
	BlockFilterParamPr(m);
	switch(rgFilterTypePr->ItemIndex)
	{
		case 0:
		{
			if(rgFilterSubTypePr->ItemIndex <= 1)
				{m[0] = 1;m[2]=1;}
			else
				{m[0] = 1; m[1]=1; m[3]=1;}
			BlockFilterParamPr(m);
			break;
		}
		case 1:
		{
			if(rgFilterSubTypePr->ItemIndex <= 1)
				{m[0]=1; m[2]=1; m[4]=1;}
			else
				{m[0]=1; m[1]=1; m[3]=1; m[4]=1;}
			BlockFilterParamPr(m);
			break;
		}
		case 2:
		{
			if(rgFilterSubTypePr->ItemIndex <= 1)
				{m[0]=1; m[2]=1; m[4]=1; m[5]=1;}
			else
				{m[0]=1; m[1]=1; m[3]=1; m[4]=1; m[5]=1;}
			BlockFilterParamPr(m);
			break;
		}
	}
}
void TADCSettForm::BlockFilterParamPr(bool mask[])
{
	csOrderPr->Enabled = mask[0];
	eCenterFreqPr->Enabled = mask[1];
	eCutoffFreqPr->Enabled = mask[2];
	eWidthFreqPr->Enabled = mask[3];
	eRippleDbPr->Enabled = mask[4];
	eRolloffPr->Enabled = mask[5];
}
void __fastcall TADCSettForm::rgFilterTypePrClick(TObject *Sender)
{
	ChooseFilterPr();
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::rgFilterSubTypePrClick(TObject *Sender)
{
	ChooseFilterPr();
}
//---------------------------------------------------------------------------
void TADCSettForm::ChooseFilterPp()
{
	bool m[7] = {0,0,0,0,0,0};
	BlockFilterParamPp(m);
	switch(rgFilterTypePp->ItemIndex)
	{
		case 0:
		{
			if(rgFilterSubTypePp->ItemIndex <= 1)
				{m[0] = 1;m[2]=1;}
			else
				{m[0] = 1; m[1]=1; m[3]=1;}
			BlockFilterParamPp(m);
			break;
		}
		case 1:
		{
			if(rgFilterSubTypePp->ItemIndex <= 1)
				{m[0]=1; m[2]=1; m[4]=1;}
			else
				{m[0]=1; m[1]=1; m[3]=1; m[4]=1;}
			BlockFilterParamPp(m);
			break;
		}
		case 2:
		{
			if(rgFilterSubTypePp->ItemIndex <= 1)
				{m[0]=1; m[2]=1; m[4]=1; m[5]=1;}
			else
				{m[0]=1; m[1]=1; m[3]=1; m[4]=1; m[5]=1;}
			BlockFilterParamPp(m);
			break;
		}
	}
}
void TADCSettForm::BlockFilterParamPp(bool mask[])
{
	csOrderPp->Enabled = mask[0];
	eCenterFreqPp->Enabled = mask[1];
	eCutoffFreqPp->Enabled = mask[2];
	eWidthFreqPp->Enabled = mask[3];
	eRippleDbPp->Enabled = mask[4];
	eRolloffPp->Enabled = mask[5];
}
void __fastcall TADCSettForm::rgFilterTypePpClick(TObject *Sender)
{
	ChooseFilterPp();
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::rgFilterSubTypePpClick(TObject *Sender)
{
	ChooseFilterPp();
}
//---------------------------------------------------------------------------


