#pragma once
////#include <stdio.h>
//#include <time.h>

#undef MDEBUG
#undef  NO_PCL1730
#define WNET_FOLDER
#define ACY

#if defined(MDEBUG)
class Debug
{
public:
	static FILE *file;
public:
	Debug(void)
	{
		char tmpbuf[128];
		fopen_s(&file, "DEBUG.txt", "a");
		_strdate_s( tmpbuf, 128 );
		fprintf(file, "OS date:\t\t\t\t%s\n", tmpbuf );
		_strtime_s( tmpbuf, 128 );
		fprintf(file, "OS time:\t\t\t\t%s\n", tmpbuf );
	}
	~Debug(void)
	{
		fclose(file);
	}
};
#endif

///////////////////////////////////////////////////////////////////
#define SCREEN
////////////////////////////////////////////////////////////////////

void FormatError();
