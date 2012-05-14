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
 * @author Bogdan Iusco
 * @date 9 May 2012
 *
 * @brief Test a valid purchase of an product.
 */

#define TEST_NAME "Test 1"

#define IOS_PRODUCT_ID "com.mosync.purchase2.consumable"
#define ANDROID_PRODUCT_ID ""

#include <conprint.h>

#include "Test1.h"
#include "../Util.h"

#include "../wrapper/Purchase.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test1::Test1(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{

	}

	/**
	 * Destructor.
	 */
	Test1::~Test1()
	{

	}

	/**
	 * Start the test.
	 */
	void Test1::startTest()
	{
		MAUtil::String productType = ProductTypes::getInstance().getProductType1();
		printf("Test1::startTest sProductType1 = %s", productType.c_str());
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test1::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Notifies that the product has been validated by the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test1::productValid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product is valid.",
				mPurchase->getProductID().c_str());
		mApplicationController.log(buffer);
		mApplicationController.log("Requesting purchase...");
		mPurchase->requestPurchase();
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test1::productInvalid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "%s product is invalid.",
				mPurchase->getProductID().c_str());
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
	void Test1::requestInProgress(const Purchase& purchase)
	{
		mApplicationController.log("Request in progress...");
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test1::requestCompleted(const Purchase& purchase)
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
	void Test1::requestFailed(const Purchase& purchase,
		const int errorCode)
		{

		}

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	void Test1::receiptValid(
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
	void Test1::receiptInvalid(const Purchase& purchase)
	{

	}

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test1::receiptError(const Purchase& purchase,
		const int errorCode)
	{

	}

}
