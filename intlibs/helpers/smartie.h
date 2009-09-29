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

#ifndef SMARTIE_H
#define SMARTIE_H

#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif

template<class T> class SmartieBase {
public:
	bool operator==(T* other) const { return ptr == other; }
	bool operator!=(T* other) const { return ptr != other; }
	
	T* operator->() { return ptr; }
	const T* operator->() const { return ptr; }
	
	T& operator*() { return *ptr; }
	const T& operator*() const { return *ptr; }
	
	T& operator[](size_t i) { return ptr[i]; }
	const T& operator[](size_t i) const { return ptr[i]; }

	T* operator()() { return ptr; }
	const T* operator()() const { return ptr; }

	//operator T*() { return ptr; } //unsafe if coupled with constructor
	T* extract() { T* temp = ptr; ptr = NULL; return temp; }
	
	bool operator!() { return ptr == NULL; }
protected:
	SmartieBase(T* p) : ptr(p) {}
	T* ptr;
};

//A semi-smart pointer designed to delete the object being pointed to
//when the pointer goes out of scope.
template<class T> class Smartie : public SmartieBase<T> {
public:
	~Smartie() {
		if(this->ptr) {
			//LOGD("~Smartie 0x%08X\n", this->ptr);
			delete this->ptr;
		}
	}
	Smartie() : SmartieBase<T>(NULL) {}
	Smartie(T* other) : SmartieBase<T>(other) {}
	Smartie(Smartie& other) : SmartieBase<T>(other.ptr) { other.ptr = NULL; } //takeover
	Smartie& operator=(T* other) {
		if(this->ptr)
			delete this->ptr;
		this->ptr = other;
		return *this;
	}
};

#endif	//SMARTIE_H
