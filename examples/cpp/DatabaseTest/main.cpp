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

/**
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * This is a test program that illustrates how to use the SQL API.
 *
 * The program exits with a panic message if a test fails.
 *
 * Touch the screen to start running the test. A database file will
 * be created on the device, in the local file system of the application.
 *
 * If the test is successful the text "Database test passed successfully"
 * is displayed at the end of the output.
 *
 * If you run in MoRE and output text strings are truncated, select
 * a device profile with a bigger screen, then run again.
 */


#include <conprint.h>

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>

#include "dbtest.h"

using namespace MAUtil;

/**
 * Moblet that runs the database tests.
 */
class DBTestMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	DBTestMoblet()
	{
		printf("Database test\n");

		DBTest test;
		test.runTest();
		test.testParameters();

		//printf("Touch screen to run test 1000 times\n");
		printf("Press zero or back (on Android) to exit\n");
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Closes the application.
			close();
		}
	}

	/**
	 * Called when the screen is touched.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		close();
//		DBTest test;
//		for (int i = 0; i < 1000; ++i)
//		{
//			test.runTest();
//			test.testParameters();
//		}
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new DBTestMoblet());
	return 0;
}
