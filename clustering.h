/*! @file clustering.h
 *  @author
 *	@date 21.09.2001
 *	@version 			*/

#ifndef clusteringH
#define clusteringH

#include <vector>
#include <map>

using namespace std;

namespace SolidGroups
{

struct solid_group
{
	std::vector <int> mask;
	std::vector <int> data;
	double threshold;
	int group;
	double result;
};

/*! @brief Класс объектов кластерного множества */
class ClusterPoint
{
public:
	//! Конструктор по умолчанию
	ClusterPoint();
	/*! @brief Явный конструктор
		@param coordinates Вектор координат */
	ClusterPoint( vector<double> coordinates );

	//! Считывание координат объекта
	const vector<double>& Coordinates() const { return coords; };
	//! Установка координат объекта
	void setCoordinates( vector<double> Coords );

	//! Установка индекса кластера, к которому принадлежит объект
	void setClusterIndex( double Index ) { index = Index; }
	//! Считывание индекса кластера, к которому принадлежит объект
	const double& clusterIndex() const {return index; };

	//! @brief Размерность множества, к которому принадлежит объект
	 const unsigned int& dimension() const { return coords.size(); };

protected:
	//! Координаты объекта
	vector<double> coords;
	//! Индекс кластера, к которому принадлежит объект
	double index;
};

//-----------------------------------------------------------------------------

class TubeType
{
public:
	enum Types   { tsUnknown, tsTube_60mm, tsTube_73mm, tsTube_89mm };

	TubeType();
	TubeType( TubeType::Types typeSize );
	TubeType( int typeSize );

	const TubeType::Types& value() const { return tubetype; }

	operator int();
	operator const int() const;
	TStringList* getTubeTypeNames();

	const map< TubeType::Types, int > init_tsToint();
	const map<int, TubeType::Types  > init_intTots();

private:
    TubeType::Types tubetype;

	const static map< TubeType::Types, int > tsToint;
	const static map< int, TubeType::Types > intTots;

};

//-----------------------------------------------------------------------------

class Group
{
public:
	enum Types { sgUnknown=0, sgD, sgK, sgE, sgL, sgM, sgK72, sgN80, sgL80, sgP110 };

	Group();
	Group( Group::Types solidGroup );
	Group( String solidGroup );

	const Group::Types& value() const { return group; }

	const map< Group::Types, String > init_sgToStr();
	const map< String, Group::Types > init_strToSg();
	const map< Group::Types, String > init_sgToEnglish();
	const map< Group::Types, int> init_sgToNumber();

	operator String();
	operator const String() const;

	const String GroupToEnglish() const;
	const int GroupToNumber() const;
	TStringList* getGroupNames();

	inline bool operator < ( const Group& sg ) const {return (group <  sg.group);}

private:
	Group::Types group;

	const static map< Group::Types, String > sgToStr;
	const static map< String, Group::Types > strToSg;
	const static map< Group::Types, String > sgToEnglish;
	const static map< Group::Types, int> sgToNumber;

};


//-----------------------------------------------------------------------------

class Tube : public ClusterPoint
{
 public:

	Tube();
	Tube( vector<double> coordinates );
	Tube( vector<double> coordinates, TubeType tubeType, Group solidGroup );
	Tube( vector<double>    sourceSignals, TubeType tubeType );

	const TubeType& 	type()   	   const  { return tubetype; }
	const Group&        solidGroup()   const  { return solidgroup; }

   //	Tube& operator = (const Tube& _tube);

	void setTubeType  ( TubeType tubeType ) { tubetype = tubeType; }
	void setTubeType  ( TubeType::Types tubeType );

	void setSolidGroup( Group solidGroup );

