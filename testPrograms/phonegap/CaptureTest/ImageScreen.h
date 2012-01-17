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
 * @file ImageScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a image.
 */

#ifndef IMAGESCREEN_H_
#define IMAGESCREEN_H_


// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>

using namespace NativeUI;
using namespace MAUtil;

/**
 * @brief Screen used for displaying a image.
 */
class ImageScreen:
	public Screen
{
public:
	/**
	 * Constructor.
	 */
	ImageScreen();

	/**
	 * Destructor.
	 */
	virtual ~ImageScreen();

	/**
	 * Set currently displayed image.
	 * @param handle Handle to a image.
	 */
	void setImage(MAHandle handle);

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

private:
	/**
	 * Used for displaying a image.
	 */
	Image* mImage;
};


#endif /* IMAGESCREEN_H_ */
