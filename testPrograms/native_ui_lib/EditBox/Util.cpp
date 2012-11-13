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
 *  Util.cpp
 *
 *  Created on: Sept 28, 2012
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
