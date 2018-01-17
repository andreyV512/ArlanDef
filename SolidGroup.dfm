object SolidGroupForm: TSolidGroupForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1086#1087#1088#1077#1076#1077#1083#1077#1085#1080#1103' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
  ClientHeight = 481
  ClientWidth = 501
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 56
    Top = 48
    Width = 94
    Height = 19
    Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 56
    Top = 104
    Width = 140
    Height = 19
    Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object cbSolidGroup: TComboBox
    Left = 224
    Top = 106
    Width = 145
    Height = 21
    Style = csDropDownList
    TabOrder = 1
    OnKeyPress = FormKeyPress
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
  object cbTypeSize: TComboBox
    Left = 224
    Top = 50
    Width = 145
    Height = 21
    Style = csDropDownList
    TabOrder = 0
    OnKeyPress = FormKeyPress
  end
  object bAddInserted: TButton
    Left = 56
    Top = 360
    Width = 145
    Height = 41
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 2
    OnClick = bAddInsertedClick
    OnKeyPress = FormKeyPress
  end
  object bAddCurrentTube: TButton
    Left = 272
    Top = 360
    Width = 145
    Height = 41
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1087#1088#1086#1096#1077#1076#1096#1091#1102
    TabOrder = 3
    Visible = False
    OnClick = bAddCurrentTubeClick
    OnKeyPress = FormKeyPress
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 447
    Width = 495
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Width = 400
      end
      item
        Width = 400
      end>
    ParentColor = True
    UseSystemFont = False
  end
end
