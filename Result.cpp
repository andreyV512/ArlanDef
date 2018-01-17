//---------------------------------------------------------------------------
#pragma hdrstop
#include "Result.h"
#include "IniFiles.hpp"
#include <algorithm>
#include "Global.h"
#include "DspFilters/Dsp.h"
#include <Math.hpp>
#include <Series.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//-----------------------------------------------------------------------------
// abs работает только с long, пришлось запилить свою функцию
template <typename T>
T Abs(T value)
{
	if(value < 0)
		value = value * -1;

	return value;
}
vector<double> Abs(vector<double> V)
{
	for(__int64 i=0; i< V.size(); i++)
	{
		V[i] = Abs(V[i]);
	}
	return V;
}
Singleton::Singleton()
{
	CrossResult = new Result(Globals::Cross_sensors);
	LinearResult = new Result(Globals::LinSensors);
	ThResult = new ThicknessResult();
	SumResult = new SummaryResult();
}

Singleton::~Singleton()
{
	delete CrossResult;
	delete LinearResult;
	delete ThResult;
	delete SumResult;
}

Singleton* Singleton::Instance()
{
	if( _self == NULL )
		_self = new Singleton();

	return _self;
}

void Singleton::deleteInstance()
{
	if( _self )
	{
		delete _self;
	}
}
Singleton* Singleton ::_self = NULL;

