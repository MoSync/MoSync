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
#include <conprint.h>
#include <mastring.h>
#include <mastdlib.h>

int MAMain() {
	double d;
	static const char str1[] = "4.56";
	static const char str2[] = "12389.5612323545034954378343";

	InitConsole();
	printf("Hello World.\n");
	printf("1: %s\n", str1);
	d = strtod(str1, NULL);
	printf("d: %f\n", d);
	printf("2: %s\n", str2);
	d = strtod(str2, NULL);
	printf("d: %f\n", d);

	//Freeze
	Freeze(0);
	return 0;
}
