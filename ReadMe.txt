///FILE Main.cpp
/////////////////////////////////////////////////////////////////////
 ISG_DLL *sg;


void __fastcall TMainForm::FormCreate(TObject *Sender)
{
.....................
sg = InitSolid();				//c 167
	if(NULL == sg)
	{
		Application->MessageBoxW(L"Библиотека группы прочности не загружена",L"Ошибка",MB_OK+MB_ICONERROR);
	}
.............................................
}

void __fastcall TMainForm::CompleteWork(TObject *Sender)
{
......................
//Писать расчёт группы прочности
			if(NULL != sg)
			{
                  vector<double> data = lcard->getSolidGroupSignal();
				  wchar_t groupName[128];
				  double result;
				  unsigned color;
				  
				  sg->Compute(
					  Globals::current_typesize.w_str()
					  , (int)lcard->getSettings().frequencyPerChannel
					  , &data[0]
					  , data.size() / 2
					  , groupName
					  , &result
					  , &color
				  );

				  pSolidGroup->Caption = groupName;
				  pSolidGroup->Color = color;
			}
			//Писать расчёт группы прочности конец
...........................
}

void __fastcall TMainForm::SolidGroupClick(TObject *Sender)
{
  if(NULL != sg)
  {
	  sg->OptionsWindow();
  }
}

void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	DestroySolid(&sg);  //176