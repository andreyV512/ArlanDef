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

/*! @brief ����� �������� ����������� ��������� */
class ClusterPoint
{
public:
	//! ����������� �� ���������
	ClusterPoint();
	/*! @brief ����� �����������
		@param coordinates ������ ��������� */
	ClusterPoint( vector<double> coordinates );

	//! ���������� ��������� �������
	const vector<double>& Coordinates() const { return coords; };
	//! ��������� ��������� �������
	void setCoordinates( vector<double> Coords );

	//! ��������� ������� ��������, � �������� ����������� ������
	void setClusterIndex( double Index ) { index = Index; }
	//! ���������� ������� ��������, � �������� ����������� ������
	const double& clusterIndex() const {return index; };

	//! @brief ����������� ���������, � �������� ����������� ������
	 const unsigned int& dimension() const { return coords.size(); };

protected:
	//! ���������� �������
	vector<double> coords;
	//! ������ ��������, � �������� ����������� ������
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
	//! ���������� ������ ��������� �������� �� ����������� ������
	vector<double> getParameters(int par);
	//! ���������� ������ ��������� ��������
	vector<double> getThresholds();
	//! ������������� ������ �� ��
	void setThresholds(vector<double> _thresholds) { thresholds = _thresholds; }
	//! ��������� ������� ������ ��������� ���������� �����������
	void ReadThresholds();
	//! ���������� ������ ������ ��������� � ini ����
	void WriteThresholds();
	void setTemperature(int temp);
	//! ������� ������ ��������� �� ����� �����, � �������������� ����������,
	//! ��� ������ ��� ������� ��������� ���� �������.txt (���� ��� ������� �����������) � ����� settings ���������
	//! ������ ����� D,K,E.
	String calcSolidGroup(vector<double> source_signal);
private:
	static int temperature;
//------------------------------------------------------------------------------
	//! ��������� ����������� ����������
	double CalcCorrelation( solid_group sg, vector<double> median);
	//! �.�. ABS �������� ������ � int (��� � �� ����� ��� double, ������ �����������), �� �������� ������� ����.
	double Absolute(double value);
	//! ��������� ������ �� ��, �� �����
	void ReadThresholdFromFile();
	//! ������ ���� ����� ����������� ������ �� ����� � ��������, ������ � ��.
	std::vector<solid_group> sg;
//------------------------------------------------------------------------------
	//TypeSize 	typesize;
	TubeType tubetype;
	Group  	 solidgroup;
	//! ������ �� 8 ��������, ��������������� ��� ����� ������� ������ ���������� �������
	vector< vector<double> > parameters;
	//! ������ �� 8 ��������� ��������
	vector<double> thresholds;
};
int SolidGroups::Tube::temperature = 0;
//-----------------------------------------------------------------------------

/*! @brief ���������� ��������� ��������� ������������� c-means */
class CMeansAlgorithm
{
public:
	/*! @brief �����������
	 *  @param Points ��������� �����, ������� ��������� ������� �� ��������
	 *  @param clusterCount ���������� ���������
	 *  @param Fuzzy ���������������� ��� */
	CMeansAlgorithm(	vector<Tube> Points,
						int clusterCount,
						double Fuzzy
				   );
	CMeansAlgorithm();

	void		doIteration();
	void	   	run();

	vector<Tube> Points()   { return points; }
	vector<Tube> Clusters() { return clusters; }

   	/*! @brief ���������� �������� ���������� ����� �����
		   ������� ����������� ���������
	@param point ������ �����
	@param centroid ������ ����� */
	double  calcEulerDistance(Tube point, Tube centroid);

	double  Fuzzyness() { return fuzzyness; }

private:

	/*! @brief ��������� �������� ���������� ��������
			   ��� ������� ������� �� ��������� */
	void 	recalculateClusterIndexes();


    /*! @brief ���������� �������� ������� ������� */
	double  calcObjectiveFunction();

	/*! @brief ���������� ��������� ���������� ������� */
	void	calcClusterCenters();

	/*! @brief �������� ���������� �������. �������� ���������� ���������
	 *  ���������. ��� ������ �������� @b accuracy, ��� ������� ����������
	 *  �������� ����������� ��������� ��� ���������� ������� */
	double 						accuracy;
	/*! @brief ���������� ����� ���������, ������������ �� �������� */
	vector<Tube> 		points;
	/*! @brief ���������� ���������� ������� */
	vector<Tube> 		clusters;
	/*! @brief ������� ��������� ���������� */
	vector< vector<double> >	U;
	/*! @brief ���������������� ���.
	 *  ������ �� �������� �������� ��������������, � ��� ����������� �������
	 *  ����� ������������ � ��������� � ����� ���������� ��������
	 *  ��������������. �� ���������� ������������ ������������� ������� ������
	 *  �������� ������� ���������, ������� ������ ��� ��������� ������ 2.0.  */
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
