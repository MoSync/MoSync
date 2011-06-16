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
 * @file Image.cpp
 * @author Mikael Kindborg
 *
 * Class for layout of widgets. Create a layout instance by
 * using the WidgetManager.
 */

#include "Image.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	Image::Image() :
		Widget(MAW_IMAGE)
	{
		// Set some common default values.
		this->fillSpaceVertically();
		this->fillSpaceHorizontally();
	}
	/**
	 * Destructor.
	 */
	Image::~Image()
	{
	}

	/*
	 * Set the image resource of the image widget.
	 * @res The handle of the image resource.
	 */
	void Image::setResource(MAHandle res)
	{
		setProperty(MAW_IMAGE_IMAGE, res);
	}

	} // namespace UI
} // namespace MoSync
