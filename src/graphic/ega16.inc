;//
;// ega16.inc
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

VERSION                 = 0100h
MISC                    = 03c2h         ;Misc. Output
SEQ                     = 03c4h         ;Seqencer Index
GDC                     = 03ceh         ;Graphics Controller Index
CRTC                    = 03d4h         ;CRT Controller Index
STATUS6845              = 03dah
TEXT_MODE               = 03h
GRAPHIC_MODE            = 010h
X_RES                   = 640
Y_RES                   = 350
FONT_X_RES              = 8
FONT_Y_RES              = 8
PIXEL_PER_BYTE          = 8
BYTES_PER_LINE          = X_RES/PIXEL_PER_BYTE
GRAPHIC_BUFFER_SIZE     = BYTES_PER_LINE*Y_RES
TEXT_BUFFER_SIZE        = 80*25*2
TEXT_SEGMENT            = 0b8000h
GRAPHIC_SEGMENT         = 0a0000h

;State Flag
TEXT_MODE_ON            = 1
GRAPHIC_MODE_ON         = 2
VIDEO_MODE_SAVED        = 4
SCREEN_SAVED            = 8
ND_PAGE_ACTIVE          = 10h
ND_PAGE_VISIBLE         = 20h

POINT   struc
X       dw ?
Y       dw ?
POINT   ends

LRECT   struc
LEFT    dd ?
TOP     dd ?
RIGHT   dd ?
BOTTOM  dd ?
LRECT   ends


