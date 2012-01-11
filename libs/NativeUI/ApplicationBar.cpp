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
 * @file ApplicationBar.cpp
 * @author Ciprian Filipas
 *
 * Class that represents a visible applicationBar. Only one applicationBar
 * is visible at a time.
 */

#include "ApplicationBar.h"
#include <MAutil/String.h>

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	ApplicationBar::ApplicationBar() :
		Widget(MAW_APPLICATION_BAR)
	{
	}

	/**
	 * Destructor.
	 */
	ApplicationBar::~ApplicationBar()
	{
	}

	/**
	 * Set the title of the screen.
	 * The title is used by tab screen to display a text on the tab indicator.
	 * @param title The screen title.
	 */
	void ApplicationBar::setIsVisible(bool visibility)
	{
		if(visibility)
		{
			setProperty(MAW_APPLICATION_BAR_IS_VISIBLE, "true");
		}
		else
		{
			setProperty(MAW_APPLICATION_BAR_IS_VISIBLE, "false");
		}
	}

	/**
	 * Get the IsVisible property of the application bar
	 * @return isVisible bool
	 */
	bool ApplicationBar::getIsVisible()
	{
		MAUtil::String s = this->getPropertyString(MAW_APPLICATION_BAR_IS_VISIBLE);
		if(strcmp(s.c_str(), "true"))
			return true;
		else return false;
	}
} // namespace NativeUI
