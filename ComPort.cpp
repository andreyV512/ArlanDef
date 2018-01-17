//---------------------------------------------------------------------------


#pragma hdrstop

#include "ComPort.h"
#include "Main.h"
#include <Math.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

MyComPort *MyCom;

//---------------------------------------------------------------------------
MyComPort::MyComPort()
{
	PortName = "COM1";
	BaudRate = 115200;
	DataBits = 8;
	StopBits = 0;
	Parity = 0;
	Create();
}
MyComPort::MyComPort(AnsiString _ini_section)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	if(!ini->ValueExists(_ini_section,"PortName"))
		ini->WriteString(_ini_section,"PortName","COM1");
	if(!ini->ValueExists(_ini_section,"BaudRate"))
		ini->WriteInteger(_ini_section,"BaudRate",115200);
	if(!ini->ValueExists(_ini_section,"DataBits"))
		ini->WriteInteger(_ini_section,"DataBits",8);
	if(!ini->ValueExists(_ini_section,"StopBits"))
		ini->WriteInteger(_ini_section,"StopBits",0);
	if(!ini->ValueExists(_ini_section,"Parity"))
		ini->WriteInteger(_ini_section,"Parity",0);
	PortName = ini->ReadString(_ini_section,"PortName","COM1");
	BaudRate = ini->ReadInteger(_ini_section,"BaudRate",115200);
	DataBits = ini->ReadInteger(_ini_section,"DataBits",8);
	StopBits = ini->ReadInteger(_ini_section,"StopBits",0);
	Parity = ini->ReadInteger(_ini_section,"Parity",0);
	delete ini;
	Create();
}
void MyComPort::Create()
{
// ������� �������
	DataReceived = new TEvent( (_SECURITY_ATTRIBUTES *)NULL, true, false, String( "rxEvent" ), false );
	DataReceived->ResetEvent();
// ������� COM ����
	CommPort = new TCommPort(MainForm);
	SetComPortSettings();
	CommPort->OnDataReceived = MyComPortDataReceived;
	CommPort->Open = true;
	ASUMode = 2;	// ����� �� 2 ��������� - ����-�� (�� ������� �������, ��������)
	abonent = 2;	// ����� �������� ��� � ����
	length = 0;		// ����� ����������� ��� ��������
	function = 0;	// ����������
	plant_cut = 0;
	TubeNumTimeout = 4000;		// �������� ������� � �������
	ResultTimeout = 5000;		// �������� ������ ����� ������� � �����������
	TestTimeout = 3000;			// �������� �������� �������
	SolidGroupTimeout = 5000;	// �������� ������ ���������
	OnStatus=NULL;
}