	vector<double> coordsFromSignal( vector<double> sourceSignals );
	//! возвращает массив магнитных сигналов по конкретному порогу
	vector<double> getParameters(int par);
	//! возвращает вектор пороговых значений
	vector<double> getThresholds();
	//! устанавливает пороги по гп
	void setThresholds(vector<double> _thresholds) { thresholds = _thresholds; }
	//! Считывает порогии группы прочности дефолтного типоразмера
	void ReadThresholds();
	//! Записывает пороги группы прочности в ini файл
	void WriteThresholds();
	void setTemperature(int temp);
	//! Считает группу прочности по новой схеме, с использованием карреляции,
	//! для вызова это функции необходим файл диаметр.txt (свой для каждого типоразмера) в папке settings программы
	//! вернет букву D,K,E.
	String calcSolidGroup(vector<double> source_signal);
private:
	static int temperature;
//------------------------------------------------------------------------------
	//! вычисляет коэффициент карреляции
	double CalcCorrelation( solid_group sg, vector<double> median);
	//! т.к. ABS работает только с int (или я не нашел для double, нужное подчеркнуть), то пришлось сделать свою.
	double Absolute(double value);
	//! считывает пороги по гп, из файла
	void ReadThresholdFromFile();
	//! вектор куда будут сохраняться данные из файла с порогами, маской и тд.
	std::vector<solid_group> sg;
//------------------------------------------------------------------------------
	//TypeSize 	typesize;
	TubeType tubetype;
	Group  	 solidgroup;
	//! вектор из 8 векторов, характеризующих все точки данного порога магнитного сигнала
	vector< vector<double> > parameters;
	//! вектор из 8 пороговых значений
	vector<double> thresholds;
};
int SolidGroups::Tube::temperature = 0;
//-----------------------------------------------------------------------------

/*! @brief Реализация нечёткого алгоритма кластеризации c-means */
class CMeansAlgorithm
{
public:
	/*! @brief Конструктор
	 *  @param Points Множество точек, которое предстоит разбить на кластеры
	 *  @param clusterCount Количество кластеров
	 *  @param Fuzzy Экспоненциальный вес */
	CMeansAlgorithm(	vector<Tube> Points,
						int clusterCount,
						double Fuzzy
				   );
	CMeansAlgorithm();

	void		doIteration();
	void	   	run();

	vector<Tube> Points()   { return points; }
	vector<Tube> Clusters() { return clusters; }

   	/*! @brief Вычисление Эйлерова расстояния между двумя
		   точками кластерного множества
	@param point первая точка
	@param centroid вторая точка */
	double  calcEulerDistance(Tube point, Tube centroid);

	double  Fuzzyness() { return fuzzyness; }

private:

	/*! @brief Пересмотр значений кластерных индексов
			   для каждого объекта из множества */
	void 	recalculateClusterIndexes();


    /*! @brief Вычисление значения целевой функции */
	double  calcObjectiveFunction();

	/*! @brief Вычисление координат кластерных центров */
	void	calcClusterCenters();

	/*! @brief Точность выполнения расчёта. Является параметром остановки
	 *  алгоритма. Чем меньше значение @b accuracy, тем большее количество
	 *  итераций потребуется выполнить для завершения расчёта */
	double 						accuracy;
	/*! @brief Координаты точек множества, разбиваемого на кластеры */
	vector<Tube> 		points;
	/*! @brief Координаты кластерных центров */
	vector<Tube> 		clusters;
	/*! @brief Матрица нечёткого разделения */
	vector< vector<double> >	U;
	/*! @brief Экспоненциальный вес.
	 *  Влияет на значение степеней принадлежности, с его увеличением элемент
	 *  будет принадлежать к кластерам с почти одинаковой степенью
	 *  принадлежности. Не существует теоретически обоснованного правила выбора
	 *  значения данного параметра, поэтому обычно его принимают равным 2.0.  */
	double						fuzzyness;
	double						Eps;
	double 						J;
};

class Analyzer
{
public:

	Analyzer();

	void initEtalons( vector<Tube> Etalons );

	vector<Tube> Etalons() {return etalons;}
	vector<Tube> etalonCenters() {return etaloncenters;}
	//vector<double> checkTube( const SolidGroupTube& tube );
	map< Group, double > checkTube( Tube tube );

private:

	void calcEtalonCenters ();

	vector<Tube> etalons;
	vector<Tube> etaloncenters;
	CMeansAlgorithm algo;
};


};


#endif
