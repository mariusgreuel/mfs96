//
// dispobj.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "object.hpp"

#define MAXPOINTS       100

static FIX32 BackPlane = -1.0;
static CRect Window;
static CBPoint PointBuffer1[ MAXPOINTS ];
static CBPoint PointBuffer2[ MAXPOINTS ];

static void BackIntersection( C3D_Point *p1, C3D_Point *p2, CBPoint *b )
{
  short AddX = Window.left + ( Window.right - Window.left ) / 2;
  short AddY = Window.top + ( Window.bottom - Window.top ) / 2;
  CVector dv = p2->tp - p1->tp;
  FIX32 d = BackPlane - p1->tp.v[Z];
  b->x = (long)((p1->tp.v[X] + dv.v[X]/dv.v[Z]*d ) / BackPlane ).Int() + AddX;
  b->y = (long)((p1->tp.v[Y] + dv.v[Y]/dv.v[Z]*d ) / BackPlane ).Int() + AddY;
}

static short LeftIntersection( CBPoint *p1, CBPoint *p2 )
{
  CBPoint dp = *p2 - *p1;
  return p1->y + ( Window.left - p1->x ) * dp.y / dp.x;
}

static short RightIntersection( CBPoint *p1, CBPoint *p2 )
{
  CBPoint dp = *p2 - *p1;
  return p1->y + ( Window.right - p1->x - 1 ) * dp.y / dp.x;
}

static short TopIntersection( CBPoint *p1, CBPoint *p2 )
{
  CBPoint dp = *p2 - *p1;
  return p1->x + ( Window.top - p1->y ) * dp.x / dp.y;
}

static short BottomIntersection( CBPoint *p1, CBPoint *p2 )
{
  CBPoint dp = *p2 - *p1;
  return p1->x + ( Window.bottom - p1->y - 1 ) * dp.x / dp.y;
}

static int ClipBack( int Points, C3D_Point **pt, CBPoint *Buffer )
{
  int r1, r2;
  C3D_Point *p1, *p2;
  CBPoint *b;

  b = Buffer;
  p1 = pt[ Points - 1 ];
  r1 = p1->tp.v[Z] < BackPlane ? 0 : 1;
  for( int i = 0; i<Points; i++ )
  {
    p2 = *pt; pt++;
    r2 = p2->tp.v[Z] < BackPlane ? 0 : 1;
    if( r1 == 0 ) {
      *b = p1->pp;
      b++;
    }
    if( r1 ^ r2 ) {
      BackIntersection( p1, p2, b );
      b++;
    }
    p1 = p2;
    r1 = r2;
  }
  return b - Buffer;
}

static int ClipLeft( int Points, CBPoint *Point, CBPoint *Buffer )
{
  int r1, r2;
  CBPoint *b, *p1, *p2;

  b = Buffer;
  p1 = Point + Points-1;
  r1 = p1->x >= Window.left ? 0 : 1;
  p2 = Point;
  for( int i=0; i<Points; i++ ) {
    r2 = p2->x >= Window.left ? 0 : 1;
    if( r1 == 0 ) {
      *b = *p1;
      b++;
    }
    if( r1 ^ r2 ) {
      b->x = Window.left;
      b->y = LeftIntersection( p1, p2 );
      b++;
    }
    p1 = p2;
    r1 = r2;
    p2++;
  }
  return b - Buffer;
}

static int ClipRight( int Points, CBPoint *Point, CBPoint *Buffer )
{
  int r1, r2;
  CBPoint *b, *p1, *p2;

  b = Buffer;
  p1 = Point + Points-1;
  r1 = p1->x < Window.right ? 0 : 1;
  p2 = Point;
  for( int i=0; i<Points; i++ )
  {
    r2 = p2->x < Window.right ? 0 : 1;
    if( r1 == 0 ) {
      *b = *p1;
      b++;
    }
    if( r1 ^ r2 ) {
      b->x = Window.right-1;
      b->y = RightIntersection( p1, p2 );
      b++;
    }
    p1 = p2;
    r1 = r2;
    p2++;
  }
  return b - Buffer;
}

static int ClipTop( int Points, CBPoint *Point, CBPoint *Buffer )
{
  int r1, r2;
  CBPoint *b, *p1, *p2;

  b = Buffer;
  p1 = Point + Points - 1;
  r1 = p1->y >= Window.top ? 0 : 1;
  p2 = Point;
  for( int i=0; i<Points; i++ )
  {
    r2 = p2->y >= Window.top ? 0 : 1;
    if( r1 == 0 ) {
      *b = *p1;
      b++;
    }
    if( r1 ^ r2 ) {
      b->x = TopIntersection( p1, p2 );
      b->y = Window.top;
      b++;
    }
    p1 = p2;
    r1 = r2;
    p2++;
  }
  return b - Buffer;
}

