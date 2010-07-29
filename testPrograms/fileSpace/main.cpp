/* Copyright (C) 2010 Mobile Sorcery AB

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
#include <MAUtil/String.h>
#include <IX_FILE.h>

#define TEST(func) do { int _res = (func); if(_res < 0) { printf("Fail %i @ %i\n", _res, __LINE__); return; } } while(0)

static void checkSpace() {
	MAHandle file = maFileOpen("/", MA_ACCESS_READ);
	TEST(file);
	int total = maFileTotalSpace(file);
	int avail = maFileAvailableSpace(file);
	printf("total: %i\n", total);
	printf("avail: %i\n", avail);
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");

	checkSpace();

	FREEZE;
}
