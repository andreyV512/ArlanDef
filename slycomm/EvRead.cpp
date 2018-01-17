//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EvRead.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TReadEventThread::TReadEventThread(TCommPort *ComPort) : TThread(false)
{
  FComPort = ComPort;
}

//---------------------------------------------------------------------------
void __fastcall TReadEventThread::Execute()
{
  while( !Terminated ){
    if( WaitForSingleObject(FComPort->reEvent, INFINITE) != WAIT_OBJECT_0 )  continue;
    Synchronize( DoOnReceived );
    // DoOnReceived();
  }
}

//---------------------------------------------------------------------------
void __fastcall TReadEventThread::DoOnReceived(void)
{
  if( FComPort->OnDataReceived )
      FComPort->OnDataReceived( FComPort, FComPort->IBuffUsed );
}

//---------------------------------------------------------------------------

