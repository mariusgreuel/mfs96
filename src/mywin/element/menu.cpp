//
// menu.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
// 

#include "mywin.hpp"
#include "menu.hpp"

#define ColorMenuText           BLACK
#define ColorMenuTextBack       GRAY
#define ColorInvMenuText        WHITE
#define ColorInvMenuTextBack    DARKBLUE

#define MenuHeight              14
#define ItemHeight              13
#define ItemAddWidth            12

/////////////////////////////////////////////////////////////////////////////
// class CItem

// neuen Menüpunkt erzeugen
void CItem::Create( const char *_Text, DWORD _ID, BYTE _Style )
{
  Text = _Text;
  ID = _ID;
  Style = _Style;
}

// Menüpunkt darstellen
void CItem::DrawItem( CDC *hDC, const CRect &_r )
{
  COLOR bg, fg;
  CRect r = _r;
  if( !( Flag & IF_INVERTED ) )
    bg = ColorMenuTextBack, fg = ColorMenuText;
  else
    bg = ColorInvMenuTextBack, fg = ColorInvMenuText;
  hDC->SetColor( bg );
  hDC->FillRect( r );
  r.x += ItemAddWidth / 2;
  hDC->SetColor( fg );
  hDC->DrawText( Text, r );
}

// Breite des Menüpunktes ermitteln
short CItem::GetWidth()
{
  return GetTextLineExtent( Text ) + ItemAddWidth;
}

// Menüpunkt ausführen
void CItem::Action( CWnd *Parent, CPoint )
{
  Parent->OnCommand( ID );
}

// Menüpunkt anwählen
void CItem::Select( CWnd *, CPoint, BOOL Select )
{
  if( Select )
    Flag |= IF_INVERTED;
  else
    Flag &= ~IF_INVERTED;
}

/////////////////////////////////////////////////////////////////////////////
// class CMenu

// CMenu Destructor
CMenu::~CMenu()
{
  CItem *Item;
  while( ( Item = ItemList.Get()) != NULL )
    delete Item;
}

// Menü einen Menüpunkt hinzufügen
CItem *CMenu::AppendItem( const char *Name, DWORD ID, DWORD Style )
{
  CItem *Item = new CItem;
  Item->Create( Name, ID, Style );
  ItemList.Append( Item );
  return Item;
}

// Menü ein Unter-Menü hinzufügen
CMenu *CMenu::AppendMenu( const char *Name, DWORD ID, DWORD Style )
{
  CMenu *Menu = new CMenu;
  Menu->Create( Name, ID, Style );
  ItemList.Append( Menu );
  return Menu;
}

// Größe des Menüs ermitteln
CSize CMenu::GetSize()
{
  CSize Size( 0, 0 );
  for( CIDIter<CItem> ItemIter( ItemList ); ItemIter(); ItemIter++ ) {
    short Width = ItemIter()->GetWidth();
    if( Size.cx < Width )
      Size.cx = Width;
    Size.cy += ItemHeight;
  }
  return Size;
}

// Menüpunkt ausführen
void CMenu::Action( CWnd *, CPoint )
{
}

