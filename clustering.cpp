//! @file clustering.cpp

 #include <vcl.h>
#pragma hdrstop

#include  <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "clustering.h"
#include "Protocol.h"
#include <IniFiles.hpp>
#include "Global.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace SolidGroups;

ClusterPoint::ClusterPoint()
{
	index = -1;
	coords.resize(0);
}

ClusterPoint::ClusterPoint( vector<double> coordinates )
{
	if ( coordinates.size() )
	{
		coords = coordinates;
	}

	index = -1;
}

void ClusterPoint::setCoordinates( vector<double> Coords )
{
	coords = Coords;
}

//----------------------------------------------------------------------------

const map< TubeType::Types, int> TubeType::tsToint = TubeType().init_tsToint();
const map< int, TubeType::Types> TubeType::intTots = TubeType().init_intTots();

TubeType::TubeType()
{
	tubetype = TubeType::tsUnknown;
}

TubeType::TubeType( TubeType::Types tubeType )
{
	tubetype = tubeType;
}

TubeType::TubeType( int tubeType )
{
	if( intTots.count( tubeType ) )
	{
		tubetype = intTots.find( tubeType )->second;
	}
	else
	{
		tubetype = TubeType::tsUnknown;
	}
}

TubeType::operator int()
{
	map<TubeType::Types, int>::const_iterator it = tsToint.find( tubetype );

	return ( it == tsToint.end() ) ? 0 : it->second;
}

TubeType::operator const int() const
{
	map<TubeType::Types, int>::const_iterator it = tsToint.find( tubetype );

	return ( it == tsToint.end() ) ? 0 : it->second;
}


//static const map<TubeSize::TypeSizes, int>& TubeSize::init_tsToint() const
const map<TubeType::Types, int> TubeType::init_tsToint()
{
	map<TubeType::Types, int> _map;

	_map[ TubeType::tsTube_60mm ] = 60;
	_map[ TubeType::tsTube_73mm ] = 73;
	_map[ TubeType::tsTube_89mm ] = 89;

	return _map;
}

const map<int, TubeType::Types> TubeType::init_intTots()
{
	map<int, TubeType::Types> _map;

	_map[ 60 ] = TubeType::tsTube_60mm;
	_map[ 73 ] = TubeType::tsTube_73mm;
	_map[ 89 ] = TubeType::tsTube_89mm;

	return _map;
}
TStringList* TubeType::getTubeTypeNames()
{
	TStringList* ret = new TStringList();
	map< TubeType::Types, int >::const_iterator it;
	for (it = tsToint.begin(); it != tsToint.end(); it++)
		ret->Add( IntToStr(it->second) );
	return ret;
}

//-----------------------------------------------------------------------------
//! инициализация всех карт по группе прочности
const map< Group::Types, String > Group::sgToStr = Group().init_sgToStr();
const map< String, Group::Types > Group::strToSg = Group().init_strToSg();
const map< Group::Types, String > Group::sgToEnglish = Group().init_sgToEnglish();
const map< Group::Types, int>   Group::sgToNumber = Group().init_sgToNumber();

Group::Group()
{
	group = sgUnknown;
}

Group::Group( Group::Types solidGroup )
{
 	group = solidGroup;
}

Group::Group( String solidGroup )
{
	if( strToSg.count( solidGroup ) )
	{
		group = strToSg.find( solidGroup )->second;
	}
	else
	{
		group = Group::sgUnknown;
	}
}
//! Инициализация карты "Тип группы - строка"
const map< Group::Types, String > Group::init_sgToStr()
{
	map< Group::Types, String > _map;

	_map[ Group::sgD ] = String( L"D" );
	_map[ Group::sgK ] = String( L"K" );
	_map[ Group::sgE ] = String( L"E" );
	_map[ Group::sgL ] = String( L"L" );
	_map[ Group::sgM ] = String( L"M" );
	_map[ Group::sgK72 ] = String( L"K72" );
	_map[ Group::sgN80 ] = String( L"N80" );
	_map[ Group::sgL80 ] = String( L"L80" );
	_map[ Group::sgP110 ] = String( L"P110" );

	return _map;
}

