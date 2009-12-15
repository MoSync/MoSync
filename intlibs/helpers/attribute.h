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

#if defined(__GNUC__) || defined(__SYMBIAN32__)
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#define ATTRIB(a) __attribute__ ((a))
#define GCCATTRIB(a) __attribute__ ((a))
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#define GCCATTRIB(a)
#else
#error Unsupported compiler!
#endif

#define PRINTF_ATTRIB(a,b) GCCATTRIB(format(printf, a, b))
