//
// windows.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include <stdio.h>
#include <fstream.h>
#include "mywin.hpp"
#include "elements.hpp"
#include "variable.hpp"
#include "object.hpp"
#include "airplane.hpp"
#include "vars.hpp"
#include "mfs.hpp"
#include "windows.hpp"

extern CVarList VarList;
extern CGraphic Graphic;
extern CObject *Airplane;

CStatusBar *StatusBar;

enum {
  CM_FILE = 0x200, CM_FILENEW, CM_FILEEND,
  CM_AIRPLANE, CM_HELICOPTER,
  CM_RCBOX, CM_JOYSTICK,
  CM_COMMON,
  CM_MISC, CM_ENGINE, CM_DRAG,
  CM_MAINROTOR,
  CM_PARMWIND, CM_PARMWINDCOMMON, CM_PARMWINDCHANGES, CM_PARMWINDGUST, 
  CM_PARMSAVE,
  CM_OPTIONCONTROLRCBOX,
  CM_DIV, CM_GPF, CM_SF, 
};

void DIV()
{
  int a;
  float b;
  a = 0;
  a = 10 / a;
  b = 10 / a;
}

void GPF()
{
  char __far *p = 0;
  *p = 0;
}

#pragma aux SF = \
"L1: push eax" \
    "jmp L1"

BOOL SaveVariableList()
{
  fstream OutFile;
  OutFile.open( "DEFAULT.IF", ios::out );
  if( OutFile.fail() ) {
    return TRUE;
  }
  OutFile << ";\n;Do not modify! File was generated automatically by MFS.\n;\n";
  OutFile << VarList;
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// MainWindow

void DialogParmWindCommon()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Stetiger Wind" );
  Dialog->Add( "Wind ein",      WindOn );
  Dialog->Add( "Windstärke",    WindBaseSpeed );
  Dialog->Add( "Richtung",      WindBaseDir );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void DialogParmWindChanges()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Windänderungen" );
  Dialog->Group( "Änderung der Windstärke" );
  Dialog->Add( "Häufigkeit",            WindSpeedChgFreq );
  Dialog->Add( "Bereich",               WindSpeedChgRange );
  Dialog->Add( "Beschleunigung",        WindSpeedRateOfChg );
  Dialog->Add( "Dauer Minimal",         WindSpeedChgDurMin );
  Dialog->Add( "Dauer Maximal",         WindSpeedChgDurMax );
  Dialog->EndGroup();
  Dialog->Group( "Änderung der Windrichtung" );
  Dialog->Add( "Häufigkeit",            WindDirChgFreq );
  Dialog->Add( "Bereich",               WindDirChgRange );
  Dialog->Add( "Beschleunigung",        WindDirRateOfChg );
  Dialog->Add( "Dauer Minimal",         WindDirChgDurMin );
  Dialog->Add( "Dauer Maximal",         WindDirChgDurMax );
  Dialog->GetReturnID( TRUE );
  Dialog->EndGroup();
  delete Dialog;
}

void DialogParmWindGust()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Böen" );
  Dialog->Add( "Häufigkeit",            GustFreq );
  Dialog->Add( "Spitzenwert Minimal",   GustPeakMin );
  Dialog->Add( "Spitzenwert Maximal",   GustPeakMax );
  Dialog->Add( "Dauer Minimal",         GustDirMin );
  Dialog->Add( "Dauer Maximal",         GustDirMax );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void DialogParmSave()
{
  SaveVariableList();
}

