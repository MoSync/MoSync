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
 * @file IApplicationController.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Interface for application controller.
 */

#ifndef PURCHASE_IAPPLICATION_CONTROLLER_H_
#define PURCHASE_IAPPLICATION_CONTROLLER_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	// Forward declaration
	class ITest;
	class IDatabaseManager;

	/**
	 * Interface for controller.
	 */
	class IApplicationController
	{
	public:
		/**
		 * Notify controller that a test has failed.
		 * @param test Test that failed.
		 */
		virtual void testFailed(ITest& test) = 0;

		/**
		 * Notify controller that a test has successfully finished.
		 * @param test That that has finished.
		 */
		virtual void testSucceeded(ITest& test) = 0;

		/**
		 * Print log on the screen.
		 * @param test Text to be printed.
		 */
		virtual void log(const MAUtil::String& test) = 0;

		/**
		 * Get the interface to database.
		 * @return The database's interface.
		 */
		virtual IDatabaseManager& getDatabase() const = 0;

	};

} // end of PurchaseApp

#endif /* PURCHASE_IAPPLICATION_CONTROLLER_H_ */
