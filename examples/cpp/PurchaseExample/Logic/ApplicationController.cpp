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
 * @file ApplicationController.cpp
 *
 * @brief  The controller that handles all purchase related
 * The controller is responsible for updating the UI with all
 * purchase events.
 * @author Emma Tresanszki
 */

#include <maapi.h>
#include <Purchase/Purchase.h>

#include "Config.h"
#include "../UI/MainScreen.h"
#include "ApplicationController.h"
#include "../Database/DatabaseManager.h"
#include "../Database/DatabaseProduct.h"
//#include "Util.h"

/**
 * Cosntructor.
 */
ApplicationController::ApplicationController():
mMainScreen(NULL),
mDatabase(NULL)
{
	mDatabase = new DatabaseManager();
	mMainScreen = new MainScreen(this);
	mMainScreen->show();
	mMainScreen->getBuyButton()->addButtonListener(this);
	mMainScreen->getPurchasedItemList()->addListViewListener(this);

	PurchaseManager::getInstance()->setStoreURL(sAppStoreSandboxURL);
}

/**
 * Destructor.
 */
ApplicationController::~ApplicationController()
{
	mMainScreen->getBuyButton()->removeButtonListener(this);
	mMainScreen->getPurchasedItemList()->removeListViewListener(this);
	delete mMainScreen;
	delete mDatabase;
}

/**
 * Get the interface to database.
 * @return The database's interface.
 */
DatabaseManager& ApplicationController::getDatabase() const
{
	return *mDatabase;
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void ApplicationController::buttonClicked(Widget* button)
{
	if ( mMainScreen->getSelectedAvailableProduct() != "")
	{
		// Create new entry in the DB.
		DatabaseProduct* dbProduct = mDatabase->getRow(
				mMainScreen->getSelectedAvailableProduct());
		if (!dbProduct)
		{
			// Purchase a non consumable product.
			Purchase* purchaseObj = new Purchase(
					mMainScreen->getSelectedAvailableProduct(), this);
			mPurchases.add(purchaseObj);
			mCurrentPurchase = mPurchases.size()-1;
		}
	}
}

/**
 * Restore all owned products.
 */
void ApplicationController::restoreTransactions()
{
	PurchaseManager* manager = PurchaseManager::getInstance();
	manager->addPurchaseListener(this);
	manager->restoreTransactions();
}

/**
 * Notifies that a purchase has been restored.
 * Platform: iOS and Android.
 * @param purchase The purchase that has been restored.
 */
void ApplicationController::purchaseRestored(Purchase& purchase)
{
	Purchase* restoredItem = new Purchase(purchase.getProductId(), this);
	mPurchases.add(restoredItem);
	mCurrentPurchase = mPurchases.size()-1;
	//restoredItem->addPurchaseListener(this);
	restoredItem->verifyReceipt();
//	mCurrentPurchase = mPurchase

	// Store the product in the repository.
	DatabaseProduct* dbProduct = new DatabaseProduct();
	dbProduct->setProductID(purchase.getProductId());
	int date = maLocalTime();
	dbProduct->setDate(date);
	mDatabase->addRow(*dbProduct);
	delete dbProduct;
	dbProduct = NULL;
	delete restoredItem;
	restoredItem = NULL;

	// Notify The UI.
	mMainScreen->productRestored(purchase.getProductId());
}

/**
 * Notifies that restoreTransactions() has failed.
 * @param errorCode The reason why it failed.
 * Platform: iOS and Android.
 */
void ApplicationController::purchaseRestoreError(int errorCode)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, "Restoring products failed with error code %d", errorCode);
	maAlert("Restore error", "restore failed" , "OK", NULL, NULL);
}

/**
 * Notifies that a purchase has been refunded.
 * Platform: Android.
 * @param purchase The purchase that has been refunded.
 */
