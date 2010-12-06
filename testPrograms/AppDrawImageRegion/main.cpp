/* Copyright (C) 2010 MoSync AB

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
#include <MAUtil/Graphics.h>

/*
 Author: mikael.kindborg@mosync.com

 Program that tests maDrawImageRegion with clipping.

 How to use:

   You should see a blue rectangle with a yellow corner.
   It should be clipped by the green rectangle at the top of the screen.
   Touch screen or press a key to go to the next state.
   There are eight states:
     Normal
     Normal rotated 90 degrees
     Normal rotated 180 degrees
     Normal rotated 270 degrees
     Mirrored
     Mirrored rotated 90 degrees
     Mirrored rotated 180 degrees
     Mirrored rotated 270 degrees
   After state eight the program exits.

   You can exit at any time with the zero or back key.
   This app can also be used to test screen orientation.
*/

static int gState;
static MAHandle gImage;
static int gImageWidth = 200;
static int gImageHeight = 100;

static int gTransforms[] = {
	TRANS_NONE,
	TRANS_ROT90,
	TRANS_ROT180,
	TRANS_ROT270,
	TRANS_MIRROR,
	TRANS_MIRROR_ROT90,
	TRANS_MIRROR_ROT180,
	TRANS_MIRROR_ROT270
};

static MAHandle CreateImage()
{
	MAHandle image = maCreatePlaceholder();

	int result = maCreateDrawableImage(image, gImageWidth, gImageHeight);
	if (RES_OK != result) { maPanic(1, "maCreateDrawableImage failed"); }

	MAHandle previousDrawTarget = maSetDrawTarget(image);
	maSetColor(0x0000CC);
	maFillRect(0, 0, gImageWidth, gImageHeight);
	maSetColor(0xFFFF00);
	maFillRect(0, 0, 100, 50);

	maSetDrawTarget(previousDrawTarget);

	return image;
}

static void DrawImage(MAHandle image, int left, int top, int transform)
{
	MARect sourceRect;
	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.width = gImageWidth;
	sourceRect.height = gImageHeight / 2 + 10;

	MAPoint2d destPoint;
	destPoint.x = left;
	destPoint.y = top;

	maDrawImageRegion(image, &sourceRect, &destPoint, transform);
}

/**
 * Version of PaintScreen that tests maDrawImageRegion with a clip rectangle.
 */
static void PaintScreen()
{
	MAExtent extent = maGetScrSize();
	lprintfln(
		"maGetScrSize: %i %i",
		EXTENT_X(extent),
		EXTENT_Y(extent));

	int color = 0x77FF77;
	maSetColor(color);
	maFillRect(0, 0, EXTENT_X(extent), EXTENT_Y(extent));

	maSetColor(0xFFFF77);
	maFillRect(0, 120, EXTENT_X(extent), EXTENT_Y(extent));

	MARect previousClipRect;
	maGetClipRect(&previousClipRect);
	lprintfln(
		"previousClipRect: %i %i %i %i",
		previousClipRect.left,
		previousClipRect.top,
		previousClipRect.width,
		previousClipRect.height);

	maSetClipRect(0, 120, EXTENT_X(extent), EXTENT_Y(extent));

	DrawImage(gImage, 100, 100, gTransforms[gState]);

	maSetClipRect(
		previousClipRect.left,
		previousClipRect.top,
		previousClipRect.width,
		previousClipRect.height);

	maUpdateScreen();
}

static void gotoNextState()
{
	if (gState < 7)
	{
		++gState;
		PaintScreen();
	}
	else
	{
		maExit(0);
	}
}

/**
 * Simplified version PaintScreen that is NOT used.
 * If you want to use it for testing purposes, replace
 * calls to PaintScreen with calls to xPaintScreen in
 * the code below.
 */
static void xPaintScreen()
{
	MAExtent extent = maGetScrSize();
	int color = (1 == gState ? 0x00FF00 : 0xFFFF00);
	maSetColor(color);
	maFillRect(10, 10, EXTENT_X(extent) - 20, EXTENT_Y(extent) - 20);
	maUpdateScreen();
}

extern "C" int MAMain()
{
	gState = 0;
	gImage = CreateImage();
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
					gotoNextState();
				}
			}
			else if (event.type == EVENT_TYPE_SCREEN_CHANGED)
			{
				lprintfln("EVENT_TYPE_SCREEN_CHANGED");
				PaintScreen();
			}
			else if (event.type == EVENT_TYPE_POINTER_PRESSED)
			{
				gotoNextState();
			}
		}
	}

	maDestroyObject(gImage);

	return 0;
}
