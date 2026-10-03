object Form1: TForm1
  Left = 218
  Top = 152
  AutoScroll = False
  Caption = 'CreakBox V0.99 Beta  Copyright 2002 Bass Assessor'
  ClientHeight = 519
  ClientWidth = 492
  Color = clBtnFace
  Constraints.MaxHeight = 565
  Constraints.MaxWidth = 500
  Constraints.MinHeight = 565
  Constraints.MinWidth = 500
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PopupMenu = x
  Position = poDesktopCenter
  Visible = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 14
  object Label11: TLabel
    Left = 360
    Top = 96
    Width = 21
    Height = 14
    Caption = 'BPM'
  end
  object Label1: TLabel
    Left = 300
    Top = 96
    Width = 20
    Height = 14
    Caption = 'Oct.'
  end
  object Label4: TLabel
    Left = 176
    Top = 124
    Width = 150
    Height = 14
    Caption = 'Random Wildness in editor (F1)'
  end
  object Label18: TLabel
    Left = 152
    Top = 100
    Width = 8
    Height = 14
    Caption = 'A'
  end
  object Label22: TLabel
    Left = 132
    Top = 100
    Width = 7
    Height = 14
    Caption = 'S'
  end
  object Label2: TLabel
    Left = 200
    Top = 8
    Width = 24
    Height = 14
    Caption = 'From'
  end
  object Label3: TLabel
    Left = 212
    Top = 32
    Width = 12
    Height = 14
    Caption = 'To'
  end
  object Label5: TLabel
    Left = 80
    Top = 504
    Width = 320
    Height = 14
    Caption = 'Copyright 2002 Bass Assessor - www.mp3.com/Bass_Assessor'
  end
  object rgType: TRadioGroup
    Left = 176
    Top = 84
    Width = 117
    Height = 33
    BiDiMode = bdLeftToRight
    Caption = 'Waveform'
    Columns = 2
    ItemIndex = 1
    Items.Strings = ('Saw', 'Square')
    ParentBiDiMode = False
    TabOrder = 0
  end
  object eOctave: TEdit
    Left = 320
    Top = 92
    Width = 17
    Height = 22
    Hint = 'Octave'
    BiDiMode = bdLeftToRight
    Color = clMenu
    ParentBiDiMode = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 50
    Text = '2'
  end
  object btnOctMinus: TButton
    Left = 340
    Top = 102
    Width = 13
    Height = 11
    Hint = 'Octave Down'
    Caption = '-'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 55
    OnClick = btnOctMinusClick
  end
  object Button4: TButton
    Left = 132
    Top = 4
    Width = 61
    Height = 21
    Caption = 'Make Wave'
    TabOrder = 56
    OnClick = Button4Click
  end
  object eBPM: TEdit
    Left = 384
    Top = 92
    Width = 33
    Height = 22
    TabOrder = 57
    Text = '150'
    OnChange = eBPMChange
  end
  object knobpage: TPageControl
    Left = 176
    Top = 144
    Width = 313
    Height = 361
    ActivePage = tsCutoff
    HotTrack = True
    TabOrder = 58
    object TabSheet1: TTabSheet
      Caption = 'volume'
      ImageIndex = 6
      object gridVolume: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsTune: TTabSheet
      Caption = 'tune'
      object gridTune: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsCutoff: TTabSheet
      Caption = 'cutoff'
      ImageIndex = 1
      object gridCutoff: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsRezo: TTabSheet
      Caption = 'rezo'
      ImageIndex = 2
      object gridRezo: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsEnvMod: TTabSheet
      Caption = 'envmod'
      ImageIndex = 3
      object gridEnvMod: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsDecay: TTabSheet
      Caption = 'decay'
      ImageIndex = 4
      object gridDecay: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
    object tsAccent: TTabSheet
      Caption = 'accent'
      ImageIndex = 5
      object gridAccent: TStringGrid
        Left = 4
        Top = 4
        Width = 169
        Height = 325
        ColCount = 3
        DefaultRowHeight = 14
        FixedCols = 0
        RowCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        TabOrder = 0
        OnKeyDown = gridTuneKeyDown
        ColWidths = (36, 44, 66)
      end
    end
  end
  object btnOctUp: TButton
    Left = 340
    Top = 92
    Width = 13
    Height = 11
    Hint = 'Octave Up'
    Caption = '+'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 54
    OnClick = btnOctUpClick
  end
  object lbNR: TListBox
    Left = 4
    Top = 35
    Width = 45
    Height = 468
    BorderStyle = bsNone
    Color = clMenu
    Ctl3D = False
    ExtendedSelect = False
    ItemHeight = 14
    Items.Strings = ('1')
    ParentCtl3D = False
    TabOrder = 59
    OnClick = lbOrderClick
  end
  object lbOrder: TListBox
    Left = 28
    Top = 34
    Width = 49
    Height = 471
    BiDiMode = bdLeftToRight
    ExtendedSelect = False
    ItemHeight = 14
    Items.Strings = ('1')
    ParentBiDiMode = False
    TabOrder = 60
    OnClick = lbOrderClick
  end
  object Panel1: TPanel
    Left = 59
    Top = 32
    Width = 18
    Height = 473
    BevelOuter = bvNone
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 61
  end
  object eNote1: TEdit
    Tag = 1
    Left = 76
    Top = 116
    Width = 45
    Height = 22
    Color = 14869218
    TabOrder = 1
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote2: TEdit
    Tag = 2
    Left = 76
    Top = 141
    Width = 45
    Height = 22
    TabOrder = 2
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote3: TEdit
    Tag = 3
    Left = 76
    Top = 165
    Width = 45
    Height = 22
    TabOrder = 3
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote4: TEdit
    Tag = 4
    Left = 76
    Top = 190
    Width = 45
    Height = 22
    TabOrder = 4
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote5: TEdit
    Tag = 5
    Left = 76
    Top = 214
    Width = 45
    Height = 22
    Color = 14869218
    TabOrder = 5
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote6: TEdit
    Tag = 6
    Left = 76
    Top = 239
    Width = 45
    Height = 22
    TabOrder = 6
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote7: TEdit
    Tag = 7
    Left = 76
    Top = 263
    Width = 45
    Height = 22
    TabOrder = 7
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote8: TEdit
    Tag = 8
    Left = 76
    Top = 288
    Width = 45
    Height = 22
    TabOrder = 8
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote9: TEdit
    Tag = 9
    Left = 76
    Top = 312
    Width = 45
    Height = 22
    Color = 14869218
    TabOrder = 9
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote10: TEdit
    Tag = 10
    Left = 76
    Top = 337
    Width = 45
    Height = 22
    TabOrder = 10
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote11: TEdit
    Tag = 11
    Left = 76
    Top = 361
    Width = 45
    Height = 22
    TabOrder = 11
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote12: TEdit
    Tag = 12
    Left = 76
    Top = 386
    Width = 45
    Height = 22
    TabOrder = 12
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote13: TEdit
    Tag = 13
    Left = 76
    Top = 410
    Width = 45
    Height = 22
    Color = 14869218
    TabOrder = 13
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote14: TEdit
    Tag = 14
    Left = 76
    Top = 435
    Width = 45
    Height = 22
    TabOrder = 14
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote15: TEdit
    Tag = 15
    Left = 76
    Top = 459
    Width = 45
    Height = 22
    TabOrder = 15
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object eNote16: TEdit
    Tag = 16
    Left = 76
    Top = 484
    Width = 45
    Height = 22
    TabOrder = 16
    OnKeyDown = eNote1KeyDown
    OnKeyPress = eNote1KeyPress
  end
  object cbSlide1: TCheckBox
    Tag = 1
    Left = 128
    Top = 120
    Width = 13
    Height = 17
    TabOrder = 17
    OnClick = cbSlide1Click
  end
  object cbSlide8: TCheckBox
    Tag = 8
    Left = 128
    Top = 292
    Width = 17
    Height = 17
    Caption = 'cbSlide8'
    TabOrder = 18
    OnClick = cbSlide1Click
  end
  object cbSlide2: TCheckBox
    Tag = 2
    Left = 128
    Top = 145
    Width = 17
    Height = 17
    Caption = 'cbSlide2'
    TabOrder = 19
    OnClick = cbSlide1Click
  end
  object cbSlide3: TCheckBox
    Tag = 3
    Left = 128
    Top = 169
    Width = 17
    Height = 17
    Caption = 'cbSlide3'
    TabOrder = 20
    OnClick = cbSlide1Click
  end
  object cbSlide4: TCheckBox
    Tag = 4
    Left = 128
    Top = 194
    Width = 17
    Height = 17
    Caption = 'cbSlide4'
    TabOrder = 21
    OnClick = cbSlide1Click
  end
  object cbSlide5: TCheckBox
    Tag = 5
    Left = 128
    Top = 218
    Width = 17
    Height = 17
    Caption = 'cbSlide5'
    TabOrder = 22
    OnClick = cbSlide1Click
  end
  object cbSlide6: TCheckBox
    Tag = 6
    Left = 128
    Top = 243
    Width = 17
    Height = 17
    Caption = 'cbSlide6'
    TabOrder = 23
    OnClick = cbSlide1Click
  end
  object cbSlide7: TCheckBox
    Tag = 7
    Left = 128
    Top = 267
    Width = 17
    Height = 17
    Caption = 'cbSlide7'
    TabOrder = 24
    OnClick = cbSlide1Click
  end
  object cbSlide9: TCheckBox
    Tag = 9
    Left = 128
    Top = 316
    Width = 17
    Height = 17
    Caption = 'cbSlide9'
    TabOrder = 25
    OnClick = cbSlide1Click
  end
  object cbSlide10: TCheckBox
    Tag = 10
    Left = 128
    Top = 341
    Width = 17
    Height = 17
    Caption = 'cbSlide10'
    TabOrder = 26
    OnClick = cbSlide1Click
  end
  object cbSlide11: TCheckBox
    Tag = 11
    Left = 128
    Top = 365
    Width = 17
    Height = 17
    Caption = 'cbSlide11'
    TabOrder = 27
    OnClick = cbSlide1Click
  end
  object cbSlide12: TCheckBox
    Tag = 12
    Left = 128
    Top = 390
    Width = 17
    Height = 17
    Caption = 'cbSlide12'
    TabOrder = 28
    OnClick = cbSlide1Click
  end
  object cbSlide13: TCheckBox
    Tag = 13
    Left = 128
    Top = 414
    Width = 17
    Height = 17
    Caption = 'cbSlide13'
    TabOrder = 29
    OnClick = cbSlide1Click
  end
  object cbSlide14: TCheckBox
    Tag = 14
    Left = 128
    Top = 439
    Width = 17
    Height = 17
    Caption = 'cbSlide14'
    TabOrder = 30
    OnClick = cbSlide1Click
  end
  object cbSlide15: TCheckBox
    Tag = 15
    Left = 128
    Top = 463
    Width = 17
    Height = 17
    Caption = 'cbSlide15'
    TabOrder = 31
    OnClick = cbSlide1Click
  end
  object cbSlide16: TCheckBox
    Tag = 16
    Left = 128
    Top = 488
    Width = 17
    Height = 17
    Caption = 'cbSlide16'
    TabOrder = 32
    OnClick = cbSlide1Click
  end
  object cbAccent1: TCheckBox
    Tag = 1
    Left = 148
    Top = 120
    Width = 17
    Height = 17
    Caption = 'cbAccent1'
    TabOrder = 33
    OnClick = cbAccent1Click
  end
  object cbAccent8: TCheckBox
    Tag = 8
    Left = 148
    Top = 292
    Width = 17
    Height = 17
    Caption = 'CheckBox2'
    TabOrder = 34
    OnClick = cbAccent1Click
  end
  object cbAccent2: TCheckBox
    Tag = 2
    Left = 148
    Top = 145
    Width = 17
    Height = 17
    Caption = 'CheckBox3'
    TabOrder = 35
    OnClick = cbAccent1Click
  end
  object cbAccent3: TCheckBox
    Tag = 3
    Left = 148
    Top = 169
    Width = 17
    Height = 17
    Caption = 'CheckBox4'
    TabOrder = 36
    OnClick = cbAccent1Click
  end
  object cbAccent4: TCheckBox
    Tag = 4
    Left = 148
    Top = 194
    Width = 17
    Height = 17
    Caption = 'CheckBox5'
    TabOrder = 37
    OnClick = cbAccent1Click
  end
  object cbAccent5: TCheckBox
    Tag = 5
    Left = 148
    Top = 218
    Width = 17
    Height = 17
    Caption = 'CheckBox6'
    TabOrder = 38
    OnClick = cbAccent1Click
  end
  object cbAccent6: TCheckBox
    Tag = 6
    Left = 148
    Top = 243
    Width = 17
    Height = 17
    Caption = 'CheckBox7'
    TabOrder = 39
    OnClick = cbAccent1Click
  end
  object cbAccent7: TCheckBox
    Tag = 7
    Left = 148
    Top = 267
    Width = 17
    Height = 17
    Caption = 'CheckBox8'
    TabOrder = 40
    OnClick = cbAccent1Click
  end
  object cbAccent9: TCheckBox
    Tag = 9
    Left = 148
    Top = 316
    Width = 17
    Height = 17
    Caption = 'CheckBox9'
    TabOrder = 41
    OnClick = cbAccent1Click
  end
  object cbAccent10: TCheckBox
    Tag = 10
    Left = 148
    Top = 341
    Width = 17
    Height = 17
    Caption = 'CheckBox10'
    TabOrder = 42
    OnClick = cbAccent1Click
  end
  object cbAccent11: TCheckBox
    Tag = 11
    Left = 148
    Top = 365
    Width = 17
    Height = 17
    Caption = 'CheckBox11'
    TabOrder = 43
    OnClick = cbAccent1Click
  end
  object cbAccent12: TCheckBox
    Tag = 12
    Left = 148
    Top = 390
    Width = 17
    Height = 17
    Caption = 'CheckBox12'
    TabOrder = 44
    OnClick = cbAccent1Click
  end
  object cbAccent13: TCheckBox
    Tag = 13
    Left = 148
    Top = 414
    Width = 17
    Height = 17
    Caption = 'CheckBox13'
    TabOrder = 45
    OnClick = cbAccent1Click
  end
  object cbAccent14: TCheckBox
    Tag = 14
    Left = 148
    Top = 439
    Width = 17
    Height = 17
    Caption = 'CheckBox14'
    TabOrder = 46
    OnClick = cbAccent1Click
  end
  object cbAccent15: TCheckBox
    Tag = 15
    Left = 148
    Top = 463
    Width = 17
    Height = 17
    Caption = 'CheckBox15'
    TabOrder = 47
    OnClick = cbAccent1Click
  end
  object cbAccent16: TCheckBox
    Tag = 16
    Left = 148
    Top = 488
    Width = 17
    Height = 17
    Caption = 'CheckBox16'
    TabOrder = 48
    OnClick = cbAccent1Click
  end
  object btnAdd: TButton
    Left = 76
    Top = 36
    Width = 29
    Height = 17
    Hint = 'Add pattern'
    Caption = 'add'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 49
    OnClick = btnAddClick
  end
  object btnDel: TButton
    Left = 76
    Top = 56
    Width = 29
    Height = 17
    Hint = 'Delete Pattern'
    Caption = 'del'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 51
    OnClick = btnDelClick
  end
  object btnPlus: TButton
    Left = 108
    Top = 36
    Width = 29
    Height = 17
    Hint = 'Increase Patternnumber'
    Caption = '+'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 52
    OnClick = btnPlusClick
  end
  object btnMinus: TButton
    Left = 108
    Top = 56
    Width = 29
    Height = 17
    Hint = 'Increase Patternnumber'
    Caption = '-'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 53
    OnClick = btnMinusClick
  end
  object eRandom: TEdit
    Left = 332
    Top = 120
    Width = 29
    Height = 22
    TabOrder = 62
    Text = '50'
  end
  object eFilename: TEdit
    Left = 4
    Top = 4
    Width = 121
    Height = 22
    TabOrder = 63
    Text = 'c:\cb303.wav'
  end
  object seFROM: TSpinEdit
    Left = 228
    Top = 4
    Width = 45
    Height = 23
    MaxValue = 1
    MinValue = 1
    TabOrder = 64
    Value = 1
    OnChange = seFROMChange
  end
  object seTO: TSpinEdit
    Left = 228
    Top = 28
    Width = 45
    Height = 23
    MaxValue = 1
    MinValue = 1
    TabOrder = 65
    Value = 1
    OnChange = seTOChange
  end
  object ProgressBar: TProgressBar
    Left = 336
    Top = 4
    Width = 150
    Height = 16
    Min = 0
    Max = 100
    TabOrder = 66
    Visible = False
  end
  object MainMenu1: TMainMenu
    Left = 264
    Top = 384
    object File1: TMenuItem
      Caption = 'File'
      object New1: TMenuItem
        Caption = 'New'
        ShortCut = 16462
        OnClick = New1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Loadmodule1: TMenuItem
        Caption = 'Load module'
        ShortCut = 16460
        OnClick = Loadmodule1Click
      end
      object Savemodule1: TMenuItem
        Caption = 'Save module'
        ShortCut = 16467
        OnClick = Savemodule1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
      object Copypattern1: TMenuItem
        Caption = 'Copy pattern'
        ShortCut = 16451
        OnClick = Copypattern1Click
      end
      object Cutpattern1: TMenuItem
        Caption = 'Cut pattern'
        ShortCut = 16472
        OnClick = Cutpattern1Click
      end
      object Pastepattern1: TMenuItem
        Caption = 'Paste pattern'
        ShortCut = 16470
        OnClick = Pastepattern1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Pattern11: TMenuItem
        Caption = 'Pattern -1'
        ShortCut = 16500
        OnClick = Pattern11Click
      end
      object Pattern12: TMenuItem
        Caption = 'Pattern +1'
        ShortCut = 16501
        OnClick = Pattern12Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Chaospattern1: TMenuItem
        Caption = 'Chaos pattern'
        ShortCut = 16466
        OnClick = Chaospattern1Click
      end
      object Chaospatternnicely1: TMenuItem
        Caption = 'Chaos pattern nicely'
        ShortCut = 16453
        OnClick = Chaospatternnicely1Click
      end
    end
  end
  object sdSave: TSaveDialog
    DefaultExt = 'cb'
    Filter = 'CreakBox file|*.cb'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 196
    Top = 408
  end
  object odOpen: TOpenDialog
    DefaultExt = 'cb'
    Filter = 'CreakBox file|*.cb'
    Left = 228
    Top = 388
  end
  object x: TPopupMenu
    AutoPopup = False
    Left = 364
    Top = 400
    object OctaveUp1: TMenuItem
      Caption = 'OctaveUp'
      ShortCut = 16503
      OnClick = btnOctUpClick
    end
    object OctaveDn1: TMenuItem
      Caption = 'OctaveDn'
      ShortCut = 16502
      OnClick = btnOctMinusClick
    end
    object AddOrderNR1: TMenuItem
      Caption = 'Add OrderNR'
      ShortCut = 16429
      OnClick = btnAddClick
    end
    object DeleteOrderNR1: TMenuItem
      Caption = 'Delete OrderNR'
      ShortCut = 16430
      OnClick = btnDelClick
    end
    object Rotatepatternup1: TMenuItem
      Caption = 'Rotate pattern up'
      ShortCut = 8238
      OnClick = Rotatepatternup1Click
    end
    object rotatepatterndown1: TMenuItem
      Caption = 'rotate pattern down'
      ShortCut = 8237
      OnClick = rotatepatterndown1Click
    end
  end
end