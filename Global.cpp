#pragma hdrstop
#include "Global.h"
#include <IniFiles.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
void SystemConst::SetSystemConstants()
{
	isSolidity=true;
	isLinearSolenoid=false;

	// читаем параметры из файла
	TIniFile *ini=new TIniFile(Globals::IniFileName);
	isSolidGroupMS_DOS=ini->ReadBool("OtherSettings","IsCalcMS_DOS",1);
	ComWithASU=ini->ReadBool("OtherSettings","ComWithASU",0);
	SolidGroupSource=ini->ReadInteger("OtherSettings","SolidGroupSource",-1);
	isMedianFilter=ini->ReadBool("OtherSettings","isMedianFilter",0);
	isTempSensor=ini->ReadBool("OtherSettings","isTempSensor",0);

	isLinearDigitalFilter=ini->ReadBool("Filters","isFilterPr",1);
	isCrossDigitalFilter=ini->ReadBool("Filters","isFilterPp",1);
	delete ini;
}
// ---------------------------------------------------------------------------
