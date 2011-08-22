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
 * @file EditBox.cpp
 * @author Emma Tresanszki
 *
 * Class for edit boxes.
 */

#include "EditBox.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	EditBox::EditBox():
		TextWidget(MAW_EDIT_BOX)
	{
		this->wrapContentVertically();
	}

	/**
	 * Destructor.
	 */
	EditBox::~EditBox()
	{
	}

	/*
	 * Specifies that the editing mode should be 'password'.
	 */
	void EditBox::setPasswordInputMode()
	{
		setProperty(MAW_EDIT_BOX_EDIT_MODE, "password");
	}

	/*
	 * Specifies that the editing mode should be 'text'.
	 */
	void EditBox::setTextInputMode()
	{
		setProperty(MAW_EDIT_BOX_EDIT_MODE, "text");
	}

	/*
	 * Set a text in the edit box that acts as a placeholder
	 * when an edit box is empty.
	 * @param text A null-terminated string
	 */
	void EditBox::setPlaceholder(const MAUtil::String& text)
	{
		setProperty(MAW_EDIT_BOX_PLACEHOLDER, text);
	}

	} // namespace UI
} // namespace MoSync
