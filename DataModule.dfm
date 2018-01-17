object DM: TDM
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 236
  Width = 491
  object ADOCon: TADOConnection
    CommandTimeout = 3
    ConnectionTimeout = 5
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 224
    Top = 8
  end
  object DataDefect: TDataSource
    DataSet = DefectTable
    Left = 24
    Top = 160
  end
  object DataTube: TDataSource
    DataSet = TubeTable
    Left = 120
    Top = 160
  end
  object TubeTable: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'InfoTube'
    Left = 120
    Top = 112
    object TubeTableID_Tube: TLargeintField
      FieldName = 'ID_Tube'
      ReadOnly = True
    end
    object TubeTableResult: TWideStringField
      FieldName = 'Result'
      FixedChar = True
      Size = 2
    end
    object TubeTableSolidGroup: TWideStringField
      FieldName = 'SolidGroup'
      Size = 2
    end
    object TubeTableDate: TDateTimeField
      FieldName = 'Date'
    end
    object TubeTableMinThickness: TFloatField
      FieldName = 'MinThickness'
    end
    object TubeTableCrossResult: TStringField
      FieldName = 'CrossResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableLineResult: TStringField
      FieldName = 'LineResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableLength: TFloatField
      FieldName = 'Length'
    end
    object TubeTableThicknessResult: TStringField
      FieldName = 'ThicknessResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableTypeSize: TIntegerField
      FieldName = 'TypeSize'
    end
    object TubeTableisEtalon: TBooleanField
      FieldName = 'isEtalon'
    end
    object TubeTableDefectoscoperID: TIntegerField
      FieldName = 'DefectoscoperID'
    end
    object TubeTableTubeGroup: TIntegerField
      FieldName = 'TubeGroup'
    end
    object TubeTableCustomerID: TIntegerField
      FieldName = 'CustomerID'
    end
    object TubeTableNumberTube: TLargeintField
      FieldName = 'NumberTube'
    end
  end
  object DefectTable: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'Defect'
    Left = 24
    Top = 112
    object DefectTableID_Tube: TLargeintField
      FieldName = 'ID_Tube'
    end
    object DefectTableZone: TWordField
      FieldName = 'Zone'
    end
    object DefectTableCrossDefect: TFloatField
      FieldName = 'CrossDefect'
    end
    object DefectTableLineDefect: TFloatField
      FieldName = 'LineDefect'
    end
  end
  object QueryThick: TADOQuery
    Connection = ADOCon
    Parameters = <
      item
        Name = 'Par'
        DataType = ftVariant
        Value = 0
      end>
    Left = 200
    Top = 112
  end
  object Find: TADOQuery
    Connection = ADOCon
    Parameters = <
      item
        Name = 'par'
        DataType = ftWideMemo
        Size = 1
        Value = ''
      end
      item
        Name = 'par1'
        DataType = ftInteger
        Value = 0
      end
      item
        Name = 'ParSG0'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult0'
        Size = -1
        Value = Null
      end
      item
        Name = 'Begin'
        DataType = ftDateTime
        Size = -1
        Value = Null
      end
      item
        Name = 'End'
        DataType = ftDateTime
        Size = -1
        Value = Null
      end
      item
        Name = 'ParSG1'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParSG2'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult1'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult2'
        Size = -1
        Value = Null
      end>
    Left = 384
    Top = 112
  end
  object DataFind: TDataSource
    DataSet = Find
    Left = 392
    Top = 160
  end
  object TEtalons: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'SGEtalon'
    Left = 200
    Top = 160
    object TEtalonsName: TMemoField
      FieldName = 'Name'
      BlobType = ftWideString
      Size = 5
    end
    object TEtalonsTypeSize: TSmallintField
      FieldName = 'TypeSize'
    end
    object TEtalonsDate: TDateTimeField
      FieldName = 'Date'
    end
    object TEtalonsp1: TBCDField
      FieldName = 'p1'
      Precision = 5
      Size = 3
    end
    object TEtalonsp2: TBCDField
      FieldName = 'p2'
      Precision = 5
      Size = 3
    end
    object TEtalonsp3: TBCDField
      FieldName = 'p3'
      Precision = 5
      Size = 3
    end
    object TEtalonsp4: TBCDField
      FieldName = 'p4'
      Precision = 5
      Size = 3
    end
    object TEtalonsp5: TBCDField
      FieldName = 'p5'
      Precision = 5
      Size = 3
    end
    object TEtalonsp6: TBCDField
      FieldName = 'p6'
      Precision = 5
      Size = 3
    end
    object TEtalonsp7: TBCDField
      FieldName = 'p7'
      Precision = 5
      Size = 3
    end
    object TEtalonsp8: TBCDField
      FieldName = 'p8'
      Precision = 5
      Size = 3
    end
  end
  object DataEtalons: TDataSource
    DataSet = QEtalons
    Left = 296
    Top = 160
  end
  object QEtalons: TADOQuery
    Connection = ADOCon
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.SGEtalon')
    Left = 296
    Top = 112
  end
  object tTemp: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'SGEtalon'
    Left = 320
    Top = 8
    object tTempID: TAutoIncField
      FieldName = 'ID'
      ReadOnly = True
    end
    object tTempName: TWideStringField
      FieldName = 'Name'
      Size = 5
    end
    object tTempp1: TSmallintField
      FieldName = 'p1'
    end
    object tTempp2: TSmallintField
      FieldName = 'p2'
    end
    object tTempp3: TSmallintField
      FieldName = 'p3'
    end
    object tTempp4: TSmallintField
      FieldName = 'p4'
    end
    object tTempp5: TSmallintField
      FieldName = 'p5'
    end
    object tTempp6: TSmallintField
      FieldName = 'p6'
    end
    object tTempp7: TSmallintField
      FieldName = 'p7'
    end
    object tTempp8: TSmallintField
      FieldName = 'p8'
    end
    object tTempTypeSize: TSmallintField
      FieldName = 'TypeSize'
    end
    object tTempDate: TDateTimeField
      FieldName = 'Date'
    end
  end
  object Table: TADOTable
    Left = 88
    Top = 24
  end
  object tableASU: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'Uran_NKT'
    Left = 424
    Top = 16
    object tableASUAI: TAutoIncField
      FieldName = 'AI'
      ReadOnly = True
    end
    object tableASUN_TR_URAN: TIntegerField
      FieldName = 'N_TR_URAN'
    end
    object tableASUDAT_T: TDateTimeField
      FieldName = 'DAT_T'
    end
    object tableASUDAT_TIM: TDateTimeField
      FieldName = 'DAT_TIM'
    end
    object tableASUPBR_1: TSmallintField
      FieldName = 'PBR_1'
    end
    object tableASUP2C_1: TSmallintField
      FieldName = 'P2C_1'
    end
    object tableASUPBR_2: TSmallintField
      FieldName = 'PBR_2'
    end
    object tableASUP2C_2: TSmallintField
      FieldName = 'P2C_2'
    end
    object tableASUPBR_3: TSmallintField
      FieldName = 'PBR_3'
    end
    object tableASUP2C_3: TSmallintField
      FieldName = 'P2C_3'
    end
    object tableASUL_URAN: TSmallintField
      FieldName = 'L_URAN'
    end
    object tableASUREZ1: TSmallintField
      FieldName = 'REZ1'
    end
    object tableASUREZ2: TSmallintField
      FieldName = 'REZ2'
    end
    object tableASUPRIZN_URAN: TSmallintField
      FieldName = 'PRIZN_URAN'
    end
    object tableASUPRIZN_URAN_MUFTA: TSmallintField
      FieldName = 'PRIZN_URAN_MUFTA'
    end
    object tableASUPR_GROUP_URAN: TStringField
      FieldName = 'PR_GROUP_URAN'
      FixedChar = True
      Size = 3
    end
    object tableASUPR_GROUP_MUFTA: TStringField
      FieldName = 'PR_GROUP_MUFTA'
      FixedChar = True
      Size = 3
    end
    object tableASUDEF_0: TSmallintField
      FieldName = 'DEF_0'
    end
    object tableASUDEF_1: TSmallintField
      FieldName = 'DEF_1'
    end
    object tableASUDEF_2: TSmallintField
      FieldName = 'DEF_2'
    end
    object tableASUDEF_3: TSmallintField
      FieldName = 'DEF_3'
    end
    object tableASUDEF_4: TSmallintField
      FieldName = 'DEF_4'
    end
    object tableASUDEF_5: TSmallintField
      FieldName = 'DEF_5'
    end
    object tableASUDEF_6: TSmallintField
      FieldName = 'DEF_6'
    end
    object tableASUDEF_7: TSmallintField
      FieldName = 'DEF_7'
    end
    object tableASUDEF_8: TSmallintField
      FieldName = 'DEF_8'
    end
    object tableASUDEF_9: TSmallintField
      FieldName = 'DEF_9'
    end
    object tableASUDEF_10: TSmallintField
      FieldName = 'DEF_10'
    end
    object tableASUDEF_11: TSmallintField
      FieldName = 'DEF_11'
    end
    object tableASUDEF_12: TSmallintField
      FieldName = 'DEF_12'
    end
    object tableASUDEF_13: TSmallintField
      FieldName = 'DEF_13'
    end
    object tableASUDEF_14: TSmallintField
      FieldName = 'DEF_14'
    end
    object tableASUDEF_15: TSmallintField
      FieldName = 'DEF_15'
    end
    object tableASUDEF_16: TSmallintField
      FieldName = 'DEF_16'
    end
    object tableASUDEF_17: TSmallintField
      FieldName = 'DEF_17'
    end
    object tableASUDEF_18: TSmallintField
      FieldName = 'DEF_18'
    end
    object tableASUDEF_19: TSmallintField
      FieldName = 'DEF_19'
    end
    object tableASUDEF_20: TSmallintField
      FieldName = 'DEF_20'
    end
    object tableASUDEF_21: TSmallintField
      FieldName = 'DEF_21'
    end
    object tableASUDEF_22: TSmallintField
      FieldName = 'DEF_22'
    end
    object tableASUDEF_23: TSmallintField
      FieldName = 'DEF_23'
    end
    object tableASUDEF_24: TSmallintField
      FieldName = 'DEF_24'
    end
    object tableASUDEF_25: TSmallintField
      FieldName = 'DEF_25'
    end
    object tableASUDEF_26: TSmallintField
      FieldName = 'DEF_26'
    end
    object tableASUDEF_27: TSmallintField
      FieldName = 'DEF_27'
    end
    object tableASUDEF_28: TSmallintField
      FieldName = 'DEF_28'
    end
    object tableASUDEF_29: TSmallintField
      FieldName = 'DEF_29'
    end
    object tableASUDEF_30: TSmallintField
      FieldName = 'DEF_30'
    end
    object tableASUDEF_31: TSmallintField
      FieldName = 'DEF_31'
    end
    object tableASUDEF_32: TSmallintField
      FieldName = 'DEF_32'
    end
    object tableASUDEF_33: TSmallintField
      FieldName = 'DEF_33'
    end
    object tableASUDEF_34: TSmallintField
      FieldName = 'DEF_34'
    end
    object tableASUDEF_35: TSmallintField
      FieldName = 'DEF_35'
    end
    object tableASUDEF_36: TSmallintField
      FieldName = 'DEF_36'
    end
    object tableASUDEF_37: TSmallintField
      FieldName = 'DEF_37'
    end
    object tableASUDEF_38: TSmallintField
      FieldName = 'DEF_38'
    end
    object tableASUDEF_39: TSmallintField
      FieldName = 'DEF_39'
    end
    object tableASUDEF_40: TSmallintField
      FieldName = 'DEF_40'
    end
    object tableASUDEF_41: TSmallintField
      FieldName = 'DEF_41'
    end
    object tableASUDEF_42: TSmallintField
      FieldName = 'DEF_42'
    end
    object tableASUDEF_43: TSmallintField
      FieldName = 'DEF_43'
    end
    object tableASUDEF_44: TSmallintField
      FieldName = 'DEF_44'
    end
    object tableASUDEF_45: TSmallintField
      FieldName = 'DEF_45'
    end
    object tableASUDEF_46: TSmallintField
      FieldName = 'DEF_46'
    end
    object tableASUDEF_47: TSmallintField
      FieldName = 'DEF_47'
    end
    object tableASUDEF_48: TSmallintField
      FieldName = 'DEF_48'
    end
    object tableASUDEF_49: TSmallintField
      FieldName = 'DEF_49'
    end
    object tableASUDEF_50: TSmallintField
      FieldName = 'DEF_50'
    end
    object tableASUDEF_51: TSmallintField
      FieldName = 'DEF_51'
    end
    object tableASUDEF_52: TSmallintField
      FieldName = 'DEF_52'
    end
    object tableASUDEF_53: TSmallintField
      FieldName = 'DEF_53'
    end
    object tableASUDEF_54: TSmallintField
      FieldName = 'DEF_54'
    end
    object tableASUDEF_55: TSmallintField
      FieldName = 'DEF_55'
    end
    object tableASUDEF_56: TSmallintField
      FieldName = 'DEF_56'
    end
    object tableASUDEF_57: TSmallintField
      FieldName = 'DEF_57'
    end
    object tableASUDEF_58: TSmallintField
      FieldName = 'DEF_58'
    end
    object tableASUDEF_59: TSmallintField
      FieldName = 'DEF_59'
    end
    object tableASUDEF_60: TSmallintField
      FieldName = 'DEF_60'
    end
    object tableASUDEF_61: TSmallintField
      FieldName = 'DEF_61'
    end
    object tableASUDEF_62: TSmallintField
      FieldName = 'DEF_62'
    end
    object tableASUDEF_63: TSmallintField
      FieldName = 'DEF_63'
    end
    object tableASUDEF_64: TSmallintField
      FieldName = 'DEF_64'
    end
    object tableASUDEF_65: TSmallintField
      FieldName = 'DEF_65'
    end
    object tableASUDEF_66: TSmallintField
      FieldName = 'DEF_66'
    end
    object tableASUDEF_67: TSmallintField
      FieldName = 'DEF_67'
    end
    object tableASUDEF_68: TSmallintField
      FieldName = 'DEF_68'
    end
    object tableASUDEF_69: TSmallintField
      FieldName = 'DEF_69'
    end
    object tableASUDEF_70: TSmallintField
      FieldName = 'DEF_70'
    end
    object tableASUDEF_71: TSmallintField
      FieldName = 'DEF_71'
    end
    object tableASUDEF_72: TSmallintField
      FieldName = 'DEF_72'
    end
    object tableASUDEF_73: TSmallintField
      FieldName = 'DEF_73'
    end
    object tableASUDEF_74: TSmallintField
      FieldName = 'DEF_74'
    end
    object tableASUDEF_75: TSmallintField
      FieldName = 'DEF_75'
    end
    object tableASUDEF_76: TSmallintField
      FieldName = 'DEF_76'
    end
    object tableASUDEF_77: TSmallintField
      FieldName = 'DEF_77'
    end
    object tableASUDEF_78: TSmallintField
      FieldName = 'DEF_78'
    end
    object tableASUDEF_79: TSmallintField
      FieldName = 'DEF_79'
    end
    object tableASUDEF_80: TSmallintField
      FieldName = 'DEF_80'
    end
    object tableASUDEF_81: TSmallintField
      FieldName = 'DEF_81'
    end
    object tableASUDEF_82: TSmallintField
      FieldName = 'DEF_82'
    end
    object tableASUDEF_83: TSmallintField
      FieldName = 'DEF_83'
    end
    object tableASUDEF_84: TSmallintField
      FieldName = 'DEF_84'
    end
    object tableASUDEF_85: TSmallintField
      FieldName = 'DEF_85'
    end
    object tableASUDEF_86: TSmallintField
      FieldName = 'DEF_86'
    end
    object tableASUDEF_87: TSmallintField
      FieldName = 'DEF_87'
    end
    object tableASUDEF_88: TSmallintField
      FieldName = 'DEF_88'
    end
    object tableASUDEF_89: TSmallintField
      FieldName = 'DEF_89'
    end
    object tableASUDEF_90: TSmallintField
      FieldName = 'DEF_90'
    end
    object tableASUDEF_91: TSmallintField
      FieldName = 'DEF_91'
    end
    object tableASUDEF_92: TSmallintField
      FieldName = 'DEF_92'
    end
    object tableASUDEF_93: TSmallintField
      FieldName = 'DEF_93'
    end
    object tableASUDEF_94: TSmallintField
      FieldName = 'DEF_94'
    end
    object tableASUDEF_95: TSmallintField
      FieldName = 'DEF_95'
    end
    object tableASUDEF_96: TSmallintField
      FieldName = 'DEF_96'
    end
    object tableASUDEF_97: TSmallintField
      FieldName = 'DEF_97'
    end
    object tableASUDEF_98: TSmallintField
      FieldName = 'DEF_98'
    end
    object tableASUDEF_99: TSmallintField
      FieldName = 'DEF_99'
    end
    object tableASUDEF_100: TSmallintField
      FieldName = 'DEF_100'
    end
    object tableASUDEF_101: TSmallintField
      FieldName = 'DEF_101'
    end
    object tableASUDEF_102: TSmallintField
      FieldName = 'DEF_102'
    end
    object tableASUDEF_103: TSmallintField
      FieldName = 'DEF_103'
    end
    object tableASUDEF_104: TSmallintField
      FieldName = 'DEF_104'
    end
    object tableASUDEF_105: TSmallintField
      FieldName = 'DEF_105'
    end
    object tableASUDEF_106: TSmallintField
      FieldName = 'DEF_106'
    end
    object tableASUDEF_107: TSmallintField
      FieldName = 'DEF_107'
    end
    object tableASUDEF_108: TSmallintField
      FieldName = 'DEF_108'
    end
    object tableASUDEF_109: TSmallintField
      FieldName = 'DEF_109'
    end
    object tableASUDEF_110: TSmallintField
      FieldName = 'DEF_110'
    end
    object tableASUDEF_111: TSmallintField
      FieldName = 'DEF_111'
    end
    object tableASUDEF_112: TSmallintField
      FieldName = 'DEF_112'
    end
    object tableASUDEF_113: TSmallintField
      FieldName = 'DEF_113'
    end
    object tableASUDEF_114: TSmallintField
      FieldName = 'DEF_114'
    end
    object tableASUDEF_115: TSmallintField
      FieldName = 'DEF_115'
    end
    object tableASUDEF_116: TSmallintField
      FieldName = 'DEF_116'
    end
    object tableASUDEF_117: TSmallintField
      FieldName = 'DEF_117'
    end
    object tableASUDEF_118: TSmallintField
      FieldName = 'DEF_118'
    end
    object tableASUDEF_119: TSmallintField
      FieldName = 'DEF_119'
    end
    object tableASUDEF_120: TSmallintField
      FieldName = 'DEF_120'
    end
    object tableASUDEF_121: TSmallintField
      FieldName = 'DEF_121'
    end
  end
  object tASU: TDataSource
    DataSet = tableASU
    Left = 424
    Top = 64
  end
end
