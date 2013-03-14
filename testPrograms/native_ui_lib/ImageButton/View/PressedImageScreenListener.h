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
 * @file PressedImageScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PressedImageScreen.
 */

#ifndef IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_
#define IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageButtonTest
{

	class PressedImageScreenListener
	{
	public:
		/**
		 * Display the pressed image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectPressedImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the pressed image from resources.
		 */
		virtual void selectPressedImageFromHandle() = 0;

		/**
		 * Show previous set pressed image handle.
		 */
		virtual void showPressedImageHandle() = 0;

		/**
		 * Show previous set pressed image path.
		 */
		virtual void showPressedImagePath() = 0;
	};

} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_ */
