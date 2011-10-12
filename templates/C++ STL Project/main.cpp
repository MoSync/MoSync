/*
 * This project is set up to use STL (Standard Template Library) and
 * the Newlib standard C library (Newlib is required for STL).
 *
 * The program below makes a simple demonstration of STL. Numbers are
 * added to a vector and printed to the screen when the display is
 * touched or a key on the keypad is pressed.
 */

#include <ma.h>
#include <stdio.h>
#include <vector>

/**
 * Function used to demonstrate STL. Add a number
 * to a vector and print the numbers in the vector.
 */
void addAndPrint(std::vector<int>& v, int n)
{
	// Add number.
	v.push_back(n);

	// If more that 5 numbers, remove the first number.
	if (v.size() > 5)
	{
		v.erase(v.begin());
	}

	// Print numbers with a space between them.
	for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
	{
		printf("%d ", *it);
	}

	// Output line.
	printf("\n");
}

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	MAEvent event;
	std::vector<int> v;

	printf("Press zero or back to exit\n");
	printf("Press key or touch screen to add number to vector\n");

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

			addAndPrint(v, event.key);
		}
		else if (EVENT_TYPE_POINTER_PRESSED == event.type)
		{
			addAndPrint(v, event.point.y);
		}
	}

	return 0;
}
