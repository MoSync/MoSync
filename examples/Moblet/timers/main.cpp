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

/** \file main.cpp
 * This application demonstrates how to use timers in Moblet-based
 * application. It produces an animation of a bouncing rectangle
 * which is updated every TIMER_PERIOD milliseconds.
 *
 * Prerequisites: The first Moblet example, some familiarity with
 * the graphics API.
 * 
**/

#include <conprint.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Moblet.h>
//#include <maprofile.h>
#include <madmath.h>

using namespace MAUtil;

/**
 * A bunch of preprocessor defintions for various constants.
 */

 // How often the timer will be invoked
#define TIMER_PERIOD 20
// How big the bouncing square will be
#define OBJ_SIZE (screenHeight/4)
// The ground level above the bottom of the screen
#define FLOOR_HEIGHT 18
// Floor level adjusted by size of object
#define FLOOR_Y (OBJ_SIZE + (FLOOR_HEIGHT))  
// Newtonian gravity constant, swedish edition
#define GRAVITY 9.82
// How much velocity we lose at each bounce
#define BOUNCE_PERC 0.9
// How much positive velocity we gain when pressing fire
#define FIRE_VELOCITY (screenHeight*0.04) 

// Take a guess
#define TEXT_COLOR 0xffffff
#define BACK_COLOR 0x516B78
#define FLOOR_COLOR 0xCC9940
#define OBJ_COLOR 0xF7DE5E

#ifdef MA_PROF_SUPPORT_STYLUS
#define MESSAGE "Tap screen to bounce"
#else
#define MESSAGE "Press fire to bounce"
#endif

/**
 * We make our Moblet inherit TimerListener and PointerListener.
 */

class TimerMoblet : public Moblet, public TimerListener {
public:

	/**
	 * In the Moblet constructor we initialize things,
	 * among others our instance variables.
	**/

	TimerMoblet() : bounceTime(0), velocity(0), t(0) {
		// Store screen dimensions in instance variables
		MAExtent scrSize = maGetScrSize();
		screenWidth = EXTENT_X(scrSize);
		screenHeight = EXTENT_Y(scrSize);
		// Start with the box on the floor
		y = FLOOR_Y;
		// This is th	e interesting part! We're registering ourselves
		// as a timer. That means that the function runTimerEvent()
		// will be called every TIMER_PERIOD milliseconds starting from
		// the time when Moblet::run() is invoked.
		addTimer(this, TIMER_PERIOD, 0);
		// Now we proceed to draw the screen background
		maSetColor(BACK_COLOR);
		maFillRect(0, 0, screenWidth, screenHeight - FLOOR_HEIGHT);
		maSetColor(FLOOR_COLOR);
		maFillRect(0, screenHeight - FLOOR_HEIGHT, screenWidth, FLOOR_HEIGHT);
		drawObject(OBJ_COLOR);
		maSetColor(TEXT_COLOR);
		maDrawText(3, 3, MESSAGE);
		maUpdateScreen();
	}

	// This function takes care of the actual rendering of our bouncing box
	void drawObject(int color) {
		maSetColor(color);
		maFillRect(screenWidth/2 - OBJ_SIZE/2, screenHeight - y, OBJ_SIZE, OBJ_SIZE);
	}

	void runTimerEvent() {
		if(fabs(velocity) - DBL_EPSILON < 0) {
			velocity = 0;
			return;
		}
		// instead of clearing the entire screen, we cunningly
		// erase only the area covered by our bouncing box by 
		// drawing it again using the background color.
		drawObject(BACK_COLOR);

		// Do some physics: v = v - g*t^2
		velocity -= (t*t*GRAVITY)/(1000000); // 
		// update box position
		y += (int)velocity;

		// make sure we don't fall through the floor
		if(y <= FLOOR_Y) { 
			y = FLOOR_Y;
			velocity = -velocity*BOUNCE_PERC;
		}
		// ..and draw the box at the newly calculated position
		drawObject(OBJ_COLOR);
		maUpdateScreen();
		t += TIMER_PERIOD;
	}
	
	/**
	 * The Moblet class inherits KeyListener, and automatically registers itself
	 * to recieve key events. Therefore, it must implement the functions keyPressEvent
	 * and KeyReleaseEvent. There are default implementations of these in the Moblet
	 * class, but it's likely you'll want to reimplement if your moblet is to be
	 * interactive. In this example, we will simply react to some key being pressed
	 * by printing out which key it was.
	**/

	void keyPressEvent(int keyCode, int nativeCode) {
#ifndef MA_PROF_SUPPORT_STYLUS
		switch(keyCode) {
			case MAK_FIRE:
				if(y == FLOOR_Y)  {
					// Reset time and give the box new velocity
					t = 0;
					velocity = FIRE_VELOCITY;
				}
				break;
			case MAK_SOFTRIGHT:
			case MAK_0:
				maExit(0);
			default:
				break;
		}
#endif
	}

	virtual void pointerPressEvent(MAPoint2d p) {
		if(y == FLOOR_Y)  {
			t = 0;
			velocity = FIRE_VELOCITY;
		}
	}
	virtual void pointerMoveEvent(MAPoint2d p) {}
	virtual void pointerReleaseEvent(MAPoint2d p) {}

private:
	int screenWidth;
	int screenHeight;
	int bounceTime;
	double velocity;
	int t;
	int y;
};

extern "C" int MAMain() {
	Moblet::run(new TimerMoblet());
	return 0;
}