//! Инициализация карты "Строка - Тип группы"
const map< String, Group::Types > Group::init_strToSg()
{
	map< String, Group::Types > _map;

	_map[  String( L"D" ) ] =  Group::sgD;
	_map[  String( L"K" ) ] =  Group::sgK;
	_map[  String( L"E" ) ] =  Group::sgE;
	_map[  String( L"L" ) ] =  Group::sgL;
	_map[  String( L"M" ) ] =  Group::sgM;
	_map[  String( L"K72" ) ] =  Group::sgK72;
	_map[  String( L"N80" ) ] =  Group::sgN80;
	_map[  String( L"L80" ) ] =  Group::sgL80;
	_map[  String( L"P110" ) ] =  Group::sgP110;

	return _map;
}

//! Старая карта - "Тип группы - символ". Для сохранения в БД Пионерного.
//! Вряд ли стоит ее расширять до 9 групп прочности
const map< Group::Types, String > Group::init_sgToEnglish()
{
	map< Group::Types, String > _map;

	_map[ Group::sgD ] = String( L"D" );
	_map[ Group::sgK ] = String( L"K" );
	_map[ Group::sgE ] = String( L"E" );
	_map[ Group::sgL ] = String( L"L" );
	_map[ Group::sgM ] = String( L"M" );
	_map[ Group::sgK72 ] = String( L"K72" );
	_map[ Group::sgN80 ] = String( L"N80" );
	_map[ Group::sgL80 ] = String( L"L80" );
	_map[ Group::sgP110 ] = String( L"P110" );

	return _map;
}

//! Инициализация карты "Тип группы - Цифра". Как раз для Таскина.
//! Он хранит все группы в базе по цифрам, ДКЕ почему-то 2,4,5 (Task-style)
//! Договоришься с ним по номерам и соответствию групп номеру
const map< Group::Types, int > Group::init_sgToNumber()
{
	map <Group::Types, int> _map;
	_map [Group::sgUnknown] = 0;
	_map [Group::sgD] = 2;
	_map [Group::sgK] = 4;
	_map [Group::sgE] = 5;
	_map [Group::sgL] = 6;
	_map [Group::sgM] = 7;
	_map [Group::sgK72] = 8;
	_map [Group::sgN80] = 9;
	_map [Group::sgL80] = 10;
	_map [Group::sgP110] = 11;

	return _map;
}

Group::operator String()
{
	map< Group::Types, String >::const_iterator it = sgToStr.find( group );

	return ( it == sgToStr.end() ) ? String( L"-" ) :
									 it->second;
}

Group::operator const String() const
{
	map< Group::Types, String >::const_iterator it = sgToStr.find( group );

	return ( it == sgToStr.end() ) ? String( L"-" ) :
									 it->second;
}

const String Group::GroupToEnglish() const
{
	map< Group::Types, String >::const_iterator it = sgToEnglish.find( group );

	return ( it == sgToEnglish.end() ) ? String( L"-" ) :
									 it->second;
}

const int Group::GroupToNumber() const
{
	map <Group::Types, int>::const_iterator it = sgToNumber.find( group );
	return ( it == sgToNumber.end() ) ? (-1) : (it->second);
}

TStringList* Group::getGroupNames()
{
	TStringList* ret = new TStringList();
	map< Group::Types, String >::const_iterator it;
	for (it = sgToStr.begin(); it != sgToStr.end(); it++)
		ret->Add(it->second);
	return ret;
}

//-----------------------------------------------------------------------------

Tube::Tube() : ClusterPoint()
{
//	typesize   = tsUnknown;
//	solidgroup = sgUnknown;
}

Tube::Tube( vector<double> coordinates ) : ClusterPoint( coordinates )
{
	//typesize   = tsUnknown;
//	solidgroup = sgUnknown;
}

Tube::Tube( vector<double> coordinates, TubeType tubeType, Group solidGroup )
	: ClusterPoint( coordinates )
{
	tubetype   = tubeType;
	solidgroup = solidGroup;
}

