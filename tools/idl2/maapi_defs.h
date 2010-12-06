/* Copyright (C) 2010 MoSync AB

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

#ifndef _MAAPI_DEFS_H
#define _MAAPI_DEFS_H

#ifdef MAPIP
#define MA_IOCTL_ELLIPSIS , ...
#else
#define MA_IOCTL_ELLIPSIS
#endif

#if defined(__GNUC__) || defined(__SYMBIAN32__)
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#define ATTRIB(a) __attribute__ ((a))
#define GCCATTRIB(a) __attribute__ ((a))
#define IOCTLDEF static inline
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#define GCCATTRIB(a)
#define IOCTLDEF static
#else
#error Unsupported compiler!
#endif

#if defined(__MARM_ARMI__)	//Symbian S60, 2nd edition hardware
#define MA_DV_HI i[1]
#define MA_DV_LO i[0]
#define MA_LL_HI i[0]
#define MA_LL_LO i[1]
#else
#define MA_DV_HI hi
#define MA_DV_LO lo
#define MA_LL_HI hi
#define MA_LL_LO lo
#endif

// Convert between double, 2 ints and long long.
union MA_DV {
#if defined(__MARM_ARMI__)	//Symbian S60, 2nd edition hardware
	int i[2];
#else	//everything else
	struct {
		int hi, lo;
	};
#endif	//__MARM_ARMI__
	long long ll;
	double d;
};

// Convert between float and int.
union MA_FV {
	float f;
	int i;
};

#endif	//_MAAPI_DEFS_H
