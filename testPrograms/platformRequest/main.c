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
#include <maassert.h>

int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	printf("Calling...\n");
	int result = maPlatformRequest("http://www.example.com/");
//	int result = maPlatformRequest("tel:0763-113276");
	printf("result: %i\n", result);

	for(;;) {
		EVENT event;
		while(maGetEvent(&event)) {
			printf("e\n");
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			}
		}
		maWait(0);
	}
}
