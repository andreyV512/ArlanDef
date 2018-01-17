//---------------------------------------------------------------------------
// Copyright (C) 1999 by Dmitriy Vassiliev
// slydiman@mailru.com
//---------------------------------------------------------------------------

#ifndef CommPortH
#define CommPortH

//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
#define  DCB_BINARY           0x0001  // binary mode, no EOF check
#define  DCB_PARITY           0x0002  // enable parity checking
#define  DCB_OUT_CTS_FLOW     0x0004  // CTS output flow control
#define  DCB_OUT_DSR_FLOW     0x0008  // DSR output flow control

#define  DCB_DTR_DISABLE      0x0000  // DTR flow control type
#define  DCB_DTR_ENABLE       0x0010
#define  DCB_DTR_HANDSHAKE    0x0020

#define  DCB_DSR_SENSITIVITY  0x0040  // DSR sensitivity
#define  DCB_TX_CONT_ON_XOFF  0x0080  // XOFF continues Tx
#define  DCB_OUT_X_CTRL       0x0100  // XON/XOFF out flow control
#define  DCB_IN_X_CTRL        0x0200  // XON/XOFF in flow control
#define  DCB_ERROR_CHAR       0x0400  // enable error replacement
#define  DCB_NULL             0x0800  // enable null stripping

#define  DCB_RTS_DISABLE      0x0000  // RTS flow control
#define  DCB_RTS_ENABLE       0x1000
#define  DCB_RTS_HANDSHAKE    0x2000
#define  DCB_RTS_TOGGLE       0x3000

#define  DCB_ABORT_ON_ERROR   0x4000  // abort reads/writes on error

//----------------------------------
#define  NONE_FLOW   (DCB_DTR_ENABLE    | DCB_RTS_ENABLE)
#define  SOFT_FLOW   (DCB_DTR_ENABLE    | DCB_RTS_ENABLE    | DCB_OUT_X_CTRL   | DCB_IN_X_CTRL)
#define  HARD_FLOW   (DCB_DTR_HANDSHAKE | DCB_RTS_HANDSHAKE | DCB_OUT_CTS_FLOW | DCB_OUT_DSR_FLOW)

//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TOpenEvent  )(System::TObject* Sender, int Error );
typedef void __fastcall (__closure *TErrorEvent )(System::TObject* Sender, int Error );
typedef void __fastcall (__closure *TReadEvent  )(System::TObject* Sender, DWORD Count );

//---------------------------------------------------------------------------
enum TEventState { evRxChar,
                   evRxEventChar,
                   evTxEmpty,
                   evCTS,
                   evDSR,
                   evRlsd,
                   evBreak,
                   evError,
                   evRing };

typedef Set <TEventState, evRxChar, evRing>  TComEventType;

enum TBaudRate  { cbr110,
                  cbr300,
                  cbr600,
                  cbr1200,
                  cbr2400,
                  cbr4800,
                  cbr9600,
                  cbr14400,
                  cbr19200,
                  cbr38400,
                  cbr56000,
                  cbr57600,
                  cbr115200,
                  cbr128000,
                  cbr256000};

enum TParity  { paNone, paOdd, paEven, paMark, paSpace };

enum TStopBits  { sb1_0, sb1_5, sb2_0 };

enum TComControl { ccNone, ccSoft, ccHard };

typedef WORD TDataBits;

//---------------------------------------------------------------------------
class TCommPort;
class TReadThread;
class TWriteThread;
class TStatusThread;
class TReadEventThread;
class TStatusEventThread;
#include "ThRead.h"
#include "ThWrite.h"
#include "ThStatus.h"
#include "EvRead.h"
#include "EvStatus.h"

//---------------------------------------------------------------------------
class PACKAGE TCommPort : public TComponent
{
private:
    HANDLE FComHandle;
    HWND FMsgHandler;
    DCB FDCB;

