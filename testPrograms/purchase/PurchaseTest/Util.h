/*
Copyright (C) 2011 MoSync AB

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
#include <mavsprintf.h>
#include <MAUtil/String.h>

#define IOS_PRODUCT_TYPE_1 "com.mosync.purchase2.consumable"
#define ANDROID_PRODUCT_TYPE_1 ""
#define IOS_PRODUCT_TYPE_2 "com.mosync.purchase2.nonconsumable"
#define ANDROID_PRODUCT_TYPE_2 ""


/**
 * On Android this is an managed product.
 * On iOS this is a consumable product.
 * This type of product can be purchased multiple times.
 */
static MAUtil::String* sProductType1 = NULL;

/**
 * On Android this is an unmanaged product.
 * On iOS this is a non-consumable product.
 * This type of product can be purchased only one time by a user.
 */
static MAUtil::String* sProductType2 = NULL;

#define BUF_MAX 256

enum platform_code{
	ANDROID = 0,
	IOS = 1,
	WINDOWSPHONE7 = 2
};

/**
 * Detects the current platform
 * @return platform_code specific for Android, iPhone OS or WindowsPhone
 */
static int getPlatform()
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

/**
 * Creates the product types.
 * Call this method when the application starts.
 */
static void createProductTypes()
{
	delete sProductType1;
	delete sProductType2;

	int platform = getPlatform();
	if (platform == ANDROID)
	{
		sProductType1 = new MAUtil::String(ANDROID_PRODUCT_TYPE_1);
		sProductType2 = new MAUtil::String(ANDROID_PRODUCT_TYPE_2);
	}
	else if (platform == IOS)
	{
		sProductType1 = new MAUtil::String(IOS_PRODUCT_TYPE_1);
		sProductType2 = new MAUtil::String(IOS_PRODUCT_TYPE_2);
	}
}

/**
 * Destroy the product type strings.
 * Call this method when the app exits.
 */
static void destroyProductTypes()
{
	delete sProductType1;
	delete sProductType2;
}

#endif /* PURCHASE_UTIL_H_ */
