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
 * @file EditBox.h
 * @author Emma Tresanszki
 *
 * Class for edit boxes.
 */
#ifndef MOSYNC_UI_EDITBOX_H_
#define MOSYNC_UI_EDITBOX_H_

#include "TextWidget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for edit boxes.
	 */
	class EditBox : public TextWidget
	{
	public:
		/**
		 * Constructor.
		 */
		EditBox();

		/**
		 * Destructor.
		 */
		virtual ~EditBox();

		/*
		 * Specifies that the editing mode should be 'password'.
		 */
		virtual void setPasswordInputMode();

		/*
		 * Specifies that the editing mode should be 'text'.
		 */
		virtual void setTextInputMode();

		/*
		 * Set a text in the edit box that acts as a placeholder
		 * when an edit box is empty.
		 * @param text A null-terminated string
		 */
		virtual void setPlaceholder(const MAUtil::String& text);
	};

	} // namespace UI
} // namespace MoSync

#endif /* MOSYNC_UI_EDITBOX_H_ */