Tube::Tube(  vector<double> sourceSignals, TubeType tubeType )
{
	tubetype   = tubeType;
	coords     = coordsFromSignal( sourceSignals );
}
void Tube::setTemperature(int temp)
{
	temperature = temp;
}
void Tube::setSolidGroup( Group solidGroup )
{
	solidgroup = solidGroup;
}

//------------------------------------------------------------------------------
// Считывает пороги группы прочности дефолтного типоразмера
void Tube::ReadThresholds()
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	this->thresholds.resize(0);
	double threshold = 0;
	for(int i = 0; i < 8; i++)
	{
		threshold = ini->ReadFloat("Type_"+Globals::current_typesize, "Threshold_"+String(i),2.5);
		this->thresholds.push_back(threshold);
	}
	delete ini;
}
//------------------------------------------------------------------------------
// Записывает пороги группы прочности в ini файл
void Tube::WriteThresholds()
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);

	for(unsigned i = 0; i < 8; i++)
		ini->WriteFloat("Type_"+Globals::current_typesize, "Threshold_"+String(i),thresholds[i]);

	delete ini;
}
//------------------------------------------------------------------------------
vector<double> Tube::coordsFromSignal( vector<double> sourceSignals)
{
	vector<double> _coords;
	vector<double> ret_coords;
	vector<int> quantity;
	parameters.resize(8);
	if(thresholds.size() == 0)
		ReadThresholds();

	const int threshold = 30;
	TPr::SendToProtocol("Размер массива по ГП: " + IntToStr((int)sourceSignals.size()));

	for( unsigned int step = 0; step < 8; ++step )
	{
		_coords.push_back( 0.0 );
		quantity.push_back(0);
	}

	// проверяем на принадлежность к только нужному (текущему) порогу
	unsigned int th = 0;

	//Чудо Урановского кастылестроения ( правит косяк Александра)
	//Не фига это не косяк, это фича, которую давно поправили, а кастыль остался!
	for (int start = 0; start < 2; start++ )
	{
		for( unsigned int pair = 0; pair < ( sourceSignals.size() / 2 - 1); ++pair )
		{
			double    magnetic = sourceSignals[ 2*pair + 0 + start],
					  current  = sourceSignals[ 2*pair + 1 + start],
					  next_cur = sourceSignals[ 2*(pair + 1) + 1 + start];

			if( current >= thresholds[th] && next_cur < thresholds[th] && magnetic > 0)    // фильтр!
			{		// здесь все правильно!! работает! не изменять - руки оторву!
					// в условии можно менять только ЗНАК у magnetiс - если надо считывать нижнюю полуплоскоть сигнала
				parameters[th].push_back(magnetic);
				th++;
			}
			if (th == 8)
				th = 0;
		}
		if (parameters[0].size() > 2 && parameters[1].size() > 2)
			break;
	}

	// выделяем из выбранных значений только нужные - отсекаем 1/8 с начала и с конца
	int trash = parameters[0].size() / 8;
	TPr::SendToProtocol("Размер параметров по ГП: " + IntToStr((int)parameters[0].size()));
	TPr::SendToProtocol("Размер отсеченного: " + IntToStr((int)trash));
	if (parameters[0].size())
	{
		for (unsigned int i = 0; i < parameters.size(); i++)
			for (unsigned int j = trash; j < parameters[i].size()-trash; j++)
			{
				_coords[i] +=  parameters[i][j];
				quantity[i]++;
			}

		for (int i = 0 ; i < 8; i++)
		{
			ret_coords.push_back ((double ) (_coords[i] / quantity[i]));
		}
//		if(SystemConst::isTempSensor)
//		{
//			TIniFile *ini = new TIniFile(Globals::IniFileName);
//			int numberParameterTemperature = ini->ReadInteger("OtherSettings", "numberParameter", 4);
//			delete ini;
//
//			ret_coords[numberParameterTemperature] = temperature;
//		}
	}
	else
		for (int i = 0 ; i < 8; i++)
			ret_coords.push_back ((double ) 1.5);
	return ret_coords;
}
//------------------------------------------------------------------------------
void Tube::setTubeType( TubeType::Types tubeType )
{
	tubetype = TubeType( tubeType );
}
//------------------------------------------------------------------------------
vector<double> Tube::getParameters(int par)
{
	if (parameters.size() == dimension() && parameters.size() > 0)
	{
		return parameters[par];
	}
	else
	{
		vector<double> t;
		return t;
	}
}
//------------------------------------------------------------------------------
vector<double> Tube::getThresholds()
{
  //	if (thresholds.size() == dimension() && thresholds.size() > 0)
  if ( thresholds.size() == 8 )
		return thresholds;
	else
	{
		vector<double> t;
		return t;
	}
}

