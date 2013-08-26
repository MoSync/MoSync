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
