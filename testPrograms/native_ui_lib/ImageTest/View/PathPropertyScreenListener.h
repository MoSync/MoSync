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
 * @file PathPropertyScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PathPropertyScreen.
 */

#ifndef PATH_PROPERTY_SCREEN_LISTENER_H_
#define PATH_PROPERTY_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageTest
{

	class PathPropertyScreenListener
	{
	public:
		/**
		 * Display a image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the image from resources.
		 */
		virtual void selectImageFromHandle() = 0;

		/**
		 * Show previous set image path.
		 */
		virtual void showImagePath() = 0;
	};

}

#endif /* PATH_PROPERTY_SCREEN_LISTENER_H_ */
