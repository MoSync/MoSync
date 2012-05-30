/*
 * This project is set up to use the Newlib standard C library.
 */

#include <ma.h>
#include <stdio.h>

/**
 * Entry point of the program.
 */
int MAMain()
{
	MAEvent event;

	printf("Press zero or back to exit\n");

	BOOL isRunning = TRUE;
	while (isRunning)
	{
		maWait(0);
		while (maGetEvent(&event))
		{
			if (EVENT_TYPE_CLOSE == event.type)
			{
				isRunning = FALSE;
				break;
			}
			else if (EVENT_TYPE_KEY_PRESSED == event.type)
			{
				if (MAK_BACK == event.key || MAK_0 == event.key)
				{
					isRunning = FALSE;
					break;
				}

				printf("You typed: %c\n", event.key);
			}
		}
	}

	return 0;
}
