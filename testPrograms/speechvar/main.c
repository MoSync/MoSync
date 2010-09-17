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

static void Speak(int vol10, int pitch10, int speed10) {
	int vol100 = vol10 * 10;
	int pitch100 = pitch10 * 10;
	int speed100 = speed10 * 10;
	char buffer[32];
	maStopSpeaking();
	printf("volym %i, prev %i\n", vol100, maSetSpeechVolume(vol100));
	printf("speed %i, prev %i\n", speed100, maSetSpeechSpeed(speed100));
	sprintf(buffer, "pitch %i, prev %i", pitch100, maSetSpeechPitch(pitch100));
	printf("%s\n", buffer);
	maStartSpeaking(buffer);
}

int MAMain() {
	int vol10 = 10;
	int pitch10 = 10;
	int speed10 = 10;

	printf("Hello World!\n");

	//Speak(vol10, pitch10);
	for(;;) {
		MAEvent e;
		maWait(0);
		if(maGetEvent(&e) == 0)
			continue;
		printf("event %x\n", e.type);
		if(e.type == EVENT_TYPE_CLOSE) {
			maExit(0);
		} else if(e.type == EVENT_TYPE_KEY_PRESSED) switch(e.key) {
		case MAK_0:
			maExit(0);
		case MAK_UP:
			if(vol10 < 10)
				vol10 += 3;
			Speak(vol10, pitch10, speed10);
			break;
		case MAK_DOWN:
			if(vol10 > 0)
				vol10 -= 3;
			Speak(vol10, pitch10, speed10);
			break;
		case MAK_LEFT:
			if(pitch10 > 0)
				pitch10 -= 3;
			Speak(vol10, pitch10, speed10);
			break;
		case MAK_RIGHT:
			if(pitch10 < 10)
				pitch10 += 3;
			Speak(vol10, pitch10, speed10);
			break;
		case MAK_1:
			if(speed10 > 0)
				speed10 -= 3;
			Speak(vol10, pitch10, speed10);
			break;
		case MAK_3:
			if(speed10 < 10)
				speed10 += 3;
			Speak(vol10, pitch10, speed10);
			break;
		}
	}
}