//------------------------------------------------------------------------------
//-----------функции для определения ГП, с помощью MS-DOS,овской проги----------
//------------------------------------------------------------------------------

String Tube::calcSolidGroup(vector<double> source_signal)
{
	using namespace std;

	int result = 0;    //1 = E, 0 = K, -1 = D
	int nn = 0;
	double buffer[3][3];         //сделал нумерацию с 1(ну вот просто захотелось и все)

	//соберем необходимую инфу

	ReadThresholdFromFile();
	for(int step = 0; step < sg.size(); ++step)
	{
		String res;
		sg[step].result = CalcCorrelation(sg[step], source_signal);
		//вот это КЫСТЫЛИЩЕ был создан чтобы выводить очень большие дробные числа в Memo.
		//Если делать все по человечески то падает...надо подумать на досуге как это поправить
		if( sg[step].result != 1.0 )
			res = UnicodeString(sg[step].result).SubString(0,8);
		else
		{
			sg[step].result *=1.0;
			res = UnicodeString(L"1.00000000");
		}
		String tr = UnicodeString(sg[step].threshold).SubString(0,7);
		TPr::SendToProtocol("Порог " + tr+" корреляция "+res);

		if( sg[step].result > sg[step].threshold )
		{
			result = (result) | (int)pow(2.0,sg[step].group);
			if(nn < 2)
			{
				nn++;
				buffer[nn][1] = sg[step].result;
				buffer[nn][2] = sg[step].group;
			}
		}
	}
	if(result == 3 )
		if( buffer[1][2] != buffer[2][2] )
		{
			if( (Absolute( buffer[1][1] - buffer[2][1] )) < 0.00009 )
				result = 2;
			else
				result = 1;
		}
	result--;
	//строчка по сути бессмысленна, сделана лишь для того что бы легче было проверять соответствие с MS-DOS прогой
	//хотел сделать по крутому через карту, но не допетрил как надо...поэтому switch
	switch(result)
	{
		case -1:return "D";
				break;
		case 0: return "K";
				break;
		case 1: return "E";
				break;
	}
	return "D";
}
//------------------------------------------------------------------------------
double Tube::CalcCorrelation(solid_group sg, std::vector<double> median)
{
	using namespace std;
	int n=0; //кол-во элементов которые учитываются при рассчете
	for(int i=0; i < sg.mask.size(); i++)
		if(sg.mask[i] == 1)
			n++;

	double tmp, xy, ex, ey, ex2, ey2,R = 0;

	for(int i=0; i < 8; i++)
	if (sg.mask[i] == 1)
	{
		try
		{
			tmp = median[i] * double(sg.data[i]);
			xy 	+= tmp;
			ex  += median[i];
			ey  += double(sg.data[i]);
			ex2 += (pow(median[i]*1.0,2.0)  );
			ey2 += (pow(sg.data[i]*1.0,2.0) );
		}
		catch (...)
		{}
	}

	try
	{
		R = xy - ((ex * ey) / n);
		R = R / (sqrt(ex2 - (pow(ex,2.0)/n)) );
		R = R / (sqrt(ey2 - (pow(ey,2.0)/n)) );
	}
	catch (...)
	{}

	return R;
}

