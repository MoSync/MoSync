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

/** \file mactype.h
* \brief Character classification.
*/

#ifndef MACTYPE_H
#define MACTYPE_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MAPIP

int _isctype(int c, int mask);

int isspace(int c);
int isupper(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int ispunct(int c);
int isalpha(int c);
int isalnum(int c);
int isprint(int c);
int isgraph(int c);
int iscntrl(int c);
int isleadbyte(int c);

int toupper(int c);
int tolower(int c);

int isascii(int c);

#else
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif	//_CRT_SECURE_NO_WARNINGS
#include <ctype.h>

#endif	//MAPIP

#ifdef  __cplusplus
}
#endif

#endif	//MACTYPE_H
