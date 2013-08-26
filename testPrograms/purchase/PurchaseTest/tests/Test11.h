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

/**
 * @file Test11.h
 * @author emma
 *
 * @brief Test case 11: Test Refunded product on Android.
 *
 */

#ifndef PURCHASE_TEST11_H_
#define PURCHASE_TEST11_H_

#include <Purchase/Purchase.h>
#include <Purchase/PurchaseManager.h>
#include <Purchase/PurchaseManagerListener.h>

#include "ITest.h"
#include "../Controller/IApplicationController.h"
#include "../Util.h"

using namespace IAP;

namespace PurchaseTest
{

	/**
	 * @brief Test Refunded product on Android.
	 * Platform: Android and iOS.
	 */
	class Test11:
		public ITest,
		public PurchaseManagerListener,
		public PurchaseListener
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test11(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test11();

		/**
		 * Start the test.
		 */
		virtual void startTest();

		/**
		 * Get the name of the test.
		 * @return Test's name.
		 */
		virtual MAUtil::String getTestName() const;

	private:
		/**
		 * Notifies that the product has been validated by the App Store.
		 * NOTE: On Android there is no validation done at this step, if the
		 * product results to be unavailable, then the application will be later
		 * notified via a requestFailed() callback. The errorCode will equal
		 * #MA_PURCHASE_ERROR_INVALID_PRODUCT.
		 * Platform: iOS and Android.
		 * @param purchase The object that sent the event.
		 */
		virtual void productValid(const Purchase& purchase);
		/**
		 * Notifies that the transaction has been received by the App Store/
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void requestInProgress(const Purchase& purchase);

		/**
		 * Notifies that the transaction has been successfully processed.
		 * The user should receive the purchased product.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void requestCompleted(const Purchase& purchase);

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
			const int errorCode);

		/**
		 * Notifies that the transaction has been validated by the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 * @param receipt Transaction receipt.
		 */
		virtual void receiptValid(
			const Purchase& purchase,
			Receipt& receipt);

		/**
		 * Notifies that the transaction is not valid on the App Store /
		 * Google Play.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void receiptInvalid(const Purchase& purchase);

		/**
		 * Notifies that an error occurred while verifying the receipt.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 * @param errorCode The reason why it failed.
		 */
		virtual void receiptError(const Purchase& purchase,
			const int errorCode);

		/**
		 * Notifies that a purchase has been restored.
		 * Platform: iOS and Android.
		 * @param purchase The purchase that has been restored.
		 */
		virtual void purchaseRestored(Purchase& purchase){};

		/**
		 * Notifies that restoreTransactions() has failed.
		 * @param errorCode The reason why it failed.
		 * Platform: iOS and Android.
		 */
		virtual void purchaseRestoreError(int errorCode){};

		/**
		 * Notifies that a purchase has been refunded.
		 * Platform: Android.
		 * @param purchase The purchase that has been refunded.
		 */
		virtual void purchaseRefunded(Purchase& purchase);
	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;
		Purchase* mPurchase;Purchase* refunded;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST11_H_ */
