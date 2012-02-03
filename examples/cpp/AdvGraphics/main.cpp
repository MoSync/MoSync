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

/** \file main.cpp
*
* This file contains the main example program source.
*
* \author Patrick Broman and Niklas Nummelin
*
*/


#include <ma.h>
#include <mastdlib.h>
#include <conprint.h>
#ifdef MAPIP
#include <maprofile.h>
#endif

#include "MAHeaders.h"

class DynamicResources {
public:
	static MAHandle maCreateDrawableImage(int width, int height) {
		MAHandle h = maCreatePlaceholder();
		::maCreateDrawableImage(h, width, height);
		return h;
	}

	static MAHandle maCreateImageRaw(const void *src, MAExtent size, int alpha) {
		MAHandle h = maCreatePlaceholder();
		::maCreateImageRaw(h, src, size, alpha);
		return h;
	}
};

#define RGBA(r, g, b, a) ((r)<<16)|((g)<<8)|((b))|((a)<<24)
#define MAX_IMAGES 100

#ifdef MA_PROF_SUPPORT_STYLUS
int gIndex = 0;
int gTimeToUpdate = 0;
#endif	// MA_PROF_SUPPORT_STYLUS
int gNumImages = 0;
MAHandle gImages[MAX_IMAGES];

static MAHandle generateImage(int width, int height, int r, int g, int b) {
	int *temp = new int[width*height];
	int *dst = temp;
	double longestLength;
	if(width<height)
		longestLength = width>>1;
	else
		longestLength = height>>1;

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			double d = sqrt((double)((width>>1)-i)*((width>>1)-i) +
				((height>>1)-j)*((height>>1)-j));
			d /= longestLength;
			d = 1.0-d;
			if(d<0) d = 0;
			if(d>1) d = 1;
			unsigned char intensity = (unsigned char)(d*255.0);
			*dst++ = RGBA((intensity*r)>>8, (intensity*g)>>8, (intensity*b)>>8, intensity);
		}
	}

	MAHandle h = DynamicResources::maCreateImageRaw(temp, EXTENT(width, height), 1);
	delete temp;
	return h;
}

static void createImage() {
	if(gNumImages+1>=MAX_IMAGES) return;
	gImages[gNumImages] = generateImage(32, 32, rand()&0xff, rand()&0xff, rand()&0xff);
	gNumImages++;
}

static void drawImages() {
	double a = 0;
	double time = maGetMilliSecondCount()*0.001;
	MAExtent scr = maGetScrSize();
	int w = EXTENT_X(scr)>>1;
	int h = EXTENT_Y(scr)>>1;
	for(int i = 0; i < gNumImages; i++) {
		int x = (int)(cos(a*1.5+time*1.1)*50.0) + w;
		int y = (int)(sin(a*1.09+time*1.2)*50.0) + h;
		MAExtent imageSize = maGetImageSize(gImages[i]);
		maDrawImage(gImages[i], x-(EXTENT_X(imageSize)>>1), y-(EXTENT_Y(imageSize)>>1));
		a+=1.0;
	}
}

static void showInstruction() {
#ifdef MA_PROF_SUPPORT_STYLUS
	if (gTimeToUpdate <= maGetMilliSecondCount()) {
		gIndex=!gIndex;
		gTimeToUpdate = maGetMilliSecondCount() + 1000;
	}

	static const char* text[2] = {
			"Tap the screen to cast a spell.",
			"Press fire to cast a spell."
	};
#else	// MA_PROF_SUPPORT_STYLUS
	static const char* text = "Press fire to cast a spell.";
#endif
#ifdef MA_PROF_SUPPORT_STYLUS
	int textHeight = EXTENT_Y(maGetTextSize(text[gIndex]));
#else	// MA_PROF_SUPPORT_STYLUS
	int textHeight = EXTENT_Y(maGetTextSize(text));
#endif	// MA_PROF_SUPPORT_STYLUS
	int screenWidth = EXTENT_X(maGetScrSize());
	maSetColor(0);	//black
	maFillRect(0,0, screenWidth, textHeight);
	maSetColor(~0);	//white
#ifdef MA_PROF_SUPPORT_STYLUS
	maDrawText(0, 0, text[gIndex]);
#else	// MA_PROF_SUPPORT_STYLUS
	maDrawText(0, 0, text);
#endif	// MA_PROF_SUPPORT_STYLUS
}

extern "C" {
/**
 * \brief The entry point.
 */
int MAMain()
{
	bool run = true;
	bool focus = true;
	while(run) {
		if(focus) {
			drawImages();

			// write some information text
			showInstruction();

			/// Updates the screen
			maUpdateScreen();

			/// Keep the backlight alive.
			maResetBacklight();
		} else {
			maWait(0);
		}

		/// Get any available events.
		/// If MAK_FIRE is pressed, change mode.
		/// On Close event or MAK_0 press, close program.
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				switch(event.key) {
					case MAK_FIRE:
					case MAK_5:
						{
							createImage();
						}
						break;
					case MAK_SOFTRIGHT:
					case MAK_0:
					case MAK_BACK:
						run = false;
						break;

				}
#ifdef MA_PROF_SUPPORT_STYLUS
			} else if(event.type == EVENT_TYPE_POINTER_PRESSED) {
				createImage();
#endif	// MA_PROF_SUPPORT_STYLUS
			} else if(event.type == EVENT_TYPE_CLOSE) {
				run = false;
			} else if(event.type == EVENT_TYPE_FOCUS_LOST) {
				focus = false;
			} else if(event.type == EVENT_TYPE_FOCUS_GAINED) {
				focus = true;
			}
		}
	}

	return 0;
}
}
