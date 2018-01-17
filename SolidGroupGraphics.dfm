object SGGraphForm: TSGGraphForm
  Left = 0
  Top = 0
  Caption = #1043#1088#1072#1092#1080#1082#1080' '#1043#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
  ClientHeight = 331
  ClientWidth = 684
  Color = clBtnFace
  Constraints.MinHeight = 365
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StringGridParameters: TStringGrid
    Left = 609
    Top = 0
    Width = 75
    Height = 331
    Align = alRight
    Anchors = []
    Color = clSilver
    ColCount = 1
    DefaultColWidth = 75
    DefaultRowHeight = 40
    DoubleBuffered = True
    DrawingStyle = gdsClassic
    FixedCols = 0
    RowCount = 8
    FixedRows = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    ParentDoubleBuffered = False
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 0
    OnKeyDown = FormKeyDown
    OnSelectCell = StringGridParametersSelectCell
  end
  object ParameterChart: TChart
    Left = 0
    Top = 0
    Width = 609
    Height = 331
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Maximum = 3500.000000000000000000
    LeftAxis.Minimum = -4000.000000000000000000
    View3D = False
    Align = alClient
    TabOrder = 1
    OnKeyDown = FormKeyDown
    ColorPaletteIndex = 13
    object Series1: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      LinePen.Color = 10708548
      LinePen.Width = 2
      OutLine.Width = 2
      Pointer.Brush.Gradient.EndColor = 10708548
      Pointer.Depth = 1
      Pointer.Gradient.EndColor = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = 33023
      LinePen.Color = 33023
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {
        0219000000000000000058AB4000000020000000000058ABC0000000205EBA49
        0C02DD7B40FFFFFF1F5EBA490C02B77B40FFFFFF1F5EBA490C02EF7940FFFFFF
        1F00000000004D7840FFFFFF1F0000000000D27840FFFFFF1F00000000003B7A
        40FFFFFF1F00000000809A7B40FFFFFF1F0000000000CA7B40FFFFFF1F000000
        0080A37A40FFFFFF1F0000000080027B40FFFFFF1F00000000804D7940FFFFFF
        1F0000000080AB7740FFFFFF1F00000000807C7840FFFFFF1F00000000809877
        40FFFFFF1F0000000080DB7840FFFFFF1F0000000000277840FFFFFF1F000000
        0080BF7940FFFFFF1F0000000000747A400000002000000000001E7940000000
        200000000000867840000000200000000080017940000000200000000080907A
        40000000200000000000617A4000000020}
    end
    object Series3: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = 33023
      LinePen.Color = 33023
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {
        00190000000000000000003E4000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000677340000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000}
    end
  end
end
