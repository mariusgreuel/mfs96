//
// sbfmreg.hpp, program adlib/soundblaster register
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _SBFMREG_HPP_INCLUDED
#define _SBFMREG_HPP_INCLUDED

BYTE FM_ReadReg( BYTE Register );
void FM_WriteReg( BYTE Register, BYTE Value );

// This function reads the FM chip register
#pragma aux FM_ReadReg = \
        "mov dx,0388h"  \
        "out dx,al"     \
        "mov cl,7"      \
"L1:    in al,dx"       \
        "dec cl"        \
        "jnz L1"        \
        "inc dl"        \
        "in al,dx"      \
        "mov ah,al"     \
        "out dx,al"     \
        "mov cl,35"     \
"L2:    in al,dx"       \
        "dec cl"        \
        "jnz L2"        \
        parm [al]       \
        value [ah]      \
        modify [cl dx];

// This function writes the FM chip register
#pragma aux FM_WriteReg = \
        "mov dx,0388h"  \
        "out dx,al"     \
        "mov cl,7"      \
"L1:    in al,dx"       \
        "dec cl"        \
        "jnz L1"        \
        "inc dl"        \
        "mov al,ah"     \
        "out dx,al"     \
        "mov cl,35"     \
"L2:    in al,dx"       \
        "dec cl"        \
        "jnz L2"        \
        parm [al][ah]   \
        modify [cl dx];

#endif // _SBFMREG_HPP_INCLUDED
