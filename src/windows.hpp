//
// windows.hpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#define PARAMETERDIALOGWIDTH    300
#define SWITCH  1
#define SLIDER  2

// DialogElement
class CDialogElement : public CIDLink<CDialogElement> {
  CVarKey *Variable;
  CWnd *Wnd;
  char Type;
  friend class CParameterDialog;
};

// CParameterDialog
class CParameterDialog : public CWnd {
  CIDList<CDialogElement> ElementList;
  CWnd *Button1, *Button2, *Button3;
  CPoint Pos;
  CSize  Size;
protected:
  void OnCommand( DWORD ID );
  void ResizeWindow( CWnd *Wnd, short Height );
  void RegisterVariable( CWnd *Wnd, CVarKey *Variable, char Type );
  void SetValues();
  void DefaultValues();
public:
  CParameterDialog() : Pos( 5,10 ), Size( PARAMETERDIALOGWIDTH-10, 0 ) {}
  void Create( const char *Text );
  void Group( const char *Text );
  void EndGroup();
  void Add( const char *Text, CBVar &Variable );
  void Add( const char *Text, CF16Var &Variable );
};


