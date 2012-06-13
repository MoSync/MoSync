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
 * @file Receipt.h
 * @author Bogdan Iusco
 * @date 7 May 2012
 *
 * @brief A Receipt wraps the details of a purchase.
 */

#ifndef IAP_PURCHASE_RECEIPT_H_
#define IAP_PURCHASE_RECEIPT_H_

#include <MAUtil/String.h>
#include <maapi.h>

namespace IAP
{

	/**
	 * @brief A Receipt wraps the details of a purchase.
	 */
	class Receipt
	{
	public:
		/**
		 * Constructor.
		 * @param productHandle Handle to the receipt's product.
		 */
		Receipt(MAHandle productHandle);

		/**
		 * Copy constructor.
		 */
		Receipt(const Receipt& receipt);

		/**
		 * Destructor.
		 */
		~Receipt() {};

		/**
		 * Get the product id of the item that was purchased.
		 * Platform: Android and iOS.
		 */
		MAUtil::String getProductID();

		/**
		 * Get the transaction identifier of the item that has been purchased.
		 * Platform: iOS and Android.
		 */
		MAUtil::String getTransactionID();

		/**
		 * Get the transaction date and time of the item that has been purchased.
		 * Platform: Android and iOS.
		 * @return Time in seconds since January 1st, 1970.
		 */
		int getTransactionDate();

		/**
		 * Get a string that the App Store/Google Play uses to uniquely identify
		 * the application that created the payment transaction. If your server
		 * supports multipl applications, you can use this value to differentiate
		 * between them.
		 * Applications that are executing in the sandbox do not yet have an
		 * app-item-id assigned to them,so this key is missing from receipts created
		 * by the sandbox.
		 * Platform: iOS and Android.
		 */
		MAUtil::String getAppID();

		/**
		 * Get the arbitrary number that uniquely identifies a revision of your
		 * application.
		 * This key is missing in receipts created by the sandbox.
		 * Platform: iOS.
		 */
		MAUtil::String getVersionExternalID();

		/**
		 * Get the bundle identifier for the application.
		 * Platform: iOS.
		 */
		MAUtil::String getBID();

		/**
		 * Get the version number of the application.
		 * Platform: iOS.
		 */
		MAUtil::String getBVRS();

		/**
		 * The cost of the product in the local currency.
		 * Platform: iOS.
		 */
		double getPrice();

		/**
		 * Get the localized product title.
		 * Platform: iOS.
		 */
		MAUtil::String getTitle();

		/**
		 * Get the localized product description.
		 * Platform: iOS.
		 */
		MAUtil::String getDescription();

		/**
		 * Get a receipt field value.
		 * Platform: Android and iOS.
		 * @param field One of the MA_PURCHASE_RECEIPT constants.
		 * @return The field value, or a empty string in case of error.
		 */
		MAUtil::String getFieldValue(const MAUtil::String field);

	private:
		/**
		 * Handle to the product that onws the receipt.
		 */
		MAHandle mHandle;
	};

} // namespace IAP

#endif /* IAP_PURCHASE_RECEIPT_H_ */

/*! @} */
