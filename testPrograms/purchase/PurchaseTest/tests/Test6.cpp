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
 * @file Test6.cpp
 * @author emma
 *
 * @brief Test case 6: VerifyReceipt for an item that cannot be purchased.
 * IOS:
 *	1. Create new purchase object.
 * 	2. Validation fails, fails because the item is not available: receive event productInvalid.
 * 	3. Call verifyReceipt and check the error event.
 * 	Expected event: receiptError, with errorCode = MA_PURCHASE_ERROR_NO_RECEIPT.
 * ANDROID:
 *	1. Create new purchase object.
 * 	2. Try to buy the item, purchase fails because the item is not available: receive
 * 	event requestFailed with error code MA_PURCHASE_ERROR_INVALID_PRODUCT.
 * 	3. Call verifyReceipt and check the error event.
 * 	Expected event: receiptError, with errorCode = MA_PURCHASE_ERROR_NO_RECEIPT.
 */

#include <Purchase/PurchaseManager.h>
#include <MAUtil/util.h>

#include "Test6.h"


namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test6::Test6(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test6::~Test6()
	{
		mPurchase->removePurchaseListener(this);
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test6::startTest()
	{
		MAUtil::String productType = ProductTypes::getInstance().getProductTypeUnavailable();
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test6::getTestName() const
	{
		return "Test 6";
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
	void Test6::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			if ( getPlatform() == ANDROID )
			{
				mPurchase->verifyReceipt();
			}
			else
			{
				MAUtil::String info = "Product " +
						MAUtil::integerToString(purchase.getHandle()) + " is validated on iOS";
				mApplicationController.log(info);
				this->setFailedReason(info);
				mApplicationController.testFailed(*this);
			}

		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test6::productInvalid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			if ( getPlatform() == IOS )
			{
				mPurchase->verifyReceipt();
			}
			else
			{
				MAUtil::String info = "Product " +
						MAUtil::integerToString(purchase.getHandle()) + " was invalidated on Android";
				mApplicationController.log(info);
				this->setFailedReason(info);
				mApplicationController.testFailed(*this);
			}

		}
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test6::requestCompleted(const Purchase& purchase)
	{
	}

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	void Test6::receiptValid(
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
	void Test6::receiptInvalid(const Purchase& purchase)
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
	void Test6::receiptError(const Purchase& purchase,
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
