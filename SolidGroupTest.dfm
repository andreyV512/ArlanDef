object SGForm: TSGForm
  Left = 221
  Top = 186
  Caption = #1058#1077#1089#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
  ClientHeight = 538
  ClientWidth = 952
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenuTestSG
  Position = poDesigned
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pTop: TPanel
    Left = 0
    Top = 0
    Width = 952
    Height = 33
    Align = alTop
    TabOrder = 0
    object lTestTime: TLabel
      Left = 337
      Top = 4
      Width = 97
      Height = 21
      Caption = #1042#1088#1077#1084#1103' '#1090#1077#1089#1090#1072
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
    end
    object bTest: TButton
      Left = 217
      Top = 2
      Width = 91
      Height = 25
      Caption = #1058#1077#1089#1090
      TabOrder = 0
      OnClick = bTestClick
      OnKeyDown = FormKeyDown
    end
    object bOpenTube: TButton
      Left = 9
      Top = 3
      Width = 97
      Height = 25
      Caption = '&'#1054#1090#1082#1088#1099#1090#1100' '#1090#1088#1091#1073#1091
      TabOrder = 1
      OnClick = bOpenTubeClick
      OnKeyDown = FormKeyDown
    end
    object bSaveTube: TButton
      Left = 114
      Top = 3
      Width = 97
      Height = 25
      Caption = '&'#1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1090#1088#1091#1073#1091
      TabOrder = 2
      OnClick = bSaveTubeClick
      OnKeyDown = FormKeyDown
    end
    object eTestTime: TEdit
      Left = 440
      Top = 7
      Width = 81
      Height = 21
      NumbersOnly = True
      TabOrder = 3
      OnKeyDown = FormKeyDown
    end
    object bShowCurrentTube: TButton
      Left = 731
      Top = 3
      Width = 91
      Height = 25
      Caption = #1058#1077#1082#1091#1097#1072#1103' '#1090#1088#1091#1073#1072
      TabOrder = 4
      OnClick = bShowCurrentTubeClick
      OnKeyDown = FormKeyDown
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 516
    Width = 952
    Height = 22
    Panels = <
      item
        Width = 300
      end
      item
        Width = 700
      end
      item
        Width = 50
      end>
  end
  object SignalChart: TChart
    Left = 0
    Top = 33
    Width = 872
    Height = 452
    Legend.Visible = False
    ScrollMouseButton = mbMiddle
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    OnScroll = SignalChartScroll
    OnUndoZoom = SignalChartScroll
    OnZoom = SignalChartScroll
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMaximum = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.ExactDateTime = False
    BottomAxis.Maximum = 1000.000000000000000000
    DepthAxis.Automatic = False
    DepthAxis.AutomaticMaximum = False
    DepthAxis.AutomaticMinimum = False
    DepthAxis.Maximum = 0.500000000000000000
    DepthAxis.Minimum = -0.500000000000000200
    DepthTopAxis.Automatic = False
    DepthTopAxis.AutomaticMaximum = False
    DepthTopAxis.AutomaticMinimum = False
    DepthTopAxis.Maximum = 0.500000000000000000
    DepthTopAxis.Minimum = -0.500000000000000200
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Maximum = 1500.000000000000000000
    LeftAxis.Minimum = -1500.000000000000000000
    RightAxis.Automatic = False
    RightAxis.AutomaticMaximum = False
    RightAxis.AutomaticMinimum = False
    View3D = False
    Align = alClient
    TabOrder = 2
    OnClick = SignalChartClick
    OnKeyDown = FormKeyDown
    PrintMargins = (
      15
      22
      15
      22)
    ColorPaletteIndex = 16
    object Series3: TPointSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clBlue
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = clBlue
      Pointer.Gradient.EndColor = clBlue
      Pointer.HorizSize = 3
      Pointer.InflateMargins = True
      Pointer.Style = psCircle
      Pointer.VertSize = 3
      Pointer.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      object TeeFunction1: TCustomTeeFunction
        Period = 1.000000000000000000
        NumPoints = 100
      end
    end
    object Series2: TPointSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      DataSource = Series3
      SeriesColor = clRed
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = clRed
      Pointer.Gradient.EndColor = clRed
      Pointer.HorizSize = 3
      Pointer.InflateMargins = True
      Pointer.Style = psCircle
      Pointer.VertSize = 3
      Pointer.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      object TeeFunction2: TAverageTeeFunction
      end
    end
    object Series: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = 10708548
      ClickableLine = False
      LinePen.Color = 10708548
      Pointer.Brush.Gradient.EndColor = 10708548
      Pointer.Gradient.EndColor = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series1: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clRed
      ClickableLine = False
      LinePen.Color = clRed
      Pointer.Brush.Gradient.EndColor = clRed
      Pointer.Gradient.EndColor = clRed
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 485
    Width = 952
    Height = 31
    Align = alBottom
    TabOrder = 3
    object lMinScrollbar: TLabel
      Left = 64
      Top = 6
      Width = 16
      Height = 13
      Caption = 'Min'
      Transparent = True
      Visible = False
    end
    object lMaxScrollbar: TLabel
      Left = 835
      Top = 6
      Width = 20
      Height = 13
      Caption = 'Max'
      Visible = False
    end
    object scbChartDragger: TScrollBar
      Left = 352
      Top = 6
      Width = 350
      Height = 17
      Max = 10000
      Min = 1
      PageSize = 0
      Position = 1
      TabOrder = 0
      OnChange = scbChartDraggerChange
    end
  end
  object pThresholds: TPanel
    Left = 872
    Top = 33
    Width = 80
    Height = 452
    Align = alRight
    TabOrder = 4
    Visible = False
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '.sg'
    Filter = #1057#1080#1075#1085#1072#1083' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' (*.sg)|*.sg'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 664
    Top = 40
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.sg'
    Filter = #1057#1080#1075#1085#1072#1083' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' (*.sg)|*.sg'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 728
    Top = 40
  end
  object PopupMenuTestSG: TPopupMenu
    Left = 472
    Top = 360
    object N1: TMenuItem
      Caption = #1053#1072#1074#1080#1075#1072#1094#1080#1103
      OnClick = N1Click
    end
    object N2: TMenuItem
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
      OnClick = N2Click
    end
    object AddParameters: TMenuItem
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099' '#1074' '#1041#1044
      OnClick = AddParametersClick
    end
  end
end
