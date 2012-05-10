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
 * @file PurchaseListener.h
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief The PurchaseListener can be used to listen for a purchase events.
 */

#ifndef IAP_PURCHASE_LISTENER_H_
#define IAP_PURCHASE_LISTENER_H_

namespace IAP
{
	// Forward declarations.
	class Purchase;
	class Receipt;

	/**
	 * @brief Listener for purchase related events.
	 */
	class PurchaseListener
	{
	public:
		/**
		 * Notifies that the product has been validated by the App Store.
		 * NOTE: On Android there is no validation done at this step, if the
		 * product results to be unavailable, then the application will be later
		 * notified via a requestFailed() callback. The errorCode will equal
		 * #MA_PURCHASE_ERROR_INVALID_PRODUCT.
		 * Platform: iOS and Android.
		 * @param purchase The object that sent the event.
		 */
		virtual void productValid(const Purchase& purchase) = 0;

		/**
		 * Notifies that the product is not valid on the App Store.
		 * Platform: iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void productInvalid(const Purchase& purchase) {};

		/**
		 * Notifies that the transaction has been received by the App Store/
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void requestInProgress(const Purchase& purchase) {};

		/**
		 * Notifies that the transaction has been successfully processed.
		 * The user should receive the purchased product.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void requestCompleted(const Purchase& purchase) = 0;

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
			const int errorCode) {};

		/**
		 * Notifies that the transaction has been validated by the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 * @param receipt Transaction receipt.
		 */
		virtual void receiptValid(
			const Purchase& purchase,
			Receipt& receipt) {};

		/**
		 * Notifies that the transaction is not valid on the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void receiptInvalid(const Purchase& purchase) {};

		/**
		 * Notifies that an error occurred while verifying the receipt.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 * @param errorCode The reason why it failed.
		 */
		virtual void receiptError(const Purchase& purchase,
			const int errorCode) {};

	};

} // namespace IAP

#endif /* IAP_PURCHASE_LISTENER_H_ */

/*! @} */
