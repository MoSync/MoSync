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
