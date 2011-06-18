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
 * @file NavigationBar.cpp
 * @author Emma Tresanszki
 *
 * Class for navigation bars.
 *
 * This widget is available only on iOS.
 */

#include "NavigationBar.h"

namespace MoSync
{
	namespace UI
	{

	NavigationBar::NavigationBar() :
		Widget("NavBar")
	{
		// Set some common default values to
		// simplify when creating navigation bars.
		this->fillSpaceHorizontally();
		this->wrapContentVertically();
	}

	/**
	 * Destructor.
	 */
	NavigationBar::~NavigationBar()
	{
	}

	/**
	 * Sets the title of the navigation bar.
	 * @param title A String object.
	 */
	void NavigationBar::setTitle(const MAUtil::String& title)
	{
		this->setProperty("title", title);
	}

	/**
	 * Sets the title of the back button.
	 * @param title A String object.
	 */
	void NavigationBar::setBackButtonTitle(const MAUtil::String& title)
	{
		this->setProperty("backButtonTitle", title);
	}

	} // namespace UI
} // namespace MoSync
