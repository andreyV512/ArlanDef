// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "clustering.h"
// #include "About.h"
#include "ColorSettings.h"
#include "ReportsView.h"
#include "ViewEtalons.h"
#include "SMS.h"
#include "registry.hpp"
#include "uFASMTest.h"
#include "Solenoid.h"
#include "uFSignalsState.h"
#include "uFunctions.h"
#include "global.h"
#include "SpectroSettings.h"
#include "Classes.hpp"
#include "Math.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "TeeGDIPlus"
#pragma link "CommPort"
#pragma link "uFRHistory"
#pragma resource "*.dfm"

using System::TThreadFunc;

TMainForm *MainForm;
bool LastSynchronizeReturnCode;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	ini = new TIniFile(Globals::IniFileName);

	MainForm->Tag = 0; // запуск произведен 1й раз очистка буфера не требуется
	// spectroscope = new Spectroscope();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender) {
	if (ParamCount() > 0)
		ReadFile(ParamStr(1));
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender) {
	LoadFormPos(this, ini);
	SLD = new SignalListDef(ini);
	FSignalsState = new TFSignalsState(this, ini, SLD);
	if (ini->ReadBool("OtherSettings", "SignalsVisible", false))
		FSignalsState->Show();
	if (ini->ReadBool("OtherSettings", "ProtocolVisible", false))
		TPr::Show();
	DragAcceptFiles(Handle, true); // Разрешаем перетаскивание файлов

	bool RHKret = RegisterHotKey(MainForm->Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00E, // УСЛОВНЫЙ идентификатор горячего ключа
		0, // модификатор
		VK_F1); // код клавиши

	bool RHKret1 = RegisterHotKey(MainForm->Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00A, // УСЛОВНЫЙ идентификатор горячего ключа
		MOD_ALT + MOD_CONTROL, // модификатор
		VK_ADD); // код клавиши

	MainFormInit(Sender);
	KeyPreview = true;

	// Работа со спектроскопом
	/*
	 if (ini->ReadBool("Spectroscope", "IsPresent", false))
	 {
	 cbSpectrotest->Checked = true;
	 if (!spectroscope->Init(ini))
	 {
	 UnicodeString str = UnicodeString("Не удалось открыть порт ") +
	 spectroscope->getName() +
	 " \nдля приема данных со спестроскопа.";
	 Application->MessageBox(str.c_str(), L"Ошибка");
	 cbSpectrotest->Checked = false;
	 ini->WriteBool("Spectroscope", "IsPresent", false);
	 }
	 }
	 */

	Globals::current_typesize = ini->ReadString("Default", "TypeSize", "1");
	IsSendResultToProtocol = ini->ReadBool("OtherSettings",
		"IsSendResultToProtocol", false);
	String str = Globals::current_typesize;

	// Считаем пороги из ini файла
	Globals::tubeSG.ReadThresholds();

	lcard = new LCard502();
	std::vector<String>serial = lcard->getDeviceList();
	if (serial.size() == 0)
		StatusBarBottom->Panels->Items[2]->Text =
			"Плата LCard-502 не обнаружена";
	else if (lcard->Initialization(serial[0])) {
		StatusBarBottom->Panels->Items[2]->Text =
			"Плата lcard-502 инициализирована";
		lcard->setSettingsFromIniFile();
	}
	else
		StatusBarBottom->Panels->Items[2]->Text =
			"Не удалось инициализировать плату LCard-502";

	CrossSolenoid = new Solenoid("PP");
	LinearSolenoid = new Solenoid("PR");

	UpdateComboBox();
	LoadSettings(Sender);
	ClearCharts();
	frHistory->Clear();

	// откроем COM порт для работы
	MyCom = new MyComPort("ASM");

	frConverter = new Inverter();

	CrossFilter = new Filters();
	LinearFilter = new Filters();

	CrossFilter->setSettingsFromIniFile("Pp");
	LinearFilter->setSettingsFromIniFile("Pr");

	// инициализируем передачу данных между модулями по TCP/IP
	sms = new SMS(MainForm);
	sms->StartServer();

	// установи настройки групп прочности
	// SetSolidGroup(Sender); // Не нужно, так как LoadSettings установит сам

	// смотрим, какие модули работают по умолчанию (в последний раз)
	this->cbLinear->Checked = ini->ReadBool("Default", "IsLinear", true);
	this->cbInterruptView->Checked =
		ini->ReadBool("Default", "IsInterruptView", false);

	SystemConst::SetSystemConstants();
	SetProjectSpecialty();

	TThread::CurrentThread->NameThreadForDebugging("Main ");

	stats = new Statistics();
	UpdateStats();

	SLD->oLPCHPOW->Set(true);
	Sleep(1000);

	if (!frConverter->stateRead()) {
		StatusBarBottom->Panels->Items[0]->Text = "Не удалось подключить ПЧ";
		StatusBarBottom->Refresh();
	}
	else {
		StatusBarBottom->Panels->Items[0]->Text = "ПЧ подключен";
		StatusBarBottom->Refresh();
	}
	sg = InitSolid();
	if (NULL == sg) {
		Application->MessageBoxW(L"Библиотека группы прочности не загружена",
			L"Ошибка", MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	DestroySolid(&sg);
	DragAcceptFiles(Handle, false); // Запрещаем перетаскивание файлов

	SLD->oLSOLPOW->Set(false);
	SLD->oCSOLPOW->Set(false);
	SLD->oLPCHPOW->Set(false);

	// запись дефолтного всего
	ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
	ini->WriteBool("Default", "IsLinear", cbLinear->Checked);
	ini->WriteBool("Default", "IsInterruptView", cbInterruptView->Checked);
	// ini->WriteBool("Spectroscope", "IsPresent", cbSpectrotest->Checked);
	delete stats;
	delete CrossSolenoid;
	delete LinearSolenoid;
	// закрытие COM-порта
	delete MyCom;

	UnregisterHotKey(MainForm->Handle, // Handle окна
		0x00E); // наш идентификатор горячего ключа
	UnregisterHotKey(MainForm->Handle, // Handle окна
		0x00A); // наш идентификатор горячего ключа

	Singleton::deleteInstance();
	// delete spectroscope;
	ini->WriteBool("OtherSettings", "SignalsVisible", FSignalsState->Visible);
	delete FSignalsState;
	delete SLD;
	SaveFormPos(this, ini);
	ini->WriteBool("OtherSettings", "ProtocolVisible", TPr::Visible());
	TPr::Dispose();
	delete ini;
}

// ---------------------------------------------------------------------------
void TMainForm::SetProjectSpecialty() {
	cbLinear->Visible = !SystemConst::HideInGain;
	SolidGroup->Visible = SystemConst::isSolidity;
	bGoodTube->Visible = ini->ReadBool("hide", "hide", false);
	pSolidGroup->Visible = SystemConst::isSolidity;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MainFormInit(TObject *Sender) {
	int space = 3;
	int bigspace = 30;

	pStatistics->Left = ClientWidth - pStatistics->Width - space;

	gbCrossDef->Left = space;
	gbCrossDef->Width = ClientWidth - gbCrossDef->Left - space;
	gbLinearDef->Left = gbCrossDef->Left;
	gbLinearDef->Width = gbCrossDef->Width;
	gbThickness->Left = gbCrossDef->Left;
	gbThickness->Width = gbCrossDef->Width;
	gbSummary->Left = gbCrossDef->Left;
	gbSummary->Width = gbCrossDef->Width;

	eCrossDeadZoneFinish->Left = gbCrossDef->ClientWidth -
		eCrossDeadZoneFinish->Width - space;
	Label11->Left = eCrossDeadZoneFinish->Left - Label11->Width - space;
	eCrossDeadZoneStart->Left = Label11->Left - eCrossDeadZoneStart->Width -
		bigspace;
	Label10->Left = eCrossDeadZoneStart->Left - Label10->Width - space;

	eLinearDeadZoneFinish->Left = gbLinearDef->ClientWidth -
		eLinearDeadZoneFinish->Width - space;
	Label13->Left = eLinearDeadZoneFinish->Left - Label13->Width - space;
	eLinearDeadZoneStart->Left = Label13->Left - eLinearDeadZoneStart->Width -
		bigspace;
	Label5->Left = eLinearDeadZoneStart->Left - Label5->Width - space;

	eMin_Good_Length->Left = gbSummary->ClientWidth -
		eMin_Good_Length->Width - space;
	Label8->Left = eMin_Good_Length->Left - Label8->Width - space;
	lCut2->Left = Label8->Left - lCut2->Width - space;
	Label9->Left = lCut2->Left - Label9->Width - space;
	lNN->Left = space;
	pNN->Left = lNN->Left + lNN->Width + space;
	bBadTube->Left = pNN->Left + pNN->Width + space;
	bGoodTube->Left = bBadTube->Left;
	pSolidGroup->Left = bBadTube->Left + bBadTube->Width + space;
	Label7->Left = pSolidGroup->Left + pSolidGroup->Width + space;
	lCut1->Left = Label7->Left + Label7->Width + space;
	lResult->Left = lCut1->Left + lCut1->Width + space;
	lResult->Width = Label9->Left - lResult->Left - space;

	gbCrossDef->Top = pStatistics->Top + pStatistics->Height;
	int hhh = (StatusBarBottom->Top - gbCrossDef->Top - frHistory->Height) / 5;
	gbCrossDef->Height = hhh;

	gbLinearDef->Top = gbCrossDef->Top + gbCrossDef->Height;
	gbLinearDef->Height = hhh;

	gbThickness->Top = gbLinearDef->Top + gbLinearDef->Height;
	gbThickness->Height = hhh * 2;

	gbSummary->Top = gbThickness->Top + gbThickness->Height;
	gbSummary->Height = hhh;

	frHistory->Top = gbSummary->Top + gbSummary->Height;
	frHistory->Left = 0;
	frHistory->Width = ClientWidth;

	CrossDefectChart->Left = space;
	CrossDefectChart->Width = gbCrossDef->ClientWidth -
		CrossDefectChart->Left - space;
	CrossDefectChart->Top = eCrossBorder1->Top + eCrossBorder1->Height + space;
	CrossDefectChart->Height = gbCrossDef->Height -
		CrossDefectChart->Top - space;

	LinearDefectChart->Left = space;
	LinearDefectChart->Width = gbLinearDef->ClientWidth -
		LinearDefectChart->Left - space;
	LinearDefectChart->Top = eLinearBorder1->Top +
		eLinearBorder1->Height + space;
	LinearDefectChart->Height = gbLinearDef->Height -
		LinearDefectChart->Top - space;

	ThicknessChart->Left = space;
	ThicknessChart->Width = gbThickness->ClientWidth -
		ThicknessChart->Left - space;
	ThicknessChart->Top = eThicknessBorder1->Top +
		eThicknessBorder1->Height + space;
	ThicknessChart->Height = gbThickness->Height - ThicknessChart->Top - space;

	SummaryChart->Left = space;
	SummaryChart->Width = gbSummary->ClientWidth - SummaryChart->Left - space;
	SummaryChart->Top = bBadTube->Top + bBadTube->Height + space;
	SummaryChart->Height = gbSummary->Height - SummaryChart->Top - space;

	StatusBarBottom->Panels->Items[0]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[1]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[2]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[3]->Width = ClientWidth / 4;

	StatusBarTop->Panels->Items[0]->Width = ClientWidth / 4;
	StatusBarTop->Panels->Items[1]->Width =
		(ClientWidth - StatusBarTop->Panels->Items[0]->Width) / 2;
	StatusBarTop->Panels->Items[2]->Width =
		StatusBarTop->Panels->Items[1]->Width;

	LinearDefectChart->Title->Visible = false;
	LinearDefectChart->BottomAxis->Visible = true;
	LinearDefectChart->BottomAxis->Automatic = false;
	LinearDefectChart->BottomAxis->Minimum = 0;
	LinearDefectChart->BottomAxis->Maximum = Globals::max_zones;
	LinearDefectChart->LeftAxis->Visible = true;
	LinearDefectChart->LeftAxis->Automatic = false;
	LinearDefectChart->LeftAxis->Minimum = 0;
	LinearDefectChart->LeftAxis->Maximum = Globals::LinSensors;
	LinearDefectChart->Zoom->Allow = false;
	LinearDefectChart->AllowPanning = TPanningMode::pmNone;
	for (int i = 0; i < Globals::LinSensors; i++) {
		LinearDefectChart->Series[i]->ColorEachPoint = true;
		LinearDefectChart->Series[i]->Marks->Visible = false;
		((TBarSeries*) LinearDefectChart->Series[i])->BarWidthPercent = 100;
		((TBarSeries*) LinearDefectChart->Series[i])->MultiBar = mbStacked;
		// TMultiBar(3);
		((TBarSeries*) LinearDefectChart->Series[i])->SideMargins = false;
		((TBarSeries*) LinearDefectChart->Series[i])->OffsetPercent = 50;
	}

	CrossDefectChart->Title = LinearDefectChart->Title;
	CrossDefectChart->BottomAxis = LinearDefectChart->BottomAxis;
	CrossDefectChart->LeftAxis->Visible = true;
	CrossDefectChart->LeftAxis->Automatic = false;
	CrossDefectChart->LeftAxis->Minimum = 0;
	CrossDefectChart->LeftAxis->Maximum = 12;
	CrossDefectChart->Zoom->Allow = false;
	CrossDefectChart->AllowPanning = TPanningMode::pmNone;
	for (int i = 0; i < 12; i++) {
		CrossDefectChart->Series[i]->ColorEachPoint = true;
		CrossDefectChart->Series[i]->Marks->Visible = false;
		((TBarSeries*) CrossDefectChart->Series[i])->BarWidthPercent = 100;
		((TBarSeries*) CrossDefectChart->Series[i])->MultiBar = mbStacked;
		// TMultiBar(3);
		((TBarSeries*) CrossDefectChart->Series[i])->SideMargins = false;
		((TBarSeries*) CrossDefectChart->Series[i])->OffsetPercent = 50;
	}

	ThicknessChart->Title->Visible = false;
	ThicknessChart->LeftAxis->Automatic = false;
	ThicknessChart->LeftAxis->Minimum = 0;
	ThicknessChart->LeftAxis->Maximum = 10;
	ThicknessChart->BottomAxis = LinearDefectChart->BottomAxis;
	ThicknessChart->Zoom->Allow = false;
	ThicknessChart->AllowPanning = TPanningMode::pmNone;
	ThicknessChart->Series[0]->ColorEachPoint = true;
	ThicknessChart->Series[0]->Marks->Visible = false;
	((TBarSeries*) ThicknessChart->Series[0])->BarWidthPercent = 100;
	((TBarSeries*) ThicknessChart->Series[0])->OffsetPercent = 50;
	((TBarSeries*) ThicknessChart->Series[0])->SideMargins = true;

	SummaryChart->Title->Visible = false;
	SummaryChart->BottomAxis = LinearDefectChart->BottomAxis;
	SummaryChart->LeftAxis->Automatic = false;
	SummaryChart->LeftAxis->Minimum = 0;
	SummaryChart->LeftAxis->Maximum = 1;
	SummaryChart->Zoom->Allow = False;
	SummaryChart->AllowPanning = TPanningMode::pmNone;
	SummaryChart->Series[0]->ColorEachPoint = true;
	SummaryChart->Series[0]->Marks->Visible = false;
	((TBarSeries*) SummaryChart->Series[0])->BarWidthPercent = 100;
	((TBarSeries*) SummaryChart->Series[0])->OffsetPercent = 50;
	((TBarSeries*) SummaryChart->Series[0])->SideMargins = true;
}

// ---------------------------------------------------------------------------
void TMainForm::ClearCharts(void) {
	TPr::SendToProtocol("ClearCharts()");
	Singleton::Instance()->CrossResult->DeleteData();
	Singleton::Instance()->LinearResult->DeleteData();
	Singleton::Instance()->ThResult->DeleteData();
	Singleton::Instance()->SumResult->DeleteData();

	for (int i = 0; i < 12; i++)
		CrossDefectChart->Series[i]->Clear();
	for (int i = 0; i < Globals::LinSensors; i++)
		LinearDefectChart->Series[i]->Clear();
	ThicknessChart->Series[0]->Clear();
	SummaryChart->Series[0]->Clear();

	lResult->Caption = "";
	lResult->Color = clBtnFace;
	lCut1->Caption = "";
	lCut2->Caption = "";
	pSolidGroup->Caption = "ГП";
	pSolidGroup->Color = clDkGray;
	pNN->Caption = "";

	MainForm->Refresh();
	MainFormInit(this);
	Globals::isView = false;
}

void __fastcall TMainForm::FormActivate(TObject *Sender) {
	// при активации формы загружаем настройки
	UpdateComboBox();
	LoadSettings(Sender);
	// если были добавлены эталоны по группе прочности - пересчитываем центры
	/* if (SolidGroupForm->Tag == 1)
	 {
	 SolidGroupForm->Tag = 0;
	 SetSolidGroup(Sender);
	 } */
}
// ---------------------------------------------------------------------------

void TMainForm::LoadSettings(TObject *Sender) {
	String sect = "Type_" + Globals::current_typesize;
	// прочитаем используемый диаметр (типоразмер) и прогрузим эталоны по группе прочности
	int temp_diam = Globals::current_diameter;
	Globals::current_diameter = ini->ReadInteger(sect, "Diameter", 1);
	if (temp_diam != Globals::current_diameter)
		SetSolidGroup(Sender);
	// читаем параметры типоразмера - пороги отбраковки, мертвые зоны, минимальный гнодный участок
	double p1, r1, t1;

	Singleton::Instance()->CrossResult->borders[0] =
		ini->ReadFloat(sect, "CrossBorder1", 0.0);
	Singleton::Instance()->LinearResult->borders[0] =
		ini->ReadFloat(sect, "LinearBorder1", 0.0);
	Singleton::Instance()->ThResult->borders[0] =
		ini->ReadFloat(sect, "ThicknessBorder1", 0.0);
	Singleton::Instance()->SumResult->min_good_length =
		ini->ReadInteger(sect, "Min_Good_Length", 0);
	Singleton::Instance()->SumResult->end_cut =
		ini->ReadInteger(sect, "EndCut", 0);

	p1 = ini->ReadFloat(sect, "CrossBorder2", 0.0);
	r1 = ini->ReadFloat(sect, "LinearBorder2", 0.0);
	t1 = ini->ReadFloat(sect, "ThicknessBorder2", 0.0);

	Singleton::Instance()->CrossResult->dead_zone_start =
		ini->ReadInteger(sect, "CrossDeadZoneStart", 0);
	Singleton::Instance()->CrossResult->dead_zone_finish =
		ini->ReadInteger(sect, "CrossDeadZoneFinish", 0);

	Singleton::Instance()->LinearResult->dead_zone_start =
		ini->ReadInteger(sect, "LinearDeadZoneStart", 0);
	Singleton::Instance()->LinearResult->dead_zone_finish =
		ini->ReadInteger(sect, "LinearDeadZoneFinish", 0);

	eCrossBorder1->Text = Singleton::Instance()->CrossResult->borders[0];
	this->eLinearBorder1->Text =
		Singleton::Instance()->LinearResult->borders[0];
	this->eThicknessBorder1->Text = Singleton::Instance()->ThResult->borders[0];
	this->eCrossBorder2->Text = p1;
	this->eLinearBorder2->Text = r1;
	this->eThicknessBorder2->Text = t1;
	this->eMin_Good_Length->Text =
		Singleton::Instance()->SumResult->min_good_length;

	this->eCrossDeadZoneStart->Text =
		IntToStr(Singleton::Instance()->CrossResult->dead_zone_start);
	this->eCrossDeadZoneFinish->Text =
		IntToStr(Singleton::Instance()->CrossResult->dead_zone_finish);
	this->eLinearDeadZoneStart->Text =
		Singleton::Instance()->LinearResult->dead_zone_start;
	this->eLinearDeadZoneFinish->Text =
		Singleton::Instance()->LinearResult->dead_zone_finish;

	if (p1 == 0)
		Singleton::Instance()->CrossResult->borders.set_length(1);
	else {
		Singleton::Instance()->CrossResult->borders.set_length(2);
		Singleton::Instance()->CrossResult->borders[1] = p1;
	}

	if (r1 == 0)
		Singleton::Instance()->LinearResult->borders.set_length(1);
	else {
		Singleton::Instance()->LinearResult->borders.set_length(2);
		Singleton::Instance()->LinearResult->borders[1] = r1;
	}

	if (t1 == 0)
		Singleton::Instance()->ThResult->borders.set_length(1);
	else {
		Singleton::Instance()->ThResult->borders.set_length(2);
		Singleton::Instance()->ThResult->borders[1] = t1;
	}

}
// ---------------------------------------------------------------------------

void TMainForm::UpdateComboBox() {
	cbTypeSize->Items->Clear();
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf
		(Globals::current_typesize);
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW
			(L"Ни одного типоразмера не найдено! Создайте их, нажав клавишу F4",
		L"Предупреждение!", MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::cbTypeSizeSelect(TObject *Sender) {
	Globals::current_typesize = cbTypeSize->Text;
	LoadSettings(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::eSettingsChange(TObject *Sender) {
	ini->WriteString("Type_" + cbTypeSize->Text,
		((TEdit *) Sender)->Name.SubString(2, 29), ((TEdit *) Sender)->Text);
	LoadSettings(Sender);
}

// ---------------------------------------------------------------------------
void TMainForm::SetSolidGroup(TObject *Sender) {
	/*
	 using namespace SolidGroups;
	 int ts = Globals::current_diameter; // текущий типоразмер (диаметр)
	 vector<Tube>etalons; // вектор эталонов из БД
	 if (DM->Connect)
	 {
	 etalons = DM->ReadEtalon(ts);
	 // ---
	 int EtNum = etalons.size();
	 TPr::SendToProtocol("Эталонов ГП загружено: " + IntToStr(EtNum));
	 // читаем эталоны конкретного диаметра из базы
	 if (etalons.empty() || etalons.size() < 3)
	 {
	 StatusBarBottom->Panels->Items[1]->Text =
	 "ЭТАЛОНЫ ГРУППЫ ПРОЧНОСТИ НЕ ОБНАРУЖЕНЫ";
	 StatusBarBottom->Refresh();
	 }
	 else
	 {
	 anal.initEtalons(etalons);
	 StatusBarBottom->Panels->Items[1]->Text =
	 "Эталоны группы прочности загружены";
	 StatusBarBottom->Refresh();

	 // memo---------------------------------------
	 Memo1->Lines->Add
	 ("1-------------------------------------------------------------"
	 );
	 for (int i = 0; i < (int)etalons.size(); i++)
	 {
	 vector<double>buf;
	 buf = etalons[i].Coordinates();
	 UnicodeString tube = (String) etalons[i].solidGroup() + " ";
	 for (int j = 0; j < 8; j++)
	 tube += UnicodeString(buf[j]) + "\t";
	 Memo1->Lines->Add(tube);
	 }

	 vector<Tube>centers;
	 centers = anal.etalonCenters();
	 Memo1->Lines->Add
	 ("2-------------------------------------------------------------"
	 );
	 TPr::SendToProtocol("Загружены эталоны группы прочности");
	 // ---
	 int CentNum = centers.size();
	 TPr::SendToProtocol("Центров группы прочности: " +
	 IntToStr(CentNum));
	 for (int i = 0; i < (int)centers.size(); i++)
	 {
	 vector<double>buf;
	 buf = centers[i].Coordinates();
	 UnicodeString tube = (String) centers[i].solidGroup() + " ";
	 for (int j = 0; j < buf.size(); j++)
	 tube += UnicodeString(Math::SimpleRoundTo(buf[j], 0))
	 + "\t";
	 Memo1->Lines->Add(tube);
	 TPr::SendToProtocol(tube);
	 }
	 // memo ---------------------------------------------------
	 }
	 }
	 */
}

// ---------------------------------------------------------------------------
void TMainForm::IdentifySolidGroup() {
	// *********считаем группу прочности*************************
	/*
	 if (!SystemConst::isSolidGroupMS_DOS) // если считаем по старой схеме
	 {
	 using namespace SolidGroups;
	 Group group;
	 map<Group, double>mp;
	 map<Group, double>::iterator it;
	 AnsiString a="tubeSG.dimension()=";
	 a+=tubeSG.dimension();
	 a+=",cbSpectrotest->Checked=";
	 a+=cbSpectrotest->Checked?"true":"false";
	 a+=",spectroscope->isDataReady()=";
	 a+=spectroscope->isDataReady()?"true":"false";
	 TPr::pr(a);
	 if (tubeSG.dimension() >
	 8 && cbSpectrotest->Checked && spectroscope->isDataReady())
	 // Сработал Спектроскоп
	 {
	 mp = anal.checkTube(tubeSG);

	 Memo1->Lines->Add("******* ГП **********");
	 for (it = mp.begin(); it != mp.end(); ++it)
	 Memo1->Lines->Add(" " + (String) it->first + " " +
	 FloatToStrF(it->second, ffFixed, 4, 4));

	 double maxValue = 1.0e-8;

	 for (it = mp.begin(); it != mp.end(); ++it)
	 {
	 TPr::SendToProtocol("Группа " + (String)it->first +
	 " вероятность " + Math::SimpleRoundTo(it->second,
	 -3)*100.0 + " %");
	 if (it->second > maxValue)
	 {
	 maxValue = it->second;
	 group = it->first;
	 }
	 }
	 pSolidGroup->Color = clYellow;
	 if (maxValue > 0.4)
	 {
	 pSolidGroup->Caption = (String)group;
	 tubeSG.setSolidGroup(group);
	 Globals::tubeSG.setSolidGroup(group);
	 manual_num = group.GroupToNumber(); // костыль
	 }
	 else // НЕ ОПРЕДЕЛЕНО !
	 {

	 pSolidGroup->Caption =
	 ini->ReadString("OtherSettings", "StandartSolidGroup", "K");
	 group = Group((UnicodeString)ini->ReadString("OtherSettings",
	 "StandartSolidGroup", "K"));
	 tubeSG.setSolidGroup(group);

	 Globals::tubeSG.setSolidGroup(group);
	 }
	 }
	 else // Нет химсостава
	 {
	 TPr::SendToProtocol("нет химсостава");

	 pSolidGroup->Caption =
	 ini->ReadString("OtherSettings", "StandartSolidGroup", "K");
	 group = Group((UnicodeString)ini->ReadString("OtherSettings",
	 "StandartSolidGroup", "K"));
	 tubeSG.setSolidGroup(group);
	 Globals::tubeSG.setSolidGroup(group);
	 TPr::SendToProtocol("установили ГП");
	 }
	 pSolidGroup->Refresh();
	 }
	 else // если считаем через коэффициенты корреляции (c помощью проги MS-DOS)
	 {
	 using namespace SolidGroups;
	 String result = tubeSG.calcSolidGroup(tubeSG.Coordinates());
	 result = tubeSG.calcSolidGroup(tubeSG.Coordinates());
	 pSolidGroup->Caption = result;
	 if (result == "K")
	 {
	 pSolidGroup->Color = clGreen;
	 // Globals::tubeSG.setSolidGroup(Group::Types::sgK);
	 tubeSG.setSolidGroup(Group::Types::sgK);
	 }
	 else if (result == "E")
	 {
	 pSolidGroup->Color = clBlue;
	 // Globals::tubeSG.setSolidGroup(Group::Types::sgE);
	 tubeSG.setSolidGroup(Group::Types::sgE);
	 }
	 else
	 {
	 pSolidGroup->Color = clYellow;
	 // Globals::tubeSG.setSolidGroup(Group::Types::sgD);
	 tubeSG.setSolidGroup(Group::Types::sgD);
	 }
	 }
	 */
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bTestClick(TObject *Sender) {
	SLD->oLSCANPOW->Set(true);
	SLD->oCSOLPOW->Set(true);
	Sleep(1000);
	SLD->oLSOLPOW->Set(true);

	ClearCharts();
	Globals::isView = false;

	StatusBarTop->Panels->Items[1]->Text = L"Идет сбор данных с АЦП";
	StatusBarTop->Refresh();
	lcard->setSettingsFromIniFile();
	if (lcard->Start(true)) {
		StatusBarTop->Panels->Items[1]->Text = L"Сбор данных не удался";
		StatusBarTop->Refresh();
		return;
	}

	SetAbleButtons(false); // Блокируем главную форму от кривых рук

	for (DWORD tt = GetTickCount(); GetTickCount() - tt < 5000; Sleep(10))
		Application->ProcessMessages();

	SetAbleButtons(true); // Разблокировка формы

	if (!lcard->Stop()) {
		StatusBarTop->Panels->Items[1]->Text =
			L"Сбор данных завершен с ошибкой";
		StatusBarTop->Refresh();
		return;
	}

	StatusBarTop->Panels->Items[1]->Text = L"Сбор данных с АЦП завершен";
	StatusBarTop->Refresh();
	// ------------------Тестовое заполнение результата и вывод на экран------------
	Singleton::Instance()->CrossResult->zones = 30;
	Singleton::Instance()->CrossResult->FillData(lcard->getCrossMeasure());
	Singleton::Instance()->CrossResult->meas_per_sensor.set_length
		(Singleton::Instance()->CrossResult->zones);

	for (int i = 0; i < Singleton::Instance()->CrossResult->zones; i++)
		Singleton::Instance()->CrossResult->meas_per_sensor[i] =
			Singleton::Instance()->CrossResult->meas_per_zone;

	Singleton::Instance()->CrossResult->ComputeZoneData();
	Singleton::Instance()->CrossResult->CalculateDeadZone
		(ini->ReadInteger("Size", "ZoneLength", 2));

	Singleton::Instance()->LinearResult->zones = 30;
	Singleton::Instance()->LinearResult->FillData(lcard->getLinearMeasure());
	Singleton::Instance()->LinearResult->meas_per_sensor.set_length
		(Singleton::Instance()->LinearResult->zones);

	for (int i = 0; i < Singleton::Instance()->LinearResult->zones; i++)
		Singleton::Instance()->LinearResult->meas_per_sensor[i] =
			Singleton::Instance()->LinearResult->meas_per_zone;

	Singleton::Instance()->LinearResult->ComputeZoneData(true);
	Singleton::Instance()->LinearResult->CalculateDeadZone
		(ini->ReadInteger("Size", "ZoneLength", 2));

	Singleton::Instance()->SumResult->ComputeZonesData();

	Singleton::Instance()->CrossResult->PutResultOnChart(CrossDefectChart,
		MainForm);
	Singleton::Instance()->LinearResult->PutResultOnChart(LinearDefectChart,
		MainForm);
	Singleton::Instance()->ThResult->PutResultOnChart(ThicknessChart, MainForm);
	Singleton::Instance()->SumResult->PutResultOnChart(SummaryChart, MainForm);

	Singleton::Instance()->SumResult->MakeDecision();
	if (Singleton::Instance()->SumResult->decision == "Брак")
		lResult->Color = clRed;
	else
		lResult->Color = clGreen;
	this->lResult->Caption = Singleton::Instance()->SumResult->decision;

	this->lCut1->Caption = Singleton::Instance()->SumResult->cut1;
	this->lCut2->Caption = Singleton::Instance()->SumResult->cut2;

	Globals::isView = true;
	SLD->oCSOLPOW->Set(false);
	SLD->oLSOLPOW->Set(false);
	SLD->oLSCANPOW->Set(false);
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bViewClick(TObject *Sender) {
	if (Globals::isView) {
		ViewForm->Show();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bWorkClick(TObject *Sender) {
	SetAbleButtons(false);
	if (workonline != NULL) {
		workonline = NULL;
		TPr::SendToProtocol(L"Старый тред убит? Таки нет.");
	}
	if (bWork->Caption == "F5 Продолжить") {
		SendResultToASM();
		if (!LastSynchronizeReturnCode) {
			bWork->Caption = "F5 Работа";
			SetAbleButtons(true);
			return;
		}
		SLD->oSHIFT->Set(true);
		if (Singleton::Instance()->SumResult->decision != "Брак") {
			if (cbLinear->Checked)
				SLD->oLSTROBE->Set(true);
			stats->AddTube("Г");
		}
		else
			stats->AddTube("Б");

		DM->SaveTube();
	}

	UpdateStats();
	GoodTube = false;
	BadTube = false;
	TPr::Clear();

	bWork->Caption = "F5 Продолжить";

	// Начало работы
	StatusBarBottom->Refresh();
	 //workonline = new ThreadOnLine(false, cbLinear->Checked,
	 //cbSpectrotest->Checked, spectroscope);

		 workonline = new ThreadOnLine(false, cbLinear->Checked,
	 false, NULL);//spectroscope);

	workonline->OnTerminate = CompleteWork;
	workonline->FreeOnTerminate = true;
	frConverter->ResetErrors();
	SLD->SetAlarm(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bCancelWorkClick(TObject *Sender) {
	SLD->LatchesTerminate();
	if (workonline != NULL) {
		if (!workonline->Finished) {
			workonline->Terminate();
			// workonline->WaitFor();
			TPr::SendToProtocol("User: Сбросили Онлайн работу");
		}
	}
	SLD->SetAlarm(false);
	// if (cbSpectrotest->Checked)
	// {
	// TPr::SendToProtocol("Закрываем сбор данных от спектроскопа...");
	// spectroscope->Stop();
	// }
	StatusBarTop->Panels->Items[1]->Text = "Режим \"Работа\" не завершен!";
	StatusBarTop->Panels->Items[2]->Text = "Прервано пользователем!";
	TPr::SendToProtocol("Прервано пользователем!");

	bWork->Caption = "F5 Работа";
	SetAbleButtons(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::CompleteWork(TObject *Sender) {
	TPr::SendToProtocol("Зашли в Complete Work");
	SLD->LatchesTerminate();
	SLD->SetAlarm(false);
	if (!Globals::tube_transit) {
		if (workonline->cool) {
			TPr::SendToProtocol("cool==true");
			Singleton::Instance()->CrossResult->CalculateDeadZone(200);
			Singleton::Instance()->LinearResult->CalculateDeadZone(200);
			Singleton::Instance()->CrossResult->PutResultOnChart
				(CrossDefectChart, MainForm);
			Singleton::Instance()->LinearResult->PutResultOnChart
				(LinearDefectChart, MainForm);
			CrossDefectChart->Refresh();
			LinearDefectChart->Refresh();
			/*
			 // ждем получения группы прочности
			 if (SystemConst::isSolidity)
			 {
			 TPr::SendToProtocol("Ждем получения группы прочности");

			 using namespace SolidGroups;
			 vector<double>buffer = lcard->getSolidGroupSignal();
			 // Берем вектор
			 tubeSG = Tube(buffer, TubeType(Globals::current_diameter));
			 // Считаем сигнал
			 // Если был запущен спектротест получаем данные
			 if (cbSpectrotest->Checked && spectroscope)
			 {
			 if (spectroscope->isDataReady())
			 {
			 spectroscope->AddSpectroDataToSGTube(&tubeSG);
			 TPr::SendToProtocol
			 ("Получены данные ГП со спектроскопа");
			 // delete spectroscope;
			 // spectroscope=NULL;
			 // ------
			 vector<double>Coords = tubeSG.Coordinates();
			 if (Coords.size() == 0)
			 {
			 TPr::SendToProtocol("Нет координат ГП");
			 }
			 else
			 {
			 for (int i = 0; i < Coords.size(); i++)
			 TPr::SendToProtocol
			 ("Координата ГП" + IntToStr(i + 1) + " : " +
			 FloatToStr(Coords[i]));
			 }
			 // ------
			 }
			 }
			 Globals::tubeSG = tubeSG;

			 StatusBarTop->Refresh();
			 IdentifySolidGroup(); // пытаемся опеределить ГП

			 }
			 //Писать расчёт группы прочности конец
			 */

			// Писать расчёт группы прочности
			if (NULL != sg) {
				vector<double>data = lcard->getSolidGroupSignal();
				wchar_t groupName[128];
				double result;
				unsigned color;

				sg->Compute1(Globals::current_typesize.w_str(),
					(int)lcard->getSettings().frequencyPerChannel, &data[0],
					data.size() / 2, groupName, &result, &color);

				pSolidGroup->Caption = groupName;
				pSolidGroup->Color = clWhite;//color;
				Caption = groupName;
			}
			// Писать расчёт группы прочности конец

			Singleton::Instance()->SumResult->ComputeZonesData();
			Singleton::Instance()->SumResult->PutResultOnChart(SummaryChart,
				MainForm);
			Singleton::Instance()->SumResult->MakeDecision();
			if (Singleton::Instance()->SumResult->decision == "Брак")
				lResult->Color = clRed;
			else
				lResult->Color = clGreen;
			lResult->Caption = Singleton::Instance()->SumResult->decision;

			lCut1->Caption = Singleton::Instance()->SumResult->cut1;
			lCut2->Caption = Singleton::Instance()->SumResult->cut2;

			Globals::isView = true;
			StatusBarTop->Panels->Items[2]->Text = " ";

			// записываем полный результат в базу
			DM->SaveTube(Singleton::Instance()->CrossResult->zone_data,
				Singleton::Instance()->LinearResult->zone_data,
				Singleton::Instance()->SumResult->zones,
				Singleton::Instance()->SumResult->decision.SubString(1, 1),
				// конечный результат по трубе
				// "D", // группа прочности
				pSolidGroup->Caption, Globals::current_diameter, // Типоразмер
				MainForm->cbEtalon->Checked);
			// смотрим, что делать дальше
			if (cbInterruptView->Checked) {
				SetAbleButtons(true);
			}
			else if (cbEtalon->Checked) {
				SetAbleButtons(true, bWork, menuWork);
			}
			else {
				Sleep(2000);
				SetAbleButtons(true);
				TimerThreadComplete->Enabled = true;
				// bWorkClick(Sender);
			}
		}
		else {
			TPr::SendToProtocol("cool==false");
			bWork->Caption = "F5 Работа";
			SetAbleButtons(true);
		}
	}
	else {
		lResult->Color = clGray;
		lResult->Caption = "Транзит";
		StatusBarTop->Panels->Items[2]->Text = " ";
		Sleep(2000);
		SetAbleButtons(true);
		TimerThreadComplete->Enabled = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bManageClick(TObject *Sender) {
	ManageForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bGoodTubeClick(TObject *Sender) {
	TPr::SendToProtocol("Оператор сменил результат на Годно");
	StatusBarBottom->Panels->Items[0]->Text =
		"Оператор сменил результат на Годно";
	StatusBarBottom->Refresh();
	GoodTube = true;
	Singleton::Instance()->SumResult->decision = "Годно";

	bWorkClick(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bBadTubeClick(TObject *Sender) {
	TPr::SendToProtocol("Оператор сменил результат на Брак");
	StatusBarBottom->Panels->Items[0]->Text =
		"Оператор сменил результат на Брак";
	StatusBarBottom->Refresh();
	BadTube = true;
	if (cbLinear->Checked)
		SLD->oLSTROBE->Set(true);
	Singleton::Instance()->SumResult->decision = "Брак";

	bWorkClick(Sender);
	SLD->oCSTROBE->Set(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyPress(TObject *Sender, wchar_t &Key) {
	if (Key == 112)
		WinExec("hh Help.chm", SW_RESTORE);
	if (Key == '`' || Key == L'ё') {
		cbTypeSize->ItemIndex =
			(cbTypeSize->ItemIndex < cbTypeSize->Items->Count - 1) ?
			(cbTypeSize->ItemIndex + 1) : (0);
		cbTypeSizeSelect(Sender);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::ChartClick(TObject *Sender) {
	if (Globals::isView) {
		Singleton::Instance()->CrossResult->CalculateDeadZone(200);
		Singleton::Instance()->LinearResult->CalculateDeadZone(200);
		Singleton::Instance()->CrossResult->PutResultOnChart(CrossDefectChart,
			MainForm);
		Singleton::Instance()->LinearResult->PutResultOnChart(LinearDefectChart,
			MainForm);
		Singleton::Instance()->SumResult->ComputeZonesData();
		Singleton::Instance()->SumResult->MakeDecision();
		if (Singleton::Instance()->SumResult->decision == "Брак")
			lResult->Color = clRed;
		else
			lResult->Color = clGreen;
		this->lResult->Caption = Singleton::Instance()->SumResult->decision;
		this->lCut1->Caption = Singleton::Instance()->SumResult->cut1;
		this->lCut2->Caption = Singleton::Instance()->SumResult->cut2;
		Singleton::Instance()->SumResult->PutResultOnChart(SummaryChart,
			MainForm);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuTypeSizeClick(TObject *Sender) {
	// показать окно с настройками типоразмера
	SettingsForm->Show();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuMeasuresClick(TObject *Sender) {
	// показать окно с настройками измерений
	ADCSettForm->Show();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuQuitClick(TObject *Sender) {
	// выход из программы
	MainForm->Close();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSignalsStateClick(TObject *Sender) {
	FSignalsState->Show();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSaveTubeClick(TObject *Sender) {
	if (SaveToFileDialog->Execute()) {
		// Создаем пустой файл, чтобы записать в него собранные данные
		FILE *file;
		file = fopen(AnsiString(SaveToFileDialog->FileName).c_str(), "w");
		// fprintf(file,"%s \t %s\n","Zones","Sensors");
		fclose(file);

		Singleton::Instance()->CrossResult->SaveTubeToFile
			(SaveToFileDialog->FileName); // Поперечные данные

		Singleton::Instance()->LinearResult->SaveTubeToFile
			(SaveToFileDialog->FileName); // Продольные данные

		Singleton::Instance()->ThResult->SaveTubeToFile
			(SaveToFileDialog->FileName); // Толщинометрия (только zone_data)

			if (NULL != sg) {
				vector<double>data = lcard->getSolidGroupSignal();
				wchar_t groupName[128];
				double result;
				unsigned color;

				sg->Compute1(Globals::current_typesize.w_str(),
					(int)lcard->getSettings().frequencyPerChannel, &data[0],
					data.size() / 2, groupName, &result, &color);

				pSolidGroup->Caption = groupName;
				pSolidGroup->Color = clWhite;//color;
			}
	}
}

// ------------------------------------------------------------------------------
void __fastcall TMainForm::menuLoadTubeClick(TObject *Sender) {
	if (OpenDialogFromFile->Execute()) {
		ClearCharts();
		ReadFromFile((OpenDialogFromFile->FileName).c_str());
	}
}

// ---------------------------------------------------------------------------
void TMainForm::ReadFromFile(UnicodeString path) {
	FILE *file;
	file = fopen(AnsiString(path).c_str(), "r+");

	if (file == NULL)
		Application->MessageBoxW(L"Не удалось открыть файл для чтения",
		L"Ошибка", MB_ICONERROR | MB_OK);
	Singleton::Instance()->CrossResult->LoadTubeFromFile(file);
	Singleton::Instance()->CrossResult->ComputeZoneData();
	Singleton::Instance()->CrossResult->CalculateDeadZone
		(ini->ReadInteger("Size", "ZoneLength", 2));

	Singleton::Instance()->LinearResult->LoadTubeFromFile(file);
	Singleton::Instance()->LinearResult->ComputeZoneData(true);
	Singleton::Instance()->LinearResult->CalculateDeadZone
		(ini->ReadInteger("Size", "ZoneLength", 2));

	Singleton::Instance()->ThResult->LoadTubeFromFile(file);
	fclose(file); // закрыли

	Singleton::Instance()->CrossResult->PutResultOnChart(CrossDefectChart,
		MainForm);
	Singleton::Instance()->LinearResult->PutResultOnChart(LinearDefectChart,
		MainForm);
	Singleton::Instance()->ThResult->PutResultOnChart(ThicknessChart, MainForm);

	Singleton::Instance()->SumResult->ComputeZonesData();
	Singleton::Instance()->SumResult->PutResultOnChart(SummaryChart, MainForm);
	Singleton::Instance()->SumResult->MakeDecision();
	if (Singleton::Instance()->SumResult->decision == "Брак")
		lResult->Color = clRed;
	else
		lResult->Color = clGreen;
	this->lResult->Caption = Singleton::Instance()->SumResult->decision;
	this->lCut1->Caption = Singleton::Instance()->SumResult->cut1;
	this->lCut2->Caption = Singleton::Instance()->SumResult->cut2;
	Globals::isView = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuSolidGroupClick(TObject *Sender) {
	// добавление эталонов по группе прочности
	SolidGroupForm->Send_Tube(tubeSG);
	SolidGroupForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestAdvantechClick(TObject *Sender) {
	// закрывает прогу и вызывает прогу с управлением Выходами платы Advantech
	ShellExecute(0, L"open", L"..\\..\\Settings\\IO_Management.exe", 0, 0,
		SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuF1Click(TObject *Sender) {
	WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuAboutClick(TObject *Sender) {
	// AboutBox->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuColorsClick(TObject *Sender) {
	FormColor->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuViewEtalonsClick(TObject *Sender) {
	FormViewEtalons->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuProtocolClick(TObject *Sender) {
	TPr::Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ReportViewClick(TObject *Sender) {
	FormReport->Show();
}

// ----------------------------------------------------------------------------
void __fastcall TMainForm::menuSGTestClick(TObject *Sender) {
	SGForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestASUConnectionClick(TObject *Sender) {
	MyCom->TestConnection();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuGraphicsSGClick(TObject *Sender) {
	SGGraphForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY) { // сообщение наше
		if (Msg.wParam == 0x00E) // идентификатор наш
				WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
		else if (Msg.wParam == 0x00A) // идентификатор наш
		{
			if (SystemConst::HideInGain) {
				cbLinear->Visible = !cbLinear->Visible;

				bGoodTube->Visible = !bGoodTube->Visible;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// блокировка/разблокировка клавиш
void TMainForm::SetAbleButtons(bool state, TButton *exc1, TMenuItem *exc2,
	TButton *exc3, TButton *exc4) {
	this->cbLinear->Enabled = state;
	this->cbTypeSize->Enabled = state;
	// this->cbSpectrotest->Enabled = state;
	this->bTest->Enabled = state;
	this->bView->Enabled = state;
	this->bWork->Enabled = state;
	this->bCrashForward->Enabled = state;
	this->bReturnMode->Enabled = state;
	this->bManage->Enabled = state;
	this->bGoodTube->Enabled = state;
	this->bBadTube->Enabled = state;

	this->eCrossBorder1->Enabled = state;
	this->eCrossBorder2->Enabled = state;
	this->eLinearBorder1->Enabled = state;
	this->eLinearBorder2->Enabled = state;
	this->eMin_Good_Length->Enabled = state;
	this->eThicknessBorder1->Enabled = state;
	this->eThicknessBorder2->Enabled = state;
	this->eCrossDeadZoneStart->Enabled = state;
	this->eCrossDeadZoneFinish->Enabled = state;
	this->eLinearDeadZoneStart->Enabled = state;
	this->eLinearDeadZoneFinish->Enabled = state;

	this->menuTube->Enabled = state;
	this->menuSettings->Enabled = state;
	this->menuTest->Enabled = state;
	this->menuTestAdvantech->Enabled = state;
	// this->menuTestLcard->Enabled     	 = state;
	// this->menuSGTest->Enabled = state;
	// this->menuWork->Enabled = state;
	this->menuCrashForward->Enabled = state;
	this->menuReturnMode->Enabled = state;
	this->menuManagement->Enabled = state;
	this->menuQuit->Enabled = state;

	if (exc1 != NULL)
		exc1->Enabled = !state;
	if (exc2 != NULL)
		exc2->Enabled = !state;
	if (exc3 != NULL)
		exc3->Enabled = !state;
	if (exc4 != NULL)
		exc4->Enabled = !state;
}

// ----------------------------------------------------------------------------
void __fastcall TMainForm::cbEtalonClick(TObject *Sender) {
	if (cbEtalon->Checked) {
		SetAbleButtons(false, bWork, NULL);
		bView->Enabled = true;
		bReturnMode->Enabled = true;
		cbInterruptView->Checked = true;
		cbInterruptView->Enabled = false;
	}
	else {
		cbInterruptView->Enabled = true;
		SetAbleButtons(true);
		bWork->Enabled = true;
	}
}

// ----------------------------------------------------------------------------
void __fastcall TMainForm::bStatsToNullClick(TObject *Sender) {
	stats->StatsToNull();
	UpdateStats();
}

// ---------------------------------------------------------------------------
void TMainForm::UpdateStats() {
	eBrack->Text = stats->GetBrack();
	eValid->Text = stats->GetSecondClass() + stats->GetValid();
	eBrackInDaStreet->Text = stats->GetBrackInDaStreet();
	eBrack->Refresh();
	eValid->Refresh();
	eBrackInDaStreet->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::eBrackInDaStreetChange(TObject *Sender) {
	ini->WriteInteger("Statistics", "BrackInDaStreet",
		eBrackInDaStreet->Text.ToInt());
	stats->SetBrackInDaStreet(eBrackInDaStreet->Text.ToInt());
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bManualSGClick(TObject *Sender) {
	SLD->oCWORK->Set(true);
	SLD->oSHIFT->Set(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::WmDropFiles(TWMDropFiles& Message) {
	HDROP drop_handle = (HDROP)Message.Drop;
	wchar_t fName[MAXPATH];
	int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
	for (int i = 0; i < filenum; i++) {
		DragQueryFile(drop_handle, i, fName, MAXPATH);
		ReadFile(fName);
	}
	DragFinish(drop_handle);
}

// ---------------------------------------------------------------------------------

// После перетаскивания, вычисляем расширения и открываем файл -----
void __fastcall TMainForm::ReadFile(AnsiString FileName) {
	AnsiString str = ExtractFileExt(FileName);
	if (str == ".sg") {
		SGForm->Show();
		SGForm->LoadFromFile(FileName);
	}
	else if (str == ".dkb")
		ReadFromFile(FileName);
}
// ---------------------------------------------------------------------------------

void __fastcall TMainForm::N1Click(TObject *Sender) {
	// about
	AboutBox1->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::TestRotationClick(TObject *Sender) {
	SLD->oLSCANPOW->Set(true);
	Sleep(1000);
	SLD->oLSOLPOW->Set(true);
	Sleep(1000);
	Application->ProcessMessages();
	Sleep(1000);
	Application->ProcessMessages();
	Sleep(1000);
	Application->ProcessMessages();
	Sleep(1000);
	Application->ProcessMessages();
	bTest->Enabled = false;
	int speed = ini->ReadInteger("Type_" + Globals::current_typesize,
		"WorkSpeed", 35);

	if (!frConverter->setParameterSpeed(Globals::defaultRotParameter, speed)) {
		StatusBarBottom->SimpleText = "Не удалось выставить скорость";
		return;
	}

	if (!frConverter->startRotation()) {
		StatusBarBottom->SimpleText = "Не удалось запустить вращение";
		return;
	}
	if (SLD->iLPCHRUN->Wait(true, 10000)) {
		bTestClick(Sender);
		if (!frConverter->stopRotation()) {
			StatusBarBottom->SimpleText = "Не удалось остановить вращение";
			return;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::N2Click(TObject *Sender) {
	TestRotationClick(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender) {
	DM->SaveTube();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MIASMTestClick(TObject *Sender) {
	TFASMTest* FASMTest = new TFASMTest(this);
	FASMTest->ShowModal();
	delete FASMTest;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerThreadCompleteTimer(TObject *Sender) {
	TTimer* t = (TTimer*)Sender;
	t->Enabled = false;
	Application->ProcessMessages();
	TThread::Sleep(50);
	Application->ProcessMessages();
	TThread::Sleep(50);
	bWorkClick(bWork);
}

// ---------------------------------------------------------------------------
void TMainForm::UpdateStatus(String _text1, String _text2) {
	StatusBarTop->Panels->Items[1]->Text = _text1;
	StatusBarTop->Panels->Items[2]->Text = _text2;
	StatusBarTop->Refresh();

	Application->ProcessMessages();
}

void TMainForm::GetTubeNumber(void) {
	LastSynchronizeReturnCode = false;
	StatusBarBottom->Panels->Items[0]->Text = "";
	StatusBarBottom->Refresh();
	if (SystemConst::ComWithASU) {
		if (MyCom->IsOpened()) {
			int iter = 0;
			while (true) {
				bool transit;
				Globals::tube_number = MyCom->GetTubeNumber(&transit);
				Globals::tube_transit = transit;
				iter++;
				if (Globals::tube_number.Length() != 0) {
					AnsiString a = Globals::tube_number;
					// pNN->Caption = a;
					LastSynchronizeReturnCode = true;
					AnsiString b = "Труба: ";
					b += a;
					if (transit)
						b += "(Т)";
					TPr::pr(b);
					return;
				}
				if (iter <= 3)
					Sleep(2000);
				else {
					iter = 0;
					if (Application->MessageBoxW
						(L"Не получили номер трубы из АСУ, повторить?",
						L"Номер трубы", MB_YESNO) == IDNO)
						return;
				}
			}
		}
	}
	else
		LastSynchronizeReturnCode = true;
}

void TMainForm::NextTube(void) {
	GetTubeNumber();
	if (!LastSynchronizeReturnCode)
		return;
	if (pNN->Caption != Globals::tube_number && pNN->Caption != "5555555555")
		frHistory->Add(pNN->Caption, pSolidGroup->Caption, pSolidGroup->Color,
		lResult->Caption, lResult->Color, lCut1->Caption, lCut2->Caption);
	ClearCharts();
	pNN->Caption = Globals::tube_number;
	LastSynchronizeReturnCode = true;
}

void TMainForm::SendResultToASM(void) {
	StatusBarBottom->Panels->Items[0]->Text = "";
	StatusBarBottom->Refresh();
	LastSynchronizeReturnCode = false;
	if (SystemConst::ComWithASU) {
		if (MyCom->IsOpened()) {
			int solid_num = 0;
			if (pSolidGroup->Caption == "D")
				solid_num = 1;
			else if (pSolidGroup->Caption == "K")
				solid_num = 2;
			else if (pSolidGroup->Caption == "E")
				solid_num = 3;
			else
				solid_num = 1;
			// -----
			int iter = 0;
			while (true) {
				if (MyCom->SendResultToASU(solid_num)) {
					LastSynchronizeReturnCode = true;
					return;
				}
				iter++;
				if (iter <= 3)
					Sleep(2000);
				else {
					iter = 0;
					if (Application->MessageBoxW
						(L"Не смогли отправить результаты в АСУ, повторить?",
						L"Отправка результатов", MB_YESNO) == IDNO)
						return;
				}
			}
		}
	}
	else
		LastSynchronizeReturnCode = true;
	if (IsSendResultToProtocol)
		MyCom->SendToProtocol();
}

// --------------------------------------------------------------------------------
void __fastcall TMainForm::menuTestSpectroscopeClick(TObject *Sender) {
	/*
	 spectroscope->Init(ini);
	 map<string, double>spectroResult;
	 TPr::SendToProtocol("Запускаем сбор данных от спектроскопа...");
	 spectroscope->Start();
	 unsigned long tick = GetTickCount();
	 bool bSpectroAnswer = false;
	 while (GetTickCount() - tick < 10000)
	 {
	 Application->ProcessMessages();
	 if (spectroscope->isDataReady())
	 {
	 bSpectroAnswer = true;
	 TPr::SendToProtocol("Получены данные спектроскопа:");
	 spectroResult = spectroscope->getSpectroData();
	 UnicodeString data = "";
	 for (map<string, double>::iterator i = spectroResult.begin();
	 i != spectroResult.end(); i++)
	 {
	 data += UnicodeString((*i).first.c_str()) + " : " +
	 FloatToStr(RoundTo((*i).second,-3)) + "; ";
	 //FloatToStr((float)(*i).second) + "; ";
	 }
	 TPr::SendToProtocol(data);
	 }
	 }
	 if (!bSpectroAnswer)
	 TPr::SendToProtocol("Нет данных со спектроскопа.");
	 spectroscope->Stop();
	 TPr::SendToProtocol("Спектроскоп закрыт.");
	 */
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuSpectroSettingsClick(TObject *Sender) {
	/*
	 SpectroSettingForm = new TSpectroSettingForm(this);
	 String str;
	 int index;

	 // Считываем данные для спектроскопа
	 // ! Имя порта
	 index = SpectroSettingForm->cbxSerialPort->Items->IndexOf
	 (ini->ReadString("Spectroscope", "SerialPortName", "COM1"));
	 SpectroSettingForm->cbxSerialPort->ItemIndex = index;
	 // Скорость порта
	 index = SpectroSettingForm->cbxBaudrate->Items->IndexOf
	 (String(ini->ReadInteger("Spectroscope", "SerialBaudRate", 9600)));
	 SpectroSettingForm->cbxBaudrate->ItemIndex = index;

	 switch (ini->ReadInteger("Spectroscope", "SerialStopBits", 1))
	 {
	 case 2:
	 str = L"2";
	 break;
	 case 3:
	 str = L"1.5";
	 break;
	 case 1:
	 default:
	 str = L"1";
	 };

	 index = SpectroSettingForm->cbxStopBits->Items->IndexOf(str);
	 SpectroSettingForm->cbxStopBits->ItemIndex = index;

	 switch (ini->ReadInteger("Spectroscope", "SerialParity", 0))
	 {
	 case 1:
	 str = L"Чётный";
	 break;
	 case 2:
	 str = L"Нечётный";
	 break;
	 case 0:
	 default:
	 str = L"Отсутствует";
	 break;
	 };
	 index = SpectroSettingForm->cbxParity->Items->IndexOf(str);
	 SpectroSettingForm->cbxParity->ItemIndex = index;

	 if (SpectroSettingForm->ShowModal() == mrOk)
	 {
	 ini->WriteString("Spectroscope", "SerialPortName",
	 SpectroSettingForm->cbxSerialPort->Text);
	 ini->WriteInteger("Spectroscope", "SerialBaudRate",
	 SpectroSettingForm->cbxBaudrate->Text.ToInt());
	 int serialStopBits;
	 if (SpectroSettingForm->cbxStopBits->Text == "2")
	 serialStopBits = 2;
	 else if (SpectroSettingForm->cbxStopBits->Text == "1.5")
	 serialStopBits = 1;
	 else
	 serialStopBits = 0;
	 ini->WriteInteger("Spectroscope", "SerialStopBits", serialStopBits);
	 int serialParity;
	 if (SpectroSettingForm->cbxParity->Text == "Отсутствует")
	 serialParity = 0;
	 else if (SpectroSettingForm->cbxParity->Text == "Чётный")
	 serialParity = 1;
	 else if (SpectroSettingForm->cbxParity->Text == "Нечётный")
	 serialParity = 2;
	 ini->WriteInteger("Spectroscope", "SerialParity", serialParity);
	 if (!spectroscope->Init(ini))
	 {
	 UnicodeString str = UnicodeString("Не удалось открыть порт ") +
	 spectroscope->getName() +
	 " \nдля приема данных со спестроскопа.";
	 Application->MessageBox(str.c_str(), L"Ошибка");
	 cbSpectrotest->Checked = false;
	 ini->WriteBool("Spectroscope", "IsPresent", false);
	 }
	 }
	 delete SpectroSettingForm;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::cbSpectrotestClick(TObject *Sender) {
	// ini->WriteBool("Spectroscope", "IsPresent", cbSpectrotest->Enabled);
	// if (cbSpectrotest->Enabled)
	// spectroscope->Init(ini);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SolidGroupClick(TObject *Sender) {
	if (NULL != sg) {
		vector<double>data = lcard->getSolidGroupSignal();
		wchar_t groupName[128];
		double result;
		unsigned color;
		int size = data.size();
			sg->Compute1(Globals::current_typesize.w_str(),
				(int)lcard->getSettings().frequencyPerChannel, &data[0],
				data.size() / 2, groupName, &result, &color);
		sg->OptionsWindow();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::pSolidGroupClick(TObject *Sender)
{
    	if (NULL != sg) {
				vector<double>data = lcard->getSolidGroupSignal();
				wchar_t groupName[128];
				double result;
				unsigned color;

				sg->Compute1(Globals::current_typesize.w_str(),
					(int)lcard->getSettings().frequencyPerChannel, &data[0],
					data.size() / 2, groupName, &result, &color);

				pSolidGroup->Caption = groupName;
				Caption = groupName;
				pSolidGroup->Color = clWhite;//color;
			}
}
//---------------------------------------------------------------------------