static int ClipBottom( int Points, CBPoint *Point, CBPoint *Buffer )
{
  int r1, r2;
  CBPoint *b, *p1, *p2;

  b = Buffer;
  p1 = Point + Points - 1;
  r1 = p1->y < Window.bottom ? 0 : 1;
  p2 = Point;
  for( int i=0; i<Points; i++ )
  {
    r2 = p2->y < Window.bottom ? 0 : 1;
    if( r1 == 0 ) {
      *b = *p1;
      b++;
    }
    if( r1 ^ r2 ) {
      b->x = BottomIntersection( p1, p2 );
      b->y = Window.bottom-1;
      b++;
    }
    p1 = p2;
    r1 = r2;
    p2++;
  }
  return b - Buffer;
}

// ermittelt, ob das Polygon die Vor- oder Rückseite zeigt
// Vektoren aus den ersten drei Punkten, (v1 x v2).z > 0
static int IsFrontSide( CBspPlane *p )
{
  return (p->Point[1]->pp.x - p->Point[0]->pp.x)*(p->Point[2]->pp.y - p->Point[0]->pp.y) -
         (p->Point[1]->pp.y - p->Point[0]->pp.y)*(p->Point[2]->pp.x - p->Point[0]->pp.x)
         > 0.0 ? 1 : 0;
}


int StorePoints( CBspPlane *p )
{
  int pts = p->Points;
  pts = ClipBack( pts, p->Point, PointBuffer1 );
  pts = ClipLeft( pts, PointBuffer1, PointBuffer2 );
  pts = ClipRight( pts, PointBuffer2, PointBuffer1 );
  pts = ClipTop( pts, PointBuffer1, PointBuffer2 );
  pts = ClipBottom( pts, PointBuffer2, PointBuffer1 );
  return pts;
}

// BSP-Baum durchlaufen
void DisplayPolygon( CBspPlane *p )
{
  if( p == NULL )
    return;
  if( IsFrontSide( p ) ) {
    DisplayPolygon( p->Behind );
    if( p->Color.FGD != COLOR_NULL || p->Color.FGF != COLOR_NULL ) {
      int pts = StorePoints( p );
      if( pts >= 3 ) {
        if( p->Color.FGF != COLOR_NULL ) {
          _SetColor( p->Color.FGF );
          if( p->Color.FGF != COLOR_SHADOW )
            _FillPolygon( pts, (short*)PointBuffer1 );
          else
            _FillPolygonT( pts, (short*)PointBuffer1 );
        }
        if( p->Color.FGD < COLOR_SHADOW ) {
          _SetColor( p->Color.FGD );
          _DrawPolygon( pts, (short*)PointBuffer1 );
        }
      }
    }
    DisplayPolygon( p->Before );
  } else {
    DisplayPolygon( p->Before );
    if( p->Color.BGD != COLOR_NULL || p->Color.BGF != COLOR_NULL ) {
      int pts = StorePoints( p );
      if( pts >= 3 ) {
        if( p->Color.BGF != COLOR_NULL ) {
          _SetColor( p->Color.BGF );
          if( p->Color.BGF != COLOR_SHADOW )
            _FillPolygon( pts, (short*)PointBuffer1 );
          else
            _FillPolygonT( pts, (short*)PointBuffer1 );
        }
        if( p->Color.BGD < COLOR_SHADOW ) {
          _SetColor( p->Color.BGD );
          _DrawPolygon( pts, (short*)PointBuffer1 );
        }
      }
    }
    DisplayPolygon( p->Behind );
  }
}

// transformiert und projeziert die Objektpunkte
static void TransformPoints( int Points, C3D_Point *Point, const CMatrix &Matrix )
{
  short AddX = Window.left + ( Window.right - Window.left ) / 2;
  short AddY = Window.top + ( Window.bottom - Window.top ) / 2;
  for( int i=0; i<Points; i++ ) {
    for( int j=0; j<3; j++ ) {
      FIX32 d = 0.0;
      for( int k=0; k<3; k++ )
        d += Matrix.v[k].v[j] * Point->op.v[k];
      d += Matrix.v[H].v[j];
      Point->tp.v[j] = d;
    }
    if( abs( Point->tp.v[Z] ) > 1.0 ) {
      Point->pp.x = (short)( Point->tp.v[X] / Point->tp.v[Z] ) + AddX;
      Point->pp.y = (short)( Point->tp.v[Y] / Point->tp.v[Z] ) + AddY;
    }
    Point++;
  }
}

void SetWindowDimension( CRect *r )
{
  Window = *r;
}

void DisplayObject( CBspObject *o, const CMatrix &m )
{
  TransformPoints( o->Points, o->Point, m );
  DisplayPolygon( o->BspRoot);
}
