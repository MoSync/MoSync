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
 * @file Test6.h
 * @author emma
 *
 * @brief Test case 6: VerifyReceipt for an item that cannot be purchased.
 * IOS:
 *	1. Create new purchase object.
 * 	2. Validation fails, fails because the item is not available: receive event productInvalid.
 * 	3. Call verifyReceipt and check the error event.
 * 	Expected event: receiptError, with errorCode = MA_PURCHASE_ERROR_NO_RECEIPT.
 * ANDROID:
 *	1. Create new purchase object.
 * 	2. Try to buy the item, purchase fails because the item is not available: receive
 * 	event requestFailed with error code MA_PURCHASE_ERROR_INVALID_PRODUCT.
 * 	3. Call verifyReceipt and check the error event.
 * 	Expected event: receiptError, with errorCode = MA_PURCHASE_ERROR_NO_RECEIPT.
 */

#ifndef PURCHASE_TEST6_H_
#define PURCHASE_TEST6_H_

#include <Purchase/Purchase.h>

#include "ITest.h"
#include "../Controller/IApplicationController.h"
#include "../Util.h"

using namespace IAP;

namespace PurchaseTest
{

	/**
	 * @brief VerifyReceipt for an item that cannot be purchased.
	 * Platform: Android and iOS.
	 */
	class Test6: public ITest, public PurchaseListener
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test6(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test6();

		/**
		 * Start the test.
		 */
		virtual void startTest();

		/**
		 * Get the name of the test.
		 * @return Test's name.
		 */
		virtual MAUtil::String getTestName() const;

		// From PurchaseListener:
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
		 * Notifies that the product is not valid on the App Store.
		 * Platform: iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void productInvalid(const Purchase& purchase);

		/**
		 * Notifies that the transaction has been successfully processed.
		 * The user should receive the purchased product.
		 * Platform: Android and iOS.
		 * @param purchase The object that sent the event.
		 */
		virtual void requestCompleted(const Purchase& purchase);

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
	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;
		Purchase* mPurchase;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST6_H_ */
