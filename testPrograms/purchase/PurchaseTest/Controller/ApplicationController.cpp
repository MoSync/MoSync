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
#include <Purchase/PurchaseManager.h>

#include "ApplicationController.h"
#include "../View/MainScreen.h"
#include "../Database/DatabaseManager.h"
#include "../Database/DatabaseProduct.h"

#include "../tests/ITest.h"
#include "../tests/Test1.h"
#include "../tests/Test2.h"
#include "../tests/Test3.h"
#include "../tests/Test4.h"
#include "../tests/Test5.h"
#include "../tests/Test6.h"
#include "../tests/Test7.h"
#include "../tests/Test8.h"
#include "../tests/Test9.h"
#include "../tests/Test10.h"
#include "../tests/Test11.h"

#include "../Util.h"
#include "Config.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 */
	ApplicationController::ApplicationController() :
		mMainScreen(NULL),
		mDatabase(NULL),
		mCountSucceededTests(0)
	{
		mMainScreen = new MainScreen();
		mMainScreen->show();

		this->log("Application started!");

		mDatabase = new DatabaseManager();
		DatabaseProduct* dbProduct = new DatabaseProduct();

		int platform = getPlatform();
		if (platform != IOS &&
			platform != ANDROID)
		{
			maAlert("Error", "This program runs only on Android and iOS devices",
				"OK", NULL, NULL);
		}
		else if ( PurchaseManager::getInstance()->checkPurchaseSupported() != MA_PURCHASE_RES_OK )
		{
			maAlert("Error", "Billing is not supported on this device",
				"OK", NULL, NULL);
		}
		else
		{
			MAUtil::String developerKey = DEVELOPER_PUBLIC_KEY;
			if ( developerKey.size() == 0 &&
				 platform == ANDROID )
			{
				maAlert("Error", "You need to set developer key in Config.h ",
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
	}

	/**
	 * Destructor.
	 */
	ApplicationController::~ApplicationController()
	{
		delete mMainScreen;
		delete mDatabase;

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
		this->log(test.getReason());
		this->log("Test failed!");
		this->log("================================");
		MAUtil::String* testName = new MAUtil::String(test.getTestName());
		mFailedTests.add(testName);

		ITest* testAux = mTests[0];
		mTests.remove(0);
		delete testAux;

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

		ITest* testAux = mTests[0];
		mTests.remove(0);
		delete testAux;

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
	 * Get the interface to database.
	 * @return The database's interface.
	 */
	IDatabaseManager& ApplicationController::getDatabase() const
	{
		return *mDatabase;
	}

	/**
	 * Creates test objects and stores them into an array.
	 */
	void ApplicationController::createTests()
	{
		mTests.add(new Test1(*this));
		// Test2 will always succeed on iOS.
		mTests.add(new Test2(*this));
		mTests.add(new Test3(*this));
		mTests.add(new Test4(*this));
		mTests.add(new Test5(*this));
		mTests.add(new Test6(*this));
		mTests.add(new Test7(*this));
		mTests.add(new Test8(*this));
		mTests.add(new Test9(*this));
		mTests.add(new Test10(*this));
		mTests.add(new Test11(*this));
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
		sprintf(buffer, "Started %s", test->getTestName().c_str());
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
		sprintf(buffer, "%d test(s) succeeded!", mCountSucceededTests);
		this->log(buffer);

		int countFailedTests = mFailedTests.size();
		if (countFailedTests > 0)
		{
			this->log("The tests that failed are:");
			for(int i = 0; i < countFailedTests; i++)
			{
				this->log(*(mFailedTests[i]));
			}
		}
	}

} // namespace PurchaseTest
