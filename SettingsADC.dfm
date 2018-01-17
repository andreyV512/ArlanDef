object FormSettADC: TFormSettADC
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
  ClientHeight = 343
  ClientWidth = 908
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Times New Roman'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 19
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 908
    Height = 343
    ActivePage = TabSheet3
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1055#1086#1087#1077#1088#1077#1095#1085#1099#1081' '#1082#1086#1085#1090#1088#1086#1083#1100
      DoubleBuffered = False
      ParentDoubleBuffered = False
      object LSizeBuf: TLabel
        Left = 552
        Top = 45
        Width = 161
        Height = 19
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1091#1092#1077#1088#1072' '#1040#1062#1055'('#1084#1073')'
      end
      object LRate: TLabel
        Left = 552
        Top = 98
        Width = 174
        Height = 38
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1086#1087#1088#1086#1089#1072' '#1082#1072#1085#1072#1083#1086#1074' '#1074' '#1082#1072#1076#1088#1077' ('#1082#1043#1094')'
        WordWrap = True
      end
      object Label13: TLabel
        Left = 542
        Top = 174
        Width = 184
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103' ('#1043#1094')'
        WordWrap = True
      end
      object GroupBox1: TGroupBox
        Left = 3
        Top = 16
        Width = 486
        Height = 265
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1072#1085#1072#1083#1086#1074
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 29
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'1'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 68
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'2'
        end
        object Label3: TLabel
          Left = 16
          Top = 105
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'3'
        end
        object Label4: TLabel
          Left = 16
          Top = 145
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'4'
        end
        object Label5: TLabel
          Left = 16
          Top = 185
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'5'
        end
        object Label6: TLabel
          Left = 16
          Top = 225
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'6'
        end
        object Label7: TLabel
          Left = 247
          Top = 31
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'7'
        end
        object Label8: TLabel
          Left = 247
          Top = 70
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'8'
        end
        object Label9: TLabel
          Left = 247
          Top = 107
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'9'
        end
        object Label10: TLabel
          Left = 247
          Top = 147
          Width = 84
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'10'
        end
        object Label11: TLabel
          Left = 247
          Top = 187
          Width = 83
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'11'
        end
        object Label12: TLabel
          Left = 247
          Top = 227
          Width = 84
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'12'
        end
        object ComboBox1: TComboBox
          Tag = 1
          Left = 162
          Top = 29
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 0
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox2: TComboBox
          Tag = 2
          Left = 162
          Top = 64
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 1
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox3: TComboBox
          Tag = 3
          Left = 162
          Top = 102
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 2
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox4: TComboBox
          Tag = 4
          Left = 162
          Top = 141
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 3
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox5: TComboBox
          Tag = 5
          Left = 162
          Top = 179
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 4
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox6: TComboBox
          Tag = 6
          Left = 162
          Top = 224
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 5
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox7: TComboBox
          Tag = 7
          Left = 394
          Top = 29
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 6
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox8: TComboBox
          Tag = 8
          Left = 394
          Top = 64
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 7
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox9: TComboBox
          Tag = 9
          Left = 394
          Top = 102
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 8
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox10: TComboBox
          Tag = 10
          Left = 394
          Top = 141
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 9
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox11: TComboBox
          Tag = 11
          Left = 394
          Top = 179
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 10
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox12: TComboBox
          Tag = 12
          Left = 394
          Top = 224
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 11
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object CSpinEdit1: TCSpinEdit
          Left = 106
          Top = 29
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 12
        end
        object CSpinEdit2: TCSpinEdit
          Tag = 1
          Left = 106
          Top = 64
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 13
        end
        object CSpinEdit3: TCSpinEdit
          Tag = 2
          Left = 106
          Top = 102
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 14
        end
        object CSpinEdit4: TCSpinEdit
          Tag = 3
          Left = 106
          Top = 141
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 15
        end
        object CSpinEdit5: TCSpinEdit
          Tag = 4
          Left = 106
          Top = 179
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 16
        end
        object CSpinEdit6: TCSpinEdit
          Tag = 5
          Left = 106
          Top = 224
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 17
        end
        object CSpinEdit7: TCSpinEdit
          Tag = 6
          Left = 338
          Top = 29
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 18
        end
        object CSpinEdit8: TCSpinEdit
          Tag = 7
          Left = 338
          Top = 64
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 19
        end
        object CSpinEdit9: TCSpinEdit
          Tag = 8
          Left = 338
          Top = 102
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 20
        end
        object CSpinEdit10: TCSpinEdit
          Tag = 9
          Left = 338
          Top = 141
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 21
        end
        object CSpinEdit11: TCSpinEdit
          Tag = 10
          Left = 336
          Top = 179
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 22
        end
        object CSpinEdit12: TCSpinEdit
          Tag = 11
          Left = 338
          Top = 224
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 32
          ParentFont = False
          TabOrder = 23
        end
      end
      object EditSizeBuf: TEdit
        Left = 754
        Top = 42
        Width = 136
        Height = 27
        TabOrder = 1
        OnKeyPress = EditSizeBufKeyPress
      end
      object EditRate: TEdit
        Left = 754
        Top = 115
        Width = 136
        Height = 27
        TabOrder = 2
        OnKeyPress = EditSizeBufKeyPress
      end
      object EditKadr: TEdit
        Left = 754
        Top = 171
        Width = 136
        Height = 27
        TabOrder = 3
        OnKeyPress = EditSizeBufKeyPress
      end
      object bSave: TButton
        Left = 574
        Top = 243
        Width = 152
        Height = 32
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        TabOrder = 4
        OnClick = bSaveClick
      end
      object Button2: TButton
        Left = 754
        Top = 243
        Width = 111
        Height = 32
        Caption = #1042#1099#1081#1090#1080
        TabOrder = 5
        OnClick = Button2Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1099#1081' '#1082#1086#1085#1090#1088#1086#1083#1100
      ImageIndex = 1
      object Label26: TLabel
        Left = 320
        Top = 29
        Width = 161
        Height = 19
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1091#1092#1077#1088#1072' '#1040#1062#1055'('#1084#1073')'
      end
      object Label27: TLabel
        Left = 320
        Top = 82
        Width = 174
        Height = 38
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1086#1087#1088#1086#1089#1072' '#1082#1072#1085#1072#1083#1086#1074' '#1074' '#1082#1072#1076#1088#1077' ('#1082#1043#1094')'
        WordWrap = True
      end
      object Label28: TLabel
        Left = 301
        Top = 157
        Width = 184
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103' ('#1043#1094')'
        WordWrap = True
      end
      object GroupBox2: TGroupBox
        Left = 3
        Top = 16
        Width = 262
        Height = 183
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1072#1085#1072#1083#1086#1074
        TabOrder = 0
        object Label14: TLabel
          Left = 16
          Top = 29
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'1'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 16
          Top = 68
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'2'
        end
        object Label16: TLabel
          Left = 16
          Top = 105
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'3'
        end
        object Label17: TLabel
          Left = 16
          Top = 145
          Width = 76
          Height = 19
          Caption = #1044#1072#1090#1095#1080#1082' '#8470'4'
        end
        object ComboBox13: TComboBox
          Tag = 1
          Left = 162
          Top = 29
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 0
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox14: TComboBox
          Tag = 2
          Left = 162
          Top = 64
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 1
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox15: TComboBox
          Tag = 3
          Left = 162
          Top = 102
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 2
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object ComboBox16: TComboBox
          Tag = 4
          Left = 162
          Top = 141
          Width = 79
          Height = 23
          Style = csDropDownList
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          ItemIndex = 0
          ParentFont = False
          TabOrder = 3
          Text = #177'10'
          Items.Strings = (
            #177'10'
            #177'5'
            #177'2.5'
            #177'1.25'
            #177'0.625')
        end
        object CSpinEdit13: TCSpinEdit
          Left = 106
          Top = 29
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 4
        end
        object CSpinEdit14: TCSpinEdit
          Tag = 1
          Left = 106
          Top = 64
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 5
        end
        object CSpinEdit15: TCSpinEdit
          Tag = 2
          Left = 106
          Top = 102
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 6
        end
        object CSpinEdit16: TCSpinEdit
          Tag = 3
          Left = 106
          Top = 141
          Width = 35
          Height = 24
          Enabled = False
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 7
        end
      end
      object Edit1: TEdit
        Left = 522
        Top = 26
        Width = 136
        Height = 27
        TabOrder = 1
        OnKeyPress = EditSizeBufKeyPress
      end
      object Edit2: TEdit
        Left = 522
        Top = 93
        Width = 136
        Height = 27
        TabOrder = 2
        OnKeyPress = EditSizeBufKeyPress
      end
      object Edit3: TEdit
        Left = 522
        Top = 154
        Width = 136
        Height = 27
        TabOrder = 3
        OnKeyPress = EditSizeBufKeyPress
      end
      object Button1: TButton
        Left = 560
        Top = 248
        Width = 146
        Height = 33
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        TabOrder = 4
        OnClick = Button1Click
      end
      object Button3: TButton
        Left = 744
        Top = 248
        Width = 130
        Height = 33
        Caption = #1042#1099#1081#1090#1080
        TabOrder = 5
        OnClick = Button3Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1055#1088#1086#1095#1077#1077
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      object LabelTemp1: TLabel
        Left = 7
        Top = 59
        Width = 223
        Height = 21
        Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1089#1086#1083#1077#1085#1086#1080#1076#1072' '#8470'1'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object LabelTemp2: TLabel
        Left = 7
        Top = 115
        Width = 223
        Height = 21
        Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1089#1086#1083#1077#1085#1086#1080#1076#1072' '#8470'2'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object NumberCh: TLabel
        Left = 237
        Top = 31
        Width = 93
        Height = 19
        Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
      end
      object LabelMagnetic: TLabel
        Left = 7
        Top = 171
        Width = 220
        Height = 21
        Caption = #1050#1086#1085#1090#1088#1086#1083#1100' '#1084#1072#1075#1085#1080#1090#1085#1086#1075#1086' '#1087#1086#1083#1103
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object LabelKatphot1: TLabel
        Left = 384
        Top = 56
        Width = 162
        Height = 21
        Hint = #1042#1093#1086#1076#1085#1086#1081' '#1076#1072#1090#1095#1080#1082' '#1084#1086#1076#1091#1083#1103' '#1088#1072#1079#1084#1072#1075#1085#1080#1095#1077#1074#1072#1085#1080#1103
        Caption = #1044#1072#1090#1095#1080#1082' '#1085#1072' '#1074#1093#1086#1076#1077' '#1052#1056
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
      end
      object LabelKataphot2: TLabel
        Left = 384
        Top = 115
        Width = 184
        Height = 21
        Hint = #1044#1072#1090#1095#1080#1082' '#1085#1072' '#1074#1099#1093#1086#1076#1077' '#1084#1086#1076#1091#1083#1103' '#1088#1072#1079#1084#1072#1075#1085#1080#1095#1080#1074#1072#1085#1080#1103
        Caption = #1050#1072#1090#1072#1092#1086#1090' '#1052#1056' '#1085#1072' '#1074#1099#1093#1086#1076#1077
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object LabelSG: TLabel
        Left = 384
        Top = 171
        Width = 87
        Height = 21
        Hint = #1044#1072#1090#1095#1080#1082' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
        Caption = #1044#1072#1090#1095#1080#1082' '#1043#1055
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object LabelMag: TLabel
        Left = 384
        Top = 219
        Width = 130
        Height = 21
        Hint = #1058#1086#1082#1086#1074#1099#1081' '#1076#1072#1090#1095#1080#1082' '#1084#1086#1076#1091#1083#1103' '#1088#1072#1079#1084#1072#1075#1085#1080#1095#1080#1074#1072#1085#1080#1103
        Caption = #1044#1072#1090#1095#1080#1082' '#1090#1086#1082#1072' '#1052#1056
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object Label18: TLabel
        Left = 660
        Top = 32
        Width = 93
        Height = 19
        Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
      end
      object SpinTemp1: TCSpinEdit
        Left = 236
        Top = 57
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 0
      end
      object SpinTemp2: TCSpinEdit
        Left = 236
        Top = 110
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 1
      end
      object SpinMagnetic: TCSpinEdit
        Left = 236
        Top = 166
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 2
      end
      object Button4: TButton
        Left = 260
        Top = 264
        Width = 147
        Height = 33
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1080' '#1074#1099#1081#1090#1080
        TabOrder = 3
        OnClick = Button4Click
      end
      object BExit: TButton
        Left = 456
        Top = 264
        Width = 121
        Height = 33
        Caption = #1042#1099#1081#1090#1080
        TabOrder = 4
        OnClick = Button3Click
      end
      object SpinKataphot1: TCSpinEdit
        Left = 660
        Top = 57
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 5
      end
      object SpinKataphot2: TCSpinEdit
        Left = 660
        Top = 110
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 6
      end
      object SpinSG: TCSpinEdit
        Left = 660
        Top = 166
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 7
      end
      object SpinCurr: TCSpinEdit
        Left = 660
        Top = 217
        Width = 97
        Height = 29
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        MaxValue = 31
        TabOrder = 8
      end
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 608
    Top = 8
  end
end
