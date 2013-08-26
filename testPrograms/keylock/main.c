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

//Tests the key lock mechanism.

#include <ma.h>
#include <conprint.h>
#include <IX_GUIDO.H>

int MAMain() {
	InitConsole();

	printf("Lock status: %i", maIsKeypadLocked());
	printf("Focus: %i", maHasFocus());

	for(;;) {
		int event;
		maWait(WAIT_EVENT, 0);
		event = maGetEvent();
		printf("event %x\n", event);
		switch(event) {
		case EVENT_CLOSE:
		case MAK_0:
			maExit(0);
			break;
		case MAK_DOWN:
			printf("Focus: %i", maHasFocus());
			printf("5 sek...");
			maStartSpeaking("Tangentlås ettusen, tvåtusen, tretusen");
			//maSleep(5000);	//buggy, doesn't actually yield. should set timer
			{
				int startTime = maGetMilliSecondCount();
				while(maGetMilliSecondCount() - startTime < 5000) {
					maWait(0, 20);
				}
			}
			printf("Focus: %i", maHasFocus());
			break;
		case MAK_FIRE:
			if(!maIsKeypadLocked()) {
				//int i = 0;
				printf("Focus: %i", maHasFocus());
				printf("Locking for 5 seconds...");
				maStartSpeaking("Tangentlås ettusen, tvåtusen, tretusen");
				maLockKeypad();
				printf("Nu är det låst!");
				printf("Focus: %i", maHasFocus());
				printf("...");
				printf("...");
				printf("...");
				printf("...");
				{
					int startTime = maGetMilliSecondCount();
					while(maGetMilliSecondCount() - startTime < 5000) {
						maWait(0, 20);
					}
				}
				if(maIsKeypadLocked()) {
					printf("Focus: %i", maHasFocus());
					printf("Unlocking...");
					maUnlockKeypad();
					printf("Unlocked!");
					printf("Focus: %i", maHasFocus());
				} else {
					printf("Already unlocked!");
					printf("Focus: %i", maHasFocus());
				}
				/*while(maIsKeypadLocked()) {
					printf("%i", i++);
				}*/
			} else {
				printf("This shouldn't happen.");
				printf("Focus: %i", maHasFocus());
			}
		}
	}
}
