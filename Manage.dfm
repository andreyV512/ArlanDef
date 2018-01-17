object ManageForm: TManageForm
  Left = 200
  Top = 108
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072#1084#1080
  ClientHeight = 363
  ClientWidth = 676
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbLinear: TGroupBox
    Left = 24
    Top = 20
    Width = 611
    Height = 120
    Caption = #1052#1086#1076#1091#1083#1100' '#1087#1088#1086#1076#1086#1083#1100#1085#1086#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1080
    TabOrder = 0
    object Label1: TLabel
      Left = 287
      Top = 35
      Width = 69
      Height = 13
      Caption = #1063#1072#1089#1090#1086#1090#1072' ('#1043#1094'.)'
    end
    object bRotation: TBitBtn
      Left = 152
      Top = 35
      Width = 128
      Height = 49
      Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1074#1088#1072#1097#1077#1085#1080#1077
      DoubleBuffered = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
      OnClick = bRotationClick
      OnKeyPress = FormKeyPress
    end
    object pLinearSolenoid: TPanel
      Left = 403
      Top = 24
      Width = 195
      Height = 80
      BevelWidth = 2
      ParentColor = True
      TabOrder = 1
      object lLinearAmperage: TLabel
        Left = 10
        Top = 7
        Width = 74
        Height = 13
        Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072', ('#1040')'
        Color = clCream
        ParentColor = False
      end
      object lLinearVoltage: TLabel
        Left = 10
        Top = 32
        Width = 84
        Height = 13
        Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077', ('#1042')'
      end
      object lLinearResistance: TLabel
        Left = 10
        Top = 57
        Width = 108
        Height = 13
        Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077', ('#1054#1084')'
      end
      object eLinearAmperage: TEdit
        Left = 141
        Top = 4
        Width = 40
        Height = 21
        Color = clWhite
        ReadOnly = True
        TabOrder = 0
      end
      object eLinearVoltage: TEdit
        Left = 141
        Top = 28
        Width = 40
        Height = 21
        ReadOnly = True
        TabOrder = 1
      end
      object eLinearTemperature: TEdit
        Left = 141
        Top = 53
        Width = 40
        Height = 21
        ReadOnly = True
        TabOrder = 2
      end
    end
    object eFriquencyRot: TEdit
      Left = 286
      Top = 64
      Width = 76
      Height = 21
      NumbersOnly = True
      TabOrder = 2
    end
    object bLinearSolenoid: TBitBtn
      Left = 10
      Top = 35
      Width = 126
      Height = 49
      Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1084#1072#1075#1085#1080#1090#1085#1086#1077' '#1087#1086#1083#1077
      Default = True
      DoubleBuffered = True
      ModalResult = 1
      NumGlyphs = 2
      ParentDoubleBuffered = False
      TabOrder = 3
      WordWrap = True
      OnClick = bLinearSolenoidClick
      OnKeyPress = FormKeyPress
    end
  end
  object gbCross: TGroupBox
    Left = 24
    Top = 174
    Width = 401
    Height = 120
    Caption = #1052#1086#1076#1091#1083#1100' '#1087#1086#1087#1077#1088#1077#1095#1085#1086#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1080
    TabOrder = 1
    object pCrossSolenoidNew: TPanel
      Left = 181
      Top = 20
      Width = 195
      Height = 80
      BevelWidth = 2
      ParentColor = True
      TabOrder = 0
      object lCrossAmperage: TLabel
        Left = 10
        Top = 5
        Width = 74
        Height = 13
        Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072', ('#1040')'
        Color = clCream
        ParentColor = False
      end
      object lCrossVoltage: TLabel
        Left = 10
        Top = 32
        Width = 84
        Height = 13
        Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077', ('#1042')'
      end
      object lCrossResistance: TLabel
        Left = 10
        Top = 57
        Width = 108
        Height = 13
        Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077', ('#1054#1084')'
      end
      object eCrossAmperage: TEdit
        Left = 123
        Top = 4
        Width = 60
        Height = 21
        Color = clWhite
        ReadOnly = True
        TabOrder = 0
      end
      object eCrossVoltage: TEdit
        Left = 123
        Top = 28
        Width = 60
        Height = 21
        ReadOnly = True
        TabOrder = 1
      end
      object eCrossTemperature: TEdit
        Left = 123
        Top = 53
        Width = 60
        Height = 21
        ReadOnly = True
        TabOrder = 2
      end
    end
    object bCrossSolenoid: TBitBtn
      Left = 10
      Top = 35
      Width = 126
      Height = 49
      Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1084#1072#1075#1085#1080#1090#1085#1086#1077' '#1087#1086#1083#1077
      Default = True
      DoubleBuffered = True
      ModalResult = 1
      NumGlyphs = 2
      ParentDoubleBuffered = False
      TabOrder = 1
      WordWrap = True
      OnClick = bCrossSolenoidClick
      OnKeyPress = FormKeyPress
    end
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 329
    Width = 670
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Width = 600
      end>
    ParentColor = True
    UseSystemFont = False
  end
  object TemperatureTimer: TTimer
    Enabled = False
    Interval = 2000
    Left = 720
    Top = 160
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 440
    Top = 160
  end
end
