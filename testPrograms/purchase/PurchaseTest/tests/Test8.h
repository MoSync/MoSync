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
 * @file Test8.h
 * @author Bogdan Iusco
 * @date 15 May 2012
 *
 * @brief Create two products using the same product handle.
 * Expected result: the second product should receive an purchase
 * event with MA_PURCHASE_STATE_DUPLICATE_HANDLE as state.
 */

#ifndef PURCHASE_TEST8_H_
#define PURCHASE_TEST8_H_

#include <MAUtil/Environment.h>

#include "ITest.h"
#include "../Controller/IApplicationController.h"


using namespace MAUtil;

namespace PurchaseTest
{

	/**
	 * @brief Test duplicate handle for a product.
	 * Platform: Android and iOS.
	 */
	class Test8:
		public ITest,
		public CustomEventListener
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test8(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test8();

		/**
		 * Start the test.
		 */
		virtual void startTest();

		/**
		 * Get the name of the test.
		 * @return Test's name.
		 */
		virtual MAUtil::String getTestName() const;

	private:
		/**
		 * Handle a new event.
		 * @param event The new event.
		 */
		virtual void customEvent(const MAEvent& event);

	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;

		/**
		 * Handle to the first product.
		 */
		MAHandle mFirstProduct;

		/**
		 * Handle to the second product.
		 * This product should be invalid because it has the same handle as the
		 * first one.
		 */
		MAHandle mSecondProduct;

		/**
		 * Flag to indicate if the first product was created.
		 */
		bool mIsFirstProductCreated;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST8_H_ */
