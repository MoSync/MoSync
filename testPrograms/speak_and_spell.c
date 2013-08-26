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
#include <IX_GUIDO.H>

int MAMain() {
	maSetColor(0x00ff00);
	maDrawText(10, 10, "Hello World!");
	maDrawText(10, 40, "Use the joystick.");
	maDrawText(10, 60, "Press 0 to exit.");
	maUpdateScreen();
	while(1) {
		maWait(0);
		MAEvent event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				break;
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == 0)
					break;
				if(event.key == MAK_DOWN) {
//					maStartSpeaking("Nao spell, Com back A-li, Com back A-li's sister.");
					maStartSpeaking("Now spell, Come back Ali, Come back Ali's sister.");
				}
				if(event.key == MAK_5) {
					maStartSpeaking("Nao spell, Com back A-li, Com back A-li's sister.");
					maStartSpeaking("Now spell, Carp-for-brains.");
				}
				if(event.key == MAK_UP) {
//					maStartSpeaking("Things wer just about to start getting crazy-go-nuts.");
					maStartSpeaking("Things were just about to start getting crazy-go-nuts.");
				}
				if(event.key == MAK_RIGHT) {
//					maStartSpeaking("Nao spell, Douuuuuuuuble Deuce.");
					maStartSpeaking("Noe spell, Douuuuuuuuble Deuce.");
				}
				if(event.key == MAK_LEFT) {
					maStartSpeaking("Hellote");
				}
			}	//EVENT_TYPE_KEY_PRESSED
		}	//maGetEvent
	}	//1
	return 0;
}
