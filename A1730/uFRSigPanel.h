// ---------------------------------------------------------------------------

#ifndef uFRSigPanelH
#define uFRSigPanelH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Signal.h"

// ---------------------------------------------------------------------------
class TFRSigPanel : public TFrame
{
__published: // IDE-managed Components

	TPanel *Panel1;

private: // User declarations
	CSignal* signal;

public: // User declarations

	__fastcall TFRSigPanel(TComponent* Owner, CSignal* _signal);
	void CheckSignal(void);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFRSigPanel *FRSigPanel;
// ---------------------------------------------------------------------------
#endif
