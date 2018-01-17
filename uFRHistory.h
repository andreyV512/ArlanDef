// ---------------------------------------------------------------------------

#ifndef uFRHistoryH
#define uFRHistoryH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "uFRTubeHistory.h"

// ---------------------------------------------------------------------------
class TFRHistory : public TFrame {
__published: // IDE-managed Components

	TFRTubeHistory *FRTubeHistory1;
	TFRTubeHistory *FRTubeHistory2;
	TFRTubeHistory *FRTubeHistory3;
	TLabel *Label1;

	void __fastcall FrameResize(TObject *Sender);

private: // User declarations

public: // User declarations
	__fastcall TFRHistory(TComponent* Owner);
	void Add(AnsiString _TubeNumber,AnsiString _SolidGroup, TColor _SolidGroupColor, AnsiString _Result,
		TColor _ResultColor, AnsiString _cut1, AnsiString _cut2);
	void Clear();
		};

// ---------------------------------------------------------------------------
extern PACKAGE TFRHistory *FRHistory;
// ---------------------------------------------------------------------------
#endif
