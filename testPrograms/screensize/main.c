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
	InitConsole();

	Extent e = maGetScrSize();
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	printf("%i x %i\n", EXTENT_X(e), EXTENT_Y(e));
	//freeze
	while(1) {
		maWait(WAIT_EVENT, 0);
		switch(maGetEvent()) {
		case EVENT_CLOSE:
		case MAK_0:
			return 0;
		}
	}
}
