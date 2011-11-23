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
 *
 * Utility functions for the application.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <maprofile.h>	// Profile database.
#include <mastring.h>		// C string functions

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
static bool isAndroid()
{
	if ( NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
static bool isIOS()
{
	if ( NULL != strstr(MA_PROF_STRING_PLATFORM, "iphoneos"))
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif /* UTIL_H_ */
