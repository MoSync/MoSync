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
 * @brief Interface for test classes.
 */

#ifndef PURCHASE_ITEST_H_
#define PURCHASE_ITEST_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	class ITest
	{
	public:
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
		 */
		virtual MAUtil::String getReason() = 0;

		/**
		 * Destructor.
		 */
		virtual ~ITest() {};
	};
} // namespace PurchaseTest


#endif /* PURCHASE_ITEST_H_ */
