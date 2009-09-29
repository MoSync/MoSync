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

//Testing speech variables (volume, speed, pitch)

#include <ma.h>
#include <conprint.h>
#include <IX_GUIDO.H>

void Speak(int vol10) {
	int vol100 = vol10 * 10;
	char buffer[32];
	maStopSpeaking();
	sprintf(buffer, "volym %i, prev %i", vol100, maSetSpeechVolume(vol100));
	PrintConsole(buffer);
	maStartSpeaking(buffer);
}

int MAMain() {
	int vol10 = 10;

	InitConsole();

	Speak(vol10);
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
		case MAK_UP:
			if(vol10 < 10)
				vol10+=3;
			Speak(vol10);
			break;
		case MAK_DOWN:
			if(vol10 > 0)
				vol10-=3;
			Speak(vol10);
			break;
		}
	}
}
