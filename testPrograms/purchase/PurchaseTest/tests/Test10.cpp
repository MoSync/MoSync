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
 * @file Test10.cpp
 * @author Bogdan Iusco
 * @date 23 May 2012
 *
 * @brief It runs only on iOS.
 * Restores previously bought products.
 * If no products were restored a new non consumable product will be
 * purchased.
 */

#define TEST_NAME "Test 10"

#include <Purchase/Purchase.h>
#include <Purchase/PurchaseManager.h>
#include <Purchase/Receipt.h>
#include <conprint.h>

#include "Test10.h"
#include "../Util.h"
#include "../Database/IDatabaseManager.h"
#include "../Database/DatabaseProduct.h"

namespace PurchaseTest
{

	using namespace MAUtil;

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test10::Test10(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mPurchase(NULL)
	{

	}

	/**
	 * Destructor.
	 */
	Test10::~Test10()
	{
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test10::startTest()
	{
		if (getPlatform() == IOS)
		{
			String productID = ProductTypes::getInstance().getProductType2();
			IDatabaseManager& database = mApplicationController.getDatabase();
			DatabaseProduct* dbProduct = database.getRow(productID);
			if (!dbProduct)
			{
				// Purchase a non consumable product.
				mApplicationController.log("Purchase the product!");
				mPurchase = new Purchase(productID, this);
			}
			else
			{
				PurchaseManager* manager = PurchaseManager::getInstance();
				manager->addPurchaseListener(this);
				mApplicationController.log("Restoring transactions");
				manager->restoreTransactions();
				delete dbProduct;
				dbProduct = NULL;
			}
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
	MAUtil::String Test10::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Notifies that a purchase has been restored.
	 * Platform: iOS and Android.
	 * @param purchase The purchase that has been restored.
	 */
	void Test10::purchaseRestored(Purchase& purchase)
	{
		PurchaseManager::getInstance()->setStoreURL(sAppStoreSandboxURL);
		char buffer[BUF_MAX];
		sprintf(buffer, "Product %s was restored", purchase.getProductId().c_str());
		mApplicationController.log(buffer);
		mPurchase = new Purchase(purchase);
		mPurchase->addPurchaseListener(this);
		mApplicationController.log("Verifying receipt...");
		mPurchase->verifyReceipt();
	}

	/**
	 * Notifies that restoreTransactions() has failed.
	 * @param errorCode The reason why it failed.
	 * Platform: iOS and Android.
	 */
	void Test10::purchaseRestoreError(int errorCode)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Restoring products failed with error code %d", errorCode);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that a purchase has been refunded.
	 * Platform: Android.
	 * @param purchase The purchase that has been refunded.
	 */
	void Test10::purchaseRefunded(Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Product %s was refunded and not restored",
			purchase.getProductId().c_str());
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
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
	void Test10::productValid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Product %s is valid", purchase.getProductId().c_str());
		mApplicationController.log(buffer);
		mPurchase->requestPurchase();
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test10::productInvalid(const Purchase& purchase)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Product %s is invalid", purchase.getProductId().c_str());
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test10::requestInProgress(const Purchase& purchase)
	{
		mApplicationController.log("Request in progress...");
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test10::requestCompleted(const Purchase& purchase)
	{
		mApplicationController.log("Purchase completed!");
		IDatabaseManager& database = mApplicationController.getDatabase();
		DatabaseProduct* dbProduct = new DatabaseProduct();
		dbProduct->setProductID(mPurchase->getProductId());
		int date = maLocalTime();
		dbProduct->setDate(date);
		database.addRow(*dbProduct);
		delete dbProduct;
		dbProduct = NULL;

		mApplicationController.testSucceeded(*this);
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
	void Test10::requestFailed(const Purchase& purchase,
		const int errorCode)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Purchase failed with error code %d.",
			errorCode);
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
	void Test10::receiptValid(
		const Purchase& purchase,
		Receipt& receipt)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Receipt valid for %s.", receipt.getProductID().c_str());
		mApplicationController.testSucceeded(*this);
	}

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test10::receiptInvalid(const Purchase& purchase)
	{
		this->setFailedReason("Invalid receipt");
		mApplicationController.testFailed(*this);
	}

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	void Test10::receiptError(const Purchase& purchase,
		const int errorCode)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "Receipt error %d.", errorCode);
		this->setFailedReason(buffer);
		mApplicationController.testFailed(*this);
	}

} // namespace PurchaseTest
