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
 * @file Util.cpp
 * @author Bogdan Iusco
 *
 * Utility functions for the application.
 */

#include "Util.h"


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
