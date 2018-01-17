//---------------------------------------------------------------------------

#ifndef ProtocolH
#define ProtocolH
//---------------------------------------------------------------------------
#include <Forms.hpp>
#include <stdio.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TPr : public TForm
{
__published:	// IDE-managed Components
	TMemo *mProtocol;
	TCheckBox *cbProtocolToFile;
	TTimer *Timer1;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	AnsiString FileName;	// ��� ����� ���������
	TCriticalSection* cs;
	FILE *file;
	TStringList* L;
	TPr(void);
	static TPr* Instance;
	void Add(AnsiString _msg);
	void Clear0(void);
	void Show0(void);
	bool Visible0(void);
public:		// User declarations
	void static SendToProtocol(AnsiString _msg);	// ��������� ������ � ��������
	void static pr(AnsiString _msg);	// ��������� ������ � ��������
	void static Clear(void);
	void static Dispose(void);
	void static Show(void);
	bool static Visible(void);
};

//---------------------------------------------------------------------------
#endif
