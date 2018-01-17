//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThStatus.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TStatusThread::TStatusThread(TCommPort *ComPort, TComEventType Events) : TThread(false)
{
  DWORD EvList[9] = {EV_RXCHAR,
                     EV_RXFLAG,
                     EV_TXEMPTY,
                     EV_CTS,
                     EV_DSR,
                     EV_RLSD,
                     EV_BREAK,
                     EV_ERR,
                     EV_RING };

  FComPort = ComPort;
  FComHandle = ComPort->ComHandle;
  DWORD AttrWord = 0;

  for( int i=0; i < 9; i++ ){
    TEventState EvIndex = (TEventState)i;
    if( Events.Contains(EvIndex) )  AttrWord |= EvList[i];
  }

  SetCommMask( FComPort->ComHandle, AttrWord );

  ZeroMemory( &SOL, sizeof(SOL) );
  SOL.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
}

//---------------------------------------------------------------------------
__fastcall TStatusThread::~TStatusThread(void)
{
  CloseHandle( SOL.hEvent );
}

//---------------------------------------------------------------------------
void __fastcall TStatusThread::Execute()
{
  DWORD lpModemStatus;

  if( FComPort->IsEnabled() && GetCommModemStatus( FComHandle, &lpModemStatus) ){
    FComPort->FCTS  = (lpModemStatus & MS_CTS_ON);
    FComPort->FDSR  = (lpModemStatus & MS_DSR_ON);
    FComPort->FDCD  = (lpModemStatus & MS_RLSD_ON);
    FComPort->FRing = (lpModemStatus & MS_RING_ON);
    FComPort->Status = 0;
  }

  while( !Terminated ){
    if( !FComPort->IsEnabled() )  continue;
    DWORD Status;
    bool OK = WaitCommEvent(FComHandle, &Status, &SOL);
    if( !OK ){
      Sleep( 50 );
      if( GetLastError() == ERROR_IO_PENDING ){
        DWORD Temp;
        OK = GetOverlappedResult(FComHandle, &SOL, &Temp, TRUE );
        if( OK )  ResetEvent(SOL.hEvent);
      }
    }

    if( OK ){
      FComPort->Status = Status;

      if( (Status & (EV_CTS | EV_DSR | EV_RING | EV_RLSD | EV_ERR)) != 0 &&
        GetCommModemStatus(FComHandle, &lpModemStatus) ){
        FComPort->FCTS = (lpModemStatus & MS_CTS_ON);
        FComPort->FDSR = (lpModemStatus & MS_DSR_ON);
        FComPort->FDCD = (lpModemStatus & MS_RLSD_ON);
        FComPort->FRing = (lpModemStatus & MS_RING_ON);

        if( FComPort->Control == ccHard &&
            FComPort->FCTS &&
            FComPort->OBuffUsed > 0 )  SetEvent(FComPort->wtEvent);
      }

      if( (Status & EV_RXCHAR) != 0 )  SetEvent(FComPort->rtEvent);

      SetEvent(FComPort->seEvent);
    }
  }
}

//---------------------------------------------------------------------------

