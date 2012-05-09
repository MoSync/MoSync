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

namespace PurchaseTest
{

	/**
	 * @brief Test a valid purchase of an product.
	 * Platform: Android and iOS.
	 */
	class Test1: public ITest
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

		/**
		 * Get the reason why the test failed.
		 * @return Reason why it failed.
		 */
		virtual MAUtil::String getReason();

	private:
		/**
		 * Application controller.
		 */
		IApplicationController& mApplicationController;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_TEST1_H_ */
