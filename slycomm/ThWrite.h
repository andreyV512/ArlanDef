//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef ThWriteH
#define ThWriteH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "CommPort.h"

//---------------------------------------------------------------------------
class TWriteThread : public TThread
{
private:
protected:
    HANDLE FComHandle;
    TCommPort *FComPort;

public:
    OVERLAPPED WOL;
    __fastcall TWriteThread(TCommPort *ComPort );
    __fastcall ~TWriteThread(void);
    void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
