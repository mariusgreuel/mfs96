//
// keys.h, keyboard codes
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _KEYS_H_INCLUDED
#define _KEYS_H_INCLUDED

#ifdef __cplusplus
  extern "C" {
#endif

// Tastencode 16-Bit Integer
// Bit 0-7   => normaler ASCII-Code (Bit 8-14 = 0) sonst KeyCode
// Bit 8     => erweiterte Taste (z.B. Cursor-Left)
// Bit 9     => virtuelle Taste (z.B. Menutaste, Maustaste)
// Bit 10    => Kontrolltaste (z.B. Shift, Numlock)
// Bit 11-12 => unbenutzt
// Bit 13    => Taste wurde gedrückt
// Bit 14    => Taste wurde losgelassen
// Bit 15    => Taste wurde wiederholt

#define KEY_NORMAL      0x0
#define KEY_EXTENDED    0x100
#define KEY_VIRTUAL     0x200
#define KEY_CONTROL     0x400
#define KEY_PRESSED     0x1000
#define KEY_RELEASED    0x2000
#define KEY_REPEATED    0x4000

// normale Tasten
#define VK_NULL         ( KEY_NORMAL + 0    )
#define VK_BACKSPACE    ( KEY_NORMAL + 0x08 )
#define VK_TABULATOR    ( KEY_NORMAL + 0x09 )
#define VK_CLEAR        ( KEY_NORMAL + 0x0c )
#define VK_RETURN       ( KEY_NORMAL + 0x0d )
#define VK_ESCAPE       ( KEY_NORMAL + 0x1b )
#define VK_SPACE        ( KEY_NORMAL + 0x20 )

// erweiterte Taste
#define VK_LEFT         ( KEY_EXTENDED + 0x4b )
#define VK_RIGHT        ( KEY_EXTENDED + 0x4d )
#define VK_UP           ( KEY_EXTENDED + 0x48 )
#define VK_DOWN         ( KEY_EXTENDED + 0x50 )
#define VK_HOME         ( KEY_EXTENDED + 0x47 )
#define VK_END          ( KEY_EXTENDED + 0x4f )
#define VK_PAGEUP       ( KEY_EXTENDED + 0x49 )
#define VK_PAGEDOWN     ( KEY_EXTENDED + 0x51 )
#define VK_INSERT       ( KEY_EXTENDED + 0x52 )
#define VK_DELETE       ( KEY_EXTENDED + 0x53 )
#define VK_F1           ( KEY_EXTENDED + 0x3b )
#define VK_F2           ( KEY_EXTENDED + 0x3c )
#define VK_F3           ( KEY_EXTENDED + 0x3e )
#define VK_F4           ( KEY_EXTENDED + 0x3f )
#define VK_F5           ( KEY_EXTENDED + 0x40 )
#define VK_F6           ( KEY_EXTENDED + 0x41 )
#define VK_F7           ( KEY_EXTENDED + 0x42 )
#define VK_F8           ( KEY_EXTENDED + 0x43 )
#define VK_F9           ( KEY_EXTENDED + 0x44 )
#define VK_F10          ( KEY_EXTENDED + 0x45 )

// virtuelle Tastencodes
#define VK_LBUTTON      ( KEY_VIRTUAL + 0x01 )
#define VK_RBUTTON      ( KEY_VIRTUAL + 0x02 )
#define VK_MBUTTON      ( KEY_VIRTUAL + 0x03 )
#define VK_OK           ( KEY_VIRTUAL + 0x10 )
#define VK_CANCEL       ( KEY_VIRTUAL + 0x11 )
#define VK_YES          ( KEY_VIRTUAL + 0x12 )
#define VK_NO           ( KEY_VIRTUAL + 0x13 )
#define VK_MENU         ( KEY_VIRTUAL + 0x20 )
#define VK_PRINT        ( KEY_VIRTUAL + 0x70 )
#define VK_ROLL         ( KEY_VIRTUAL + 0x71 )
#define VK_PAUSE        ( KEY_VIRTUAL + 0x72 )
#define VK_BREAK        ( KEY_VIRTUAL + 0x80 )
#define VK_SYSTEM       ( KEY_VIRTUAL + 0x81 )

// Kontrolltaste
#define VK_CSHIFT       ( KEY_CONTROL + 0x01 )
#define VK_CLSHIFT      ( KEY_CONTROL + 0x02 )
#define VK_CRSHIFT      ( KEY_CONTROL + 0x03 )
#define VK_CCONTROL     ( KEY_CONTROL + 0x04 )
#define VK_CALT         ( KEY_CONTROL + 0x05 )
#define VK_CSCROLL      ( KEY_CONTROL + 0x06 )
#define VK_CNUM         ( KEY_CONTROL + 0x07 )
#define VK_CINSERT      ( KEY_CONTROL + 0x08 )

// Tastatur-Status
#define VKS_SHIFT       0x3
#define VKS_RSHIFT      0x1
#define VKS_LSHIFT      0x2
#define VKS_CTRL        0x4
#define VKS_ALT         0x8
#define VKS_SCROLL      0x10
#define VKS_NUM         0x20
#define VKS_CAPS        0x40
#define VKS_INSERT      0x80

#ifdef __cplusplus
};
#endif

#endif

