//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitry Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CommPort.h"
#pragma package(smart_init)

#pragma resource "*.res"

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCommPort *)
{
        new TCommPort(NULL);
}

//---------------------------------------------------------------------------
namespace Commport
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TCommPort)};
                 RegisterComponents("SlyDiman", classes, 0);
        }
}

//---------------------------------------------------------------------------
__fastcall TCommPort::TCommPort(TComponent* Owner) : TComponent(Owner)
{
  FComHandle = INVALID_HANDLE_VALUE;
  FComNumber = 2;
  FBaudRate = cbr9600;
  FParity = paNone;
  FStopBits = sb1_0;
  FDataBits = 8;
  FComControl = ccNone;
  FMonitorEvents << evBreak << evCTS << evDSR << evError << evRing
                 << evRlsd << evRxChar << evTxEmpty;

  IBuffSize = 2048;
  OBuffSize = 4096;
  IBuffUsed = 0;
  OBuffUsed = 0;

  FRTS = false;
  FDTR = false;

  FXonLim = 0;
  FXoffLim = 1024;

  FXonChar = 0x11;
  FXoffChar = 0x13;

  FEventChar = 0;

  InitializeCriticalSection(&WriteSection);
  InitializeCriticalSection(&ReadSection);
  rtEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  wtEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  reEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  seEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  Status = 0;
}

//---------------------------------------------------------------------------
__fastcall TCommPort::~TCommPort(void)
{
  ClosePort();
  CloseHandle(seEvent);
  CloseHandle(reEvent);
  CloseHandle(wtEvent);
  CloseHandle(rtEvent);
  DeleteCriticalSection(&ReadSection);
  DeleteCriticalSection(&WriteSection);
}

//---------------------------------------------------------------------------
bool __fastcall TCommPort::IsEnabled(void)
{
  return( FComHandle != INVALID_HANDLE_VALUE );
}

