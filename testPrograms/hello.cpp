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

#include <ma.h>
#include <maassert.h>

int array[128];

struct Base {
	int a;
	virtual void run() {
	}
};

struct Inherited : public Base {
	float b[2];

	void run() {
	}
};

Inherited i;
Inherited *ii = &i;

double add(double a, int b) {
	return a + b;
}

int MAMain() {
	maSetColor(0xFFFFFF);
	maDrawText(0,0, "Hello World!");
	maUpdateScreen();
	
	i.a = 2;
	i.b[0] = 2.341;
	i.b[1] = 3.14159;

	i.run();
	add(1, 2);

	//Freeze
	Freeze(0);
	return 0;
}
