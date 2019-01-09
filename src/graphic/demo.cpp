//
// demo.cpp, Demo für EGA16
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <iostream.h>
#include <stdlib.h>
#include <conio.h>
#include "graphic.hpp"
#include "ega16.h"

#pragma warning 389 9

CGraphic Graphic;

struct { short x, y; }
  Buffer1[10],
  Buffer2[] = { 50,150, 100,50, 200,200 },
  Buffer3[] = { 100,100, 300,103, 300,300, 100,300 };

int random( int range )
{
  return rand() % range;
}

void _DrawTextXY( char *Text, short x, short y )
{
  while( *Text && *Text != '\n' ) {
    _DrawChar( *Text, x, y );
    Text++;
    x += FontCharWidth;
  }
}

void DrawHVLineDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    for( int i=0; i<100; i++ ) {
      _DrawLineH( 200-i, 100+i, 200+i );
      _DrawLineV( 400+i, 200-i, 200+i );
    }
  }
  getch();
}

void DrawLineDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    _DrawLine( 100, 100, random( 640 ), random( 350 ));
  }
  getch();
}

void DrawRectDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    _DrawRect( random( 640 ), random( 350 ),random( 640 ), random( 350 ));
  }
  getch();
}

void FillRectDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    _FillRect( random( 640 ), random( 350 ),random( 640 ), random( 350 ));
  }
  getch();
}

void DrawPolygonDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    for( int i=0; i<3; i++ ) {
      Buffer1[i].x = random( 640 );
      Buffer1[i].y = random( 350 );
    }
    _DrawPolygon( 3, (short*)Buffer1 );
  }
  getch();
}

void FillPolygonDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    for( int i=0; i<3; i++ ) {
      Buffer1[i].x = random( 640 );
      Buffer1[i].y = random( 350 );
    }
    _FillPolygon( 3, (short*)Buffer1 );
  }
  getch();
}

void DrawCharDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    _DrawChar( random( 128-32 ) + 32 , random( 640-8 ), random( 350-8 ));
  }
  getch();
}

void DrawTextDemo( void )
{
  _SetColor( _BLACK );
  _ClearScreen();
  while( !kbhit()) {
    _SetColor( random( 16 ));
    _DrawTextXY( "Grafik-Demo", random( 640 ), random( 350 ));
  }
  getch();
}

void ImageDemo( void )
{
  char *buf = new char[ _ImageSize( 100,100 ) ];
  _SetColor( _BLACK );
  _ClearScreen();
  for( int i=0; i<100; i++ ) {
    _SetColor(random(16));
    _DrawLine( 100,100, random(400),random(350));
  }
  while( !kbhit()) {
    int i, x, y;
    i = random( 70 )+30;
    x = random( 640-i );
    y = random( 350-i );
    _GetImage( x,y, x+i, y+i, buf );
    x = random( 640-i );
    y = random( 350-i );
    _PutImage( x,y, buf, 3 );
  }
  getch();
  delete[] buf;
}

void MoveDemo( void )
{
  int p = 0, x=0, y=0, xi = 2, yi = 2, d=50;
  char Color = 0;

  _SetActivePage( 0 );
  _SetColor( _BLACK );
  _ClearScreen();
  _SetColor( _WHITE );
  _DrawTextXY( "Grafik-Demo", 280,30 );
  _SetActivePage( 1 );
  _SetColor( _BLACK );
  _ClearScreen();
  _SetColor( _WHITE );
  _DrawTextXY( "Grafik-Demo", 280,30 );
  while( !kbhit()) {
    Color += 1;
    _SetActivePage( p );
    _SetVisiblePage( p^=1 );
    _SetColorReg( _LIGHTRED, (long)Color );
    _WaitForRetrace();
    _SetColor( _DARKGRAY );
    _FillRect( d, d, 640-d, 350-d );
    _SetColor( _LIGHTRED );
    _FillPolygon( 3, (short*)Buffer2 );
    _SetColor( _WHITE );
    _DrawTextXY( "Grafik-Demo", x+100, y+100 );
    x += xi;
    y += yi;
    for( int i=0; i<3; i++ ) {
      Buffer2[i].x += xi;
      Buffer2[i].y += yi;
    }
    if( x <= 0 || x >= 390-xi )
      xi *= -1;
    if( y <= 0 || y >= 100-yi )
      yi *= -1;
  }
}

int main( void )
{
  cout << "Test des Grafikmodus Ega 640*350, 16 Farben\n";
  Graphic.Detect( 1 );
  if( Graphic.IsInstalled() == FALSE ) {
    cout << "VGA card not found!\n";
    return 2;
  }
  Graphic.Startup();
  Graphic.SetGraphMode();

  DrawHVLineDemo();
  DrawLineDemo();
  DrawRectDemo();
  FillRectDemo();
  DrawPolygonDemo();
  FillPolygonDemo();
  DrawCharDemo();
  DrawTextDemo();
  ImageDemo();
  MoveDemo();

  Graphic.SetTextMode();
  return 0;
}

