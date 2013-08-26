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
		 */
		double getPrice();

		/**
		 * Get the localized product title.
		 */
		MAUtil::String getTitle();

		/**
		 * Get the localized product description.
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
