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
 * @author Bogdan Iusco
 * @date 9 May 2012
 *
 * @brief Test a valid purchase of an product.
 */

#define TEST_NAME "Test 2"

#include <conprint.h>

#include "Test2.h"
#include "../Util.h"

#include "../wrapper/Purchase.h"
#include "../wrapper/PurchaseManager.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test2::Test2(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mPurchase(NULL)
	{

	}

	/**
	 * Destructor.
	 */
	Test2::~Test2()
	{
	if (mPurchase)
		{
			mPurchase->removePurchaseListener(this);
			delete mPurchase;
		}
	}

	/**
	 * Start the test.
	 */
	void Test2::startTest()
	{
		MAUtil::String info = "Test2 in progress...";
		mApplicationController.log(info);
		int platform = getPlatform();
		if ( platform == ANDROID )
		{
			mPurchase = new Purchase("android.test.purchased", this);
			mPurchase->addPurchaseListener(this);
		}
		else if (platform == IOS)
		{
			mApplicationController.testSucceeded(*this);
		}
		PurchaseManager::getInstance()->setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkq5y2a2tbd9xjNfBwtxt1G/wKILzrHSWp3uAwQ/Iw2SkAF72cuoLUiliTkU6IXVhD95k1VsZRLQytKOIAmPOI/faGxbZovmfIq2u/F/Gd39AyUg2geLdhV+wMrgdFWwWiPwOXl+12zigOvJdAfsxNMLh0dhJW1RAK5OaT0TXkR5d26GiUYm6kt0SW4+FX1OSiavwE4NTPTtwqsjscWxBKXvICm0A+0OYtqv6wTC3uhVMDkTnrsVG9N4pTKSGmhz//dfqJIZKI56UD21I2hjsB+8XVxy0KV741x3NLBOruB5oGGlXbCQGHTu3ekY4g5jlusCxlD19327e7LxrooM9GQIDAQAB");
		MAUtil::String productType = ProductTypes::getInstance().getProductType1();
		printf("Test2::startTest sProductType1 = %s", productType.c_str());
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test2::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Notifies that the product has been validated by the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::productValid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product is valid.",
				mPurchase->getProductId().c_str());
		mApplicationController.log(buffer);
		mApplicationController.log("Requesting purchase...");
		mPurchase->requestPurchase();
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::productInvalid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product is invalid.",
				mPurchase->getProductId().c_str());
		mApplicationController.log(buffer);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::requestInProgress(const Purchase& purchase)
	{
		mApplicationController.log("Request in progress...");
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::requestCompleted(const Purchase& purchase)
	{
		mApplicationController.log("Product purchased!");
		mApplicationController.testSucceeded(*this);
	}

	/**
	 * Notifies that the transaction has failed.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test2::requestFailed(const Purchase& purchase,
		const int errorCode)
		{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product was not purchased.",
				mPurchase->getProductId().c_str());
		mApplicationController.log(buffer);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
		}

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	void Test2::receiptValid(
		const Purchase& purchase,
		Receipt& receipt)
	{

	}

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::receiptInvalid(const Purchase& purchase)
	{

	}

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test2::receiptError(const Purchase& purchase,
		const int errorCode)
	{

	}

}
