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
#include <matime.h>

int MAMain() {
	InitConsole();

	printf("Current date/time is:\n");

	while(1) {
		MAEvent event;
		if(maGetEvent(&event))
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
				maExit(0);

		printf("%s (UTC)\n", sprint_time(maTime()));
		printf("%s (local)\n", sprint_time(maLocalTime()));
		maWait(1000);
	}
}
