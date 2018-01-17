//---------------------------------------------------------------------------

#ifndef SolidGroupH
#define SolidGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <math.h>
#include <windows.h>
#include <vector>
#include <map>
#include "TeeGDIPlus.hpp"

#include "DataModule.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSolidGroupForm : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cbSolidGroup;
	TComboBox *cbTypeSize;
	TButton *bAddInserted;
	TButton *bAddCurrentTube;
	TLabel *Label1;
	TLabel *Label2;
	TStatusBar *StatusBarBottom;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bAddInsertedClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall bAddCurrentTubeClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	vector<TEdit*> par;						// ������ ������������ ��������� ��� ��������� �����
	SolidGroups::Tube *t;				// �������� ������� �����
	SolidGroups::Tube tube_current;		// �����, ������� ������ ����
	vector <double> coord;				// ������ ���������� ������ ���������, 8 ��������
	int numCoord;
public:		// User declarations
	void Send_Tube(SolidGroups::Tube temp_tube);
	__fastcall TSolidGroupForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSolidGroupForm *SolidGroupForm;
//---------------------------------------------------------------------------
#endif
