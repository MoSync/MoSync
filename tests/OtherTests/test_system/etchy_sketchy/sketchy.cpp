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

/** \file sketchy.cpp
*	Etchy-Sketchy
*	
*	Draw quick and easy sketches by using the numbered keys:
*	1 = up left diagonal
*	2 = up
*	3 = up right diagonal
*	4 = left
*	6 = right
*	7 = down left diagonal
*	8 = down
*	9 = down right diagonal
*	
*	Press 5 to clear screen.
*	Press 0 to exit.
*
* \author Ann-Sofie Lindblom
*/

#include <ma.h>
#include "MAHeaders.h"
#include <MAUtil/Environment.h>

#define STOP 0
#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

#define TIME_STEP 5

extern "C" int MAMain() {
	Extent 	screenSize 	= maGetScrSize();
	int 	screenWidth 	= EXTENT_X(screenSize);
	int 	screenHeight 	= EXTENT_Y(screenSize);
	int 	imageWidth 	= 2;
	int 	imageHeight = 2;
	int		offset_x 	= 0;
	int		offset_y 	= 0;
	int 	horizontal = STOP;
	int 	vertical = STOP;

	int 	startTime 	= maGetMilliSecondCount();

	/* Draw background */
	maSetColor(0xffffff);
	maFillRect(0, 0, screenWidth, screenHeight);
	maUpdateScreen();
	
	while(true) {
		EVENT event;
		/* Read user input */
		while(maGetEvent(&event) > 0) {	
			if(event.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
			else if(event.type == EVENT_TYPE_KEY_PRESSED) {
				switch(event.key) {
					case MAK_0: // this is the exit key
						maExit(0);
					case MAK_1:	// left-up
						horizontal = LEFT;
						vertical = UP;
						break;
					case MAK_2:	// up
					case MAK_UP:
						vertical = UP;
						break;
					case MAK_3:	// right-up
						horizontal = RIGHT;
						vertical = UP;
						break;
					case MAK_4: // left
					case MAK_LEFT:
						horizontal = LEFT;
						break;
					case MAK_5: // clear screen, reset draw position
						offset_x = 0;
						offset_y = 0;
						maSetColor(0xffffff);
						maFillRect(0, 0, screenWidth, screenHeight);
						maUpdateScreen();
						break;
					case MAK_6: // right
					case MAK_RIGHT:
						horizontal = RIGHT;
						break;
					case MAK_7:	// left-down
						horizontal = LEFT;
						vertical = DOWN;
						break;
					case MAK_8:	// down
					case MAK_DOWN:
						vertical = DOWN;
						break;
					case MAK_9:	//right-down
						horizontal = RIGHT;
						vertical = DOWN;
						break;
				}
			}
			else if(event.type == EVENT_TYPE_KEY_RELEASED) {
				horizontal = STOP;
				vertical = STOP;
			}
		}

		/* Update draw position */
		if( (maGetMilliSecondCount()) >= (startTime) ) {
			startTime += TIME_STEP;
			if( horizontal == LEFT ) { // left
				if((screenWidth/2 - imageWidth/2) + offset_x > 0) --offset_x;
			}	
			else if( horizontal == RIGHT ) { // right
				if((screenWidth/2 + imageWidth/2) + offset_x < screenWidth) ++offset_x;
			}	

			if( vertical == UP ) { // up
				if((screenHeight/2 - imageHeight/2) + offset_y > 0) --offset_y;
			}	
			else if( vertical == DOWN ) { // down
				if((screenHeight/2 + imageHeight/2) + offset_y < screenHeight) ++offset_y;
			}
		}
		int x = (screenWidth/2 - imageWidth/2) + offset_x;
		int y = (screenHeight/2 - imageHeight/2) + offset_y;

		/* Draw line section */
		maSetColor(0x0033ff);
		maFillRect(x, y, imageWidth, imageHeight);
		maUpdateScreen();
	}
}

