// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Solenoid.h"
#include <IniFiles.hpp>
#include "Global.h"
#include "LCARD502.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
Solenoid *CrossSolenoid = NULL;
Solenoid *LinearSolenoid = NULL;

Solenoid::Solenoid(String TypeOfControl)
{
	// typeOfControl - PP или PR
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	if (TypeOfControl == "PP")
	{
		chAmperage = lcard->getSettings().channels.size();
		chVoltage = lcard->getSettings().channels.size() + 1;
		AlarmLevel = ini->ReadFloat("PP", "ResistSolenoid", 90.0) * 1.4;
		AlarmULevel = ini->ReadFloat("PP", "VoltageSolenoid", 270);
		DifULevel = ini->ReadFloat("PP", "DifVoltageSolenoid", 20);
	}
	else
	{
		chAmperage = lcard->getSettings().channels.size() + 2;
		chVoltage = lcard->getSettings().channels.size() + 3;
		AlarmLevel = ini->ReadFloat("PR", "ResistSolenoid", 90.0) * 1.4;
		AlarmULevel = ini->ReadFloat("PR", "VoltageSolenoid", 270);
		DifULevel = ini->ReadFloat("PR", "DifVoltageSolenoid", 20);
	}
	Devider = ini->ReadFloat(TypeOfControl, "Devider", 72.0);

	delete ini;
}

// ---------------------------------------------------------------------------
double Solenoid::getAmperage()
{
	vector<double>Vec = lcard->getFrameValue();
	double I = Vec[chAmperage];
	return I = (I - 2.5) * 10;
}

double Solenoid::getVoltage()
{
	vector<double>Vec = lcard->getFrameValue();
	double U = Vec[chVoltage];
	U = U * Devider;
	return U;
}

double Solenoid::getResist()
{
	double i = getAmperage();
	double u = getVoltage();
	double R = (double)(u / i);
	return R;
}

bool Solenoid::OkU()
{
	bool pu = (
		(getVoltage() < DifULevel + AlarmULevel)
		&& (getVoltage() >AlarmULevel - DifULevel)
		);
	return pu;
}
bool Solenoid::OkResist()
{
	double t = getResist();
	bool p = (t < AlarmLevel);
	return p;
}

AnsiString Solenoid::GetUIR(void)
{
	double i = getAmperage();
	double u = getVoltage();
	double R = (double)(u / i);
	AnsiString ret;
	ret.printf("U=%.3lf I=%.3lf R=%.3lf", u, i, R);
	return (ret);
}
