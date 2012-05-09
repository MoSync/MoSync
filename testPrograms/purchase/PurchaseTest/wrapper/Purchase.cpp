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
 * @brief
 */

#include "Purchase.h"
#include "Receipt.h"
#include "PurchaseManager.h"

namespace Purchase
{

	/**
	 * Constructor.
	 * @param productID String that identifies the product.
	 * This string must be used by the App Store / Google Play to identify
	 * the product.
	 */
	Purchase::Purchase(const MAUtil::String& productID):
		mHandle(-1),
		mReceipt(NULL)
	{
		mHandle = maCreatePlaceholder();
		maPurchaseCreate(mHandle, productID.c_str());
		PurchaseManager::getInstance()->registerPurchase(this);
	}

	/**
	 * Creates a purchase using a product handle.
	 * Used to create restored or refunded products.
	 * @param productHandle Handle to the product.
	 */
	Purchase::Purchase(MAHandle productHandle):
		mHandle(productHandle),
		mReceipt(NULL)

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
		maDestroyPlaceholder(mHandle);
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
	 * Handles a purchase event.
	 * Observers will be notified regarding this event.
	 * @param purchaseData Purchase event data.
	 */
	void Purchase::handlePurchaseEvent(const MAPurchaseEventData& purchaseData)
	{
		if (purchaseData.type == MA_PURCHASE_EVENT_PRODUCT_CREATE)
		{
			this->handleProductCreateEvent(purchaseData);
		}
	}

	/**
	 * Add an event listener for this purchase.
	 * @param listener The listener that will receive purchase events for
	 * this product.
	 */
	void Purchase::addPurchaseListener(PurchaseListener* listener)
	{
		for (int i = 0; i < mPurchaseEventListeners.size(); i++)
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
		for (int i = 0; i < mPurchaseEventListeners.size(); i++)
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
		// TODO optimize for loop.
		for( int i = 0; i < mPurchaseEventListeners.size(); i++)
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
		// TODO optimize for loop.
		for (int i = 0; i < mPurchaseEventListeners.size(); i++) {
			if (purchaseData.state == MA_PURCHASE_STATE_IN_PROGRESS)
			{
				mPurchaseEventListeners[i]->requestInProgress(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_COMPLETED)
			{
				mPurchaseEventListeners[i]->requestCompleted(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_FAILED)
			{
				mPurchaseEventListeners[i]->requestFailed(
					*this,
					purchaseData.errorCode);
			}
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

		// TODO optimize for loop.
		for (int i = 0; i < mPurchaseEventListeners.size(); i++)
		{
			if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_VALID)
			{
				mPurchaseEventListeners[i]->receiptValid(*this,
					*mReceipt);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_INVALID)
			{
				mPurchaseEventListeners[i]->receiptInvalid(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_ERROR)
			{
				mPurchaseEventListeners[i]->receiptError(
					*this,
					purchaseData.errorCode);
			}
		}
	}

} // namespace Purchase
