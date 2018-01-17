object SpectroSettingForm: TSpectroSettingForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 254
  ClientWidth = 433
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object btnOK: TButton
    Left = 189
    Top = 210
    Width = 97
    Height = 25
    Caption = #1054#1050
    ModalResult = 1
    TabOrder = 0
  end
  object btnCancel: TButton
    Left = 312
    Top = 210
    Width = 97
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
  end
  object gbSerialPort: TGroupBox
    Left = 8
    Top = 20
    Width = 401
    Height = 169
    Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1089#1083#1077#1076#1086#1074#1072#1090#1077#1083#1100#1085#1086#1075#1086' '#1087#1086#1088#1090#1072
    TabOrder = 2
    object Label2: TLabel
      Left = 16
      Top = 33
      Width = 126
      Height = 13
      Caption = #1055#1086#1089#1083#1077#1076#1086#1074#1072#1090#1077#1083#1100#1085#1099#1081' '#1087#1086#1088#1090
    end
    object Label3: TLabel
      Left = 16
      Top = 67
      Width = 121
      Height = 13
      Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1086#1073#1084#1077#1085#1072', '#1041#1080#1090'/c'
    end
    object Label4: TLabel
      Left = 16
      Top = 136
      Width = 145
      Height = 13
      Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1089#1090#1086#1087#1086#1074#1099#1093' '#1073#1080#1090#1086#1074
    end
    object Label5: TLabel
      Left = 16
      Top = 101
      Width = 43
      Height = 13
      Caption = #1055#1072#1088#1080#1090#1077#1090
    end
    object cbxParity: TComboBox
      Left = 288
      Top = 97
      Width = 97
      Height = 21
      Style = csDropDownList
      TabOrder = 0
      Items.Strings = (
        #1054#1090#1089#1091#1090#1089#1090#1074#1091#1077#1090
        #1063#1105#1090#1085#1099#1081
        #1053#1077#1095#1105#1090#1085#1099#1081)
    end
    object cbxStopBits: TComboBox
      Left = 288
      Top = 132
      Width = 97
      Height = 21
      Style = csDropDownList
      TabOrder = 1
      Items.Strings = (
        '1'
        '2')
    end
    object cbxBaudrate: TComboBox
      Left = 288
      Top = 64
      Width = 97
      Height = 21
      Style = csDropDownList
      TabOrder = 2
      Items.Strings = (
        '100'
        '300'
        '600'
        '1200'
        '2400'
        '4800'
        '9600'
        '14400'
        '19200'
        '38400'
        '57600'
        '115200')
    end
    object cbxSerialPort: TComboBox
      Left = 288
      Top = 29
      Width = 97
      Height = 21
      Style = csDropDownList
      TabOrder = 3
    end
  end
end