//-----------------------------------------------------------------------------
Result::Result()
{
	Init();
}
//-----------------------------------------------------------------------------
Result::Result(int sensors)
{
	Init();
	Filtered_Data.resize(Globals::max_zones);
	Source_Data.resize(Globals::max_zones);

	for (int i = 0; i < Filtered_Data.size(); i++)
	{
		Filtered_Data[i].resize(sensors);
		Source_Data[i].resize(sensors);
	}
}
//---------------------------------------------------------------------------
void Result::Init()
{
	SensorForm[0] = NULL;
	SensorForm[1] = NULL;
	shiftZone = -1;
	sensors=0;
	zones=0;
	meas_per_zone=0;
	borders.set_length(2);
	dead_zone_start=0;
	dead_zone_finish=0;
	meas_dead_zone_start=0;
	meas_dead_zone_finish=0;
	total_meas = 0;
	TypeView = 0;
    zone_length = 200;
	zone_data.set_length(Globals::max_zones);

	sensor_data.resize(Globals::max_zones);
	for (int i = 0; i < sensor_data.size(); i++)
		sensor_data[i].resize(Globals::max_sensors);

	// задаем цвета, которые хочет юзер
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	Good = (TColor) ini->ReadInteger("Color","Valid",0);
	Dead = (TColor) ini->ReadInteger("Color","DeadZone",0);
	gain.resize(Globals::max_sensors);
	WidthMedianFilter = ini->ReadInteger("OtherSettings","WidthMedianFilter",5);
	delete ini;

	data.resize(Globals::max_zones);
	for (int i = 0; i < data.size(); i++)
		data[i].resize(Globals::max_sensors);

}
//---------------------------------------------------------------------------
Result::~Result()
{
	int i,j;
	borders.~DynamicArray();
	zone_data.~DynamicArray();
}
//---------------------------------------------------------------------------
void Result::ComputeZoneData(bool isLinear)
{
// пробегает по всем значениям, выискивая максимальные дефекты по зонам и датчикам
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors; i++)
		{
			total_meas += Source_Data[j][i].size();
			data[j][i] = Source_Data[j][i];
			if(SystemConst::isMedianFilter)
			{
				data[j][i].resize( Source_Data[j][i].size() );
				medianfilter(&Source_Data[j][i][0],&data[j][i][0],Source_Data[j][i].size());
			}
			//! переводим значения в абсолютные и фильтруем если надо
			data[j][i] = Abs( Source_Data[j][i] );
			Filtered_Data[j][i] = Source_Data[j][i];

			if(SystemConst::isLinearDigitalFilter && isLinear)
			{
				LinearFilter->toFilter(&Filtered_Data[j][i][0],Filtered_Data[j][i].size());
				data[j][i] = Abs(Filtered_Data[j][i]);
			}
			if(SystemConst::isCrossDigitalFilter && !isLinear)
			{
				CrossFilter->toFilter(&Filtered_Data[j][i][0],Filtered_Data[j][i].size());
				data[j][i] = Abs(Filtered_Data[j][i]);
			}

			//макс. значение по датчику в текущей зоне
			sensor_data[j][i] = gain[i] * (*std::max_element(data[j][i].begin(),data[j][i].end()) );
		}
        //макс. значение в текущей зоне
		zone_data[j] = *std::max_element( sensor_data[j].begin(),sensor_data[j].end() );
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Result::_medianfilter(double* signal, double* result, int N)
{

   //   Move window through all elements of the signal
   for (int i = 2; i < N - 2; ++i)
   {
	  //   Pick up window elements
	  vector <double> window;
	  window.resize(WidthMedianFilter);
	  for (int j = 0; j < WidthMedianFilter; ++j)
		 window[j] = signal[i - 2 + j];
	  //Order elements (only half of them)
	  for (int j = 0; j < 3; ++j)
	  {
		 //Find position of minimum element
		 int min = j;
		 for (int k = j + 1; k < WidthMedianFilter; ++k)
			if (window[k] < window[min])
			   min = k;
		 //   Put found minimum element in its place
		 const double temp = window[j];
		 window[j] = window[min];
		 window[min] = temp;
	  }
	  //Get result - the middle element
	  result[i - 2] = window[2];
   }
}
//------------------------------------------------------------------------------
//   1D MEDIAN FILTER wrapper
//     signal - input signal
//     result - output signal
//     N      - length of the signal
void Result::medianfilter(double* signal, double* result, int N)
{
   //   Check arguments
   if (!signal || N < 1)
	  return;
   //   Treat special case N = 1
   if (N == 1)
   {
      if (result)
		 result[0] = signal[0];
      return;
   }
   //   Allocate memory for signal extension
   double* extension = new double[N + 4];
   //   Check memory allocation
   if (!extension)
      return;
   //   Create signal extension
   memcpy(extension + 2, signal, N * sizeof(double));
   for (int i = 0; i < 2; ++i)
   {
      extension[i] = signal[1 - i];
      extension[N + 2 + i] = signal[N - 1 - i];
   }
   //   Call median filter implementation
   _medianfilter(extension, result ? result : signal, N + 4);
   //   Free memory
   delete[] extension;
}
//----------------------------------------------------------------------------
//bool Result::AddZone( vector< vector < double > > new_data )
//{
//	sensors = (short) new_data.size();
//	if(SystemConst::isMedianFilter)
//	{   for(int i=0; i < sensors; i++)
//			medianfilter(&new_data[i][0] , &new_data[i][0] , new_data.size());
//	}
//	for (long i = 0; i < sensors; i++)
//	{
//		Source_Data[zones][i] = new_data[i];
//		data[zones][i] = Abs(Source_Data[zones][i]);
//		sensor_data[ zones ][ i ] = gain[i]* (*std::max_element(data[zones][i].begin(),data[zones][i].end() ) );
//	}
//	meas_per_sensor.Length++;
//	meas_per_sensor[meas_per_sensor.High] = new_data[0].size();
//	zone_data[ zones ] = *std::max_element(sensor_data[zones].begin(),sensor_data[zones].end());
//	total_meas += meas_per_sensor[meas_per_sensor.High];
//	zones++;
//	if(ZoneColor(zone_data[zones-1]) == Brack)
//		return true;
//	return false;
//}bool Result::AddZone( vector< vector < double > > new_data,bool isLinear )
{
	if(zones>=Globals::max_zones)
	{
		AnsiString a="Result::AddZone: Количество зон ";
		a+=zones;
		a+=" больше или равно максимума ";
		a+=Globals::max_zones;
		TPr::pr(a);
		return false;
	}
	DWORD begin = GetTickCount();
	sensors = (short) new_data.size();
	if(SystemConst::isMedianFilter)
	{   for(int i=0; i < sensors; i++)
			medianfilter(&new_data[i][0] , &new_data[i][0] , new_data.size());
	}
	for (long i = 0; i < sensors; i++)
	{
		Source_Data[zones][i] = new_data[i];
		if(SystemConst::isLinearDigitalFilter && isLinear)
		{
			Filtered_Data[zones][i] = new_data[i];
			LinearFilter->toFilter(&Filtered_Data[zones][i][0],Filtered_Data[zones][i].size());
			data[zones][i] = Abs(Filtered_Data[zones][i]);
		}
		else if(SystemConst::isCrossDigitalFilter && !isLinear)
		{
			Filtered_Data[zones][i] = new_data[i];
			CrossFilter->toFilter(&Filtered_Data[zones][i][0],Filtered_Data[zones][i].size());
			data[zones][i] = Abs(Filtered_Data[zones][i]);
		}
		else
			data[zones][i] = Abs(Source_Data[zones][i]);

		sensor_data[ zones ][ i ] = gain[i]* (*std::max_element(data[zones][i].begin(),data[zones][i].end() ) );
	}

	meas_per_sensor.Length++;
	meas_per_sensor[meas_per_sensor.High] = new_data[0].size();
	zone_data[ zones ] = *std::max_element(sensor_data[zones].begin(),sensor_data[zones].end());
	total_meas += meas_per_sensor[meas_per_sensor.High];
	zones++;

//	DWORD end = GetTickCount();
//	if(!isLinear)
//		TPr::SendToProtocol("Время расчета зоны "+String(zones)+" поперечного "+String(end-begin));
//	else
//		TPr::SendToProtocol("Время расчета зоны "+String(zones)+" продольного "+String(end-begin));
	if(ZoneColor(zone_data[zones-1]) == Brack)
		return true;
	return false;
}
//---------------------------------------------------------------------------
void Result::CalculateDeadFront(int LengthZone)				// считает мертвую зону в начале
{
	//Костыль для Игола
	dead_zone_start+=200;
	if (SystemConst::IsOnline)    	// для онлайн дефектоскопии (ЛИР)
	{
		int full_start = Math::Floor((float)dead_zone_start / (float)LengthZone);
		if (zones > 0)
		{
			// полные зоны
			for (int j = 0; j < full_start; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[j][i] = 1;
				zone_data[j] = 1;
			}
			// неполная зона  Старт
			int dead_part = (int) ( (float)(dead_zone_start % LengthZone) / (float)LengthZone * meas_per_sensor[full_start] );
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = dead_part; k < meas_per_sensor[full_start]; k++)
				{
					if ( data[ full_start ][i][k] > max1 )
						max1 = data[ full_start ][i][k];
				}
				sensor_data[ full_start ][i] = max1 * gain[i];
				if ( sensor_data[ full_start ][i] > max2 )
					max2 = sensor_data[ full_start ][i];
			}
			zone_data [ full_start ] = max2;
			// для закрашивания серым
			meas_dead_zone_start = 0;
			for (int p = 0; p < full_start; p++)
				meas_dead_zone_start += meas_per_sensor[p];
			meas_dead_zone_start += dead_part;
		}
	}
	else		// статика (без ЛИРов)
	{
		meas_dead_zone_start = ( (float) meas_per_zone / (float) LengthZone) * (float) dead_zone_start;
		if ( meas_per_zone )
		{
			// кол-во полных мертвых зон в начале
			int dead_zones_start = 	Math::Floor( (float) meas_dead_zone_start / (float) meas_per_zone );
			// остаток мертвых измреений в неполной зоне
			int dead_part = (int) meas_dead_zone_start - dead_zones_start * meas_per_zone ;
			for (int j = 0; j < dead_zones_start; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[j][i] = 1;
				zone_data[j] = 1;
			}

			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = dead_part; k < meas_per_zone; k++)
				{
					if ( data[ dead_zones_start ][i][k] > max1 )
						max1 = data[ dead_zones_start ][i][k];
				}
				sensor_data[ dead_zones_start ][i] = max1 * gain[i];
				if ( sensor_data[ dead_zones_start ][i] > max2 )
					max2 = sensor_data[ dead_zones_start ][i];
			}
			zone_data [ dead_zones_start ] = max2;
		}
	}
	dead_zone_start-=200;
}
//---------------------------------------------------------------------------
void Result::CalculateDeadBack(int LengthZone)				// считает мертвую зону в конце
{
	dead_zone_finish+=200;
	if (SystemConst::IsOnline)
	{
		int full_finish = 0;
		if (zones > 0)
		{
			double dead_zones = (double)dead_zone_finish / (double)LengthZone;
			// ищем начало фэйка в последней неполной зоне
			int i;
			for ( i = meas_per_sensor[zones - 1] - 1; i > 0 ; i--)
				if (data[zones-1][0][i] != 0.0)
					break;
			int meas_real = i;		// реальные измерения в последней зоне
			double last_part = (double) meas_real / (double) meas_per_sensor[zones - 1];
			double dead_full = dead_zones - last_part;
			while (dead_full > 0.0)		// ищем, сколько полных зон покрыло мертвым
			{
				dead_full -= 1.0;
				full_finish ++;
			}
			double not_full = 1.0 + dead_full;		// неполная зона (не обязательно последняя, может быть = zones-2,-3 и т.д.)
			// полные зоны
			for (int j = 0; j < full_finish; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[ zones - j -1][i] = 1;
				zone_data[ zones - j -1] = 1;
			}
			// неполная зона ФИниш
			int dead_part_end = (int) (not_full * (double) meas_per_sensor[zones-full_finish-1]);
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = 0; k < meas_per_sensor[zones-full_finish-1] - dead_part_end; k++)
				{
					if ( data[ zones - full_finish-1 ][i][k] > max1 )
						max1 = data[ zones - full_finish-1 ][i][k];
				}
				sensor_data[ zones - full_finish-1 ][i] = max1 * gain[i];
				if ( sensor_data[ zones - full_finish-1 ][i] > max2 )
					max2 = sensor_data[ zones - full_finish-1 ][i];
			}
			zone_data [ zones - full_finish-1 ] = max2;

			// для закрашивания серым
			meas_dead_zone_finish = 0;
			for (int p = 0; p < full_finish; p++)
				meas_dead_zone_finish += meas_per_sensor[zones - p -1];
			meas_dead_zone_finish += dead_part_end;
		}
	}
	else		// статика (без ЛИРа)
	{
		meas_dead_zone_finish = ( (float) meas_per_zone / (float) LengthZone) * (float) dead_zone_finish;

		//------------- посчитаем мертвые зоны----------
		if ( meas_per_zone )
		{

			// кол-во полных мертвых зон в конце (НЕПРАВИЛЬНО, нужно учесть последнюю неполную зону)
			int dead_zones_finish = Math::Floor( (float) meas_dead_zone_finish / (float) meas_per_zone );
			// остаток мертвых измреений в неполной зоне
			int dead_part_end = (int) meas_dead_zone_finish - dead_zones_finish * meas_per_zone ;
			for (int j = 0; j < dead_zones_finish; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[ zones - j -1][i] = 1;
				zone_data[ zones - j -1] = 1;
			}
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = 0; k < meas_per_zone - dead_part_end; k++)
				{
					if ( data[ zones - dead_zones_finish - 1 ][i][k] > max1 )
						max1 = data[ zones - dead_zones_finish - 1 ][i][k];
				}
				sensor_data[ zones - dead_zones_finish - 1 ][i] = max1 * gain[i];
				if ( sensor_data[ zones - dead_zones_finish - 1 ][i] > max2 )
					max2 = sensor_data[ zones - dead_zones_finish - 1 ][i];
			}
			zone_data [ zones - dead_zones_finish - 1 ] = max2;
		}
	}
	dead_zone_finish-=200;
}
//---------------------------------------------------------------------------
void Result::CalculateDeadZone(int LengthZone)
{
	CalculateDeadFront(LengthZone);
	CalculateDeadBack(LengthZone);
}
//---------------------------------------------------------------------------
TColor Result::ZoneColor(double meas,int XCoord)
{
	// возвращает цвет в зависимости от измерения
	if( ( (XCoord < meas_dead_zone_start) ||
		  (XCoord > (total_meas - meas_dead_zone_finish)) ) &&
		  (XCoord > 0) )
		return Dead;
	else
	if (meas < borders[borders.get_high()])
		return Good;
	else
	if (meas >= borders[0])
		return Brack;
	else
		return Class2;
}
//---------------------------------------------------------------------------
void Result::PutResultOnChart(TChart *C, TComponent *Owner)
{
// рисует результат по зонам на чарте С;
// чтобы графики были адекватными, все оставшиеся зоны необходимо забить нулями (глюк Teechart)
	C->LeftAxis->Maximum = sensors;
	for (int i = 0; i < sensors; i++)
	{
		C->Series[i]->Tag=i;
		((TBarSeries*) C->Series[i])->OnDblClick=SeriesDblClick;
		((TBarSeries*) C->Series[i])->OnClick=SeriesShiftClick;
		C->Series[i]->Clear();
		for (int j = 0; j < Globals::max_zones; j++)
			if ( j<zones )
				C->Series[i]->AddXY(j,1,"",ZoneColor( sensor_data[j][i] ));
			else
				C->Series[i]->Add(0,"",clWhite);
	}
// создание формы для вывода измерений по одному датчику
	for(int i=0; i < 2; i++)
		if (!SensorForm[i])
		{
			SensorForm[i] = new TForm(Owner);
			SensorForm[i]->OnKeyPress = (TKeyPressEvent) &EscapeKeyPress;
			SensorForm[i]->SetBounds(600,500,300,300);

			SensorChart[i] = new TChart(SensorForm[i]);
			SensorChart[i]->Parent = SensorForm[i];
			SensorChart[i]->SetBounds( 10, 10 , 260 , 240 );
			SensorChart[i]->Legend->Visible = false;
			SensorChart[i]->View3D = false;
			SensorChart[i]->BottomAxis->Visible = true;
			SensorChart[i]->LeftAxis->Automatic = false;
			if(i == 0)
				SensorChart[i]->LeftAxis->Minimum = 0;
			else
				SensorChart[i]->LeftAxis->Minimum = -100;

			SensorChart[i]->LeftAxis->Maximum = 100;
			SensorChart[i]->Walls->Back->Transparent = false;
			SensorChart[i]->Walls->Back->Transparency = 30;
			SensorChart[i]->BackWall->Color = clDkGray;
			SensorChart[i]->Align = alClient;
			if(i == 0)
				SensorChart[i]->AddSeries(new TBarSeries(Owner));
			else
				SensorChart[i]->AddSeries(new TLineSeries(Owner));
			SensorChart[i]->AddSeries(new TLineSeries(Owner));
			SensorChart[i]->AddSeries(new TLineSeries(Owner));
			SensorChart[i]->AddSeries(new TLineSeries(Owner));                   //Это чарт для отображения сырого сигнала
			SensorChart[i]->Series[0]->Marks->Visible = false;
			if(i == 0)
				((TBarSeries*) SensorChart[i]->Series[0])->BarPen->Visible = false;

			SensorBar[i] = new TStatusBar(SensorForm[i]);
			SensorBar[i]->Parent = SensorForm[i];
			SensorBar[i]->Panels->Add();
			SensorBar[i]->SetBounds( 10, 260 , 260 , 30 );
			SensorBar[i]->Panels->Items[0]->Text = "Расстояние:";
			SensorForm[i]->Refresh();
			SensorChart[i]->OnClickBackground = (TChartClick) &ChartClickBackground;
		}
}
//---------------------------------------------------------------------------
void __fastcall Result::ChartClickBackground(TCustomChart *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbLeft)
	{
		double x,y;
		SensorChart[TypeView]->Series[0]->GetCursorValues(x,y);
      	int curr_sens_cur = (int)x;
        if ( curr_sens_cur < 1 )
        {
        	curr_sens_cur = 1;
        }
        long temp_meas = 0;
        	for(int k = 0; k < countZone; k++)
        		temp_meas += meas_per_sensor[startZone+k];
		int Range;
		Range = (startZone) * zone_length + (curr_sens_cur*zone_length*countZone)/temp_meas;
		SensorChart[TypeView]->SetFocus();
		SensorBar[TypeView]->Panels->Items[0]->Text = L"Расстояние: ~" + FloatToStr(Range)+L"мм";
 	}
}
//---------------------------------------------------------------------------
void Result::PutSourceDataOnChart(TChart *Chart,TComponent *Owner, int zone, int sensor)
{
	double v;
	Chart->Series[0]->Clear();
	Chart->Series[3]->Clear();
	if (SystemConst::IsOnline)
	{
		long temp_meas = 0;
		for(int k = 0; k < zone; k++)
			temp_meas += meas_per_sensor[k];

		for (int j = 0; j < meas_per_sensor[zone]; j++)
		{
	 //		v = Filtered_Data[zone][sensor][j];
			Chart->Series[0]->AddXY( j , Source_Data[zone][sensor][j] * gain[sensor],"",clGreen);
		//	Chart->Series[3]->AddXY( j , v * gain[sensor],"",clRed);
		}
	}
	else
	{
		for (int j = 0; j < meas_per_zone; j++)
		{
	 //		v = Filtered_Data[zone][sensor][j];
			Chart->Series[0]->AddXY( j , Source_Data[zone][sensor][j] * gain[sensor],"",clGreen);
		//	Chart->Series[3]->AddXY( j , v * gain[sensor],"",clRed);
		}
	}

	// рисуем пороги брака и класса 2, если он включен
	Chart->Series[1]->Clear();
	Chart->Series[2]->Clear();
	for (int j = 0; j < borders.Length; j++)
	{
		Chart->Series[j+1]-> AddXY( 0 , borders[j] ,"" , ZoneColor( borders[j] ) );
		Chart->Series[j+1]-> AddXY(Chart->Series[0]->MaxXValue() , borders[j] , "" , ZoneColor( borders[j] ) );
	}
}
//-------------------------------------------------------------------------------
void Result::PutDataOnChart(TChart *Chart,TComponent *Owner, int zone, int sensor)
{
	// рисует измерения в зоне/датчике на графике
	Chart->Series[0]->Clear();

	if (SystemConst::IsOnline)
	{
		long temp_meas = 0;
		for(int k = 0; k < zone; k++)
			temp_meas += meas_per_sensor[k];
		for (int j = 0; j < meas_per_sensor[zone]; j++)
		{
			Chart->Series[0]->AddXY( j , data[zone][sensor][j] * gain[sensor],"" ,
				ZoneColor( data[zone][sensor][j] * gain[sensor], temp_meas + j ));
		}
	}
	else
	{
		for (int j = 0; j < meas_per_zone; j++)
		{
			Chart->Series[0]->AddXY( j , data[zone][sensor][j] * gain[sensor],"" ,
				ZoneColor( data[zone][sensor][j] * gain[sensor], zone*meas_per_zone+j ));
		}
	}
// рисуем пороги брака и класса 2, если он включен
	Chart->Series[1]->Clear();
	Chart->Series[2]->Clear();
	for (int j = 0; j < borders.Length; j++)
	{
		Chart->Series[j+1]-> AddXY( 0 , borders[j] ,"" , ZoneColor( borders[j] ) );
		Chart->Series[j+1]-> AddXY(Chart->Series[0]->MaxXValue() , borders[j] , "" , ZoneColor( borders[j] ) );
	}
}
//---------------------------------------------------------------------------
void __fastcall Result::SeriesDblClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if( Shift.Contains(ssCtrl) )
		TypeView = 1;
	else	TypeView = 0;

