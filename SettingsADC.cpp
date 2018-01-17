//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SettingsADC.h"
#include "DataModule.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormSettADC *FormSettADC;
//---------------------------------------------------------------------------
__fastcall TFormSettADC::TFormSettADC(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormCreate(TObject *Sender)
{

	Ini = new TIniFile(Globals::IniFileName);
		if(Ini->ReadInteger("RangePP","Channels",12)==10)
			{
				CSpinEdit11->Enabled=false;
				CSpinEdit12->Enabled=false;
				this->ComboBox11->Enabled=false;
				this->ComboBox12->Enabled=false;
			}
//Читаем входной диапазон (Тут надо бы перелопатить этот быдло код)

	Read(ComboBox1,"RangePP"); Read(ComboBox2,"RangePP");
	Read(ComboBox3,"RangePP"); Read(ComboBox4,"RangePP");
	Read(ComboBox5,"RangePP"); Read(ComboBox6,"RangePP");
	Read(ComboBox7,"RangePP"); Read(ComboBox8,"RangePP");
	Read(ComboBox9,"RangePP"); Read(ComboBox10,"RangePP");
	Read(ComboBox11,"RangePP"); Read(ComboBox12,"RangePP");

//Читаем логические номера каналов
	ReadNum(CSpinEdit1,"RangePP"); ReadNum(CSpinEdit2,"RangePP");
	ReadNum(CSpinEdit3,"RangePP");ReadNum(CSpinEdit4,"RangePP");
	ReadNum(CSpinEdit5,"RangePP");ReadNum(CSpinEdit6,"RangePP");
	ReadNum(CSpinEdit7,"RangePP");ReadNum(CSpinEdit8,"RangePP");
	ReadNum(CSpinEdit9,"RangePP");ReadNum(CSpinEdit10,"RangePP");
	ReadNum(CSpinEdit11,"RangePP");ReadNum(CSpinEdit12,"RangePP");

//Читаем задержку между кадрами,частоту АЦП,размер буфера АЦП
	EditKadr->Text=1/(float)(Ini->ReadFloat("RangePP", "Kadr",5));
	EditRate->Text=(int)(Ini->ReadInteger("RangePP", "Rate",     0));
	EditSizeBuf->Text=(int)(Ini->ReadInteger("RangePP","SizeBuf",0));

//--------------------работаем с продольной платой------------------------------

	Read(ComboBox13,"RangePR"); Read(ComboBox14,"RangePR");
	Read(ComboBox15,"RangePR"); Read(ComboBox16,"RangePR");

//Читаем логические номера каналов
	ReadNum(CSpinEdit13,"RangePR"); ReadNum(CSpinEdit14,"RangePR");
	ReadNum(CSpinEdit15,"RangePR");ReadNum(CSpinEdit16,"RangePR");

//Читаем задержку между кадрами,частоту АЦП,размер буфера АЦП
	Edit3->Text=1/(float)(Ini->ReadFloat("RangePR", "Kadr",	5));
	Edit2->Text=(int)(Ini->ReadInteger("RangePR", "Rate",   100));
	Edit1->Text=(int)(Ini->ReadInteger("RangePR","SizeBuf", 100));

//Читаем настройки температуры соленоидов и контроля магнитного поля
	SpinTemp1->Value=Ini->ReadInteger("OtherSettings","Temp1",29);
	SpinTemp2->Value=Ini->ReadInteger("OtherSettings","Temp2",30);
	SpinMagnetic->Value=Ini->ReadInteger("OtherSettings","Magnetic",31);

	SpinKataphot1->Value=Ini->ReadInteger("OtherSettings","InKataphot",14);
	SpinKataphot2->Value=Ini->ReadInteger("OtherSettings","OutKataphot",15);
	SpinSG->Value=Ini->ReadInteger("OtherSettings","SensorSG",16);
	SpinCurr->Value=Ini->ReadInteger("OtherSettings","SensorCurr",17);

}
//---------------------------------------------------------------------------
void __fastcall TFormSettADC::ReadNum(TCSpinEdit* CS,UnicodeString Type)     //Читаем номер логического канала
{
	CS->Value=Ini->ReadInteger(Type,"Number"+(UnicodeString)CS->Tag,CS->Tag);
}


//---------------------------------------------------------------------------
void __fastcall TFormSettADC::WriteNum(TCSpinEdit* CS,UnicodeString Type)   //Пишем номер логического канала
{

	Ini->WriteInteger(Type,"Number"+(UnicodeString)(CS->Tag),CS->Value);

}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::Write(TComboBox* t,UnicodeString Type)              //Пишем настройки в Ini файл
{
   int Range;
	switch (t->ItemIndex)
	{      //Получаем индекс записываем усиление
		   case 0: { Range=1;  break; }
		   case 1: { Range=2;  break; }
		   case 2: { Range=4;  break; }
		   case 3: { Range=8;  break; }
		   case 4: { Range=16; break; }
	default:;
	}
	Ini->WriteInteger(Type,"Range"+(UnicodeString)(t->Tag-1),Range);
}
//---------------------------------------------------------------------------
void __fastcall TFormSettADC::Read(TComboBox* t,UnicodeString Type)             //Читаем настройки из Ini Файла и втыкаем в форму
{
	int buf;
	 buf=Ini->ReadInteger(Type,"Range"+(UnicodeString)(t->Tag-1),0);
		switch(buf)
		{       //Получаем усиление и выставляем индекс в ComboBоx
			   case 1: { t->ItemIndex=0;  break; }
			   case 2: { t->ItemIndex=1;  break; }
			   case 4: { t->ItemIndex=2;  break; }
			   case 8: { t->ItemIndex=3;  break; }
			   case 16:{ t->ItemIndex=4;  break; }
		default:;
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormSettADC::bSaveClick(TObject *Sender)
{
	//Проверяем на ввод данных и пишем Default
	if (EditKadr->Text=="") EditKadr->Text="5";
	if ((EditRate->Text=="") || (EditRate->Text=="0")) EditRate->Text="100";
	if ((EditSizeBuf->Text=="") || (EditSizeBuf->Text=="0")) EditSizeBuf->Text="100";

	//Пишем в файл данные о входных диапазонах
	Write(ComboBox1,"RangePP"); Write(ComboBox2,"RangePP");
	Write(ComboBox3,"RangePP"); Write(ComboBox4,"RangePP");
	Write(ComboBox5,"RangePP"); Write(ComboBox6,"RangePP");
	Write(ComboBox7,"RangePP"); Write(ComboBox8,"RangePP");
	Write(ComboBox9,"RangePP"); Write(ComboBox10,"RangePP");
	Write(ComboBox11,"RangePP");Write(ComboBox12,"RangePP");

	//Пишем данные о логических каналах
	WriteNum(CSpinEdit1, "RangePP");  WriteNum(CSpinEdit2, "RangePP");
	WriteNum(CSpinEdit3, "RangePP");  WriteNum(CSpinEdit4, "RangePP");
	WriteNum(CSpinEdit5, "RangePP");  WriteNum(CSpinEdit6, "RangePP");
	WriteNum(CSpinEdit7, "RangePP");  WriteNum(CSpinEdit8, "RangePP");
	WriteNum(CSpinEdit9, "RangePP");  WriteNum(CSpinEdit10,"RangePP");
	WriteNum(CSpinEdit11,"RangePP");  WriteNum(CSpinEdit12, "RangePP");

	//пишем остальные параметры
	Ini->WriteInteger("RangePP","SizeBuf", (EditSizeBuf->Text.ToInt()));
	Ini->WriteInteger("RangePP","Rate",	(EditRate->Text.ToInt()	     ));

	if (EditKadr->Text.ToDouble()==0) EditKadr->Text="5";
	Ini->WriteFloat("RangePP","Kadr",	(1/EditKadr->Text.ToDouble()));

}
//---------------------------------------------------------------------------


void __fastcall TFormSettADC::Button1Click(TObject *Sender)
{
	//Проверяем на ввод данных и пишем Default
	if (Edit1->Text=="") Edit1->Text="120";                                 //буфер АЦП
	if ((Edit2->Text=="") || (Edit2->Text=="0")) Edit2->Text="100";    //Частота АЦП
	if ((Edit3->Text=="") || (Edit3->Text=="0")) Edit3->Text="5";     //Задержка между кадрами(Гц)

	//Пишем в файл данные о входных диапазонах
	Write(ComboBox13,"RangePR"); Write(ComboBox14,"RangePR");
	Write(ComboBox15,"RangePR"); Write(ComboBox16,"RangePR");


	//Пишем данные о логических каналах
	WriteNum(CSpinEdit13, "RangePR");  WriteNum(CSpinEdit14, "RangePR");
	WriteNum(CSpinEdit15, "RangePR");  WriteNum(CSpinEdit16, "RangePR");


	Ini->WriteInteger("RangePR","SizeBuf", (Edit1->Text.ToInt() ));
	Ini->WriteInteger("RangePR","Rate",	(Edit2->Text.ToInt()    ));
	if (Edit3->Text.ToDouble()==0) Edit3->Text="5";
	Ini->WriteFloat("RangePR","Kadr",	(1/Edit3->Text.ToDouble()));;


}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::Button2Click(TObject *Sender)
{
	FormSettADC->Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormSettADC::EditSizeBufKeyPress(TObject *Sender, wchar_t &Key)
{
	if(!((Key >= '0' && Key <= '9') || Key == VK_BACK)) Key = 0x00;       //Не даем нажать ничего кроме цифр и BackSpace
}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
	//Проверяем на нажатие сочетания клавиш Ctrl+Alt+Enter
	if (Msg.message == WM_HOTKEY) // сообщение наше
		   {
		   if (Msg.wParam == 0x00F) // идентификатор наш
				   {
						if (Edit1->Enabled)
						{
							Edit1->Enabled=false;
							Edit2->Enabled=false;
							Edit3->Enabled=false;
							EditKadr->Enabled=false;
							EditRate->Enabled=false;
							EditSizeBuf->Enabled=false;
							TabSheet3->Enabled=false;
							CSpinEdit1->Enabled=false;
							CSpinEdit2->Enabled=false;
							CSpinEdit3->Enabled=false;
							CSpinEdit4->Enabled=false;
							CSpinEdit5->Enabled=false;
							CSpinEdit6->Enabled=false;
							CSpinEdit7->Enabled=false;
							CSpinEdit8->Enabled=false;
							CSpinEdit9->Enabled=false;
							CSpinEdit10->Enabled=false;
							CSpinEdit11->Enabled=false;
							CSpinEdit12->Enabled=false;
							CSpinEdit13->Enabled=false;
							CSpinEdit14->Enabled=false;
							CSpinEdit15->Enabled=false;
							CSpinEdit16->Enabled=false;


						}
						else
						{
							Edit1->Enabled=true;
							Edit2->Enabled=true;
							Edit3->Enabled=true;
							EditKadr->Enabled=true;
							EditRate->Enabled=true;
							EditSizeBuf->Enabled=true;
							TabSheet3->Enabled=true;
							CSpinEdit1->Enabled=true;
							CSpinEdit2->Enabled=true;
							CSpinEdit3->Enabled=true;
							CSpinEdit4->Enabled=true;
							CSpinEdit5->Enabled=true;
							CSpinEdit6->Enabled=true;
							CSpinEdit7->Enabled=true;
							CSpinEdit8->Enabled=true;
							CSpinEdit9->Enabled=true;
							CSpinEdit10->Enabled=true;
							CSpinEdit11->Enabled=true;
							CSpinEdit12->Enabled=true;
							CSpinEdit13->Enabled=true;
							CSpinEdit14->Enabled=true;
							CSpinEdit15->Enabled=true;
							CSpinEdit16->Enabled=true;
						}
				   }
		   }
}
//---------------------------------------------------------------------------



void __fastcall TFormSettADC::Button3Click(TObject *Sender)
{
	FormSettADC->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormActivate(TObject *Sender)
{
		//Резервируем сочетание горячих клавиш Ctrl+Alt+Enter, для скрытия/открытия доп. настроек

bool RHKret = RegisterHotKey(FormSettADC->Handle,         // Handle окна, которому отправлять сообщения WM_HOTKEY
								0x00F,                 // УСЛОВНЫЙ идентификатор горячего ключа
								MOD_ALT + MOD_CONTROL, // модификаторы
								VK_RETURN              // код клавиши
								);

	//при активации прячем доп. настройки
	Edit1->Enabled=false;
	Edit2->Enabled=false;
	Edit3->Enabled=false;
	EditKadr->Enabled=false;
	EditRate->Enabled=false;
	EditSizeBuf->Enabled=false;
	TabSheet3->Enabled=false;
//Резервируем сочетание горячих клавиш Ctrl+Alt+Enter, для скрытия/открытия доп. настроек


}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormDestroy(TObject *Sender)
{

	delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TFormSettADC::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27) FormSettADC->Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormSettADC::Button4Click(TObject *Sender)
{
	//Пишем данные по датчикам контроля магнитного поля,
	Ini->WriteInteger("OtherSettings","Temp1",SpinTemp1->Value);
	Ini->WriteInteger("OtherSettings","Temp2",SpinTemp2->Value);
	Ini->WriteInteger("OtherSettings","Magnetic",SpinMagnetic->Value);

	Ini->WriteInteger("OtherSettings","InKataphot",SpinKataphot1->Value);
	Ini->WriteInteger("OtherSettings","OutKataphot",SpinKataphot2->Value);
	Ini->WriteInteger("OtherSettings","SensorSG",SpinCurr->Value);
	Ini->WriteInteger("OtherSettings","SensorCurr",SpinSG->Value);

	FormSettADC->Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormSettADC::FormClose(TObject *Sender, TCloseAction &Action)
{
	UnregisterHotKey(FormSettADC->Handle, // Handle окна
					0x00F          // наш идентификатор горячего ключа
					);
}
//---------------------------------------------------------------------------

