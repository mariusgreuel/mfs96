//
// ega16.h
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _EGA16_H_INCLUDED
#define _EGA16_H_INCLUDED
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef __COLORS
#define __COLORS
enum COLORS {
    _BLACK,                     // dark colors
    _BLUE,
    _GREEN,
    _CYAN,
    _RED,
    _MAGENTA,
    _BROWN,
    _LIGHTGRAY,
    _DARKGRAY,                  // light colors
    _LIGHTBLUE,
    _LIGHTGREEN,
    _LIGHTCYAN,
    _LIGHTRED,
    _LIGHTMAGENTA,
    _YELLOW,
    _WHITE
};
#endif

// Low-level assembler module
void _SetTextMode( void  );
void _SetGraphicMode( void  );
void _SaveScreen( void );
void _RestoreScreen( void );
char _DetectGraph( void );
void _WaitForRetrace( void  );
char _GetActivePage( void );
char _SetActivePage( char Page );
char _GetVisiblePage( void );
char _SetVisiblePage( char Page );
void _SetColor( short Color );
void _SetColorReg( char Register, long RBG );
void _ClearScreen( void  );
void _SetClipRegion( short x1, short y1, short x2, short y2 );
void _DrawLine( short x1, short y1, short x2, short y2 );
void _DrawLineH( short x1, short y1, short x2 );
void _DrawLineV( short x1, short y1, short y2 );
void _DrawLineHC( short x1, short y1, short x2 );
void _DrawLineVC( short x1, short y1, short y2 );
void _DrawRect( short x1, short y1, short x2, short y2 );
void _FillRect( short x1, short y1, short x2, short y2 );
void _DrawRectC( short x1, short y1, short x2, short y2 );
void _FillRectC( short x1, short y1, short x2, short y2 );
void _DrawPolygon( short Count, short *PolygonList );
void _FillPolygon( short Count, short *PolygonList );
void _FillPolygonT( short Count, short *PolygonList );
void _DrawChar( char Char, short x, short y );
void _DrawCharC( char Char, short x, short y );
void _GetImage( short x1, short y1, short x2, short y2, char *buf );
void _PutImage( short x1, short y1, char *buf, short mode );
unsigned int _ImageSize( short Width, short Height );

#ifdef __cplusplus
};
#endif
#endif // _EGA16_H_INCLUDED

