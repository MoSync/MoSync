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
#include <MAUtil/Graphics.h>

/*
 Author: mikael.kindborg@mosync.com

 Program that tests screen orientation.

 How to use:

   You should see a green rectangle.

   When the device is rotated the color should change
   to be brighter in landscape orientation and darker
   in portrait orientation.

   Touch screen or press a key to toggle color
   between blue and green.

   Press zero or back to exit.
*/

static int gState;

static void PaintScreen()
{
	MAExtent extent = maGetScrSize();
	int color;
	if (EXTENT_X(extent) > EXTENT_Y(extent)) // Landscape
	{
		color = (gState % 2 == 0 ? 0x00FF00 : 0x0000FF);
	}
	else // Portrait
	{
		color = (gState % 2 == 0 ? 0x009900 : 0x000099);
	}
	maSetColor(color);
	maFillRect(0, 0, EXTENT_X(extent), EXTENT_Y(extent));
	maUpdateScreen();
}

extern "C" int MAMain()
{
	gState = 0;
	PaintScreen();

	while (1)
	{
		MAEvent event;
		maWait(0);
		while(maGetEvent(&event))
		{
			if (event.type == EVENT_TYPE_CLOSE)
			{
				maExit(0);
			}
			else
			if (event.type == EVENT_TYPE_KEY_PRESSED)
			{
				if (event.key == MAK_0 || event.key == MAK_BACK)
				{
					maExit(0);
				}
				else
				{
					++gState;
					PaintScreen();
				}
			}
			else if (event.type == EVENT_TYPE_SCREEN_CHANGED)
			{
				lprintfln("EVENT_TYPE_SCREEN_CHANGED");
				PaintScreen();
			}
			else if (event.type == EVENT_TYPE_POINTER_PRESSED)
			{
				++gState;
				PaintScreen();
			}
		}
	}

	return 0;
}
