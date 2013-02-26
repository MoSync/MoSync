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
 * @file ImagePropertiesScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PathPropertyScreen.
 */

#ifndef IMAGE_PROPERTIES_SCREEN_LISTENER_H_
#define IMAGE_PROPERTIES_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageButtonTest
{

	class ImagePropertiesScreenListener
	{
	public:
		/**
		 * Display the foreground image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectForegroundImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the foreground image from resources.
		 */
		virtual void selectForegroundImageFromHandle() = 0;

		/**
		 * Show previous set foreground image path.
		 */
		virtual void showForegroundImagePath() = 0;

		/**
		 * Display the background image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectBackgroundImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the background image from resources.
		 */
		virtual void selectBackgroundImageFromHandle() = 0;

		/**
		 * Show previous set background image path.
		 */
		virtual void showBackgroundImagePath() = 0;
	};

} // end of ImageButtonTest

#endif /* IMAGE_PROPERTIES_SCREEN_LISTENER_H_ */
