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

#include <conprint.h>
#include <maassert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

#define DUMPD(a) printf("%f", a)
void DUMPDH(double d) {
	int* p = (int*)&d;
	int sign = 0;
	printf("dcmp(d, 0) = %i\n", dcmp(d, 0));
	if(d < 0) {
		d = -d;
		sign = 1;
	}
	printf("%s%f %08X %08X\n", sign ? "-" : "", d, p[1], p[0]);
}

int MAMain() {
	double SDA = sin(0.0981746875);	//(0x3fb921f9f01b866e) sin error, returning 1
	//) returned 0x000000003ff00000(5.29980882362664E-315)

	InitConsole();
	printf("Floating test");

	DUMPDH(179.288537549407);
	DUMPDH(SDA);

	//return 0;

	Extent scrSize = maGetScrSize();
	scrSize = EXTENT(176, 208);
	double d;

	d = __adddf3(__floatsidf(EXTENT_X(scrSize)), __muldf3(__floatsidf(EXTENT_Y(scrSize)), 63.25));


	d /= 0.0;	//division by zero.

	DUMPD(__floatsidf(EXTENT_X(scrSize)));
	DUMPD(__floatsidf(EXTENT_Y(scrSize)));
	DUMPD(63.25);
	DUMPD(__muldf3(__floatsidf(EXTENT_Y(scrSize)), 63.25));
	d = __adddf3(__floatsidf(EXTENT_X(scrSize)), __divdf3(__floatsidf(EXTENT_Y(scrSize)), 63.25));
	printf("%f %i", d, __fixdfsi(d));
	printf("%f %i", __floatsidf(42), __fixdfsi(__floatsidf(42)));
	printf("%f %i", 63.25, __fixdfsi(63.25));

	printf("%i", dcmp(63.25, 42.0));
	printf("%i", dcmp(42.0, 63.25));
	printf("%i", dcmp(42.0, 42.0));

	DUMPDH(63.25);
	DUMPDH(42.0);
	DUMPDH(__negdf2(63.25));
	DUMPDH(__negdf2(42.0));
	DUMPDH(-63.25);
	DUMPDH(-42.0);
	DUMPDH(2.1470*1000000000.0);
	DUMPDH(2.1475*1000000000.0);	//vsprintf has a bug

	//wait for keypress, then exit
	printf("Press 0 to exit\n");
	FREEZE;
	return 0;
}

/*float fcal(float a, float b) {
	float f = a * 42 + b / 42;
	return f + 42;
}*/
