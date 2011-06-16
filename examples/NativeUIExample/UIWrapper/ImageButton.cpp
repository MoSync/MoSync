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
 * @file ImageButton.cpp
 * @author Bogdan Iusco
 *
 * Class for image buttons.
 */

#include "ImageButton.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	ImageButton::ImageButton() :
		Widget(MAW_IMAGE_BUTTON)
	{
		// Set default values.
		this->wrapContentHorizontally();
		this->wrapContentVertically();
	}

	/**
	 * Destructor.
	 */
	ImageButton::~ImageButton()
	{
	}

	/**
	 * Set a image for this button.
	 * @param imageHandle An image handle.
	 */
	void ImageButton::setImage(MAHandle imageHandle)
	{
		setProperty(
			MAW_IMAGE_BUTTON_BACKGROUND_IMAGE,
			imageHandle);
	}

	} // namespace UI
} // namespace MoSync
