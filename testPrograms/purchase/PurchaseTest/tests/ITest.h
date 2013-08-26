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
 * @file ITest.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Base class for test classes.
 */

#ifndef PURCHASE_ITEST_H_
#define PURCHASE_ITEST_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	/**
	 * @brief Base class for test classes.
	 */
	class ITest
	{
	public:

		/**
		 * Constructor.
		 */
		ITest();

		/**
		 * Start the test.
		 */
		virtual void startTest() = 0;

		/**
		 * Get the name of the test.
		 * @return Test's name.
		 */
		virtual MAUtil::String getTestName() const = 0;

		/**
		 * Get the reason why the test failed.
		 * @return Reason why it failed.
		 * If the test has not finished or if it succeeded the returned string
		 * will be empty.
		 */
		virtual MAUtil::String getReason();

		/**
		 * Destructor.
		 */
		virtual ~ITest();

	protected:
		/**
		 * Set the reason why the test failed.
		 * @param reason The reason why it failed.
		 */
		void setFailedReason(const MAUtil::String& reason);

	private:
		/**
		 * In case the test fails this string will contain the reason.
		 * If the test has not started or if it succeeded this string will be
		 * NULL.
		 */
		MAUtil::String* mFailedReason;
	};
} // namespace PurchaseTest


#endif /* PURCHASE_ITEST_H_ */
