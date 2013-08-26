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

#ifndef _MOBRIS_CLASSES_H_
#define _MOBRIS_CLASSES_H_

#include "types.h"
#include <maapi.h>

#include "starfield.h"
#include "Room.h"

#include <maapi.h>
#include "MAHeaders.h"
#include <madmath.h>

#ifndef _WIN32

void startcrt() {
	MAMain();
}
#define //printf(a,...)

#else
/*#pragma comment(lib,"MAWin.lib")*/
#include <stdio.h>

#endif


#ifdef _WIN32

#endif

#define FRAMERATE 200
#define FRAMEDELAY 1.0/FRAMERATE


/** Instance Pool **/

/*typedef struct InstancePoolElemenent_t {
	GameInstance inst;
} InstancePoolElement; */


void mobris_init() {
	initStars();
}


Float frameStart = 0.0;

#ifdef _WIN32
#define inline
#endif


inline void beginFrame() {
	frameStart = (Float)maGetMilliSecondCount()/1000.0;
}

inline void endFrame() {
	Float now = (Float)maGetMilliSecondCount()/1000.0;
	while(now - frameStart < FRAMEDELAY) {
		now = (Float)maGetMilliSecondCount()/1000.0;
	}
}

void init();

void runGame(GameRoom* r) {
	int keys;
	init();
	//initStars();
	screen_size = maGetScrSize();
	g_currFrame = 0;
	while(!(maGetEvent()==EVENT_CLOSE)) {
		beginFrame();

		maSetColor(0);
		maFillRect(0,0,EXTENT_X(screen_size), EXTENT_Y(screen_size));

		keys = maGetKeys();

		if(keys & MAK_LEFT) {
			Room_event(r, ET_LEFT);
		}
		if(keys & MAK_RIGHT) {
			Room_event(r, ET_RIGHT);
		}
		if(keys & MAK_UP) {
			Room_event(r, ET_UP);
		}
		if(keys & MAK_DOWN) {
			Room_event(r, ET_DOWN);
		}
		if(keys & MAK_FIRE) {
			Room_event(r, ET_FIRE);
		}

		Room_event(r, ET_FRAME);
		Room_render(r, g_currFrame);
		g_currFrame++; 

		endFrame();
		//updateStars();
		maUpdateScreen();
	}
}

Integer mobRand(Integer low, Integer high) {
	Integer dx = (high-low);
	Integer xRand = dx > 0 ? low + (rand() % dx) : 0;
}




#endif /* _MOBRIS_CLASSES_H_ */