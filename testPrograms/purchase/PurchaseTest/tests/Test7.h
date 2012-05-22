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
 * @file Test7.h
 * @author emma
 *
 * @brief Test case 7:  Test checkPurchaseSupported.
 * Expected result: call returns MA_PURCHASE_RES_OK.
 */

#ifndef PURCHASE_TEST7_H_
#define PURCHASE_TEST7_H_

#include <Purchase/Purchase.h>

#include "ITest.h"
#include "../Controller/IApplicationController.h"
#include "../Util.h"

using namespace IAP;

namespace PurchaseTest
{

	/**
	 * @brief Test checkPurchaseSupported.
	 * Platform: Android and iOS.
	 */
	class Test7: public ITest
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test7(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test7();

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
		 * Application controller.
		 */
		IApplicationController& mApplicationController;
		Purchase* mPurchase;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST7_H_ */