void ApplicationController::purchaseRefunded(Purchase& purchase)
{
	MAUtil::String refundString = "You received a refund for " + purchase.getProductId();
	maAlert("Product refunded", refundString.c_str(), "OK", NULL,NULL);
	Purchase* restoredItem = new Purchase(purchase.getProductId(), this);
	mPurchases.add(restoredItem);
	mCurrentPurchase = mPurchases.size()-1;

	// Notify The UI.
	mMainScreen->productRefunded(purchase.getProductId());
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param sectionIndex The index of the section that contains the selected item.
 * Will be #MAW_RES_INVALID_INDEX for default type list views.
 * @param itemIndex The index (within the parent section if the section is valid)
 * of the list view item clicked.
 */
void ApplicationController::listViewItemClicked(
	ListView *listView,
	const int sectionIndex,
	const int itemIndex)
{
	// An owned item was clicked, so now get the receipt for it
	// and display it in a dialog.
	mCurrentPurchase = itemIndex;
	mPurchases[itemIndex]->verifyReceipt();
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
void ApplicationController::productValid(const Purchase& purchase)
{
	//maAlert("Product valid", "Purchase in progres..", "OK","","");
	mPurchases[mCurrentPurchase]->requestPurchase();
}

/**
 * Notifies that the product is not valid on the App Store.
 * Platform: iOS.
 * @param purchase The object that sent the event.
 */
void ApplicationController::productInvalid(const Purchase& purchase)
{
	MAUtil::String errorMessage = "Product " + purchase.getProductId()
			+ " is invalid!";
	mMainScreen->productError(errorMessage);
	mPurchases.remove( mCurrentPurchase );
	mCurrentPurchase = -1;
	//TODO remove, only for android.test.purchased
//	/mPurchases[mCurrentPurchase]->requestPurchase();
}

/**
 * Notifies that the transaction has been successfully processed.
 * The user should receive the purchased product.
 * Platform: Android and iOS.
 * @param purchase The object that sent the event.
 */
void ApplicationController::requestCompleted(const Purchase& purchase)
{
	// Notify UI that a product was purchased.
	mMainScreen->productPurchased(purchase.getProductId());

	// Update DB.
	DatabaseProduct* dbProduct = new DatabaseProduct();
	dbProduct->setProductID(purchase.getProductId());
	int date = maLocalTime();
	dbProduct->setDate(date);
	mDatabase->addRow(*dbProduct);
	delete dbProduct;
	dbProduct = NULL;
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
void ApplicationController::requestFailed(const Purchase& purchase,
	const int errorCode)
{
	mPurchases.remove( mCurrentPurchase );
	mCurrentPurchase = -1;
	MAUtil::String errorMessage = "Purchase failed for product " + purchase.getProductId();
	if ( errorCode == MA_PURCHASE_ERROR_PRODUCT_ALREADY_OWNED)
	{
		errorMessage += ". Item already owned!";
	}
	else
	{
		errorMessage += " with err code = "  + MAUtil::integerToString(errorCode);
	}
	mMainScreen->productError(errorMessage);
}

/**
 * Notifies that the transaction has been validated by the App Store /
 * Google Play.
 * Platform: Android and iOS.
 * @param purchase The object that sent the event.
 * @param receipt Transaction receipt.
 */
void ApplicationController::receiptValid(
	const Purchase& purchase,
	Receipt& receipt)
{
	if ( purchase.getHandle() == mPurchases[mCurrentPurchase]->getHandle() )
	{
		// Display the dialog containing the receipt information.
		mMainScreen->fillReceiptDialog(receipt.getAppID(), receipt.getProductID(),
				receipt.getTransactionDate(), receipt.getTransactionID(),
				receipt.getBID(), receipt.getPrice(), receipt.getTitle());
	}
}

/**
 * Notifies that the transaction is not valid on the App Store /
 * Google Play.
 * Platform: Android and iOS.
 * @param purchase The object that sent the event.
 */
void ApplicationController::receiptInvalid(const Purchase& purchase)
{
	mMainScreen->productError("Product " + purchase.getProductId()
			+ " does not have a receipt. It may be a managed item.");
}

/**
 * Notifies that an error occurred while verifying the receipt.
 * Platform: Android and iOS.
 * @param purchase The object that sent the event.
 * @param errorCode The reason why it failed.
 */
void ApplicationController::receiptError(const Purchase& purchase,
	const int errorCode)
{
	mMainScreen->productError("Product " + purchase.getProductId()
			+ " does not have a receipt!");
}
