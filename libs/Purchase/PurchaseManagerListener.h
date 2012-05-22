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
 * @file PurchaseManagerListener.h
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief The PurchaseManagerListener can be used to listen for restored or
 * refunded products.
 */

#ifndef IAP_PURCHASE_MANAGER_LISTENER_H_
#define IAP_PURCHASE_MANAGER_LISTENER_H_

namespace IAP
{
	// Forward declaration.
	class Product;

	/**
	 * @brief Listener for restored or refunded products.
	 */
	class PurchaseManagerListener
	{
	public:
		/**
		 * Notifies that a purchase has been restored.
		 * Platform: iOS and Android.
		 * @param purchase The purchase that has been restored.
		 */
		virtual void purchaseRestored(Purchase& purchase) = 0;

		/**
		 * Notifies that restoreTransactions() has failed.
		 * @param errorCode The reason why it failed.
		 * Platform: iOS and Android.
		 */
		virtual void purchaseRestoreError(int errorCode) = 0;

		/**
		 * Notifies that a purchase has been refunded.
		 * Platform: Android.
		 * @param purchase The purchase that has been refunded.
		 */
		virtual void purchaseRefunded(Purchase& purchase) = 0;
	};

} // namespace IAP

#endif /* IAP_PURCHASE_MANAGER_LISTENER_H_ */

/*! @} */
