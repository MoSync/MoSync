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

#ifndef UTIL_H_
#define UTIL_H_

#include <mastring.h>		// C string functions
#include "mavsprintf.h"

#define BUF_MAX 256

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
static int getPlatform()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

	if(strcmp(platform, "Android") == 0)
	{
		return 0;
	}
	else
	{
		if(strcmp(platform, "iPhone OS") == 0)
			return 1;
	}
	return 2;
}

#endif /* UTIL_H_ */
