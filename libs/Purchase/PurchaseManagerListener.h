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
