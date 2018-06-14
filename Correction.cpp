#include "Correction.h"
#include <IniFiles.hpp>
#include "Global.h"

namespace Correction
{
	double coeff = 1.0;
	bool isEtalon;
	void Init()
	{
     	coeff = 1.0;
        TIniFile *ini = new TIniFile(Globals::IniFileName);
		 int val = ini->ReadInteger( "Correction", "coeff", 0 );
		 delete ini;
		 coeff -= 0.01 * val;
    }
	void Start()
	{
	   coeff = 1.0;

	   if(!isEtalon)
	   {
		 TIniFile *ini = new TIniFile(Globals::IniFileName);
		 int val = ini->ReadInteger( "Correction", "coeff", 0 );
		 delete ini;
		 coeff -= 0.01 * val;
	   }
	}
}
