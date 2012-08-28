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
 * @file Test11.cpp
 * @author emma
 *
 * @brief Test case 11: Test Refunded product on Android.
 *
 */

#include <Purchase/PurchaseManager.h>
#include <Purchase/Receipt.h>
#include <MAUtil/util.h>

#include "Test11.h"

#define PACKAGE_NAME "com.mosync.app_PurchaseTest"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test11::Test11(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test11::~Test11()
	{
		mPurchase->removePurchaseListener(this);
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test11::startTest()
	{
		if ( getPlatform() == ANDROID )
		{
			PurchaseManager::getInstance()->addPurchaseListener(this);
			MAUtil::String productType = ProductTypes::getInstance().getProductTypeRefunded();
			mPurchase = new Purchase(productType, this);
		}
		else
		{
			mApplicationController.testSucceeded(*this);
		}
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test11::getTestName() const
	{
		return "Test 11";
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
	void Test11::productValid(const Purchase& purchase)
	{
		mApplicationController.log("Receiving refund...");
		mPurchase->addPurchaseListener(this);
		mPurchase->requestPurchase();
	}

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test11::requestInProgress(const Purchase& purchase)
	{
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test11::requestCompleted(const Purchase& purchase)
	{
		mApplicationController.log("Request in progress...");
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
	void Test11::requestFailed(const Purchase& purchase,
		const int errorCode)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product was not refunded. Error code: %d",
				mPurchase->getProductId().c_str(), errorCode);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that a purchase has been refunded.
	 * Platform: Android.
	 * @param purchase The purchase that has been refunded.
	 */
	void Test11::purchaseRefunded(Purchase& purchase)
	{
		mApplicationController.log("Product was refunded.Parsing receipt..");
		purchase.addPurchaseListener(this);
		purchase.verifyReceipt();
	}

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	void Test11::receiptValid(
		const Purchase& purchase,
		Receipt& receipt)
	{
		if ( receipt.getProductID() == ProductTypes::getInstance().getProductTypeRefunded()
			&&
			receipt.getAppID() == PACKAGE_NAME)
		{
			mApplicationController.log("Receipt valid");
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			receiptInvalid(purchase);
		}
	}

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test11::receiptInvalid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Receipt for refunded product is invalid.");
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test11::receiptError(const Purchase& purchase,
		const int errorCode)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Receipt error. Error code: %d", errorCode);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}
}
