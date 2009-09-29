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
