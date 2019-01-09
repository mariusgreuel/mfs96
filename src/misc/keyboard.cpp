//
// keyboard.cpp, keyboard io routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <conio.h>
#include "debug.hpp"
#include "keyboard.hpp"

#define BufferSize      32              // size for keyboard buffer

/////////////////////////////////////////////////////////////////////////////
// Class Keyboard

// Keyboard aktivieren
void CKeyboard::_Startup()
{
  Buffer = new KEY[ BufferSize ];
  FirstKey = LastKey = Buffer;
}

// Keyboard deaktivieren
void CKeyboard::_Shutdown()
{
  PurgeBuffer();
  delete [] Buffer;
}

// konvertiert den BIOS Tastencode in den virtuellen Tastencode
KEY CKeyboard::ConvertKey( KEY Key )
{
  return Key;
}

// Puffer löschen
void CKeyboard::PurgeBuffer()
{
  FirstKey = LastKey = Buffer;
  while( kbhit())
    getch();
}

// Tastatureingaben auswerten
void CKeyboard::Read()
{
  while( kbhit() ) {
    KEY Key = (KEY)getch();
    if( Key == 0 )
      Key = KEY_EXTENDED + (KEY)getch();
    PutVKey( ConvertKey( Key ));
  }
}

// True, wenn Taste im Puffer
BOOL CKeyboard::KeyPressed()
{
  Read();
  return !( FirstKey == LastKey );
}

// virtuellen Tastencode in Puffer stellen
// Funktionswert == 0, wenn Puffer voll
KEY CKeyboard::PutVKey( KEY Key )
{
  if( !IsInstalled() )
    return VK_NULL;
  KEY *p = LastKey+1;
  if( p == Buffer+BufferSize )
    p = Buffer;
  if( p == FirstKey )
    return VK_NULL;             // Puffer voll
  *LastKey = Key;
  LastKey = p;
  return Key;
}

// virtuellen Tastencode aus Puffer holen
// Funktionswert == Taste
KEY CKeyboard::GetVKey()
{
  if( !IsInstalled() )
    return VK_NULL;
  Read();
  if( !KeyPressed())
    return VK_NULL;
  KEY Key = *FirstKey;
  FirstKey++;
  if( FirstKey == Buffer+BufferSize )
    FirstKey = Buffer;
  return Key;
}

// Tastaturstatus holen
KEY CKeyboard::GetKeyboardState()
{
  return 0;
}

