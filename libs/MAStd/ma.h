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

/** \file ma.h
* \brief Defines MoSync program entry point, size_t, NULL and BOOL
*/


#ifndef MA_H
#define MA_H

#include "maapi.h"

#ifndef SIZE_T_DEFINED
#ifdef _WIN32
typedef unsigned int size_t;
#elif defined(MAPIP)
typedef unsigned long size_t;
#endif
#define SIZE_T_DEFINED
#endif

#ifdef MAPIP
#define BOOL	int
#else //NOT MAPIP
typedef int BOOL;
#endif //MAPIP

#define TRUE	1
#define FALSE	0

#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
extern "C"
#endif
int MAMain(void);

#ifdef MAPIP
#define _ma_inline(string) __asm__(string "\n")	
#else
#define _ma_inline(string)	
#endif

void ATTRIBUTE(noreturn, ErrorExit(const char * str, int v));

#endif /* MA_H */
