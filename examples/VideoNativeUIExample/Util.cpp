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
 * @file Util.cpp
 * @author Emma Tresanszki
 *
 * This file contains the utility functions and values for the application.
 */

#include <maapi.h>

#include <maprofile.h>		// Profile database

#include "Util.h"

int screenWidth;
int screenHeight;

/**
 * Get the screen size.
 */
void getScreenSize()
{
	MAExtent screenSize = maGetScrSize();
	screenWidth = EXTENT_X(screenSize);
	screenHeight = EXTENT_Y(screenSize);
}

int getScreenWidth()
{
	return screenWidth;
}

int getScreenHeight()
{
	return screenHeight;
}

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid()
{
	if (NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
	{
		return true;
	}
	else
	{
		return false;
	}
}
