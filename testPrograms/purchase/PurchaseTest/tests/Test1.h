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
 * @file Test1.h
 * @author Bogdan Iusco
 * @date 9 May 2012
 *
 * @brief Test a valid purchase of an product.
 */

#ifndef PURCHASE_TEST1_H_
#define PURCHASE_TEST1_H_

#include "ITest.h"
#include "../Controller/IApplicationController.h"

#include "../wrapper/PurchaseListener.h"

using namespace IAP;

namespace PurchaseTest
{

	class IAP::Purchase;

	/**
	 * @brief Test a valid purchase of an product.
	 * Platform: Android and iOS.
	 */
	class Test1:
		public ITest,
		public PurchaseListener
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test1(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test1();

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
		 * Platform: iOS.
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

	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;

		/**
		 * Tested object.
		 */
		Purchase* mPurchase;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST1_H_ */
