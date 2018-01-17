// ---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include <Classes.hpp>
#include "clustering.h"
// ---------------------------------------------------------------------------
enum SGSource
{
	sgsLCard=0,sgsCOMPort=1,sgsNone=-1
};
// ! Класс глобальных переменных
class Globals
{
public:
	// ! @todo Удалять, переводить все в отдельный класс TypeSize, как в толщиномере
	// ! @brief Текущий типоразмер (может быть любым словом: название, удобное юзерам)
	static String current_typesize;
	// ! Текущий диаметр (строго 60,73 или 89)
	static int current_diameter;
	// ! Разрешение на просмотр
	static bool isView;
	// ! Имя и путь к файлу с настройками
	static String IniFileName;
	// ! Имя и путь к файлу с сигналами цифровыми
	static String SignalsFileName;
	// ! Путь к файлу с порогами для гп с ms-dos
	static String PathFile_threshold_sg;
	// ! Номер трубы
	static AnsiString tube_number;
	// ! Номер трубы
	static bool tube_transit;
	// ! Труба с характеристиками группы прочности
	static SolidGroups::Tube tubeSG;

	// ! @deprecated устарело, нужно все переводить на векторы
	// ! @brief Макс кол-во датчиков, устарело наверное, нужно выкуривать
	static const int max_sensors=16;
	// ! @deprecated устарело, нужно все переводить на векторы
	// ! @brief Макс кол-во зон, устарело, выкуривать
	static const int max_zones=60;
	// ! @deprecated устарело, нужно все переводить на векторы
	// ! @brief Макс кол-во измерений в зоне, надо выкуривать
	static const int max_meas=10000;
	// ! Число датчиков продольного контроля в проекте
	static const int LinSensors=4;   //Щукин 20160427 теперь будет 4
	// ! Максимальное число датчиков поперечного контроля в проекте
	static const int Cross_sensors=12;
	// ! параметр частотника в котором хранится скорость врщения
	static const int defaultRotParameter=4;
private:

};

String Globals::current_typesize=L"2";
int Globals::current_diameter=2;
bool Globals::isView=false;
String Globals::PathFile_threshold_sg="..\\..\\Settings\\";
String Globals::IniFileName="..\\..\\Settings\\SettingsDefectoscope.ini";
String Globals::SignalsFileName="..\\..\\Settings\\IO_Management.ini";
AnsiString Globals::tube_number="";
bool Globals::tube_transit;
SolidGroups::Tube Globals::tubeSG=SolidGroups::Tube();

// -------------------------------------------------------------------------------------------
// ! Класс констант, относящихся к особенностям проекта
class SystemConst
{
public:
	// ! Работа с медианным фильтром
	static bool isMedianFilter;
	// ! Обмен с АСУ цеха по COM порту
	static bool ComWithASU;
	// ! Источник группы прочности
	static SGSource SolidGroupSource;
	// !	Прятать ли усиления каналов и отключение модулей
	static bool HideInGain;
	// ! проект с ЛИРами или без
	static bool IsOnline;
	// ! Считаем группу прочности с помощью проги ms-dos'овской
	static bool isSolidGroupMS_DOS;
	// ! Использовать ли датчик температуры при определении ГП
	static bool isTempSensor;
	// ! Есть ли вообще группа прочности
	static bool isSolidity;
	// ! Есть ли у продольного соленоид (котнроль тока, напряжения, сопротивления)
	static bool isLinearSolenoid;
	// ! Будем ли фильтровать сигнал поперечного цифровым фильтром
	static bool isCrossDigitalFilter;
	// ! Будем ли фильтровать сигнал продольного цифровым фильтром
	static bool isLinearDigitalFilter;

	// ! Установим постоянные характеристики проекта
	static void SetSystemConstants();

};
bool SystemConst::isCrossDigitalFilter=true;
bool SystemConst::isLinearDigitalFilter=true;
bool SystemConst::isMedianFilter=false;
bool SystemConst::ComWithASU=false;
SGSource SystemConst::SolidGroupSource=sgsNone;
bool SystemConst::HideInGain=false;
bool SystemConst::IsOnline=true;
bool SystemConst::isSolidGroupMS_DOS=false;
bool SystemConst::isTempSensor=false;
bool SystemConst::isSolidity=false;
bool SystemConst::isLinearSolenoid=false;

// ---------------------------------------------------------------------------
#endif
