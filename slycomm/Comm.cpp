//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitry Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

USERES("Comm.res");
USEUNIT("CommPort.cpp");
USEUNIT("ThWrite.cpp");
USEUNIT("EvRead.cpp");
USEUNIT("EvRead.cpp");
USEUNIT("ThStatus.cpp");
USEUNIT("EvStatus.cpp");
USEUNIT("ThRead.cpp");
USEPACKAGE("vcl50.bpi");
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
