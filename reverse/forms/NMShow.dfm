object NMShow: TNMShow
  Left = 359
  Top = 165
  BorderStyle = bsDialog
  Caption = 'Netmasters '
  ClientHeight = 225
  ClientWidth = 359
  Color = clWhite
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 234
    Height = 13
    Caption = 'Copyright © 1996-1998 NetMasters L.L.C'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label2: TLabel
    Left = 128
    Top = 40
    Width = 115
    Height = 13
    Caption = 'To Register Component,'
  end
  object Label3: TLabel
    Left = 128
    Top = 88
    Width = 89
    Height = 13
    Caption = 'Call  603-578-2200'
  end
  object Label4: TLabel
    Left = 152
    Top = 104
    Width = 135
    Height = 13
    Caption = '1-888-2-GET-WEB  (In USA)'
  end
  object Label5: TLabel
    Left = 128
    Top = 120
    Width = 89
    Height = 13
    Caption = 'Fax  603-578-2228'
  end
  object Label6: TLabel
    Left = 128
    Top = 136
    Width = 149
    Height = 13
    Caption = 'E-mail   info@netmastersllc.com'
  end
  object Label7: TLabel
    Left = 128
    Top = 72
    Width = 119
    Height = 13
    Caption = 'Contact NetMasters LLC '
  end
  object Label8: TLabel
    Left = 160
    Top = 152
    Width = 142
    Height = 13
    Caption = 'http://www.netmastersllc.com'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = Label8Click
  end
  object Image1: TImage
    Left = 16
    Top = 40
    Width = 97
    Height = 49
    Picture.Data = <5506 bytes binary>
  end
  object Label9: TLabel
    Left = 144
    Top = 56
    Width = 166
    Height = 13
    Caption = '  Obtain  Support and Source Code'
  end
  object Label10: TLabel
    Left = 128
    Top = 152
    Width = 26
    Height = 13
    Caption = 'Web '
  end
  object OKBtn: TButton
    Left = 167
    Top = 180
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
end