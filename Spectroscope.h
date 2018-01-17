//---------------------------------------------------------------------------

#ifndef SpectroscopeH
#define SpectroscopeH
#include <classes.hpp>
#include <IniFiles.hpp>
#include <ADODB.hpp>
#include "uCCOMPort.h"
#include "clustering.h"
#include <map>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class Spectroscope
{
	public:
		Spectroscope();
		~Spectroscope(){if(cp)delete cp;}
		bool Init(TIniFile *ini);
		bool Start();
		void Stop();
		const UnicodeString getName();
		map<string,double> getSpectroData();
		bool isDataReady();
		void AddSpectroDataToSGTube(SolidGroups::Tube *tubeSG);
		static vector<double> AddSpectroDataToSGEtalon(int tubeId,TADOConnection *conn);
		static void WriteSpectroDataToDB(vector<double> coords,TADOConnection *conn);
		static void initElemIndexes();
	private:
		CCOMPort *cp;
		//���������� ������ ��� ������ ������
		HANDLE hThread;
		//���� ���������� ���� �������� �� ����� �������� ��������� �
		// ���� �������� hWndForMsg
		HWND hWndForMsg;
		static unsigned long __stdcall Execute(void *params);
		bool dataReady;
		map<string,double> himsostav;
		bool running;
		UnicodeString name;
		int BaudRate;
		int ByteSize;
		int Parity;
		int StopBits;
		int ReadIntervalTimeout;
		int ReadTotalTimeoutConstant;
		int ReadTotalTimeoutMultiplier;
		bool terminate;
		//������� ������������ ����� ���. ��������� � �������� � �������
		//��� ����������� ������ ���������
		static map<string,int> elemIndexes;
		void pr(AnsiString str);
};
#endif
