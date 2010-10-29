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

#include <helpers/maapi_defs.h>

template <typename T>
inline void convertRet(int& r14, int& r15, T type) {
	r14 = (int)type;
}

template <>
inline void convertRet<double>(int& r14, int& r15, double type) {
	MA_DV dv;
	dv.d = type;
	r14 = dv.hi;
	r15 = dv.lo;
}

template <>
inline void convertRet<float>(int& r14, int& r15, float type) {
	MA_FV fv;
	fv.f = type;
	r14 = fv.i;
}

template <>
inline void convertRet<void*>(int& r14, int& r15, void* type) {
	r14 = (int)((byte*)type - (int)mem_ds);
}

template <typename T>
inline T convertSingleArg(int arg) {
	return (T)arg;
}

template <>
inline float convertSingleArg<float>(int arg) {
	MA_FV fv;
	fv.i = arg;
	return fv.f;
}

inline double convertDoubleArg(int arg1, int arg2) {
	MA_DV dv;
	dv.hi = arg1;
	dv.lo = arg2;
	return dv.d;
}


template <typename T>
inline T convertPointerArg(int arg) {
	// validate
	return (T) ((byte*)mem_ds+arg);
}
