#ifndef _ADVANTECH_BIONIC_DAQ_CLASS_LIBRARY
#define _ADVANTECH_BIONIC_DAQ_CLASS_LIBRARY

#include <windows.h>

#if !defined(_BDAQ_NO_NAMESPACE) && defined(__cplusplus)
   #define _BDAQ_BEGIN	namespace Automation { namespace BDaq {
   #define _BDAQ_END		} }
#else
   #define _BDAQ_BEGIN	
   #define _BDAQ_END		
#endif

// **********************************************************
// Bionic DAQ types
// **********************************************************
_BDAQ_BEGIN
   typedef enum tagTerminalBoard {
      WiringBoard = 0,
      PCLD8710,
      PCLD789,
      PCLD8115,
   } TerminalBoard;

   typedef enum tagModuleType {
      DaqAny   = -1,
      DaqGroup = 1,
      DaqDevice,
      DaqAi,
      DaqAo,
      DaqDio,
      DaqCounter,
   } ModuleType;

   typedef enum tagAccessMode {
      ModeRead = 0,
      ModeWrite,
      ModeWriteWithReset,
   } AccessMode;

   typedef enum tagMathIntervalType {
      /* Right boundary definition, define the maximum value state, use the bit 0,1 */
      RightOpenSet        = 0x0, /* No maximum value limitation.  */
      RightClosedBoundary = 0x1,	/* The maximum value is included. */
      RightOpenBoundary   = 0x2, /* The maximum value is not included. */

      /* Left boundary definition, define the minimum value state, used the bit 2, 3 */
      LeftOpenSet        = 0x0,	/* No minimum value limitation. */
      LeftClosedBoundary = 0x4, 	/* The minimum value is included. */
      LeftOpenBoundary   = 0x8,	/* The minimum value is not included */

      /* The signality expression */
      Boundless          = 0x0,  /* Boundless set. (LeftOpenSet | RightOpenSet) */

      /* The combination notation */
      LOSROS = 0x0,	 /* ( LeftOpenSet | RightOpenSet), algebra notation: ( un-limit, max) */
      LOSRCB = 0x1,	 /* ( LeftOpenSet | RightClosedBoundary), algebra notation: ( un-limit, max ] */
      LOSROB = 0x2,	 /* ( LeftOpenSet | RightOpenBoundary), algebra notation: ( un-limit, max) */

      LCBROS = 0x4,	 /* ( LeftClosedBoundary | RightOpenSet), algebra notation: [min, un-limit) */
      LCBRCB = 0x5,	 /* ( LeftClosedBoundary | RightClosedBoundary), algebra notation: [ min, right ] */
      LCBROB = 0x6,	 /* ( LeftClosedBoundary | RightOpenBoundary), algebra notation: [ min, right) */

      LOBROS = 0x8,	 /* ( LeftOpenBoundary | RightOpenSet), algebra notation: ( min, un-limit) */
      LOBRCB = 0x9,	 /* ( LeftOpenBoundary | RightClosedBoundary), algebra notation: ( min, right ] */
      LOBROB = 0xA,	 /* ( LeftOpenBoundary | RightOpenBoundary), algebra notation: ( min, right) */
   } MathIntervalType;

   typedef struct tagMathInterval {
      long   Type; 
      double Min;
      double Max;
   } MathInterval, * PMathInterval;

   typedef enum tagAiChannelType {
      AllSingleEnded = 0,
      AllDifferential,
      AllSeDiffAdj,
      MixedSeDiffAdj,
   } AiChannelType;

   typedef enum tagAiSignalType {
      SingleEnded = 0,
      Differential,
   } AiSignalType;

   typedef enum tagDioPortType {
      PortDi = 0,        // the port number references to a DI port
      PortDo,            // the port number references to a DO port
      PortDio,           // the port number references to a DI port and a DO port
      Port8255A,         // the port number references to a PPI port A mode DIO port.
      Port8255C,         // the port number references to a PPI port C mode DIO port.
      PortIndvdlDio,     // the port number references to a port whose each channel can be configured as in or out.
   } DioPortType;

   typedef enum tagDioPortDir {
      Input   = 0x00,
      LoutHin = 0x0F,
      LinHout = 0xF0,
      Output  = 0xFF,
   } DioPortDir;

   typedef enum tagSamplingMethod {
      EqualTimeSwitch = 0,
      Simultaneous,
   } SamplingMethod;

   typedef enum tagTemperatureDegree {
      Celsius = 0,
      Fahrenheit,
      Rankine,
      Kelvin,
   } TemperatureDegree;

   typedef enum tagBurnoutRetType {
      Current = 0,
      ParticularValue,
      UpLimit,
      LowLimit,
      LastCorrectValue,
   } BurnoutRetType;

   typedef enum tagValueUnit {
      Kilovolt,      /* KV */
      Volt,          /* V  */
      Millivolt,     /* mV */
      Microvolt,     /* uV */
      Kiloampere,    /* KA */
      Ampere,        /* A  */
      Milliampere,   /* mA */
      Microampere,   /* uA */
      CelsiusUnit,   /* Celsius */
   } ValueUnit;

   typedef enum tagValueRange {
      V_OMIT = -1,            /* Unknown when get, ignored when set */
      V_Neg15To15 = 0,        /* +/- 15 V  */
      V_Neg10To10,            /* +/- 10 V  */
      V_Neg5To5,              /* +/- 5 V */
      V_Neg2pt5To2pt5,        /* +/- 2.5 V */
      V_Neg1pt25To1pt25,      /* +/- 1.25 V */
      V_Neg1To1,              /* +/- 1 V */

      V_0To15,                /* 0~15 V  */
      V_0To10,                /* 0~10 V  */
      V_0To5,                 /* 0~5 V */
      V_0To2pt5,              /* 0~2.5 V */
      V_0To1pt25,             /* 0~1.25 V */
      V_0To1,                 /* 0~1 V */

      mV_Neg625To625,         /* +/- 625mV */
      mV_Neg500To500,         /* +/- 500 mV */
      mV_Neg312pt5To312pt5,   /* +/- 312.5 mV */
      mV_Neg200To200,         /* +/- 200 mV */
      mV_Neg150To150,         /* +/- 150 mV */
      mV_Neg100To100,         /* +/- 100 mV */
      mV_Neg50To50,           /* +/- 50 mV */
      mV_Neg30To30,           /* +/- 30 mV */
      mV_Neg20To20,           /* +/- 20 mV */
      mV_Neg15To15,           /* +/- 15 mV  */
      mV_Neg10To10,           /* +/- 10 mV */
      mV_Neg5To5,             /* +/- 5 mV */

      mV_0To625,              /* 0 ~ 625 mV */
      mV_0To500,              /* 0 ~ 500 mV */
      mV_0To150,              /* 0 ~ 150 mV */
      mV_0To100,              /* 0 ~ 100 mV */
      mV_0To50,               /* 0 ~ 50 mV */
      mV_0To20,               /* 0 ~ 20 mV */
      mV_0To15,               /* 0 ~ 15 mV */
      mV_0To10,               /* 0 ~ 10 mV */

      mA_Neg20To20,           /* +/- 20mA */
      mA_0To20,               /* 0 ~ 20 mA */
      mA_4To20,               /* 4 ~ 20 mA */
      mA_0To24,               /* 0 ~ 24 mA */

      /* For USB4702_4704 */
      V_Neg2To2,              /* +/- 2 V */
      V_Neg4To4,              /* +/- 4 V */
      V_Neg20To20,            /* +/- 20 V */

      Jtype_0To760C = 0x8000, /* T/C J type 0~760 'C */
      Ktype_0To1370C,		   /* T/C K type 0~1370 'C */
      Ttype_Neg100To400C,     /* T/C T type -100~400 'C */
      Etype_0To1000C,		   /* T/C E type 0~1000 'C */
      Rtype_500To1750C,	      /* T/C R type 500~1750 'C */
      Stype_500To1750C,	      /* T/C S type 500~1750 'C */
      Btype_500To1800C,	      /* T/C B type 500~1800 'C */

      Pt392_Neg50To150,	      /* Pt392 -50~150 'C  */
      Pt385_Neg200To200,	   /* Pt385 -200~200 'C */
      Pt385_0To400,		      /* Pt385 0~400 'C */
      Pt385_Neg50To150,	      /* Pt385 -50~150 'C */
      Pt385_Neg100To100,      /* Pt385 -100~100 'C */
      Pt385_0To100,		      /* Pt385 0~100 'C  */
      Pt385_0To200,		      /* Pt385 0~200 'C */
      Pt385_0To600,		      /* Pt385 0~600 'C */
      Pt392_Neg100To100,      /* Pt392 -100~100 'C  */
      Pt392_0To100,           /* Pt392 0~100 'C */
      Pt392_0To200,           /* Pt392 0~200 'C */
      Pt392_0To600,           /* Pt392 0~600 'C */
      Pt392_0To400,           /* Pt392 0~400 'C */
      Pt392_Neg200To200,      /* Pt392 -200~200 'C  */
      Pt1000_Neg40To160,      /* Pt1000 -40~160 'C  */

      Balcon500_Neg30To120,   /* Balcon500 -30~120 'C  */

      Ni518_Neg80To100,       /* Ni518 -80~100 'C */
      Ni518_0To100,           /* Ni518 0~100 'C */
      Ni508_0To100,           /* Ni508 0~100 'C */
      Ni508_Neg50To200,       /* Ni508 -50~200 'C */

      Thermistor_3K_0To100,   /* Thermistor 3K 0~100 'C */
      Thermistor_10K_0To100,  /* Thermistor 10K 0~100 'C */

      Jtype_Neg210To1200C,    /* T/C J type -210~1200 'C */
      Ktype_Neg270To1372C,    /* T/C K type -270~1372 'C */
      Ttype_Neg270To400C,     /* T/C T type -270~400 'C */
      Etype_Neg270To1000C,		/* T/C E type -270~1000 'C */
      Rtype_Neg50To1768C,	   /* T/C R type -50~1768 'C */
      Stype_Neg50To1768C,	   /* T/C S type -50~1768 'C */
      Btype_40To1820C,	      /* T/C B type 40~1820 'C */

      Jtype_Neg210To870C,     /* T/C J type -210~870 'C */
      Rtype_0To1768C,	      /* T/C R type 0~1768 'C */
      Stype_0To1768C,	      /* T/C S type 0~1768 'C */

      /* 0xC000 ~ 0xF000 : user customized value range type */
      UserCustomizedVrgStart = 0xC000,
      UserCustomizedVrgEnd = 0xF000,

      /* AO external reference type */
      V_ExternalRefBipolar = 0xF001, /* External reference voltage unipolar */
      V_ExternalRefUnipolar = 0xF002, /* External reference voltage bipolar */
   } ValueRange;

   typedef enum tagSignalPolarity {
      Negative = 0,
      Positive,
   } SignalPolarity;

   typedef enum tagSignalCountingType {
      DownCount = 0,  /* counter value decreases on each clock */
      UpCount,        /* counter value increases on each clock */
      PulseDirection, /* counting direction is determined by two signals, one is clock, the other is direction signal */
      TwoPulse,       /* counting direction is determined by two signals, one is up-counting signal, the other is down-counting signal */
      AbPhaseX1,      /* AB phase, 1x rate up/down counting */
      AbPhaseX2,      /* AB phase, 2x rate up/down counting */
      AbPhaseX4,      /* AB phase, 4x rate up/down counting */
   } SignalCountingType;

   typedef enum tagOutSignalType{
      SignalOutNone = 0,  /* no output or output is 'disabled' */
      ChipDefined,        /* hardware chip defined */
      NegChipDefined,     /* hardware chip defined, negative logical */
      PositivePulse,      /* a low-to-high pulse */
      NegativePulse,      /* a high-to-low pulse */
      ToggledFromLow,     /* the level toggled from low to high */
      ToggledFromHigh,    /* the level toggled from high to low */
   } OutSignalType;

   typedef enum tagCounterCapability {
      Primary = 1,
      InstantEventCount,
      OneShot,
      TimerPulse,
      InstantFreqMeter,
      InstantPwmIn,
      InstantPwmOut,
      SnapCount,
   } CounterCapability;

   typedef enum tagCounterOperationMode {
      C8254_M0 = 0, /*8254 mode 0, interrupt on terminal count */
      C8254_M1,     /*8254 mode 1, hardware retriggerable one-shot */
      C8254_M2,     /*8254 mode 2, rate generator */
      C8254_M3,     /*8254 mode 3, square save mode */
      C8254_M4,     /*8254 mode 4, software triggered strobe */
      C8254_M5,     /*8254 mode 5, hardware triggered strobe */

      C1780_MA,	/* Mode A level & pulse out, Software-Triggered without Hardware Gating */
      C1780_MB,	/* Mode B level & pulse out, Software-Triggered with Level Gating, = 8254_M0 */
      C1780_MC,	/* Mode C level & pulse out, Hardware-triggered strobe level */
      C1780_MD,	/* Mode D level & Pulse out, Rate generate with no hardware gating */
      C1780_ME,	/* Mode E level & pulse out, Rate generator with level Gating */
      C1780_MF,	/* Mode F level & pulse out, Non-retriggerable One-shot (Pulse type = 8254_M1) */
      C1780_MG,	/* Mode G level & pulse out, Software-triggered delayed pulse one-shot */
      C1780_MH,	/* Mode H level & pulse out, Software-triggered delayed pulse one-shot with hardware gating */
      C1780_MI,	/* Mode I level & pulse out, Hardware-triggered delay pulse strobe */
      C1780_MJ,	/* Mode J level & pulse out, Variable Duty Cycle Rate Generator with No Hardware Gating */
      C1780_MK,	/* Mode K level & pulse out, Variable Duty Cycle Rate Generator with Level Gating */
      C1780_ML,	/* Mode L level & pulse out, Hardware-Triggered Delayed Pulse One-Shot */
      C1780_MO,	/* Mode O level & pulse out, Hardware-Triggered Strobe with Edge Disarm */
      C1780_MR,	/* Mode R level & pulse out, Non-Retriggerbale One-Shot with Edge Disarm */
      C1780_MU,	/* Mode U level & pulse out, Hardware-Triggered Delayed Pulse Strobe with Edge Disarm */
      C1780_MX,	/* Mode X level & pulse out, Hardware-Triggered Delayed Pulse One-Shot with Edge Disarm */
   } CounterOperationMode;

   typedef enum tagCounterValueRegister {
      CntLoad,
      CntPreset = CntLoad,
      CntHold,
      CntOverCompare,
      CntUnderCompare,
   } CounterValueRegister;

   typedef enum tagCounterCascadeGroup {
      GroupNone = 0,    /* no cascade*/
      Cnt0Cnt1,	      /* Counter 0 as first, counter 1 as second. */
      Cnt2Cnt3,	      /* Counter 2 as first, counter 3 as second */
      Cnt4Cnt5,	      /* Counter 4 as first, counter 5 as second */
      Cnt6Cnt7,	      /* Counter 6 as first, counter 7 as second */
   } CounterCascadeGroup;

   typedef enum tagFreqMeasureMethod {
      AutoAdaptive = 0, 		   /* Intelligently select the measurement method according to the input signal. */
      CountingPulseBySysTime, 	/* Using system timing clock to calculate the frequency */
      CountingPulseByDevTime,	   /* Using the device timing clock to calculate the frequency */
      PeriodInverse,			      /* Calculate the frequency from the period of the signal */
   } FreqMeasureMethod;

   typedef enum tagActiveSignal {
      ActiveNone = 0,
      RisingEdge,
      FallingEdge,
      BothEdge,
      HighLevel,
      LowLevel,
   } ActiveSignal;

   typedef enum tagTriggerAction {
      ActionNone = 0,   /* No action to take even if the trigger condition is satisfied */
      DelayToStart,     /* Begin to start after the specified time is elapsed if the trigger condition is satisfied */
      DelayToStop,      /* Stop execution after the specified time is elapsed if the trigger condition is satisfied */
   } TriggerAction;

   typedef enum tagSignalPosition {
      InternalSig = 0,
      OnConnector,
      OnAmsi,
   } SignalPosition;

   typedef enum tagSignalDrop {
      SignalNone = 0,      /* No connection */

      /*Internal signal connector*/
      SigInternalClock,        /* Device built-in clock, If the device has several built-in clock, this represent the highest freq one. */
      SigInternal1KHz,         /* Device built-in clock, 1KHz */
      SigInternal10KHz,        /* Device built-in clock, 10KHz */
      SigInternal100KHz,       /* Device built-in clock, 100KHz */
      SigInternal1MHz,         /* Device built-in clock, 1MHz */
      SigInternal10MHz,        /* Device built-in clock, 10MHz */
      SigInternal20MHz,        /* Device built-in clock, 20MHz */
      SigInternal30MHz,        /* Device built-in clock, 30MHz */
      SigInternal40MHz,        /* Device built-in clock, 40MHz */
      SigInternal50MHz,        /* Device built-in clock, 50MHz */
      SigInternal60MHz,        /* Device built-in clock, 60MHz */

      SigDiPatternMatch,       /* When DI pattern match occurred */
      SigDiStatusChange,       /* When DI status change occurred */

      /*Function pin on connector*/
      SigExtAnaClock,          /* Analog clock pin of connector */
      SigExtAnaScanClock,      /* scan clock pin of connector */
      SigExtAnaTrigger,        /* external analog trigger pin of connector */
      SigExtDigClock,          /* digital clock pin of connector */
      SigExtDigTrigger0,       /* external digital trigger 0 pin( or DI start trigger pin) of connector */
      SigExtDigTrigger1,       /* external digital trigger 1 pin( or DI stop trigger pin) of connector  */
      SigExtDigTrigger2,       /* external digital trigger 2 pin( or DO start trigger pin) of connector */
      SigExtDigTrigger3,       /* external digital trigger 3 pin( or DO stop trigger pin) of connector  */
      SigCHFrzDo,              /* Channel freeze DO ports pin */

      /*Signal source or target on the connector*/
      /*AI channel pins*/
      SigAi0,  SigAi1,  SigAi2,  SigAi3,  SigAi4,  SigAi5,  SigAi6,  SigAi7, 
      SigAi8,  SigAi9,  SigAi10, SigAi11, SigAi12, SigAi13, SigAi14, SigAi15,
      SigAi16, SigAi17, SigAi18, SigAi19, SigAi20, SigAi21, SigAi22, SigAi23,
      SigAi24, SigAi25, SigAi26, SigAi27, SigAi28, SigAi29, SigAi30, SigAi31, 
      SigAi32, SigAi33, SigAi34, SigAi35, SigAi36, SigAi37, SigAi38, SigAi39,
      SigAi40, SigAi41, SigAi42, SigAi43, SigAi44, SigAi45, SigAi46, SigAi47,
      SigAi48, SigAi49, SigAi50, SigAi51, SigAi52, SigAi53, SigAi54, SigAi55, 
      SigAi56, SigAi57, SigAi58, SigAi59, SigAi60, SigAi61, SigAi62, SigAi63,

      /*AO channel pins*/
      SigAo0,  SigAo1,  SigAo2,  SigAo3,  SigAo4,  SigAo5,  SigAo6,  SigAo7,
      SigAo8,  SigAo9,  SigAo10, SigAo11, SigAo12, SigAo13, SigAo14, SigAo15,
      SigAo16, SigAo17, SigAo18, SigAo19, SigAo20, SigAo21, SigAo22, SigAo23,
      SigAo24, SigAo25, SigAo26, SigAo27, SigAo28, SigAo29, SigAo30, SigAo31,

      /*DI pins*/
      SigDi0,   SigDi1,   SigDi2,   SigDi3,   SigDi4,   SigDi5,   SigDi6,   SigDi7,
      SigDi8,   SigDi9,   SigDi10,  SigDi11,  SigDi12,  SigDi13,  SigDi14,  SigDi15,
      SigDi16,  SigDi17,  SigDi18,  SigDi19,  SigDi20,  SigDi21,  SigDi22,  SigDi23,
      SigDi24,  SigDi25,  SigDi26,  SigDi27,  SigDi28,  SigDi29,  SigDi30,  SigDi31,
      SigDi32,  SigDi33,  SigDi34,  SigDi35,  SigDi36,  SigDi37,  SigDi38,  SigDi39,
      SigDi40,  SigDi41,  SigDi42,  SigDi43,  SigDi44,  SigDi45,  SigDi46,  SigDi47,
      SigDi48,  SigDi49,  SigDi50,  SigDi51,  SigDi52,  SigDi53,  SigDi54,  SigDi55,
      SigDi56,  SigDi57,  SigDi58,  SigDi59,  SigDi60,  SigDi61,  SigDi62,  SigDi63,
      SigDi64,  SigDi65,  SigDi66,  SigDi67,  SigDi68,  SigDi69,  SigDi70,  SigDi71,
      SigDi72,  SigDi73,  SigDi74,  SigDi75,  SigDi76,  SigDi77,  SigDi78,  SigDi79,
      SigDi80,  SigDi81,  SigDi82,  SigDi83,  SigDi84,  SigDi85,  SigDi86,  SigDi87,
      SigDi88,  SigDi89,  SigDi90,  SigDi91,  SigDi92,  SigDi93,  SigDi94,  SigDi95,
      SigDi96,  SigDi97,  SigDi98,  SigDi99,  SigDi100, SigDi101, SigDi102, SigDi103,
      SigDi104, SigDi105, SigDi106, SigDi107, SigDi108, SigDi109, SigDi110, SigDi111,
      SigDi112, SigDi113, SigDi114, SigDi115, SigDi116, SigDi117, SigDi118, SigDi119,
      SigDi120, SigDi121, SigDi122, SigDi123, SigDi124, SigDi125, SigDi126, SigDi127,
      SigDi128, SigDi129, SigDi130, SigDi131, SigDi132, SigDi133, SigDi134, SigDi135,
      SigDi136, SigDi137, SigDi138, SigDi139, SigDi140, SigDi141, SigDi142, SigDi143,
      SigDi144, SigDi145, SigDi146, SigDi147, SigDi148, SigDi149, SigDi150, SigDi151,
      SigDi152, SigDi153, SigDi154, SigDi155, SigDi156, SigDi157, SigDi158, SigDi159,
      SigDi160, SigDi161, SigDi162, SigDi163, SigDi164, SigDi165, SigDi166, SigDi167,
      SigDi168, SigDi169, SigDi170, SigDi171, SigDi172, SigDi173, SigDi174, SigDi175,
      SigDi176, SigDi177, SigDi178, SigDi179, SigDi180, SigDi181, SigDi182, SigDi183,
      SigDi184, SigDi185, SigDi186, SigDi187, SigDi188, SigDi189, SigDi190, SigDi191,
      SigDi192, SigDi193, SigDi194, SigDi195, SigDi196, SigDi197, SigDi198, SigDi199,
      SigDi200, SigDi201, SigDi202, SigDi203, SigDi204, SigDi205, SigDi206, SigDi207,
      SigDi208, SigDi209, SigDi210, SigDi211, SigDi212, SigDi213, SigDi214, SigDi215,
      SigDi216, SigDi217, SigDi218, SigDi219, SigDi220, SigDi221, SigDi222, SigDi223,
      SigDi224, SigDi225, SigDi226, SigDi227, SigDi228, SigDi229, SigDi230, SigDi231,
      SigDi232, SigDi233, SigDi234, SigDi235, SigDi236, SigDi237, SigDi238, SigDi239,
      SigDi240, SigDi241, SigDi242, SigDi243, SigDi244, SigDi245, SigDi246, SigDi247,
      SigDi248, SigDi249, SigDi250, SigDi251, SigDi252, SigDi253, SigDi254, SigDi255,

      /*DIO pins*/
      SigDio0,   SigDio1,   SigDio2,   SigDio3,   SigDio4,   SigDio5,   SigDio6,   SigDio7,
      SigDio8,   SigDio9,   SigDio10,  SigDio11,  SigDio12,  SigDio13,  SigDio14,  SigDio15,
      SigDio16,  SigDio17,  SigDio18,  SigDio19,  SigDio20,  SigDio21,  SigDio22,  SigDio23,
      SigDio24,  SigDio25,  SigDio26,  SigDio27,  SigDio28,  SigDio29,  SigDio30,  SigDio31,
      SigDio32,  SigDio33,  SigDio34,  SigDio35,  SigDio36,  SigDio37,  SigDio38,  SigDio39,
      SigDio40,  SigDio41,  SigDio42,  SigDio43,  SigDio44,  SigDio45,  SigDio46,  SigDio47,
      SigDio48,  SigDio49,  SigDio50,  SigDio51,  SigDio52,  SigDio53,  SigDio54,  SigDio55,
      SigDio56,  SigDio57,  SigDio58,  SigDio59,  SigDio60,  SigDio61,  SigDio62,  SigDio63,
      SigDio64,  SigDio65,  SigDio66,  SigDio67,  SigDio68,  SigDio69,  SigDio70,  SigDio71,
      SigDio72,  SigDio73,  SigDio74,  SigDio75,  SigDio76,  SigDio77,  SigDio78,  SigDio79,
      SigDio80,  SigDio81,  SigDio82,  SigDio83,  SigDio84,  SigDio85,  SigDio86,  SigDio87,
      SigDio88,  SigDio89,  SigDio90,  SigDio91,  SigDio92,  SigDio93,  SigDio94,  SigDio95,
      SigDio96,  SigDio97,  SigDio98,  SigDio99,  SigDio100, SigDio101, SigDio102, SigDio103,
      SigDio104, SigDio105, SigDio106, SigDio107, SigDio108, SigDio109, SigDio110, SigDio111,
      SigDio112, SigDio113, SigDio114, SigDio115, SigDio116, SigDio117, SigDio118, SigDio119,
      SigDio120, SigDio121, SigDio122, SigDio123, SigDio124, SigDio125, SigDio126, SigDio127,
      SigDio128, SigDio129, SigDio130, SigDio131, SigDio132, SigDio133, SigDio134, SigDio135,
      SigDio136, SigDio137, SigDio138, SigDio139, SigDio140, SigDio141, SigDio142, SigDio143,
      SigDio144, SigDio145, SigDio146, SigDio147, SigDio148, SigDio149, SigDio150, SigDio151,
      SigDio152, SigDio153, SigDio154, SigDio155, SigDio156, SigDio157, SigDio158, SigDio159,
      SigDio160, SigDio161, SigDio162, SigDio163, SigDio164, SigDio165, SigDio166, SigDio167,
      SigDio168, SigDio169, SigDio170, SigDio171, SigDio172, SigDio173, SigDio174, SigDio175,
      SigDio176, SigDio177, SigDio178, SigDio179, SigDio180, SigDio181, SigDio182, SigDio183,
      SigDio184, SigDio185, SigDio186, SigDio187, SigDio188, SigDio189, SigDio190, SigDio191,
      SigDio192, SigDio193, SigDio194, SigDio195, SigDio196, SigDio197, SigDio198, SigDio199,
      SigDio200, SigDio201, SigDio202, SigDio203, SigDio204, SigDio205, SigDio206, SigDio207,
      SigDio208, SigDio209, SigDio210, SigDio211, SigDio212, SigDio213, SigDio214, SigDio215,
      SigDio216, SigDio217, SigDio218, SigDio219, SigDio220, SigDio221, SigDio222, SigDio223,
      SigDio224, SigDio225, SigDio226, SigDio227, SigDio228, SigDio229, SigDio230, SigDio231,
      SigDio232, SigDio233, SigDio234, SigDio235, SigDio236, SigDio237, SigDio238, SigDio239,
      SigDio240, SigDio241, SigDio242, SigDio243, SigDio244, SigDio245, SigDio246, SigDio247,
      SigDio248, SigDio249, SigDio250, SigDio251, SigDio252, SigDio253, SigDio254, SigDio255,

      /*Counter clock pins*/
      SigCntClk0, SigCntClk1, SigCntClk2, SigCntClk3, SigCntClk4, SigCntClk5, SigCntClk6, SigCntClk7,

      /*counter gate pins*/
      SigCntGate0, SigCntGate1, SigCntGate2, SigCntGate3, SigCntGate4, SigCntGate5, SigCntGate6, SigCntGate7,

      /*counter out pins*/
      SigCntOut0,  SigCntOut1,  SigCntOut2,  SigCntOut3,  SigCntOut4,  SigCntOut5,  SigCntOut6,  SigCntOut7,

      /*counter frequency out pins*/
      SigCntFout0, SigCntFout1, SigCntFout2, SigCntFout3, SigCntFout4, SigCntFout5, SigCntFout6, SigCntFout7,

      /*AMSI pins*/
      SigAmsiPin0,  SigAmsiPin1,  SigAmsiPin2,  SigAmsiPin3,  SigAmsiPin4,  SigAmsiPin5,  SigAmsiPin6,  SigAmsiPin7,
      SigAmsiPin8,  SigAmsiPin9,  SigAmsiPin10, SigAmsiPin11, SigAmsiPin12, SigAmsiPin13, SigAmsiPin14, SigAmsiPin15,
      SigAmsiPin16, SigAmsiPin17, SigAmsiPin18, SigAmsiPin19,

      /*new clocks*/
      SigInternal2Hz,         /* Device built-in clock, 2Hz */
      SigInternal20Hz,        /* Device built-in clock, 20Hz */
      SigInternal200Hz,       /* Device built-in clock, 200KHz */
      SigInternal2KHz,        /* Device built-in clock, 2KHz */
      SigInternal20KHz,       /* Device built-in clock, 20KHz */
      SigInternal200KHz,      /* Device built-in clock, 200KHz */
      SigInternal2MHz,        /* Device built-in clock, 2MHz */
   } SignalDrop;

   typedef enum tagEventId {
      EvtDeviceRemoved = 0,  /* The device was removed from system */
      EvtDeviceReconnected,  /* The device is reconnected */
      EvtPropertyChanged,    /* Some properties of the device were changed */
      /*-----------------------------------------------------------------
      * AI events
      *-----------------------------------------------------------------*/
      EvtBufferedAiDataReady,
      EvtBufferedAiOverrun,
      EvtBufferedAiCacheOverflow,
      EvtBufferedAiStopped,

      /*-----------------------------------------------------------------
      * AO event IDs
      *-----------------------------------------------------------------*/
      EvtBufferedAoDataTransmitted,
      EvtBufferedAoUnderrun,
      EvtBufferedAoCacheEmptied,
      EvtBufferedAoTransStopped,
      EvtBufferedAoStopped,

      /*-----------------------------------------------------------------
      * DIO event IDs
      *-----------------------------------------------------------------*/
      EvtDiintChannel000, EvtDiintChannel001, EvtDiintChannel002, EvtDiintChannel003,
      EvtDiintChannel004, EvtDiintChannel005, EvtDiintChannel006, EvtDiintChannel007,
      EvtDiintChannel008, EvtDiintChannel009, EvtDiintChannel010, EvtDiintChannel011,
      EvtDiintChannel012, EvtDiintChannel013, EvtDiintChannel014, EvtDiintChannel015,
      EvtDiintChannel016, EvtDiintChannel017, EvtDiintChannel018, EvtDiintChannel019,
      EvtDiintChannel020, EvtDiintChannel021, EvtDiintChannel022, EvtDiintChannel023,
      EvtDiintChannel024, EvtDiintChannel025, EvtDiintChannel026, EvtDiintChannel027,
      EvtDiintChannel028, EvtDiintChannel029, EvtDiintChannel030, EvtDiintChannel031,
      EvtDiintChannel032, EvtDiintChannel033, EvtDiintChannel034, EvtDiintChannel035,
      EvtDiintChannel036, EvtDiintChannel037, EvtDiintChannel038, EvtDiintChannel039,
      EvtDiintChannel040, EvtDiintChannel041, EvtDiintChannel042, EvtDiintChannel043,
      EvtDiintChannel044, EvtDiintChannel045, EvtDiintChannel046, EvtDiintChannel047,
      EvtDiintChannel048, EvtDiintChannel049, EvtDiintChannel050, EvtDiintChannel051,
      EvtDiintChannel052, EvtDiintChannel053, EvtDiintChannel054, EvtDiintChannel055,
      EvtDiintChannel056, EvtDiintChannel057, EvtDiintChannel058, EvtDiintChannel059,
      EvtDiintChannel060, EvtDiintChannel061, EvtDiintChannel062, EvtDiintChannel063,
      EvtDiintChannel064, EvtDiintChannel065, EvtDiintChannel066, EvtDiintChannel067,
      EvtDiintChannel068, EvtDiintChannel069, EvtDiintChannel070, EvtDiintChannel071,
      EvtDiintChannel072, EvtDiintChannel073, EvtDiintChannel074, EvtDiintChannel075,
      EvtDiintChannel076, EvtDiintChannel077, EvtDiintChannel078, EvtDiintChannel079,
      EvtDiintChannel080, EvtDiintChannel081, EvtDiintChannel082, EvtDiintChannel083,
      EvtDiintChannel084, EvtDiintChannel085, EvtDiintChannel086, EvtDiintChannel087,
      EvtDiintChannel088, EvtDiintChannel089, EvtDiintChannel090, EvtDiintChannel091,
      EvtDiintChannel092, EvtDiintChannel093, EvtDiintChannel094, EvtDiintChannel095,
      EvtDiintChannel096, EvtDiintChannel097, EvtDiintChannel098, EvtDiintChannel099,
      EvtDiintChannel100, EvtDiintChannel101, EvtDiintChannel102, EvtDiintChannel103,
      EvtDiintChannel104, EvtDiintChannel105, EvtDiintChannel106, EvtDiintChannel107,
      EvtDiintChannel108, EvtDiintChannel109, EvtDiintChannel110, EvtDiintChannel111,
      EvtDiintChannel112, EvtDiintChannel113, EvtDiintChannel114, EvtDiintChannel115,
      EvtDiintChannel116, EvtDiintChannel117, EvtDiintChannel118, EvtDiintChannel119,
      EvtDiintChannel120, EvtDiintChannel121, EvtDiintChannel122, EvtDiintChannel123,
      EvtDiintChannel124, EvtDiintChannel125, EvtDiintChannel126, EvtDiintChannel127,
      EvtDiintChannel128, EvtDiintChannel129, EvtDiintChannel130, EvtDiintChannel131,
      EvtDiintChannel132, EvtDiintChannel133, EvtDiintChannel134, EvtDiintChannel135,
      EvtDiintChannel136, EvtDiintChannel137, EvtDiintChannel138, EvtDiintChannel139,
      EvtDiintChannel140, EvtDiintChannel141, EvtDiintChannel142, EvtDiintChannel143,
      EvtDiintChannel144, EvtDiintChannel145, EvtDiintChannel146, EvtDiintChannel147,
      EvtDiintChannel148, EvtDiintChannel149, EvtDiintChannel150, EvtDiintChannel151,
      EvtDiintChannel152, EvtDiintChannel153, EvtDiintChannel154, EvtDiintChannel155,
      EvtDiintChannel156, EvtDiintChannel157, EvtDiintChannel158, EvtDiintChannel159,
      EvtDiintChannel160, EvtDiintChannel161, EvtDiintChannel162, EvtDiintChannel163,
      EvtDiintChannel164, EvtDiintChannel165, EvtDiintChannel166, EvtDiintChannel167,
      EvtDiintChannel168, EvtDiintChannel169, EvtDiintChannel170, EvtDiintChannel171,
      EvtDiintChannel172, EvtDiintChannel173, EvtDiintChannel174, EvtDiintChannel175,
      EvtDiintChannel176, EvtDiintChannel177, EvtDiintChannel178, EvtDiintChannel179,
      EvtDiintChannel180, EvtDiintChannel181, EvtDiintChannel182, EvtDiintChannel183,
      EvtDiintChannel184, EvtDiintChannel185, EvtDiintChannel186, EvtDiintChannel187,
      EvtDiintChannel188, EvtDiintChannel189, EvtDiintChannel190, EvtDiintChannel191,
      EvtDiintChannel192, EvtDiintChannel193, EvtDiintChannel194, EvtDiintChannel195,
      EvtDiintChannel196, EvtDiintChannel197, EvtDiintChannel198, EvtDiintChannel199,
      EvtDiintChannel200, EvtDiintChannel201, EvtDiintChannel202, EvtDiintChannel203,
      EvtDiintChannel204, EvtDiintChannel205, EvtDiintChannel206, EvtDiintChannel207,
      EvtDiintChannel208, EvtDiintChannel209, EvtDiintChannel210, EvtDiintChannel211,
      EvtDiintChannel212, EvtDiintChannel213, EvtDiintChannel214, EvtDiintChannel215,
      EvtDiintChannel216, EvtDiintChannel217, EvtDiintChannel218, EvtDiintChannel219,
      EvtDiintChannel220, EvtDiintChannel221, EvtDiintChannel222, EvtDiintChannel223,
      EvtDiintChannel224, EvtDiintChannel225, EvtDiintChannel226, EvtDiintChannel227,
      EvtDiintChannel228, EvtDiintChannel229, EvtDiintChannel230, EvtDiintChannel231,
      EvtDiintChannel232, EvtDiintChannel233, EvtDiintChannel234, EvtDiintChannel235,
      EvtDiintChannel236, EvtDiintChannel237, EvtDiintChannel238, EvtDiintChannel239,
      EvtDiintChannel240, EvtDiintChannel241, EvtDiintChannel242, EvtDiintChannel243,
      EvtDiintChannel244, EvtDiintChannel245, EvtDiintChannel246, EvtDiintChannel247,
      EvtDiintChannel248, EvtDiintChannel249, EvtDiintChannel250, EvtDiintChannel251,
      EvtDiintChannel252, EvtDiintChannel253, EvtDiintChannel254, EvtDiintChannel255,

      EvtDiCosintPort000, EvtDiCosintPort001, EvtDiCosintPort002, EvtDiCosintPort003,
      EvtDiCosintPort004, EvtDiCosintPort005, EvtDiCosintPort006, EvtDiCosintPort007,
      EvtDiCosintPort008, EvtDiCosintPort009, EvtDiCosintPort010, EvtDiCosintPort011,
      EvtDiCosintPort012, EvtDiCosintPort013, EvtDiCosintPort014, EvtDiCosintPort015,
      EvtDiCosintPort016, EvtDiCosintPort017, EvtDiCosintPort018, EvtDiCosintPort019,
      EvtDiCosintPort020, EvtDiCosintPort021, EvtDiCosintPort022, EvtDiCosintPort023,
      EvtDiCosintPort024, EvtDiCosintPort025, EvtDiCosintPort026, EvtDiCosintPort027,
      EvtDiCosintPort028, EvtDiCosintPort029, EvtDiCosintPort030, EvtDiCosintPort031,

      EvtDiPmintPort000,  EvtDiPmintPort001,  EvtDiPmintPort002,  EvtDiPmintPort003,
      EvtDiPmintPort004,  EvtDiPmintPort005,  EvtDiPmintPort006,  EvtDiPmintPort007,
      EvtDiPmintPort008,  EvtDiPmintPort009,  EvtDiPmintPort010,  EvtDiPmintPort011,
      EvtDiPmintPort012,  EvtDiPmintPort013,  EvtDiPmintPort014,  EvtDiPmintPort015,
      EvtDiPmintPort016,  EvtDiPmintPort017,  EvtDiPmintPort018,  EvtDiPmintPort019,
      EvtDiPmintPort020,  EvtDiPmintPort021,  EvtDiPmintPort022,  EvtDiPmintPort023,
      EvtDiPmintPort024,  EvtDiPmintPort025,  EvtDiPmintPort026,  EvtDiPmintPort027,
      EvtDiPmintPort028,  EvtDiPmintPort029,  EvtDiPmintPort030,  EvtDiPmintPort031,

      EvtBufferedDiDataReady,
      EvtBufferedDiOverrun,
      EvtBufferedDiCacheOverflow,
      EvtBufferedDiStopped,

      EvtBufferedDoDataTransmitted,
      EvtBufferedDoUnderrun,
      EvtBufferedDoCacheEmptied,
      EvtBufferedDoTransStopped,
      EvtBufferedDoStopped,

      EvtReflectWdtOccured,
      /*-----------------------------------------------------------------
      * Counter/Timer event IDs
      *-----------------------------------------------------------------*/
      EvtCntTerminalCount0, EvtCntTerminalCount1, EvtCntTerminalCount2, EvtCntTerminalCount3,
      EvtCntTerminalCount4, EvtCntTerminalCount5, EvtCntTerminalCount6, EvtCntTerminalCount7,

      EvtCntOverCompare0,   EvtCntOverCompare1,   EvtCntOverCompare2,   EvtCntOverCompare3,
      EvtCntOverCompare4,   EvtCntOverCompare5,   EvtCntOverCompare6,   EvtCntOverCompare7,

      EvtCntUnderCompare0,  EvtCntUnderCompare1,  EvtCntUnderCompare2,  EvtCntUnderCompare3,
      EvtCntUnderCompare4,  EvtCntUnderCompare5,  EvtCntUnderCompare6,  EvtCntUnderCompare7,

      EvtCntEcOverCompare0, EvtCntEcOverCompare1, EvtCntEcOverCompare2, EvtCntEcOverCompare3,
      EvtCntEcOverCompare4, EvtCntEcOverCompare5, EvtCntEcOverCompare6, EvtCntEcOverCompare7,

      EvtCntEcUnderCompare0,EvtCntEcUnderCompare1,EvtCntEcUnderCompare2,EvtCntEcUnderCompare3,
      EvtCntEcUnderCompare4,EvtCntEcUnderCompare5,EvtCntEcUnderCompare6,EvtCntEcUnderCompare7,

      EvtCntOneShot0,       EvtCntOneShot1,       EvtCntOneShot2,       EvtCntOneShot3,
      EvtCntOneShot4,       EvtCntOneShot5,       EvtCntOneShot6,       EvtCntOneShot7,

      EvtCntTimer0,         EvtCntTimer1,         EvtCntTimer2,         EvtCntTimer3,
      EvtCntTimer4,         EvtCntTimer5,         EvtCntTimer6,         EvtCntTimer7,

      EvtCntPwmInOverflow0, EvtCntPwmInOverflow1, EvtCntPwmInOverflow2, EvtCntPwmInOverflow3,
      EvtCntPwmInOverflow4, EvtCntPwmInOverflow5, EvtCntPwmInOverflow6, EvtCntPwmInOverflow7,

   } EventId ;

   typedef enum tagPropertyAttribute {
      ReadOnly = 0,
      Writable = 1,
      Modal = 0,
      Nature = 2,
   } PropertyAttribute;

   typedef enum tagPropertyId {
      /*-----------------------------------------------------------------
      * common property
      *-----------------------------------------------------------------*/
      CFG_Number,
      CFG_ComponentType,
      CFG_Description,
      CFG_Parent,
      CFG_ChildList,

      /*-----------------------------------------------------------------
      * component specified Property IDs -- group
      *-----------------------------------------------------------------*/
      CFG_DevicesNumber,
      CFG_DevicesHandle,

      /*-----------------------------------------------------------------
      * component specified Property IDs -- device
      *-----------------------------------------------------------------*/
      CFG_DeviceGroupNumber,
      CFG_DeviceProductID,
      CFG_DeviceBoardID,
      CFG_DeviceBoardVersion,
      CFG_DeviceDriverVersion,
      CFG_DeviceDllVersion,
      CFG_DeviceLocation,                       /* Reserved for later using */
      CFG_DeviceBaseAddresses,                  /* Reserved for later using */
      CFG_DeviceInterrupts,                     /* Reserved for later using */
      CFG_DeviceSupportedTerminalBoardTypes,    /* Reserved for later using */
      CFG_DeviceTerminalBoardType,              /* Reserved for later using */
      CFG_DeviceSupportedEvents,
      CFG_DeviceHotResetPreventable,            /* Reserved for later using */
      CFG_DeviceLoadingTimeInit,                /* Reserved for later using */
      CFG_DeviceWaitingForReconnect,
      CFG_DeviceWaitingForSleep,

      /*-----------------------------------------------------------------
      * component specified Property IDs -- AI, AO...
      *-----------------------------------------------------------------*/
      CFG_FeatureResolutionInBit,
      CFG_FeatureDataSize,
      CFG_FeatureDataMask,
      CFG_FeatureChannelNumberMax,
      CFG_FeatureChannelConnectionType,
      CFG_FeatureBurnDetectedReturnTypes,
      CFG_FeatureBurnoutDetectionChannels,
      CFG_FeatureOverallVrgType,
      CFG_FeatureVrgTypes,
      CFG_FeatureExtRefRange,
      CFG_FeatureExtRefAntiPolar,
      CFG_FeatureCjcChannels,
      CFG_FeatureChannelScanMethod,
      CFG_FeatureScanChannelStartBase,
      CFG_FeatureScanChannelCountBase,
      CFG_FeatureConvertClockSources,
      CFG_FeatureConvertClockRateRange,       /* Reserved for later using */
      CFG_FeatureScanClockSources,
      CFG_FeatureScanClockRateRange,         /* Reserved for later using */
      CFG_FeatureScanCountMax,               /* Reserved for later using */
      CFG_FeatureTriggersCount,
      CFG_FeatureTriggerSources,
      CFG_FeatureTriggerActions,
      CFG_FeatureTriggerDelayCountRange,
      CFG_FeatureTriggerSources1,            /* Reserved for later using */
      CFG_FeatureTriggerActions1,            /* Reserved for later using */
      CFG_FeatureTriggerDelayCountRange1,    /* Reserved for later using */

      CFG_ChannelCount,
      CFG_ConnectionTypeOfChannels,
      CFG_VrgTypeOfChannels,
      CFG_BurnDetectedReturnTypeOfChannels,
      CFG_BurnoutReturnValueOfChannels,
      CFG_ExtRefValueForUnipolar,         /* Reserved for later using */
      CFG_ExtRefValueForBipolar,          /* Reserved for later using */

      CFG_CjcChannel,
      CFG_CjcUpdateFrequency,             /* Reserved for later using */
      CFG_CjcValue,

      CFG_SectionDataCount,
      CFG_ConvertClockSource,
      CFG_ConvertClockRatePerChannel,
      CFG_ScanChannelStart,
      CFG_ScanChannelCount,
      CFG_ScanClockSource,                /* Reserved for later using */
      CFG_ScanClockRate,                  /* Reserved for later using */
      CFG_ScanCount,                      /* Reserved for later using */
      CFG_TriggerSource,
      CFG_TriggerSourceEdge,
      CFG_TriggerSourceLevel,
      CFG_TriggerDelayCount,
      CFG_TriggerAction,
      CFG_TriggerSource1,                 /* Reserved for later using */
      CFG_TriggerSourceEdge1,             /* Reserved for later using */
      CFG_TriggerSourceLevel1,            /* Reserved for later using */
      CFG_TriggerDelayCount1,             /* Reserved for later using */
      CFG_TriggerAction1,                 /* Reserved for later using */
      CFG_ParentSignalConnectionChannel,
      CFG_ParentCjcConnectionChannel,
      CFG_ParentControlPort,

      /*-----------------------------------------------------------------
      * component specified Property IDs -- DIO
      *-----------------------------------------------------------------*/
      CFG_FeaturePortsCount,
      CFG_FeaturePortsType,
      CFG_FeatureDiNoiseFilterOfChannels,
      CFG_FeatureDiNoiseFilterBlockTimeRange,     /* Reserved for later using */
      CFG_FeatureDiintTriggerEdges,
      CFG_FeatureDiintOfChannels,
      CFG_FeatureDiintGateOfChannels,
      CFG_FeatureDiCosintOfChannels,
      CFG_FeatureDiPmintOfChannels,
      CFG_FeatureDiSnapEventSources,
      CFG_FeatureDoFreezeSignalSources,            /* Reserved for later using */
      CFG_FeatureDoReflectWdtFeedIntervalRange,    /* Reserved for later using */

      CFG_FeatureDiPortScanMethod,                 /* Reserved for later using */
      CFG_FeatureDiConvertClockSources,            /* Reserved for later using */
      CFG_FeatureDiConvertClockRateRange,          /* Reserved for later using */
      CFG_FeatureDiScanClockSources,
      CFG_FeatureDiScanClockRateRange,             /* Reserved for later using */
      CFG_FeatureDiScanCountMax,
      CFG_FeatureDiTriggersCount,
      CFG_FeatureDiTriggerSources,
      CFG_FeatureDiTriggerActions,
      CFG_FeatureDiTriggerDelayCountRange,
      CFG_FeatureDiTriggerSources1,
      CFG_FeatureDiTriggerActions1,
      CFG_FeatureDiTriggerDelayCountRange1,

      CFG_FeatureDoPortScanMethod,                 /* Reserved for later using */
      CFG_FeatureDoConvertClockSources,            /* Reserved for later using */
      CFG_FeatureDoConvertClockRateRange,          /* Reserved for later using */
      CFG_FeatureDoScanClockSources,
      CFG_FeatureDoScanClockRateRange,             /* Reserved for later using */
      CFG_FeatureDoScanCountMax,
      CFG_FeatureDoTriggersCount,
      CFG_FeatureDoTriggerSources,
      CFG_FeatureDoTriggerActions,
      CFG_FeatureDoTriggerDelayCountRange,
      CFG_FeatureDoTriggerSources1,
      CFG_FeatureDoTriggerActions1,
      CFG_FeatureDoTriggerDelayCountRange1,

      CFG_DirectionOfPorts,
      CFG_DiDataMaskOfPorts,
      CFG_DoDataMaskOfPorts,

      CFG_DiNoiseFilterOverallBlockTime,              /* Reserved for later using */
      CFG_DiNoiseFilterEnabledChannels,
      CFG_DiintTriggerEdgeOfChannels,
      CFG_DiintGateEnabledChannels,
      CFG_DiCosintEnabledChannels,
      CFG_DiPmintEnabledChannels,
      CFG_DiPmintValueOfPorts,
      CFG_DoInitialStateOfPorts,                   /* Reserved for later using */
      CFG_DoFreezeEnabled,                         /* Reserved for later using */
      CFG_DoFreezeSignalState,                     /* Reserved for later using */
      CFG_DoReflectWdtFeedInterval,                /* Reserved for later using */
      CFG_DoReflectWdtLockValue,                   /* Reserved for later using */
      CFG_DiSectionDataCount,
      CFG_DiConvertClockSource,
      CFG_DiConvertClockRatePerPort,
      CFG_DiScanPortStart,
      CFG_DiScanPortCount,
      CFG_DiScanClockSource,
      CFG_DiScanClockRate,
      CFG_DiScanCount,
      CFG_DiTriggerAction,
      CFG_DiTriggerSource,
      CFG_DiTriggerSourceEdge,
      CFG_DiTriggerSourceLevel,                    /* Reserved for later using */
      CFG_DiTriggerDelayCount,
      CFG_DiTriggerAction1,
      CFG_DiTriggerSource1,
      CFG_DiTriggerSourceEdge1,
      CFG_DiTriggerSourceLevel1,                   /* Reserved for later using */
      CFG_DiTriggerDelayCount1,

      CFG_DoSectionDataCount,
      CFG_DoConvertClockSource,
      CFG_DoConvertClockRatePerPort,
      CFG_DoScanPortStart,
      CFG_DoScanPortCount,
      CFG_DoScanClockSource,
      CFG_DoScanClockRate,
      CFG_DoScanCount,
      CFG_DoTriggerAction,
      CFG_DoTriggerSource,
      CFG_DoTriggerSourceEdge,
      CFG_DoTriggerSourceLevel,                    /* Reserved for later using */
      CFG_DoTriggerDelayCount,
      CFG_DoTriggerAction1,
      CFG_DoTriggerSource1,
      CFG_DoTriggerSourceEdge1,
      CFG_DoTriggerSourceLevel1,                   /* Reserved for later using */
      CFG_DoTriggerDelayCount1,

      /*-----------------------------------------------------------------
      * component specified Property IDs -- Counter/Timer
      *-----------------------------------------------------------------*/
      /*common feature*/
      CFG_FeatureClkPolarities,
      CFG_FeatureGatePolarities,

      CFG_FeatureCapabilitiesOfCounter0 = CFG_FeatureGatePolarities + 3,
      CFG_FeatureCapabilitiesOfCounter1,
      CFG_FeatureCapabilitiesOfCounter2,
      CFG_FeatureCapabilitiesOfCounter3,
      CFG_FeatureCapabilitiesOfCounter4,
      CFG_FeatureCapabilitiesOfCounter5,
      CFG_FeatureCapabilitiesOfCounter6,
      CFG_FeatureCapabilitiesOfCounter7,

      /*primal counter features*/
      CFG_FeatureChipOperationModes = CFG_FeatureCapabilitiesOfCounter7 + 25,
      CFG_FeatureChipSignalCountingTypes,

      /*event counting features*/
      CFG_FeatureEcSignalCountingTypes,
      CFG_FeatureEcOverCompareIntCounters,
      CFG_FeatureEcUnderCompareIntCounters,

      /*timer/pulse features*/
      CFG_FeatureTmrCascadeGroups,

      /*frequency measurement features*/
      CFG_FeatureFmCascadeGroups,
      CFG_FeatureFmMethods,

      /*Primal counter properties */
      CFG_ChipOperationModeOfCounters = CFG_FeatureFmMethods + 7,
      CFG_ChipSignalCountingTypeOfCounters,
      CFG_ChipLoadValueOfCounters,
      CFG_ChipHoldValueOfCounters,
      CFG_ChipOverCompareValueOfCounters,
      CFG_ChipUnderCompareValueOfCounters,
      CFG_ChipOverCompareEnabledCounters,
      CFG_ChipUnderCompareEnabledCounters,

      /*Event counting properties*/
      CFG_EcOverCompareValueOfCounters,
      CFG_EcUnderCompareValueOfCounters,
      CFG_EcSignalCountingTypeOfCounters,

      /*frequency measurement properties*/
      CFG_FmMethodOfCounters,
      CFG_FmCollectionPeriodOfCounters,

      //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      // v1.1
      //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      CFG_DevicePrivateRegionLength,
      CFG_SaiAutoConvertClockRate,
      CFG_SaiAutoConvertChannelStart,
      CFG_SaiAutoConvertChannelCount,
      CFG_ExtPauseSignalEnabled,
      CFG_ExtPauseSignalPolarity,
      CFG_OrderOfChannels,
      CFG_InitialStateOfChannels,

      //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      // v1.2: new features & properties of counter
      //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      /*common features*/
      CFG_FeatureOutSignalTypes, 

      /*primal counter features*/
      CFG_FeatureChipClkSourceOfCounter0,
      CFG_FeatureChipClkSourceOfCounter1,
      CFG_FeatureChipClkSourceOfCounter2,
      CFG_FeatureChipClkSourceOfCounter3,
      CFG_FeatureChipClkSourceOfCounter4,
      CFG_FeatureChipClkSourceOfCounter5,
      CFG_FeatureChipClkSourceOfCounter6,
      CFG_FeatureChipClkSourceOfCounter7,

      CFG_FeatureChipGateSourceOfCounter0,
      CFG_FeatureChipGateSourceOfCounter1,
      CFG_FeatureChipGateSourceOfCounter2,
      CFG_FeatureChipGateSourceOfCounter3,
      CFG_FeatureChipGateSourceOfCounter4,
      CFG_FeatureChipGateSourceOfCounter5,
      CFG_FeatureChipGateSourceOfCounter6,
      CFG_FeatureChipGateSourceOfCounter7,

      CFG_FeatureChipValueRegisters,

      /*one-shot features*/
      CFG_FeatureOsClkSourceOfCounter0,
      CFG_FeatureOsClkSourceOfCounter1,
      CFG_FeatureOsClkSourceOfCounter2,
      CFG_FeatureOsClkSourceOfCounter3,
      CFG_FeatureOsClkSourceOfCounter4,
      CFG_FeatureOsClkSourceOfCounter5,
      CFG_FeatureOsClkSourceOfCounter6,
      CFG_FeatureOsClkSourceOfCounter7,

      CFG_FeatureOsGateSourceOfCounter0,
      CFG_FeatureOsGateSourceOfCounter1,
      CFG_FeatureOsGateSourceOfCounter2,
      CFG_FeatureOsGateSourceOfCounter3,
      CFG_FeatureOsGateSourceOfCounter4,
      CFG_FeatureOsGateSourceOfCounter5,
      CFG_FeatureOsGateSourceOfCounter6,
      CFG_FeatureOsGateSourceOfCounter7,

      /*Pulse width measurement features*/
      CFG_FeaturePiCascadeGroups,

      /*common properties*/
      CFG_ClkPolarityOfCounters,
      CFG_GatePolarityOfCounters,
      CFG_OutSignalTypeOfCounters,

      /*Primal counter properties */
      CFG_ChipClkSourceOfCounters, 
      CFG_ChipGateSourceOfCounters,

      /*one-shot properties*/
      CFG_OsClkSourceOfCounters, 
      CFG_OsGateSourceOfCounters,
      CFG_OsDelayCountOfCounters,

      /*Timer pulse properties*/
      CFG_TmrFrequencyOfCounters,

      /*Pulse width modulation properties*/
      CFG_PoHiPeriodOfCounters,
      CFG_PoLoPeriodOfCounters,
   } PropertyId;

   #define BioFailed(c)       ( (long)(c) >= 0xE0000000 )   
   typedef enum tagErrorCode {
      /// <summary>
      /// The operation is completed successfully. 
      /// </summary>
      Success = 0, 

      ///************************************************************************
      /// warning                                                              
      ///************************************************************************
      /// <summary>
      /// The interrupt resource is not available. 
      /// </summary>
      WarningIntrNotAvailable = 0xA0000000,

      /// <summary>
      /// The parameter is out of the range. 
      /// </summary>
      WarningParamOutOfRange = 0xA0000001,

      /// <summary>
      /// The property value is out of range. 
      /// </summary>
      WarningPropValueOutOfRange = 0xA0000002,

      /// <summary>
      /// The property value is not supported. 
      /// </summary>
      WarningPropValueNotSpted = 0xA0000003,

      /// <summary>
      /// The property value conflicts with the current state.
      /// </summary>
      WarningPropValueConflict = 0xA0000004,

      ///***********************************************************************
      /// error                                                                
      ///***********************************************************************
      /// <summary>
      /// The handle is NULL or its type doesn't match the required operation. 
      /// </summary>
      ErrorHandleNotValid = 0xE0000000,

      /// <summary>
      /// The parameter value is out of range.
      /// </summary>
      ErrorParamOutOfRange = 0xE0000001,

      /// <summary>
      /// The parameter value is not supported.
      /// </summary>
      ErrorParamNotSpted = 0xE0000002,

      /// <summary>
      /// The parameter value format is not the expected. 
      /// </summary>
      ErrorParamFmtUnexpted = 0xE0000003,

      /// <summary>
      /// Not enough memory is available to complete the operation. 
      /// </summary>
      ErrorMemoryNotEnough = 0xE0000004,

      /// <summary>
      /// The data buffer is null. 
      /// </summary>
      ErrorBufferIsNull = 0xE0000005,

      /// <summary>
      /// The data buffer is too small for the operation. 
      /// </summary>
      ErrorBufferTooSmall = 0xE0000006,

      /// <summary>
      /// The data length exceeded the limitation. 
      /// </summary>
      ErrorDataLenExceedLimit = 0xE0000007,

      /// <summary>
      /// The required function is not supported. 
      /// </summary>
      ErrorFuncNotSpted = 0xE0000008,

      /// <summary>
      /// The required event is not supported. 
      /// </summary>
      ErrorEventNotSpted = 0xE0000009,

      /// <summary>
      /// The required property is not supported. 
      /// </summary>
      ErrorPropNotSpted = 0xE000000A, 

      /// <summary>
      /// The required property is read-only. 
      /// </summary>
      ErrorPropReadOnly = 0xE000000B,

      /// <summary>
      /// The specified property value conflicts with the current state.
      /// </summary>
      ErrorPropValueConflict = 0xE000000C,

      /// <summary>
      /// The specified property value is out of range.
      /// </summary>
      ErrorPropValueOutOfRange = 0xE000000D,

      /// <summary>
      /// The specified property value is not supported. 
      /// </summary>
      ErrorPropValueNotSpted = 0xE000000E,

      /// <summary>
      /// The handle hasn't own the privilege of the operation the user wanted. 
      /// </summary>
      ErrorPrivilegeNotHeld = 0xE000000F,

      /// <summary>
      /// The required privilege is not available because someone else had own it. 
      /// </summary>
      ErrorPrivilegeNotAvailable = 0xE0000010,

      /// <summary>
      /// The driver of specified device was not found. 
      /// </summary>
      ErrorDriverNotFound = 0xE0000011,

      /// <summary>
      /// The driver version of the specified device mismatched. 
      /// </summary>
      ErrorDriverVerMismatch = 0xE0000012,

      /// <summary>
      /// The loaded driver count exceeded the limitation. 
      /// </summary>
      ErrorDriverCountExceedLimit = 0xE0000013,

      /// <summary>
      /// The device is not opened. 
      /// </summary>
      ErrorDeviceNotOpened = 0xE0000014,      

      /// <summary>
      /// The required device does not exist. 
      /// </summary>
      ErrorDeviceNotExist = 0xE0000015,

      /// <summary>
      /// The required device is unrecognized by driver. 
      /// </summary>
      ErrorDeviceUnrecognized = 0xE0000016,

      /// <summary>
      /// The configuration data of the specified device is lost or unavailable. 
      /// </summary>
      ErrorConfigDataLost = 0xE0000017,

      /// <summary>
      /// The function is not initialized and can't be started. 
      /// </summary>
      ErrorFuncNotInited = 0xE0000018,

      /// <summary>
      /// The function is busy. 
      /// </summary>
      ErrorFuncBusy = 0xE0000019,

      /// <summary>
      /// The interrupt resource is not available. 
      /// </summary>
      ErrorIntrNotAvailable = 0xE000001A,

      /// <summary>
      /// The DMA channel is not available. 
      /// </summary>
      ErrorDmaNotAvailable = 0xE000001B,

      /// <summary>
      /// Time out when reading/writing the device. 
      /// </summary>
      ErrorDeviceIoTimeOut = 0xE000001C,

      /// <summary>
      /// The given signature does not match with the device current one.
      /// </summary>
      ErrorSignatureNotMatch = 0xE000001D,
     
      /// <summary>
      /// Undefined error 
      /// </summary>
      ErrorUndefined = 0xE000FFFF,
   } ErrorCode;

   // Advantech CardType ID 
   typedef enum tagProductId {
      BD_DEMO   = 0x00,		// demo board
      BD_PCL818 = 0x05,		// PCL-818 board
      BD_PCL818H = 0x11,	// PCL-818H
      BD_PCL818L = 0x21,	// PCL-818L
      BD_PCL818HG = 0x22,	// PCL-818HG
      BD_PCL818HD = 0x2b,	// PCL-818HD
      BD_PCM3718 = 0x37,	// PCM-3718
      BD_PCM3724 = 0x38,	// PCM-3724
      BD_PCM3730 = 0x5a,	// PCM-3730
      BD_PCI1750 = 0x5e,	// PCI-1750
      BD_PCI1751 = 0x5f,	// PCI-1751
      BD_PCI1710 = 0x60,	// PCI-1710
      BD_PCI1712 = 0x61,	// PCI-1712
      BD_PCI1710HG = 0x67,	// PCI-1710HG
      BD_PCI1711 = 0x73,	// PCI-1711
      BD_PCI1711L = 0x75,	// PCI-1711L 
      BD_PCI1713 = 0x68,	// PCI-1713
      BD_PCI1753 = 0x69,	// PCI-1753
      BD_PCI1760 = 0x6a,	// PCI-1760
      BD_PCI1720 = 0x6b,	// PCI-1720
      BD_PCM3718H = 0x6d,	// PCM-3718H
      BD_PCM3718HG = 0x6e,	// PCM-3718HG
      BD_PCI1716 = 0x74,	// PCI-1716
      BD_PCI1731 = 0x75,	// PCI-1731
      BD_PCI1754 = 0x7b,	// PCI-1754
      BD_PCI1752 = 0x7c,	// PCI-1752
      BD_PCI1756 = 0x7d,	// PCI-1756
      BD_PCM3725 = 0x7f,	// PCM-3725
      BD_PCI1762 = 0x80,	// PCI-1762
      BD_PCI1721 = 0x81,	// PCI-1721
      BD_PCI1761 = 0x82,	// PCI-1761
      BD_PCI1723 = 0x83,	// PCI-1723
      BD_PCI1730 = 0x87,	// PCI-1730
      BD_PCI1733 = 0x88,	// PCI-1733
      BD_PCI1734 = 0x89,	// PCI-1734
      BD_PCI1710L = 0x90,	// PCI-1710L
      BD_PCI1710HGL = 0x91,// PCI-1710HGL
      BD_PCM3712 = 0x93,	// PCM-3712
      BD_PCM3723 = 0x94,	// PCM-3723
      BD_PCI1780 = 0x95,	// PCI-1780
      BD_CPCI3756 = 0x96,	// CPCI-3756
      BD_PCI1755 = 0x97,	// PCI-1755
      BD_PCI1714 = 0x98,	// PCI-1714
      BD_PCI1757 = 0x99,	// PCI-1757
      BD_MIC3716 = 0x9A,	// MIC-3716
      BD_MIC3761 = 0x9B,	// MIC-3761
      BD_MIC3753 = 0x9C,		// MIC-3753
      BD_MIC3780 = 0x9D,		// MIC-3780
      BD_PCI1724 = 0x9E,		// PCI-1724
      BD_PCI1758UDI = 0xA3,	// PCI-1758UDI
      BD_PCI1758UDO = 0xA4,	// PCI-1758UDO
      BD_PCI1747 = 0xA5,		// PCI-1747
      BD_PCM3780 = 0xA6,		// PCM-3780 
      BD_MIC3747 = 0xA7,		// MIC-3747
      BD_PCI1758UDIO = 0xA8,	// PCI-1758UDIO
      BD_PCI1712L = 0xA9,		// PCI-1712L
      BD_PCI1763UP = 0xAC,	   // PCI-1763UP
      BD_PCI1736UP = 0xAD,	   // PCI-1736UP
      BD_PCI1714UL = 0xAE,	   // PCI-1714UL
      BD_MIC3714 = 0xAF,		// MIC-3714
      BD_PCM3718HO = 0xB1,	   // PCM-3718HO
      BD_PCI1741U = 0xB3,		// PCI-1741U
      BD_MIC3723 = 0xB4,		// MIC-3723 
      BD_PCI1718HDU = 0xB5,	// PCI-1718HDU
      BD_MIC3758DIO = 0xB6,	// MIC-3758DIO
      BD_PCI1727U = 0xB7,		// PCI-1727U
      BD_PCI1718HGU = 0xB8,	// PCI-1718HGU
      BD_PCI1715U = 0xB9,		// PCI-1715U
      BD_PCI1716L = 0xBA,		// PCI-1716L
      BD_PCI1735U = 0xBB,		// PCI-1735U
      BD_USB4711 = 0xBC,		// USB4711
      BD_PCI1737U = 0xBD,		// PCI-1737U
      BD_PCI1739U = 0xBE,		// PCI-1739U
      BD_PCI1742U = 0xC0,		// PCI-1742U
      BD_USB4718 = 0xC6,		// USB-4718
      BD_MIC3755 = 0xC7,		// MIC3755
      BD_USB4761 = 0xC8,		// USB4761
      BD_PCI1784 = 0XCC,		// PCI-1784
      BD_USB4716 = 0xCD,		// USB4716
      BD_PCI1752U = 0xCE,		// PCI-1752U
      BD_PCI1752USO = 0xCF,	// PCI-1752USO
      BD_USB4751 = 0xD0,		// USB4751
      BD_USB4751L = 0xD1,		// USB4751L
      BD_USB4750 = 0xD2,		// USB4750
      BD_MIC3713 = 0xD3,		// MIC-3713
      BD_USB4711A = 0xD8,		// USB4711A
      BD_PCM3753P = 0xD9,		// PCM3753P
      BD_PCM3784  = 0xDA,		// PCM3784
      BD_PCM3761I = 0xDB,     // PCM-3761I
      BD_MIC3751  = 0xDC,     // MIC-3751
      BD_PCM3730I = 0xDD,     // PCM-3730I
      BD_PCM3813I = 0xE0,     // PCM-3813I
      BD_PCIE1744	= 0xE1,     //PCIE-1744
      BD_PCI1730U	= 0xE2, 	   // PCI-1730U
      BD_PCI1760U	= 0xE3,	   //PCI-1760U
      BD_MIC3720	= 0xE4,	   //MIC-3720
      BD_PCM3810I = 0xE9,     // PCM-3810I
      BD_USB4702  = 0xEA,     // USB4702
      BD_USB4704  = 0xEB,     // USB4704
      BD_PCM3810I_HG = 0xEC,  // PCM-3810I_HG
      BD_PCI1713U = 0xED,		// PCI-1713U 

      // !!!BioDAQ only Product ID starts from here!!!
      BD_PCI1706 = 0x800,
   } ProductId;

