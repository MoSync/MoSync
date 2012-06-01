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
 * @file RelativeLayout.cpp
 * @author Emma Tresanszki
 *
 * Class for relative layout of widgets.
 */

// String delimitator for MAW_RELATIVE_LAYOUT_CONTENT_OFFSET property.
#define DELIM_DASH "-"

#include <MAUtil/String.h>
#include <MAUtil/util.h>
#include <MAUtil/Vector.h>

#include "RelativeLayout.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	RelativeLayout::RelativeLayout() :
		Layout(MAW_RELATIVE_LAYOUT)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	RelativeLayout::~RelativeLayout()
	{
	}

	/**
	 * Sets the content offset.
	 * The layout will be scrolled to the given coordinate so its
	 * left-top visible corner will match the content offset point.
	 *
	 * Platform: iOS
	 * @param xCoord The left coordinate of the content offset point.
	 * @param yCoord The top coordinate of the content offset point.
	 */
	void RelativeLayout::setContentOffset(const int xCoord, const int yCoord)
	{
		MAUtil::String leftCoord = MAUtil::integerToString(xCoord);
		MAUtil::String topCoord = MAUtil::integerToString(yCoord);
		MAUtil::String value = leftCoord + DELIM_DASH + topCoord;
		this->setProperty(
			MAW_RELATIVE_LAYOUT_CONTENT_OFFSET,
			value);
	}

	/**
	 * Get the x coordinate of the content offset point.
	 * The content offset point repesents the visible top-left point
	 * of a layout.
	 * Platform: iOS.
	 * @return The left coordinate.
	 */
	int RelativeLayout::getContentOffsetCoordX()
	{
		MAUtil::String contentOffset = this->getPropertyString(
			MAW_RELATIVE_LAYOUT_CONTENT_OFFSET);
		MAUtil::Vector<MAUtil::String> coordinate;
		MAUtil::stringSplit(contentOffset, DELIM_DASH, coordinate);
		if (coordinate.size() != 2)
		{
			return 0;
		}
		return MAUtil::stringToInteger(coordinate[0]);
	}

	/**
	 * Get the y coordinate of the content offset point.
	 * The content offset point repesents the visible top-left point
	 * of a layout.
	 * Platform: iOS.
	 * @return The top coordinate.
	 */
	int RelativeLayout::getContentOffsetCoordY()
	{
		MAUtil::String contentOffset = this->getPropertyString(
			MAW_RELATIVE_LAYOUT_CONTENT_OFFSET);
		MAUtil::Vector<MAUtil::String> coordinate;
		MAUtil::stringSplit(contentOffset, DELIM_DASH, coordinate);
		if (coordinate.size() != 2)
		{
			return 0;
		}
		return MAUtil::stringToInteger(coordinate[1]);
	}

} // namespace NativeUI
