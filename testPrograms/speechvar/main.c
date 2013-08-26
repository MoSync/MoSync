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
