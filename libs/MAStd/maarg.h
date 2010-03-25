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

/** \file maarg.h
* \brief Support for variadric functions.

* VAARGS
* Functions in C take a variable amount of arguments, functions
* such as printf and sprintf, these function use VAARGS which allow
* a function to read the arguments passed from an ellipsis parameter
* (...).
* Note all va functions are implemented as macroes.
*/

#ifdef MAPIP

#ifndef MAARG_H
#define MAARG_H

typedef __builtin_va_list	__gnuc_va_list;
typedef __gnuc_va_list		va_list;

/**
* va_start
* Sets the va parameter to the first parameter
*/

#define va_start(v,l)		__builtin_stdarg_start((v),l)

/**
* va_end
* A house keeping function that must be used when your finished with
* va args.
*/

#define va_end				__builtin_va_end

/**
* va_arg
* Accesses a functions variable argument list
*/

#define va_arg				__builtin_va_arg

#define va_copy(d,s)	__builtin_va_copy((d),(s))

#endif //MAARG_H
#else
#include <stdarg.h>			/* WIN32*/
#endif
