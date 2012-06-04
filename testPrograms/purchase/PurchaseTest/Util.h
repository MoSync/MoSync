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
 * @file Util.h
 * @author Emma Tresanszki.
 *
 * Utility functions for the application.
 */

#ifndef PURCHASE_UTIL_H_
#define PURCHASE_UTIL_H_

#include <mastring.h>		// C string functions
#include <conprint.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>

namespace PurchaseTest
{
	// Usual size for a buffer.
	const int BUF_SIZE = 256;

	#define BUF_MAX 256

	enum platform_code
	{
		ANDROID = 0,
		IOS = 1,
		WINDOWSPHONE7 = 2
	};

	/**
	 * Detects the current platform
	 * @return platform_code specific for Android, iPhone OS or WindowsPhone
	 */
	extern int getPlatform();

	/**
	 * Store the products types used by the test classes.
	 */
	class ProductTypes
	{
	public:
		/**
		 * Return the instance to the singleton object.
		 */
		static ProductTypes& getInstance();

		/**
		 * Destroy the singleton object.
		 */
		static void destroyInstance();

		/**
		 * Get the first product type.
		 * On Android this is an managed product.
		 * On iOS this is a consumable product.
		 * This type of product can be purchased multiple times.
		 * @return The first product type.
		 */
		const MAUtil::String& getProductType1() const;

		/**
		 * Get the second product type.
		 * On Android this is an unmanaged product.
		 * On iOS this is a non-consumable product.
		 * This type of product can be purchased only one time by a user.
		 * @return The second product type.
		 */
		const MAUtil::String& getProductType2() const;

		/**
		 * Get the unavailable product type.
		 * This type of product cannot be purchased.
		 * @return The unavailable product type.
		 */
		const MAUtil::String& getProductTypeUnavailable() const;

		/**
		 * Get the cancelled product type.
		 * This type of product is used when testing a cancelled order.
		 * @return The cancelled product type.
		 */
		const MAUtil::String& getProductTypeCancelled() const;

		/**
		 * Get the refunded product type.
		 */
		const MAUtil::String& getProductTypeRefunded() const;
	private:
		/**
		 * Constructor.
		 */
		ProductTypes();

		/**
		 * Destructor.
		 */
		~ProductTypes();

	private:
		/**
		 * Class instance.
		 */
		static ProductTypes* mInstance;

		/**
		 * First product type.
		 * On Android this is an managed product.
		 * On iOS this is a consumable product.
		 * This type of product can be purchased multiple times.
		 */
		MAUtil::String* mProductType1;

		/**
		 * Unavailable product type.
		 * Android only.
		 */
		MAUtil::String* mProductTypeUnavailable;

		/**
		 * Second product type.
		 * On Android this is an unmanaged product.
		 * On iOS this is a non-consumable product.
		 * This type of product can be purchased only one time by a user.
		 */
		MAUtil::String* mProductType2;

		/**
		 * Cancelled product type.
		 * This type of product is used when testing the pruchase
		 * of a cancelled order.
		 */
		MAUtil::String* mProductTypeCancelled;

		/**
		 * Refunded product type.
		 * Available only on Android.
		 */
		MAUtil::String* mProductTypeRefunded;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_UTIL_H_ */
