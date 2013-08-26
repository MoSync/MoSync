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
#include <IX_CALL.h>
#include <conprint.h>
#include <maassert.h>

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	printf("Calling...\n");
	int res = maCallDial("0703075652");
	printf("First result: %i\n", res);

	while(res == 0) {
		EVENT event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CALL) {
				printf("Call state: %i\n", event.state);
			}
		}
	}
	FREEZE;
}