//---------------------------------------------------------------------------
MyComPort::~MyComPort()
{
	CommPort->Open = false;
	delete CommPort;
	delete DataReceived;
}
//---------------------------------------------------------------------------
//------��������� ����������-------------------------------------------------
#define BAUDRATECASE(XXX) case XXX: CommPort->BaudRate=cbr##XXX##;break;
void MyComPort::SetComPortSettings()
{
	CommPort->ComNumber = PortName.SubString(4,3).ToInt();
	switch(BaudRate)
	{
		BAUDRATECASE(110)
		BAUDRATECASE(300)
		BAUDRATECASE(600)
		BAUDRATECASE(1200)
		BAUDRATECASE(2400)
		BAUDRATECASE(4800)
		BAUDRATECASE(9600)
		BAUDRATECASE(14400)
		BAUDRATECASE(19200)
		BAUDRATECASE(38400)
		BAUDRATECASE(56000)
		BAUDRATECASE(57600)
		BAUDRATECASE(115200)
		BAUDRATECASE(128000)
		BAUDRATECASE(256000)
		default:
			throw Exception("MyComPort::SetComPortSettings: �� ���������� �������� �����");
	}
	CommPort->DataBits = DataBits;
	switch(Parity)
	{
		case 0:
			CommPort->Parity=paNone;
		break;
		case 1:
			CommPort->Parity=paOdd;
		break;
		case 2:
			CommPort->Parity=paEven;
		break;
		case 3:
			CommPort->Parity=paMark;
		break;
		case 4:
			CommPort->Parity=paSpace;
		break;
		default:
			throw Exception("MyComPort::SetComPortSettings: �� ���������� ��������");
	}
	switch(StopBits)
	{
		case 0:
			CommPort->StopBits=sb1_0;
		break;
		case 1:
			CommPort->StopBits=sb1_5;
		break;
		case 2:
			CommPort->StopBits=sb2_0;
		break;
		default:
			throw Exception("MyComPort::SetComPortSettings: �� ���������� ����-����");
	}
}
//---------------------------------------------------------------------------
//------������� ����������� ����� �� 1 ��������� (���������)-----------------
unsigned char MyComPort::countCRC16_1(unsigned char* buff, unsigned int size)
{
	unsigned long sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum +=buff[i];
	}
	sum = sum & 0xFF;
	return sum;
}
//---------------------------------------------------------------------------
//------������� ����������� ����� �� 2 ��������� (����-��)-------------------
unsigned short MyComPort::countCRC16_2(unsigned char* bufData, unsigned int sizeData)
{
	unsigned short __CRC=0xFFFF;

	for(int i=0; i < sizeData; i++)
	{
		__CRC ^= bufData[i];
		for(int j=0; j < 8; j++)
		{
			unsigned short testbit = __CRC & 0x0001;
			__CRC =(__CRC>>1);
			if( testbit )
				__CRC ^= 0xA001;
/*
			AnsiString b="   [";
			b+=i;
			b+=",";
			b+=j;
			b+="]";
			b+=__CRC;
			TPr::SendToProtocol(b);
*/
		}
/*
		AnsiString a="CRC: ";
		a.cat_printf("0x%X",(int)(bufData[i]));
		a+=" ";
		a+=bufData[i];
		a+=" [";
		a+=i;
		a+="] ";
		a+=__CRC;
		TPr::SendToProtocol(a);
*/
	}
	return __CRC;
}
//---------------------------------------------------------------------------
/*
unsigned short MyComPort::countCRC16_2(unsigned char* bufData, unsigned int sizeData)
{
	unsigned char j;
	unsigned int __CRC;
	unsigned int TmpCRC;
	unsigned int i;

	__CRC = 0x7FFFFFFF;

	for(i=0; i < sizeData; i++)
	{
		__CRC = __CRC ^ bufData[i];
		for(j=0; j < 8; j++)
		{
			int testbit = __CRC & 0x0001;
			__CRC = (__CRC >> 1 ) & 0x7FFF;
			if( testbit )
				__CRC = __CRC ^ 0xA001;
		}
	}
	//__CRC = __CRC >> 16;

	return __CRC;
}
*/
//---------------------------------------------------------------------------
//------����� ������� ������� ������ �����-----------------------------------
AnsiString MyComPort::GetTubeNumber(bool* _transit)
{
		return GetTubeNumber_2(_transit);
}
//---------------------------------------------------------------------------
//----����� ������� ������� ����������---------------------------------------
/*
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	if (ASUMode == 1)
		return SendResultToASU_1(res, c1, c2, len , sgg);
	else if (ASUMode == 2)
		return SendResultToASU_2(res, c1, c2, len , sgg);
}
*/
//---------------------------------------------------------------------------
// �������� ��������� � ���: res = ������� �����/����, c1 = ��� 1, �2 = ��� 2, len = ����� � �����, sgg - ������ ���������
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "���� �������� ���������� ";
	// ������ ������ ������ ������ 2 (�������� ������)
	CommPort->PutChar('\5');
	bool ok = false;
	// ���� ������ - �������
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� \"ACK\" �� ���!";
		return false;
	}
	else
	{
		if ( data_size == 1 && data[0] == 21 )
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "��� �� ����� � ������ ����������!";
			return false;
		}
		else if ( data_size != 1)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "�������� ����� �������!";
			return false;
		}
		else if ( data_size == 1 && data[0] == 6 )
		{
			// ��������� �������
			unsigned char sending[18] = { 2 ,0,0,0,0,0,0,0,0,0,0,0,0,0 ,48,48,0, 3 };
			// ����� 1-5 = ����� �����
			String tubnum = Globals::tube_number;
			while ( tubnum.Length() < 5 )
				tubnum = "0" + tubnum;
			for (int i=1; i <= 5; i++)
				sending[i] = (char)tubnum[ i ];
			// ���� 6 - ���������
			sending[6] = res + 48;
			// ���� 7 - ������ ���������
			if ( sgg.value() == SolidGroups::Group::sgUnknown )
				sgg = SolidGroups::Group::sgK;
//			sending[7] = sgg.GroupToEnglish();
			// ����� 8,9 - ���1
			String temp = IntToStr(c1);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 8] = (char)temp[ i+1 ];
			// ����� 10,11 - ���2
			temp = IntToStr(c2);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 10] = (char)temp[ i+1 ];
			// ����� 12,13 - ����� ����� � �����
			temp = IntToStr( len );
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 12] = (char)temp[ i+1 ];
			// ���� 16 - ����������� �����
			unsigned char* b;
			b = &sending[1];
			sending[16] = countCRC16_1( b , 15);
			CommPort->PutBlock( sending , sizeof (sending) );
			// ���� ������ - �������
			ok = false;
			for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
			{
				Application->ProcessMessages();
				if( DataReceived->WaitFor( 200 ) == wrSignaled )
				{
					DataReceived->ResetEvent();
					ok = true;
					break;
				}
			}
			if ( !ok )
			{
				MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� \"ACK\"�� ���!";
				return false;
			}
			else
			{
				if ( data_size == 1 && data[0] == 21 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "��� ������ ��������� � �������!";
					return false;
				}
				else if ( data_size != 1)
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "�������� ����� ������� �� ���!";
					return false;
				}
				else if ( data_size == 1 && data[0] == 6 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "��������� ��������� �������";
					return true;
				}
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------
//------������� ��������� ������� ������ ��������� (������ ���)--------------
SolidGroups::Tube MyComPort::GetSolidGroup()
{
	using namespace SolidGroups;
	SolidGroups::Tube tubeSG;	// ������ ��������� �����
	tubeSG = SolidGroups::Tube();
	short measures[8] = {0};
	vector<double>t;
	t.resize(8);

	CommPort->PutChar('0');
	bool ok = false;
	// ���� ������ - �������
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < SolidGroupTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}

	if (!ok)
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� � ��!";
		return tubeSG;
	}

	for (int i = 0; i < 8; i++)
	{
		measures[i] = (int16_t)(data[2 * i + 1] << 8) | (data[2 * i]);
		t[i]        = (double) measures[i];
	}
	CommPort->FlushInBuffer();

	tubeSG.setCoordinates(t);
	tubeSG.setTubeType(TubeType(Globals::current_diameter));
	return tubeSG;
}
//---------------------------------------------------------------------------
//-----�������: ��������� ������ �� COM �����--------------------------------
void __fastcall MyComPort::MyComPortDataReceived(TObject *Sender, DWORD Count)
{
	TThread::CurrentThread->Sleep(200);
//	���� ������ ��������� �� COM ����� (������ ���)
	if ( !SystemConst::ComWithASU && SystemConst::SolidGroupSource == sgsCOMPort )
	{
		if (CommPort->InBuffUsed == 16)
		{
			data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
			DataReceived->SetEvent();
		}
	}
// ���� ����� � ����� �� COM �����  (���������)
	else if ( SystemConst::ComWithASU )
	{
		if (ASUMode == 1)		// ��������� - ���������
		{
			// ����� �����
			if (CommPort->InBuffUsed == 18)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
			// �������������� ������ ��� � ����������
			if (CommPort->InBuffUsed == 1)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
		}
		else if (ASUMode == 2)		// ����-�� - ��������
		{
			// ����� �����
			if (CommPort->InBuffUsed == 19)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
			// �������������� ������ ���  - ���� � ���������� ���������
			if (CommPort->InBuffUsed == 5)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
		}
	}
	else if ( CommPort->InBuffUsed )
	{
		unsigned char buf[50];
		int kol = CommPort->GetBlock(buf, CommPort->InBuffUsed);
		MainForm->StatusBarTop->Panels->Items[2]->Text = "������������ �������: " + IntToStr(kol) + " ����";
		MainForm->StatusBarTop->Refresh();
	}
	CommPort->FlushInBuffer();
}
//---------------------------------------------------------------------------
//----�������� ���������� ������� (������ 2 ��������)----------------------------------
bool MyComPort::CheckSentMessage()
{
	String text;
	if (data[0] != length)	// ����� ������� �������� ������ �������?
	{
		text = "�������� ����� ������� �� ���!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	if (data[1] != abonent)	// ����� ������� �������� ������ �������?
	{
		text = "������� �� ��� �� ���!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	if (data[2] != function)	// ����� ������� �������� ������ �������?
	{
		text = "�������� ��� ������� ��������!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	unsigned char* b;
	b = &data[0];
	unsigned short check_sum = countCRC16_2( b , length-2 );		// ���������� ����� �����
	if( data[length-1] != ((check_sum >> 8) & 0x00FF) || data[length-2] != (check_sum & 0x00FF)  )
	{
		text = "������������ ����������� �����!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
//----���� ������ ����� (������ 2 ��������)----------------------------------
bool MyComPort::TestConnection()
{
	length = 5;		// ��������� ����� ������ �� ���
	function = 1;	// ������� ������ � ���
	// ������ ������ �� ������� 1
	unsigned char test[5];
	test[0] = length;
	test[1] = abonent;
	test[2] = function;

	unsigned short crc16 = countCRC16_2( test, 3 );
	// � ������ ������� ��� ������� ����, ����� �������
	test[4] = (crc16 >> 8) & 0x00FF;
	test[3] = crc16 & 0x00FF;
	CommPort->PutBlock( test, sizeof(test) );

	bool ok = false;
	// ���� ������ - �������
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < TestTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		Status("�� ��������� �������� ������� �� ���!");
		return false;
	}

	Status("���� ����� �������");
	return true;
}
//---------------------------------------------------------------------------
//-----������ ������ ����� �� 2 ���������------------------------------------
AnsiString MyComPort::GetTubeNumber_2(bool* _transit)
{
	length = 19;		// ��������� ����� ������� �� ���
	function = 2;		// ������ ������ � ���
	plant_cut = 0;		// ���� ���� �� ��������������
	MainForm->StatusBarTop->Panels->Items[2]->Text = "������ ������ ����� �� ���";
	// ������ ������ �� ������� 1
	unsigned char numb[5] = { 0x5, 0, 0x2 ,0,0};	// ����� ������� 5, �������, ������� ������ 2
	numb[1] = abonent;

	unsigned short crc16 = countCRC16_2( numb, 3 );
	// � ������ ������� ��� ������� ����, ����� �������
	numb[4] = (crc16 >> 8) & 0x00FF;
	numb[3] = crc16 & 0x00FF;
	CommPort->PutBlock( numb, sizeof(numb) );

	bool ok = false;
	// ���� ������ - �������
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < TubeNumTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������ ����� �� ���!";
		TPr::SendToProtocol("�� ��������� ������ ����� �� ���");
		return "";
	}
/*
	else
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "��������� ������ ����� �� ��� �� �� ������ ��� �����";
		TPr::SendToProtocol("�� ��������� ������ ����� �� ���");
		return -1;
	}
*/
	char c0=data[13];
	char c1=data[14];
	int ltransit=0;
	ltransit|=c0;
	ltransit|=c1<<8;
	char* s=data+3;
	s[10]='\0';
	AnsiString ret=s;
	*_transit=(ltransit==0);
	//*_transit=true;
	return ret;
}
//---------------------------------------------------------------------------
// �������� ��������� � ���: ���������� �����, ����, ����, ��� , sgg - ������ ��������� (�������)
//bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg)
bool MyComPort::SendResultToASU(int solid_num)
{
// ��� EliseevRes, ���������
//	TIniFile *ini = new TIniFile(Globals::IniFileName);
//	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
//	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
//	Good = (TColor) ini->ReadInteger("Color","Valid",0);
//	delete ini;

	length = 5;		// ��������� ����� ������� �� ���
	function = 5;	// ������� ������
	int i;
	unsigned char send[255];
	int x=0;
	send[x++] = 0;
	send[x++] = abonent;
	send[x++] = function;
	strncpy(send+x,Globals::tube_number.c_str(),10);
	x+=10;
	// ������ ����������� ��������
	short bord[2] = {0,0};
	for (i = 0; i < Singleton::Instance()->CrossResult->borders.Length; i++)
		bord[i] = (short) Singleton::Instance()->CrossResult->borders[i];
	send[x++] = bord[0] & 0x00FF;
	send[x++] = (bord[0] >> 8) & 0x00FF;
	send[x++] = bord[1] & 0x00FF;
	send[x++] = (bord[1] >> 8) & 0x00FF;
	// ������ ����������� ��������
	if ( !MainForm->cbLinear->Checked )
		for (int j = 0; j < 4; j++)
			send[x++] = 0;
	else
	{
		bord[0]=0;
		bord[1]=0;
		for (i = 0; i < Singleton::Instance()->LinearResult->borders.Length; i++)
			bord[i] = (short) Singleton::Instance()->LinearResult->borders[i];
		send[x++] = bord[0] & 0x00FF;
		send[x++] = (bord[0] >> 8) & 0x00FF;
		send[x++] = bord[1] & 0x00FF;
		send[x++] = (bord[1] >> 8) & 0x00FF;
	}
	bord[0] = 0;
	bord[1] = 0;
	for (i = 0; i < Singleton::Instance()->ThResult->borders.Length; i++)
		bord[i] = (short) (Singleton::Instance()->ThResult->borders[i] * 10);
	send[x++] = bord[0] & 0x00FF;
	send[x++] = (bord[0] >> 8) & 0x00FF;
	send[x++] = bord[1] & 0x00FF;
	send[x++] = (bord[1] >> 8) & 0x00FF;

	// ����� ����� � �����, ���1, ���2
	send[x++] = Singleton::Instance()->SumResult->zones & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->zones >> 8) & 0x00FF;
	send[x++] = Singleton::Instance()->SumResult->cut1 & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->cut1 >> 8) & 0x00FF;
	send[x++] = Singleton::Instance()->SumResult->cut2 & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->cut2 >> 8) &0x00FF;
	// ����� ���������, ��������� ����� (������ ������)
	char q = 0;		// ���� ����
	if (Singleton::Instance()->SumResult->decision.SubString(1,1) == "�")
		q = 1;
	else if (Singleton::Instance()->SumResult->decision.SubString(1,1) == "�")
		q = 2;
	send[x++] = q;
	send[x++] = 0;
	send[x++] = 1;
	send[x++] = 0;
	// ������ ��������� ����� (������ ?), �� �����
	send[x++] = 0;
//	send[34] = (unsigned char) sgg.GroupToNumber();
	send[x++] = (unsigned char) solid_num;
	// ��������� �������� �� ����� (������ 65 ���)
	for ( i = 0; i < 65; i++)
	{
		send[x]=0;
		send[x+1]=0;
		if (i < Singleton::Instance()->ThResult->zones )
		{
			send[x] = (unsigned char)RoundTo(Singleton::Instance()->ThResult->zone_data[i] * 10,0);
			if (Singleton::Instance()->ThResult->zone_data[i] == 8.0)
				send[x] = 255;
		}
		send[x + 1] = EliseevRes(i);
		x+=2;
	}
	// ��������� ����������� �����
	send[0]=x+2;
	unsigned short crc16 = countCRC16_2( send, x );
	// � ������ ������� ��� ������� ����, ����� �������
	send[x++] = crc16 & 0x00FF;
	send[x++] = (crc16 >> 8) & 0x00FF;
	send[0]=x;
	CommPort->PutBlock( send, x );

	bool ok = false;
	// ���� ������ - �������
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������ �� ���!";
		TPr::SendToProtocol("�� ��������� ������ �� ���");
		return false;
	}
	MainForm->StatusBarTop->Panels->Items[2]->Text = "��������� ������� ��������� � ���";
	return true;
}
//----������� ������ ����������� ��-����������-------------------------------
unsigned char MyComPort::EliseevRes(int zone)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	TColor Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	TColor Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	TColor Good = (TColor) ini->ReadInteger("Color","Valid",0);
	delete ini;

	// ��������� ���� �� ��������: ������ 2 ���� - ��������� ����������
	// 00 - ��� ����������, 01 - �����, 10 - �����2, 11 - ����
	unsigned char re = 0;
	TColor color;
	Result* pp2=Singleton::Instance()->CrossResult;

	if (zone < pp2->zones)
	{
		color = pp2->ZoneColor(pp2->zone_data[zone]);
		if (color == Good)
			re += 1;
		else if (color == Class2)
			re += 2;
		else if (color == Brack)
			re += 3;
	}
	// ��� 2 ���� - ��������� ����������. ���� ��� ������: 4 � 8
	Result* pr2=Singleton::Instance()->LinearResult;
	if (zone < pr2->zones )
	{
		color = pr2->ZoneColor(pr2->zone_data[zone]);
		if (color == Good)
			re += 4;
		else if (color == Class2)
			re += 8;
		else if (color == Brack)
			re += 12;
	}
	// ��� ��� ���� - ��������� �������������. ���� 16 � 32
	ThicknessResult* th2=Singleton::Instance()->ThResult;

	if (zone < th2->zones )
	{
		color = th2->ZoneColor(th2->zone_data[zone]);
		if (color == Good)
			re += 16;
		else if (color == Class2)
			re += 32;
		else if (color == Brack)
			re += 48;
	}
	// ��� ��� ���� - ����� ���������. ���� 64 � 128
	SummaryResult* sum2=Singleton::Instance()->SumResult;
	if (zone < sum2->zones )
	{
		color = sum2->ZoneColor(sum2->zone_data[zone]);
		if (color == Good)
			re += 64;
		else if (color == Class2)
			re += 128;
		else if (color == Brack)
			re += 192;
	}
	return re;
}
//---------------------------------------------------------------------------
// ������� ������� ��������� 1 (���������)
/*
	CommPort->PutChar('?');		// ������ ������ ������ (��������)
	CommPort->PutChar('\5');    // ������ ������ ������ 2
	CommPort->PutChar('\6');		// ������
	CommPort->PutChar('\25');		// �����
*/
/*
// ���� �������
	unsigned char reply[4] = {63, 0x5, 6, 0x15};
	CommPort->PutBlock(reply, sizeof (reply) );
*/
void MyComPort::Status(AnsiString _msg)
{
	if(OnStatus!=NULL)
		OnStatus(_msg);
}
bool MyComPort::SendToProtocol()
{
//	TIniFile *ini = new TIniFile(Globals::IniFileName);
//	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
//	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
//	Good = (TColor) ini->ReadInteger("Color","Valid",0);
//	delete ini;
	for (int i = 0; i < 65; i++)
	{
		AnsiString a="";
		a+="SendToProtocol [";
		a+=i;
		a+="] ";
		if (i < Singleton::Instance()->ThResult->zones )
		{
			if (Singleton::Instance()->ThResult->zone_data[i] == 8.0)
			{
				a+=255;
				a+=" ";
				a+=255;
			}
			else
			{
				a+=(Singleton::Instance()->ThResult->zone_data[i] * 10);
				a+=" ";
				a+= (unsigned char)RoundTo(Singleton::Instance()->ThResult->zone_data[i] * 10,0);
			}
		}
		else
			a+="? ?";
		a+=" ";
		unsigned char re=EliseevRes(i);
		a+=re;
		a+=" ";
		for(int i=7;i>=0;i--)
		{
			if((re&(1<<i))!=0)
				a+="1";
			else
				a+="0";
		}
		TPr::SendToProtocol(a);
	}
}




