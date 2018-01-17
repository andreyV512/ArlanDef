//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef ThReadH
#define ThReadH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "CommPort.h"

//---------------------------------------------------------------------------
class TReadThread : public TThread
{
private:
    TCommPort *FComPort;
    HANDLE FComHandle;

protected:
    void __fastcall Execute();

public:
    OVERLAPPED ROL;
    __fastcall TReadThread(TCommPort *ComPort );
    __fastcall ~TReadThread(void);
};

//---------------------------------------------------------------------------
#endif