_BDAQ_END

// **********************************************************
// Bionic DAQ APIs
// **********************************************************
_BDAQ_BEGIN
   #define BDAQ_DEVICE_DESC_MAX_LEN          64
   #define BDAQ_VALUE_RANGE_DESC_MAX_LEN     256
   #define BDAQ_SIGNAL_DROP_DESC_MAX_LEN     256
  
   struct BDaqLib
   {
      HMODULE   instHandle;
      // global APIs
      ErrorCode (WINAPI *pfnGetValueRangeInformation)(long,long,WCHAR*,MathInterval*,long*);
      ErrorCode (WINAPI *pfnGetSignalConnectionInformation)(long,long,WCHAR*,long*);
      double    (WINAPI *pfnTranslateTemperatureScale)(long,double);
      ErrorCode (WINAPI *pfnDeviceGetLinkageInfo)(long,long,long*,WCHAR*,long*);
      // event APIs
      ErrorCode (WINAPI *pfnEventGetHandle)(HANDLE,long,HANDLE*);
      ErrorCode (WINAPI *pfnEventGetLastStatus)(HANDLE,long,long*,long*);
      ErrorCode (WINAPI *pfnEventClearFlag)(HANDLE,long,long,long);
      // property APIs
      ErrorCode (WINAPI *pfnPropertyRead)(HANDLE,long,long,void*,long*,long*);
      ErrorCode (WINAPI *pfnPropertyWrite)(HANDLE,long,long,void*,long);
      // device APIs
      ErrorCode (WINAPI *pfnDeviceOpen)(long,long,HANDLE*);
      ErrorCode (WINAPI *pfnDeviceClose)(HANDLE);
      ErrorCode (WINAPI *pfnDeviceReset)(HANDLE,long);
      ErrorCode (WINAPI *pfnDeviceGetModuleHandle)(HANDLE,long,long,HANDLE*);
      ErrorCode (WINAPI *pfnDeviceRefreshProperties)(HANDLE);
      ErrorCode (WINAPI *pfnDeviceShowConfigDialogBox)(HANDLE,HWND,RECT*,long,BOOL,HWND*);
      // Analog input APIs
      ErrorCode (WINAPI *pfnAiReadSamples)(HANDLE,long,long,void*,double*);
      ErrorCode (WINAPI *pfnBufferedAiPrepare)(HANDLE,long,void**);
      ErrorCode (WINAPI *pfnBufferedAiRunOnce)(HANDLE,BOOL);
      ErrorCode (WINAPI *pfnBufferedAiRun)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedAiStop)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedAiScaleData)(HANDLE,void*,double*,long,long*);
      ErrorCode (WINAPI *pfnBufferedAiRelease)(HANDLE);
      // Analog output APIs
      ErrorCode (WINAPI *pfnAoWriteSamples)(HANDLE,long,long,void*,double*);
      ErrorCode (WINAPI *pfnBufferedAoPrepare)(HANDLE,long,void**);
      ErrorCode (WINAPI *pfnBufferedAoRunOnce)(HANDLE,BOOL);
      ErrorCode (WINAPI *pfnBufferedAoRun)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedAoStop)(HANDLE,long);
      ErrorCode (WINAPI *pfnBufferedAoScaleData)(HANDLE,double*,void*,long,long*);
      ErrorCode (WINAPI *pfnBufferedAoRelease)(HANDLE);
      // Digital input/output APIs
      ErrorCode (WINAPI *pfnDiReadPorts)(HANDLE,long,long,BYTE*);
      ErrorCode (WINAPI *pfnDoWritePorts)(HANDLE,long,long,BYTE*);
      ErrorCode (WINAPI *pfnDoReadBackPorts)(HANDLE,long,long,BYTE*);
      ErrorCode (WINAPI *pfnDiSnapStart)(HANDLE,long,long,long,BYTE**);
      ErrorCode (WINAPI *pfnDiSnapStop)(HANDLE,long);
      ErrorCode (WINAPI *pfnBufferedDiPrepare)(HANDLE,long,void**);
      ErrorCode (WINAPI *pfnBufferedDiRunOnce)(HANDLE,BOOL);
      ErrorCode (WINAPI *pfnBufferedDiRun)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedDiStop)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedDiRelease)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedDoPrepare)(HANDLE,long,void**);
      ErrorCode (WINAPI *pfnBufferedDoRunOnce)(HANDLE,BOOL);
      ErrorCode (WINAPI *pfnBufferedDoRun)(HANDLE);
      ErrorCode (WINAPI *pfnBufferedDoStop)(HANDLE,long);
      ErrorCode (WINAPI *pfnBufferedDoRelease)(HANDLE);
      // Counter APIs
      ErrorCode (WINAPI *pfnCounterReset)(HANDLE,long,long);
      ErrorCode (WINAPI *pfnEventCountStart)(HANDLE,long,long);
      ErrorCode (WINAPI *pfnEventCountRead)(HANDLE,long,long,long*);
      ErrorCode (WINAPI *pfnOneShotStart)(HANDLE,long,long);
      ErrorCode (WINAPI *pfnTimerPulseStart)(HANDLE,long,long);
      ErrorCode (WINAPI *pfnFrequencyMeasureStart)(HANDLE,long,long);
      ErrorCode (WINAPI *pfnFrequencyMeasureRead)(HANDLE,long,long,double*);
      ErrorCode (WINAPI *pfnPwmInStart)(HANDLE, long, long);
      ErrorCode (WINAPI *pfnPwmInRead)(HANDLE,long,long,double*,double*);
      ErrorCode (WINAPI *pfnPwmOutStart)(HANDLE, long, long);
   };
   __declspec(selectany) BDaqLib s_bdaqLib = {NULL};
   #if defined(_MSC_VER) && (_MSC_VER >= 1300)
   __declspec(noinline)
   #endif
   __inline BOOL InitializeBioDaqLibray()
   {
      if (s_bdaqLib.instHandle == NULL)
      {
         if ((s_bdaqLib.instHandle = LoadLibraryW(L"BioDaq.dll")) != NULL) 
         {
            #define BDAQ_GET_PROC(fn) (*(FARPROC*)&s_bdaqLib.pfn##fn = GetProcAddress(s_bdaqLib.instHandle, "Adx"#fn))
            BDAQ_GET_PROC(GetValueRangeInformation);
            BDAQ_GET_PROC(GetSignalConnectionInformation);
            BDAQ_GET_PROC(TranslateTemperatureScale);
            BDAQ_GET_PROC(DeviceGetLinkageInfo);
            BDAQ_GET_PROC(EventGetHandle);
            BDAQ_GET_PROC(EventGetLastStatus);
            BDAQ_GET_PROC(EventClearFlag);
            BDAQ_GET_PROC(PropertyRead);
            BDAQ_GET_PROC(PropertyWrite);
            BDAQ_GET_PROC(DeviceOpen);
            BDAQ_GET_PROC(DeviceClose);
            BDAQ_GET_PROC(DeviceReset);
            BDAQ_GET_PROC(DeviceGetModuleHandle);
            BDAQ_GET_PROC(DeviceRefreshProperties);
            BDAQ_GET_PROC(DeviceShowConfigDialogBox);
            BDAQ_GET_PROC(AiReadSamples);
            BDAQ_GET_PROC(BufferedAiPrepare);
            BDAQ_GET_PROC(BufferedAiRunOnce);
            BDAQ_GET_PROC(BufferedAiRun);
            BDAQ_GET_PROC(BufferedAiStop);
            BDAQ_GET_PROC(BufferedAiScaleData);
            BDAQ_GET_PROC(BufferedAiRelease);
            BDAQ_GET_PROC(AoWriteSamples);
            BDAQ_GET_PROC(BufferedAoPrepare);
            BDAQ_GET_PROC(BufferedAoRunOnce);
            BDAQ_GET_PROC(BufferedAoRun);
            BDAQ_GET_PROC(BufferedAoStop);
            BDAQ_GET_PROC(BufferedAoScaleData);
            BDAQ_GET_PROC(BufferedAoRelease);
            BDAQ_GET_PROC(DiReadPorts);
            BDAQ_GET_PROC(DoWritePorts);
            BDAQ_GET_PROC(DoReadBackPorts);
            BDAQ_GET_PROC(DiSnapStart);
            BDAQ_GET_PROC(DiSnapStop);
            BDAQ_GET_PROC(BufferedDiPrepare);
            BDAQ_GET_PROC(BufferedDiRunOnce);
            BDAQ_GET_PROC(BufferedDiRun);
            BDAQ_GET_PROC(BufferedDiStop);
            BDAQ_GET_PROC(BufferedDiRelease);
            BDAQ_GET_PROC(BufferedDoPrepare);
            BDAQ_GET_PROC(BufferedDoRunOnce);
            BDAQ_GET_PROC(BufferedDoRun);
            BDAQ_GET_PROC(BufferedDoStop);
            BDAQ_GET_PROC(BufferedDoRelease);
            BDAQ_GET_PROC(CounterReset);
            BDAQ_GET_PROC(EventCountStart);
            BDAQ_GET_PROC(EventCountRead);
            BDAQ_GET_PROC(OneShotStart);
            BDAQ_GET_PROC(TimerPulseStart);
            BDAQ_GET_PROC(FrequencyMeasureStart);
            BDAQ_GET_PROC(FrequencyMeasureRead);
            BDAQ_GET_PROC(PwmInStart);
            BDAQ_GET_PROC(PwmInRead);
            BDAQ_GET_PROC(PwmOutStart);
         }
      }
      return s_bdaqLib.instHandle != NULL;
   }
   //-------------------------------------------------------
   // global helper APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxDeviceGetLinkageInfo(
      IN  long            deviceParent, 
      IN  long            index,
      OUT long*           deviceNumber,
      OUT OPTIONAL WCHAR* description, 
      OUT OPTIONAL long*  subDeviceCount)
   {
      if (InitializeBioDaqLibray())
      {
         return s_bdaqLib.pfnDeviceGetLinkageInfo(deviceParent, index, deviceNumber, description, subDeviceCount);
      }
      return ErrorDriverNotFound;
   }
   __inline ErrorCode WINAPI AdxGetValueRangeInformation( 
      IN ValueRange              type, 
      IN long                    descBufSize, 
      OUT OPTIONAL WCHAR*        description, 
      OUT OPTIONAL MathInterval* range,  
      OUT OPTIONAL ValueUnit*    unit)
   {
      if (InitializeBioDaqLibray())
      {
         return s_bdaqLib.pfnGetValueRangeInformation(type, descBufSize, description, range, (long*)unit);
      }
      return ErrorDriverNotFound;
   }
   __inline ErrorCode WINAPI AdxGetSignalConnectionInformation(
      IN SignalDrop       signal,
      IN long             descBufSize,
      OUT OPTIONAL WCHAR* description,
      OUT OPTIONAL SignalPosition* position)
   {
      if (InitializeBioDaqLibray())
      {
         return s_bdaqLib.pfnGetSignalConnectionInformation(signal, descBufSize, description, (long*)position);
      }
      return ErrorDriverNotFound;
   }
   __inline double WINAPI AdxTranslateTemperatureScale(
      IN TemperatureDegree degreeType,
      IN double            degreeCelsius )
   {
      if (InitializeBioDaqLibray())
      {
         return s_bdaqLib.pfnTranslateTemperatureScale(degreeType, degreeCelsius);
      }
      return ErrorDriverNotFound;
   }
   //-------------------------------------------------------
   // event APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxEventGetHandle(
      IN HANDLE   handle,
      IN EventId  id,
      OUT HANDLE* eventHandle)
   {
      return s_bdaqLib.pfnEventGetHandle(handle, id, eventHandle);
   }
   __inline ErrorCode WINAPI AdxEventGetLastStatus(
      IN HANDLE           handle, 
      IN EventId          id, 
      OUT OPTIONAL long* statusLParam,
      OUT OPTIONAL long* statusRParam)
   {
      return s_bdaqLib.pfnEventGetLastStatus(handle, id, statusLParam, statusRParam);
   }
   __inline ErrorCode WINAPI AdxEventClearFlag(
      IN HANDLE  handle, 
      IN EventId id, 
      IN long    flagLParam,
      IN long    flagRParam)
   {
      return s_bdaqLib.pfnEventClearFlag(handle, id, flagLParam, flagRParam);
   }

   //-------------------------------------------------------
   // property APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxPropertyRead(
      IN HANDLE          handle, 
      IN PropertyId      id, 
      IN long            bufferSize,
      OUT OPTIONAL void* buffer, 
      OUT OPTIONAL long* dataLength,
      OUT OPTIONAL long* attribute)
   {
      return s_bdaqLib.pfnPropertyRead(handle, id, bufferSize, buffer, dataLength, attribute);
   }
   __inline ErrorCode WINAPI AdxPropertyWrite(
      IN HANDLE     handle, 
      IN PropertyId id, 
      IN long       dataLength,
      IN void*      buffer,
      IN long       notifyNow)
   {
      return s_bdaqLib.pfnPropertyWrite(handle, id, dataLength, buffer, notifyNow);
   }

   //-------------------------------------------------------
   // device APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxDeviceOpen(
      IN long       deviceNumber,
      IN AccessMode accessMode,
      OUT HANDLE*   deviceHandle)
   {
      if (InitializeBioDaqLibray())
      {
         return s_bdaqLib.pfnDeviceOpen(deviceNumber, accessMode, deviceHandle);
      }
      return ErrorDriverNotFound;
   }
   __inline ErrorCode WINAPI AdxDeviceClose(IN HANDLE deviceHandle)
   {
      return s_bdaqLib.pfnDeviceClose(deviceHandle);
   }
   __inline ErrorCode WINAPI AdxDeviceReset(
      IN HANDLE deviceHandle, 
      IN long   state)
   {
      return s_bdaqLib.pfnDeviceReset(deviceHandle, state);
   }
   __inline ErrorCode WINAPI AdxDeviceGetModuleHandle(
      IN HANDLE     deviceHandle,
      IN ModuleType type,
      IN long       index,
      OUT HANDLE*   moduleHandle )
   {
      return s_bdaqLib.pfnDeviceGetModuleHandle(deviceHandle, type, index, moduleHandle);
   }
   __inline ErrorCode WINAPI AdxDeviceRefreshProperties(IN HANDLE deviceHandle)
   {
      return s_bdaqLib.pfnDeviceRefreshProperties(deviceHandle);
   }
   __inline ErrorCode WINAPI AdxDeviceShowConfigDialogBox(
      IN HANDLE deviceHandle, 
      IN HWND   parentWindow, 
      IN RECT*  region,
      IN long   dataSource,
      IN BOOL   modal,
      OUT HWND* dialogWindow)
   {
      return s_bdaqLib.pfnDeviceShowConfigDialogBox(deviceHandle, parentWindow, region, dataSource, modal, dialogWindow);
   }

   //-------------------------------------------------------
   // analog input APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxAiReadSamples(
      IN HANDLE            aiHandle, 
      IN long              channelStart, 
      IN long              channelCount,
      OUT OPTIONAL void*   rawData,
      OUT OPTIONAL double* scaledData)
   {
      return s_bdaqLib.pfnAiReadSamples(aiHandle, channelStart, channelCount, rawData, scaledData);
   }
   __inline ErrorCode WINAPI AdxBufferedAiPrepare(
      IN HANDLE  aiHandle, 
      IN long    dataCount, 
      OUT void** dataBuffer)
   {
      return s_bdaqLib.pfnBufferedAiPrepare(aiHandle, dataCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxBufferedAiRunOnce(
      IN HANDLE aiHandle, 
      IN BOOL   asynchronous)
   {
      return s_bdaqLib.pfnBufferedAiRunOnce(aiHandle, asynchronous);
   }
   __inline ErrorCode WINAPI AdxBufferedAiRun(IN HANDLE aiHandle)
   {
      return s_bdaqLib.pfnBufferedAiRun(aiHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedAiStop(IN HANDLE aiHandle)
   {
      return s_bdaqLib.pfnBufferedAiStop(aiHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedAiRelease(IN HANDLE aiHandle)
   {
      return s_bdaqLib.pfnBufferedAiRelease(aiHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedAiScaleData(
      IN HANDLE              aiHandle,
      IN void*               rawData,
      OUT double*            scaledData,
      IN long                dataCount,
      IN OUT OPTIONAL long*  channelOffset)
   {
      return s_bdaqLib.pfnBufferedAiScaleData(aiHandle, rawData, scaledData, dataCount, channelOffset);
   }
   //-------------------------------------------------------
   // analog output APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxAoWriteSamples(
      IN HANDLE           aoHandle,
      IN long             channelStart,
      IN long             channelCount,
      IN OPTIONAL void*   rawData,
      IN OPTIONAL double* scaledData)
   {
      return s_bdaqLib.pfnAoWriteSamples(aoHandle, channelStart, channelCount, rawData, scaledData);
   }
   __inline ErrorCode WINAPI AdxBufferedAoPrepare(
      IN HANDLE  aoHandle,
      IN long    dataCount,
      OUT void** dataBuffer)
   {
      return s_bdaqLib.pfnBufferedAoPrepare(aoHandle, dataCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxBufferedAoRunOnce(
      IN HANDLE aoHandle, 
      IN BOOL   asynchronous)
   {
      return s_bdaqLib.pfnBufferedAoRunOnce(aoHandle, asynchronous);
   }
   __inline ErrorCode WINAPI AdxBufferedAoRun(IN HANDLE aoHandle)
   {
      return s_bdaqLib.pfnBufferedAoRun(aoHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedAoStop(
      IN HANDLE aoHandle, 
      IN long   action)
   {
      return s_bdaqLib.pfnBufferedAoStop(aoHandle, action);
   }
   __inline ErrorCode WINAPI AdxBufferedAoRelease(IN HANDLE aoHandle)
   {
      return s_bdaqLib.pfnBufferedAoRelease(aoHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedAoScaleData(
      IN HANDLE             aoHandle,
      IN double*            scaledData,
      OUT void*             rawData,
      IN long               dataCount,
      IN OUT OPTIONAL long* channelOffset)
   {
      return s_bdaqLib.pfnBufferedAoScaleData(aoHandle, scaledData, rawData, dataCount, channelOffset);
   }

   //-------------------------------------------------------
   // digital input/output APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxDiReadPorts(
      IN HANDLE dioHandle,
      IN long   portStart,
      IN long   portCount,
      OUT BYTE* dataBuffer)
   {
      return s_bdaqLib.pfnDiReadPorts(dioHandle, portStart, portCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxDoWritePorts(
      IN HANDLE dioHandle,
      IN long   portStart,
      IN long	 portCount,
      IN BYTE*  dataBuffer)
   {
      return s_bdaqLib.pfnDoWritePorts(dioHandle, portStart, portCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxDoReadBackPorts(
      IN HANDLE dioHandle,
      IN long   portStart,
      IN long   portCount,
      OUT BYTE* dataBuffer)
   {
      return s_bdaqLib.pfnDoReadBackPorts(dioHandle, portStart, portCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxBufferedDiPrepare(
      IN HANDLE  dioHandle,
      IN long    dataCount,
      OUT void** dataBuffer)
   {
      return s_bdaqLib.pfnBufferedDiPrepare(dioHandle, dataCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxBufferedDiRunOnce(
      IN HANDLE dioHandle, 
      IN BOOL   asynchronous)
   {
      return s_bdaqLib.pfnBufferedDiRunOnce(dioHandle, asynchronous);
   }
   __inline ErrorCode WINAPI AdxBufferedDiRun(IN HANDLE dioHandle)
   {
      return s_bdaqLib.pfnBufferedDiRun(dioHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedDiStop(IN HANDLE dioHandle)
   {
      return s_bdaqLib.pfnBufferedDiStop(dioHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedDiRelease(IN HANDLE dioHandle)
   {
      return s_bdaqLib.pfnBufferedDiRelease(dioHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedDoPrepare(
      IN HANDLE  dioHandle,
      IN long    dataCount,
      OUT void** dataBuffer)
   {
      return s_bdaqLib.pfnBufferedDoPrepare(dioHandle, dataCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxBufferedDoRunOnce(
      IN HANDLE dioHandle, 
      IN BOOL   asynchronous)
   {
      return s_bdaqLib.pfnBufferedDoRunOnce(dioHandle, asynchronous);
   }
   __inline ErrorCode WINAPI AdxBufferedDoRun(IN HANDLE dioHandle)
   {
      return s_bdaqLib.pfnBufferedDoRun(dioHandle);
   }
   __inline ErrorCode WINAPI AdxBufferedDoStop(IN HANDLE dioHandle, long action)
   {
      return s_bdaqLib.pfnBufferedDoStop(dioHandle, action);
   }
   __inline ErrorCode WINAPI AdxBufferedDoRelease(IN HANDLE dioHandle)
   {
      return s_bdaqLib.pfnBufferedDoRelease(dioHandle);
   }
   __inline ErrorCode WINAPI AdxDiSnapStart(
      IN HANDLE  dioHandle,
      IN EventId id,
      IN long    portStart,
      IN long    portCount,
      OUT BYTE** dataBuffer)
   {
      return s_bdaqLib.pfnDiSnapStart(dioHandle, id, portStart, portCount, dataBuffer);
   }
   __inline ErrorCode WINAPI AdxDiSnapStop(
      IN HANDLE  dioHandle,
      IN EventId id)
   {
      return s_bdaqLib.pfnDiSnapStop(dioHandle, id);
   }

   //-------------------------------------------------------
   // counter/timer APIs
   //-------------------------------------------------------
   __inline ErrorCode WINAPI AdxCounterReset(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long   counterCount )
   {
      return s_bdaqLib.pfnCounterReset(cntrHandle, counterStart, counterCount);
   }
   __inline ErrorCode WINAPI AdxEventCountStart(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long   counterCount )
   {
      return s_bdaqLib.pfnEventCountStart(cntrHandle, counterStart, counterCount);
   }
   __inline ErrorCode WINAPI AdxEventCountRead(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long	 counterCount,
      OUT long* eventCount )
   {
      return s_bdaqLib.pfnEventCountRead(cntrHandle, counterStart, counterCount, eventCount);
   }
   __inline ErrorCode WINAPI AdxOneShotStart(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long	 counterCount)
   {
      return s_bdaqLib.pfnOneShotStart(cntrHandle, counterStart, counterCount);
   }
   __inline ErrorCode WINAPI AdxTimerPulseStart(
      IN HANDLE  cntrHandle,
      IN long    counterStart,
      IN long	  counterCount)
   {
      return s_bdaqLib.pfnTimerPulseStart(cntrHandle, counterStart, counterCount);
   }
   __inline ErrorCode WINAPI AdxFrequencyMeasureStart(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long   counterCount )
   {
      return s_bdaqLib.pfnFrequencyMeasureStart(cntrHandle, counterStart, counterCount);
   }
   __inline ErrorCode WINAPI AdxFrequencyMeasureRead(
      IN HANDLE   cntrHandle,
      IN long     counterStart,
      IN long     counterCount,
      OUT double* frequency )
   {
      return s_bdaqLib.pfnFrequencyMeasureRead(cntrHandle, counterStart, counterCount, frequency);
   }
   __inline ErrorCode WINAPI AdxPwmInStart(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long   groupCount )
   {
      return s_bdaqLib.pfnPwmInStart(cntrHandle, counterStart, groupCount);
   }
   __inline ErrorCode WINAPI AdxPwmInRead(
      IN HANDLE   cntrHandle,
      IN long     counterStart,
      IN long     groupCount,
      OUT double* hiPeriod,
      OUT double* lowPeriod)
   {
      return s_bdaqLib.pfnPwmInRead(cntrHandle, counterStart, groupCount, hiPeriod, lowPeriod);
   }
   __inline ErrorCode WINAPI AdxPwmOutStart(
      IN HANDLE cntrHandle,
      IN long   counterStart,
      IN long   counterCount )
   {
      return s_bdaqLib.pfnPwmOutStart(cntrHandle, counterStart, counterCount);
   }
_BDAQ_END

// **********************************************************
// Bionic DAQ classes, for c++ only
// **********************************************************
#ifdef __cplusplus

// disable the Warning in VC6: identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786) 
#include <map>

_BDAQ_BEGIN

// **********************************************************
// Forward declare
// **********************************************************
class BDaqModule;
class BDaqAi;
class BDaqAo;
class BDaqDio;
class BDaqCntr;
class BDaqDevice;
class BDaqModuleManager;

// **********************************************************
// classes definition
// **********************************************************
class BDaqEvent
{
public:
   ErrorCode GetHandle(EventId id, HANDLE & eventHandle);
   long GetStatus(EventId id);
   long GetStatus(EventId id, long & statusRParam);
   void ClearFlag(EventId id, long flagLParam, long flagRParam);

protected: 
   BDaqEvent( ) 
   {
      m_parent = NULL;
   }
   void SetParent(BDaqModule * parent)
   {
      m_parent = parent;
   }

protected:
   BDaqModule * m_parent;
   friend class BDaqModule;
};
class BDaqProperty
{
public:
   long NotifyPropertyChanged; // whether or not notify the others the property was changed.

   // helper method to get the attribute of a property
   long GetLength(PropertyId id);
   PropertyAttribute GetAttribute(PropertyId id);

   // get single-value property
   ErrorCode Get(PropertyId id, int& data);
   ErrorCode Get(PropertyId id, long& data);
   ErrorCode Get(PropertyId id, double& data);
   ErrorCode Get(PropertyId id, MathInterval& data);

   // get array type property
   ErrorCode Get(PropertyId id, long count, BYTE data[]);
   ErrorCode Get(PropertyId id, long count, long data[]);
   ErrorCode Get(PropertyId id, long count, double data[]);
   ErrorCode Get(PropertyId id, long count, WCHAR data[]);

   // set single-value property
   ErrorCode Set(PropertyId id, int data); // we need an int overload to disambiguate for the immediate number case
   ErrorCode Set(PropertyId id, long data);
   ErrorCode Set(PropertyId id, double data);
   ErrorCode Set(PropertyId id, MathInterval data);

   // set array type property
   ErrorCode Set(PropertyId id, long count, BYTE data[]);
   ErrorCode Set(PropertyId id, long count, long data[]);
   ErrorCode Set(PropertyId id, long count, double data[]);
   ErrorCode Set(PropertyId id, long count, WCHAR data[]);

protected:
   BDaqProperty()
   {
      NotifyPropertyChanged = 0; // default is not sending property change event to other listeners.
      m_parent = NULL;
   }
   void SetParent(BDaqModule * parent)
   {
      m_parent = parent;
   }
protected:
   BDaqModule * m_parent;
   friend class BDaqModule;
};

class BDaqModule
{
public: // Methods
   __declspec( property( get = _get_Handle ) ) HANDLE Handle;
   HANDLE _get_Handle()
   {
      return m_moduleHandle;
   }

   __declspec( property( get = _get_Property ) ) BDaqProperty &Property;
   BDaqProperty & _get_Property( )
   {
      return m_property;
   }

   __declspec( property( get = _get_Event ) ) BDaqEvent &Event;
   BDaqEvent & _get_Event()
   {
      return m_event;
   }

protected:
   BDaqModule(HANDLE moduleHandle)
   {
      m_moduleHandle = moduleHandle;
      m_property.SetParent(this);
      m_event.SetParent(this);
   }
   virtual ~BDaqModule()
   {
   }
   virtual void Release()
   {
      delete this;
   }
protected:
   friend class BDaqModuleManager;
   HANDLE       m_moduleHandle;
   BDaqProperty m_property;
   BDaqEvent    m_event;
};

class BDaqModuleManager
{
public:
   BDaqModuleManager()
   {
   }
   virtual ~BDaqModuleManager()
   {
      ModuleCache::iterator itor;
      BDaqModule* module;
      while( !m_cachedModules.empty() )
      {
         itor   = m_cachedModules.begin();
         module = itor->second;
         m_cachedModules.erase(itor);
         if (module)
         {
            module->Release();
         }
      }
   }
   BDaqModule* Find(long key)
   {
      ModuleCache::const_iterator itor = m_cachedModules.find(key);
      return itor != m_cachedModules.end() ? itor->second : NULL;
   }
   void Remove(long key)
   {
      m_cachedModules.erase( key );
   }
   BDaqModule* & operator[](long key)
   {
      return m_cachedModules[key];
   }

protected:
   typedef std::map<long, BDaqModule*> ModuleCache;
   ModuleCache m_cachedModules;
};

class BDaqAi : public BDaqModule
{
public:

   // instant AI single channel methods
   ErrorCode Read(long channel, double& dataScaled);
   ErrorCode Read(long channel, short& dataRaw);   // for the device whose raw data is in 16bits format
   ErrorCode Read(long channel, long& dataRaw);    // for the device whose raw data is in 32bits format

   // instant AI multi channel methods
   ErrorCode Read(long chStart, long chCount, double dataScaled[]);
   ErrorCode Read(long chStart, long chCount, short dataRaw[], double dataScaled[]);   // for the device whose raw data is in 16bits format
   ErrorCode Read(long chStart, long chCount, long dataRaw[], double dataScaled[]) ;   // for the device whose raw data is in 32bits format

   // buffered AI methods
   ErrorCode BfdAiPrepare(long dataCount, void* * dataRaw);
   ErrorCode BfdAiRunOnce(bool asynchronous);
   ErrorCode BfdAiRun();
   ErrorCode BfdAiScaleData(void* dataRaw, double dataScaled[], long dataCount, long* chOffset);
   ErrorCode BfdAiStop();
   ErrorCode BfdAiRelease();
protected:
   friend class BDaqDevice;
   // internal used only
   BDaqAi(HANDLE aiHandle) : BDaqModule(aiHandle)
   {
   }
   virtual ~BDaqAi()
   {
   }
};

class BDaqAo : public BDaqModule
{
public:
   // instant AO single channel methods
   ErrorCode Write(long channel, double dataScaled);
   ErrorCode Write(long channel, short dataRaw);   // for the device whose raw data is in 16bits format
   ErrorCode Write(long channel, long dataRaw);    // for the device whose raw data is in 32bits format

   // instant AO multi channel methods
   ErrorCode Write(long chStart, long chCount, double dataScaled[]);
   ErrorCode Write(long chStart, long chCount, short dataRaw[]);   // for the device whose raw data is in 16bits format
   ErrorCode Write(long chStart, long chCount, long dataRaw[]);    // for the device whose raw data is in 32bits format

   // buffered AO methods
   ErrorCode BfdAoPrepare(long dataCount, void* * dataRaw);
   ErrorCode BfdAoRunOnce(bool asynchronous);
   ErrorCode BfdAoRun();
   ErrorCode BfdAoScaleData(double dataScaled[], void* dataRaw, long dataCount, long* chOffset);
   ErrorCode BfdAoStop(long action);
   ErrorCode BfdAoRelease();

protected:
   friend class BDaqDevice;

   // internal used only
   BDaqAo(HANDLE aoHandle) : BDaqModule(aoHandle)
   {
   }
   virtual ~BDaqAo()
   {
   }
};

class BDaqDio : public BDaqModule
{
public:
   // Instant DI/O methods
   ErrorCode DiRead(long port, BYTE& data);
   ErrorCode DiRead(long portStart, long portCount, BYTE data[]);
   ErrorCode DoWrite(long port, BYTE data);
   ErrorCode DoWrite(long portStart, long portCount, BYTE data[]);
   ErrorCode DoRead(long port, BYTE& data);
   ErrorCode DoRead(long portStart, long portCount, BYTE data[]);

   // DI snap methods
   ErrorCode DiSnapStart(EventId id, long portStart, long portCount, BYTE** buffer);
   ErrorCode DiSnapStop(EventId id);

   // Buffered DI methods
   ErrorCode BfdDiPrepare(long dataCount, void* * data);
   ErrorCode BfdDiRunOnce(bool asynchronous);
   ErrorCode BfdDiRun();
   ErrorCode BfdDiStop();
   ErrorCode BfdDiRelease();

   // Buffered DO methods
   ErrorCode BfdDoPrepare(long dataCount, void* * data);
   ErrorCode BfdDoRunOnce(bool asynchronous);
   ErrorCode BfdDoRun();
   ErrorCode BfdDoStop(long action);
   ErrorCode BfdDoRelease();

protected:
   friend class BDaqDevice;

   // internal used only
   BDaqDio(HANDLE dioHandle) : BDaqModule(dioHandle)
   {
   }
   virtual ~BDaqDio()
   {
   }
};

class BDaqCntr : public BDaqModule
{
public:
   // Common methods
   ErrorCode Reset(long cntrStart, long cntrCount);

   // Event Counting methods
   ErrorCode EventCountStart(long cntrStart, long cntrCount);
   ErrorCode EventCountRead(long cntr, long& cntrValue);
   ErrorCode EventCountRead(long cntrStart, long cntrCount, long cntrValue[]);

   // One-Shot methods
   ErrorCode OneShotStart(long cntrStart, long cntrCount);

   // Timer/Pulse methods
   ErrorCode TimerPulseStart(long cntrStart, long cntrCount);

   // Frequency Measurement methods
   ErrorCode FreqMeasureStart(long cntrStart, long cntrCount);
   ErrorCode FreqMeasureRead(long cntr, double& frequency);
   ErrorCode FreqMeasureRead(long cntrStart, long cntrCount, double frequency[]);

   // Pulse width measurement
   ErrorCode PwmInStart(long cntrStart, long groupCount);
   ErrorCode PwmInRead(long cntr, double& hiPeriod, double& lowPeriod);
   ErrorCode PwmInRead(long cntrStart, long groupCount, double hiPeriod[], double lowPeriod[]);

   // Pulse width modulation
   ErrorCode PwmOutStart(long cntrStart, long cntrCount);

protected:
   // internal used only
   friend class BDaqDevice;
   BDaqCntr(HANDLE cntrHandle) : BDaqModule(cntrHandle)
   {
   }
   virtual ~BDaqCntr()
   {
   }
};

class BDaqDevice : public BDaqModule
{
public:
   // Methods
   static ErrorCode Open(int deviceNumber, AccessMode mode, BDaqDevice* & device);
   static ErrorCode Open(WCHAR const *deviceDescription, AccessMode mode, BDaqDevice* & device);

   ErrorCode GetModule(long index, BDaqAi * & ai);
   ErrorCode GetModule(long index, BDaqAo * & ao);
   ErrorCode GetModule(long index, BDaqDio * & dio);
   ErrorCode GetModule(long index, BDaqCntr * & cntr);
   ErrorCode GetModule(ModuleType type, long index, BDaqModule * & module);

   ErrorCode RefreshProperties();
   ErrorCode ShowModalDialog(HWND parentWnd, long dataSource);
   ErrorCode ShowPopupDialog(HWND parentWnd, long dataSource, HWND* dlgWnd);
   ErrorCode ShowEmbedDialog(HWND parentWnd, RECT* wndRect, long dataSource, HWND* dlgWnd);
   ErrorCode Reset(long state);
   void Close();

   __declspec( property( get = _get_DeviceNumber ) ) long DeviceNumber;
   long _get_DeviceNumber( )
   {
      return m_deviceNumber;
   }

   __declspec( property( get = _get_DeviceMode   ) ) AccessMode Mode;
   AccessMode _get_DeviceMode()
   {
      return m_deviceMode;
   }

protected:// methods for internal using only

   // user can only initializes a new instance by the static method 'Open' 
   BDaqDevice(HANDLE deviceHandle, int deviceNumber, AccessMode mode) : BDaqModule(deviceHandle)
   {
      m_deviceNumber = deviceNumber;
      m_deviceMode = mode;
   }
   
   // user can only delete an instance by the method 'close' 
   virtual ~BDaqDevice()
   {
   }

   virtual void Release()
   {
      if (InterlockedDecrement(&m_refCount) == 0)
      {
         GetCachedDevices().Remove(GetDeviceKey(m_deviceNumber, m_deviceMode));
      
         // close the device handle
         AdxDeviceClose(this->Handle);

         delete this;
      }
   }

   static long GetModuleKey(ModuleType type, long index)
   {
      return (long)type | (index << 16);
   }

   static long GetDeviceKey(int deviceNumber, AccessMode mode)
   {
      if (mode == ModeWriteWithReset)
      {
         mode = ModeWrite;
      }
      return (long)deviceNumber | ((long)mode << 16);
   }

   static BDaqModuleManager& GetCachedDevices()
   {
      // cached devices, used to implement the 'singleton' for the devices 
      // which own the same device number and access mode. 
      static BDaqModuleManager cachedDevices;
      return cachedDevices;
   }
protected:   // fields
   friend class BDaqModuleManager;
   long         m_deviceNumber;
   AccessMode   m_deviceMode;
   long         m_refCount;

   // module map, used to manage all the modules belongs to the device
   BDaqModuleManager m_cachedModules;
};

class BDaqEnumerator
{
public:
   struct EnumDeviceArgs 
   {
      // current device information
      long        DeviceNumber;
      WCHAR      *Description;
      BDaqDevice *Device; // Note: the device will be closed after the delegate returned.
   }; 

   struct EnumModuleArgs
   {
      // parent device information
      long        DeviceNumber;
      WCHAR      *Description;
      BDaqDevice *Device; // Note: the device will be closed after the delegate returned.

      // current module information
      int         ModuleIndex;
      BDaqModule *Module;
   };

   typedef BOOL (CALLBACK * EnumDeviceFunc)(EnumDeviceArgs *args, void *userParam);
   typedef BOOL (CALLBACK * EnumModuleFunc)(EnumModuleArgs *args, void *userParam);

   // Enumerate all devices which are on the system
   static void EnumerateDevices(EnumDeviceFunc userFunc, void *userParam)
   {
      EnumerateDevices(DaqAny, userFunc, userParam);
   }

   // Enumerate all devices which are on the system and support the specified function
   static void EnumerateDevices(ModuleType typeWanted, EnumDeviceFunc userFunc, void *userParam)
   {
      long deviceIndex = 0;
      long deviceNumber;
      WCHAR descBuff[256]; // device description buffer
      ErrorCode ret;
      BOOL keepGoing = true;

      // the user doesn't care about the supported function of the device
      if (typeWanted == DaqAny)
      {
         typeWanted = DaqDevice;
      }

      while(keepGoing)
      {
         ret = AdxDeviceGetLinkageInfo(-1, deviceIndex++, &deviceNumber, descBuff, NULL);
         if (deviceNumber == -1)
         {
            break; // no more device
         }
         if (ret != Success)
         {
            continue; // device does not be on the system 
         }

         // open the device to do further check
         BDaqDevice *device;
         if (BDaqDevice::Open(deviceNumber, ModeRead, device) == Success)
         {
            // Does the device support the function the user wanted?
            BDaqModule *module;
            if (typeWanted == DaqDevice || device->GetModule(typeWanted, 0, module) == Success)
            {
               // Call the passed-in delegate to let the user do further action
               EnumDeviceArgs args;
               args.DeviceNumber = deviceNumber;
               args.Description  = descBuff;
               args.Device       = device;
               keepGoing = userFunc(&args, userParam);
            }
            // close the device
            device->Close();
         }
      } 
   }

   // Enumerate all modules which are on the system
   static void EnumerateModules(ModuleType typeWanted, EnumModuleFunc userFunc, void *userParam)
   {
      EnumModuleCtx ctx = { typeWanted, userFunc, userParam };

      EnumerateDevices(typeWanted, (EnumDeviceFunc)EnumDeviceCallBack, (void*)&ctx);
   }

private:
   struct EnumModuleCtx
   {
      ModuleType      typeWanted;
      EnumModuleFunc  userFunc;
      void           *userParam;
   };

   static BOOL CALLBACK EnumDeviceCallBack(EnumDeviceArgs *devArgs, void *userParam)
   {
      EnumModuleCtx * ctx = (EnumModuleCtx*)userParam;

      // save parent device information into our enumeration args
      EnumModuleArgs modArgs = {devArgs->DeviceNumber, devArgs->Description, devArgs->Device};
      
      // enumerating all wanted modules
      BDaqModule *module;
      for (int i = 0; true; ++i)
      {
         if (devArgs->Device->GetModule(ctx->typeWanted, i, module) != Success)
         {
            break;
         }
         modArgs.ModuleIndex = i;
         modArgs.Module = module;
         if (!ctx->userFunc(&modArgs, ctx->userParam))
         {
            return false; // break the enumeration
         }
      }
      return true;
   }
};

// *********************************************************************
// inline functions of BDaqEvent
// *********************************************************************
inline ErrorCode BDaqEvent::GetHandle(EventId id, HANDLE& eventHandle)
{
   return AdxEventGetHandle(m_parent->Handle, id, &eventHandle);
}
inline long BDaqEvent::GetStatus(EventId id)
{
   long statusLParam = 0;
   AdxEventGetLastStatus(m_parent->Handle, id, &statusLParam, NULL);
   return statusLParam;
}
inline long BDaqEvent::GetStatus(EventId id, long & statusRParam)
{
   long statusLParam = 0;
   statusRParam = 0;
   AdxEventGetLastStatus(m_parent->Handle, id, &statusLParam, &statusRParam);
   return statusLParam;
}
inline void BDaqEvent::ClearFlag(EventId id, long flagLParam, long flagRParam)
{
   AdxEventClearFlag(m_parent->Handle, id, flagLParam, flagRParam);
}

// *********************************************************************
// inline functions of BDaqProperty
// *********************************************************************
inline long BDaqProperty::GetLength(PropertyId id)
{
   long dataLength;
   AdxPropertyRead(m_parent->Handle, id, 0, NULL, &dataLength, NULL);
   return dataLength;
}
inline PropertyAttribute BDaqProperty::GetAttribute(PropertyId id)
{
   long propAttr = 0;
   AdxPropertyRead(m_parent->Handle, id, 0, NULL, NULL, &propAttr);
   return (PropertyAttribute)propAttr;
}

// get single-value property
inline ErrorCode BDaqProperty::Get(PropertyId id, int& data)
{
   return AdxPropertyRead(m_parent->Handle, id, sizeof(int), &data, NULL, NULL);
}

inline ErrorCode BDaqProperty::Get(PropertyId id, long& data)
{
   return AdxPropertyRead(m_parent->Handle, id, sizeof(long), &data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, double& data)
{
   return AdxPropertyRead(m_parent->Handle, id, sizeof(double), &data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, MathInterval& data)
{
   return AdxPropertyRead(m_parent->Handle, id, sizeof(MathInterval), &data, NULL, NULL);
}

// get array type property
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, BYTE data[])
{
   return AdxPropertyRead(m_parent->Handle, id, count * sizeof(BYTE), data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, long data[])
{
   return AdxPropertyRead(m_parent->Handle, id, count * sizeof(long), data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, double data[])
{
   return AdxPropertyRead(m_parent->Handle, id, count * sizeof(double), data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, WCHAR data[])
{
   return AdxPropertyRead(m_parent->Handle, id, count * sizeof(WCHAR), data, NULL, NULL);
}

// set single-value property
inline ErrorCode BDaqProperty::Set(PropertyId id, int data)
{  
   // we need this int overload to disambiguate for the immediate number case
   return AdxPropertyWrite(m_parent->Handle, id, sizeof(int), (void*)&data, NotifyPropertyChanged);
}

inline ErrorCode BDaqProperty::Set(PropertyId id, long data)
{
   return AdxPropertyWrite(m_parent->Handle, id, sizeof(long), (void*)&data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, double data)
{
   return AdxPropertyWrite(m_parent->Handle, id, sizeof(double), (void*)&data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, MathInterval data)
{
   return AdxPropertyWrite(m_parent->Handle, id, sizeof(MathInterval), (void*)&data, NotifyPropertyChanged);
}

// set array type property
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, BYTE data[])
{
   return AdxPropertyWrite(m_parent->Handle, id, count * sizeof(BYTE), data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, long data[])
{
   return AdxPropertyWrite(m_parent->Handle, id, count * sizeof(long), data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, double data[])
{
   return AdxPropertyWrite(m_parent->Handle, id, count * sizeof(double), data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, WCHAR data[])
{
   return AdxPropertyWrite(m_parent->Handle, id, count * sizeof(WCHAR), data, NotifyPropertyChanged);
}

// *********************************************************************
// inline functions of BDaqAi
// *********************************************************************
// instant AI single channel methods
inline ErrorCode BDaqAi::Read(long channel, double& dataScaled)
{
   return AdxAiReadSamples(this->Handle, channel, 1, NULL, &dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAi::Read(long channel, short& dataRaw)
{
   return AdxAiReadSamples(this->Handle, channel, 1, &dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAi::Read(long channel, long& dataRaw)
{
   return AdxAiReadSamples(this->Handle, channel, 1, &dataRaw, NULL);
}

// instant AI multi channel methods
inline ErrorCode BDaqAi::Read(long chStart, long chCount, double dataScaled[])
{
   return AdxAiReadSamples(this->Handle, chStart, chCount, NULL, dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAi::Read(long chStart, long chCount, short dataRaw[], double dataScaled[]) 
{
   return AdxAiReadSamples(this->Handle, chStart, chCount, dataRaw, dataScaled);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAi::Read(long chStart, long chCount, long dataRaw[], double dataScaled[]) 
{
   return AdxAiReadSamples(this->Handle, chStart, chCount, dataRaw, dataScaled);
}

// buffered AI methods
inline ErrorCode BDaqAi::BfdAiPrepare(long dataCount, void* * dataRaw)
{
   return AdxBufferedAiPrepare(this->Handle, dataCount, dataRaw);
}
inline ErrorCode BDaqAi::BfdAiRunOnce(bool asynchronous)
{
   return AdxBufferedAiRunOnce(this->Handle, asynchronous ? 1 : 0);
}
inline ErrorCode BDaqAi::BfdAiRun()
{
   return AdxBufferedAiRun(this->Handle);
}
inline ErrorCode BDaqAi::BfdAiScaleData(void* dataRaw, double dataScaled[], long dataCount, long* chOffset)
{
   return AdxBufferedAiScaleData(this->Handle, dataRaw, dataScaled, dataCount, chOffset);
}
inline ErrorCode BDaqAi::BfdAiStop()
{
   return AdxBufferedAiStop(this->Handle);
}
inline ErrorCode BDaqAi::BfdAiRelease()
{
   return AdxBufferedAiRelease(this->Handle);
}

// *********************************************************************
// inline functions of BDaqAo
// *********************************************************************
// instant AO single channel methods
inline ErrorCode BDaqAo::Write(long channel, double dataScaled)
{
   return AdxAoWriteSamples(this->Handle, channel, 1, NULL, (double*)&dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAo::Write(long channel, short dataRaw)
{
   return AdxAoWriteSamples(this->Handle, channel, 1, (void*)&dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAo::Write(long channel, long dataRaw)
{
   return AdxAoWriteSamples(this->Handle, channel, 1, (void*)&dataRaw, NULL);
}

// instant AO multi channel methods
inline ErrorCode BDaqAo::Write(long chStart, long chCount, double dataScaled[])
{
   return AdxAoWriteSamples(this->Handle, chStart, chCount, NULL, dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAo::Write(long chStart, long chCount, short dataRaw[])
{
   return AdxAoWriteSamples(this->Handle, chStart, chCount, dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAo::Write(long chStart, long chCount, long dataRaw[])
{
   return AdxAoWriteSamples(this->Handle, chStart, chCount, dataRaw, NULL);
}

// buffered AO methods
inline ErrorCode BDaqAo::BfdAoPrepare(long dataCount, void* * dataRaw)
{
   return AdxBufferedAoPrepare(this->Handle, dataCount, dataRaw);
}
inline ErrorCode BDaqAo::BfdAoRunOnce(bool asynchronous)
{
   return AdxBufferedAoRunOnce(this->Handle, asynchronous ? 1 : 0);
}
inline ErrorCode BDaqAo::BfdAoRun()
{
   return AdxBufferedAoRun(this->Handle);
}
inline ErrorCode BDaqAo::BfdAoScaleData(double dataScaled[], void* dataRaw, long dataCount, long* chOffset)
{
   return AdxBufferedAoScaleData(this->Handle, dataScaled, dataRaw, dataCount, chOffset);
}
inline ErrorCode BDaqAo::BfdAoStop(long action)
{
   return AdxBufferedAoStop(this->Handle, action);
}
inline ErrorCode BDaqAo::BfdAoRelease()
{
   return AdxBufferedAoRelease(this->Handle);
}

// *********************************************************************
// inline functions of BDaqDio
// *********************************************************************
// Instant DI/O methods
inline ErrorCode BDaqDio::DiRead(long port, BYTE& data)
{
   return AdxDiReadPorts(this->Handle, port, 1, &data);
}
inline ErrorCode BDaqDio::DiRead(long portStart, long portCount, BYTE data[])
{
   return AdxDiReadPorts(this->Handle, portStart, portCount, data);
}

inline ErrorCode BDaqDio::DoWrite(long port, BYTE data)
{
   return AdxDoWritePorts(this->Handle, port, 1, &data);
}
inline ErrorCode BDaqDio::DoWrite(long portStart, long portCount, BYTE data[])
{
   return AdxDoWritePorts(this->Handle, portStart, portCount, data);
}

inline ErrorCode BDaqDio::DoRead(long port, BYTE& data)
{
   return AdxDoReadBackPorts(this->Handle, port, 1, &data);
}
inline ErrorCode BDaqDio::DoRead(long portStart, long portCount, BYTE data[])
{
   return AdxDoReadBackPorts(this->Handle, portStart, portCount, data);
}

// DI snap methods
inline ErrorCode BDaqDio::DiSnapStart(EventId id, long portStart, long portCount, BYTE** buffer)
{
   return AdxDiSnapStart(this->Handle, id, portStart, portCount, buffer);
}
inline ErrorCode BDaqDio::DiSnapStop(EventId id)
{
   return AdxDiSnapStop(this->Handle, id);
}

// Buffered DI methods
inline ErrorCode BDaqDio::BfdDiPrepare(long dataCount, void* * data)
{
   return AdxBufferedDiPrepare(this->Handle, dataCount, data);
}
inline ErrorCode BDaqDio::BfdDiRunOnce(bool asynchronous)
{
   return AdxBufferedDiRunOnce(this->Handle, asynchronous ? 1 : 0);
}
inline ErrorCode BDaqDio::BfdDiRun()
{
   return AdxBufferedDiRun(this->Handle);
}
inline ErrorCode BDaqDio::BfdDiStop()
{
   return AdxBufferedDiStop(this->Handle);
}
inline ErrorCode BDaqDio::BfdDiRelease()
{
   return AdxBufferedDiRelease(this->Handle);
}

// Buffered DO methods
inline ErrorCode BDaqDio::BfdDoPrepare(long dataCount, void* * data)
{
   return AdxBufferedDoPrepare(this->Handle, dataCount, data);
}
inline ErrorCode BDaqDio::BfdDoRunOnce(bool asynchronous)
{
   return AdxBufferedDoRunOnce(this->Handle, asynchronous ? 1 : 0);
}
inline ErrorCode BDaqDio::BfdDoRun()
{
   return AdxBufferedDoRun(this->Handle);
}
inline ErrorCode BDaqDio::BfdDoStop(long action)
{
   return AdxBufferedDoStop(this->Handle, action);
}
inline ErrorCode BDaqDio::BfdDoRelease()
{
   return AdxBufferedDoRelease(this->Handle);
}

// *********************************************************************
// inline functions of BDaqCntr
// *********************************************************************
// Common methods
inline ErrorCode BDaqCntr::Reset(long cntrStart, long cntrCount)
{
   return AdxCounterReset(this->Handle, cntrStart, cntrCount);
}

// Event Counting methods
inline ErrorCode BDaqCntr::EventCountStart(long cntrStart, long cntrCount)
{
   return AdxEventCountStart(this->Handle, cntrStart, cntrCount);
}
inline ErrorCode BDaqCntr::EventCountRead(long cntr, long& cntrValue)
{
   return AdxEventCountRead(this->Handle, cntr, 1, &cntrValue);
}
inline ErrorCode BDaqCntr::EventCountRead(long cntrStart, long cntrCount, long cntrValue[])
{
   return AdxEventCountRead(this->Handle, cntrStart, cntrCount, cntrValue);
}

// One-Shot methods
inline ErrorCode BDaqCntr::OneShotStart(long cntrStart, long cntrCount)
{
   return AdxOneShotStart(this->Handle, cntrStart, cntrCount);
}

// Timer/Pulse methods
inline ErrorCode BDaqCntr::TimerPulseStart(long cntrStart, long cntrCount)
{
   return AdxTimerPulseStart(this->Handle, cntrStart, cntrCount);
}

// Frequency Measurement methods
inline ErrorCode BDaqCntr::FreqMeasureStart(long cntrStart, long cntrCount)
{
   return AdxFrequencyMeasureStart(this->Handle, cntrStart, cntrCount);
}
inline ErrorCode BDaqCntr::FreqMeasureRead(long cntr, double& frequency)
{
   return AdxFrequencyMeasureRead(this->Handle, cntr, 1, &frequency);
}
inline ErrorCode BDaqCntr::FreqMeasureRead(long cntrStart, long cntrCount, double frequency[])
{
   return AdxFrequencyMeasureRead(this->Handle, cntrStart, cntrCount, frequency);
}

// Pulse width measurement
inline ErrorCode BDaqCntr::PwmInStart(long cntrStart, long groupCount)
{
   return AdxPwmInStart(this->Handle, cntrStart, groupCount);
}
inline ErrorCode BDaqCntr::PwmInRead(long cntr, double& hiPeriod, double& lowPeriod)
{
   return AdxPwmInRead(this->Handle, cntr, 1, &hiPeriod, &lowPeriod);
}
inline ErrorCode BDaqCntr::PwmInRead(long cntrStart, long groupCount, double hiPeriod[], double lowPeriod[])
{
   return AdxPwmInRead(this->Handle, cntrStart, groupCount, hiPeriod, lowPeriod);
}

// Pulse width modulation
inline ErrorCode BDaqCntr::PwmOutStart(long cntrStart, long cntrCount)
{
   return AdxPwmOutStart(this->Handle, cntrStart, cntrCount);
}

// *********************************************************************
// inline functions of BDaqDevice
// *********************************************************************
inline ErrorCode BDaqDevice::Open(int deviceNumber, AccessMode mode, BDaqDevice* & device)
{
   // 'Singleton': all devices which own same device number and access mode
   //  will reference to the same object.
   ErrorCode ret = Success;
   long key = GetDeviceKey(deviceNumber, mode);
   device = (BDaqDevice*)GetCachedDevices().Find(key);
   if ( device )
   {
      InterlockedIncrement(&device->m_refCount);
   } else{
      HANDLE deviceHandle = NULL;
      ret = AdxDeviceOpen(deviceNumber, mode, &deviceHandle);
      if (ret == Success)
      {
         device = new BDaqDevice(deviceHandle, deviceNumber, mode);
         if ( device != NULL )
         {
            device->m_refCount = 1;
            GetCachedDevices()[key] = device;
         } else {
            ret = ErrorMemoryNotEnough;
         }
      }
   }
   return ret;
}
inline ErrorCode BDaqDevice::Open(WCHAR const * deviceDescription, AccessMode mode, BDaqDevice* & device)
{
   device = NULL;
   if (deviceDescription == NULL)
   {
      return ErrorBufferIsNull;
   }

   WCHAR curDevDesc[256] = {0};
   long  deviceNumber= -1;
   long deviceIndex = 0;
   long subDevCount = 0;
   while( true )
   {
      ErrorCode ret = AdxDeviceGetLinkageInfo(-1, deviceIndex, &deviceNumber, curDevDesc, NULL);
      if ( deviceNumber == -1 )
      {
         // no more device
         return ErrorDeviceNotExist;
      }
      if (ret == Success && wcscmp(curDevDesc, deviceDescription) == 0 )
      {
         return Open(deviceNumber, mode, device);
      }
      ++deviceIndex;
   }
}

inline ErrorCode BDaqDevice::GetModule(long index, BDaqAi * & ai)
{
   return GetModule(DaqAi, index, (BDaqModule*&)ai);
}
inline ErrorCode BDaqDevice::GetModule(long index, BDaqAo * & ao)
{
   return GetModule(DaqAo, index, (BDaqModule*&)ao);
}
inline ErrorCode BDaqDevice::GetModule(long index, BDaqDio * & dio)
{
   return GetModule(DaqDio, index, (BDaqModule*&)dio);
}
inline ErrorCode BDaqDevice::GetModule(long index, BDaqCntr * & cntr)
{
   return GetModule(DaqCounter, index, (BDaqModule*&)cntr);
}

inline ErrorCode BDaqDevice::GetModule(ModuleType type, long index, BDaqModule* & module)
{
   module = NULL;
   if (this->Handle == NULL)
   {
      return ErrorHandleNotValid;
   }

   ErrorCode ret = Success;
   long key = GetModuleKey(type, index);
   module = m_cachedModules[key];
   if (!module)
   {
      HANDLE moduleHandle;
      ret = AdxDeviceGetModuleHandle(this->Handle, type, index, &moduleHandle);
      if (ret == Success)
      {
         switch (type)
         {
         case DaqAi:
            module = new BDaqAi(moduleHandle);
            break;
         case DaqAo:
            module = new BDaqAo(moduleHandle);
            break;
         case DaqDio:
            module = new BDaqDio(moduleHandle);
            break;
         case DaqCounter:
            module = new BDaqCntr(moduleHandle);
            break;
         }
         if (module != NULL) 
         {
            m_cachedModules[key] = module;
         } else {
            ret = ErrorMemoryNotEnough;
         }
      }
   }
   return ret;
}

inline ErrorCode BDaqDevice::RefreshProperties()
{
   return AdxDeviceRefreshProperties(this->Handle);
}

inline ErrorCode BDaqDevice::ShowModalDialog(HWND parentWnd, long dataSource)
{
   HWND dlgWnd;
   return AdxDeviceShowConfigDialogBox(this->Handle, parentWnd, NULL, dataSource, 1, &dlgWnd);
}
inline ErrorCode BDaqDevice::ShowPopupDialog(HWND parentWnd, long dataSource, HWND* dlgWnd)
{
   return AdxDeviceShowConfigDialogBox(this->Handle, parentWnd, NULL, dataSource, 0, dlgWnd);
}
inline ErrorCode BDaqDevice::ShowEmbedDialog(HWND parentWnd, RECT* wndRect, long dataSource, HWND* dlgWnd)
{
   return AdxDeviceShowConfigDialogBox(this->Handle, parentWnd, wndRect, dataSource, 0, dlgWnd);
}
inline ErrorCode BDaqDevice::Reset(long state)
{
   return AdxDeviceReset(this->Handle, state);
}

inline void BDaqDevice::Close()
{
   Release();
}

_BDAQ_END

#pragma warning(default : 4786)

#endif

#endif