    int FComNumber;
    TBaudRate FBaudRate;
    TParity FParity;
    TComControl FComControl;
    TStopBits FStopBits;
    TDataBits FDataBits;
    TComEventType FMonitorEvents;

    void __fastcall BitsWarning(void);

    bool __fastcall GetModemState(int Index);

    void __fastcall SetComNumber( int n );
    void __fastcall SetComControl( TComControl Value );
    void __fastcall SetBaudRate( TBaudRate Value );
    void __fastcall SetParity( TParity Value );
    void __fastcall SetStopBits( TStopBits Value );
    void __fastcall SetDataBits( TDataBits Value );

    void __fastcall SetBaud( int B );
    int __fastcall GetBaud(void);

    int __fastcall GetOutQueCount(void);

    void __fastcall SetOBuffSize( DWORD _size );
    DWORD __fastcall GetOutBuffUsed(void);
    DWORD __fastcall GetOutBuffFree(void);

    void __fastcall SetIBuffSize( DWORD _size );
    DWORD __fastcall GetInBuffUsed(void);
    DWORD __fastcall GetInBuffFree(void);

    void __fastcall SetDTR( bool State );
    void __fastcall SetRTS( bool State );

    void __fastcall SetOpen( bool o );

//----------------------------------- Com Config
    WORD FXonLim;
    WORD FXoffLim;
    BYTE FXonChar;
    BYTE FXoffChar;
    BYTE FEventChar;

    void __fastcall SetXonLim(WORD value);
    void __fastcall SetXoffLim(WORD value);
    void __fastcall SetXonChar(BYTE value);
    void __fastcall SetXoffChar(BYTE value);
    void __fastcall SetEventChar(BYTE value);

//----------------------------------- Threads
    TReadThread *ReadThread;
    TWriteThread *WriteThread;
    TStatusThread *StatusThread;    
    TReadEventThread *ReadEventThread;
    TStatusEventThread *StatusEventThread;

//----------------------------------- Events
    TNotifyEvent FOnCTSSignal;
    TNotifyEvent FOnDCDSignal;
    TNotifyEvent FOnDSRSignal;
    TNotifyEvent FOnRxDSignal;
    TNotifyEvent FOnRxEventSignal;
    TNotifyEvent FOnTxDSignal;
    TNotifyEvent FOnRingSignal;
    TNotifyEvent FOnBreakSignal;

    TErrorEvent  FOnErrorSignal;
    TReadEvent   FOnDataReceived;
    TOpenEvent   FOnOpen;
    TNotifyEvent FOnClose;

//------------------------------------------------------------------------------
protected:
    void __fastcall OpenPort(void);
    void __fastcall ClosePort(void);
    bool __fastcall SetBreak( bool State );

//------------------------------------------------------------------------------
public:
    bool FDTR;
    bool FRTS;

    bool FCTS;
    bool FDSR;
    bool FDCD;
    bool FRing;

    DWORD Status;

//----------------------------------- Event Hadles
    HANDLE rtEvent;
    HANDLE wtEvent;
    HANDLE reEvent;
    HANDLE seEvent;
    bool FOpen;

//----------------------------------- Buffers
    unsigned char *OBuffer;
    DWORD OBuffSize;
    DWORD OBuffPos;
    DWORD OBuffUsed;

    unsigned char *IBuffer;
    DWORD IBuffSize;
    DWORD IBuffPos;
    DWORD IBuffUsed;

//-----------------------------------
    _RTL_CRITICAL_SECTION WriteSection;
    _RTL_CRITICAL_SECTION ReadSection;

//-----------------------------------
    bool __fastcall IsEnabled(void);
    int __fastcall PutBlock( char *Buf, DWORD Count );
    int __fastcall GetBlock( char *Buf, DWORD Count );
    void __fastcall FlushInBuffer(void);
    void __fastcall FlushOutBuffer(void);

