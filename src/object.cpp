//
// object.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include <stdlib.h>
#include <math.h>
#include "vars.hpp"
#include "object.hpp"

#define ROUNDOFF        0.001

CIDList<CBspObject> CBspObject::BspObjectList;
CIDList<CObject> CObject::ObjectList;

CObject *Airplane = NULL;
CRect Window;

extern "C" void _SetWindowDimension( CRect *r );
extern "C" void _DisplayObject( CBspObject *o, const CMatrix16 *m );

/////////////////////////////////////////////////////////////////////////////
// CBspPlane

// neue Ebene erzeugen
CBspPlane::CBspPlane( int _Points, int Index[], C3D_Point *_Point, PGCOLOR _Color )
{
  Before = Behind = NULL;
  Points = _Points;
  for( int i=0; i<_Points; i++ )
    Point[i] = &_Point[ Index[i] ];
  Color = _Color;
}

// errechnet den Normalenvektor anhand der ersten drei Punkte
// N = v1 x v2, h = P0*N
CHVec16 CBspPlane::GetNormalVector()
{
  CVec16 v1 = Point[1]->op - Point[0]->op;
  CVec16 v2 = Point[2]->op - Point[0]->op;
  CVec16 vp = cross( v1, v2 );
  return CHVec16( vp, dot( Point[0]->op, vp ) );
}

// vergleicht die Punkte der Ebene mit der Referenzebene
// P*N-P0*N > 0 => Punkt vor der Ebene
int CBspPlane::LocatePoints( const CHVec16 &RefPlaneVector )
{
  int Before = 0, Behind = 0;
  for( int i = 0; i < Points; i++ ) {
    FIX16 diff = dot( Point[i]->op, RefPlaneVector ) - RefPlaneVector.h;
    if( diff > 1.0e-4 )
      Before++;
    else if ( diff < -1.0e-4 )
      Behind++;
  }
  if( Behind == 0 && Before == 0 )
    return 0;                           // alle Punkte in der Ebene
  if( Behind == 0 )
    return 2;                           // alle Punkte vor der Ebene
  if( Before == 0 )
    return -2;                          // alle Punkte hinter der Ebene
  if( Before > Behind )
    return 1;
  else
    return -1;
}

// setzt die Ebene an die Spitze des (Unter-)Baumes
void CBspPlane::SetToTop( CBspPlane **List )
{
  if( this == *List )
    return;
  CBspPlane *Plane = *List;
  while( Plane->Before != this )
    Plane = Plane->Before;
  Plane->Before = Before;
  Before = *List;
  *List = this;
}

/////////////////////////////////////////////////////////////////////////////
// CBspObject

CBspObject::CBspObject( const char *_Name, int _Points )
{
  BspObjectList.Insert( this );
  BspRoot = NULL;
  Points = 0;
  Polygons = 0;
  Point = new C3D_Point[ _Points ];
  Name = _Name;
  Shadow = NULL;
}

CBspObject::~CBspObject()
{
  delete[] Point;
  BspObjectList.Get( this );
}

// Objekt suchen
CBspObject *SearchBspObject( const char *Name )
{
  for( CIDIter<CBspObject> ObjectIter( CBspObject::BspObjectList ); ObjectIter(); ObjectIter++ )
    if( ObjectIter()->Name == Name )
      break;
  return ObjectIter();
}

// neuen Punkt hinzufügen
void CBspObject::AddPoint( const CVec16 &v )
{
  Point[ Points++ ].op = v;
}

// neues Polygon hinzufügen
void CBspObject::AddPolygon( int Points, int Index[], PGCOLOR Color )
{
  CBspPlane *pg = new CBspPlane( Points, Index, Point, Color );
  pg->Before = BspRoot;
  BspRoot = pg;
  Polygons++;
}

// Schatten zuweisen
void CBspObject::AddShadow( CBspObject *_Shadow )
{
  Shadow = _Shadow;
}

// sucht die Ebene im Unter-Baum, die gut geeignet ist,
// die Ebenen im Raum zu trennen
CBspPlane *CBspObject::SearchSplitPlane( CBspPlane *List )
{
  unsigned int SplitFaults, MaxSplitFaults = -1;
  CBspPlane *BestPlane, *CmpPlane;

  BestPlane = CmpPlane = List;
  while( CmpPlane ) {
    CHVec16 NormalVector = CmpPlane->GetNormalVector();
    SplitFaults = 0;
    CBspPlane *Plane = List;
    while( Plane ) {
      int i = Plane->LocatePoints( NormalVector );
      if( i == 0 || i == 1 || i == -1 )
        SplitFaults++;
      Plane = Plane->Before;
    }
    if( SplitFaults < MaxSplitFaults ) {
      MaxSplitFaults = SplitFaults;
      BestPlane = CmpPlane;
    }
    CmpPlane = CmpPlane->Before;
  }
  return BestPlane;
}