void DialogOptionControlRCBox()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Fersteuerung" );
  Dialog->Group( "Invertiere" );
  Dialog->Add( " Kanal 1",      RCBoxNegChannel[0] );
  Dialog->Add( " Kanal 2",      RCBoxNegChannel[1] );
  Dialog->Add( " Kanal 3",      RCBoxNegChannel[2] );
  Dialog->Add( " Kanal 4",      RCBoxNegChannel[3] );
  Dialog->EndGroup();
  Dialog->Group( "Impulslänge" );
  Dialog->Add( "Minimal",       RCBoxMinPulseWidth );
  Dialog->Add( "Maximal",       RCBoxMaxPulseWidth );
  Dialog->EndGroup();
  Dialog->Add( "Negativ Impuls Fernsteuerung", RCBoxNegPulse );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void CommonDialog()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Verschiedenes" );
  Dialog->Add( "Bildschirm Breite/Hoehe", ScreenAspectRatio );
  Dialog->Add( "Augenhöhe",               ViewHeight );
  Dialog->Add( "Erdbeschleunigung",       Gravity );
  Dialog->Add( "Zeitfaktor",              TimeAdjust );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void MiscDialog()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Empfindlichkeit" );
  Dialog->Add( "Trägheit", MassInertia );
  Dialog->Add( "Bodeneffekt Bereich", GroundEffectRange );
  Dialog->Add( "Bodeneffekt Faktor", GroundEffectFactor );
  Dialog->Add( "Wetterfahne Gier", TurnWithWind[Y] );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void EngineDialog()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Motor" );
  Dialog->Add( "Leistung",      EnginePower );
  Dialog->Add( "Trägheit",      EngineRiseRate );
  Dialog->Add( "Auftrieb Blatt", CollectiveLift );
  Dialog->Add( "Auftrieb Motor", RPMLift );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void DragDialog()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Windwiderstand" );
  Dialog->Add( "Vorne",         Drag[X] );
  Dialog->Add( "Seite",         Drag[Z] );
  Dialog->Add( "Oben/Unten",    Drag[Y] );
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void MainRotorDialog()
{
  CParameterDialog *Dialog = new CParameterDialog;
  Dialog->Create( "Hauptrotor" );
  Dialog->Group( "Empfindlichkeit" );
  Dialog->Add( "Nick", Sensitivity[ NICK ] );
  Dialog->Add( "Roll", Sensitivity[ ROLL ] );
  Dialog->Add( "Gier", Sensitivity[ GIER ] );
  Dialog->EndGroup();
  Dialog->Group( "Widerstand" );
  Dialog->Add( "Nick", Spin[ NICK ] );
  Dialog->Add( "Roll", Spin[ ROLL ] );
  Dialog->Add( "Gier", Spin[ GIER ] );
  Dialog->EndGroup();
  Dialog->Group( "Trägheit" );
  Dialog->Add( "Nick", Inertia[ NICK ] );
  Dialog->Add( "Roll", Inertia[ ROLL ] );
  Dialog->Add( "Gier", Inertia[ GIER ] );
  Dialog->EndGroup();
  Dialog->GetReturnID( TRUE );
  delete Dialog;
}

void SetupMenu( CWnd *Parent )
{
  CMenu *Menu = new CMenu;
    CMenu *File = Menu->AppendMenu( "Datei", CM_FILE );
      File->AppendItem( "MBox", CM_FILENEW );
      File->AppendItem( "Beenden", CM_FILEEND );
    CMenu *Error = Menu->AppendMenu( "Errors", CM_FILE );
      Error->AppendItem( "Division by zero", CM_DIV );
      Error->AppendItem( "General protection fault", CM_GPF );
      Error->AppendItem( "stack fault", CM_SF );
    CMenu *Model = Menu->AppendMenu( "Modell", CM_FILE );
      Model->AppendItem( "Flugzeug", CM_AIRPLANE );
      Model->AppendItem( "Hubschrauber", CM_HELICOPTER );
    CMenu *Parm = Menu->AppendMenu( "Parmameter", 0 );
      Parm->AppendItem( "Allgemeines", CM_COMMON );
      CMenu *Helicopter = Parm->AppendMenu( "Hubschrauber", 0 );
        Helicopter->AppendItem( "Verschiedenes", CM_MISC );
        Helicopter->AppendItem( "Motor", CM_ENGINE );
        Helicopter->AppendItem( "Windwiderstand", CM_DRAG );
        Helicopter->AppendItem( "Hauptrotor", CM_MAINROTOR );
      CMenu *Wind = Parm->AppendMenu( "Wind", CM_PARMWIND );
        Wind->AppendItem( "Grundeinstellung", CM_PARMWINDCOMMON );
        Wind->AppendItem( "Änderungen", CM_PARMWINDCHANGES );
        Wind->AppendItem( "Böen", CM_PARMWINDGUST );
      Parm->AppendItem( "Sichern", CM_PARMSAVE );
    CMenu *Options = Menu->AppendMenu( "Optionen", 0 );
      CMenu *Control = Options->AppendMenu( "Steuerung >", 0 );
        Control->AppendItem( "Fernsteuerung", CM_OPTIONCONTROLRCBOX );
    CMenu *Help = Menu->AppendMenu( "Hilfe", 0 );
      Help->AppendItem( "Hilfe 1", 0 );
      Help->AppendItem( "Hilfe 2", 0 );
      Help->AppendItem( "Hilfe 3", 0 );
      Help->AppendItem( "Hilfe 4", 0 );
      Help->AppendItem( "Hilfe 5", 0 );
      Help->AppendItem( "Hilfe 6", 0 );
    Menu->AppendItem( "Abbruch!", CM_FILEEND );
  Parent->SetMenu( Menu );
}

