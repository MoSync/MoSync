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
* Shows how to use the sound api. This example shows
* how to play a sound once.
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <conprint.h>
#include "MAHeaders.h"

// Set to zero to play only the WAV file.
#define DO_MP3 1

extern "C" {
/**
 * \brief The entry point.
 */
int MAMain() GCCATTRIB(noreturn);
int MAMain()
{
	//MAExtent e = maGetScrSize();
	int res;

	/// play R_MOSO sound
#if DO_MP3
	res = maSoundPlay(R_MOSO_MP3, 0, maGetDataSize(R_MOSO_MP3));
	if(res >= 0)
	{
		printf("MP3 succeeded.\n");
	}
	else
#endif
	{
#if DO_MP3
		printf("MP3 failed %i\n", res);
#endif
		res = maSoundPlay(R_MOSO_WAV, 0, maGetDataSize(R_MOSO_WAV));
		if(res >= 0)
		{
			printf("WAV succeeded.\n");
		}
		else
		{
			printf("WAV failed %i\n", res);
		}
	}
	printf("Press 0/RSK to exit.");

	while(1) {
		maWait(0);
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED &&
				(event.key == MAK_0 || event.key == MAK_SOFTRIGHT || event.key == MAK_BACK)))
			{
				maExit(0);
			}
		}
	}
}
}
