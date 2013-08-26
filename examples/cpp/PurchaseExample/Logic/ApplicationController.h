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
 * @file ApplicationController.h
 *
 * @brief  The controller that handles all purchase related events.
 * The controller is responsible for updating the UI with all
 * purchase events.
 * @author Emma Tresanszki
 */

#ifndef APPLICATIONCONTROLLER_H_
#define APPLICATIONCONTROLLER_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <Purchase/Purchase.h>
#include <Purchase/PurchaseListener.h>
#include <Purchase/PurchaseManager.h>
#include <Purchase/PurchaseManagerListener.h>
#include <Purchase/Receipt.h>

using namespace NativeUI;
using namespace IAP;

// Forward declarations
class MainScreen;
class DatabaseManager;

class ApplicationController:
	public ButtonListener,
	public ListViewListener,
	public PurchaseListener,
	public PurchaseManagerListener
{
public:
	/**
	 * Constructor.
	 */
	ApplicationController();

	/**
	 * Destructor.
	 */
	virtual ~ApplicationController();

	/**
	 * Get the interface to database.
	 * @return The database's interface.
	 */
	DatabaseManager& getDatabase() const;

	/**
	 * Restore all owned products.
	 */
	void restoreTransactions();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS, Android, Windows Phone.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param sectionIndex The index of the section that contains the selected item.
	 * Will be #MAW_RES_INVALID_INDEX for default type list views.
	 * @param itemIndex The index (within the parent section if the section is valid)
	 * of the list view item clicked.
	 */
	virtual void listViewItemClicked(
		ListView *listView,
		const int sectionIndex,
		const int itemIndex);

    // From PurchaseListener
	/**
	 * Notifies that the product has been validated by the App Store.
	 * NOTE: On Android there is no validation done at this step, if the
	 * product results to be unavailable, then the application will be later
	 * notified via a requestFailed() callback. The errorCode will equal
	 * #MA_PURCHASE_ERROR_INVALID_PRODUCT.
	 * Platform: iOS and Android.
	 * @param purchase The object that sent the event.
	 */
	virtual void productValid(const Purchase& purchase);

	/**
	 * Notifies that a purchase has been restored.
	 * Platform: iOS and Android.
	 * @param purchase The purchase that has been restored.
	 */
	virtual void purchaseRestored(Purchase& purchase);

	/**
	 * Notifies that restoreTransactions() has failed.
	 * @param errorCode The reason why it failed.
	 * Platform: iOS and Android.
	 */
	virtual void purchaseRestoreError(int errorCode);

	/**
	 * Notifies that a purchase has been refunded.
	 * Platform: Android.
	 * @param purchase The purchase that has been refunded.
	 */
	virtual void purchaseRefunded(Purchase& purchase);

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	virtual void productInvalid(const Purchase& purchase);

	/**
	 * Notifies that the transaction has been received by the App Store/
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	virtual void requestInProgress(const Purchase& purchase){}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	virtual void requestCompleted(const Purchase& purchase);

	/**
	 * Notifies that the transaction has failed.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 * Note that even if the request fails because it was canceled
	 * (errorCode = MA_PURCHASE_ERROR_CANCELLED), you will still be
	 * able to get a receipt for your purchase.
	 */
	virtual void requestFailed(const Purchase& purchase,
		const int errorCode);

	/**
	 * Notifies that the transaction has been validated by the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param receipt Transaction receipt.
	 */
	virtual void receiptValid(
		const Purchase& purchase,
		Receipt& receipt);

	/**
	 * Notifies that the transaction is not valid on the App Store /
	 * Google Play.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	virtual void receiptInvalid(const Purchase& purchase);

	/**
	 * Notifies that an error occurred while verifying the receipt.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 * @param errorCode The reason why it failed.
	 */
	virtual void receiptError(const Purchase& purchase,
		const int errorCode);
private:
	/**
	 * The main screen of the application.
	 */
	MainScreen* mMainScreen;

	/**
	 * List of purchases.
	 */
	MAUtil::Vector<Purchase*> mPurchases;
	int mCurrentPurchase;

	/**
	 * Database manager.
	 */
	DatabaseManager* mDatabase;
};
#endif /* APPLICATIONCONTROLLER_H_ */