void MfsWindow::OnPaint( void )
{
  CDC *hDC = GetDC();
  CRect r;
  GetAbsClientRect( r );
  DisplayScene( r );
  ReleaseDC( hDC );
}

void MfsWindow::OnCommand( DWORD Cmd )
{
  switch( Cmd ) {
    case CM_FILENEW    : MessageBox( "Hallo !", "Kleiner Messagebox Test", MB_OK ); break;
    case CM_COMMON:             CommonDialog(); break;
    case CM_MAINROTOR:          MainRotorDialog(); break;
    case CM_MISC:               MiscDialog(); break;
    case CM_ENGINE:             EngineDialog(); break;
    case CM_DRAG:               DragDialog(); break;
    case CM_PARMWINDCOMMON:     DialogParmWindCommon();         break;
    case CM_PARMWINDCHANGES:    DialogParmWindChanges();        break;
    case CM_PARMWINDGUST:       DialogParmWindGust();           break;
    case CM_PARMSAVE:           DialogParmSave();               break;
    case CM_OPTIONCONTROLRCBOX: DialogOptionControlRCBox();     break;
    case ID_CANCEL:
    case CM_FILEEND:            SaveVariableList(); SetEscapeFlag(); break;
    case CM_DIV: DIV(); break;
    case CM_GPF: GPF(); break;
    case CM_SF: SF(); break;
  }
}

void MfsWindow::OnChar( DWORD Key )
{
  FIX16 Inc = _2PI/4096;
  switch( Key ) {
    case VK_LEFT     : Airplane->Move( CVec16( -.1, 0.0, 0.0 ) ); break;
    case VK_RIGHT    : Airplane->Move( CVec16(  .1, 0.0, 0.0 ) ); break;
    case VK_UP       : Airplane->Move( CVec16( 0.0, 0.1, 0.0 ) ); break;
    case VK_DOWN     : Airplane->Move( CVec16( 0.0, -.1, 0.0 ) ); break;
    case VK_INSERT   : Airplane->Rotate( CVec16(  Inc, 0.0, 0.0 ) ); break;
    case VK_DELETE   : Airplane->Rotate( CVec16( -Inc, 0.0, 0.0 ) ); break;
    case VK_HOME     : Airplane->Rotate( CVec16(  0.0, 2*Inc, 0.0 ) ); break;
    case VK_END      : Airplane->Rotate( CVec16(  0.0,-Inc, 0.0 ) ); break;
    case VK_PAGEUP   : Airplane->Rotate( CVec16(  0.0, 0.0, Inc ) ); break;
    case VK_PAGEDOWN : Airplane->Rotate( CVec16(  0.0, 0.0,-Inc ) ); break;
  }
}

MfsWindow::MfsWindow( void )
{
  CRect r;
  Graphic.GetScreenRect( r );
  Create( "MFS V1.0", WS_VISIBLE|WS_CAPTION, CRect( 0,0,640,350 ), NULL );
  SetupMenu( this );
  StatusBar = new CStatusBar;
  StatusBar->Create( this );
}

/////////////////////////////////////////////////////////////////////////////
// Parm Dialog

