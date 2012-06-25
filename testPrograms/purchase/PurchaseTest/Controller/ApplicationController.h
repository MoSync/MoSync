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
