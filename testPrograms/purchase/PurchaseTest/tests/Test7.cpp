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
 * @file Test7.cpp
 * @author emma
 *
 * @brief Test case 7:  Test checkPurchaseSupported.
 * Expected result: call returns MA_PURCHASE_RES_OK.
 */

#include <Purchase/PurchaseManager.h>

#include "Test7.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test7::Test7(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test7::~Test7()
	{
	}

	/**
	 * Start the test.
	 */
	void Test7::startTest()
	{
		int result = PurchaseManager::getInstance()->checkPurchaseSupported();
		if ( result == MA_PURCHASE_RES_OK )
		{
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			MAUtil::String info = "Billing is not supported.";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test7::getTestName() const
	{
		return "Test 7";
	}

}
