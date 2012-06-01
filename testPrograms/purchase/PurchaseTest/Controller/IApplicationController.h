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
 * @file IApplicationController.h
 * @author Bogdan Iusco
 * @date 8 May 2012
 *
 * @brief Interface for application controller.
 */

#ifndef PURCHASE_IAPPLICATION_CONTROLLER_H_
#define PURCHASE_IAPPLICATION_CONTROLLER_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	// Forward declaration
	class ITest;
	class IDatabaseManager;

	/**
	 * Interface for controller.
	 */
	class IApplicationController
	{
	public:
		/**
		 * Notify controller that a test has failed.
		 * @param test Test that failed.
		 */
		virtual void testFailed(ITest& test) = 0;

		/**
		 * Notify controller that a test has successfully finished.
		 * @param test That that has finished.
		 */
		virtual void testSucceeded(ITest& test) = 0;

		/**
		 * Print log on the screen.
		 * @param test Text to be printed.
		 */
		virtual void log(const MAUtil::String& test) = 0;

		/**
		 * Get the interface to database.
		 * @return The database's interface.
		 */
		virtual IDatabaseManager& getDatabase() const = 0;

	};

} // end of PurchaseApp

#endif /* PURCHASE_IAPPLICATION_CONTROLLER_H_ */
