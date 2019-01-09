//
// mywin.hpp, tiny windows managment
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _MYWIN_HPP_INCLUDED
#define _MYWIN_HPP_INCLUDED

#include "types.hpp"
#include "list.hpp"
#include "mouse.hpp"
#include "context.hpp"

// RedrawWindow Flags
#define RDW_NONCLIENT   1
#define RDW_CLIENT      2
#define RDW_CHILDREN    8
#define RDW_INVALID1    0x100
#define RDW_INVALID2    0x200
#define RDW_INVALID     (RDW_INVALID1|RDW_INVALID2)
#define RDW_ALL         -1

// Fensterstile
#define WS_VISIBLE      1
#define WS_CHILD        2
#define WS_MOVEABLE     4
#define WS_BORDER       0x100
#define WS_THICKFRAME   0x200
#define WS_BACKGROUND   0x400
#define WS_CAPTION      0x800
#define WS_OVERLAPPED   (WS_VISIBLE|WS_THICKFRAME|WS_BACKGROUND|WS_CAPTION)

// WindowFlag
#define WF_ACTIVE       1

// Stile für SetWindowPos/Size
#define SW_ABSOLUTE     1
#define SW_RELATIVE     2
#define SW_CENTER       4

// Stile für ResizeClientWindow
#define RR_LEFT         1
#define RR_TOP          2
#define RR_RIGHT        4
#define RR_BOTTOM       8
#define RR_ALL          (RR_LEFT+RR_RIGHT+RR_TOP+RR_BOTTOM)

#define BORDER_WIDTH            1
#define THICK_FRAME_WIDTH       4
#define CAPTION_HEIGHT          12

// vordefinierte ID's
enum {
  ID_FALSE = -1, ID_NULL = 0,
  ID_OK, ID_CANCEL, ID_YES, ID_NO, ID_DEFAULT, ID_HELP, 
  ID_USER = 0x100,
};

class CDC;
class CMenu;

// Basisklasse aller Fenster
class CWnd : public CIDLink<CWnd>, public CRect {
  static CWnd   *MainWindow;    // Zeiger auf Hauptfenster
  static CWnd   *FocusWindow;   // Zeiger auf Fenster mit Focus
  static CWnd   *CaptureWindow; // Zeiger auf Fenster mit Mousefang
  static DWORD  ReturnID;       // Zurückgeliefertes ID
  CWnd          *Parent;        // Elternfenster
  CIDList<CWnd> ChildList;      // Kindsfensterliste
  CRect         Client;         // Abmaße des Clientbereichs
  const char    *WindowText;    // Text des Fensters
  DWORD         Style;          // Fensterstil
  DWORD         WindowFlag;     // Statusflag
  DWORD         RedrawFlag;     // Neuzeichenflag
private:
// Fensterzeichenfunktionen
  void DrawBorder();
  void DrawThickFrame();
  void DrawBackground();
public:
// Fenstermeldungen
  virtual BOOL OnCreate() { return FALSE; }
  virtual void OnClose() { delete this; }
  virtual void OnPaint() {}
  virtual void OnNCPaint();
  virtual void OnMove( CPoint ) {}
  virtual void OnSize( CSize ) {}
  virtual void OnCommand( DWORD ) {}
  virtual void OnChar( DWORD ) {}
  virtual void OnMouseMove( WORD, CPoint ) {}
  virtual void OnLButtonDown( WORD, CPoint ) {}
  virtual void OnRButtonDown( WORD, CPoint ) {}
  virtual void OnLButtonUp( WORD, CPoint ) {}
  virtual void OnRButtonUp( WORD, CPoint ) {}
public:
// Konstruktion, Initialisierung, Destruktion
  CWnd();
  virtual ~CWnd();
  BOOL Create( const char *WindowName, DWORD Style, const CRect &Rect, CWnd *Parent );
// Keyboard und Mouse input
  void SetFocus() { FocusWindow = this; }
  void ReleaseFocus() { FocusWindow = NULL; }
  CWnd* GetFocus() { return FocusWindow; }
  void SetCapture() { CaptureWindow = this; }
  void ReleaseCapture() { CaptureWindow = NULL; }
  CWnd* GetCapture() { return CaptureWindow; }
// Fensterzugriffsfunktionen
  CWnd *GetParent() const { return Parent; }
// Positions- und Größenfunktionen
  void CalcWindowRect( CRect &r, DWORD Style ) const;
  CPoint GetWindowPos() const { return CPoint( x, y ); }
  CSize GetWindowSize() const { return CSize( width, height ); }
  void GetWindowRect( CRect &r ) const { r.x = x; r.y = y; r.width = x+width, r.height = y+height; }
  void GetClientRect( CRect &r ) const { r.x = r.y = 0; r.width = Client.width, r.height = Client.height; }
  void GetAbsClientRect( CRect &r ) const { r.x = Client.x; r.y = Client.y; r.width = Client.x + Client.width, r.height = Client.y + Client.height; }
  BOOL IsInWindowRect( CPoint p ) const { return ( p.x<x | p.y<y | p.x>=x+width | p.y>=y+height ) ? FALSE : TRUE; }
  BOOL IsInClientRect( CPoint p ) const { return ( p.x<0 | p.y<0 | p.x>=Client.width | p.y>=Client.height ) ? FALSE : TRUE; }
  void SetWindowPos( CPoint p, BYTE Flag = 0 );
  void SetWindowSize( CSize s, BYTE Flag = 0 );
  void ResizeClientWindow( short Pixel, BYTE Edge );
// Fenstertextfunktionen
  void SetWindowText( const char *Text ) { WindowText = Text; RedrawWindow( RDW_CLIENT ); }
  const char *GetWindowText() const { return WindowText; }
// ZeichenContext-Funktionen
  CDC *GetWindowDC() const { CRect r; MainWindow->GetWindowRect( r ); return new CDC( r ); }
  CDC *GetDC() const { CRect r; r.left = Client.x; r.top = Client.y; r.right = Client.x + Client.width; r.bottom = Client.y + Client.height; return new CDC( r ); }
  void ReleaseDC( CDC *hDC ) const { delete hDC; }
// Darstellung
  void UpdateWindow( DWORD Flags = 0 );
  void RedrawWindow( DWORD Flags = RDW_ALL );
// Fenstererweiterungen
  BOOL SetCaption();
  BOOL SetMenu( CMenu *Menu );
  BOOL SetStatusBar();
// Meldungsversendefunktionen
private:
  void SendKeyboardMessages( DWORD Key );
  void SendMouseMessages();
  BOOL ProcessKeyboardMessages( DWORD Key );
  BOOL ProcessMouseMessages();
public:
// Prozessfunktionen
  void SetReturnID( DWORD ID );
  DWORD GetReturnID( BOOL Modal );
  friend void ProcessMessages();
// Trash
public:
  DWORD GetFlag() { return WindowFlag; }
  DWORD GetStyle() { return Style; }
};

// Verschiedenes
void SetEscapeFlag();
BOOL GetEscapeFlag();

#endif // _MYWIN_HPP_INCLUDED

