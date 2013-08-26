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
 * @file Util.cpp
 * @author Bogdan Iusco
 *
 * Utility functions for the application.
 */


#define IOS_PRODUCT_TYPE_1 "com.mosync.purchase2.consumable2"
// This test app can be purchased multiple times.
#define ANDROID_PRODUCT_TYPE_PURCHASED "android.test.purchased"
#define ANDROID_PRODUCT_TYPE_CANCELLED "android.test.canceled"
#define ANDROID_PRODUCT_TYPE_UNAVAILABLE "android.test.item_unavailable"
#define ANDROID_PRODUCT_TYPE_REFUNDED "android.test.refunded"
#define IOS_PRODUCT_TYPE_2 "com.mosync.purchase2.nonconsumable"
#define IOS_PRODUCT_TYPE_UNAVAILABLE "invalid_product"

#include "Util.h"

namespace PurchaseTest
{
	/**
	 * Detects the current platform
	 * @return platform_code specific for Android, iPhone OS or WindowsPhone
	 */
	int getPlatform()
	{
		char platform[BUF_MAX];
		maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

		if(strcmp(platform, "Android") == 0)
		{
			return ANDROID;
		}
		else
		{
			if(strcmp(platform, "iPhone OS") == 0)
				return IOS;
		}
		return WINDOWSPHONE7;
	}


	ProductTypes* ProductTypes::mInstance = NULL;

	/**
	 * Constructor.
	 */
	ProductTypes::ProductTypes():
		mProductType1(NULL),
		mProductType2(NULL)
	{
		int platform = getPlatform();
		if (platform == ANDROID)
		{
			mProductType1 = new MAUtil::String(ANDROID_PRODUCT_TYPE_PURCHASED);
			mProductType2 = new MAUtil::String(ANDROID_PRODUCT_TYPE_PURCHASED);
			mProductTypeUnavailable = new MAUtil::String(ANDROID_PRODUCT_TYPE_UNAVAILABLE);
			mProductTypeCancelled = new MAUtil::String(ANDROID_PRODUCT_TYPE_CANCELLED);
			mProductTypeRefunded = new MAUtil::String(ANDROID_PRODUCT_TYPE_REFUNDED);
		}
		else if (platform == IOS)
		{
			mProductType1 = new MAUtil::String(IOS_PRODUCT_TYPE_1);
			mProductType2 = new MAUtil::String(IOS_PRODUCT_TYPE_2);
			mProductTypeUnavailable = new MAUtil::String(IOS_PRODUCT_TYPE_UNAVAILABLE);
			mProductTypeCancelled = new MAUtil::String(IOS_PRODUCT_TYPE_1);
		}
	}

	/**
	 * Destructor.
	 */
	ProductTypes::~ProductTypes()
	{
		delete mProductType1;
		delete mProductType2;
		delete mProductTypeCancelled;
		delete mProductTypeUnavailable;
		delete mProductTypeRefunded;
	}

	/**
	 * Return the instance to the singleton object.
	 */
	ProductTypes& ProductTypes::getInstance()
	{
		if (!mInstance)
		{
			mInstance = new ProductTypes();
		}
		return *mInstance;
	}

	/**
	 * Destroy the singleton object.
	 */
	void ProductTypes::destroyInstance()
	{
		delete mInstance;
	}

	/**
	 * Get the first product type.
	 * On Android this is an managed product.
	 * On iOS this is a consumable product.
	 * This type of product can be purchased multiple times.
	 * @return The first product type.
	 */
	const MAUtil::String& ProductTypes::getProductType1() const
	{
		return *mProductType1;
	}

	/**
	 * Get the second product type.
	 * On Android this is an unmanaged product.
	 * On iOS this is a non-consumable product.
	 * This type of product can be purchased only one time by a user.
	 * @return The second product type.
	 */
	const MAUtil::String& ProductTypes::getProductType2() const
	{
		return *mProductType2;
	}

	/**
	 * Get the unavailable product type.
	 * This type of product cannot be purchased.
	 * @return The unavailable product type.
	 */
	const MAUtil::String& ProductTypes::getProductTypeUnavailable() const
	{
		return *mProductTypeUnavailable;
	}

	/**
	 * Get the cancelled product type.
	 * This type of product is used when testing a cancelled order.
	 * @return The cancelled product type.
	 */
	const MAUtil::String& ProductTypes::getProductTypeCancelled() const
	{
		return *mProductTypeCancelled;
	}

	/**
	 * Get the refunded product type.
	 */
	const MAUtil::String& ProductTypes::getProductTypeRefunded() const
	{
		return *mProductTypeRefunded;
	}
} // namespace Purchase
