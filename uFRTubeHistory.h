//---------------------------------------------------------------------------

#ifndef uFRTubeHistoryH
#define uFRTubeHistoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFRTubeHistory : public TFrame
{
__published:	// IDE-managed Components
	TLabel *lCut2;
	TPanel *pSolidGroup;
	TPanel *lResult;
	TLabel *lCut1;
	TBevel *Bevel1;
	TLabel *lNN;
	TPanel *pNN;
	void __fastcall FrameResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFRTubeHistory(TComponent* Owner);
	void Set(AnsiString _TubeNumber,
		AnsiString _SolidGroup,
		TColor _SolidGroupColor,
		AnsiString _Result,
		TColor _ResultColor,
		AnsiString _cut1,
		AnsiString _cut2);
	void Clear();
	void CopyFrom(TFRTubeHistory* _th);
	AnsiString GetTubeNumber(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRTubeHistory *FRTubeHistory;
//---------------------------------------------------------------------------
#endif
