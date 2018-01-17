	//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SolidGroupTest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "BubbleCh"
#pragma link "TeeFunci"
#pragma resource "*.dfm"
TSGForm *SGForm;
//---------------------------------------------------------------------------
__fastcall TSGForm::TSGForm(TComponent* Owner)
	: TForm(Owner)
{
	for (int i = 0; i < 8; i++)
		thresholds[i] = new	TLineSeries(SignalChart);
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::FormDestroy(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
		delete thresholds[i];
	DragAcceptFiles(Handle, false); // Запрещаем перетаскивание файлов
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::FormCreate(TObject *Sender)
{

	for (int i = 0; i < quantity_thresholds; i++)
	{
		int left = 2;
		int width = pThresholds->Width-left*2;
		int top = 10;
		int height = 21;
		eThresholds[i] = new TEdit(pThresholds);
		eThresholds[i] -> Parent = pThresholds;
		eThresholds[i] -> SetBounds(left,height*i+top,width,height);
		eThresholds[i] -> Name = L"eThresholds" + IntToStr(i);
		eThresholds[i] -> Clear();
		eThresholds[i] -> OnKeyDown = EditKeyDown;
		eThresholds[i] -> Refresh();
		eThresholds[i] -> Tag = i;

	}
	tube_sg = SolidGroups::Tube();
	DragAcceptFiles(Handle, true); // Разрешаем перетаскивание файлов
}
//------------------------------------------------------------------------------
void __fastcall TSGForm::EditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Key == 40)
	{
		int tag = ( (TEdit*) Sender )->Tag;
		if(tag == 7)
			tag = 0;
		else
			tag++;
		eThresholds[tag]->SetFocus();
	}
	else
	if( Key == 38)
	{
		int tag = ( (TEdit*) Sender )->Tag;
		if(tag == 0)
			tag = 7;
		else
			tag--;
		eThresholds[tag]->SetFocus();
	}

	if( Key == 0xD )
	{
		//тут пересчет результата после изменения порога
		vector<double> local_th;
		local_th.resize(quantity_thresholds);

		for(int i=0; i < quantity_thresholds; i++)
		{
			if( eThresholds[i]->Text == "")
				eThresholds[i]->Text = "0";
			local_th[i] = eThresholds[i]->Text.ToDouble();
		}
		tube_sg.setThresholds(local_th);
		DrawOnChart();
	}
	else
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',') || ('-') )) Key = 0x00;

}
//------------------------------------------------------------------------------
void __fastcall TSGForm::FormShow(TObject *Sender)
{
	tube_sg.ReadThresholds();                                    //считываем пороги по текущему типоразмеру из ini-файла

	vector<double> local_th = tube_sg.getThresholds();
	for (int i = 0; i < quantity_thresholds; i++)
		eThresholds[i] -> Text = String(local_th[i]);
	buffer = local_th;	//создадим копию перед выводом в эдиты, для последующего сравнения

	StatusBar->Panels->Items[0]->Text = "";
	StatusBar->Panels->Items[1]->Text = "";
	SignalChart->BottomAxis->SetMinMax(0,1000);
	SignalChart->UndoZoom();
	Series->Clear();
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();
	for (int i = 0; i < 8; i++)
		thresholds[i]->Clear();
	scbChartDragger->Position = 1;
	curPos = 1;
	SignalChart->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::bTestClick(TObject *Sender)
{
	bOpenTube->Enabled=false;
	bSaveTube->Enabled=false;
	eTestTime->Enabled=false;
	bTest->Enabled=false;
	Series->Clear();
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();
	for (int i = 0; i < 8; i++)
		thresholds[i]->Clear();
	StatusBar->Panels->Items[0]->Text = "";
	StatusBar->Refresh();
	lcard->setSettingsFromIniFile();

	if(eTestTime->Text == "" )
		eTestTime->Text = 5;
	if(StrToInt(eTestTime->Text) > 30)
	   eTestTime->Text = "30";
	if(StrToInt(eTestTime->Text) < 0)
	   eTestTime->Text = "5";

	int Time=StrToInt(eTestTime->Text);
	StatusBar->Panels->Items[0]->Text="Начинаем сбор данных с АЦП длительностью " + UnicodeString(Time)+" сек";
	lcard->Start();

	for (DWORD tt = GetTickCount(); GetTickCount() - tt < Time*1000; Sleep(10))
		Application->ProcessMessages();

	lcard->Stop();
	StatusBar->Panels->Items[0]->Text="Сбор данных завершен успешно";

	Signal = lcard->getSolidGroupSignal();
	TPr::SendToProtocol("Размер сигнала по ГП "+String(Signal.size()));
	DrawOnChart();

	using namespace SolidGroups;
	MainForm->tubeSG = tube_sg;
	MainForm->IdentifySolidGroup();

	bOpenTube->Enabled=true;
	bSaveTube->Enabled=true;
	eTestTime->Enabled=true;
	bTest->Enabled=true;
}
//---------------------------------------------------------------------------
void TSGForm::DrawOnChart()
{
	 //очистим серии
	Series->Clear();
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();
	for (int i=0; i < 8; i++)
		thresholds[i]->Clear();

	// рисуем графики токового и магнитного сигнала
	double xv = 0;
	for(UINT i = 0; i < Signal.size(); i += 2)
	{
		Series->AddXY(xv,Signal[i],"",clBlue);
		Series1->AddXY(xv,Signal[i+1],"",clRed);
		xv += 1;
	}

	SignalChart->BottomAxis->Automatic = true;
	SignalChart->BottomAxis->SetMinMax(0, 1000);
	SignalChart->LeftAxis->Automatic = true;
	SignalChart->Refresh();
	if (Signal.size())
	{
		tube_sg.setTubeType(SolidGroups::TubeType(Globals::current_diameter));
		tube_sg.setCoordinates(tube_sg.coordsFromSignal(Signal));
	}
	// выводим 8 параметров в строку
	if (Signal.size() > 0)
	{
		vector <double> pars = tube_sg.Coordinates();
		String str = "";
		for (unsigned i = 0; i < pars.size(); i++)
			str += FloatToStrF(pars[i],ffFixed,3,3) + "   ";
		StatusBar->Panels->Items[1]->Text = str;
		StatusBar->Refresh();
	}

	vector<double> local_th = tube_sg.getThresholds();
	unsigned int th = 0;
	for( unsigned int pair = 0; pair < ( Signal.size() / 2 - 1); ++pair )
	{
		double    magnetic = Signal[ 2*pair + 0],
				  current  = Signal[ 2*pair + 1],
				  next_cur = Signal[ 2*(pair + 1) + 1];

		if( current >= local_th[th] && next_cur < local_th[th] && magnetic > 0)
		{		// здесь все правильно!! работает! не изменять - руки оторву!
				// в условии можно менять только ЗНАК у magnetiс - если надо считывать нижнюю полуплоскоть сигнала
			Series2->AddXY(pair,magnetic,"",clRed);
			Series3->AddXY(pair,current,"",clBlue);
			th++;
		}
		if (th == 8)
			th = 0;
	}

 //	DrawThresholds();
}
//---------------------------------------------------------------------------
void TSGForm::LoadFromFile(UnicodeString FileName)
{
	scbChartDragger->Position = 1;
	Series->Clear();
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();

	FILE *file;
	file = fopen(AnsiString(FileName).c_str(), "r");
	if( file == NULL )
	{
		Application->MessageBoxW( L"Не удалось открыть файл для чтения",L"Ошибка", MB_ICONERROR | MB_OK  );
		return;
	}
	int size;
	fscanf(file,"%d \n",&size);
	if( size != 0 )
		{
			Signal.resize(size);
			for(int i = 0; i < size; i += 2 )
			{
				fscanf(file,"%lf  ",&Signal[i]);
				fscanf(file,"%lf \n",&Signal[i+1]);
			}
			DrawOnChart();
			int pos = FileName.LastDelimiter("\\");
			FileName = FileName.SubString( pos + 1, FileName.Length() - pos);
			StatusBar->Panels->Items[0]->Text="Успешно открыт файл: " + FileName;
		}
	else
		StatusBar->Panels->Items[0]->Text="Данный файл пуст";
	fclose(file);
	Globals::tubeSG = tube_sg;
	MainForm->tubeSG = tube_sg;
	MainForm->IdentifySolidGroup();
}
//---------------------------------------------------------------------------
void TSGForm::SaveToFile(UnicodeString FileName)
{
	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"Не удалось открыть файл для записи",L"Ошибка", MB_ICONERROR | MB_OK  );
	else
	{
		//Запишем длину массива
		fprintf(file,"%d \n",Signal.size() );
		//запишем сами данные
		for( unsigned i = 0; i < Signal.size(); i += 2 )
		{
			fprintf(file,"%lf  ",Signal[i]);
			fprintf(file,"%lf \n",Signal[i+1]);
		}
		StatusBar->Panels->Items[0]->Text="Файл успешно сохранен";
	}
	fclose(file);
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::bOpenTubeClick(TObject *Sender)
{
	if(OpenDialog->Execute( ) )
		LoadFromFile(OpenDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::bSaveTubeClick(TObject *Sender)
{
	if(	SaveDialog->Execute() )
		SaveToFile(SaveDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::bShowCurrentTubeClick(TObject *Sender)
{
	tube_sg = Globals::tubeSG;                   //Точно не известно будет ли это работать без перегруженного оператора =
	Signal = lcard->getSolidGroupSignal();
	if(Signal.size()==0)
		return;
	scbChartDragger->Position = 1;
	curPos = 1;
	DrawOnChart();
}
//---------------------------------------------------------------------------
void TSGForm::DrawThresholds()
{
	vector<double> local_th = tube_sg.getThresholds();
	for (int i = 0; i < local_th.size(); i++)
	{
		thresholds[i]->AddXY(0,local_th[i] , "", clBlack);
		thresholds[i]->AddXY(SignalChart->Series[0]->MaxXValue(),local_th[i] , "", clBlack);
		SignalChart->AddSeries(thresholds[i]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 27)
		SGForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::N1Click(TObject *Sender)
{
	BottomPanel->Visible = !BottomPanel->Visible;

}
//---------------------------------------------------------------------------
void __fastcall TSGForm::tbZoomChange(TObject *Sender)
{
	long max = SignalChart->BottomAxis->Maximum;
	long min = SignalChart->BottomAxis->Minimum;
	long center = max - min;
	SignalChart->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::scbChartDraggerChange(TObject *Sender)
{
	long length = Series->MaxXValue();
	long max = SignalChart->BottomAxis->Maximum;
	long min = SignalChart->BottomAxis->Minimum;
	double step = (double) (max-min) / 100.0;
	if (length > 0)
		scbChartDragger->Max = ceil((double)length * 100.0 / (double) (max-min));
	else
		scbChartDragger->Max = ceil((double)100.0 * 100.0 / (double) (max-min));
//	curPos = max/step;
	long change = scbChartDragger->Position - curPos;
	if (change > 0)
	{
		SignalChart->BottomAxis->Maximum = change * step + max;
		SignalChart->BottomAxis->Minimum = change * step + min;
	}
	else
	{
		SignalChart->BottomAxis->Minimum = change * step + min;
		SignalChart->BottomAxis->Maximum = change * step + max;
	}

	SignalChart->Refresh();
	curPos = scbChartDragger->Position;
	SignalChartScroll(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::N2Click(TObject *Sender)
{
	pThresholds->Visible = !pThresholds->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
//получим те значения что забиты в эдиты и те что были раньше когда открыли форму
	vector<double> local_th = tube_sg.getThresholds();
	bool result = false;
	for(int i = 0; i < quantity_thresholds; i++)
	{
		if(local_th[i] != buffer[i])
			{	result = true;	break;	}
	}
	if (result)
	{
		int ID = Application->MessageBoxW(L"Внимание, пороги группы прочности были изменены!\n Сохранить изменения?",
				L"Внимание!",MB_YESNOCANCEL | MB_ICONWARNING);
		if( ID == IDYES )
		{
			Globals::tubeSG.setThresholds(local_th);
			Globals::tubeSG.WriteThresholds();
		}
		else
		if(ID == IDNO)
		{
			//оставим старые значения
			tube_sg.setThresholds(buffer);
			tube_sg.WriteThresholds();
			Close();
		}
		else
		if(ID == IDCANCEL)
			CanClose = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::WmDropFiles(TWMDropFiles& Message)
{
   HDROP drop_handle = (HDROP)Message.Drop;
   wchar_t fName[MAXPATH];
   int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
   for(int i = 0; i < filenum; i++){
	  DragQueryFile(drop_handle, i, fName, MAXPATH);
	  ReadFile(fName);
	 }
   DragFinish(drop_handle);
}
//---------------------------------------------------------------------------------
// После перетаскивания, вычисляем расширения и открываем файл -----
void __fastcall TSGForm::ReadFile(AnsiString FileName)
{
   AnsiString str = ExtractFileExt(FileName);
   if(str == ".sg")
		SGForm->LoadFromFile(FileName);
}
//---------------------------------------------------------------------------------

void __fastcall TSGForm::AddParametersClick(TObject *Sender)
{
// добавление эталонов по группе прочности
	SolidGroupForm->Send_Tube(tube_sg);
	SolidGroupForm->Show();
}

//---------------------------------------------------------------------------
void __fastcall TSGForm::SignalChartClick(TObject *Sender)
{
		SignalChart->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::SignalChartScroll(TObject *Sender)
{
	lMinScrollbar->Caption = "" + IntToStr((int) SignalChart->BottomAxis->Minimum );
	lMaxScrollbar->Caption = "" + IntToStr((int) SignalChart->BottomAxis->Maximum );
}
//---------------------------------------------------------------------------

