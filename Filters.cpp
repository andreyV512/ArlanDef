//---------------------------------------------------------------------------
#pragma hdrstop
#include "Series.hpp"
#include "Filters.h"
#include "Global.h"
//#include "Main.h"

//------------------------------------------------------------------------------
Filters *LinearFilter;
Filters *CrossFilter;
Filters::Filters()
{
	dllInstance = LoadLibrary(L"filters.dll");

	if ( !dllInstance )
	{
		String strError = L"Не удалось загрузить библиотеку фильтров";
		MessageBox( NULL, strError.w_str(), L"Ошибка", MB_ICONERROR | MB_OK );
	}
	else
	{
		filter_chebyshev  = (_ChebyshevI) GetProcAddress( dllInstance, "ChebyshevI" );
		filter_butterworth  =  (_Butterworth) GetProcAddress( dllInstance, "Butterworth");
		filter_elliptic=  (_Elliptic) GetProcAddress( dllInstance, "Elliptic");
		setSettingsFromIniFile("Pp");
	}
}
void Filters::toFilter(double *data, int N)
{
	switch(settings.CurrentType)
	{
		case 0:
		{
			filter_butterworth(data,
							   N,
							   settings.order,
							   settings.sampleRate,
							   settings.cutoffFrequency,
							   settings.centerFrequency,
							   settings.widthFrequency,
							   settings.CurrentSubType);
			break;
		}
		case 1:
		{
			filter_chebyshev(data,
							N,
							settings.order,
							settings.sampleRate,
							settings.cutoffFrequency,
							settings.centerFrequency,
							settings.widthFrequency,
							settings.rippleDb,
							settings.CurrentSubType);
			break;
		}
		case 2:
		{
			filter_elliptic (data,
							N,
							settings.order,
							settings.sampleRate,
							settings.cutoffFrequency,
							settings.centerFrequency,
							settings.widthFrequency,
							settings.rippleDb,
							settings.rolloff,
							settings.CurrentSubType);
			break;
		}
    }
}
void Filters::setSettingsFromIniFile(String type)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);

	settings.CurrentType 	    = ini->ReadInteger("Filters","FilterType"+type, 0);
	settings.CurrentSubType     = ini->ReadInteger("Filters","FilterSubType"+type, 0);
	settings.order 				= ini->ReadInteger("Filters","order"+type, 2);
	settings.cutoffFrequency	= ini->ReadFloat("Filters","cutoffFrequency"+type, 300.0);
	settings.widthFrequency 	= ini->ReadFloat("Filters","widthFrequency"+type, 300.0);
	settings.centerFrequency 	= ini->ReadFloat("Filters","centerFrequency"+type, 300.0);
	settings.rippleDb 			= ini->ReadFloat("Filters","rippleDb"+type, 0.3);
	settings.rolloff 			= ini->ReadFloat("Filters","rolloff"+type, 0.3);
	settings.sampleRate 		= ini->ReadFloat("LCard", "Rate",1);

	delete ini;
}

 #pragma package(smart_init)
