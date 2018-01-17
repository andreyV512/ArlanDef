object FormColor: TFormColor
  Left = 353
  Top = 233
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1094#1074#1077#1090#1072
  ClientHeight = 318
  ClientWidth = 518
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object ColorBrackAnalog: TGroupBox
    Left = 8
    Top = 8
    Width = 221
    Height = 241
    Caption = #1062#1074#1077#1090#1072' '#1072#1085#1072#1083#1086#1075#1086#1074#1099#1093' '#1089#1080#1075#1085#1072#1083#1086#1074
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object LabelBrack: TLabel
      Left = 6
      Top = 32
      Width = 31
      Height = 19
      Caption = #1041#1088#1072#1082
    end
    object LabelValid: TLabel
      Left = 6
      Top = 73
      Width = 41
      Height = 19
      Caption = #1043#1086#1076#1085#1086
    end
    object LabelSecondClass: TLabel
      Left = 6
      Top = 114
      Width = 52
      Height = 19
      Caption = #1050#1083#1072#1089#1089' 2'
    end
    object LabelSelectZone: TLabel
      Left = 6
      Top = 151
      Width = 117
      Height = 19
      Caption = #1042#1099#1076#1077#1083#1077#1085#1085#1072#1103' '#1079#1086#1085#1072
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object lDeadZone: TLabel
      Left = 6
      Top = 192
      Width = 92
      Height = 19
      Caption = #1052#1077#1088#1090#1074#1072#1103' '#1079#1086#1085#1072
    end
    object PanelValid: TPanel
      Left = 137
      Top = 73
      Width = 73
      Height = 19
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelValidClick
    end
    object PanelBrack: TPanel
      Left = 137
      Top = 32
      Width = 73
      Height = 19
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelValidClick
    end
    object PanelSecondClass: TPanel
      Left = 137
      Top = 112
      Width = 73
      Height = 21
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelValidClick
    end
    object pSelectZone: TPanel
      Left = 137
      Top = 150
      Width = 73
      Height = 21
      ParentBackground = False
      TabOrder = 3
      OnClick = PanelValidClick
    end
    object pDeadZone: TPanel
      Left = 136
      Top = 192
      Width = 73
      Height = 21
      ParentBackground = False
      TabOrder = 4
      OnClick = PanelValidClick
    end
  end
  object GroupBoxDigital: TGroupBox
    Left = 237
    Top = 8
    Width = 273
    Height = 177
    Caption = #1062#1074#1077#1090#1072' '#1094#1080#1092#1088#1086#1074#1099#1093' '#1089#1080#1075#1085#1072#1083#1086#1074
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object LabelInActive: TLabel
      Left = 6
      Top = 32
      Width = 105
      Height = 19
      Caption = #1040#1082#1090#1080#1074#1085#1099#1081' '#1074#1093#1086#1076
    end
    object LabelNotInActiv: TLabel
      Left = 6
      Top = 73
      Width = 119
      Height = 19
      Caption = #1053#1077#1072#1082#1090#1080#1074#1085#1099#1081' '#1074#1093#1086#1076
    end
    object LabelActiveOut: TLabel
      Left = 6
      Top = 114
      Width = 116
      Height = 19
      Caption = #1040#1082#1090#1080#1074#1085#1099#1081' '#1074#1099#1093#1086#1076
    end
    object LabelNotActiveOut: TLabel
      Left = 8
      Top = 152
      Width = 130
      Height = 19
      Caption = #1053#1077#1072#1082#1090#1080#1074#1085#1099#1081' '#1074#1099#1093#1086#1076
    end
    object NotInActive: TPanel
      Left = 176
      Top = 73
      Width = 73
      Height = 19
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelValidClick
    end
    object ActiveIn: TPanel
      Left = 176
      Top = 32
      Width = 73
      Height = 19
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelValidClick
    end
    object ActiveOut: TPanel
      Left = 176
      Top = 112
      Width = 73
      Height = 21
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelValidClick
    end
    object NotActiveOut: TPanel
      Left = 176
      Top = 153
      Width = 73
      Height = 16
      ParentBackground = False
      TabOrder = 3
      OnClick = PanelValidClick
    end
  end
  object BSave: TButton
    Left = 224
    Top = 265
    Width = 89
    Height = 34
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 2
    OnClick = BSaveClick
  end
  object BClose: TButton
    Left = 340
    Top = 265
    Width = 89
    Height = 34
    Caption = #1047#1072#1082#1088#1099#1090#1100
    TabOrder = 3
    OnClick = BCloseClick
  end
  object BDefault: TButton
    Left = 104
    Top = 265
    Width = 89
    Height = 34
    Caption = #1055#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
    TabOrder = 4
    OnClick = BDefaultClick
  end
  object GroupBox1: TGroupBox
    Left = 235
    Top = 191
    Width = 275
    Height = 58
    Caption = #1062#1074#1077#1090#1072' '#1075#1088#1091#1087#1087' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    object Label1: TLabel
      Left = 10
      Top = 24
      Width = 11
      Height = 19
      Caption = #1044
    end
    object Label2: TLabel
      Left = 97
      Top = 24
      Width = 12
      Height = 19
      Caption = 'K'
    end
    object Label3: TLabel
      Left = 184
      Top = 24
      Width = 9
      Height = 19
      Caption = 'E'
    end
    object sgD: TPanel
      Left = 42
      Top = 24
      Width = 49
      Height = 21
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelValidClick
    end
    object sgK: TPanel
      Left = 124
      Top = 24
      Width = 49
      Height = 21
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelValidClick
    end
    object sgE: TPanel
      Left = 212
      Top = 24
      Width = 49
      Height = 21
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelValidClick
    end
  end
  object ColorDialog: TColorDialog
    Color = clGreen
    Left = 56
    Top = 282
  end
end
