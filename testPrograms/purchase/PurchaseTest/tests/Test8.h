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
 * @file Test8.h
 * @author Bogdan Iusco
 * @date 15 May 2012
 *
 * @brief Create two products using the same product handle.
 * Expected result: the second product should receive an purchase
 * event with MA_PURCHASE_STATE_DUPLICATE_HANDLE as state.
 */

#ifndef PURCHASE_TEST8_H_
#define PURCHASE_TEST8_H_

#include <MAUtil/Environment.h>

#include "ITest.h"
#include "../Controller/IApplicationController.h"


using namespace MAUtil;

namespace PurchaseTest
{

	/**
	 * @brief Test duplicate handle for a product.
	 * Platform: Android and iOS.
	 */
	class Test8:
		public ITest,
		public CustomEventListener
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Will be notified when test's status
		 * changes.
		 */
		Test8(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		virtual ~Test8();

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
		 * Handle a new event.
		 * @param event The new event.
		 */
		virtual void customEvent(const MAEvent& event);

	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;

		/**
		 * Handle to the first product.
		 */
		MAHandle mFirstProduct;

		/**
		 * Handle to the second product.
		 * This product should be invalid because it has the same handle as the
		 * first one.
		 */
		MAHandle mSecondProduct;

		/**
		 * Flag to indicate if the first product was created.
		 */
		bool mIsFirstProductCreated;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST8_H_ */
