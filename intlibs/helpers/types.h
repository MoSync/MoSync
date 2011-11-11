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

#ifndef _INTLIBS_TYPES_H_
#define _INTLIBS_TYPES_H_

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

#ifdef _MSC_VER
# define PFZT_PREFIX "I"
typedef long int ssize_t;
#elif defined(SYMBIAN)
# define PFZT_PREFIX ""
#elif defined(__GNUC__)
# if (__GNUC__ == 4) && defined(WIN32)
#  define PFZT_PREFIX "I"
# else
#  define PFZT_PREFIX "z"
# endif	//WIN32
#else
# error Unsupported compiler!
#endif

#define PFZT PFZT_PREFIX "u"
#define PFSZT PFZT_PREFIX "i"
#define PFXZT PFZT_PREFIX "x"

#ifdef SYMBIAN
# define PFP "08x"
#else
# define PFP "p"
#endif

#endif // _INTLIBS_TYPES_H_
