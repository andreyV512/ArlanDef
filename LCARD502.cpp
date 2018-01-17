//---------------------------------------------------------------------------


#pragma hdrstop

#include "LCARD502.h"
#include <iterator>
#include "Global.h"

LCard502 *lcard;
//---------------------------------------------------------------------------
LCard502::LCard502()
{
	//Заполним дефолтные настройки
	Parameters.RECV_TOUT = 250;
	Parameters.syncMode = L502_SYNC_INTERNAL;                        //см. l502api.h
	Parameters.syncStartMode = L502_SYNC_INTERNAL;
	Parameters.frequencyPerChannel	= 1000;
	int sensorCount = Globals::LinSensors + Globals::Cross_sensors;
	for(int i=0; i<sensorCount;i++)
	{
		Channel_parameters ch;
		ch.range = L502_ADC_RANGE_10;
		ch.logicalChannel = i;
		ch.collectedMode = L502_LCH_MODE_DIFF;
		Parameters.channels.push_back(ch);
	}
	Parameters.others.resize(7);
	for(int i=0; i< Parameters.others.size(); i++)
	{
		Channel_parameters ch;
		ch.range = 0;
		ch.collectedMode = 0;
		Parameters.others[i] = ch;
	}
	//Номера канала разделения по зонам
	Parameters.others[6].logicalChannel = 21;
	Parameters.frequencyCollect = Parameters.getCountChannels() *	Parameters.frequencyPerChannel;
	voltToPercent[0] = 10;
	voltToPercent[1] = 20;
	voltToPercent[2] = 50;
	voltToPercent[3] = 100;
	voltToPercent[4] = 200;
	voltToPercent[5] = 500;
	isTest = false;
	handle = NULL;
	thread = NULL;
	threadRunning = false;
}
//---------------------------------------------------------------------------
LCard502::~LCard502()
{

}
//---------------------------------------------------------------------------
bool LCard502::Initialization(String serial_number)
{
	if (handle==NULL)
	{
		handle = L502_Create();
		if (handle==NULL)
		{
			MessageDlg("Ошибка создания описателя модуля", mtError, TMsgDlgButtons() << mbOK,NULL);
			return false;
		}
		else
		{
			 AnsiString serial = AnsiString(serial_number);
			 int32_t err = L502_Open(handle, serial.c_str());
			 if (err)
			 {
				MessageDlg("Ошибка открытия модуля: " + String(L502_GetErrorString(err)),
				mtError, TMsgDlgButtons() << mbOK,NULL);
				L502_Free(handle);
				handle = NULL;
				return false;
			 }
			 return true;
		}

	}
	else
		return false;
}
//------------------------------------------------------------------------------
vector< String > LCard502::getDeviceList()
{
	uint32_t dev_cnt = 0;
	int32_t res;

	vector<String> DeviceList;

	/* сперва получаем общее количество устройств */
	res  = L502_GetSerialList(NULL, 0, 0, &dev_cnt);
	if ((res>=0) && dev_cnt)
	{
		/* выделяем памяти под серийные номера всех найденных устройств */
		t_l502_serial_list list = (t_l502_serial_list) malloc(dev_cnt*L502_SERIAL_SIZE);
		res = L502_GetSerialList(list, dev_cnt, 0, NULL);
		if (res>0)
		{
			for (int32_t i=0; i < res; i++)
				DeviceList.push_back(String(list[i]));
		}
		/* освобождаем выделенную память */
		free(list);
	}
	return DeviceList;
}
//------------------------------------------------------------------------------
__int32 LCard502::setSettingsFromIniFile()
{
	TIniFile* ini = new TIniFile(Globals::IniFileName);
	Parameters.channels.clear();
	for(int i=0; i<Globals::Cross_sensors;i++)
	{
		Channel_parameters chPar;
		chPar.range  		 = ini->ReadInteger("Range_PP_"+String(Globals::current_diameter),"Range"+IntToStr(i),0);
		chPar.logicalChannel = ini->ReadInteger("PP","Number"+UnicodeString(i),i);
		chPar.collectedMode  = ini->ReadInteger("CollectedMode_PP_"+String(Globals::current_diameter),"Mode"+IntToStr(i),1);
		Parameters.channels.push_back(chPar);
	}
	for(int i=0; i<Globals::LinSensors; i++)
	{
		Channel_parameters chPar;
		chPar.range = (ini->ReadInteger("Range_PR_"+String(Globals::current_diameter),"Range"+IntToStr(i),1));
		chPar.logicalChannel = ini->ReadInteger("PR","Number"+UnicodeString(i),i);
		chPar.collectedMode = ini->ReadInteger("CollectedMode_PR_"+String(Globals::current_diameter),"Mode"+IntToStr(i),1);
		Parameters.channels.push_back(chPar);
	}

	Parameters.others[0].logicalChannel = ini->ReadInteger("PP", "amperageChannel", 3);
	Parameters.others[1].logicalChannel = ini->ReadInteger("PP", "voltageChannel",  4);

	Parameters.others[2].logicalChannel = ini->ReadInteger("PR", "amperageChannel", 5);
	Parameters.others[3].logicalChannel = ini->ReadInteger("PR", "voltageChannel",  6);

	Parameters.others[4].logicalChannel = ini->ReadInteger("OtherSettings", "SensorSG",  16);
	Parameters.others[5].logicalChannel = ini->ReadInteger("OtherSettings", "SensorCurr",17);

	Parameters.others[4].collectedMode = 0;
	Parameters.others[5].collectedMode = 0;

	Parameters.others[4].range = 1;		//плюс/минус 5В
	Parameters.others[5].range = 1;     //плюс/минус 5В

	Parameters.syncMode =ini->ReadInteger("LCard", "SyncMode",0);
	Parameters.syncStartMode = ini->ReadInteger("LCard", "SyncStartMode",0);
	Parameters.frequencyCollect = ini->ReadFloat("LCard", "Friquency",1000.0);
	Parameters.frequencyPerChannel = ini->ReadFloat("LCard", "Rate",1000.0);
	delete ini;

	return setSettings();
}
//------------------------------------------------------------------------------
__int32 LCard502::setSettings(LCard_parameters &parameters)
{
	Parameters = parameters;
	int32_t err = 0;
	//Сколько всего каналов будем собирать
	err = L502_SetLChannelCount(handle, uint32_t(Parameters.getCountChannels()));
	//Индивидуальные настройки для каждого канала
	if(!err)
	{
		int i;
		for(i=0; i < Parameters.channels.size(); i++)
		{
			err = L502_SetLChannel(handle,i,
								   Parameters.channels[i].logicalChannel,
								   Parameters.channels[i].collectedMode,
								   Parameters.channels[i].range,0);
		}
		for(int j=0; j<Parameters.others.size(); j++)
		{
			err = L502_SetLChannel(handle,i,
								   Parameters.others[j].logicalChannel,
								   Parameters.others[j].collectedMode,
								   Parameters.others[j].range,0);
			i++;
		}
	}

	//Настраиваем источник частоты синхронизации
	if (!err)
		err = L502_SetSyncMode(handle, Parameters.syncMode);
	//Настраиваем  источник запуска сбора
	if (!err)
		err = L502_SetSyncStartMode(handle, Parameters.syncStartMode);
	double f_acq;
	double f_lch;
	//настраиваем частоту сбора с АЦП
	if (!err)
	{
		f_acq = Parameters.frequencyPerChannel*Parameters.getCountChannels();
		f_lch = Parameters.frequencyPerChannel;
		err = L502_SetAdcFreq(handle, &f_acq, &f_lch);
	}
	if(!err)
	{
		Parameters.frequencyCollect = f_acq;
		Parameters.frequencyPerChannel = f_lch;
	}
	// Записываем настройки в модуль
	if (!err)
		err = L502_Configure(handle, 0);

	if(err)
		Application->MessageBoxW(L"Не удалось задать параметры L-502",L"Ошибка",MB_OK);
	if(!err)
	{
		//Выделим память под данные
		ULONG buffer_size =
			  Parameters.frequencyPerChannel *
			  Parameters.channels.size()*
			  35; //Примерное время прохождения 10 метровой трубы через установку
		try
		{
			SourceData.resize(buffer_size);
		}catch(...)
		{	Application->MessageBoxW(L"Не удалось выделить память для буфера L-502",L"Ошибка",MB_OK);}
	}

	return err;
}
int32_t LCard502::setSettings()
{
	return setSettings(Parameters);
}
//---------------------------------------------------------------------------
void LCard502::LinearIsStarted()
{
	CountMeasurementsStartLinear = CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
void LCard502::CrossIsStarted()
{
	CountMeasurementsStartCross = CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
void LCard502::CrossIsStopped()
{
	CountMeasurementsStoppedCross = CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
void LCard502::LinearIsStopped()
{
	CountMeasurementsStoppedLinear = CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
void LCard502::SolidGroupIsStarted()
{
	CountMeasurementsStartSolidGroup = CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
void LCard502::insertData(const vector<double> &data, __int32 size)
{
	if( (CountCollectedMeasurements + size) > SourceData.size() )
		SourceData.resize(SourceData.size()+size);
	std::copy(data.begin(),data.begin()+size,SourceData.begin()+CountCollectedMeasurements);
	CountCollectedMeasurements+=size;
}
//---------------------------------------------------------------------------
__int64 LCard502::getCountCollectedMeasurements()
{
	return CountCollectedMeasurements;
}
//---------------------------------------------------------------------------
__int32 LCard502::getMaxBufferSize()
{
	return SourceData.size();
}
//---------------------------------------------------------------------------
vector < vector < double > > LCard502::getLinearMeasure()
{
//	int CrossSensorsCount=(Globals::current_diameter == 60) ? (10) : (12);
	int CrossSensorsCount = 12;
	return getMeasure(CrossSensorsCount,
					  CrossSensorsCount + Globals::LinSensors,
					  CountMeasurementsStartLinear,
					  CountMeasurementsStoppedLinear);
}
//---------------------------------------------------------------------------
vector < vector < double > > LCard502::getCrossMeasure()
{
	int CrossSensorsCount=(Globals::current_diameter == 60) ? (10) : (12);

	return getMeasure(0,
					  CrossSensorsCount,
					  CountMeasurementsStartCross,
					  CountMeasurementsStoppedCross);
}
//---------------------------------------------------------------------------
vector < vector < double> >  LCard502::getLinearMeasure( __int64 oldPosition )
{
	//int CrossSensorsCount=(Globals::current_diameter == 60) ? (10) : (12);
	int CrossSensorsCount = 12;
	return getMeasure(CrossSensorsCount,
					  CrossSensorsCount + Globals::LinSensors,
					  oldPosition,
					  CountCollectedMeasurements);
}
//---------------------------------------------------------------------------
vector < vector < double > > LCard502::getCrossMeasure ( __int64 oldPosition )
{
	int CrossSensorsCount=(Globals::current_diameter == 60) ? (10) : (12);

	return getMeasure(0,
					  CrossSensorsCount,
					  oldPosition,
					  CountCollectedMeasurements);
}
//---------------------------------------------------------------------------
vector< vector<double> > LCard502::getMeasure(int firstSensorPosition,
											  int lastSensorPosition,
											  __int64 StartPosition,
											  __int64 StopPosition)
{
	if(isTest)
		StopPosition = CountCollectedMeasurements;

	vector< vector<double> > tempVector;
	tempVector.resize(lastSensorPosition - firstSensorPosition);
	int k=0;
	for( int i=firstSensorPosition; i < lastSensorPosition; i++)
	{
/*
			AnsiString a;
			a="SSize=";
			a+=SourceData.size();
			a+=" VSize=";
			a+=tempVector.size();
			a+=" k,i";
			a+=k;
			a+=" ";
			a+=i;
			a+=" ";
			TPr::pr(a);
*/
		for(int j=StartPosition; j < StopPosition; j+=Parameters.getCountChannels())
		{
			int range = Parameters.channels[i].range;
			tempVector[k].push_back(SourceData[i+j] * voltToPercent[range]);
		}
		k++;
	}
	return tempVector;
}
//---------------------------------------------------------------------------
vector<double> LCard502::getSolidGroupSignal()
{
	std::vector<double> temp;
	__int64 size = getCountCollectedMeasurements();
	int defSensorCount = Parameters.channels.size();
	for(__int64 i = CountMeasurementsStartSolidGroup; i < size;  i+=Parameters.getCountChannels())
	{
		temp.push_back(( SourceData[i+defSensorCount+4] * voltToPercent[Parameters.others[4].range]*10 ));
		temp.push_back(( SourceData[i+defSensorCount+5] * voltToPercent[Parameters.others[5].range]*10 ));
	}
	return temp;
}
//---------------------------------------------------------------------------
vector<double> LCard502::getFrameValue()
{
	std::vector<double> temp, avg;
	temp.resize(Parameters.getCountChannels());
	avg.resize(Parameters.getCountChannels());

	int32_t err = setSettings(Parameters);
	if(!err)
	{

		if (threadRunning)
		{
			for (int i = 0; i < temp.size(); i++)
			{
				avg[i]=-100500;
			}
			return avg;
		}
		else
		{
			for(int i = 0; i < 5; i++)
			{
				err = L502_AsyncGetAdcFrame(handle, L502_PROC_FLAGS_VOLT, 1000, &temp[0]);
				if(!err)
					for (int j = 0; j < temp.size(); j++)
					{
						avg[j]+=temp[j];
					}
			}
			for (int i = 0; i < temp.size(); i++)
				avg[i]=avg[i]/5.0;

			return avg;
		}
	}
	else
	{
		for (int i = 0; i < temp.size(); i++)
		{
			avg[i]=-500100;
		}
		return avg;
	}
}
//---------------------------------------------------------------------------
int LCard502::Start(bool isTest_)
{
	isTest = isTest_;
	int32_t err = setSettings(Parameters);
	if(err)
		return err;
	//	Разрешение синхронных потоков на ввод/вывод.
	if(!err)
		err = L502_StreamsEnable(handle, L502_STREAM_ADC);
	if(!err)
	{
		  /* если остался не удаленный объект потока - удаляем */
		if (thread)
		{
			delete thread;
			thread = NULL;
		}
		//создание класса потока
		thread = new L502_ProcessThread(true,handle);
		thread->OnTerminate = OnThreadTerminate;
		//!Todo подумать над Start();
		thread->Resume();  			//запуск потока
		threadRunning = true;
	}

	CountCollectedMeasurements=0;
	CountMeasurementsStartLinear = 0;
	CountMeasurementsStartCross = 0;
	CountMeasurementsStoppedCross = 0;
	CountMeasurementsStoppedLinear = 0;
	CountMeasurementsStartSolidGroup = 0;
	return err;
}
//---------------------------------------------------------------------------
bool LCard502::Stop()
{
	if (threadRunning)
	{
		thread->stop = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall LCard502::OnThreadTerminate(TObject *obj)
{
	threadRunning = false;
}
//---------------------------------------------------------------------------
__fastcall L502_ProcessThread::L502_ProcessThread(bool CreateSuspended, t_l502_hnd hnd_)
	: TThread(CreateSuspended), stop(false), err (L502_ERR_OK)
{
	hnd = hnd_;
	LCard_parameters param = lcard->getSettings();
	RECV_TOUT  = param.RECV_TOUT;
	bufferSize = param.getCountChannels() *5;

}
//---------------------------------------------------------------------------
void __fastcall L502_ProcessThread::Execute()
{
	/* выделяем буферы под принимаемые данные */
	std::vector<uint32_t> adc_data;
	std::vector<double> volt_data;

	adc_data.resize(bufferSize);
	volt_data.resize(bufferSize);

	if (adc_data.empty() || volt_data.empty())
	{
		err = L502_ERR_MEMORY_ALLOC;
	}
	else
	{
		/* запускаем синхронные потоки */
		err = L502_StreamsStart(hnd);
		if (!err)
		{
			/* выполняем прием пока не произойдет ошибка или
				не будет запроса на останов от основного приложения */
			while (!stop && !err)
			{
				/* принимаем данные синхронного ввода */
				int32_t rcv_size = L502_Recv(hnd, &adc_data[0], bufferSize, RECV_TOUT);
				/* значение меньше нуля означает ошибку... */
				if (rcv_size < 0)
					err = rcv_size;
				else if (rcv_size>0)
				{
					/* если больше нуля - значит приняли новые данные */
					uint32_t size = uint32_t(rcv_size);
					/* получаем номер лог. канала, соответствующий первому
						отсчету АЦП, так как до этого могли обработать
						некратное количество кадров */
					err = L502_GetNextExpectedLchNum(hnd, &firstLch);
					if (!err)
					{
					 //		переводим АЦП в Вольты
						err = L502_ProcessAdcData(hnd, &adc_data[0], &volt_data[0], &size, L502_PROC_FLAGS_VOLT);
					}

					if (!err)
					{
						lcard->insertData(volt_data,rcv_size);
					}
				}
			}
			/* по выходу из цикла отсанавливаем поток.
			   Чтобы не сбросить код ошибки (если вышли по ошибке)
			   результат останова сохраняем в отдельную переменную */
			int32_t stop_err = L502_StreamsStop(hnd);
			if (!err)
				err = stop_err;
		}
	}
}
String L502_ProcessThread::GetError()
{
	return String(L502_GetErrorString(err));
}

#pragma package(smart_init)