void CParameterDialog::Create( const char *Text )
{
  CRect r( 0, 0, PARAMETERDIALOGWIDTH, STD_BUTTONHEIGHT + 20 );
  CalcWindowRect( r, WS_OVERLAPPED );
  CWnd::Create( Text, WS_OVERLAPPED|WS_MOVEABLE, r, NULL );
  Button1 = new CStdButton( MB_OK, Pos, this );
  Button2 = new CStdButton( MB_CANCEL, CPoint( Pos.x + STD_BUTTONDIST, Pos.y ), this );
  Button3 = new CStdButton( MB_DEFAULT, CPoint( Pos.x + 2*STD_BUTTONDIST, Pos.y ), this );
}

void CParameterDialog::ResizeWindow( CWnd *Wnd, short Height )
{
  Size.cy = Height;
  Wnd->SetWindowPos( Pos );
  Wnd->SetWindowSize( Size );
  Height += 4;
  Pos.y += Height;
  SetWindowSize( CSize( 0, Height ), SW_RELATIVE );
  SetWindowPos( CPoint( 0,0 ), SW_CENTER );
  Button1->SetWindowPos( CPoint( 0, Height ), SW_RELATIVE );
  Button2->SetWindowPos( CPoint( 0, Height ), SW_RELATIVE );
  Button3->SetWindowPos( CPoint( 0, Height ), SW_RELATIVE );
}

void CParameterDialog::RegisterVariable( CWnd *Wnd, CVarKey *Variable, char Type )
{
  CDialogElement *Element = new CDialogElement;
  ElementList.Append( Element );
  Element->Wnd = Wnd;
  Element->Variable = Variable;
  Element->Type = Type;
}

void CParameterDialog::Group( const char *Text )
{
  CRect r( 0,0,PARAMETERDIALOGWIDTH-10,10 );
  CText *TextWnd = new CText;
  TextWnd->Create( Text, r, this );
  ResizeWindow( TextWnd, 8 );
  Pos.x += 15;
  Size.cx -= 15;
}

void CParameterDialog::EndGroup()
{
  Pos.x -= 15;
  Size.cx += 15;
}

void CParameterDialog::Add( const char *Text, CBVar &Variable )
{
  CRect r( 0,0,0,0 );
  CSwitchButton *SwitchWnd = new CSwitchButton;
  SwitchWnd->Create( Text, r, this );
  SwitchWnd->SetValue( Variable );
  ResizeWindow( SwitchWnd, 10 );
  RegisterVariable( SwitchWnd, &Variable, SWITCH );
}

void CParameterDialog::Add( const char *Text, CF16Var &Variable )
{
  CRect r( 0,0,0,0 );
  CSlider *SlideWnd = new CSlider;
  SlideWnd->Create( Text, r, this );
  SlideWnd->SetRange( Variable.GetMin().Float(), Variable.GetMax().Float() );
  SlideWnd->SetValue( Variable.GetValue().Float() );
  ResizeWindow( SlideWnd, 15 );
  RegisterVariable( SlideWnd, &Variable, SLIDER );
}

void CParameterDialog::SetValues()
{
  for( CIDIter<CDialogElement> ElementIter( ElementList ); ElementIter(); ElementIter++ )
    switch( ElementIter()->Type ) {
      case SWITCH:
        *((CBVar*)ElementIter()->Variable) = ((CSwitchButton*)ElementIter()->Wnd)->GetValue();
        break;
      case SLIDER:
        *((CF16Var*)ElementIter()->Variable) = ((CSlider*)ElementIter()->Wnd)->GetValue();
        break;
    }
}

void CParameterDialog::DefaultValues()
{
  for( CIDIter<CDialogElement> ElementIter( ElementList ); ElementIter(); ElementIter++ )
    switch( ElementIter()->Type ) {
      case SWITCH:
        ((CSwitchButton*)ElementIter()->Wnd)->SetValue( ((CBVar*)ElementIter()->Variable)->GetDefault() );
        break;
      case SLIDER:
        ((CSlider*)ElementIter()->Wnd)->SetValue( ((CF16Var*)ElementIter()->Variable)->GetDefault().Float() );
        break;
    }
}

void CParameterDialog::OnCommand( DWORD Cmd )
{
  switch( Cmd ) {
    case ID_OK:
      SetValues();
      SetReturnID( Cmd );
      break;
    case ID_CANCEL:
      SetReturnID( Cmd );
      break;
    case ID_DEFAULT:
      DefaultValues();
      break;
  }
}


