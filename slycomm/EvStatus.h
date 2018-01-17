//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef EvStatusH
#define EvStatusH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "CommPort.h"

//---------------------------------------------------------------------------
class TStatusEventThread : public TThread
{
private:
protected:
    TCommPort *FComPort;
    void __fastcall DoOnSignal(void);

public:
    __fastcall TStatusEventThread(TCommPort *ComPort);
    void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