// erstellt aus der Polygonliste gemäß dem
// BinarySpacePartitioning-Algorithmus ( nach Henry Fuchs )
// einen einfach verketteten binären Baum 
void CBspObject::BuildBspSubTree( CBspPlane **List )
{
  if( *List == NULL )
    return;
  CBspPlane *RefPlane = SearchSplitPlane( *List );
  CHVec16 NormalVector = RefPlane->GetNormalVector();
  RefPlane->SetToTop( List );
  CBspPlane *Plane = RefPlane->Before;
  RefPlane->Before = NULL;
  RefPlane->Behind = NULL;
  while( Plane ) {
    CBspPlane *NextPlane = Plane->Before;
    if( Plane->LocatePoints( NormalVector ) < 0 ) {
      Plane->Before = RefPlane->Before;
      RefPlane->Before = Plane;
    } else {
      Plane->Before = RefPlane->Behind;
      RefPlane->Behind = Plane;
    }
    Plane = NextPlane;
  }
  BuildBspSubTree( &RefPlane->Before );
  BuildBspSubTree( &RefPlane->Behind );
}

void CBspObject::BuildBspTree()
{
  BuildBspSubTree( &BspRoot );
}

// gibt ein polygonorientiertes Objekt aus
void CBspObject::Display( const CMatrix16 &Matrix )
{
  _SetWindowDimension( &Window );
  _DisplayObject( this, &Matrix );
}

/////////////////////////////////////////////////////////////////////////////
// CObject

// Constructor
CObject::CObject( CBspObject *_BspObject, const CVec16 &Position )
{
  ObjectList.Append( this );
  BspObject = _BspObject;
  Matrix.Init();
  Matrix.h = Position;
  Airplane = this;
}

// Destructor
CObject::~CObject()
{
  ObjectList.Get( this );
}

// bewegt ein Objekt
void CObject::Move( const CVec16 &Vector )
{
  Matrix.h += Vector;
}

// rotiert ein Objekt um seine eigene Achse
void CObject::Rotate( const CVec16 &Vector )
{
  Matrix.RotateByAxe( Vector );
}

/////////////////////////////////////////////////////////////////////////////
// Ausgabe

// berechnet die Bildtransformationsmatrix
CMatrix16 CalcViewMatrix( const CVec16 &ViewPoint, const CVec16 &ViewVector )
{
  CMatrix16 Matrix;
  Matrix.Init();

  for( int i=0; i<3; i++ )
    Matrix.v[H].v[i] = -ViewPoint.v[i];
  FIX16 a, b, sx, cx, sy, cy;
  a = ViewVector.v[X] * ViewVector.v[X] + ViewVector.v[Z] * ViewVector.v[Z];
  b = sqrt( a + ViewVector.v[Y] * ViewVector.v[Y] );
  if( b > ROUNDOFF ) {
    a = sqrt( a );
    if( a > ROUNDOFF ) {
      sy = -ViewVector.v[X] / a;
      cy = -ViewVector.v[Z] / a;           // '-' wegen rechtshaendigem System
    } else {
      sy = 0.0;
      cy = 1.0;
    }
    sx = -ViewVector.v[Y] / b;
    cx = a / b;
    Matrix.Rotate( ROT_Y, sy, cy );
    Matrix.Rotate( ROT_X, sx, cx );
  }

  Matrix *= CVec16( -640.0/350.0, ScreenAspectRatio, 1 );
  return Matrix;
}

CMatrix30 CalcShadowMatrix()
{
  CMatrix30 Matrix = Airplane->Matrix;
  Matrix.h.v[Y] = 0.0;
  return Matrix;
} 

void DisplayBackground( const CRect &r, const CMatrix16 &ViewMatrix )
{
  short mid = ViewMatrix.v[Z].v[Z] > 0 ? ( ViewMatrix.v[Z].v[Y] *  1024.0 ).Int()
                                       : ( ViewMatrix.v[Z].v[Y] * -1024.0 ).Int();
  mid += r.top + ( r.bottom - r.top ) / 2;
  if( mid < r.top )
    mid = r.top;
  else if( mid > r.bottom )
    mid = r.bottom;
  _SetColor( BLUE );
  _FillRectC( r.left, r.top, r.right, mid );
  _SetColor( GREEN );
  _FillRectC( r.left, mid, r.right, r.bottom );
}

void Disp( const char *format, FIX16 f1=0.0, FIX16 f2=0.0, FIX16 f3=0.0 );

CVec16 ViewPoint, ViewVector;

BOOL IsVisible( CObject *Object )
{
  FIX16 Phi = dot( Object->Matrix.h, ViewVector ) / ( abs( Object->Matrix.h ) * abs( ViewVector ) );
  Disp( "Phi ", Phi );
  if( Phi > 0.90 )
    return TRUE;
  else
    return FALSE;
}

// zeigt komplette Scenerie an
void DisplayScene( const CRect &r )
{
  Window = r;
  ViewPoint.v[X] = 0;
  ViewPoint.v[Y] = ViewHeight;
  ViewPoint.v[Z] = 0;

  if( Airplane == 0 )
    ViewVector = CVec16 ( 0.0,0.0,-15.0 );
  else
    ViewVector = Airplane->Matrix.h - ViewPoint;
  CMatrix16 ViewMatrix = CalcViewMatrix( ViewPoint, ViewVector );
  DisplayBackground( Window, ViewMatrix );
  for( CIDIter<CObject> ObjectIter( CObject::ObjectList ); ObjectIter(); ObjectIter++ ) {
//    if( !IsVisible( ObjectIter() ) )
//      continue;
    if( ObjectIter()->BspObject->GetShadow() )
      ObjectIter()->BspObject->GetShadow()->Display( CalcShadowMatrix() * ViewMatrix );
    ObjectIter()->BspObject->Display( ObjectIter()->Matrix * ViewMatrix );
  }
}


