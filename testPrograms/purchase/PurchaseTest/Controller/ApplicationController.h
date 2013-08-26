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
 * @file ApplicationController.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Application's controller.
 */

#ifndef PURCHASE_APPLICATION_CONTROLLER_H_
#define PURCHASE_APPLICATION_CONTROLLER_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#include "IApplicationController.h"

namespace PurchaseTest
{

	// Forward declarations
	class MainScreen;
	class ITest;
	class DatabaseManager;

	/**
	 * @brief Create main screen and tests.
	 */
	class ApplicationController:
		public IApplicationController
	{
	public:
		/**
		 * Constructor.
		 */
		ApplicationController();

		/**
		 * Destructor.
		 */
		virtual ~ApplicationController();

		/**
		 * Notify controller that a test has failed.
		 * @param test Test that failed.
		 */
		virtual void testFailed(ITest& test);

		/**
		 * Notify controller that a test has successfully finished.
		 * @param test That that has finished.
		 */
		virtual void testSucceeded(ITest& test);

		/**
		 * Print log on the screen.
		 * @param text Text to be printed.
		 */
		virtual void log(const MAUtil::String& text);

		/**
		 * Get the interface to database.
		 * @return The database's interface.
		 */
		virtual IDatabaseManager& getDatabase() const;

	private:
		/**
		 * Creates test objects and stores them into an array.
		 */
		void createTests();

		/**
		 * Run the next text from the array.
		 */
		void runNextTest();

		/**
		 * Show tests results.
		 * Call this method after all test have been finished.
		 */
		void finishTesting();

	private:
		/**
		 * Main screen.
		 * Displays logs.
		 */
		MainScreen* mMainScreen;

		/**
		 * Database manager.
		 */
		DatabaseManager* mDatabase;

		/**
		 * Stores test objects.
		 */
		MAUtil::Vector<ITest*> mTests;

		/**
		 * Store the name of the failed tests;
		 */
		MAUtil::Vector<MAUtil::String*> mFailedTests;

		/**
		 * Count the number of succeeded tests.
		 */
		int mCountSucceededTests;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_APPLICATION_CONTROLLER_H_ */
