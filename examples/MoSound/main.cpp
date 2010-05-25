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
* Shows how to use the sound api. This example shows
* how to play and loop a sound once.
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <conprint.h>
#include <maprofile.h>
#include "MAHeaders.h"

//
// Handle different decoding capabilities of different
// runtimes.
//
#if defined( MA_PROF_STRING_PLATFORM_WM5 ) || \
	defined( MA_PROF_STRING_PLATFORM_WM6 ) || \
	defined( MA_PROF_STRING_PLATFORM_WM6PRO ) || \
	defined( MA_PROF_STRING_PLATFORM_SP2003 ) || \
	defined( MA_PROF_STRING_PLATFORM_MOBLIN )
#define SOUND_RESOURCE R_MOSO_WAV
#else
#define SOUND_RESOURCE R_MOSO_MP3
#endif




extern "C" {
/**
 * \brief The entry point.
 */
int MAMain() GCCATTRIB(noreturn);
int MAMain()
{
	MAExtent e = maGetScrSize();

	/// play R_MOSO sound
	maSoundPlay(SOUND_RESOURCE, 0, maGetDataSize(SOUND_RESOURCE));

	printf("Press 0/RSK to exit.");

	while(1) {
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && (event.key == MAK_0 || event.key == MAK_SOFTRIGHT)))
			{
				maExit(0);
			}
		}
	}
}
}