// закрашиваем выделенный пользователем датчик
	Sender->ValueColor[ValueIndex]=clNavy;
// в тэге хранится номер датчика (серии точек)
	int sn = Sender->Tag;
	SensorForm[TypeView]->Left = 600;
	SensorForm[TypeView]->Top = 500;
	SensorForm[TypeView]->Width = 400;
	SensorForm[TypeView]->Height = 400;
//	поместим измерения на график
// Сигнал по модулю
	if(TypeView == 0)
		{
			SensorForm[TypeView]->Caption="Датчик "+IntToStr(sn+1)+", зона "+IntToStr(ValueIndex+1);
			PutDataOnChart(SensorChart[TypeView], Sender, ValueIndex, sn);
		}
	else      //Исходный сигнал
		{
			SensorForm[TypeView]->Caption="Датчик "+IntToStr(sn+1)+", зона "+IntToStr(ValueIndex+1)+" (Биполярный сигнал)";
			PutSourceDataOnChart(SensorChart[TypeView], Sender, ValueIndex, sn);
		}
	startZone = ValueIndex;
	countZone = 1;
	SensorForm[TypeView]->Update();
	SensorForm[TypeView]->Refresh();
	SensorForm[TypeView]->Repaint();
	SensorForm[TypeView]->Show();
}
//---------------------------------------------------------------------------
void __fastcall Result::SeriesShiftClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if( Shift.Contains(ssCtrl) )
		TypeView = 1;
	else	TypeView = 0;

	if (Shift.Contains(ssShift))
	{
		 // закрашиваем выделенный пользователем датчик
		 Sender->ValueColor[ValueIndex]=clPurple;
		 if (shiftZone==-1) //Выбрана первая граница
		 {
			shiftZone = ValueIndex;
			sens = Sender->Tag;
		 }
		 else //Выбрана вторая граница
		 {
			if (shiftZone>ValueIndex)
			{
				int x = shiftZone;
				shiftZone = ValueIndex;
				ValueIndex = x;
			}
			// в тэге хранится номер датчика (серии точек)
			//if (sn==null) sensor = Sender->Tag;
			SensorForm[TypeView]->Left = 600;
			SensorForm[TypeView]->Top = 500;
			SensorForm[TypeView]->Width = 400;
			SensorForm[TypeView]->Height = 400;
			//	поместим измерения на график
			// Сигнал по модулю
			if(TypeView == 0)
			{
				SensorForm[TypeView]->Caption="Датчик "+IntToStr(sens+1)+", зоны с "+IntToStr(shiftZone+1)+" по "+IntToStr(ValueIndex+1);
				PutDataOnChartMas(SensorChart[TypeView], Sender, shiftZone, ValueIndex, sens);
			}
			else      //Исходный сигнал
			{
				SensorForm[TypeView]->Caption="Датчик "+IntToStr(sens+1)+", зоны с "+IntToStr(shiftZone+1)+" по "+IntToStr(ValueIndex+1)+" (Биполярный сигнал)";
				PutSourceDataOnChartMas(SensorChart[TypeView], Sender, shiftZone, ValueIndex, sens);
			}
			startZone = shiftZone;
			countZone = ValueIndex-shiftZone+1;
			shiftZone=-1;
			sens=NULL;
			SensorForm[TypeView]->Update();
			SensorForm[TypeView]->Refresh();
			SensorForm[TypeView]->Repaint();
			SensorForm[TypeView]->Show();
		 }
	}

}
//---------------------------------------------------------------------------
void Result::PutDataOnChartMas(TChart *Chart,TComponent *Owner, int zone1, int zone2, int sensor)
{
	// рисует измерения в зоне/датчике на графике
	Chart->Series[0]->Clear();

	if (zone1>zone2)
	{
		int x = zone1;
		zone1 = zone2;
		zone2 = x;
	}
	int Xposition=0;
	if (SystemConst::IsOnline)
	{
		long temp_meas = 0;
		for(int k = 0; k < zone1; k++)
			temp_meas += meas_per_sensor[k];
		for (int z = zone1; z <= zone2; z++)
		{
			for (int j = 0; j < meas_per_sensor[z]; j++)
			{
				Chart->Series[0]->AddXY( Xposition , data[z][sensor][j] * gain[sensor],"" ,
					ZoneColor( data[z][sensor][j] * gain[sensor] ));
				Xposition++;
			}
		}

	}
	else
	{
		for (int z = zone1; z <= zone2; z++)
		{
			for (int j = 0; j < meas_per_zone; j++)
			{
				Chart->Series[0]->AddXY( Xposition , data[z][sensor][j] * gain[sensor],"" ,
					ZoneColor( data[z][sensor][j] * gain[sensor] ));
				Xposition++;
			}
		}

	}
// рисуем пороги брака и класса 2, если он включен
	Chart->Series[1]->Clear();
	Chart->Series[2]->Clear();
	for (int j = 0; j < borders.Length; j++)
	{
		Chart->Series[j+1]-> AddXY( 0 , borders[j] ,"" , ZoneColor( borders[j] ) );
		Chart->Series[j+1]-> AddXY(Chart->Series[0]->MaxXValue() , borders[j] , "" , ZoneColor( borders[j] ) );
	}
}
//---------------------------------------------------------------------------
void Result::PutSourceDataOnChartMas(TChart *Chart,TComponent *Owner, int zone1, int zone2, int sensor)
{
	double v;
	Chart->Series[0]->Clear();
	Chart->Series[3]->Clear();
 	if (zone1>zone2)
	{
		int x = zone1;
		zone1 = zone2;
		zone2 = x;
	}
	int Xposition=0;
	if (SystemConst::IsOnline)
	{
		long temp_meas = 0;
		for(int k = 0; k < zone2; k++)
			temp_meas += meas_per_sensor[k];
		for (int z = zone1; z <= zone2; z++)
		{
			for (int j = 0; j < meas_per_sensor[z]; j++)
			{
		 //		v = Filtered_Data[zone][sensor][j];
				Chart->Series[0]->AddXY( Xposition , Source_Data[z][sensor][j] * gain[sensor],"",clGreen);
			//	Chart->Series[3]->AddXY( j , v * gain[sensor],"",clRed);
				Xposition++;
			}
		}
	}
	else
	{
		for (int z = zone1; z <= zone2; z++)
		{
			for (int j = 0; j < meas_per_zone; j++)
			{
		 //		v = Filtered_Data[zone][sensor][j];
				Chart->Series[0]->AddXY( Xposition , Source_Data[z][sensor][j] * gain[sensor],"",clGreen);
			//	Chart->Series[3]->AddXY( j , v * gain[sensor],"",clRed);
				Xposition++;
			}
		}
	}

	// рисуем пороги брака и класса 2, если он включен
	Chart->Series[1]->Clear();
	Chart->Series[2]->Clear();
	for (int j = 0; j < borders.Length; j++)
	{
		Chart->Series[j+1]-> AddXY( 0 , borders[j] ,"" , ZoneColor( borders[j] ) );
		Chart->Series[j+1]-> AddXY(Chart->Series[0]->MaxXValue() , borders[j] , "" , ZoneColor( borders[j] ) );
	}
}
//-------------------------------------------------------------------------------

