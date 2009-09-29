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

#ifndef _MEMFUNCS_H_
#define _MEMFUNCS_H_

#define ZEROMEM(mem, size) memset(mem, 0, size)
#define MEMCPY(dst, src, size) memcpy(dst, src, size)
#define MEMSET(mem, val, size) memset(mem, val, size)
#define MEMCMP(a, b, size) memcmp(a, b, size)

#endif // _MEMFUNCS_H_
