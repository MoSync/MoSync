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
 * @file Purchase.cpp
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief The Purchase class wraps a product that can be buyed and also all
 * the details of the transaction(such as the receipt).
 */

// Size of the buffer used to get product's id.
#define PRODUCT_ID_BUFFER_SIZE 1024

#include "Purchase.h"
#include "Receipt.h"
#include "PurchaseManager.h"

namespace IAP
{
	// Usual size for a buffer.
	const int BUF_SIZE = 256;

	/**
	 * Constructor.
	 * @param productID String that identifies the product.
	 * This string must be used by the App Store / Google Play to identify
	 * the product.
	 * @param listener The listener that will receive purchase events.
	 */
	Purchase::Purchase(const MAUtil::String& productID,
		PurchaseListener* listener):
		mHandle(-1),
		mReceipt(NULL),
		mIsRestored(false)
	{
		this->addPurchaseListener(listener);
		mHandle = maCreatePlaceholder();
		PurchaseManager::getInstance()->registerPurchase(this);
		maPurchaseCreate(mHandle, productID.c_str());
	}

	/**
	 * Creates a purchase using a product handle.
	 * Used to create restored or refunded products.
	 * @param productHandle Handle to the product.
	 */
	Purchase::Purchase(MAHandle productHandle):
		mHandle(productHandle),
		mReceipt(NULL),
		mIsRestored(true)

	{
		PurchaseManager::getInstance()->registerPurchase(this);
	}

	/**
	 * Copy constructor.
	 */
	Purchase::Purchase(const IAP::Purchase &purchase):
		mHandle(purchase.mHandle),
		mReceipt(NULL),
		mIsRestored(true)
	{
		PurchaseManager::getInstance()->registerPurchase(this);
	}

	/**
	 * Destructor.
	 */
	Purchase::~Purchase()
	{
		PurchaseManager::getInstance()->unregisterPurchase(this);
		maPurchaseDestroy(mHandle);
		// If the purchase is restored do not destroy placeholder.
		if (!mIsRestored)
		{
			maDestroyPlaceholder(mHandle);
		}
		mPurchaseEventListeners.clear();

		delete mReceipt;
	}

	/**
	 * @return The handle of the purchase.
	 */
	MAHandle Purchase::getHandle() const
	{
		return mHandle;
	}

	/**
	 * Get the productId that identifies the product.
	 * Platform: Android and iOS.
	 * @return The string that identifies the product, or an
	 * empty string in case of error.
	 */
	MAUtil::String Purchase::getProductId() const
	{
		char buffer[BUF_SIZE];
		int result = maPurchaseGetName(mHandle,
			buffer,
			BUF_SIZE);
		if (result < 0)
		{
			return "";
		}
		return buffer;
	}

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
	void Purchase::requestPurchase(int quantity)
	{
		maPurchaseRequest(mHandle, quantity);
	}

	/**
	 * Verify if the receipt came from Apple App Store / Google Play.
	 * Make sure that the product is purchased before calling this method.
	 * Listeners will be notified.
	 */
	void Purchase::verifyReceipt()
	{
		maPurchaseVerifyReceipt(mHandle);
	}

	/**
	 * Handles a purchase event.
	 * Observers will be notified regarding this event.
	 * @param purchaseData Purchase event data.
	 */
	void Purchase::handlePurchaseEvent(const MAPurchaseEventData& purchaseData)
	{
		switch (purchaseData.type)
		{
			case MA_PURCHASE_EVENT_PRODUCT_CREATE:
				this->handleProductCreateEvent(purchaseData);
				break;
			case MA_PURCHASE_EVENT_REQUEST:
				this->handleRequestEvent(purchaseData);
				break;
			case MA_PURCHASE_EVENT_VERIFY_RECEIPT:
				this->handleReceiptEvent(purchaseData);
				break;
		}
	}

	/**
	 * Add an event listener for this purchase.
	 * @param listener The listener that will receive purchase events for
	 * this product.
	 */
	void Purchase::addPurchaseListener(PurchaseListener* listener)
	{
		int countListeners = mPurchaseEventListeners.size();
		for (int i = 0; i < countListeners; i++)
		{
			if (listener == mPurchaseEventListeners[i])
			{
				return;
			}
		}

		mPurchaseEventListeners.add(listener);
	}

