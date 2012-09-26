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