    bool __fastcall CharReady(void);
    char __fastcall GetChar(void);
    void __fastcall PutChar( char Ch );
    void __fastcall PutString( char *s );
    void __fastcall SendBreak( WORD Ticks );

    void __fastcall PowerOn(void);
    void __fastcall PowerOff(void);

    __property HANDLE ComHandle = {read=FComHandle};

//-----------------------------------
    __fastcall TCommPort(TComponent* Owner);
    __fastcall ~TCommPort(void);

//------------------------------------------------------------------------------
__published:
    __property int ComNumber = {read=FComNumber, write=SetComNumber};
    __property bool Open = {read=FOpen, write=SetOpen};

//----------------------------------- Com Status
    __property bool DTR = {read=FDTR, write=SetDTR};
    __property bool RTS = {read=FRTS, write=SetRTS};

    __property bool CTS = {read=FCTS};
    __property bool DSR = {read=FDSR};
    __property bool DCD = {read=FDCD};
    __property bool Ring = {read=FRing};

//----------------------------------- Com Info
    __property DWORD InSize = {read=IBuffSize, write=SetIBuffSize};
    __property DWORD OutSize = {read=OBuffSize, write=SetOBuffSize};
    __property DWORD InBuffFree = {read=GetInBuffFree};
    __property DWORD InBuffUsed = {read=GetInBuffUsed};
    __property DWORD OutBuffFree = {read=GetOutBuffFree};
    __property DWORD OutBuffUsed = {read=GetOutBuffUsed};
    __property int OutQueCount = {read=GetOutQueCount};

//----------------------------------- Com Config
    __property int Baud = {read=GetBaud, write=SetBaud};
    __property TBaudRate BaudRate = {read=FBaudRate, write=SetBaudRate};
    __property TComControl Control = {read=FComControl, write=SetComControl};
    __property TParity Parity = {read=FParity, write=SetParity};
    __property TStopBits StopBits = {read=FStopBits, write=SetStopBits};
    __property TDataBits DataBits = {read=FDataBits, write=SetDataBits};
    __property TComEventType MonitorEvents = {read=FMonitorEvents, write=FMonitorEvents};

//-----------------------------------
    __property WORD XonLim    = { read = FXonLim, write = SetXonLim };
    __property WORD XoffLim   = { read = FXoffLim, write = SetXoffLim };
    __property BYTE XonChar   = { read = FXonChar, write = SetXonChar };
    __property BYTE XoffChar  = { read = FXoffChar, write = SetXoffChar };
    __property BYTE EventChar = { read = FEventChar, write = SetEventChar };

//------------------------------------ Events
    __property TNotifyEvent OnCTSSignal     = {read=FOnCTSSignal, write=FOnCTSSignal};
    __property TNotifyEvent OnDCDSignal     = {read=FOnDCDSignal, write=FOnDCDSignal};
    __property TNotifyEvent OnDSRSignal     = {read=FOnDSRSignal, write=FOnDSRSignal};
    __property TNotifyEvent OnRxDSignal     = {read=FOnRxDSignal, write=FOnRxDSignal};
    __property TNotifyEvent OnTxDSignal     = {read=FOnTxDSignal, write=FOnTxDSignal};
    __property TNotifyEvent OnRingSignal    = {read=FOnRingSignal, write=FOnRingSignal};
    __property TNotifyEvent OnBreakSignal   = {read=FOnBreakSignal, write=FOnBreakSignal};
    __property TNotifyEvent OnRxEventSignal = {read=FOnRxEventSignal, write=FOnRxEventSignal};

    __property TErrorEvent  OnErrorSignal   = {read=FOnErrorSignal, write=FOnErrorSignal};
    __property TReadEvent   OnDataReceived  = {read=FOnDataReceived, write=FOnDataReceived};
    __property TOpenEvent   OnOpen          = {read=FOnOpen, write=FOnOpen};
    __property TNotifyEvent OnClose         = {read=FOnClose, write=FOnClose};
};

//---------------------------------------------------------------------------
#endif
