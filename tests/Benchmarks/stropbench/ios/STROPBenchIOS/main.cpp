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

/*
 * STROPS is a String operation benchmark
 * Written by Alexander Samuelsson 7/2011
 */

#include "stropbench.h"

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 *
 * Create a Moblet project to use the high level
 * MoSync C++ library.
 */
extern "C" int MAMain()
{

	StropBencher * stropbencher = new StropBencher;
	stropbencher->bench();

	/*MAEvent event;

	printf("Press zero or back to exit");

	while (TRUE)
	{
		maWait(0);
		maGetEvent(&event);

		if (EVENT_TYPE_CLOSE == event.type)
		{
			// Exit while loop.
			break;
		}
		else if (EVENT_TYPE_KEY_PRESSED == event.type)
		{
			if (MAK_BACK == event.key || MAK_0 == event.key)
			{
				// Exit while loop.
				break;
			}

			printf("You typed: %c", event.key);
		}
	}*/

	//free heap mem
	delete stropbencher;

	return 0;
}
