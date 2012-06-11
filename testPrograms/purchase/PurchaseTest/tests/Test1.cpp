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
 * @file Test1.cpp
 * @author emma
 *
 * @brief  The user tries to buy a product, but does not set the public key.
 * Expected result:
 * - Android: maPurchaseRequest fails with errorCode = MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET.
 * - iOS: always succeeds.
 */

#include <MAUtil/util.h>
#include <Purchase/PurchaseManager.h>

#include "Test1.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test1::Test1(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mPurchase(NULL)
	{
	}

	/**
	 * Destructor.
	 */
	Test1::~Test1()
	{
		if ( mPurchase )
		{
			mPurchase->removePurchaseListener(this);
			delete mPurchase;
		}
	}

	/**
	 * Start the test.
	 */
	void Test1::startTest()
	{
		if ( getPlatform() == ANDROID )
		{
			MAUtil::String productType = ProductTypes::getInstance().getProductType1();
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
	MAUtil::String Test1::getTestName() const
	{
		return "Test 1";
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
	void Test1::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle()
				&& getPlatform() == ANDROID)
		{
			mPurchase->requestPurchase();
		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test1::productInvalid(const Purchase& purchase)
	{
		if ( mPurchase->getHandle() == purchase.getHandle() )
		{
			MAUtil::String info = "Product " +
					MAUtil::integerToString(purchase.getHandle()) + " is invalid";
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
	void Test1::requestInProgress(const Purchase& purchase)
	{
		if ( mPurchase->getHandle() == purchase.getHandle() )
		{
			MAUtil::String info = "Product " +
					MAUtil::integerToString(purchase.getHandle()) + " failed, product is in progress";
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
	void Test1::requestCompleted(const Purchase& purchase)
	{
		if ( mPurchase->getHandle() == purchase.getHandle() )
		{
			MAUtil::String info = "Product " +
					MAUtil::integerToString(purchase.getHandle()) + "was purchased without public key";
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
	void Test1::requestFailed(const Purchase& purchase,
		const int errorCode)
	{
		if ( mPurchase->getHandle() == purchase.getHandle()
				&& getPlatform() == ANDROID
				&& errorCode == MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET )
		{
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			MAUtil::String info = "Purchase failed for different reason";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}
}
