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

#include <MAUtil/String.h>
#include <conprint.h>
#include <maheap.h>

using namespace MAUtil;

struct A {
int a;
String b;
};

struct S {
	int a;
	String n;
	Vector<A> b;
};

Vector<S> s;
//String s;

extern "C" int MAMain() {
	InitConsole();
	printf("reading stroe...");
	//printf("%s", s[0].b[0].b.c_str());
	s.resize(7);
	maExit(999);
	return 0;
}
