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

/*! \addtogroup PurchaseLib
 *  @{
 */

/**
 *  @defgroup PurchaseLib Purchase Library
 *  @{
 */

/**
 * @file Purchase.h
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief The Purchase class wraps a product that can be buyed and also all
 * the details of the transaction(such as the receipt).
 */

#ifndef IAP_PURCHASE_H_
#define IAP_PURCHASE_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <maapi.h>

#include "PurchaseListener.h"

namespace IAP
{

	// Forward declaration.
	class PurchaseManager;

	/**
	 *  GooglePlay reserved productID for testing static in-app billing response
	 *  as if a purchase was made.
	 * (testing purpose).
	 */
	const MAUtil::String sGooglePlayPurchasedProductId = "android.test.purchased";

	/**
	 *  GooglePlay reserved productID for testing static in-app billing response
	 *  as if a refund was received.
	 *  When you make an in-app billing request with this product ID, Google Play
	 *  responds as though the purchase was refunded.
	 * (testing purpose).
	 */
	const MAUtil::String sGooglePlayRefundedProductId = "android.test.refunded";

	/**
	 *  GooglePlay reserved productID for testing static in-app billing response
	 *  as if an unavailable product tried to be purchased.
	 *  When you make an in-app billing request with this product ID, Google Play
	 *  responds as though the item being purchased was not listed in your
	 *  application's product list.
	 * (testing purpose).
	 */
	const MAUtil::String sGooglePlayItemUnavailableProductId = "android.test.item_unavailable";

	/**
	 *  GooglePlay reserved productID for testing static in-app billing response
	 *  as if a purchase was canceled.
	 *  When you make an in-app billing request with this product ID Google Play
	 *  responds as though the purchase was canceled.
	 * (testing purpose).
	 */
	const MAUtil::String sGooglePlayCanceledPurchaseProductId = "android.test.canceled";

	/**
	 *\brief The Purchase class wraps a product that can be bought and also all
	 * the details of the transaction(such as the receipt).
	 */
	class Purchase
	{
	public:
		/**
		 * Constructor.
		 * @param productID String that identifies the product.
		 * This string must be used by the App Store / Google Play to identify
		 * the product.
		 * @param listener The listener that will receive purchase events.
		 */
		Purchase(const MAUtil::String& productID,
			PurchaseListener* listener);

		/**
		 * Copy constructor.
		 */
		Purchase(const IAP::Purchase &purchase);

		/**
		 * Destructor.
		 */
		~Purchase();

		/**
		 * @return The handle of the purchase.
		 */
		MAHandle getHandle() const;

		/**
		 * Get the productId that identifies the product.
		 * Platform: Android and iOS.
		 * @return The string that identifies the product, or an
		 * empty string in case of error.
		 */
		MAUtil::String getProductId() const;

		/**
		 * Request the user to purchase a product.
		 * The system will handle the process of purchasing.
		 * Note: if there are another requests in progress, the requests will
		 * be queued.
		 * Listeners will be notified.
		 * @param quantity How many products to purchase. This value must be
		 * greater than zero. This param is used only by the iOS platform.
		 * On Android platform it will be ignored(only one product will be
		 * purchased).
		 */
		void requestPurchase(int quantity = 1);

		/**
		 * Verify if the receipt came from Apple App Store / Google Play.
		 * Make sure that the product is purchased before calling this method.
		 * Listeners will be notified.
		 */
		void verifyReceipt();

		/**
		 * Handles a purchase event.
		 * Observers will be notified regarding this event.
		 * @param purchaseData Purchase event data.
		 */
		void handlePurchaseEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Add an event listener for this purchase.
		 * @param listener The listener that will receive purchase events for
		 * this product.
		 */
		void addPurchaseListener(PurchaseListener* listener);

		/**
		 * Remove the event listener for this purchase.
		 * @param listener The listener that receives purchase events
		 * for this purchase.
		 */
		void removePurchaseListener(PurchaseListener* listener);

	private:
		/**
		 * Creates a purchase using a product handle.
		 * Used to create restored or refunded products.
		 * @param productHandle Handle to the product.
		 */
		Purchase(MAHandle productHandle);

		/**
		 * Handle MA_PURCHASE_EVENT_PRODUCT_CREATE.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleProductCreateEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Handle MA_PURCHASE_EVENT_REQUEST.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleRequestEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Notifies one listener about a MA_PURCHASE_EVENT_REQUEST event.
		 * @param purchaseData Event data.
		 * @param listener Listener that will be notified.
		 */
		void notifyListenerRequestEvent(const MAPurchaseEventData& purchaseData,
			PurchaseListener* listener);

		/**
		 * Handle MA_PURCHASE_EVENT_RECEIPT.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleReceiptEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Notifies one listener about a MA_PURCHASE_EVENT_RECEIPT event.
		 * @param purchaseData Event data.
		 * @param listener Listener that will be notified.
		 */
		void notifyListenerReceiptEvent(const MAPurchaseEventData& purchaseData,
			PurchaseListener* listener);

	private:
		/**
		 * Handle that identifies the product.
		 */
		MAHandle mHandle;

		/**
		 * Transaction receipt.
		 */
		Receipt* mReceipt;

		/**
		 * Array with product listeners.
		 */
		MAUtil::Vector<PurchaseListener*> mPurchaseEventListeners;

		/**
		 * Flag to indicate if the product was restored.
		 */
		bool mIsRestored;

		// Let the PurchaseManager create Purchase objects using a product handle.
		friend class PurchaseManager;
	};
}

#endif /* IAP_PURCHASE_H_ */

/*! @} */