//------------------------------------------------------------------------------
double Tube::Absolute(double value)
{
	if ( value < 0 )
		return value * (-1);
	else
		return value;

}
//------------------------------------------------------------------------------
void Tube::ReadThresholdFromFile()
{
	String filepath = Globals::PathFile_threshold_sg;
	filepath+=String(Globals::current_diameter);
	filepath+=".txt";

	FILE *file;
	file = fopen(AnsiString(filepath).c_str(), "r");
	if (file == NULL)
	{
		Application->MessageBoxW(L"Не удалось открыть файл для рассчета ГП",L"Ошибка", MB_ICONERROR | MB_OK  );
		return;
	}
	else
	{
		int q;//кол-во вариаций ГП в файле
		fscanf(file,"%d",&q);
		sg.resize(q);
		if (q == 0)
			Application->MessageBoxW(L"Нет записей в файле с порогами ГП",L"Ошибка", MB_ICONERROR | MB_OK  );
		else
		{
			for(int i = 0; i < q; i++)
			{
				sg[i].mask.resize(8);
				sg[i].data.resize(8);
				for(int j=0; j < 8; j++)
				{
					fscanf(file,"%d",&sg[i].mask[j]);
					int mask = sg[i].mask[j];
					int adas=0;
				}
				for(int j=0; j < 8; j++)
					fscanf(file,"%d",&sg[i].data[j]);

				fscanf(file, "%lf", &sg[i].threshold);
				fscanf(file, "%d", &sg[i].group);
			}
		}
		fclose(file);
	}
}
//------------------------------------------------------------------------------

CMeansAlgorithm::CMeansAlgorithm()
{
	Eps       = 0.00001;
	accuracy  = 0.00001;
}

CMeansAlgorithm::CMeansAlgorithm( 	vector<Tube> Points,
									int clusterCount,
									double Fuzzy = 2.0
									 )
{
	points    = Points;
	fuzzyness = Fuzzy;
	Eps       = 0.00001;
	accuracy  = 0.00001;

	U.resize( clusterCount );
	clusters.resize( clusterCount );

    srand ( time(NULL) );


	// формируем матрицу U
	for(int i = 0; i < clusterCount; i++)
	{
		U[i].resize( points.size() );

		// Центры кластеров в идеале выбираются случайно из Points
		// на деле же пока возьмём первые <clusterCount> точек
		// int randIndex = rand() % points.size();

		clusters[i] = points[ i ];
	}


	for(unsigned int i = 0; i < points.size(); i++)
	{
		Tube p = points[i];
		double diff;
		double sum = 0.0;

		for(unsigned int j = 0; j < clusters.size(); j++)
		{
			Tube c = clusters[j];

			diff = calcEulerDistance( p, c );
			U[j][i] = (diff == 0.0) ? Eps : diff;
			sum += U[j][i];
		}

		double sum2 = 0.0;
		for(unsigned int j = 0; j < clusters.size(); j++)
		{
			U[j][i] = 1.0 / pow( U[j][i] / sum, 2.0 / (fuzzyness - 1.0));
			sum2 += U[j][i];
		}

		for(unsigned int j = 0; j < clusters.size(); j++)
		{
			U[j][i] = U[j][i] / sum2;
		}

	}

/*
	//! @todo удалить после завершения отладки
	log->Lines->Add( String("Матрица U после инициализации:") );
	for(unsigned int j = 0; j < clusters.size(); j++)
	{
		String str;

		for(unsigned int i = 0; i < points.size(); i++)
		{
			str += String().sprintf( L"%7.4f   ", U[j][i] );
		}

		log->Lines->Add( str );
	}    */


	recalculateClusterIndexes();
}

