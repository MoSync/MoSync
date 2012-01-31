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
#include <IX_CALL.h>

int MAMain(void) {
	int result;
	InitConsole();
	gConsoleLogging = 1;

	printf("Calling...\n");
#if 1
	result = maPlatformRequest("http://www.example.com/");
#else
	result = maPlatformRequest("tel:0763113276");
#endif
	printf("result: %i\n", result);

	for(;;) {
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CALL) {
				printf("call state: %i\n", event.state);
				continue;
			}
			printf("event %i\n", event.type);
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				return 0;
			}
		}
		maWait(0);
	}
}
