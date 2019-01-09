//
// menu.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#define IF_INVERTED             1
#define PF_CAPTURED             1
#define PF_TRACKED              2
#define MBF_CAPTURED            1
#define MBF_TRACKED             2

class CPopupMenuWnd;

class CItem : public CIDLink<CItem> {
private:
  const char *Text;
  DWORD ID;
  BYTE  Style;
protected:
  BYTE Flag;
public:
  CItem() { Flag = 0; }
  virtual ~CItem() {}
  void Create( const char *Text, DWORD ID, BYTE Style );
  void DrawItem( CDC *hDC, const CRect &r );
  short GetWidth();
  virtual void Select( CWnd *Parent, CPoint p, BOOL State );
  virtual void Action( CWnd *Parent, CPoint p );
};

class CMenu : public CItem {
  CIDList<CItem> ItemList;
  CPopupMenuWnd *Popup;
public:
  ~CMenu();
  CItem *AppendItem( const char *Text, DWORD ID, DWORD Style = 0 );
  CMenu *AppendMenu( const char *Text, DWORD ID, DWORD Style = 0 );
  CSize GetSize();
  void Select( CWnd *Parent, CPoint p, BOOL State );
  void Action( CWnd *Parent, CPoint p );
  friend class CPopupMenuWnd;
  friend class CMenuBarWnd;
};

class CPopupMenuWnd : public CWnd {
  CMenu *Menu;
  CItem *CurrItem;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void OnCommand( DWORD );
  void OnMouseMove( WORD, CPoint );
  void OnLButtonDown( WORD, CPoint );
  void OnLButtonUp( WORD, CPoint );
public:
  CPopupMenuWnd() { CurrItem = NULL; }
  ~CPopupMenuWnd();
  BOOL Create( CMenu *Menu, CPoint p, CWnd *Parent );
};

class CMenuBarWnd : public CWnd {
  BYTE Flag;
  CMenu *Menu;
  CItem *CurrItem;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void OnCommand( DWORD );
  void OnMouseMove( WORD, CPoint );
  void OnLButtonDown( WORD, CPoint );
  void OnLButtonUp( WORD, CPoint );
public:
  CMenuBarWnd();
  BOOL Create( CMenu *Menu, CWnd *Parent );
};

