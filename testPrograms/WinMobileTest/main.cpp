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
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Patrick Broman and Niklas Nummelin
*
*/


#include <ma.h>
#include <mastdlib.h>
#include <mastring.h>
#include <conprint.h>
#include "MAHeaders.h"
#include "MAUI/Scaler.h"

extern "C" {
/**
 * \brief The entry point.
 */
int MAMain()
{
	Extent e = maGetScrSize();
	int width = EXTENT_X(e);
	int height = EXTENT_Y(e);

	Extent ep = maGetImageSize(SPOTIFY_LOGO);
	int iw = EXTENT_X(ep);
	int ih = EXTENT_Y(ep);
		int j = 0;

		int rotMap[] = {
			TRANS_NONE,
			TRANS_ROT90,
			TRANS_ROT180,
			TRANS_ROT270
		};

		int mirrorRotMap[] = {
			TRANS_MIRROR,
			TRANS_MIRROR_ROT90,
			TRANS_MIRROR_ROT180,
			TRANS_MIRROR_ROT270
		};

		int scale = 0;
		
	while(1) {
			/// Get the current events, if event equals MAK_FIRE change mode of example, if event equals EVENT_CLOSE, close program.
			int event;
			while((event = maGetEvent())!=0) {
				if(event == EVENT_CLOSE || event == MAK_0) {
					maExit(0);
				}
			}


			maSetColor(0xff0000);
			maFillRect(0, 0, width, height);


			MARect rect = {0, 0, iw>>1, ih};
			MAPoint2d p = {(width>>1)+(int)(sin((double)maGetMilliSecondCount()/1700)*280.0), (height>>1)};
			maDrawImageRegion(SPOTIFY_LOGO, &rect, &p, rotMap[j]);
			j++;
			j&=0x3;

			/// Updates the screen
			maUpdateScreen();

			/// Keep the backlight alive.
			maResetBacklight();

			maWait(0, 500);
		}

		return 0;
	}
}
