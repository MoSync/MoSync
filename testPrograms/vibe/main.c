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

int MAMain() {
	int available = maVibrate(1);
	int on = FALSE;

	maSetColor(-1);
	maDrawText(0, 0, available ? "Vibrator available" : "Vibrator unavaliable");
	maUpdateScreen();

	for(;;) {
		EVENT event;
		while(maGetEvent(&event)) {
			switch(event.type) {
				case EVENT_TYPE_KEY_PRESSED:
					switch(event.key) {
				case MAK_FIRE:
					if(!on) {
						on = TRUE;
						if(!maVibrate(1000))
							maDrawText(0, 100, "Error!");
					} else {
						on = FALSE;
						maVibrate(0);
					}
					maSetColor(0);
					maFillRect(0, 50, EXTENT_X(maGetScrSize()), 50);
					maSetColor(-1);
					maDrawText(0, 50, on ? "Vibrator on" : "Vibrator off");
					maUpdateScreen();
					break;
				case MAK_0:
					maExit(0);
					break;
					}
					break;
				case EVENT_TYPE_CLOSE:
					maExit(0);
					break;
			}
		}
		if(on) {
			if(!maVibrate(1000))
				maDrawText(0, 100, "Error!");
			maWait(40);
		} else {
			maWait(0);
		}
	}
}
