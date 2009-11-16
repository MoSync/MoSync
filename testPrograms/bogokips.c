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
