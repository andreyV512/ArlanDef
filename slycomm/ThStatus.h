//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef ThStatusH
#define ThStatusH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "CommPort.h"

//---------------------------------------------------------------------------
class TStatusThread : public TThread
{
private:
    TCommPort *FComPort;
    HANDLE FComHandle;

protected:
    void __fastcall Execute();

public:
    OVERLAPPED SOL;
    __fastcall TStatusThread(TCommPort *ComPort, TComEventType Events );
    __fastcall ~TStatusThread(void);
};

//---------------------------------------------------------------------------
#endif
