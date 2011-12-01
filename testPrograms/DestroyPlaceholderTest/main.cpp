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

/**
 * This is an app that is meant to test maDestoryPlaceholder.
 *
 * You will likely have to tweak the constants below for
 * the platform you run the test on, and depending on the
 * amount of memory on the device.
 */
#include <ma.h>
#include <conprint.h>
#include <MAUtil/String.h>

// Parameters for testCreateData
#define NUMBER_OF_ALLOCS 100000
#define CHUNK_SIZE (1024 * 16)

// Parameters for testAccumulatedCreateData
#define NUMBER_OF_HANDLES 10000

/**
 * Run a loop NUMBER_OF_ALLOCS times:
 *   maCreatePlaceholder()
 *   maCreateData(CHUNK_SIZE)
 *   maDestroyObject() if destroyObject == true
 *   maDestroyPlaceholder() if destroyPlaceholder == true
 */
void testCreateData(bool destroyObject, bool destroyPlaceholder)
{
	bool success = true;

	for (int i = 0; i < NUMBER_OF_ALLOCS; ++i)
	{
		if (0 == (i % 10000))
		{
			printf("testCreateData: i = %d\n", i);
		}

		int handle = maCreatePlaceholder();
		if (handle < 0)
		{
			printf("testCreateData: maCreatePlaceholder failed\n");
			success = false;
			break;
		}

		int result = maCreateData(handle, CHUNK_SIZE);
		if (result < 0)
		{
			printf("testCreateData: maCreateData failed\n");
			success = false;
			break;
		}

		if (destroyObject)
		{
			maDestroyObject(handle);
		}

		if (destroyPlaceholder)
		{
			result = maDestroyPlaceholder(handle);
			if (result < 0)
			{
				printf("testCreateData: maDestroyPlaceholder failed\n");
				success = false;
				break;
			}
		}
	}

	if (success)
	{
		printf("Test passed\n");
	}
	else
	{
		printf("Test failed\n");
	}
}

/**
 * Test that allocates a large number of objects, then releases them.
 */
void testAccumulatedCreateData()
{
	bool success = true;

	MAHandle handles[NUMBER_OF_HANDLES];

	// Allocate NUMBER_OF_HANDLES objects.
	for (int i = 0; i < NUMBER_OF_HANDLES; ++i)
	{
		if (0 == (i % 1000))
		{
			printf("testAccumulatedCreateData: i = %d\n", i);
		}

		int handle = maCreatePlaceholder();
		if (handle < 0)
		{
			printf("testAccumulatedCreateData: maCreatePlaceholder failed\n");
			success = false;
			break;
		}

		int result = maCreateData(handle, 1024);
		if (result < 0)
		{
			printf("testAccumulatedCreateData: maCreateData failed\n");
			success = false;
			break;
		}

		handles[i] = handle;
	}

	// Deallocate the objects.
	for (int i = 0; i < NUMBER_OF_HANDLES; ++i)
	{
		if (0 == (i % 1000))
		{
			printf("testAccumulatedCreateData: i = %d\n", i);
		}

		int handle = handles[i];
		int result = maDestroyPlaceholder(handle);
		if (result < 0)
		{
			printf("testCreateData: maDestroyPlaceholder failed\n");
			success = false;
			break;
		}
	}

	if (success)
	{
		printf("Test passed\n");
	}
	else
	{
		printf("Test failed\n");
	}
}

/**
 * Negative tests.
 */
void testNegativeCases()
{
	int result;

	// Comment out to test panics.
	maSyscallPanicsDisable();

	MAHandle h = maCreatePlaceholder();
	if (h < 0)
	{
		maPanic(0, "testNegativeCases: maCreatePlaceholder should not fail");
	}

	result = maDestroyPlaceholder(h);
	if (RES_OK != result)
	{
		maPanic(0, "testNegativeCases: maDestroyPlaceholder should not fail");
	}

	result = maDestroyPlaceholder(h);
	if (RES_OK == result)
	{
		maPanic(0, "testNegativeCases: maDestroyPlaceholder should fail (1)");
	}

	result = maDestroyPlaceholder(1000);
	if (RES_OK == result)
	{
		maPanic(0, "testNegativeCases: maDestroyPlaceholder should fail (2)");
	}

	maSyscallPanicsEnable();

	printf("Negative tests passed\n");
}

/**
 * This test is expected to make the app run out of
 * memory, and crash on Android at least. Perhaps
 * on some other platforms it exits gracefully?
 */
void testThatShouldCrashApp()
{
	testCreateData(false, false);
	printf("Expected app to crash, test failed\n");
}

/**
 * Test that should pass.
 */
void testThatShouldPassSuccessfully()
{
	printf("Test 1: Negative tests\n");
	testNegativeCases();

	printf("Test 2: Call maDestroyPlaceholder on accumulated data\n");
	testAccumulatedCreateData();

	printf("Test 3: Call maDestroyObject and maDestroyPlaceholder\n");
	testCreateData(true, true);

	printf("Test 4: Call only maDestroyPlaceholder\n");
	testCreateData(false, true);
}

/**
 * Program that does a simple database test.
 */
extern "C" int MAMain()
{
	MAEvent event;

	printf(
		"Press 1 or tap UPPER part of screen\n"
		"to run test that SHOULD CRASH the app\n");
	printf(
		"Press 2 or tap LOWER part of screen\n"
		"to run test that SHOULD PASS\n");
	printf("Press zero or back to exit\n");

	// Enable panics if not enabled by default.
	maSyscallPanicsEnable();

	while (true)
	{
		maWait(1000);
		maGetEvent(&event);

		if (EVENT_TYPE_CLOSE == event.type)
		{
			break;
		}
		else if (EVENT_TYPE_KEY_PRESSED == event.type)
		{
			if (MAK_BACK == event.key || MAK_0 == event.key)
			{
				return 0;
			}
			else if (MAK_1 == event.key)
			{
				testThatShouldCrashApp();
			}
			else if (MAK_2 == event.key)
			{
				testThatShouldPassSuccessfully();
			}
		}
		else if (EVENT_TYPE_POINTER_PRESSED == event.type)
		{
			int halfHeight = EXTENT_Y(maGetScrSize()) / 2;
			if (event.point.y < halfHeight)
			{
				testThatShouldCrashApp();
			}
			else
			{
				testThatShouldPassSuccessfully();
			}
		}
	}

	return 0;
}
