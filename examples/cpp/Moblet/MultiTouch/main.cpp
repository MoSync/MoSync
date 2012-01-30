/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/


/**
 * @file main.cpp
 *
 * This example illustrates how to handle multitouch on devices that support it.
 * A map of touchIds to coordinates are kept in memory. Whenever a touch
 * stops, it is removed from the map.
 *
 * When running in the emulator, right click to simulate multitouches around
 * the center of the screen.
 *
 * @author Niklas Nummelin and Anders Malm
 */

#include <MAUtil/Moblet.h>
#include <MAUtil/Map.h>
#include <mavsprintf.h>

/**
 * The size of the marker that will be shown at each touch position.
 */
#define MARKER_SIZE 50

class MyMoblet : public MAUtil::Moblet, public MAUtil::IdleListener
{
public:
	MyMoblet()
	{
		//When the moblet is has nothing else to do it will run the idle function.
		MAUtil::Environment::getEnvironment().addIdleListener(this);
	}

	/**
	* This function is called with a coordinate when the screen is touched.
	* The touch id will remain the same as long as this touch is active.
	*/
	virtual void multitouchPressEvent(MAPoint2d p, int touchId)
	{
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when an active touch moves.
	*/
	virtual void multitouchMoveEvent(MAPoint2d p, int touchId)
	{
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when the screen isn't touched
	*  anymore. This touch id will be deleted and will be reused when a new
	*  touch is initiated.
	*/
	virtual void multitouchReleaseEvent(MAPoint2d p, int touchId)
	{
		mTouches.erase(touchId);
	}

	/**
	 * This method is called when a key is pressed. The method
	 * is inherited from the Moblet class, and is overridden here.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode)
		{
			// Close the application if the back key or key 0 is pressed.
			if (MAK_BACK == keyCode || MAK_0 == keyCode)
			{
				// Call close to exit the application.
				close();
			}
		}

	/**
	 * This function will be called whenever the moblet has nothing else to do,
	 * drawing rectangles with the touchIds where the fingers touches the
	 * screen. The amount of touches possible on a device is platform dependent.
	 *
	 * In the MoRE emulator you can simulate multitouch by right-clicking the screen.
	 * This will simulate two touches, mirrored around the center of the screen.
	 * Each of these two touches will be marked with a blue circle.
	 */
	void idle()
	{

		// Clear the screen by drawing a black rectangle covering the visible area.
		maSetColor(0);
		int screenSize = maGetScrSize();
		maFillRect(0, 0, EXTENT_X(screenSize), EXTENT_Y(screenSize));

		// Loop through all the current touches and draw for each a rectangle
		// and the touch id.
		for(MAUtil::Map<int, MAPoint2d>::Iterator iterator = mTouches.begin();
			iterator != mTouches.end();
			iterator++)
		{
			MAPoint2d point = iterator->second;
			int touchId = iterator->first;

			char touchStr[16];
			sprintf(touchStr, "%d", touchId);

			// Draw a red rectangle, centred around the touch position
			maSetColor(0xff0000);
			maFillRect(point.x - (MARKER_SIZE/2), point.y - (MARKER_SIZE/2),
					MARKER_SIZE, MARKER_SIZE);

			// Draw the touch id to the top-left of each red rectangle.
			maSetColor(0xffffff);
			maDrawText(point.x + (MARKER_SIZE/2), point.y - (MARKER_SIZE/2),
					touchStr);
		}

		maUpdateScreen();
	}

private:

	//Define a map to store all the touches.
	MAUtil::Map<int, MAPoint2d> mTouches;

};

//The entry point for the application.
extern "C" int MAMain() {
	MAUtil::Moblet::run(new MyMoblet());
	return 0;
};
