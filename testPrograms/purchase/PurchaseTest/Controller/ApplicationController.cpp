/*
 Copyright (C) 2012 MoSync AB

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
 * @file ApplicationController.cpp
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Application's controller.
 */

#include <maapi.h>

#include "ApplicationController.h"
#include "../View/MainScreen.h"

#include "../tests/ITest.h"
#include "../tests/Test1.h"
#include "../tests/Test2.h"
#include "../tests/Test3.h"
#include "../tests/Test4.h"
#include "../tests/Test5.h"
#include "../tests/Test6.h"
#include "../tests/Test7.h"
#include "../Util.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 */
	ApplicationController::ApplicationController() :
		mMainScreen(NULL),
		mCountSucceededTests(0)
	{
		mMainScreen = new MainScreen();
		mMainScreen->show();

		this->log("Application started!");

		//TODO check platform version also.
//		maGetSystemProperty("mosync.device.OS.version")
		int platform = getPlatform();
		if (platform != IOS &&
			platform != ANDROID)
		{
			maAlert("Error", "This program runs only on Android and iOS devices",
				"OK", NULL, NULL);
		}
		else
		{
			this->log("Creating tests...");
			this->createTests();
			this->log("Tests are created!");
			this->runNextTest();
		}
	}

	/**
	 * Destructor.
	 */
	ApplicationController::~ApplicationController()
	{
		delete mMainScreen;

		// Delete any remaining tests.
		while (mTests.size() > 0)
		{
			ITest* test = mTests[0];
			mTests.remove(0);
			delete test;
		}

		// Delete failed test names.
		while (mFailedTests.size() > 0)
		{
			MAUtil::String* testName = mFailedTests[0];
			mFailedTests.remove(0);
			delete testName;
		}
	}

	/**
	 * Notify controller that a test has failed.
	 * @param test Test that failed.
	 */
	void ApplicationController::testFailed(ITest& test)
	{
		this->log("Test failed!");
		this->log("================================");
		MAUtil::String* testName = new MAUtil::String(test.getTestName());
		mFailedTests.add(testName);
		mTests.remove(0);
		this->runNextTest();
	}

	/**
	 * Notify controller that a test has successfully finished.
	 * @param test That that has finished.
	 */
	void ApplicationController::testSucceeded(ITest& test)
	{
		this->log("Test succeeded!");
		this->log("================================");
		mCountSucceededTests++;
		mTests.remove(0);
		this->runNextTest();
	}

	/**
	 * Print log on the screen.
	 * @param text Text to be printed.
	 */
	void ApplicationController::log(const MAUtil::String& text)
	{
		mMainScreen->printText(text);
	}

	/**
	 * Creates test objects and stores them into an array.
	 */
	void ApplicationController::createTests()
	{
		//mTests.add(new Test1(*this));

		if ( getPlatform() == ANDROID )
		{
			mTests.add(new Test2(*this));
		}
		mTests.add(new Test3(*this));
		mTests.add(new Test4(*this));
		mTests.add(new Test5(*this));
		mTests.add(new Test6(*this));
		mTests.add(new Test7(*this));

	}

	/**
	 * Run the next text from the array.
	 */
	void ApplicationController::runNextTest()
	{
		if (mTests.size() == 0)
		{
			this->finishTesting();
			return;
		}
		this->log("================================");
		ITest* test = mTests[0];
		char buffer[BUF_MAX];
		sprintf(buffer, "Started test %s", test->getTestName().c_str());
		this->log(buffer);
		test->startTest();
	}

	/**
	 * Show tests results.
	 * Call this method after all test have been finished.
	 */
	void ApplicationController::finishTesting()
	{
		this->log("All tests have been completed!");

		char buffer[512];
		sprintf(buffer, "%d test succeeded!", mCountSucceededTests);
		this->log(buffer);

		this->log("The tests that failed are:");
		for(int i = 0; i < mFailedTests.size(); i++)
		{
			this->log(*(mFailedTests[i]));
		}
	}

} // namespace PurchaseTest
