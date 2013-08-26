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
