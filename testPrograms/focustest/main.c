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

int MAMain() {
	const int baseTime = maGetMilliSecondCount();
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");

	while(1) {
		EVENT event;
		while(maGetEvent(&event)) {
			switch(event.type) {
				case EVENT_TYPE_CLOSE:
					maExit(0);
					break;
				case EVENT_TYPE_FOCUS_GAINED:
					printf("focus gained: %d", maGetMilliSecondCount() - baseTime);
					break;
				case EVENT_TYPE_FOCUS_LOST:
					printf("focus lost: %d", maGetMilliSecondCount() - baseTime);
					break;
				case EVENT_TYPE_KEY_PRESSED:
					if(event.key == MAK_0)
						maExit(0);
					break;
			}
		}
		maWait(0);
	}

	return 0;
}
