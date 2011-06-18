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
 * @file NavigationBar.h
 * @author Emma Tresanszki
 *
 * Class for navigation bars.
 *
 * This widget is available only on iOS.
 */
#ifndef MOSYNC_UI_NAVIGATIONBAR_H_
#define MOSYNC_UI_NAVIGATIONBAR_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for navigation bars.
	 * Available only on iOS.
	 */
	class NavigationBar : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		NavigationBar();

		/**
		 * Destructor.
		 */
		virtual ~NavigationBar();

		/**
		 * Sets the title of the navigation bar.
		 * @param title A String object.
		 */
		virtual void setTitle(const MAUtil::String& title);

		/**
		 * Sets the title of the back button.
		 * @param title A String object.
		 */
		virtual void setBackButtonTitle(const MAUtil::String& title);
	};

	} // namespace UI
} // namespace MoSync

#endif
