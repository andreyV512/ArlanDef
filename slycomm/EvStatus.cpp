//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EvStatus.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TStatusEventThread::TStatusEventThread(TCommPort *ComPort) : TThread(false)
{
  FComPort = ComPort;
}

//---------------------------------------------------------------------------
void __fastcall TStatusEventThread::Execute()
{
  while( !Terminated ){
    if( WaitForSingleObject(FComPort->seEvent, INFINITE) != WAIT_OBJECT_0 )  continue;
    Synchronize( DoOnSignal );
    // DoOnSignal();
  }
}

//---------------------------------------------------------------------------
void __fastcall TStatusEventThread::DoOnSignal(void)
{
  DWORD Status = FComPort->Status;
  if( Status & EV_CTS     )  if( FComPort->OnCTSSignal     )  FComPort->OnCTSSignal(FComPort);
  if( Status & EV_RLSD    )  if( FComPort->OnDCDSignal     )  FComPort->OnDCDSignal(FComPort);
  if( Status & EV_DSR     )  if( FComPort->OnDSRSignal     )  FComPort->OnDSRSignal(FComPort);
  if( Status & EV_RING    )  if( FComPort->OnRingSignal    )  FComPort->OnRingSignal(FComPort);
  if( Status & EV_RXCHAR  )  if( FComPort->OnRxDSignal     )  FComPort->OnRxDSignal(FComPort);
  if( Status & EV_RXFLAG  )  if( FComPort->OnRxEventSignal )  FComPort->OnRxEventSignal(FComPort);
  if( Status & EV_TXEMPTY )  if( FComPort->OnTxDSignal     )  FComPort->OnTxDSignal(FComPort);
  if( Status & EV_BREAK   )  if( FComPort->OnBreakSignal   )  FComPort->OnBreakSignal(FComPort);
  if( Status & EV_ERR     )  if( FComPort->OnErrorSignal   )  FComPort->OnErrorSignal(FComPort, GetLastError() );

/*
Errorflags for OnErrorSignal

 CE_BREAK       The hardware detected a break condition.
 CE_DNS	        Windows 95 only: A parallel device is not selected.
 CE_FRAME       The hardware detected a framing error.
 CE_IOE	        An I/O error occurred during communications with the device.
 CE_MODE        The requested mode is not supported, or the hFile parameter
                is invalid. If this value is specified, it is the only valid error.
 CE_OOP	        Windows 95 only: A parallel device signaled that it is out of paper.
 CE_OVERRUN     A character-buffer overrun has occurred. The next character is lost.
 CE_PTO	        Windows 95 only: A time-out occurred on a parallel device.
 CE_RXOVER      An input buffer overflow has occurred. There is either no
                room in the input buffer, or a character was received after
                the end-of-file (EOF) character.
 CE_RXPARITY    The hardware detected a parity error.
 CE_TXFULL      The application tried to transmit a character, but the output
                buffer was full.
 */
}

//---------------------------------------------------------------------------