void Result::FillData(vector < vector < double > > sourceData)
{
	long MeasPerSensor = sourceData[0].size();
	Result::sensors = sourceData.size();
	long i,j,p,q;

	if ( ( MeasPerSensor % zones) != 0 )
	{
		// если последняя зона не является короткой
		// заполняем целые зоны
		int pos=(int)(MeasPerSensor/zones); //кол-во измерений в одной зоне

		Result::meas_per_zone = pos;
		for ( i = 0; i < zones; i++)
			for ( j = 0; j < sensors; j++)
				for ( p = pos*i; p < pos*(i+1); p++)
					Source_Data[i][j].push_back(sourceData[j][p]);


	}
	else
	{
		//заполняем целые зоны
		int pos=((int)(MeasPerSensor))/(zones-1); //кол-во измерений в одной полной	зоне
		Result::meas_per_zone = pos;
		for ( i = 0; i < zones-1; i++)
			for ( j = 0; j < sensors; j++)
				for ( p = pos*i; p < pos*(i+1); p++)
					Source_Data[i][j].push_back(  sourceData[j][p] );

		//Заполняем последнюю зону
		for ( j = 0; j < sensors; j++)
			for ( q = pos*(zones-1); q < MeasPerSensor; q++)
				Source_Data[zones-1][j].push_back( sourceData[j][q] );
	}
}
//------------------------------------------------------------------------------
void __fastcall Result::EscapeKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27) ((TForm*)Sender)->Close();
}
//------------------------------------------------------------------------------
void Result::SaveTubeToFile(UnicodeString FileName)
{
	//Формат файла .dkb для дефектоскопа
	//1/0 метка RunTime/Normal
	//zones,sensors
	//meas_per_zone / meas_per_sensor[]
	//zone_data[]
	//sensor_data[][]
	//Source_Data[][][]

	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"Не удалось открыть файл для записи",L"Ошибка", MB_ICONERROR | MB_OK  );

	if (SystemConst::IsOnline)
		fprintf(file,"%d",1);                                       //Ставим метку, если это RunTime труба
	else
		fprintf(file,"%d",0);                                       //или обычный дефектоскоп
	fprintf(file,"\n");
	fprintf(file,"%d %d ", zones, sensors);                         //кол-во зон,датчиков

	if (SystemConst::IsOnline)                                      //Если дефектоскоп RunTime, то пишем длину каждой из зон
		for (int i = 0; i < zones; i++)
			fprintf( file,"%d ",meas_per_sensor[i] );
	else
		fprintf(file, "%d",meas_per_zone);							//Иначе просто записываем длину одной зоны

	//пишем данные по зонам
	fprintf(file,"\n");
	for( int i=0; i< zones; i++ )
		fprintf(file,"%4.1f ",zone_data[i]);
	fprintf(file,"\n");
	//пишем данные по датчикам
	for ( int i = 0; i <zones ; i++)
		for(int j=0; j<sensors; j++)
			fprintf(file,"%4.1f ",sensor_data[i][j]);

	//пишем все собранные данные
	fprintf(file,"\n");
	for (int j = 0; j < zones; j++)                                 //Зона
	{
		fprintf(file,"\n");
		for (int i = 0; i < sensors; i++)                           //Датчик
		{
		if (SystemConst::IsOnline)                                  //Если RunTime, то в каждой зоне разное кол-во измерений
			for (int k = 0; k < meas_per_sensor[j]; k++)
				fprintf(file,"%4.1f ",Source_Data[j][i][k]);
		else                                                        //Если обычный, то пишем кол-во измерений в одной зоне
			for (int k = 0; k < meas_per_zone; k++)
				fprintf(file,"%4.1f ",Source_Data[j][i][k]);
		}
	}
	fclose(file);
}
//------------------------------------------------------------------------------
void Result::LoadTubeFromFile(FILE*file)
{
	int isRunTime=0;

	fscanf(file,"%d",&isRunTime);                       //Смотрим в каком формате был сохранен файл ( 1-RunTime,0-normal )
	if( isRunTime > 1 )                                 //Если старый формат файлов - Activated Spike-nail
		{
			isRunTime = 0;
			rewind(file);
		}

	fscanf(file,"%d%d",&zones,&sensors);				//Кол-во зон и датчиков

	if (isRunTime)
	{
		meas_per_sensor.set_length(zones);
		for( int i=0; i < zones; i++ )                  //Если дефектоскоп RunTime, то читаем массив длин каждой зоны
		{
			fscanf(file,"%d", &meas_per_sensor[i]);
			total_meas += meas_per_sensor[i];
		}
	}
	else
	{
		fscanf(file,"%d",&meas_per_zone);               //Если обычный то просто считаем длину зоны
		total_meas = meas_per_zone * zones;
		// заполним значения для онлайн
		meas_per_sensor.set_length(zones);
		for (int i = 0; i < zones; i++)
			meas_per_sensor[i] = meas_per_zone;
	}
	//Читаем данные по зонам
	for( int i=0; i<zones; i++ )
		fscanf(file,"%lf ",&zone_data[i]);

	//Читаем данные по датчикам
	for ( int i = 0; i <zones ; i++)
		for(int j=0; j<sensors; j++)
			fscanf(file,"%lf",&sensor_data[i][j]);

	//Читаем все собранные данные
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors; i++)
		  if (isRunTime)
		  {
			Source_Data[j][i].resize(meas_per_sensor[j]);
			for (int k = 0; k < meas_per_sensor[j]; k++)
			{
				fscanf(file,"%lf ",&Source_Data[j][i][k]);
			}
		  }
		  else
		  {
  			Source_Data[j][i].resize(meas_per_zone);
			for (int k = 0; k < meas_per_zone; k++)
			{
				fscanf(file,"%lf ",&Source_Data[j][i][k]);
			}
		  }
	}
