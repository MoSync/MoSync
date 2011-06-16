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
 * @file CheckBox.cpp
 * @author Emma Tresanszki
 *
 * Class for check boxes. Create a check box instance by
 * using the WidgetManager.
 */

#include "CheckBox.h"

namespace MoSync
{
	namespace UI
	{

	CheckBox::CheckBox() :
		Widget(MAW_CHECK_BOX)
	{
		// Set some common default values to
		// simplify when creating buttons.
//		this->fillSpaceHorizontally();
		this->wrapContentVertically();
	}

	/**
	 * Destructor.
	 */
	CheckBox::~CheckBox()
	{
	}

	/*
	 * Sets the current check box state.
	 * @param checkboxState The current state.
	 */
	void CheckBox::setCheckedState(bool checkboxState)
	{
		// 0 unchecked, 1 checked
		setProperty(MAW_CHECK_BOX_CHECKED, checkboxState);
	}

	/*
	 * Gets the current check box state.
	 * @return The check box state.
	 */
	bool CheckBox::getCheckedState()
	{
		MAUtil::String checked = getPropertyString(MAW_CHECK_BOX_CHECKED);

		if ( checked == "true")
		{
			return true;
		}
		return false;
	}

	} // namespace UI
} // namespace MoSync
