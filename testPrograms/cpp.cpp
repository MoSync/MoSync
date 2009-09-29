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

int i1=-42, i2=2;	//crash test dummy

//This is a test of the C++ compiler
//Testing now:
//Classes, inheritance, (pure) virtual functions, new([]), delete([]), templates, global/static objects...
//Features that will not be implemented now:
//Exceptions, run-time type information (RTTI).

#include <ma.h>
#include <maheap.h>
#include <conprint.h>

#define FREEZE for(;;) { maWait(WAIT_KEY); }

#define MYASSERT(a) if(!(a)) { maExit(__LINE__); }

class A {
public:
	A();
	virtual int func();
	virtual ~A();
protected:
	int i;
	char c;
};

class B : public A {
public:
	B(const char*);
	~B();
	int func();
};

B gb("hello");	//works now.

template<class T> class Array {
public:
	Array(size_t size) {
		allocate(size);
	}
	~Array() {
		delete[] a;
	}
	void resize(size_t size) {
		if(b == size)
			return;
		delete[] a;
		allocate(size);
	}

	T& operator[](size_t index) {
		MYASSERT(index < b);
		return a[index];
	}
	const T& operator[](size_t index) const {
		MYASSERT(index < b);
		return a[index];
	}

	//operator const T*() const { return a; }
	//operator T*() { return a; }

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

	void allocate(size_t size) {
		b = size;
		if(size == 0)
			a = NULL;
		else {
			a = new T[size];
			MYASSERT(a);
		}
	}
};

void test2() {
	A* pa = new A;
	printf("Hello World\n");
	printf("pa(this): 0x%p\n", pa);
	printf("&pa: 0x%p\n", &pa);
	printf("%i\n", pa->func());
}

void test1() {
test2();
}

extern "C" {
int MAMain() {
	EVENT event;
	/*{
		//B b;
		B* bp = new B;
		A& a = *bp;
		printf("%i\n", a.func());
		printf("%i\n", a.func());
		printf("%i\n", a.func());
		printf("%i\n", a.func());
		delete bp;
		printf("DELETED!!\n");
	}*/
	/*{
		A& a = gb;
		printf("%i\n", a.func());
		printf("%i\n", a.func());
		printf("%i\n", a.func());
		printf("%i\n", a.func());
	}*/
	/*{
		Array<A> arra(2);
		printf("%i\n", arra[1].func());
		printf("%i\n", arra[0].func());
		printf("%i\n", arra[0].func());
		printf("%i\n", arra[0].func());
	}*/
	test1();
	printf("Done\n");

	while(maGetEvent(&event) != MAK_DOWN) {
		maWait(0);
	}
	return 42;
}
}

A::A() : i(1), c(1) {}

A::~A() {
	printf("Goodbye World!\n");
}

B::~B() {
	printf("Goodbye B!\n");
}

int A::func() {
	i++;
	c *= 2;
	i ^= c;
	return i;
}

B::B(const char*) {
	i++;
}

int B::func() {
	i += 3;
	c *= 2;
	i ^= c;
	return i;
}