//Файл не закрываем, он будет закрыт в главной функции передающий указатель на открытый файл
}
//----------------------------------------------------------------------------
void Result::DeleteData()
{
	meas_per_sensor.set_length( 0 );
	zone_data.set_length( 0 );
    zone_data.set_length(Globals::max_zones);
	zones = 0;
	sensors = 0;
	meas_per_zone = 0;
	total_meas = 0;

	for (int i = 0; i < Filtered_Data.size(); i++)
		for (long j = 0; j < Filtered_Data[i].size(); j++)
		{
			Filtered_Data[i][j].clear();
			Source_Data[i][j].clear();
			data[i][j].clear();
		}
}
//----------------------------------------------------------------------------

void Result::SetGains(vector <double> _Gain)
{
	gain = _Gain;
}

//----------------------------------------------------------------------------
void Result::SetSensorGain(double _gain, int sensor)
{
	gain [sensor] = _gain;
}
//----------------------------------------------------------------------------
void Result::AutoCalibration(int zone)
{
	double averageMaximus;
	double min;
	double max;
	min = sensor_data[zone][0];
	max = sensor_data[zone][0];
	for(int i=0; i<Globals::Cross_sensors; i++)
	{
		if( ( sensor_data[zone][i] / gain[i] ) > max )
			max = sensor_data[zone][i] / gain[i];

		if( ( sensor_data[zone][i] / gain[i] ) < min )
			min = sensor_data[zone][i] / gain[i];
	}
	averageMaximus = max;//(min + max) / 2;

	TIniFile *ini = new TIniFile(Globals::IniFileName);
	for(int i=0; i<Globals::Cross_sensors; i++)
	{
		gain[i] = averageMaximus / ( (sensor_data[zone][i] / gain[i]) );
		String gain_str = "Gain " + IntToStr(Globals::current_diameter);
		ini->WriteFloat( gain_str , "Gain"+IntToStr(i),StrToFloat(gain[i]));
	}
	delete ini;
}
//----------------------------------------------------------------------------
//-------------Методы наследника  - ThicknessResult--------------------------
//----------------------------------------------------------------------------
ThicknessResult::ThicknessResult()
{
	ThicknessResult::zones=0;
	borders.set_length(2);
	ThicknessResult::zone_data.set_length(Globals::max_zones);
}
//----------------------------------------------------------------------------
ThicknessResult::~ThicknessResult()
{
	ThicknessResult::zone_data.~DynamicArray();
}
//---------------------------------------------------------------------------
void ThicknessResult::ComputeZoneData()
{
	double min1,min2;
	for (int j = 0; j < zones; j++)
	{
		min2=1000;
		for (int i = 0; i < sensors; i++)
		{
			min1=1000;
			for (int k = 0; k < Globals::max_meas; k++)
				if (data[j][i][k]<min1)
					min1=data[j][i][k];
			if (min1<min2) min2=min1;
		}
		zone_data[j]=min2;
	}
}
//----------------------------------------------------------------------------
TColor ThicknessResult::ZoneColor(double meas)
{
// возвращает цвет зоны в зависимости от толщины в этой зоне
	if ( meas == 10.0 )
		return Dead;
	else if (meas > ThicknessResult::borders[borders.get_high()])
		return Good;
	else if (meas <= ThicknessResult::borders[0])
		return Brack;
	else
		return Class2;
}
//----------------------------------------------------------------------------
void ThicknessResult::PutResultOnChart(TChart *Chart, TComponent *Owner)
{
// рисует результат по зонам на чарте С;
	Chart->Series[0]->Clear();
	for (int j = 0; j <= Globals::max_zones; j++)
		if ( j<zones )
			((TBarSeries*) Chart->Series[0])->AddXY(j,zone_data[j],"",ZoneColor(zone_data[j]));
		else
			Chart->Series[0]->Add(0,"",clWhite);
}
//------------------------------------------------------------------------------
void ThicknessResult::SaveTubeToFile(UnicodeString FileName)
{
	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"Не удалось открыть файл для записи",L"Ошибка", MB_ICONERROR | MB_OK  );
	fprintf(file,"\n");
	fprintf(file,"%d ",zones);
	for(int i=0; i < zones; i++)
		fprintf(file, "%2.2f ", zone_data[i]);
	fclose(file);
}
//------------------------------------------------------------------------------
void ThicknessResult::LoadTubeFromFile(FILE *file)
{
	fscanf(file,"%d ",&zones);
	for(int i=0; i < zones; i++)
	{
		double ppp;
//		fscanf(file,"%lf", &zone_data[i]);
		fscanf(file,"%lf", &ppp);
		zone_data[i] = ppp;
	}
}
//------------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------Методы наследника - класса SummaryResult----------------------
//---------------------------------------------------------------------------
SummaryResult::SummaryResult()
{
	SummaryResult::zones = 0;
	SummaryResult::zone_data.set_length(Globals::max_zones);
	cut1 = cut2 = 0;

	decision = "";
}
//---------------------------------------------------------------------------
SummaryResult::~SummaryResult()
{
	SummaryResult::zone_data.~DynamicArray();
}
//---------------------------------------------------------------------------
void SummaryResult::ComputeZonesData()
{

// функция вычисляет решение по зоне из совокупности дефектов и толщины
// смотрим, какие модули принимали участие в работе
	bool lin = Singleton::Instance()->LinearResult->zones > 0;
	bool th = Singleton::Instance()->ThResult->zones > 0;
	if ( lin && th )
		SummaryResult::zones = (short) Min( Singleton::Instance()->CrossResult->zones, Min( Singleton::Instance()->LinearResult->zones, Singleton::Instance()->ThResult->zones ) );
	else if ( lin && !th )
		SummaryResult::zones = (short) Min( Singleton::Instance()->CrossResult->zones, Singleton::Instance()->LinearResult->zones );
	else if ( !lin && th )
		SummaryResult::zones = (short) Min( Singleton::Instance()->CrossResult->zones, Singleton::Instance()->ThResult->zones );
	else if ( !lin && !th )
		SummaryResult::zones = Singleton::Instance()->CrossResult->zones;

// 1 - годно, 0 - брак, 2 - 2 класс, 3 - 3 класс...
	for (int i = 0; i < SummaryResult::zones; i++)
	{
		if ( Singleton::Instance()->CrossResult->zone_data[i] >= Singleton::Instance()->CrossResult->borders[0] ||
				( (lin) && Singleton::Instance()->LinearResult->zone_data[i] >= Singleton::Instance()->LinearResult->borders[0] ) ||
				( (th) && Singleton::Instance()->ThResult->zone_data[i] <= Singleton::Instance()->ThResult->borders[0] ) )
		{
			SummaryResult::zone_data[i]=0;
		}
		else if ( Singleton::Instance()->CrossResult->zone_data[i] < Singleton::Instance()->CrossResult->borders[Singleton::Instance()->CrossResult->borders.get_high()] &&
					( (!lin) || Singleton::Instance()->LinearResult->zone_data[i] < Singleton::Instance()->LinearResult->borders[Singleton::Instance()->LinearResult->borders.get_high()] ) &&
					( (!th) || Singleton::Instance()->ThResult->zone_data[i] > Singleton::Instance()->ThResult->borders[Singleton::Instance()->ThResult->borders.get_high()]) )
		{
			SummaryResult::zone_data[i]=1;
		}
		else
			SummaryResult::zone_data[i]=2;
	}
}
//---------------------------------------------------------------------------
TColor SummaryResult::ZoneColor(double meas)
{
// возвращает цвет зоны в зависимости от суммарного результата в этой зоне
	if (meas == 1)
		return Good;
	else if (meas == 0)
		return Brack;
	else
		return Class2;
}
//---------------------------------------------------------------------------
void SummaryResult::PutResultOnChart(TChart *Chart, TComponent *Owner)
{
// рисует результат по зонам на чарте С;
	Chart->Series[0]->Clear();
	for (int j = 0; j <= Globals::max_zones; j++)
		if ( j < zones)
			((TBarSeries*) Chart->Series[0])->Add(1,"",ZoneColor(zone_data[j]));
		else
			Chart->Series[0]->Add(0,"",clWhite);
	Chart->Refresh();
}
//---------------------------------------------------------------------------
void SummaryResult::MakeDecision()
{
// принимает решение по трубе, заполняет значения резов
	bool flag=false;		// сигнализирует, что текущий участок будет помечен 2-ым классом, если превысит минимальную годную длину
	bool class2=false;		// участок точно будет помечен 2-ым классом
	short temp=0;			// длина текущего участка в зонах
	short tcut1=0; 			// временный первый рез
	cut1=cut2=0;			// резов по умолчанию нет
	decision="";			// решение не принято
	if (min_good_length > 0)
	{
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i]==1)
				temp++;
			else if (zone_data[i]==0)
			{
				if (temp>=min_good_length)
				{
					cut2 = (short) i;
					cut1 = (short) i-temp;
					class2=flag;
				}
				temp=0;
				tcut1=i+1;
				flag=false;
			}
			else if (zone_data[i]==2)
			{
				flag=true;
				temp++;
			}
		}

		if ( temp < min_good_length && cut2==0 && cut1==0 )
			decision = "Брак";
		else if ( temp >= min_good_length )
		{
			if (flag) decision = "Класс 2";
			else decision = "Годно";
			cut1 = tcut1;
			cut2 = 0;
		}
		else if ( cut1>0 || cut2>0 )
		{
			if (class2) decision = "Класс 2";
			else decision = "Годно";
		}

		// ебаный пыть-ях блеать
		if (end_cut > 0 && decision != "Брак")
		{
			if (cut2 == 0)
				cut2 = zones;
			int f1 = ( end_cut - cut1 > 0) ? (end_cut-cut1) : (0);
			int f2 = ( end_cut - zones + cut2 > 0 ) ? (end_cut-zones+cut2) : (0);
			if ( cut2 - cut1 - f1 - f2 < min_good_length )
				decision = "Брак";
		}
        //Примочка для Казахстана
		if(cut1 > 0)
			cut1++;
	}
	// работа по зеленке - любой брак - сразу косяк
	else
	{
		bool brack = false;
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i] == 0)
			{
				brack = true;
				break;
			}
			else if (zone_data[i] == 2)
				class2 = true;
		}
		if (brack)
			decision = "Брак";
		else if (class2)
			decision = "Класс 2";
		else
	        decision = "Годно";
    }
}
//---------------------------------------------------------------------------
void SummaryResult::AddZone()
{
// функция вычисляет решение по зоне из совокупности дефектов и толщины
// смотрим, какие модули принимали участие в работе
	bool lin = Singleton::Instance()->LinearResult->zones > 0;
	bool th = Singleton::Instance()->ThResult->zones > 0;

// 1 - годно, 0 - брак, 2 - 2 класс, 3 - 3 класс...
	int i =	zones;
	if ( Singleton::Instance()->CrossResult->zone_data[i] >= Singleton::Instance()->CrossResult->borders[0] ||
			( (lin) && Singleton::Instance()->LinearResult->zone_data[i] >= Singleton::Instance()->LinearResult->borders[0] ) ||
			( (th) && Singleton::Instance()->ThResult->zone_data[i] < Singleton::Instance()->ThResult->borders[0] ) )
	{
		SummaryResult::zone_data[i]=0;
	}
	else if ( Singleton::Instance()->CrossResult->zone_data[i] < Singleton::Instance()->CrossResult->borders[Singleton::Instance()->CrossResult->borders.get_high()] &&
				( (!lin) || Singleton::Instance()->LinearResult->zone_data[i] < Singleton::Instance()->LinearResult->borders[Singleton::Instance()->LinearResult->borders.get_high()] ) &&
				( (!th) || Singleton::Instance()->ThResult->zone_data[i] >= Singleton::Instance()->ThResult->borders[Singleton::Instance()->ThResult->borders.get_high()]) )
	{
		SummaryResult::zone_data[i]=1;
	}
	else
		SummaryResult::zone_data[i]=2;

	zones++;

}
//---------------------------------------------------------------------------

