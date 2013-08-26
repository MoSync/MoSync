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
 * Util.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Spiridon Alexandru
 */

#include <maapi.h>

#include "Util.h"

#define BUF_MAX 256

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);
	if ( strcmp(platform,"Android") == 0 )
	{
		return true;
	}

	return false;
}

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool isIOS()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);
	for (unsigned int i = 0; i < strlen(platform); i++)
	{
		platform[i] = tolower(platform[i]);
	}
	if (strstr(platform,"iphone") != NULL)
	{
		return true;
	}

	return false;
}

/**
 * Detects if the current platform is Windows Phone.
 * @return true if the platform is Windows Phone, false otherwise.
 */
bool isWindowsPhone()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);
	for (unsigned int i = 0; i < strlen(platform); i++)
	{
		platform[i] = tolower(platform[i]);
	}
	if (strstr(platform,"microsoft") != NULL &&
		strstr(platform,"windows") != NULL)
	{
		return true;
	}

	return false;
}


/**
 * Get the string associated with a ListViewItemSelectionStyle.
 * @param style The given style.
 * @return The string associated with the given style.
 */
MAUtil::String getSelectionStyleString(NativeUI::ListViewItemSelectionStyle style)
{
	MAUtil::String returnedValue;
	switch (style)
	{
	case NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_BLUE:
		returnedValue = "blue";
		break;
	case NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_GRAY:
		returnedValue = "gray";
		break;
	case NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_NONE:
	default:
		returnedValue = "none";
		break;
	}
	return returnedValue;
}
