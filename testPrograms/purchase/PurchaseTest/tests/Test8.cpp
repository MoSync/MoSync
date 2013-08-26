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
 * @file Test8.cpp
 * @author Bogdan Iusco
 * @date 15 May 2012
 *
 * @brief Create two products using the same product handle.
 * Expected result: the second product should receive an purchase
 * event with MA_PURCHASE_STATE_DUPLICATE_HANDLE as state.
 */

#define TEST_NAME "Test 8"

#include <conprint.h>
#include <maapi.h>

#include "Test8.h"
#include "../Util.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test8::Test8(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mFirstProduct(1),
		mSecondProduct(mFirstProduct),
		mIsFirstProductCreated(false)
	{

	}

	/**
	 * Destructor.
	 */
	Test8::~Test8()
	{
		Environment::getEnvironment().removeCustomEventListener(this);
	}

	/**
	 * Start the test.
	 */
	void Test8::startTest()
	{
		String productType = ProductTypes::getInstance().getProductType1();
		Environment::getEnvironment().addCustomEventListener(this);
		maPurchaseCreate(mFirstProduct, productType.c_str());
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test8::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Handle a new event.
	 * @param event The new event.
	 */
	void Test8::customEvent(const MAEvent& event)
	{
		if (event.type == EVENT_TYPE_PURCHASE)
		{
			MAPurchaseEventData purchaseData = event.purchaseData;

			// Check if first product was created.
			if (!mIsFirstProductCreated)
			{
				// Create second product.
				mIsFirstProductCreated = true;
				String productType = ProductTypes::getInstance().getProductType1();
				maPurchaseCreate(mSecondProduct, productType.c_str());
				return;
			}

			if (purchaseData.state == MA_PURCHASE_STATE_DUPLICATE_HANDLE)
			{
				mApplicationController.testSucceeded(*this);
			}
			else
			{
				this->setFailedReason(
					"Did not receive MA_PURCHASE_STATE_DUPLICATE_HANDLE");
				mApplicationController.testFailed(*this);
			}
		}
	}

}