//---------------------------------------------------------------------------
int __fastcall TCommPort::GetOutQueCount(void)
{

  DWORD lpErrors;
  COMSTAT lpStat;

  int Result = 0;
  if( IsEnabled() ){
    ClearCommError( FComHandle, &lpErrors, &lpStat );
    Result = lpStat.cbOutQue;
  }
  return(Result);
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::OpenPort(void)
{
  if( FOpen )  return;

  IBuffUsed = 0;
  OBuffUsed = 0;

  OBuffer = new unsigned char[OBuffSize];
  IBuffer = new unsigned char[IBuffSize];
  SetLastError(0); //remove any pending errors

  AnsiString DeviceName = AnsiString("\\\\.\\COM") + IntToStr(FComNumber);
  FComHandle = CreateFile( DeviceName.c_str(),
                           GENERIC_READ | GENERIC_WRITE,
                           0, NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
  if( IsEnabled() ){
    COMMTIMEOUTS CommTimeOuts;
    SetupComm( FComHandle, IBuffSize, OBuffSize );

    ZeroMemory( &CommTimeOuts, sizeof(COMMTIMEOUTS) );
//    CommTimeouts.ReadIntervalTimeout = MAXDWORD;
    SetCommTimeouts( FComHandle, &CommTimeOuts );

    SetComControl(FComControl);
    SetBaudRate(FBaudRate);
    SetParity(FParity);
    SetStopBits(FStopBits);
    SetDataBits(FDataBits);
    SetXonLim(FXonLim);
    SetXoffLim(FXoffLim);
    SetXonChar(FXonChar);
    SetXoffChar(FXoffChar);
    SetEventChar(FEventChar);

    StatusThread = new TStatusThread( this, FMonitorEvents );

    ResetEvent(rtEvent);
    ReadThread = new TReadThread( this );

    ResetEvent(wtEvent);
    WriteThread = new TWriteThread( this );

    ResetEvent(reEvent);
    ReadEventThread = new TReadEventThread( this );

    ResetEvent(seEvent);
    StatusEventThread = new TStatusEventThread( this );

    FOpen = true;

    SetDTR(FDTR);
    SetRTS(FRTS);

    if( FOnOpen )  FOnOpen( this, GetLastError() );
  }else{
	Application->MessageBox(L"Can't open COM port", L"COM port error", MB_OK | MB_ICONSTOP );
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::ClosePort(void)
{
  if( !FOpen )   return;

    FCTS  = False;
    FDSR  = False;
    FDCD  = False;
    FRing = False;

    StatusThread->Terminate();
    ResetEvent(StatusThread->SOL.hEvent);
    SetCommMask(FComHandle, 0);
    StatusThread->WaitFor();
    delete StatusThread;
    StatusThread = NULL;

    ReadThread->Terminate();
    ResetEvent(ReadThread->ROL.hEvent);
    SetEvent(rtEvent);
    ReadThread->WaitFor();
    delete ReadThread;
    ReadThread = NULL;

    WriteThread->Terminate();
    ResetEvent(WriteThread->WOL.hEvent);
    SetEvent(wtEvent);
    WriteThread->WaitFor();
    delete WriteThread;
    WriteThread = NULL;

    ReadEventThread->Terminate();
    SetEvent(reEvent);
    ReadEventThread->WaitFor();
    delete ReadEventThread;
    ReadEventThread = NULL;

    StatusEventThread->Terminate();
    SetEvent(seEvent);
    StatusEventThread->WaitFor();
    delete StatusEventThread;
    StatusEventThread = NULL;

    CloseHandle(FComHandle);
    FComHandle = INVALID_HANDLE_VALUE;

    delete IBuffer;
    delete OBuffer;
    IBuffer = NULL;
    OBuffer = NULL;
    IBuffUsed = 0;
    OBuffUsed = 0;

    FOpen = false;

  if( FOnClose )  FOnClose( this );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetComNumber( int n )
{
  Open = false;
  FComNumber = n;
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetIBuffSize( DWORD _size )
{
  bool tmp = Open;
  Open = false;
  IBuffSize = _size;
  Open = tmp;
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetOBuffSize( DWORD _size )
{
  bool tmp = Open;
  Open = false;
  OBuffSize = _size;
  Open = tmp;
}

//---------------------------------------------------------------------------
DWORD __fastcall TCommPort::GetInBuffUsed(void)
{
  return( IBuffUsed );
}

//---------------------------------------------------------------------------
DWORD __fastcall TCommPort::GetInBuffFree(void)
{
  EnterCriticalSection( &ReadSection );
  int Result = IBuffSize - IBuffUsed;
  LeaveCriticalSection( &ReadSection );
  return( Result );
}

//---------------------------------------------------------------------------
DWORD __fastcall TCommPort::GetOutBuffUsed(void)
{
  return( OBuffUsed );
}

//---------------------------------------------------------------------------
DWORD __fastcall TCommPort::GetOutBuffFree(void)
{
  EnterCriticalSection( &WriteSection );
  int Result = OBuffSize - OBuffUsed;
  LeaveCriticalSection( &WriteSection );
  return( Result );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetOpen( bool o )
{
  if( FOpen == o )  return;

  if( o )
    OpenPort();
  else
    ClosePort();
}

//---------------------------------------------------------------------------
bool __fastcall TCommPort::CharReady(void)
{
  return( IBuffUsed > 0 );
}

//---------------------------------------------------------------------------
char __fastcall TCommPort::GetChar(void)
{
  char Result = 0;
  if( IBuffUsed > 0 )  GetBlock( &Result, 1 );
  return( Result );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::PutChar( char ch)
{
  PutBlock( &ch, 1 );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::PutString( char *s )
{
  int l = strlen(s);
  if( l > 0 )  PutBlock( s, l );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SendBreak( WORD Ticks )
{
  if( IsEnabled() )  SetBreak(true);
  Sleep( Ticks * 55 );
  if( IsEnabled() )  SetBreak(false);
}

//---------------------------------------------------------------------------
int __fastcall TCommPort::PutBlock( char *Buf, DWORD Count )
{
  if( !IsEnabled() )  return(0);

  EnterCriticalSection(&WriteSection);
  DWORD used = OBuffUsed;
  DWORD pos1 = OBuffPos;
  LeaveCriticalSection(&WriteSection);

  DWORD pos2 = pos1 + used;

  DWORD size1;
  DWORD size2 = 0;

  if( pos2 < OBuffSize ){
    size1 = OBuffSize - pos2;
    if( size1 > Count ){
      size1 = Count;
    }else{
      Count -= size1;
      size2 = pos1;
      if( size2 > Count )  size2 = Count;
    }
  }else{
    pos2 -= OBuffSize;
    size1 = pos1 - pos2;
    if( size1 > Count )  size1 = Count;
  }

  if( size1 > 0 )  CopyMemory( &OBuffer[pos2], Buf, size1 );
  if( size2 > 0 )  CopyMemory( OBuffer, &Buf[size1], size2 );

  DWORD Result = size1 + size2;

  OBuffUsed += Result;

  if( Result )  SetEvent(wtEvent);

  return( Result );
}

//---------------------------------------------------------------------------
int __fastcall TCommPort::GetBlock( char *Buf, DWORD Count )
{
  if( !IsEnabled() )  return(0);

  if( IBuffUsed == 0 )  return(0);

  EnterCriticalSection(&ReadSection);
  DWORD used = IBuffUsed;
  DWORD pos1 = IBuffPos;
  LeaveCriticalSection(&ReadSection);

  DWORD pos2 = pos1 + used;

  DWORD size1;
  DWORD size2 = 0;

  if( pos2 < IBuffSize ){
    size1 = pos2 - pos1;
    if( size1 > Count )  size1 = Count;
  }else{
    pos2 -= IBuffSize;
    size1 = IBuffSize - pos1;
    if( size1 > Count ){
      size1 = Count;
    }else{
      Count -= size1;
      size2 = pos2;
      if( size2 > Count )  size2 = Count;
    }
  }

  if( size1 > 0 )  CopyMemory( Buf, &IBuffer[pos1], size1 );
  if( size2 > 0 )  CopyMemory( &Buf[size1], IBuffer, size2 );

  DWORD Result = size1 + size2;

  EnterCriticalSection(&ReadSection);
  IBuffUsed -= Result;
  IBuffPos += Result;
  if( IBuffPos >= IBuffSize )  IBuffPos -= IBuffSize;
  LeaveCriticalSection(&ReadSection);

  SetEvent(rtEvent);  // read from port buffer to main buffer

  return( Result );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetComControl( TComControl Value )
{
  FComControl = Value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    DWORD flags = DCB_BINARY;
    switch( FComControl ){
    case ccSoft: flags |= SOFT_FLOW;  break;
    case ccHard: flags |= HARD_FLOW;  break;
    }
    DWORD *tmp = (DWORD *)(&FDCB);
    tmp[2] = flags;
    SetCommState(FComHandle, &FDCB);
  }

}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetXonLim(WORD value)
{
  FXonLim = value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.XonLim = FXonLim;
    SetCommState(FComHandle, &FDCB);
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetXoffLim(WORD value)
{
  DWORD ds = IBuffSize;
  if( ds > 65535 )  ds = 65535;
  WORD ws = (WORD)ds;
  if( value > ws )  value = ws;

  FXoffLim = value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.XoffLim = FXoffLim;
    SetCommState(FComHandle, &FDCB);
  }

}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetXonChar(BYTE value)
{
  FXonChar = value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.XonChar = FXonChar;
    SetCommState(FComHandle, &FDCB);
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetXoffChar(BYTE value)
{
  FXoffChar = value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.XoffChar = FXoffChar;
    SetCommState(FComHandle, &FDCB);
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetEventChar(BYTE value)
{
  FEventChar = value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.EvtChar = FEventChar;
    SetCommState(FComHandle, &FDCB);
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetBaudRate( TBaudRate Value )
{
  int CBR[15] = {CBR_110,
                 CBR_300,
                 CBR_600,
                 CBR_1200,
                 CBR_2400,
                 CBR_4800,
                 CBR_9600,
                 CBR_14400,
                 CBR_19200,
                 CBR_38400,
                 CBR_56000,
                 CBR_57600,
                 CBR_115200,
                 CBR_128000,
                 CBR_256000 };

  FBaudRate = Value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.BaudRate = CBR[FBaudRate];
    SetCommState(FComHandle, &FDCB);
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetParity( TParity Value )
{
  char PAR[5] = {NOPARITY,
                 ODDPARITY,
                 EVENPARITY,
                 MARKPARITY,
                 SPACEPARITY };
  FParity = Value;
  if( IsEnabled() ){
    GetCommState( FComHandle, &FDCB );
    FDCB.Parity = PAR[FParity] ;
    SetCommState( FComHandle, &FDCB );
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetStopBits( TStopBits Value )
{
  char STB[3] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};

  FStopBits = Value;
  if( IsEnabled() ){
    GetCommState(FComHandle, &FDCB);
    FDCB.StopBits = STB[FStopBits];
    SetCommState(FComHandle, &FDCB);
  }

  BitsWarning();
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetDataBits( TDataBits Value )
{
  if( Value < 5 || Value > 8 )  Value = 8;

  FDataBits = Value;
  if( IsEnabled() ){
    GetCommState(FComHandle, &FDCB);
    FDCB.ByteSize = Value;
    SetCommState(FComHandle, &FDCB);
  }

  BitsWarning();
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::BitsWarning(void)
{
  if( FDataBits != 5 ){
    if( FStopBits == sb1_5 )
	  Application->MessageBox(L"Invalid combination (6,7 or 8 data bits with 1.5 stop bits)",
							  L"COM port warning", MB_OK | MB_ICONWARNING );
  }else{
    if( FStopBits == sb2_0 )
	  Application->MessageBox(L"Invalid combination (5 data bits with 2 stop bits)",
							  L"COM port warning", MB_OK | MB_ICONWARNING );
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetBaud( int B )
{
  switch( B ){
    case 110     : BaudRate = cbr110;    break;
    case 300     : BaudRate = cbr300;    break;
    case 600     : BaudRate = cbr600;    break;
    case 1200    : BaudRate = cbr1200;   break;
    case 2400    : BaudRate = cbr2400;   break;
    case 4800    : BaudRate = cbr4800;   break;
    case 9600    : BaudRate = cbr9600;   break;
    case 14400   : BaudRate = cbr14400;  break;
    case 19200   : BaudRate = cbr19200;  break;
    case 38400   : BaudRate = cbr38400;  break;
    case 56000   : BaudRate = cbr56000;  break;
    case 57600   : BaudRate = cbr57600;  break;
    case 115200  : BaudRate = cbr115200; break;
    case 128000  : BaudRate = cbr128000; break;
    case 256000  : BaudRate = cbr256000; break;
  }
}

//---------------------------------------------------------------------------
int __fastcall TCommPort::GetBaud(void)
{
  int result = 0;
  switch( BaudRate ){
    case cbr110     : result = 110;    break;
    case cbr300     : result = 300;    break;
    case cbr600     : result = 600;    break;
    case cbr1200    : result = 1200;   break;
    case cbr2400    : result = 2400;   break;
    case cbr4800    : result = 4800;   break;
    case cbr9600    : result = 9600;   break;
    case cbr14400   : result = 14400;  break;
    case cbr19200   : result = 19200;  break;
    case cbr38400   : result = 38400;  break;
    case cbr56000   : result = 56000;  break;
    case cbr57600   : result = 57600;  break;
    case cbr115200  : result = 115200; break;
    case cbr128000  : result = 128000; break;
    case cbr256000  : result = 256000; break;
  }
  return( result );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::FlushInBuffer(void)
{
  if( IsEnabled() ){
    PurgeComm( FComHandle, PURGE_RXABORT | PURGE_RXCLEAR );
  }
  EnterCriticalSection(&ReadSection);
  IBuffUsed = 0;
  LeaveCriticalSection(&ReadSection);
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::FlushOutBuffer(void)
{
  if( IsEnabled() ){
    PurgeComm( FComHandle, PURGE_TXABORT | PURGE_TXCLEAR );
  }
  EnterCriticalSection(&WriteSection);
  ResetEvent(wtEvent);
  OBuffUsed = 0;
  LeaveCriticalSection(&WriteSection);
}

//---------------------------------------------------------------------------
bool __fastcall TCommPort::GetModemState( int Index )
{
  DWORD Flag, State;

  switch( Index ){
    case 1:  State = MS_CTS_ON;  break;
    case 2:  State = MS_DSR_ON;  break;
    case 3:  State = MS_RING_ON; break;
    case 4:  State = MS_RLSD_ON; break;
    default: State = 0;          break;
  }

  bool Result = false;
  if( IsEnabled() ){
    if( GetCommModemStatus(FComHandle, &Flag) )
      Result = (Flag & State);
  }
  return( Result );
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::PowerOn(void)
{
  EscapeCommFunction( FComHandle, SETDTR );
  EscapeCommFunction( FComHandle, SETRTS );
  FRTS = true;
  FDTR = true;
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::PowerOff(void)
{
  EscapeCommFunction( FComHandle, CLRDTR );
  EscapeCommFunction( FComHandle, CLRRTS );
  FRTS = false;
  FDTR = false;
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetDTR( bool State )
{
  if( FOpen ){
    if( EscapeCommFunction( FComHandle, State ? SETDTR : CLRDTR ) ){
      FDTR = State;
    }
  }else{
    FDTR = State;
  }
}

//---------------------------------------------------------------------------
void __fastcall TCommPort::SetRTS( bool State )
{
  if( FOpen ){
    if( EscapeCommFunction( FComHandle, State ? SETRTS : CLRRTS ) ){
      FRTS = State;
    }
  }else{
    FRTS = State;
  }
}

//---------------------------------------------------------------------------
bool __fastcall TCommPort::SetBreak( bool State )
{
  bool Result = false;
  if( FOpen )
    Result = EscapeCommFunction( FComHandle, State ? SETBREAK : CLRBREAK );

  return( Result );
}

//---------------------------------------------------------------------------
