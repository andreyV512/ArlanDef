object SettingsForm: TSettingsForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1090#1080#1087#1086#1088#1072#1079#1084#1077#1088#1072
  ClientHeight = 536
  ClientWidth = 497
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyPress = ValueListKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ValueListEditor: TValueListEditor
    Left = 8
    Top = 58
    Width = 482
    Height = 438
    DrawingStyle = gdsGradient
    TabOrder = 0
    TitleCaptions.Strings = (
      #1055#1072#1088#1072#1084#1077#1090#1088
      #1047#1085#1072#1095#1077#1085#1080#1077)
    OnKeyPress = ValueListKeyPress
    OnSelectCell = ValueListEditorSelectCell
    ColWidths = (
      349
      127)
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 502
    Width = 491
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Width = 300
      end
      item
        Width = 400
      end>
    ParentColor = True
    UseSystemFont = False
  end
  object cbTypeSize: TComboBox
    Left = 32
    Top = 14
    Width = 145
    Height = 31
    Style = csDropDownList
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnKeyPress = ValueListKeyPress
    OnSelect = cbTypeSizeSelect
  end
  object bCreateTypeSize: TButton
    Left = 200
    Top = 8
    Width = 129
    Height = 44
    Caption = #1057#1086#1079#1076#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = bCreateTypeSizeClick
    OnKeyPress = ValueListKeyPress
  end
  object bDeleteTypeSize: TButton
    Left = 352
    Top = 8
    Width = 129
    Height = 44
    Caption = #1059#1076#1072#1083#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = bDeleteTypeSizeClick
    OnKeyPress = ValueListKeyPress
  end
end
