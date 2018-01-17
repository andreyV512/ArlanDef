#pragma hdrstop
#include "Spectroscope.h"
#include "DataModule.h"
#include "Protocol.h"
#pragma package(smart_init)

map<string,int> Spectroscope::elemIndexes;
//---------------------------------------------------------------------------
Spectroscope::Spectroscope()
{
		hWndForMsg = NULL;
		terminate = false;
		cp = NULL;
}
//---------------------------------------------------------------------------
bool Spectroscope::Init(TIniFile *ini)
{
	name = ini->ReadString("Spectroscope","SerialPortName",NULL);
	BaudRate = ini->ReadInteger("Spectroscope","SerialBaudRate",9600);
	ByteSize = 8;
	StopBits = ini->ReadInteger("Spectroscope","SerialStopBits",2);
	Parity = ini->ReadInteger("Spectroscope","SerialParity",0);
	ReadIntervalTimeout=100;
	ReadTotalTimeoutConstant=1000;
	ReadTotalTimeoutMultiplier=100;
/*
	if(cp)delete cp;
	cp = CCOMPort::Create(name,BaudRate,ByteSize,Parity,StopBits,
	   ReadIntervalTimeout,ReadTotalTimeoutConstant,ReadTotalTimeoutMultiplier);
	if(!cp)return false;
	cp->OnProtocol = pr;
*/
	return true;
}
//---------------------------------------------------------------------------
bool Spectroscope::Start()
{
	dataReady = false;
	if(!running)
	{
		terminate = false;
		//Execute((LPVOID)this);
		// ������� ����� ���������
		hThread = CreateThread(NULL,    	// default security attributes
				0,                      	// use default stack size
				Spectroscope::Execute,		// thread function name
				(LPVOID)this,          		// argument to thread function
				0,                      	// use default creation flags
				(unsigned long*)NULL);		// returns the thread identifier
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void Spectroscope::Stop()
{
	if(running)
	{
		terminate=true;
		while(running);
		CloseHandle(hThread);
	}
}
//---------------------------------------------------------------------------
unsigned long __stdcall Spectroscope::Execute(void *params)
{
	Spectroscope *s = (Spectroscope*)params;

	//���� ��� ���� ��������� ����������� ������, �� ������ ������ �� ����,
	//������ ������ ��������...
	if(s->cp)
	{
		//���� ����� ����� �� ������ ��������
		TPr::SendToProtocol("���� ��� ������������������" );
		return 0;
	}
	s->cp = CCOMPort::Create(s->name,s->BaudRate,s->ByteSize,s->Parity,s->StopBits,
		s->ReadIntervalTimeout,s->ReadTotalTimeoutConstant,s->ReadTotalTimeoutMultiplier);
	if(!s->cp)
	{
		TPr::SendToProtocol("�� ���������� ������� COM-���� � ������ �����������" );
		return 0;
	}
	s->cp->OnProtocol = s->pr;
    s->running = true;
	unsigned char  buf[1000];
	static TCHAR dbgStr[256];
	OutputDebugString(L"Spectro thread started...");
	s->dataReady = false;
	while(1)
	{
		if(s->terminate)break;
		unsigned char startByte;
		int cnt = s->cp->read_s(&startByte,1);
		if(cnt == 1 & startByte==5)
		{
			OutputDebugString(_T("�������� ���� ������ �������� ������..."));
			bool dataIsValid = false;
			OutputDebugString(_T("�������� ���� ���������� � �����..."));
			unsigned char ans=6;
			s->cp->write_s(&ans,1);
			ZeroMemory(buf,sizeof(buf));
			unsigned char *pp = buf;
			while((cnt = s->cp->read_s(&ans,1))==1 && ans !=3)
			{
				*pp++ = ans;
			}
			OutputDebugString(L"�������� ������...");
			OutputDebugString(UnicodeString(AnsiString((char*)buf)).c_str());
			//�� ���� ������ � ��� �������� ������ �� ������������.
			//���� � ���������
			//����������� ����� - 4 ��������� �����
			char tmp[30],val[11];
			memcpy(tmp,pp-4,4);tmp[4]=0;
			//������� ����������� �����
			int calcedCrc = 0;
			for(unsigned char *p=buf+1; p < pp-4; p++) calcedCrc += *p;
			calcedCrc = calcedCrc % 10000;
			wsprintf(dbgStr,L"%d - %s",calcedCrc,UnicodeString(AnsiString((char*)tmp)).c_str());
			OutputDebugString(dbgStr);
			if(pp>buf && calcedCrc==atoi(tmp))
			{
				//�������� ������������ �����, ��� ������ ������� ���������
				ans = 6;
				s->cp->write_s(&ans,1);
				//������ ����� � �������
				s->himsostav.clear();
				//������ ���������� ������
				memcpy(tmp,buf+162,3);
				tmp[3]=0;
				wsprintf(dbgStr,L"���������� ��������� � ������:%s",UnicodeString(AnsiString((char*)tmp)).c_str());
				OutputDebugString(dbgStr);
				int cntElems = atoi(tmp);
				wsprintf(dbgStr,L"%s",UnicodeString(AnsiString((char*)(buf+165))).c_str());
				OutputDebugString(dbgStr);
				for(int i = 0; i < cntElems; i++)
				{
					//������ ������� � ��������
					//18 �������� �� ���� �������
					memcpy(tmp,buf+165+i*18,8);
					tmp[7]=0;
					{
						char *p;
						for(p=tmp;!isspace(*p);p++);
						*p=0;
					}
					memcpy(val,buf+165+i*18+8,10);
					val[10]=0;
					for(int i = 0; i<10; i++)
					{
						if(val[i]==0x20)val[i]=0x30;
					}
					float dv;
					sscanf(val,"%f",&dv);
					s->himsostav.insert(pair<string,double>(string(tmp),(double)dv));
				}
				//������ ��������� ���������� �������, ��� ����� ��������
				s->dataReady = true;
				s->terminate=true;
			}
			else
			{
			   //�� ������� ����������� �����
			   ans = 21;
			   s->cp->write_s(&ans,1);
			   //����� ���-�� ���� �������� �� ������
			   OutputDebugString(L"������: �� ������� ����������� �����...");
			   s->dataReady = false;
			}
		}
/*
		else
		{
			   OutputDebugString(L"������: �����...");
		}
*/
	}
	delete s->cp;
	s->cp = NULL;
	OutputDebugString(L"Spectro thread finished...");
	s->running = false;
	return 0;
}
//---------------------------------------------------------------------------
void Spectroscope::initElemIndexes()
{
	TCHAR buf[256];
	TADODataSet *ds = new TADODataSet(NULL);
	ds->Connection = DM->ADOCon;;
	ds->CommandText = "select max(ind) as maxInd from ChemIndexes";
	ds->Open();
	ds->First();
	int maxInd = ds->FieldByName("maxInd")->AsInteger + 1;
	ds->Close();

	int i = 0;
	ds->CommandText = "select elem,ind from ChemIndexes";
	ds->Open();
	ds->First();
	while(i < maxInd)
	{
		AnsiString str = ds->FieldByName("elem")->AsString;
		elemIndexes[str.c_str()]=ds->FieldByName("ind")->AsInteger;
		ds->Next();
		i++;
	}
    ds->Close();
	delete ds;
	int k = elemIndexes.size();
	TPr::SendToProtocol("���-�� ��������������������� ��� ��������� "+IntToStr(k));
}
//---------------------------------------------------------------------------
void Spectroscope::AddSpectroDataToSGTube(SolidGroups::Tube *tubeSG)
{
	vector<double> buffer;
	buffer = tubeSG->Coordinates();
	int startChem = buffer.size();
	//����������� ������ ������ �� ���������� ���������
	buffer.resize(elemIndexes.size()+startChem,0);
	UnicodeString data="";
	for(map<string,int>::iterator i=elemIndexes.begin(); i != elemIndexes.end(); i++)
	{
		int ind = (*i).second+startChem;
		if(ind>=startChem && ind<buffer.size())
		{
			buffer[ind] = himsostav[(*i).first]*100.0;
		}
		else
		{
			TPr::SendToProtocol(UnicodeString((*i).first.c_str())+" : "+FloatToStr((float)(*i).second)+" - ��� � ���� ��������");
		}
		data += UnicodeString((*i).first.c_str())+" : "+FloatToStr((float)himsostav[(*i).first]*100.0)+"; ";
	}
//	for(map<string,double>::iterator i=himsostav.begin(); i != himsostav.end(); i++)
//	{
//		data += UnicodeString((*i).first.c_str())+" : "+FloatToStr((float)(*i).second)+"; ";
//		int ind = elemIndexes[(*i).first]+startChem;
//		if(ind>=startChem && ind<buffer.size())
//		{
//			buffer[ind] = (*i).second;
//		}
//		else
//		{
//			TPr::SendToProtocol(UnicodeString((*i).first.c_str())+" : "+FloatToStr((float)(*i).second)+" - ��� � ���� ��������");
//		}
//	}
	TPr::SendToProtocol(data);
	for(int j = 0; j < buffer.size(); j++)
	{
		TPr::SendToProtocol("���������� ��"+IntToStr(j+1)+" ����� ������� ���������� = "+FloatToStr(buffer[j]));
	}
	tubeSG->setCoordinates(buffer);
}
//---------------------------------------------------------------------------
vector<double> Spectroscope::AddSpectroDataToSGEtalon(int tubeId,TADOConnection *conn)
{
	vector<double> ret;
	TADODataSet *ds = new TADODataSet(NULL);
	ds->Connection = conn;
	ds->CommandText = "select max(ind) as maxInd from ChemIndexes";
	ds->Open();
	ds->First();
	int maxInd = ds->FieldByName("maxInd")->AsInteger;
	ds->Close();
	ret.resize(maxInd+1,0);
	int i;
	ds->CommandText = "select ci.ind,e.value from SGEtalonChem e left join ChemIndexes ci on ci.elem=e.elem where id_etalon=:tubeId";
	ds->Parameters->ParamByName("tubeId")->Value = tubeId;
	ds->Open();
	ds->First();
	while(i<maxInd)
	{
		ret[ds->FieldByName("ind")->AsInteger] = ds->FieldByName("value")->AsFloat;
		ds->Next();
		i++;
	}
	ds->Close();
	delete ds;
	//initElemIndexes();
	return ret;
}
//---------------------------------------------------------------------------
void Spectroscope::WriteSpectroDataToDB(vector<double> coords,TADOConnection *conn)
{
	TADODataSet *ds = new TADODataSet(NULL);
	ds->Connection = conn;
	//������� ������������� ��������� ��������� �����
	ds->CommandText = "select max(ID) as id from SGEtalon";
	ds->Open();
	ds->First();
	int idTube = ds->FieldByName("id")->AsInteger;
	ds->Close();
	TPr::SendToProtocol("���� ����� �����-������� "+IntToStr(idTube));
	ds->CommandText = "select * from SGEtalonChem";
	ds->Open();
	string elem;
	for(int i=8;i<coords.size();i++)
	{
		elem="";
		for(map<string,int>::iterator it=elemIndexes.begin();it!=elemIndexes.end();it++)
		{
			if((*it).second==i-8)
			{
				elem=(*it).first;
				break;
			}
		}
		AnsiString ASElem = elem.c_str();
		TPr::SendToProtocol("������ � ������� � �� ������ ��: "+ASElem);
		if(!elem.empty())
		{
			ds->Insert();
			ds->FieldByName("id_etalon")->AsInteger = idTube;
			ds->FieldByName("elem")->AsString =ASElem;
			ds->FieldByName("value")->Value = (float)coords[i];
			ds->Post();

//			ds->Append();
//			ds->FieldByName("id_etalon")->Value = idTube;
//			ds->FieldByName("elem")->Value = elem.c_str();
//			ds->FieldByName("value")->Value = (float)coords[i];
		}
	}
	ds->Close();
	delete ds;
}
//---------------------------------------------------------------------------
void Spectroscope::pr(AnsiString str)
{
	TPr::SendToProtocol(str);
}
//---------------------------------------------------------------------------
const UnicodeString Spectroscope::getName()
{
	return name;
}
//---------------------------------------------------------------------------
map<string,double> Spectroscope::getSpectroData()
{
	dataReady=false;
	return himsostav;
}
//---------------------------------------------------------------------------
bool Spectroscope::isDataReady()
{
	return dataReady;
}
//---------------------------------------------------------------------------
