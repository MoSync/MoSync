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
 * @file ITest.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Base class for test classes.
 */

#ifndef PURCHASE_ITEST_H_
#define PURCHASE_ITEST_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	/**
	 * @brief Base class for test classes.
	 */
	class ITest
	{
	public:

		/**
		 * Constructor.
		 */
		ITest();

		/**
		 * Start the test.
		 */
		virtual void startTest() = 0;

		/**
		 * Get the name of the test.
		 * @return Test's name.
		 */
		virtual MAUtil::String getTestName() const = 0;

		/**
		 * Get the reason why the test failed.
		 * @return Reason why it failed.
		 * If the test has not finished or if it succeeded the returned string
		 * will be empty.
		 */
		virtual MAUtil::String getReason();

		/**
		 * Destructor.
		 */
		virtual ~ITest();

	protected:
		/**
		 * Set the reason why the test failed.
		 * @param reason The reason why it failed.
		 */
		void setFailedReason(const MAUtil::String& reason);

	private:
		/**
		 * In case the test fails this string will contain the reason.
		 * If the test has not started or if it succeeded this string will be
		 * NULL.
		 */
		MAUtil::String* mFailedReason;
	};
} // namespace PurchaseTest


#endif /* PURCHASE_ITEST_H_ */
