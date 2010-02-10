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
*
* This file contains the main example program source.
*
* \author Patrick Broman and Niklas Nummelin
*
*/


#include <ma.h>
#include <mastdlib.h>
#include <conprint.h>
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


extern "C" {
/**
 * \brief The entry point.
 */
int MAMain()
{
	static const char* text = "press fire to cast a spell.";
	int textHeight = EXTENT_Y(maGetTextSize(text));
	int screenWidth = EXTENT_X(maGetScrSize());
	bool run = true;
	while(run) {
		drawImages();

		// write some information text
		maSetColor(0);	//black
		maFillRect(0,0, screenWidth, textHeight);
		maSetColor(~0);	//white
		maDrawText(0, 0, text);

		/// Updates the screen
		maUpdateScreen();

		/// Keep the backlight alive.
		maResetBacklight();

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
					case MAK_0:
						run = false;
						break;

				}
			} else if(event.type == EVENT_TYPE_CLOSE) {
				run = false;
			}
		}
	}

	return 0;
}
}
