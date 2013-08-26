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
