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
 * @file Test2.cpp
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

#include "Test2.h"
#include "../wrapper/PurchaseManager.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test2::Test2(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
		PurchaseManager::getInstance()->setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkq5y2a2tbd9xjNfBwtxt1G/wKILzrHSWp3uAwQ/Iw2SkAF72cuoLUiliTkU6IXVhD95k1VsZRLQytKOIAmPOI/faGxbZovmfIq2u/F/Gd39AyUg2geLdhV+wMrgdFWwWiPwOXl+12zigOvJdAfsxNMLh0dhJW1RAK5OaT0TXkR5d26GiUYm6kt0SW4+FX1OSiavwE4NTPTtwqsjscWxBKXvICm0A+0OYtqv6wTC3uhVMDkTnrsVG9N4pTKSGmhz//dfqJIZKI56UD21I2hjsB+8XVxy0KV741x3NLBOruB5oGGlXbCQGHTu3ekY4g5jlusCxlD19327e7LxrooM9GQIDAQAB");
	}

	/**
	 * Destructor.
	 */
	Test2::~Test2()
	{
		mPurchase->removePurchaseListener(this);
	}

	/**
	 * Start the test.
	 */
	void Test2::startTest()
	{
		if ( getPlatform() == ANDROID )
		{
			mPurchase = new Purchase(ANDROID_UNAVAILABLE_PRODUCT_ID, this);
			mPurchase->addPurchaseListener(this);
		}
		else if ( getPlatform() == IOS )
		{
			mPurchase = new Purchase(IOS_UNAVAILABLE_PRODUCT_ID, this);
		}
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test2::getTestName() const
	{
		return "Test the purchase of an unavailable product";
	}

	/**
	 * Get the reason why the test failed.
	 * @return Reason why it failed.
	 */
	MAUtil::String Test2::getReason()
	{
		return "";
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
	void Test2::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle()
				&& getPlatform() == ANDROID)
		{
			mPurchase->requestPurchase();
		}
		else
		{
			mApplicationController.testFailed(*this);
			MAUtil::String info = "Test2 failed on iOS, product handled as valid";
			mApplicationController.log(info);
		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::productInvalid(const Purchase& purchase)
	{
		//TODO to see if no event received, test failed, otherwise it hangs.
		if ( purchase.getHandle() == mPurchase->getHandle()
				&& getPlatform() == IOS)
		{
			mApplicationController.testSucceeded(*this);
			MAUtil::String info = "Test2 succeeded";
			mApplicationController.log(info);
		}
		else
		{
			mApplicationController.testFailed(*this);
			MAUtil::String info = "Test2 failed, product invalid";
			mApplicationController.log(info);
		}
	}

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::requestInProgress(const Purchase& purchase)
	{
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::requestCompleted(const Purchase& purchase)
	{
		if ( mPurchase->getHandle() == purchase.getHandle() )
		{
			mApplicationController.testFailed(*this);
			MAUtil::String info = "Test2 failed, Unavailable product was purchased";
			mApplicationController.log(info);
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
	void Test2::requestFailed(const Purchase& purchase,
		const int errorCode)
	{
		if ( mPurchase->getHandle() == purchase.getHandle()
				&& getPlatform() == ANDROID
				&& errorCode == MA_PURCHASE_ERROR_INVALID_PRODUCT )
		{
			mApplicationController.testSucceeded(*this);
			MAUtil::String info = "Test2 succeeded";
			mApplicationController.log(info);
		}
		else
		{
			mApplicationController.testFailed(*this);
			MAUtil::String info = "Test2 failed, purchase failed for different reason";
			mApplicationController.log(info);
		}
	}
}
