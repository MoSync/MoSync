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
 * @file Product.cpp
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief
 */

#include "Product.h"
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
	Product::Product(const MAUtil::String& productID):
		mHandle(-1),
		mReceipt(NULL)
	{
		mHandle = maCreatePlaceholder();
		maPurchaseCreate(mHandle, productID.c_str());
		PurchaseManager::getInstance()->registerProduct(this);
	}

	/**
	 * Creates a product using a product handle.
	 * Used to create restored or refunded products.
	 * @param productHandle Handle to the product.
	 */
	Product::Product(MAHandle productHandle):
		mHandle(productHandle),
		mReceipt(NULL)

	{
		PurchaseManager::getInstance()->registerProduct(this);
	}

	/**
	 * Destructor.
	 */
	Product::~Product()
	{
		PurchaseManager::getInstance()->unregisterProduct(this);
		maPurchaseDestroy(mHandle);
		maDestroyPlaceholder(mHandle);
		mProductEventListeners.clear();

		delete mReceipt;
	}

	/**
	 * @return The handle of the product.
	 */
	MAHandle Product::getHandle() const
	{
		return mHandle;
	}

	/**
	 * Handles a purchase event.
	 * Observers will be notified regarding this event.
	 * @param purchaseData Purchase event data.
	 */
	void Product::handlePurchaseEvent(const MAPurchaseEventData& purchaseData)
	{
		if (purchaseData.type == MA_PURCHASE_EVENT_PRODUCT_CREATE)
		{
			this->handleProductCreateEvent(purchaseData);
		}
	}

	/**
	 * Add an event listener for this product.
	 * @param listener The listener that will receive purchase events for
	 * this product.
	 */
	void Product::addProductListener(ProductListener* listener)
	{
		for (int i = 0; i < mProductEventListeners.size(); i++)
		{
			if (listener == mProductEventListeners[i])
			{
				return;
			}
		}

		mProductEventListeners.add(listener);
	}

	/**
	 * Remove the event listener for this product.
	 * @param listener The listener that receives purchase events
	 * for this product.
	 */
	void Product::removeProductListener(ProductListener* listener)
	{
		for (int i = 0; i < mProductEventListeners.size(); i++)
		{
			if (listener == mProductEventListeners[i])
			{
				mProductEventListeners.remove(i);
				break;
			}
		}
	}

	/**
	 * Handle MA_PURCHASE_EVENT_PRODUCT_CREATE.
	 * Notifies listeners about the event.
	 * @param purchaseData Event data.
	 */
	void Product::handleProductCreateEvent(
		const MAPurchaseEventData& purchaseData)
	{
		// TODO optimize for loop.
		for( int i = 0; i < mProductEventListeners.size(); i++)
		{
			if (purchaseData.state == MA_PURCHASE_STATE_PRODUCT_VALID)
			{
				mProductEventListeners[i]->productValid(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_PRODUCT_INVALID)
			{
				mProductEventListeners[i]->productInvalid(*this);
			}
		}
	}

	/**
	 * Handle MA_PURCHASE_EVENT_REQUEST.
	 * Notifies listeners about the event.
	 * @param purchaseData Event data.
	 */
	void Product::handleRequestEvent(const MAPurchaseEventData& purchaseData)
	{
		// TODO optimize for loop.
		for (int i = 0; i < mProductEventListeners.size(); i++) {
			if (purchaseData.state == MA_PURCHASE_STATE_IN_PROGRESS)
			{
				mProductEventListeners[i]->requestInProgress(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_COMPLETED)
			{
				mProductEventListeners[i]->requestCompleted(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_FAILED)
			{
				mProductEventListeners[i]->requestFailed(
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
	void Product::handleReceiptEvent(const MAPurchaseEventData& purchaseData)
	{
		delete mReceipt;
		mReceipt = NULL;
		if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_VALID)
		{
			mReceipt = new Receipt(mHandle);
		}

		// TODO optimize for loop.
		for (int i = 0; i < mProductEventListeners.size(); i++)
		{
			if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_VALID)
			{
				mProductEventListeners[i]->receiptValid(*this,
					*mReceipt);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_INVALID)
			{
				mProductEventListeners[i]->receiptInvalid(*this);
			}
			else if (purchaseData.state == MA_PURCHASE_STATE_RECEIPT_ERROR)
			{
				mProductEventListeners[i]->receiptError(
					*this,
					purchaseData.errorCode);
			}
		}
	}

} // namespace Purchase
