//
// button.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#define STD_BUTTONWIDTH         70
#define STD_BUTTONHEIGHT        20
#define STD_BUTTONDIST          (STD_BUTTONWIDTH + 10)

#define MB_OK                   1
#define MB_CANCEL               2
#define MB_YES                  4
#define MB_NO                   8
#define MB_DEFAULT              0x10
#define MB_HELP                 0x20

// ButtonFlag
#define BT_HIGHLIGHTED  1
#define BT_FOCUSED      2
#define BT_PRESSED      4
#define BT_CHECKED      8
#define BT_CAPTURED     0x10

// Button
class CButton : public CWnd {
  BYTE  Flag;
protected:
  void OnMouseMove( WORD, CPoint );
  void OnLButtonDown( WORD, CPoint );
  void OnLButtonUp( WORD, CPoint );
  virtual void Action() = 0;
public:
  CButton() { Flag = 0; }
  BYTE GetState() { return Flag; }
};

// PushButton
class CPushButton : public CButton {
  DWORD ID;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void Action() { GetParent()->OnCommand( ID ); }
public:
  BOOL Create( const char *Text, const CRect &r, CWnd *Parent, DWORD ID )
  {
    CPushButton::ID = ID;
    return CWnd::Create( Text, WS_VISIBLE|WS_CHILD|WS_BORDER, r, Parent );
  }
};

// Switch
class CSwitchButton : public CButton {
  BOOL Value;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void Action() { Value ^= TRUE; }
public:
  CSwitchButton() { Value = FALSE; }
  BOOL Create( const char *Text, const CRect &r, CWnd *Parent )
  {
    return CWnd::Create( Text, WS_VISIBLE|WS_CHILD, r, Parent );
  }
  void SetValue( BOOL Value ) { CSwitchButton::Value = Value ? TRUE : FALSE; RedrawWindow(); }
  BOOL GetValue() { return Value; }
};

// StdButton
class CStdButton : public CPushButton {
public:
  CStdButton( DWORD Type, CPoint p, CWnd *Parent )
  {
    const char *Text;
    DWORD ID;
    switch( Type ) {
      case MB_OK        : Text = "OK";          ID = ID_OK;     break;
      case MB_CANCEL    : Text = "Abbruch";     ID = ID_CANCEL; break;
      case MB_YES       : Text = "Ja";          ID = ID_YES;    break;
      case MB_NO        : Text = "Nein";        ID = ID_NO;     break;
      case MB_DEFAULT   : Text = "Standart";    ID = ID_DEFAULT;break;
      case MB_HELP      : Text = "Hilfe";       ID = ID_HELP;   break;
    }
    Create( Text, CRect( p.x, p.y, STD_BUTTONWIDTH, STD_BUTTONHEIGHT ), Parent, ID );
  }
};