void CMeansAlgorithm::recalculateClusterIndexes()
{
	// для каждой точки множества
	for( unsigned int i = 0; i < points.size(); i++ )
	{
		double max = -1.0;

		// для каждого из рассматриваемых кластеров
		for(unsigned int j = 0; j < clusters.size(); j++ )
		{
			// выбираем наибольшее значение степени принадлежности
			// и обновляем индекс кластера
			if( max < U[j][i] )
			{
				max = U[j][i];
				// более очевидный points[i].setClusterIndex( j );
				// также работает. Пока нет ответа на вопрос,
				// в каких случаях индекс становится равным 0.5
				points[i].setClusterIndex( (max == 0.5) ? 0.5 : j );
			}
		}
	}
/*
	//! @todo почистить после завершения отладки
	String str;
	for( unsigned int i = 0; i < points.size(); i++ )
	{
		str += String().sprintf( L"%7.2f  ", points[i].clusterIndex() );
	}
	log->Lines->Add( str );
*/
}

double CMeansAlgorithm::calcEulerDistance( 	Tube point, Tube centroid )
{
	/*! Евклидово расстояние  между точками вычисляется по формуле
	   \f$ z = \|x_k - y_k\| =  \sqrt{\sum\limits_{k=1}^n (x_k - y_k)^2 } \f$ */
	double sum = 0.0;
	int dim = point.dimension();

	vector<double> Scale (dim, 1.0);  //коэффициенты //20160329
	if(dim > 8)	Scale[8] = 1000.0; //Si
	if(dim > 9) Scale[9] = 100.0; //Mn
	if(dim > 10) Scale[10] = 10000.0; //Cr
	if(dim > 11) Scale[11] = 100000.0; //Ni
	if(dim > 12) Scale[12] = 1.0; //Mo
	for ( unsigned int i = 0; i < dim; i++ )
	{
		double diff = point.Coordinates()[i] - centroid.Coordinates()[i];
		/*if(i > 7)*/ diff = diff*Scale[i]; //20160329
		sum += diff * diff;
	}

	return sqrt(sum);
}

double CMeansAlgorithm::calcObjectiveFunction()
{
	/*! Значение целевой функции J определяется по формуле
		\f$ J = \sum\limits_{i=1}^p \sum\limits_{j=1}^c (\mu_{ij})^m
		\|x_i - v_j\|^2 \f$. Здесь \f$\mu_{ij}\f$ - степень принадлежности
		объекта \f$x\f$ к кластеру с центром \f$v\f$. */

	double Jk = 0.0;

	for( unsigned int i = 0; i < points.size(); i++ )
	{
		for( unsigned int j = 0; j < clusters.size(); j++ )
		{
			double dist = calcEulerDistance( points[i], clusters[j] );
			Jk += pow( U[j][i], fuzzyness ) * dist * dist;
		}
	}

	return Jk;
}

void CMeansAlgorithm::calcClusterCenters()
{
	/*! Определение координат кластерных центров осуществляется при помощи
		следующего соотношения
		\f$ V_i = \frac{1}{\sum\limits_{j=1}^c \mu_{ij}} \sum\limits_{j=1}^c \mu_{ij}^m x_j \f$ */
	for( unsigned int j = 0; j < clusters.size(); j++ )
	{
		Tube c = clusters[j];
		double l = 0.0;
		vector<double> uc( c.dimension() );

		for( unsigned int i = 0; i < points.size(); i++ )
		{
			Tube p = points[i];

			double uu = pow( U[j][i], fuzzyness );

			for( unsigned int k = 0; k < c.dimension(); k++ )
			{
				uc[k] += uu * p.Coordinates()[k];
			}

			l += uu;

		}

		for( unsigned int k = 0; k < c.dimension(); k++ )
		{
			uc[k] /= l;
		}
		clusters[j].setCoordinates( uc );
	}
/*
	//! @todo удалить после завершения отладки
	log->Lines->Add( String("Координаты центров кластеров:") );

	for( unsigned int j = 0; j < clusters.size(); j++ )
	{
		String str;
		str += String("Центр кластера № ") + String(j) + String(":    ");

		for( unsigned int k = 0; k < clusters[j].dimension(); k++ )
			//str += UnicodeString( clusters[j].Coordinates()[k] ) + UnicodeString(" ");
			str += String().sprintf( L"%7.3f   ", clusters[j].Coordinates()[k] );

		log->Lines->Add( str );
	}
*/
}

