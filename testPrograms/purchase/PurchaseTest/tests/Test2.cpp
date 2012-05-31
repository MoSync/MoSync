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
 * Receipt's fields are also verified.
 */

#define TEST_NAME "Test 2"


#include <Purchase/Purchase.h>
#include <Purchase/PurchaseManager.h>
#include <Purchase/Receipt.h>

#include <MAUtil/util.h>
#include <conprint.h>
#include <matime.h>

#include "Test2.h"
#include "../Util.h"
#include "../Controller/Config.h"

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
		PurchaseManager::getInstance()->setPublicKey(DEVELOPER_PUBLIC_KEY);
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
		PurchaseManager::getInstance()->setStoreURL(sAppStoreSandboxURL);
		mPurchase->verifyReceipt();
		mApplicationController.log("Verifying receipt!");
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
		sprintf(buffer, "%s product was not purchased. Error code: %d",
				mPurchase->getProductId().c_str(), errorCode);
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
		mApplicationController.log("Receipt is valid.");
		this->printReceiptFieldValues(receipt);
		mApplicationController.testSucceeded(*this);
	}

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test2::receiptInvalid(const Purchase& purchase)
	{
		this->setFailedReason("Receipt is invalid!");
		mApplicationController.testFailed(*this);
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
		this->setFailedReason("An error has occurred while verifying receipt!");
		mApplicationController.testFailed(*this);
	}

	/**
	 * Print the receipt's field values.
	 * @param receipt The given receipt.
	 */
	void Test2::printReceiptFieldValues(Receipt& receipt)
	{
		this->printFieldNameAndValue("Product ID",
			receipt.getProductID());
		this->printTransactionDate(receipt);

		int platform = getPlatform();
		if (platform == IOS)
		{
			this->printFieldNameAndValue("Transaction ID",
				receipt.getTransactionID());
			this->printFieldNameAndValue("Bundle ID",
				receipt.getBID());
			this->printFieldNameAndValue("Version number",
				receipt.getBVRS());
			MAUtil::String price = MAUtil::doubleToString(
				receipt.getPrice());
			this->printFieldNameAndValue("Price",
				price);
			this->printFieldNameAndValue("Title",
				receipt.getTitle());
			this->printFieldNameAndValue("Description",
				receipt.getDescription());
		}
	}

	/**
	 * Print a receipt field name and value.
	 * @param fieldName Field's name.
	 * @param fieldValue Field's value.
	 */
	void Test2::printFieldNameAndValue(
		const MAUtil::String& fieldName,
		const MAUtil::String& fieldValue)
	{
		MAUtil::String text = fieldName + " : " + fieldValue;
		mApplicationController.log(text);
	}

	/**
	 * Print transaction date of the product.
	 * @param receipt Purchase's receipt.
	 */
	void Test2::printTransactionDate(Receipt& receipt)
	{
		int timeInSeconds = receipt.getTransactionDate();
		printf("timeInSeconds = %d", timeInSeconds);
		struct tm *date = new tm;
		split_time(timeInSeconds, date);
		char buffer[BUF_MAX];
		sprintf(buffer,
			"Transaction system date: %d/%d/%d %d:%d:%d",
			date->tm_mday,
			date->tm_mon+1,
			1900 + date->tm_year,
			date->tm_hour+1,
			date->tm_min+1,
			date->tm_sec+1);
		mApplicationController.log(buffer);
		delete date;
	}

} // namespace PurchaseTest