// Menü anwählen
void CMenu::Select( CWnd *Parent, CPoint p, BOOL Select )
{
  if( Select ) {
    if( !( Flag & IF_INVERTED )) {
      Flag |= IF_INVERTED;
      Popup = new CPopupMenuWnd();
      Popup->Create( this, p, Parent );
    }
  } else {
    if( Flag & IF_INVERTED ) {
      Flag &= ~IF_INVERTED;
      delete Popup;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// class Popup

// CPopupMenuWnd Destructor
CPopupMenuWnd::~CPopupMenuWnd()
{
  if( CurrItem ) {
    CurrItem->Select( this, CPoint( 0,0 ), FALSE );
    CurrItem = NULL;
  }
}

// eine Popupmenü erzeugen
BOOL CPopupMenuWnd::Create( CMenu *Menu, CPoint p, CWnd *Parent )
{
  CPopupMenuWnd::Menu = Menu;
  CSize Size = Menu->GetSize();
  CRect r;
  r.x = p.x; r.y = p.y; r.width = Size.cx+2; r.height = Size.cy+2;
  return CWnd::Create( NULL, WS_VISIBLE|WS_CHILD|WS_BORDER, r, Parent );
}

// Popupmenü darstellen
void CPopupMenuWnd::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetTextAlign( TA_LEFT|TA_MIDDLE );
  for( CIDIter<CItem> ItemIter( Menu->ItemList ); ItemIter(); ItemIter++ ) {
    r.bottom = r.top + ItemHeight;
    ItemIter()->DrawItem( hDC, r );
    r.top = r.bottom;
  }
  ReleaseDC( hDC );
}

void CPopupMenuWnd::OnCommand( DWORD ID )
{
  GetParent()->OnCommand( ID );
}

void CPopupMenuWnd::OnMouseMove( WORD, CPoint p )
{
  short ItemYPos, y = 0;
  for( CIDIter<CItem> ItemIter( Menu->ItemList ); ItemIter(); ItemIter++ ) {
    ItemYPos = y;
    y += ItemHeight;
    if( p.y < y )
      break;
  }
  if( ItemIter() != CurrItem ) {
    if( CurrItem )
      CurrItem->Select( this, p, FALSE );
    CurrItem = ItemIter();
    if( CurrItem )
      CurrItem->Select( this, CPoint( width, ItemYPos ), TRUE );
    RedrawWindow();
  }
}

void CPopupMenuWnd::OnLButtonDown( WORD Key, CPoint p )
{
  OnMouseMove( Key, p );
}

void CPopupMenuWnd::OnLButtonUp( WORD, CPoint p )
{
  if( CurrItem )
    CurrItem->Action( this, p );
}

/////////////////////////////////////////////////////////////////////////////
// class CMenuBarWnd

// CMenuBarWnd Constructor
CMenuBarWnd::CMenuBarWnd()
{
  Flag = 0;
  CurrItem = NULL;
}

// eine Menüzeile erzeugen
BOOL CMenuBarWnd::Create( CMenu *_Menu, CWnd *Parent )
{
  Menu = _Menu;
  CRect r;
  Parent->GetClientRect( r );
  r.height = MenuHeight;
  CWnd::Create( NULL, WS_VISIBLE|WS_CHILD, r, Parent );
  Parent->ResizeClientWindow( -MenuHeight, RR_TOP );
  return TRUE;
}

// Menuzeile darstellen
void CMenuBarWnd::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorMenuTextBack );
  hDC->FillRect( r );
  hDC->SetTextAlign( TA_LEFT|TA_MIDDLE );
  r.top++;
  r.bottom = ItemHeight;
  for( CIDIter<CItem> ItemIter( Menu->ItemList ); ItemIter(); ItemIter++ ) {
    r.right = r.left + ItemIter()->GetWidth();
    ItemIter()->DrawItem( hDC, r );
    r.left = r.right;
  }
  ReleaseDC( hDC );
}

void CMenuBarWnd::OnCommand( DWORD ID )
{
  if( CurrItem ) {
    CurrItem->Select( this, CPoint( 0,0 ), FALSE );
    CurrItem = NULL;
    Flag &= ~MBF_TRACKED;
    RedrawWindow();
  }
  ReleaseFocus();
  GetParent()->OnCommand( ID );
}

void CMenuBarWnd::OnMouseMove( WORD, CPoint p )
{
  if( Flag & MBF_TRACKED && IsInClientRect( p )) {
    short ItemXPos, x = 0;
    for( CIDIter<CItem> ItemIter( Menu->ItemList ); ItemIter(); ItemIter++ ) {
      ItemXPos = x;
      x += ItemIter()->GetWidth();
      if( p.x < x )
        break;
    }
    if( ItemIter() != CurrItem ) {
      if( CurrItem )
        CurrItem->Select( this, p, FALSE );
      CurrItem = ItemIter();
      if( CurrItem )
        CurrItem->Select( this, CPoint( ItemXPos, height ), TRUE );
      RedrawWindow();
    }
  }
}

void CMenuBarWnd::OnLButtonDown( WORD Key, CPoint p )
{
  if( !( Flag & MBF_TRACKED )) {
    if( IsInClientRect( p )) {
      Flag |= MBF_TRACKED;
      OnMouseMove( Key, p );
      SetFocus();
    }
  } else {
    if( !IsInClientRect( p ) ) {
      OnCommand( ID_NULL );
    }
  }
}

void CMenuBarWnd::OnLButtonUp( WORD, CPoint p )
{
  if( Flag & MBF_TRACKED ) {
    if( CurrItem )
      CurrItem->Action( this, p );
  }
}

/////////////////////////////////////////////////////////////////////////////
// class CWnd Erweiterung

// einem Fenster ein Menü zuweisen
BOOL CWnd::SetMenu( CMenu *Menu )
{
  return (new CMenuBarWnd)->Create( Menu, this );
}

