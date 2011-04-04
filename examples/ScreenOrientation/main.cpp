/* Copyright (C) 2011 MoSync AB

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

/** @file main.cpp
*
*  Application that detects a change of screen orientation.
*  Initially, you should see a blue rectangle that fills the screen, with the
*  text "Started. When the device is rotated (F4 in the emulator) the text will
*  change to "Portrait" or "Landscape" to show the current orientation.
*  Press the zero or back key to exit.
*
* @author Miles Midgley and Chris Hughes
*/

#include <MAUtil/Moblet.h>
#include <conprint.h>

class MyMoblet : public MAUtil::Moblet
{

public:

	//Constructor.
	MyMoblet()
	{
		//Set the orientation mode to dynamic, so that it is possible to switch
		//between portrait and landscape orientations. You can also use this
		//syscall to lock the screen in _PORTRAIT or _LANDSCAPE mode. Not all
		//platforms support or need this syscall.)
		maScreenSetOrientation(SCREEN_ORIENTATION_DYNAMIC);

		//Get the screen size.
		MAExtent extent = maGetScrSize();

		//Display an initial text message to say we are started.
		draw(EXTENT_X(extent),EXTENT_Y(extent),0,EXTENT_Y(extent)/2,"Started");
	}

	//Method for handling key presses.
	void keyPressEvent(int keyCode, int nativeCode)
	{
		//Exit on any key press
		maExit(0);
	}

	//Method for handling custom events. (Moblet is a CustomEventListener.)
	void customEvent(const MAEvent& event)
	{

		//If the event type is screen changed...
		if (event.type == EVENT_TYPE_SCREEN_CHANGED)
		{
			//Declare a character buffer to hold the display text.
			char orientationText[80];

			//Get the screen size
			MAExtent extent = maGetScrSize();

			if (EXTENT_X(extent) > EXTENT_Y(extent)) // Landscape
			{
					sprintf(orientationText,"Landscape %d %d",EXTENT_X(extent),EXTENT_Y(extent));
			}
			else // Portrait
			{
					sprintf(orientationText,"Portrait %d %d",EXTENT_X(extent),EXTENT_Y(extent));
			}

			//Call the draw method passing the screen dimensions, the position
			//to draw the display text, and the text itself.
			draw(EXTENT_X(extent),EXTENT_Y(extent),0,EXTENT_Y(extent)/2,orientationText);
		}
	}

	//Method that draws the screen, writes the text, and then updates the screen.
	void draw(int screenWidth, int screenHeight,int x, int y, const char* text)
	{
		//Set the drawing colour to blue.
		maSetColor(0x000099);
		//Draw a rectangle that will cover the screen.
		maFillRect(0, 0, screenWidth, screenHeight);
		//Reset the drawing colour to white.
		maSetColor(0xFFFFFF);
		//Draw the text at the nominated position.
		maDrawText(x, y, text);
		//Display the screen.
		maUpdateScreen();
	}

};

//Entry point for the application.
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new MyMoblet());
	return 0;
};

