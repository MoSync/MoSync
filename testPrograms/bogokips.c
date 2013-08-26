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
#include <mastdlib.h>

//#define USE_PUTS

int t[256];
static const int instructionsPerLoop = 14;
static const int nLoops = 100000;
static const int minTime = 1000;

int MAMain() {
	int v, i, time, startTime;
	int nitr;
	MAEvent event;
	char buf[64];

#ifdef USE_PUTS
	printf("Testing %i loops\n", nLoops);
	printf("Fire->Quit, other->restart\n");
	printf("of %i instructions\n", nLoops * instructionsPerLoop);
#endif
#if 0
	puts("Testing ");
	itoa(nLoops, buf, 10);
	puts(buf);
	puts(" loops\n");
	puts("Fire->Quit, other->restart\n");
#endif
beginning:
	startTime = maGetMilliSecondCount();
	nitr = 0;
	do {
		for(i=0; i<nLoops; i++)
		{
			v = t[i & 0xff];
			t[(i-v) & 0xff] = v + (41*101*65537);
		}
		time = maGetMilliSecondCount() - startTime;
		nitr++;
	} while(time < minTime);
#ifndef USE_PUTS
	sprintf(buf, "%i ms, %i KIPS", time, (nLoops * instructionsPerLoop * nitr) / time);
	maSetColor(0);
	maFillRect(0, 0, 500, 500);
	maSetColor(-1);
	maDrawText(10, 10, buf);
	maUpdateScreen();
#endif
waitLoop:
	while(!(maGetEvent(&event))) maWait(0);
#ifdef USE_PUTS
	printf("Event: %i %i\n", event.type, event.key);
#endif
	if((event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_FIRE) || event.type == EVENT_TYPE_CLOSE)
		maExit(456);
	else if((event.type == EVENT_TYPE_KEY_RELEASED))
		goto waitLoop;
	else
		goto beginning;
	return 0;
}