	/**
	 * Remove the event listener for this purchase.
	 * @param listener The listener that receives purchase events
	 * for this purchase.
	 */
	void Purchase::removePurchaseListener(PurchaseListener* listener)
	{
		int countListeners = mPurchaseEventListeners.size();
		for (int i = 0; i < countListeners; i++)
		{
			if (listener == mPurchaseEventListeners[i])
			{
				mPurchaseEventListeners.remove(i);
				break;
			}
		}
	}

	/**
	 * Handle MA_PURCHASE_EVENT_PRODUCT_CREATE.
	 * Notifies listeners about the event.
	 * @param purchaseData Event data.
	 */
	void Purchase::handleProductCreateEvent(
		const MAPurchaseEventData& purchaseData)
	{
		int countListeners = mPurchaseEventListeners.size();
		for (int i = 0; i < countListeners; i++)
		{
			if (purchaseData.state == MA_PURCHASE_STATE_PRODUCT_VALID)
			{
				mPurchaseEventListeners[i]->productValid(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_PRODUCT_INVALID)
			{
				mPurchaseEventListeners[i]->productInvalid(*this);
			}
		}
	}

	/**
	 * Handle MA_PURCHASE_EVENT_REQUEST.
	 * Notifies listeners about the event.
	 * @param purchaseData Event data.
	 */
	void Purchase::handleRequestEvent(const MAPurchaseEventData& purchaseData)
	{
		int countListeners = mPurchaseEventListeners.size();
		for (int i = 0; i < countListeners; i++)
		{
			this->notifyListenerRequestEvent(
				purchaseData,
				mPurchaseEventListeners[i]);
		}
	}

	/**
	 * Notifies one listener about a MA_PURCHASE_EVENT_REQUEST event.
	 * @param purchaseData Event data.
	 * @param listener Listener that will be notified.
	 */
	void Purchase::notifyListenerRequestEvent(
		const MAPurchaseEventData& purchaseData,
		PurchaseListener* listener)
	{
		switch (purchaseData.state)
		{
			case MA_PURCHASE_STATE_IN_PROGRESS:
				listener->requestInProgress(*this);
				break;
			case MA_PURCHASE_STATE_COMPLETED:
				listener->requestCompleted(*this);
				break;
			case MA_PURCHASE_STATE_FAILED:
				listener->requestFailed(*this, purchaseData.errorCode);
				break;
		}
	}

	/**
	 * Handle MA_PURCHASE_EVENT_RECEIPT.
	 * Notifies listeners about the event.
	 * @param purchaseData Event data.
	 */
	void Purchase::handleReceiptEvent(const MAPurchaseEventData& purchaseData)
	{
		delete mReceipt;
		mReceipt = NULL;
		if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_VALID)
		{
			mReceipt = new Receipt(mHandle);
		}

		int countListeners = mPurchaseEventListeners.size();
		for (int i = 0; i < countListeners; i++)
		{
			this->notifyListenerReceiptEvent(
				purchaseData,
				mPurchaseEventListeners[i]);
		}
	}

	/**
	 * Notifies one listener about a MA_PURCHASE_EVENT_RECEIPT event.
	 * @param purchaseData Event data.
	 * @param listener Listener that will be notified.
	 */
	void Purchase::notifyListenerReceiptEvent(
		const MAPurchaseEventData& purchaseData,
		PurchaseListener* listener)
	{
		switch (purchaseData.state)
		{
			case MA_PURCHASE_STATE_RECEIPT_VALID:
				listener->receiptValid(*this, *mReceipt);
				break;
			case MA_PURCHASE_STATE_RECEIPT_INVALID:
				listener->receiptInvalid(*this);
				break;
			case MA_PURCHASE_STATE_RECEIPT_ERROR:
				listener->receiptError(*this, purchaseData.errorCode);
				break;
		}
	}

} // namespace IAP
