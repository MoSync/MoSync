/* Copyright (C) 2010 Mobile Sorcery AB

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

// range 'expression to calculate the range'
#define _range_(x) __attribute((gccxml("range", #x)))

// defaults to handle without range
// generate 'true/false to say if a pointer wrapper should be generated' 'true/false to say if a handle wrapper should be generated' 'the range to use to validate pointers'
// must add range attribute later so that you can validate the memory. maybe add native_pointer that returns a native pointer directly
// don't support permutations of these either yet. If you have an argument of handle type it will generate a functions where all pointer arguments are handles and vice versa.
#define _use_type_(pointer, handle) __attribute((gccxml("use_pointer", #pointer), gccxml("use_handle", #handle)))
