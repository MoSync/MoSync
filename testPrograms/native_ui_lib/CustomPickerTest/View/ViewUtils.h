/*
 Copyright (C) 2013 MoSync AB

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
 * @file ViewUtils.h
 * @author Bogdan Iusco
 *
 * @brief UI utility functions.
 */

#include <mastdlib.h>
#include <conprint.h>

#ifndef CPT_VIEW_UTILS_H_
#define CPT_VIEW_UTILS_H_

namespace CustomPickerTest
{

	// Buffer size constant.
	const int kBufferSize =  256;

	/**
	 * Get a random color.
	 * @return Color in hex representation.
	 */
	static int getRandomColor()
	{
		int red = rand() / 255;
		int green = rand() / 255;
		int blue = rand() / 255;
		int hexColor = (red << 16) + (green << 8) + blue;
		return hexColor;
	}

} // end of CustomPickerTest

#endif /* CPT_VIEW_UTILS_H_ */
