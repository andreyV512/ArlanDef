//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef EvReadH
#define EvReadH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "CommPort.h"

//---------------------------------------------------------------------------
class TReadEventThread : public TThread
{
private:
protected:
    TCommPort *FComPort;
    void __fastcall DoOnReceived(void);

public:
    __fastcall TReadEventThread(TCommPort *ComPort);
    void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
