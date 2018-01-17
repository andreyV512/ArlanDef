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
// создаем событие
	DataReceived = new TEvent( (_SECURITY_ATTRIBUTES *)NULL, true, false, String( "rxEvent" ), false );
	DataReceived->ResetEvent();
// создаем COM порт
	CommPort = new TCommPort(MainForm);
	SetComPortSettings();
	CommPort->OnDataReceived = MyComPortDataReceived;
	CommPort->Open = true;
	ASUMode = 2;	// обмен по 2 протоколу - Пыть-Ях (по образцу Талинки, Приютово)
	abonent = 2;	// номер абонента АСУ в сети
	length = 0;		// длина заполняется при передаче
	function = 0;	// аналогично
	plant_cut = 0;
	TubeNumTimeout = 4000;		// ожидание посылки с номером
	ResultTimeout = 5000;		// ожидание ответа после посылки с результатом
	TestTimeout = 3000;			// ожидание тестовой посылки
	SolidGroupTimeout = 5000;	// ожидание группы прочности
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
//------настройки соединения-------------------------------------------------
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
			throw Exception("MyComPort::SetComPortSettings: не корректная скорость порта");
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
			throw Exception("MyComPort::SetComPortSettings: не корректная четность");
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
			throw Exception("MyComPort::SetComPortSettings: не корректные стоп-биты");
	}
}
//---------------------------------------------------------------------------
//------рассчет контрольной суммы по 1 протоколу (Стрежевой)-----------------
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
//------рассчет контрольной суммы по 2 протоколу (Пыть-Ях)-------------------
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
//------общая функция запроса номера трубы-----------------------------------
AnsiString MyComPort::GetTubeNumber(bool* _transit)
{
		return GetTubeNumber_2(_transit);
}
//---------------------------------------------------------------------------
//----общая функция посылки результата---------------------------------------
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
// послыает результат в АСУ: res = признак годно/брак, c1 = рез 1, с2 = рез 2, len = длина в зонах, sgg - группа прочности
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Ждем передачи результата ";
	// делаем запрос сеанса обмена 2 (отправка данных)
	CommPort->PutChar('\5');
	bool ok = false;
	// ждем ответа - события
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки \"ACK\" от АСУ!";
		return false;
	}
	else
	{
		if ( data_size == 1 && data[0] == 21 )
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "АСУ не готов к приему результата!";
			return false;
		}
		else if ( data_size != 1)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "Неверная длина посылки!";
			return false;
		}
		else if ( data_size == 1 && data[0] == 6 )
		{
			// формируем посылку
			unsigned char sending[18] = { 2 ,0,0,0,0,0,0,0,0,0,0,0,0,0 ,48,48,0, 3 };
			// байты 1-5 = номер трубы
			String tubnum = Globals::tube_number;
			while ( tubnum.Length() < 5 )
				tubnum = "0" + tubnum;
			for (int i=1; i <= 5; i++)
				sending[i] = (char)tubnum[ i ];
			// байт 6 - результат
			sending[6] = res + 48;
			// байт 7 - группа прочности
			if ( sgg.value() == SolidGroups::Group::sgUnknown )
				sgg = SolidGroups::Group::sgK;
//			sending[7] = sgg.GroupToEnglish();
			// байты 8,9 - рез1
			String temp = IntToStr(c1);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 8] = (char)temp[ i+1 ];
			// байты 10,11 - рез2
			temp = IntToStr(c2);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 10] = (char)temp[ i+1 ];
			// байты 12,13 - длина трубы в зонах
			temp = IntToStr( len );
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 12] = (char)temp[ i+1 ];
			// байт 16 - контрольная сумма
			unsigned char* b;
			b = &sending[1];
			sending[16] = countCRC16_1( b , 15);
			CommPort->PutBlock( sending , sizeof (sending) );
			// ждем ответа - события
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
				MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки \"ACK\"от АСУ!";
				return false;
			}
			else
			{
				if ( data_size == 1 && data[0] == 21 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "АСУ принял результат с ошибкой!";
					return false;
				}
				else if ( data_size != 1)
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "Неверная длина посылки от АСУ!";
					return false;
				}
				else if ( data_size == 1 && data[0] == 6 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "Результат отправлен успешно";
					return true;
				}
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------
//------функция получения посылки группы прочности (Нягань НРС)--------------
SolidGroups::Tube MyComPort::GetSolidGroup()
{
	using namespace SolidGroups;
	SolidGroups::Tube tubeSG;	// группа прочности трубы
	tubeSG = SolidGroups::Tube();
	short measures[8] = {0};
	vector<double>t;
	t.resize(8);

	CommPort->PutChar('0');
	bool ok = false;
	// ждем ответа - события
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки с ГП!";
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
//-----событие: получение данных по COM порту--------------------------------
void __fastcall MyComPort::MyComPortDataReceived(TObject *Sender, DWORD Count)
{
	TThread::CurrentThread->Sleep(200);
//	если группа прочности по COM порту (Нягань НРС)
	if ( !SystemConst::ComWithASU && SystemConst::SolidGroupSource == sgsCOMPort )
	{
		if (CommPort->InBuffUsed == 16)
		{
			data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
			DataReceived->SetEvent();
		}
	}
// если обмен с цехом по COM порту  (Стрежевой)
	else if ( SystemConst::ComWithASU )
	{
		if (ASUMode == 1)		// Стрежевой - Пионерный
		{
			// номер трубы
			if (CommPort->InBuffUsed == 18)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
			// односимвольные ответы АСУ о готовности
			if (CommPort->InBuffUsed == 1)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
		}
		else if (ASUMode == 2)		// Пыть-Ях - Приютово
		{
			// номер трубы
			if (CommPort->InBuffUsed == 19)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
			// пятисимвольные ответы АСУ  - тест и правильный результат
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Неправильная посылка: " + IntToStr(kol) + " байт";
		MainForm->StatusBarTop->Refresh();
	}
	CommPort->FlushInBuffer();
}
//---------------------------------------------------------------------------
//----проверка присланной посылки (только 2 протокол)----------------------------------
bool MyComPort::CheckSentMessage()
{
	String text;
	if (data[0] != length)	// можно сделать ожидание другой посылки?
	{
		text = "Неверная длина посылки от АСУ!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	if (data[1] != abonent)	// можно сделать ожидание другой посылки?
	{
		text = "Посылка от АСУ не нам!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	if (data[2] != function)	// можно сделать ожидание другой посылки?
	{
		text = "Неверный код функции передачи!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	unsigned char* b;
	b = &data[0];
	unsigned short check_sum = countCRC16_2( b , length-2 );		// рассчетная контр сумма
	if( data[length-1] != ((check_sum >> 8) & 0x00FF) || data[length-2] != (check_sum & 0x00FF)  )
	{
		text = "Неправильная контрольная сумма!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		TPr::SendToProtocol(text);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
//----тест канала связи (только 2 протокол)----------------------------------
bool MyComPort::TestConnection()
{
	length = 5;		// ожидаемая длина ответа от АСУ
	function = 1;	// функция обмена с АСУ
	// делаем запрос по функции 1
	unsigned char test[5];
	test[0] = length;
	test[1] = abonent;
	test[2] = function;

	unsigned short crc16 = countCRC16_2( test, 3 );
	// в пакете сначала идёт младший байт, потом старший
	test[4] = (crc16 >> 8) & 0x00FF;
	test[3] = crc16 & 0x00FF;
	CommPort->PutBlock( test, sizeof(test) );

	bool ok = false;
	// ждем ответа - события
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
		Status("Не дождались тестовой посылки от АСУ!");
		return false;
	}

	Status("Тест связи пройден");
	return true;
}
//---------------------------------------------------------------------------
//-----запрос номера трубы по 2 протоколу------------------------------------
AnsiString MyComPort::GetTubeNumber_2(bool* _transit)
{
	length = 19;		// ожидаемая длина посылки от АСУ
	function = 2;		// функия обмена с АСУ
	plant_cut = 0;		// зона реза по шаблонированию
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Запрос номера трубы из АСУ";
	// делаем запрос по функции 1
	unsigned char numb[5] = { 0x5, 0, 0x2 ,0,0};	// длина посылки 5, абонент, функция обмена 2
	numb[1] = abonent;

	unsigned short crc16 = countCRC16_2( numb, 3 );
	// в пакете сначала идёт младший байт, потом старший
	numb[4] = (crc16 >> 8) & 0x00FF;
	numb[3] = crc16 & 0x00FF;
	CommPort->PutBlock( numb, sizeof(numb) );

	bool ok = false;
	// ждем ответа - события
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались номера трубы от АСУ!";
		TPr::SendToProtocol("Не дождались номера трубы от АСУ");
		return "";
	}
/*
	else
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Дождались номера трубы от АСУ но не поедем Все равно";
		TPr::SendToProtocol("Не дождались номера трубы от АСУ");
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
// посылает результат в АСУ: результаты попер, прод, толщ, общ , sgg - группа прочности (костыль)
//bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg)
bool MyComPort::SendResultToASU(int solid_num)
{
// для EliseevRes, костылища
//	TIniFile *ini = new TIniFile(Globals::IniFileName);
//	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
//	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
//	Good = (TColor) ini->ReadInteger("Color","Valid",0);
//	delete ini;

	length = 5;		// ожидаемая длина посылки от АСУ
	function = 5;	// функция обмена
	int i;
	unsigned char send[255];
	int x=0;
	send[x++] = 0;
	send[x++] = abonent;
	send[x++] = function;
	strncpy(send+x,Globals::tube_number.c_str(),10);
	x+=10;
	// пороги поперечного контроля
	short bord[2] = {0,0};
	for (i = 0; i < Singleton::Instance()->CrossResult->borders.Length; i++)
		bord[i] = (short) Singleton::Instance()->CrossResult->borders[i];
	send[x++] = bord[0] & 0x00FF;
	send[x++] = (bord[0] >> 8) & 0x00FF;
	send[x++] = bord[1] & 0x00FF;
	send[x++] = (bord[1] >> 8) & 0x00FF;
	// пороги продольного контроля
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

	// длина трубы в зонах, рез1, рез2
	send[x++] = Singleton::Instance()->SumResult->zones & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->zones >> 8) & 0x00FF;
	send[x++] = Singleton::Instance()->SumResult->cut1 & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->cut1 >> 8) & 0x00FF;
	send[x++] = Singleton::Instance()->SumResult->cut2 & 0x00FF;
	send[x++] = (Singleton::Instance()->SumResult->cut2 >> 8) &0x00FF;
	// общий результат, результат муфты (всегда годная)
	char q = 0;		// типа брак
	if (Singleton::Instance()->SumResult->decision.SubString(1,1) == "Г")
		q = 1;
	else if (Singleton::Instance()->SumResult->decision.SubString(1,1) == "К")
		q = 2;
	send[x++] = q;
	send[x++] = 0;
	send[x++] = 1;
	send[x++] = 0;
	// группа прочности муфты (всегда ?), ГП трубы
	send[x++] = 0;
//	send[34] = (unsigned char) sgg.GroupToNumber();
	send[x++] = (unsigned char) solid_num;
	// результат контроля по зонам (всегда 65 зон)
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
	// добавляем контрольную сумму
	send[0]=x+2;
	unsigned short crc16 = countCRC16_2( send, x );
	// в пакете сначала идёт младший байт, потом старший
	send[x++] = crc16 & 0x00FF;
	send[x++] = (crc16 >> 8) & 0x00FF;
	send[0]=x;
	CommPort->PutBlock( send, x );

	bool ok = false;
	// ждем ответа - события
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались ответа от АСУ!";
		TPr::SendToProtocol("Не дождались ответа от АСУ");
		return false;
	}
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Результат успешно отправлен в АСУ";
	return true;
}
//----функция записи результатов по-Елисеевски-------------------------------
unsigned char MyComPort::EliseevRes(int zone)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	TColor Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	TColor Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	TColor Good = (TColor) ini->ReadInteger("Color","Valid",0);
	delete ini;

	// заполняем байт по правилам: первые 2 бита - результат поперечной
	// 00 - нет результата, 01 - Годно, 10 - Класс2, 11 - Брак
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
	// еще 2 бита - результат продольной. Биты уже дороже: 4 и 8
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
	// еще два бита - результат толщинометрии. Биты 16 и 32
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
	// еще два бита - общий результат. Биты 64 и 128
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
// рабочие посылки протокола 1 (Стрежевой)
/*
	CommPort->PutChar('?');		// делаем запрос номера (отправка)
	CommPort->PutChar('\5');    // запрос сеанса обмена 2
	CommPort->PutChar('\6');		// хорошо
	CommPort->PutChar('\25');		// плохо
*/
/*
// тоже рабочая
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




