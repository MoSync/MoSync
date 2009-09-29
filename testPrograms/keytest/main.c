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
#include <conprint.h>
#include "../shared/debug.h"

void doKey2(const char* name, int id);

int foo;

#define doKey(id) doKey2(#id, MAK_##id)

int MAMain() {
	//gets rid of "zero pointer to string literal" bug
	const char* meh ="MEH";
	char buffer[10];
	strcpy(buffer, meh);

	InitConsole();

	doKey(SOFTLEFT);
	doKey(SOFTRIGHT);
	doKey(CLEAR);
	/*doKey(UP);
	doKey(DOWN);
	doKey(LEFT);
	doKey(RIGHT);
	doKey(FIRE);
	doKey(1);
	doKey(2);
	doKey(3);
	doKey(4);
	doKey(5);
	doKey(6);
	doKey(7);
	doKey(8);
	doKey(9);
	doKey(0);
	doKey(STAR);
	doKey(GRID);*/

	//printf(meh);

#ifdef GUIDO
	doKey(SELLEFT);
	doKey(SELRIGHT);
	doKey(PEN);
	doKey(C);
#endif

	return 0;
}


void doKey2(const char* name, int id) {
	//printflnl("Press \"%s\"(0x%08x) (0x%x)", name, name, id);
	printflnl("Press \"%s\"(0x%x)", name, id);

	for(;;) {
		int event = maGetEvent();
		if(event == EVENT_CLOSE)
			maExit(0);
		if(event == id)//!= 0)//
			break;
		maWait(WAIT_EVENT, 0);
	}
}
