//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThRead.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TReadThread::TReadThread(TCommPort *ComPort) : TThread(false)
{
  FComPort = ComPort;
  FComHandle = ComPort->ComHandle;
  ZeroMemory( &ROL, sizeof(ROL) );
  ROL.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
}

//---------------------------------------------------------------------------
__fastcall TReadThread::~TReadThread(void)
{
  CloseHandle( ROL.hEvent );
}

//---------------------------------------------------------------------------
void __fastcall TReadThread::Execute()
{
  while( !Terminated ){
    if( WaitForSingleObject(FComPort->rtEvent, INFINITE) != WAIT_OBJECT_0 )  continue;

    COMSTAT lpStat;
    DWORD lpErrors;

    while( true ){
      ClearCommError( FComHandle, &lpErrors, &lpStat );

      EnterCriticalSection( &FComPort->ReadSection );
      DWORD pos1 = FComPort->IBuffPos;
      DWORD used = FComPort->IBuffUsed;
      LeaveCriticalSection(&FComPort->ReadSection);

      DWORD pos2 = pos1 + used;
      DWORD size;
      if( pos2 < FComPort->IBuffSize ){
        size = FComPort->IBuffSize - pos2;
      }else{
        pos2 -= FComPort->IBuffSize;
        size = pos1 - pos2;
      }
      if( size > lpStat.cbInQue )  size = lpStat.cbInQue;

      if( size == 0 )  break;

      DWORD BytesReaded = 0;
      bool OK = ReadFile( FComHandle, &(FComPort->IBuffer[pos2]), size, &BytesReaded, &ROL);
      if( !OK && GetLastError() == ERROR_IO_PENDING ){
        OK = GetOverlappedResult(FComHandle, &ROL, &BytesReaded, TRUE);
        if( OK )  ResetEvent(ROL.hEvent);
      }
      if( OK && BytesReaded > 0 ){
        FComPort->IBuffUsed += BytesReaded;
        SetEvent(FComPort->reEvent);
      }
    }

  }
}

//---------------------------------------------------------------------------

