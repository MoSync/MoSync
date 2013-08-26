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
 * @file Test3.cpp
 * @author emma
 *
 * @brief The user tries to buy an unavailable product using the wrapper.
 * ( The public key is set on Android, and checkBillingSupported was called).
 * Expected result:
 * - MA_PURCHASE_RES_UNAVAILABLE on devices with no billing support.
 * - iOS: maPurchaseCreate fails with MA_PURCHASE_STATE_PRODUCT_INVALID.
 * - Android: maPurchaseRequest fails with event type MA_PURCHASE_STATE_FAILED
 * and errorCode = MA_PURCHASE_ERROR_INVALID_PRODUCT.
 *
 */
#include <Purchase/PurchaseManager.h>
#include <MAUtil/util.h>

#include "Test3.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test3::Test3(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mPurchase(NULL)
	{
	}

	/**
	 * Destructor.
	 */
	Test3::~Test3()
	{
		mPurchase->removePurchaseListener(this);
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test3::startTest()
	{
		MAUtil::String productType = ProductTypes::getInstance().getProductTypeUnavailable();
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test3::getTestName() const
	{
		return "Test3";
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
	void Test3::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle()
				&& getPlatform() == ANDROID)
		{
			mPurchase->requestPurchase();
		}
		else
		{
			MAUtil::String info = "Test failed on iOS, product "+
					MAUtil::integerToString(purchase.getHandle()) + " handled as valid";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test3::productInvalid(const Purchase& purchase)
	{
		//TODO to see if no event received, test failed, otherwise it hangs.
		if ( purchase.getHandle() == mPurchase->getHandle()
				&& getPlatform() == IOS)
		{
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			MAUtil::String info = "Android product "+
					MAUtil::integerToString(purchase.getHandle()) + " is invalid.";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test3::requestInProgress(const Purchase& purchase)
	{
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test3::requestCompleted(const Purchase& purchase)
	{
		if ( mPurchase->getHandle() == purchase.getHandle() )
		{
			MAUtil::String info = "Unavailable product " +
					MAUtil::integerToString(purchase.getHandle()) + " was purchased";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the transaction has failed.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 * Note that even if the request fails because it was canceled
	 * (errorCode = MA_PURCHASE_ERROR_CANCELLED), you will still be
	 * able to get a receipt for your purchase.
	 */
	void Test3::requestFailed(const Purchase& purchase,
		const int errorCode)
	{
		if ( mPurchase->getHandle() == purchase.getHandle()
				&& getPlatform() == ANDROID
				&& errorCode == MA_PURCHASE_ERROR_INVALID_PRODUCT )
		{
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			MAUtil::String info = "Purchase " +
					MAUtil::integerToString(purchase.getHandle()) + "failed for different reason";
			mApplicationController.log(info);
			this-> setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}
}