void CMeansAlgorithm::doIteration()
{

	for(unsigned int c = 0; c < clusters.size(); c++)
	{
		for(unsigned int h = 0; h < points.size(); h++)
		{
			double top = calcEulerDistance( points[h], clusters[c] );

			if(top < 1.0)
				top = Eps;

			double sumTerms = 0.0;
			for(unsigned int ck = 0; ck < clusters.size(); ck++ )
			{
				double thisDistance = calcEulerDistance( points[h], clusters[ck] );

				if( thisDistance < 1.0 )
					thisDistance = Eps;

				sumTerms += pow( top / thisDistance, 2.0 / (fuzzyness - 1.0));
			}

			U[c][h] = (double)(1.0 / sumTerms);
		}

	}
/*
	//! @todo удалить после завершения отладки
	log->Lines->Add( String("Матрица U:") );
	for(unsigned int j = 0; j < clusters.size(); j++)
	{
		UnicodeString str;

		for(unsigned int i = 0; i < points.size(); i++)
		{
			str += String().sprintf( L"%7.4f   ", U[j][i] );
		}

		log->Lines->Add( str );
	}

    log->Lines->Add( String("Индексы принадлежности к кластерам") );
 */

	recalculateClusterIndexes();

}


void CMeansAlgorithm::run()
{
	int i = 0;
	int maxIterations = 200;

//	log->Lines->Add( String("Расчёт запущен") );

	do
	{
		i++;

 //		log->Lines->Add( String("-----------------------------------") );
 //		log->Lines->Add( String("Итерация № ") + UnicodeString(i) );

		J = calcObjectiveFunction();
		calcClusterCenters();
		doIteration();
		double Jnew = calcObjectiveFunction();

		if( fabsl( double(J - Jnew) ) < 0.00001 )
			break;

	} while( i < maxIterations );
}

//-----------------------------------------------------------------------------

Analyzer::Analyzer()
{

}

void Analyzer::initEtalons( vector<Tube> Etalons )
{
	etalons = Etalons;

	calcEtalonCenters();
}

void Analyzer::calcEtalonCenters()
{
	algo = CMeansAlgorithm( etalons , 3, 2.0 );
	algo.run();

	etaloncenters = algo.Clusters();
	etalons 	  = algo.Points();



	for( unsigned int j = 0; j < etaloncenters.size(); ++j )
	{
		//int centerIndex = etaloncenters[j].clusterIndex();

		for( unsigned int i = 0; i < etalons.size(); ++i )
		{
			//int pointIndex = etalons[i].clusterIndex();

			if( j == etalons[i].clusterIndex() )
			{
				etaloncenters[j].setSolidGroup( etalons[i].solidGroup() );
//				etaloncenters[j].setTypeSize( etalons[i].typeSize() ) ;
				etaloncenters[j].setTubeType( etalons[i].type() ) ;
				break;
			}
		}
	}
}

map< Group, double > Analyzer::checkTube( Tube tube )
{

	map<Group, double> probabilities;

	double bottom = 0.0;
	double Eps    = 1.0e-5;

	for( unsigned int i = 0; i < etaloncenters.size(); ++i )
	{
		double distance = algo.calcEulerDistance( tube, etaloncenters[i] );
		distance = (distance == 0.0) ? Eps : distance;
		bottom += 1.0 / pow( distance, algo.Fuzzyness() );
	}

	for( unsigned int i = 0; i < etaloncenters.size(); ++i )
	{
		double distance = algo.calcEulerDistance( tube, etaloncenters[i] );
		distance = (distance == 0.0) ? Eps : distance;
		double top =  1.0 / pow( distance, algo.Fuzzyness() );
		double prob = top / bottom;

//		probabilities.insert( pair<SolidGroup, double>( etaloncenters[i].solidGroup(), prob ) );
		Group sg = etaloncenters[i].solidGroup();

		probabilities[ sg ] = prob;
//		probabilities.insert( pair<Group, double>( sg, prob ) );
	}

	return probabilities;
}
