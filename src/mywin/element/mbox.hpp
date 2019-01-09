//
// mbox.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

// MessageBox Styles
#define PREDEFINED_BUTTONS      6
#define MB_OK                   1
#define MB_CANCEL               2
#define MB_YES                  4
#define MB_NO                   8
#define MB_DEFAULT              0x10
#define MB_HELP                 0x20
#define MB_OKCANCEL             (MB_OK|MB_CANCEL)
#define MB_YESNO                (MB_YES|MB_NO)
#define MB_YESNOCANCEL          (MB_YES|MB_NO|MB_CANCEL)
#define MB_BUTTONSTYLES         (MB_OK|MB_CANCEL|MB_YES|MB_NO)
#define MB_DEFBUTTON1           0x0
#define MB_DEFBUTTON2           0x100
#define MB_DEFBUTTON3           0x200

// Klasse CMessageBox
class CMessageBox : public CWnd {
  int  GetNumButtons( DWORD Style );
  void CalcTextSize( CRect &r, const char *Text );
  void CalcButtonSize( CRect &r, DWORD Style );
  void SetupText( const char *Text );
  void SetupButtons( DWORD Style );
protected:
  void OnCommand( DWORD Cmd ) { SetReturnID( Cmd ); }
public:
  Create( CWnd *Parent, char *Text, char *Title, DWORD Style );
  friend WORD MessageBox( char *Text, char *Title = NULL, DWORD Style = MB_OK );
};

