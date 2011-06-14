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
 * @file TabScreen.h
 * @author Niklas Nummelin & Mikael Kindborg
 *
 * Class that represents a screen with tabs.
 */

#ifndef MOSYNC_UI_TABSCREEN_H_
#define MOSYNC_UI_TABSCREEN_H_

#include "Screen.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class that represents a tab screen.
	 */
	class TabScreen : public Screen
	{
	public:
		/**
		 * Constructor.
		 */
		TabScreen();

		/**
		 * Destructor.
		 */
		virtual ~TabScreen();

		/**
		 * Add a new tab with a screen in it.
		 * @param screen The screen shown in the new tab.
		 */
		virtual void addTab(Screen* screen);

		/**
		 * Show a given tab.
		 * @param index The index of the tab to show.
		 * Index starts at zero.
		 */
		virtual void setActiveTab(int index);

		// TODO: Add method for setting the tab icon.
	};

	} // namespace UI
} // namespace MoSync

#endif
