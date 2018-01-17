object FRHistory: TFRHistory
  Left = 0
  Top = 0
  Width = 635
  Height = 36
  Color = clBtnFace
  ParentBackground = False
  ParentColor = False
  TabOrder = 0
  OnResize = FrameResize
  object Label1: TLabel
    Left = 136
    Top = 16
    Width = 31
    Height = 13
    Caption = 'Label1'
  end
  inline FRTubeHistory1: TFRTubeHistory
    Left = 0
    Top = 0
    Width = 359
    Height = 31
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 0
    inherited Bevel1: TBevel
      ExplicitLeft = -6
    end
  end
  inline FRTubeHistory2: TFRTubeHistory
    Left = 209
    Top = 0
    Width = 359
    Height = 31
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 1
    ExplicitLeft = 209
  end
  inline FRTubeHistory3: TFRTubeHistory
    Left = 418
    Top = 0
    Width = 359
    Height = 31
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 2
    ExplicitLeft = 418
  end
end
