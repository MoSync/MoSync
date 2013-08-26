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
 * @file Test5.cpp
 * @author emma
 *
 * @brief Test case 5: VerifyReceipt for a product that was not bought.
 * Create new purchase object. Call verifyReceipt without previously
 * buying the product. Expected result:
 *	- event receiptError, errorCode = MA_PURCHASE_ERROR_NO_RECEIPT.
 *
 */
#include <Purchase/PurchaseManager.h>
#include <MAUtil/util.h>

#include "Test5.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test5::Test5(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test5::~Test5()
	{
		mPurchase->removePurchaseListener(this);
	}

	/**
	 * Start the test.
	 */
	void Test5::startTest()
	{
		MAUtil::String productType = ProductTypes::getInstance().getProductType2();
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test5::getTestName() const
	{
		return "Test 5";
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
	void Test5::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			MAUtil::String info = "Test5 in progress...";
			mApplicationController.log(info);
			mPurchase->verifyReceipt();
		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test5::productInvalid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			MAUtil::String info = "Product " +
					MAUtil::integerToString(purchase.getHandle()) + " is invalid";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test5::requestCompleted(const Purchase& purchase)
	{
	}

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	void Test5::receiptValid(
		const Purchase& purchase,
		Receipt& receipt)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			MAUtil::String info = "Receipt is valid";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test5::receiptInvalid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			MAUtil::String info = "Receipt was invalidated instead of returning an error";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test5::receiptError(const Purchase& purchase,
		const int errorCode)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			if (errorCode == MA_PURCHASE_ERROR_NO_RECEIPT )
			{
				mApplicationController.testSucceeded(*this);
			}
			else
			{
				MAUtil::String info = "Receipt failed with another code";
				mApplicationController.log(info);
				this->setFailedReason(info);
				mApplicationController.testFailed(*this);
			}
		}
	}
}
