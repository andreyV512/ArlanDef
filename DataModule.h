//---------------------------------------------------------------------------

#ifndef DataModuleH
#define DataModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "IniFiles.hpp"

#include "clustering.h"

//---------------------------------------------------------------------------
class TDM : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOCon;
	TDataSource *DataDefect;
	TDataSource *DataTube;
	TADOTable *TubeTable;
	TADOTable *DefectTable;
	TADOQuery *QueryThick;
	TLargeintField *DefectTableID_Tube;
	TWordField *DefectTableZone;
	TFloatField *DefectTableCrossDefect;
	TFloatField *DefectTableLineDefect;
	TADOQuery *Find;
	TDataSource *DataFind;
	TADOTable *TEtalons;
	TDataSource *DataEtalons;
	TSmallintField *TEtalonsTypeSize;
	TADOQuery *QEtalons;
	TMemoField *TEtalonsName;
	TLargeintField *TubeTableID_Tube;
	TWideStringField *TubeTableResult;
	TWideStringField *TubeTableSolidGroup;
	TDateTimeField *TubeTableDate;
	TFloatField *TubeTableMinThickness;
	TStringField *TubeTableCrossResult;
	TStringField *TubeTableLineResult;
	TFloatField *TubeTableLength;
	TStringField *TubeTableThicknessResult;
	TIntegerField *TubeTableTypeSize;
	TBooleanField *TubeTableisEtalon;
	TIntegerField *TubeTableDefectoscoperID;
	TIntegerField *TubeTableTubeGroup;
	TIntegerField *TubeTableCustomerID;
	TADOTable *tTemp;
	TAutoIncField *tTempID;
	TWideStringField *tTempName;
	TSmallintField *tTempp1;
	TSmallintField *tTempp2;
	TSmallintField *tTempp3;
	TSmallintField *tTempp4;
	TSmallintField *tTempp5;
	TSmallintField *tTempp6;
	TSmallintField *tTempp7;
	TSmallintField *tTempp8;
	TSmallintField *tTempTypeSize;
	TDateTimeField *tTempDate;
	TADOTable *Table;
	TLargeintField *TubeTableNumberTube;
	TDateTimeField *TEtalonsDate;
	TADOTable *tableASU;
	TDataSource *tASU;
	TAutoIncField *tableASUAI;
	TIntegerField *tableASUN_TR_URAN;
	TDateTimeField *tableASUDAT_T;
	TDateTimeField *tableASUDAT_TIM;
	TSmallintField *tableASUPBR_1;
	TSmallintField *tableASUP2C_1;
	TSmallintField *tableASUPBR_2;
	TSmallintField *tableASUP2C_2;
	TSmallintField *tableASUPBR_3;
	TSmallintField *tableASUP2C_3;
	TSmallintField *tableASUL_URAN;
	TSmallintField *tableASUREZ1;
	TSmallintField *tableASUREZ2;
	TSmallintField *tableASUPRIZN_URAN;
	TSmallintField *tableASUPRIZN_URAN_MUFTA;
	TStringField *tableASUPR_GROUP_URAN;
	TStringField *tableASUPR_GROUP_MUFTA;
	TSmallintField *tableASUDEF_0;
	TSmallintField *tableASUDEF_1;
	TSmallintField *tableASUDEF_2;
	TSmallintField *tableASUDEF_3;
	TSmallintField *tableASUDEF_4;
	TSmallintField *tableASUDEF_5;
	TSmallintField *tableASUDEF_6;
	TSmallintField *tableASUDEF_7;
	TSmallintField *tableASUDEF_8;
	TSmallintField *tableASUDEF_9;
	TSmallintField *tableASUDEF_10;
	TSmallintField *tableASUDEF_11;
	TSmallintField *tableASUDEF_12;
	TSmallintField *tableASUDEF_13;
	TSmallintField *tableASUDEF_14;
	TSmallintField *tableASUDEF_15;
	TSmallintField *tableASUDEF_16;
	TSmallintField *tableASUDEF_17;
	TSmallintField *tableASUDEF_18;
	TSmallintField *tableASUDEF_19;
	TSmallintField *tableASUDEF_20;
	TSmallintField *tableASUDEF_21;
	TSmallintField *tableASUDEF_22;
	TSmallintField *tableASUDEF_23;
	TSmallintField *tableASUDEF_24;
	TSmallintField *tableASUDEF_25;
	TSmallintField *tableASUDEF_26;
	TSmallintField *tableASUDEF_27;
	TSmallintField *tableASUDEF_28;
	TSmallintField *tableASUDEF_29;
	TSmallintField *tableASUDEF_30;
	TSmallintField *tableASUDEF_31;
	TSmallintField *tableASUDEF_32;
	TSmallintField *tableASUDEF_33;
	TSmallintField *tableASUDEF_34;
	TSmallintField *tableASUDEF_35;
	TSmallintField *tableASUDEF_36;
	TSmallintField *tableASUDEF_37;
	TSmallintField *tableASUDEF_38;
	TSmallintField *tableASUDEF_39;
	TSmallintField *tableASUDEF_40;
	TSmallintField *tableASUDEF_41;
	TSmallintField *tableASUDEF_42;
	TSmallintField *tableASUDEF_43;
	TSmallintField *tableASUDEF_44;
	TSmallintField *tableASUDEF_45;
	TSmallintField *tableASUDEF_46;
	TSmallintField *tableASUDEF_47;
	TSmallintField *tableASUDEF_48;
	TSmallintField *tableASUDEF_49;
	TSmallintField *tableASUDEF_50;
	TSmallintField *tableASUDEF_51;
	TSmallintField *tableASUDEF_52;
	TSmallintField *tableASUDEF_53;
	TSmallintField *tableASUDEF_54;
	TSmallintField *tableASUDEF_55;
	TSmallintField *tableASUDEF_56;
	TSmallintField *tableASUDEF_57;
	TSmallintField *tableASUDEF_58;
	TSmallintField *tableASUDEF_59;
	TSmallintField *tableASUDEF_60;
	TSmallintField *tableASUDEF_61;
	TSmallintField *tableASUDEF_62;
	TSmallintField *tableASUDEF_63;
	TSmallintField *tableASUDEF_64;
	TSmallintField *tableASUDEF_65;
	TSmallintField *tableASUDEF_66;
	TSmallintField *tableASUDEF_67;
	TSmallintField *tableASUDEF_68;
	TSmallintField *tableASUDEF_69;
	TSmallintField *tableASUDEF_70;
	TSmallintField *tableASUDEF_71;
	TSmallintField *tableASUDEF_72;
	TSmallintField *tableASUDEF_73;
	TSmallintField *tableASUDEF_74;
	TSmallintField *tableASUDEF_75;
	TSmallintField *tableASUDEF_76;
	TSmallintField *tableASUDEF_77;
	TSmallintField *tableASUDEF_78;
	TSmallintField *tableASUDEF_79;
	TSmallintField *tableASUDEF_80;
	TSmallintField *tableASUDEF_81;
	TSmallintField *tableASUDEF_82;
	TSmallintField *tableASUDEF_83;
	TSmallintField *tableASUDEF_84;
	TSmallintField *tableASUDEF_85;
	TSmallintField *tableASUDEF_86;
	TSmallintField *tableASUDEF_87;
	TSmallintField *tableASUDEF_88;
	TSmallintField *tableASUDEF_89;
	TSmallintField *tableASUDEF_90;
	TSmallintField *tableASUDEF_91;
	TSmallintField *tableASUDEF_92;
	TSmallintField *tableASUDEF_93;
	TSmallintField *tableASUDEF_94;
	TSmallintField *tableASUDEF_95;
	TSmallintField *tableASUDEF_96;
	TSmallintField *tableASUDEF_97;
	TSmallintField *tableASUDEF_98;
	TSmallintField *tableASUDEF_99;
	TSmallintField *tableASUDEF_100;
	TSmallintField *tableASUDEF_101;
	TSmallintField *tableASUDEF_102;
	TSmallintField *tableASUDEF_103;
	TSmallintField *tableASUDEF_104;
	TSmallintField *tableASUDEF_105;
	TSmallintField *tableASUDEF_106;
	TSmallintField *tableASUDEF_107;
	TSmallintField *tableASUDEF_108;
	TSmallintField *tableASUDEF_109;
	TSmallintField *tableASUDEF_110;
	TSmallintField *tableASUDEF_111;
	TSmallintField *tableASUDEF_112;
	TSmallintField *tableASUDEF_113;
	TSmallintField *tableASUDEF_114;
	TSmallintField *tableASUDEF_115;
	TSmallintField *tableASUDEF_116;
	TSmallintField *tableASUDEF_117;
	TSmallintField *tableASUDEF_118;
	TSmallintField *tableASUDEF_119;
	TSmallintField *tableASUDEF_120;
	TSmallintField *tableASUDEF_121;
	TBCDField *TEtalonsp1;
	TBCDField *TEtalonsp2;
	TBCDField *TEtalonsp3;
	TBCDField *TEtalonsp4;
	TBCDField *TEtalonsp5;
	TBCDField *TEtalonsp6;
	TBCDField *TEtalonsp7;
	TBCDField *TEtalonsp8;
	void __fastcall DataModuleCreate(TObject *Sender);
private:	// User declarations

public:		// User declarations
	bool Connect;
	int16_t resultZone(int numberZone);
	//! @todo нужно перепилить сохранение в базу, то что есть сделано 2 года назад и уже не актуально
	void SaveTube( DynamicArray <double> CrossDef , DynamicArray <double> LineDef ,
					int lengthArray , String Result , String SolidGroup,
					int TypeSize,bool isEtalon );	 //Сохраняет все и обо всех
	void CreateTable();          //Для проекта Игол
	void SaveTube();             //Для проекта Игол
	int GetThick(DynamicArray <double> Thick, long ID);
	void SaveSG(SolidGroups::Tube Tube, UnicodeString TableName);
	vector<SolidGroups::Tube> TDM::ReadEtalon(int TypeSize);	// ,UnicodeString SG
	SolidGroups::Tube ReadCenter(int TypeSize,UnicodeString SG);
	int NumTube;
	ULONG AddTube(String result);

	TIniFile *Ini;                      //Файл настроек
		__fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDM *DM;
//---------------------------------------------------------------------------
#endif
