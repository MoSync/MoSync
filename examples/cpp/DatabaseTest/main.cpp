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
