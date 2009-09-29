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

#ifndef ARRAY_H
#define ARRAY_H

#include "helpers.h"

template<class T> class Array {
public:
	Array(size_t s) {
		allocate(s);
	}
	/*Array(size_t size, const T *data) {
	allocate(size);
	for(size_t i=0; i<size; i++) {
	a[i] = data[i];
	}
	}*/
	~Array() {
		delete[] a;
	}
	void resize(size_t s) {
		if(b == s)
			return;
		delete[] a;
		allocate(s);
	}

	T& operator[](size_t index) {
		DEBUG_ASSERT(index < b);
		return a[index];
	}
	const T& operator[](size_t index) const {
		DEBUG_ASSERT(index < b);
		return a[index];
	}
	T& operator[](int index) { return (*this)[(size_t)index]; }
	const T& operator[](int index) const { return (*this)[(size_t)index]; }

	operator const T*() const { return a; }
	operator T*() { return a; }

	const T* p() const { return a; }
	T* p() { return a; }
	size_t size() const { return b; }
	void steal(Array<T> &src) {
		resize(0); a = src.a; b = src.b;
		src.a = NULL; src.b = 0;
	}
	void swap(Array<T> &other) {
		T *ta = a; size_t tb = b;
		a = other.a; b = other.b;
		other.a = ta; other.b = tb;
	}
private:
	Array(const Array&);
	Array &operator=(const Array&);
	T *a;
	size_t b;

	void allocate(size_t s) {
		b = s;
		if(s == 0)
			a = NULL;
		else {
			a = new T[s];
			MYASSERT(a, MoSyncError::ERR_OOM);
		}
	}
};

#endif	//ARRAY_H
