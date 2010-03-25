/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef MACTYPE_H
#define MACTYPE_H

#ifndef _WIN32

#ifdef __cplusplus
extern "C" {
#endif

//
// Character types
//

#define _UC		0x1	 // Upper case letter
#define _LC		0x2	 // Lower case letter
#define _NUM	0x4	 // Digit[0-9]
#define _WHITE	0x8	 // Tab, carriage return, newline, vertical tab or form feed
#define _PUNCT	0x10	// Punctuation character
#define _CTRL	0x20	// Control character
#define _SPACE	0x40	// Space char
#define _HEX	0x80	// Hexadecimal digit
#define _ALFA	(_UC | _LC)	 	// Alphabetic character
#define _ALNUM	(_ALFA | _NUM)
#define	_PRINT	(_SPACE | _PUNCT | _ALNUM)
#define _GRAP	(_PUNCT | _ALNUM)

unsigned char _ctypetab[128] = 
{
	_CTRL,				// 00 (NUL)
	_CTRL,				// 01 (SOH)
	_CTRL,				// 02 (STX)
	_CTRL,				// 03 (ETX)
	_CTRL,				// 04 (EOT)
	_CTRL,				// 05 (ENQ)
	_CTRL,				// 06 (ACK)
	_CTRL,				// 07 (BEL)
	_CTRL,				// 08 (BS)
	_WHITE+_CTRL,		// 09 (HT)
	_WHITE+_CTRL,		// 0A (LF)
	_WHITE+_CTRL,		// 0B (VT)
	_WHITE+_CTRL,		// 0C (FF)
	_WHITE+_CTRL,		// 0D (CR)
	_CTRL,				// 0E (SI)
	_CTRL,				// 0F (SO)
	_CTRL,				// 10 (DLE)
	_CTRL,				// 11 (DC1)
	_CTRL,				// 12 (DC2)
	_CTRL,				// 13 (DC3)
	_CTRL,				// 14 (DC4)
	_CTRL,				// 15 (NAK)
	_CTRL,				// 16 (SYN)
	_CTRL,				// 17 (ETB)
	_CTRL,				// 18 (CAN)
	_CTRL,				// 19 (EM)
	_CTRL,				// 1A (SUB)
	_CTRL,				// 1B (ESC)
	_CTRL,				// 1C (FS)
	_CTRL,				// 1D (GS)
	_CTRL,				// 1E (RS)
	_CTRL,				// 1F (US)
	_WHITE+_SPACE,		// 20 SPACE
	_PUNCT,				// 21 !
	_PUNCT,				// 22 "
	_PUNCT,				// 23 #
	_PUNCT,				// 24 $
	_PUNCT,				// 25 %
	_PUNCT,				// 26 &
	_PUNCT,				// 27 '
	_PUNCT,				// 28 (
	_PUNCT,				// 29 )
	_PUNCT,				// 2A *
	_PUNCT,				// 2B +
	_PUNCT,				// 2C ,
	_PUNCT,				// 2D -
	_PUNCT,				// 2E .
	_PUNCT,				// 2F /
	_NUM+_HEX,			// 30 0
	_NUM+_HEX,			// 31 1
	_NUM+_HEX,			// 32 2
	_NUM+_HEX,			// 33 3
	_NUM+_HEX,			// 34 4
	_NUM+_HEX,			// 35 5
	_NUM+_HEX,			// 36 6
	_NUM+_HEX,			// 37 7
	_NUM+_HEX,			// 38 8
	_NUM+_HEX,			// 39 9
	_PUNCT,				// 3A :
	_PUNCT,				// 3B ;
	_PUNCT,				// 3C <
	_PUNCT,				// 3D =
	_PUNCT,				// 3E >
	_PUNCT,				// 3F ?
	_PUNCT,				// 40 @
	_UC+_HEX,			// 41 A
	_UC+_HEX,			// 42 B
	_UC+_HEX,			// 43 C
	_UC+_HEX,			// 44 D
	_UC+_HEX,			// 45 E
	_UC+_HEX,			// 46 F
	_UC,				// 47 G
	_UC,				// 48 H
	_UC,				// 49 I
	_UC,				// 4A J
	_UC,				// 4B K
	_UC,				// 4C L
	_UC,				// 4D M
	_UC,				// 4E N
	_UC,				// 4F O
	_UC,				// 50 P
	_UC,				// 51 Q
	_UC,				// 52 R
	_UC,				// 53 S
	_UC,				// 54 T
	_UC,				// 55 U
	_UC,				// 56 V
	_UC,				// 57 W
	_UC,				// 58 X
	_UC,				// 59 Y
	_UC,				// 5A Z
	_PUNCT,				// 5B [
	_PUNCT,				/* 5C \ */
	_PUNCT,				// 5D ]
	_PUNCT,				// 5E ^
	_PUNCT,				// 5F _
	_PUNCT,				// 60 `
	_LC+_HEX,			// 61 a
	_LC+_HEX,			// 62 b
	_LC+_HEX,			// 63 c
	_LC+_HEX,			// 64 d
	_LC+_HEX,			// 65 e
	_LC+_HEX,			// 66 f
	_LC,				// 67 g
	_LC,				// 68 h
	_LC,				// 69 i
	_LC,				// 6A j
	_LC,				// 6B k
	_LC,				// 6C l
	_LC,				// 6D m
	_LC,				// 6E n
	_LC,				// 6F o
	_LC,				// 70 p
	_LC,				// 71 q
	_LC,				// 72 r
	_LC,				// 73 s
	_LC,				// 74 t
	_LC,				// 75 u
	_LC,				// 76 v
	_LC,				// 77 w
	_LC,				// 78 x
	_LC,				// 79 y
	_LC,				// 7A z
	_PUNCT,				// 7B {
	_PUNCT,				// 7C |
	_PUNCT,				// 7D }
	_PUNCT,				// 7E ~
	_CTRL				// 7F (DEL)
};

#define CTYPEIDX(c)	(c < 128 ? _ctypetab[c] : 0)

#define DEF_CTYPE(fname, retval) int fname(int c); int fname(int c){return (retval);}

DEF_CTYPE(isalpha, CTYPEIDX(c) & _ALFA)
DEF_CTYPE(isupper, CTYPEIDX(c) & _UC)
DEF_CTYPE(islower, CTYPEIDX(c) & _LC)
DEF_CTYPE(isdigit, CTYPEIDX(c) & _NUM)
DEF_CTYPE(isxdigit,CTYPEIDX(c) & _HEX)
DEF_CTYPE(isspace, CTYPEIDX(c) & _WHITE)
DEF_CTYPE(ispunct, CTYPEIDX(c) & _PUNCT)
DEF_CTYPE(isalnum, CTYPEIDX(c) & _ALNUM)
DEF_CTYPE(isprint, CTYPEIDX(c) & _PRINT)
DEF_CTYPE(isgraph, CTYPEIDX(c) & _GRAP)
DEF_CTYPE(iscntrl, CTYPEIDX(c) & _CTRL)
DEF_CTYPE(toupper, islower(c) ? (c - 32) : c)
DEF_CTYPE(tolower, isupper(c) ? (c + 32) : c)

DEF_CTYPE(isascii, c >= 0 && c <= 127)

#ifdef __cplusplus
}	//extern "C"
#endif

#else	//_WIN32
#include <ctype.h>
#endif

#endif //MACTYPE_H
