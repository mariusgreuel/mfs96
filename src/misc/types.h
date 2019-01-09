//
// types.h, braucht man immer...
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _TYPES_H_INCLUDED
#define _TYPES_H_INCLUDED
#ifdef __cplusplus
  extern "C" {
#endif

// Streßfreies kopilieren
#ifdef __WATCOMC__
  #pragma warning 389 9   // suppress integral may trucated
#endif

// Boolsche Definitionen
#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE 1
#endif

#ifndef NULL
  #define NULL 0
#endif

// und ein paar neue Typen
typedef unsigned char  BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;

#define LOWORD( w ) ((WORD)((w) & 0xffff ))
#define HIGHWORD( w ) ((WORD)((w) >> 16 ))

#ifdef __cplusplus
};
#endif
#endif

