object Splash: TSplash
  Left = 466
  Top = 287
  BorderIcons = [biHelp]
  BorderStyle = bsDialog
  Caption = 'Splash'
  ClientHeight = 323
  ClientWidth = 342
  Color = clBtnFace
  Constraints.MaxHeight = 350
  Constraints.MaxWidth = 350
  Constraints.MinHeight = 350
  Constraints.MinWidth = 350
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 104
    Top = 24
    Width = 110
    Height = 29
    Caption = 'CreakBox'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 132
    Top = 56
    Width = 53
    Height = 14
    Caption = 'beta notice'
  end
  object Memo1: TMemo
    Left = 8
    Top = 80
    Width = 325
    Height = 209
    Color = clMenu
    Lines.Strings = ('CreakBox beta is distributed 'as is', so use it if you can!', '', 'The CreakBox homepage is located at', '', 'http://www.geocities.com/SiliconValley/Station/5100', 'Click "CreakBox" button!', '', 'Please visit the CreakBox-page for conditions of use! You must ', 'read and agree with the conditions before using the program.', 'The page also contains updates, samples and a list of features to ', 'implement in the future!', '', 'For questions, suggestions etc. contact me at', 'frankreitz@hotmail.com')
    ReadOnly = True
    TabOrder = 0
  end
  object Button1: TButton
    Left = 128
    Top = 296
    Width = 75
    Height = 25
    Caption = 'OK!'
    TabOrder = 1
    OnClick = Button1Click
  end
end