object ADCSettForm: TADCSettForm
  Left = 177
  Top = 186
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1086#1085#1090#1088#1086#1083#1103
  ClientHeight = 716
  ClientWidth = 965
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 965
    Height = 646
    ActivePage = OtherPage
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object ppPage: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1086#1085#1090#1088#1086#1083#1103
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object gbPP: TGroupBox
        Left = 8
        Top = 8
        Width = 457
        Height = 457
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1087#1077#1088#1077#1095#1085#1099#1093' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 0
        object Label15: TLabel
          Left = 290
          Top = 20
          Width = 122
          Height = 19
          Caption = #1056#1077#1078#1080#1084' '#1080#1079#1084#1077#1088#1077#1085#1080#1103
        end
      end
      object gbCross: TGroupBox
        Left = 3
        Top = 471
        Width = 294
        Height = 130
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1087#1077#1088#1077#1095#1085#1086#1075#1086' '#1089#1086#1083#1077#1085#1086#1080#1076#1072
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object lnumberChannel: TLabel
          Left = 168
          Top = 31
          Width = 93
          Height = 19
          Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lamperage: TLabel
          Left = 8
          Top = 53
          Width = 24
          Height = 19
          Caption = #1058#1086#1082
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lTemp: TLabel
          Left = 8
          Top = 88
          Width = 85
          Height = 19
          Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object spCrossAmperage: TCSpinEdit
          Left = 168
          Top = 56
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 0
        end
        object spCrossVoltage: TCSpinEdit
          Left = 168
          Top = 91
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbPR: TGroupBox
        Left = 490
        Top = 8
        Width = 457
        Height = 457
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1088#1086#1076#1086#1083#1100#1085#1099#1093' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 2
        object Label16: TLabel
          Left = 290
          Top = 20
          Width = 122
          Height = 19
          Caption = #1056#1077#1078#1080#1084' '#1080#1079#1084#1077#1088#1077#1085#1080#1103
        end
      end
      object gbLinear: TGroupBox
        Left = 490
        Top = 471
        Width = 289
        Height = 118
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1088#1086#1076#1086#1083#1100#1085#1086#1075#1086' '#1089#1086#1083#1077#1085#1086#1080#1076#1072
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object lnumberchannelLinear: TLabel
          Left = 168
          Top = 23
          Width = 93
          Height = 19
          Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lLinearAmperage: TLabel
          Left = 8
          Top = 48
          Width = 24
          Height = 19
          Caption = #1058#1086#1082
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lLinearTemp: TLabel
          Left = 8
          Top = 88
          Width = 85
          Height = 19
          Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object spLinearAmperage: TCSpinEdit
          Left = 168
          Top = 48
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 0
        end
        object spLinearVoltage: TCSpinEdit
          Left = 168
          Top = 83
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object prPage: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' LCard 502'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 10
        Top = 59
        Width = 182
        Height = 19
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1073#1086#1088#1072' '#1076#1072#1085#1085#1099#1093' ('#1043#1094')'
      end
      object Label3: TLabel
        Left = 19
        Top = 430
        Width = 201
        Height = 19
        Alignment = taCenter
        Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1084#1077#1078#1076#1091' '#1082#1072#1076#1088#1072#1084#1080' ('#1084#1089')'
        Visible = False
        WordWrap = True
      end
      object Label2: TLabel
        Left = 10
        Top = 17
        Width = 147
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1085#1072' '#1082#1072#1085#1072#1083' ('#1043#1094')'
        WordWrap = True
      end
      object eFriquency: TEdit
        Left = 240
        Top = 56
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 0
      end
      object ppRate: TEdit
        Left = 240
        Top = 11
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 1
        OnKeyPress = ppSizeBufferKeyPress
      end
      object ppKadr: TEdit
        Left = 240
        Top = 425
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 2
        Visible = False
        OnKeyPress = ppSizeBufferKeyPress
      end
      object GroupBox4: TGroupBox
        Left = 3
        Top = 111
        Width = 419
        Height = 146
        Caption = #1057#1080#1085#1093#1088#1086#1085#1080#1079#1072#1094#1080#1103
        TabOrder = 3
        object Label10: TLabel
          Left = 16
          Top = 20
          Width = 235
          Height = 19
          Caption = #1048#1089#1090#1086#1095#1085#1080#1082' '#1095#1072#1089#1090#1086#1090#1099' '#1089#1080#1085#1093#1088#1086#1085#1080#1079#1072#1094#1080#1080
        end
        object Label14: TLabel
          Left = 16
          Top = 81
          Width = 313
          Height = 19
          Caption = #1048#1089#1090#1086#1095#1085#1080#1082' '#1079#1072#1087#1091#1089#1082#1072'  '#1089#1080#1085#1093#1088#1086#1085#1085#1086#1075#1086' '#1074#1074#1086#1076#1072'/'#1074#1099#1074#1086#1076#1072
        end
        object cbbSyncMode: TComboBox
          Left = 16
          Top = 45
          Width = 217
          Height = 27
          Style = csDropDownList
          Enabled = False
          ItemIndex = 0
          TabOrder = 0
          Text = #1042#1085#1091#1090#1088#1077#1085#1085#1080#1081
          Items.Strings = (
            #1042#1085#1091#1090#1088#1077#1085#1085#1080#1081
            #1054#1090' '#1074#1085#1077#1096#1085#1077#1075#1086' '#1084#1072#1089#1090#1077#1088#1072
            #1060#1088#1086#1085#1090' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN1'
            #1060#1088#1086#1085#1090' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN2'
            #1057#1087#1072#1076' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN1'
            #1057#1087#1072#1076' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN2')
        end
        object cbbSyncStartMode: TComboBox
          Left = 16
          Top = 106
          Width = 217
          Height = 27
          Style = csDropDownList
          Enabled = False
          ItemIndex = 0
          TabOrder = 1
          Text = #1042#1085#1091#1090#1088#1077#1085#1085#1080#1081
          Items.Strings = (
            #1042#1085#1091#1090#1088#1077#1085#1085#1080#1081
            #1054#1090' '#1074#1085#1077#1096#1085#1077#1075#1086' '#1084#1072#1089#1090#1077#1088#1072
            #1060#1088#1086#1085#1090' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN1'
            #1060#1088#1086#1085#1090' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN2'
            #1057#1087#1072#1076' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN1'
            #1057#1087#1072#1076' '#1089#1080#1075#1085#1072#1083#1072' DI_SYN2')
        end
      end
    end
    object OtherPage: TTabSheet
      Caption = #1055#1088#1086#1095#1080#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label4: TLabel
        Left = 8
        Top = 56
        Width = 102
        Height = 19
        Caption = #1058#1086#1082#1086#1074#1099#1081' '#1082#1072#1085#1072#1083
      end
      object Label5: TLabel
        Left = 10
        Top = 104
        Width = 151
        Height = 19
        Caption = #1048#1079#1084#1077#1088#1080#1090#1077#1083#1100#1085#1099#1081' '#1082#1072#1085#1072#1083
      end
      object Label6: TLabel
        Left = 10
        Top = 152
        Width = 202
        Height = 19
        Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1074#1088#1072#1097#1077#1085#1080#1103' '#1087#1088#1080' '#1090#1077#1089#1090#1077
      end
      object Label7: TLabel
        Left = 10
        Top = 192
        Width = 214
        Height = 19
        Caption = #1057#1090#1072#1085#1076#1072#1088#1090#1085#1072#1103' '#1075#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
      end
      object SpinCurr: TCSpinEdit
        Left = 240
        Top = 48
        Width = 89
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 0
      end
      object SpinSG: TCSpinEdit
        Left = 240
        Top = 96
        Width = 89
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 1
      end
      object cbCalcSolidGroup: TCheckBox
        Left = 10
        Top = 16
        Width = 247
        Height = 17
        Caption = #1056#1072#1089#1095#1077#1090' '#1043#1055' '#1089' '#1087#1086#1084#1086#1097#1100#1102' '#1082#1086#1088#1088#1077#1083#1103#1094#1080#1080
        TabOrder = 2
      end
      object eFriquencyRot: TEdit
        Left = 240
        Top = 144
        Width = 89
        Height = 27
        NumbersOnly = True
        TabOrder = 3
      end
      object cbSolidGroup: TComboBox
        Left = 240
        Top = 189
        Width = 89
        Height = 27
        Style = csDropDownList
        TabOrder = 4
        Items.Strings = (
          'D'
          'K'
          'E'
          'L'
          'M'
          'K72'
          'N80'
          'L80'
          'P110')
      end
    end
    object FilterPAge: TTabSheet
      Caption = #1060#1080#1083#1100#1090#1088#1072#1094#1080#1103
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object lWidthMedianFilter: TLabel
        Left = 17
        Top = 43
        Width = 201
        Height = 19
        Caption = #1064#1080#1088#1080#1085#1072' '#1084#1077#1076#1080#1072#1085#1085#1086#1075#1086' '#1092#1080#1083#1100#1090#1088#1072
      end
      object cbMedianFilter: TCheckBox
        Left = 24
        Top = 16
        Width = 193
        Height = 17
        Caption = #1052#1077#1076#1080#1072#1085#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103
        TabOrder = 0
      end
      object ComboBoxWidthMF: TComboBox
        Left = 240
        Top = 44
        Width = 73
        Height = 27
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 1
        Text = '3'
        Items.Strings = (
          '3'
          '5'
          '7'
          '9')
      end
      object RadioGroup1: TRadioGroup
        Left = 24
        Top = 122
        Width = 460
        Height = 431
        Caption = #1062#1080#1092#1088#1086#1074#1099#1077' '#1092#1080#1083#1100#1090#1088#1099' '#1087#1088#1086#1076#1086#1083#1100#1085#1099#1081
        TabOrder = 2
      end
      object cbIsFiltersPr: TCheckBox
        Left = 24
        Top = 80
        Width = 361
        Height = 17
        Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1094#1080#1092#1088#1086#1074#1099#1077' '#1092#1080#1083#1100#1090#1088#1099' '#1087#1088#1086#1076#1086#1083#1100#1085#1086#1075#1086
        TabOrder = 3
      end
      object gbButterworthPr: TGroupBox
        Left = 37
        Top = 297
        Width = 425
        Height = 243
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
        TabOrder = 4
        object Label11: TLabel
          Left = 7
          Top = 16
          Width = 57
          Height = 19
          Caption = #1055#1086#1088#1103#1076#1086#1082
        end
        object Label12: TLabel
          Left = 8
          Top = 50
          Width = 92
          Height = 19
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1088#1077#1079#1072
        end
        object Label13: TLabel
          Left = 8
          Top = 80
          Width = 101
          Height = 19
          Caption = #1062#1077#1085#1090#1088' '#1092#1080#1083#1100#1090#1088#1072
        end
        object Label17: TLabel
          Left = 8
          Top = 110
          Width = 116
          Height = 19
          Caption = #1064#1080#1088#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072
        end
        object Label18: TLabel
          Left = 8
          Top = 140
          Width = 141
          Height = 38
          Caption = #1055#1091#1083#1100#1089#1072#1094#1080#1080' '#1074' '#1087#1086#1083#1086#1089#1077' '#1087#1088#1086#1087#1091#1089#1082#1072#1085#1080#1103
          WordWrap = True
        end
        object Label19: TLabel
          Left = 8
          Top = 186
          Width = 131
          Height = 38
          Caption = #1055#1091#1083#1100#1089#1072#1094#1080#1080' '#1074' '#1087#1086#1083#1086#1089#1077' '#1087#1086#1076#1072#1074#1083#1077#1085#1080#1103
          WordWrap = True
        end
        object csOrderPr: TCSpinEdit
          Left = 220
          Top = 17
          Width = 77
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 20
          ParentColor = True
          ParentFont = False
          TabOrder = 0
        end
        object eCutoffFreqPr: TEdit
          Left = 220
          Top = 54
          Width = 76
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 1
        end
        object eCenterFreqPr: TEdit
          Left = 220
          Top = 83
          Width = 76
          Height = 23
          BevelInner = bvNone
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 2
        end
        object eWidthFreqPr: TEdit
          Left = 220
          Top = 113
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 3
        end
        object eRippleDbPr: TEdit
          Left = 218
          Top = 150
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 4
        end
        object eRolloffPr: TEdit
          Left = 219
          Top = 196
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 5
        end
      end
      object rgFilterSubTypePr: TRadioGroup
        Left = 288
        Top = 146
        Width = 177
        Height = 145
        Caption = #1055#1086#1076#1090#1080#1087
        ItemIndex = 0
        Items.Strings = (
          #1053#1080#1079#1082#1080#1093' '#1095#1072#1089#1090#1086#1090
          #1042#1099#1089#1086#1082#1080#1093' '#1095#1072#1089#1090#1086#1090
          #1055#1086#1083#1086#1089#1086#1074#1086#1081
          #1047#1072#1075#1088#1072#1078#1076#1072#1102#1097#1080#1081)
        TabOrder = 5
        OnClick = rgFilterSubTypePrClick
      end
      object rgFilterTypePr: TRadioGroup
        Left = 37
        Top = 146
        Width = 230
        Height = 145
        Caption = #1058#1080#1087' '#1092#1080#1083#1100#1090#1088#1072
        ItemIndex = 0
        Items.Strings = (
          #1041#1072#1090#1090#1077#1088#1074#1086#1088#1090#1072
          #1063#1077#1073#1099#1096#1077#1074#1072
          #1069#1083#1083#1080#1087#1090#1080#1095#1077#1089#1082#1080#1081)
        TabOrder = 6
        OnClick = rgFilterTypePrClick
      end
      object RadioGroup2: TRadioGroup
        Left = 490
        Top = 122
        Width = 460
        Height = 431
        Caption = #1062#1080#1092#1088#1086#1074#1099#1077' '#1092#1080#1083#1100#1090#1088#1099' '#1087#1086#1087#1077#1088#1077#1095#1085#1099#1081
        TabOrder = 7
      end
      object rgFilterTypePp: TRadioGroup
        Left = 500
        Top = 146
        Width = 230
        Height = 145
        Caption = #1058#1080#1087' '#1092#1080#1083#1100#1090#1088#1072
        ItemIndex = 0
        Items.Strings = (
          #1041#1072#1090#1090#1077#1088#1074#1086#1088#1090#1072
          #1063#1077#1073#1099#1096#1077#1074#1072
          #1069#1083#1083#1080#1087#1090#1080#1095#1077#1089#1082#1080#1081)
        TabOrder = 8
        OnClick = rgFilterTypePpClick
      end
      object rgFilterSubTypePp: TRadioGroup
        Left = 753
        Top = 146
        Width = 177
        Height = 145
        Caption = #1055#1086#1076#1090#1080#1087
        ItemIndex = 0
        Items.Strings = (
          #1053#1080#1079#1082#1080#1093' '#1095#1072#1089#1090#1086#1090
          #1042#1099#1089#1086#1082#1080#1093' '#1095#1072#1089#1090#1086#1090
          #1055#1086#1083#1086#1089#1086#1074#1086#1081
          #1047#1072#1075#1088#1072#1078#1076#1072#1102#1097#1080#1081)
        TabOrder = 9
        OnClick = rgFilterSubTypePpClick
      end
      object gbButterworthPp: TGroupBox
        Left = 500
        Top = 297
        Width = 425
        Height = 243
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
        TabOrder = 10
        object Label20: TLabel
          Left = 7
          Top = 16
          Width = 57
          Height = 19
          Caption = #1055#1086#1088#1103#1076#1086#1082
        end
        object Label21: TLabel
          Left = 8
          Top = 50
          Width = 92
          Height = 19
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1088#1077#1079#1072
        end
        object Label22: TLabel
          Left = 8
          Top = 80
          Width = 101
          Height = 19
          Caption = #1062#1077#1085#1090#1088' '#1092#1080#1083#1100#1090#1088#1072
        end
        object Label23: TLabel
          Left = 8
          Top = 110
          Width = 116
          Height = 19
          Caption = #1064#1080#1088#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072
        end
        object Label24: TLabel
          Left = 8
          Top = 140
          Width = 141
          Height = 38
          Caption = #1055#1091#1083#1100#1089#1072#1094#1080#1080' '#1074' '#1087#1086#1083#1086#1089#1077' '#1087#1088#1086#1087#1091#1089#1082#1072#1085#1080#1103
          WordWrap = True
        end
        object Label25: TLabel
          Left = 9
          Top = 180
          Width = 131
          Height = 38
          Caption = #1055#1091#1083#1100#1089#1072#1094#1080#1080' '#1074' '#1087#1086#1083#1086#1089#1077' '#1087#1086#1076#1072#1074#1083#1077#1085#1080#1103
          WordWrap = True
        end
        object csOrderPp: TCSpinEdit
          Left = 220
          Top = 17
          Width = 77
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 20
          ParentColor = True
          ParentFont = False
          TabOrder = 0
        end
        object eCutoffFreqPp: TEdit
          Left = 220
          Top = 54
          Width = 76
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 1
        end
        object eCenterFreqPp: TEdit
          Left = 220
          Top = 81
          Width = 76
          Height = 23
          BevelInner = bvNone
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 2
        end
        object eWidthFreqPp: TEdit
          Left = 220
          Top = 110
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 3
        end
        object eRippleDbPp: TEdit
          Left = 220
          Top = 145
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 4
        end
        object eRolloffPp: TEdit
          Left = 220
          Top = 187
          Width = 77
          Height = 23
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 5
        end
      end
      object cbIsFiltersPp: TCheckBox
        Left = 490
        Top = 77
        Width = 361
        Height = 17
        Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1094#1080#1092#1088#1086#1074#1099#1077' '#1092#1080#1083#1100#1090#1088#1099' '#1087#1086#1087#1077#1088#1077#1095#1085#1086#1075#1086
        TabOrder = 11
      end
    end
  end
  object pBottom: TPanel
    Left = 0
    Top = 646
    Width = 965
    Height = 70
    Align = alBottom
    TabOrder = 1
    object bSave: TButton
      Left = 326
      Top = 16
      Width = 113
      Height = 41
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = bSaveClick
    end
    object bExit: TButton
      Left = 460
      Top = 16
      Width = 113
      Height = 41
      Caption = #1042#1099#1081#1090#1080
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = bExitClick
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 928
    Top = 664
  end
  object CopyValueCrossChannel: TPopupMenu
    Left = 752
    Top = 664
    object CopyAllSensor: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1085#1072' '#1076#1088#1091#1075#1080#1077' '#1076#1072#1090#1095#1080#1082#1080
      OnClick = CopyAllSensorClick
    end
  end
  object CopyValueLinearChannel: TPopupMenu
    Left = 624
    Top = 664
    object CopyAllSensorLinear: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1085#1072' '#1076#1088#1091#1075#1080#1077' '#1076#1072#1090#1095#1080#1082#1080
      OnClick = CopyAllSensorLinearClick
    end
  end
end
