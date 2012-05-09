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
 * @file ProductListener.h
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief The ProductListener can be used to listen for a product events.
 */

#ifndef PURCHASE_PRODUCT_LISTENER_H_
#define PURCHASE_PRODUCT_LISTENER_H_

namespace Purchase
{
	// Forward declarations.
	class Product;
	class Receipt;

	/**
	 * @brief Listener for product related events.
	 */
	class ProductListener
	{
	public:
		/**
		 * Notifies that the product has been validated by the App Store.
		 * Platform: iOS.
		 * @param product The object that sent the event.
		 */
		virtual void productValid(const Product& product) = 0;

		/**
		 * Notifies that the product is not valid on the App Store.
		 * Platform: iOS.
		 * @param product The object that sent the event.
		 */
		virtual void productInvalid(const Product& product) {};

		/**
		 * Notifies that the transaction has been received by the App Store/
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 */
		virtual void requestInProgress(const Product& product) {};

		/**
		 * Notifies that the transaction has been successfully processed.
		 * The user should receive the purchased product.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 */
		virtual void requestCompleted(const Product& product) = 0;

		/**
		 * Notifies that the transaction has failed.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 * @param errorCode The reason why it failed.
		 */
		virtual void requestFailed(const Product& product,
			const int errorCode) {};

		/**
		 * Notifies that the transaction has been validated by the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 * @param receipt Transaction receipt.
		 */
		virtual void receiptValid(
			const Product& product,
			Receipt& receipt) {};

		/**
		 * Notifies that the transaction is not valid on the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 */
		virtual void receiptInvalid(const Product& product) {};

		/**
		 * Notifies that an error occurred while verifying the receipt.
		 * Platform: Android and iOS.
		 * @param product The object that sent the event.
		 * @param errorCode The reason why it failed.
		 */
		virtual void receiptError(const Product& product,
			const int errorCode) {};

	};
}

#endif /* PURCHASE_PRODUCT_LISTENER_H_ */

/*! @} */
