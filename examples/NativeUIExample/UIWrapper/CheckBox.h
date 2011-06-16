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
 * @file CheckBox.h
 * @author Emma Tresanszki
 *
 * Class for check boxes. Create a check box instance by
 * using the WidgetManager.
 */

#ifndef MOSYNC_UI_CHECKBOX_H_
#define MOSYNC_UI_CHECKBOX_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for check boxes.
	 */
	class CheckBox : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		CheckBox();

		/**
		 * Destructor.
		 */
		virtual ~CheckBox();

		/*
		 * Sets the current check box state.
		 * @param checkboxState The current state.
		 */
		virtual void setCheckedState(bool checkboxState);

		/*
		 * Gets the current check box state.
		 * @return The check box state.
		 */
		virtual bool getCheckedState();
	};

	} // namespace UI
} // namespace MoSync

#endif
