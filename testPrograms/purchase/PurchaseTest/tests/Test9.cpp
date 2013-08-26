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
 * @file Test9.cpp
 * @author Bogdan Iusco
 * @date 22 May 2012
 *
 * @brief Test maPurchaseGetName() syscall with a small buffer size.
 * MA_PURCHASE_RES_BUFFER_TOO_SMALL error should be returned.
 */

#define TEST_NAME "Test 9"

#include <Purchase/Purchase.h>

#include "Test9.h"
#include "../Util.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test9::Test9(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mPurchase(NULL)
	{

	}

	/**
	 * Destructor.
	 */
	Test9::~Test9()
	{
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test9::startTest()
	{
		MAUtil::String productID = ProductTypes::getInstance().getProductType1();
		mPurchase = new Purchase(productID, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test9::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Notifies that the product has been validated by the App Store.
	 * NOTE: On Android there is no validation done at this step, if the
	 * product results to be unavailable, then the application will be later
	 * notified via a requestFailed() callback. The errorCode will equal
	 * #MA_PURCHASE_ERROR_INVALID_PRODUCT.
	 * Platform: iOS and Android.
	 * @param purchase The object that sent the event.
	 */
	void Test9::productValid(const Purchase& purchase)
	{
		this->testSyscall();
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test9::productInvalid(const Purchase& purchase)
	{
		this->testSyscall();
	}

	/**
	 * Test maPurchaseGetName() syscall.
	 * Expected result code: MA_PURCHASE_RES_BUFFER_TOO_SMALL
	 */
	void Test9::testSyscall()
	{
		char buffer[2];
		int result = maPurchaseGetName(mPurchase->getHandle(), buffer, 2);
		if (result != MA_PURCHASE_RES_BUFFER_TOO_SMALL)
		{
			char bigBuffer[BUF_MAX];
			sprintf(bigBuffer, "Result code %d received instead of "
				"MA_PURCHASE_RES_BUFFER_TOO_SMALL(%d)",
				result,
				MA_PURCHASE_RES_BUFFER_TOO_SMALL);
			this->setFailedReason(bigBuffer);
			mApplicationController.testFailed(*this);
		}
		else
		{
			mApplicationController.testSucceeded(*this);
		}
	}
}
