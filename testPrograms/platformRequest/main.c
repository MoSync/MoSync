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
