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