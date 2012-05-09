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

#ifndef PURCHASE_MANAGER_LISTENER_H_
#define PURCHASE_MANAGER_LISTENER_H_

namespace Purchase
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
		 * Notifies that a product has been restored.
		 * Platform: iOS.
		 * @param product The product that has been restored.
		 */
		virtual void productRestored(Product& product) = 0;

		/**
		 * Notifies that a product has been refunded.
		 * Platform: Android.
		 * @param product The product that has been refunded.
		 */
		virtual void productRefunded(Product& product) = 0;
	};
}


#endif /* PURCHASE_MANAGER_LISTENER_H_ */

/*! @} */
