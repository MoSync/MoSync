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
 * @file OptionsDialogListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for OptionsDialog events.
 */

#ifndef NATIVEUI_OPTIONS_BOX_LISTENER_H_
#define NATIVEUI_OPTIONS_BOX_LISTENER_H_

#include <MAUtil/String.h>

	/**
	 * \brief Listener for OptionsDialog events.
	 */
	class OptionsBoxListener
	{
	public:
		/**
		 * This method is called when the destructive button from options
		 * dialog was clicked.
		 */
		virtual void optionsBoxDestructiveButtonClicked()
		{
		}
		;

		/**
		 * This method is called when the cancel button from options dialog was
		 * clicked.
		 */
		virtual void optionsBoxCancelButtonClicked()
		{
		}
		;

		/**
		 * This method is called when a button from options dialog was
		 * clicked.
		 * This method is not called if the destructive or cancel button were
		 * clicked.
		 * @param buttonIndex The index of the button that was clicked.
		 * @param buttonTitle The title of the button that was clicked.
		 */
		virtual void optionsBoxButtonClicked(const int buttonIndex,
			const MAUtil::WString& buttonTitle)
		{
		}
		;
	};

#endif /* NATIVEUI_OPTIONS_BOX_LISTENER_H_ */
