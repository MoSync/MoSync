/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
