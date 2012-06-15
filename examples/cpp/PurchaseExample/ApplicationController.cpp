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
 * @file ApplicationController.cpp
 *
 * @brief  The controller that handles all purchase related
 * The controller is responsible for updating the UI with all
 * purchase events.
 * @author Emma Tresanszki
 */

#include <maapi.h>

#include "Config.h"
#include "MainScreen.h"
#include "ApplicationController.h"

/**
 * Cosntructor.
 */
ApplicationController::ApplicationController():
mMainScreen(NULL)
{
	mMainScreen = new MainScreen();
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
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android, Windows Phone.
 * @param button The button object that generated the event.
 */
void ApplicationController::buttonClicked(Widget* button)
{
	if ( mMainScreen->getSelectedAvailableProduct() != "")
	{
		Purchase* purchaseObj = new Purchase(
				mMainScreen->getSelectedAvailableProduct(), this);
		mPurchases.add(purchaseObj);
		mCurrentPurchase = mPurchases.size()-1;
	}
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param index The index on which the list view item is positioned.
 */
void ApplicationController::listViewItemClicked(
    ListView* listView,
    int index)
{
	// An owned item was clicked, so now get the receipt for it
	// and display it in a dialog.
	mCurrentPurchase = index;
	mPurchases[index]->verifyReceipt();
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
	mMainScreen->productError("Purchase failed for product "
			+ purchase.getProductId() );
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
				receipt.getBID());
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
			+ " has an invalid receipt!");
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
