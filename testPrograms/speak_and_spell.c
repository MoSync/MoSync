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
