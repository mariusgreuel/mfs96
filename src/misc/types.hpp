//
// types.hpp, Point, Size, Rect
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _TYPES_HPP_INCLUDED
#define _TYPES_HPP_INCLUDED

#include "types.h"

// Point with 'short' type
struct CSPoint {
  long x;
  long y;
};

// Point with 'long' type
struct CLPoint {
  long x;
  long y;
};

class CPoint {
public:
  short x, y;
public:
  CPoint() {}
  CPoint( const CPoint *p ) { x = p->x; y = p->y; }
  CPoint( short _x, short _y ) { x = _x; y = _y; }
  BOOL operator == ( const CPoint &p ) const { return x == p.x && y == p.y; }
  BOOL operator != ( const CPoint &p ) const { return x != p.x || y != p.y; }
  void operator += ( const CPoint &p ) { x += p.x; y += p.y; }
  void operator -= ( const CPoint &p ) { x -= p.x; y -= p.y; }
};
inline CPoint operator + ( const CPoint &p1, const CPoint &p2 ) { return CPoint( p1.x + p2.x, p1.y + p2.y ); }
inline CPoint operator - ( const CPoint &p1, const CPoint &p2 ) { return CPoint( p1.x - p2.x, p1.y - p2.y ); }

class CSize {
public:
  union { short cx; short width; };
  union { short cy; short height; };
public:
  CSize() {}
  CSize( const CSize *s ) { cx = s->cx; cy = s->cy; }
  CSize( short _cx, short _cy ) { cx = _cx; cy = _cy; }
  BOOL operator == ( const CSize &s ) const { return cx == s.cx && cy == s.cy; }
  BOOL operator != ( const CSize &s ) const { return cx != s.cx || cy != s.cy; }
  void operator += ( const CSize &s ) { cx += s.cx; cy += s.cy; }
  void operator -= ( const CSize &s ) { cx -= s.cx; cy -= s.cy; }
};
inline CSize operator + ( const CSize &s1, const CSize &s2 ) { return CSize( s1.cx + s2.cx, s1.cy + s2.cy ); }
inline CSize operator - ( const CSize &s1, const CSize &s2 ) { return CSize( s1.cx - s2.cx, s1.cy - s2.cy ); }

class CRect {
public:
  union { short x; short left; };
  union { short y; short top; };
  union { short width; short right; };
  union { short height; short bottom; };
public:
  CRect() {}
  CRect( const CRect &r ) { x = r.x; y = r.y; width = r.width; height = r.height; }
  CRect( short l, short t, short r, short b ) { left = l; top = t; right = r; bottom = b; }
};

#endif // _TYPES_HPP_INCLUDED

