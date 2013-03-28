/*
 Copyright (C) 2013 MoSync AB

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
 * @file PressedImageScreen.h
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_BUTTON_PRESSED_IMAGE and
 * MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH properties.
 */

#ifndef IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_H_
#define IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/EditBoxListener.h>
#include <NativeUI/Screen.h>
#include <MAUtil/String.h>

namespace NativeUI
{
	class Button;
	class EditBox;
	class VerticalLayout;
}

namespace ImageButtonTest
{
	class PressedImageScreenListener;

	class PressedImageScreen:
		public NativeUI::Screen,
		public NativeUI::EditBoxListener,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param listener Listener for screen.
		 */
		PressedImageScreen(PressedImageScreenListener& listener);

		/**
		 * Destructor.
		 */
		virtual ~PressedImageScreen();

		/**
		 * Set displayed pressed image path in the edit box.
		 * @param path Text to set.
		 */
		void setDisplayedPressedImagePath(const MAUtil::String& path);

	private:
		/**
		 * Create screen's UI.
		 */
		void createUI();

		/**
		 * Create layout that contains:
		 * - a Label widget.
		 * - a EditBox widget that will be used to get a file path for the
		 * pressed image.
		 */
		void createEditBoxLayout();

		/**
		 * Create an button that will be used to select the pressed image
		 * from a file path.
		 */
		void createPressedImageFromPathButton();

		/**
		 * Create an button that will be used to select the pressed image
		 * from a handle.
		 */
		void createPressedImageFromHandleButton();

		/**
		 * Create an button that will be used to display the pressed image
		 * handle.
		 */
		void createGetImageHandleButton();

		/**
		 * Create an button that will be used to display the pressed image path.
		 */
		void createGetPressedImagePathButton();

		/**
		 * This method is called when the return button was pressed.
		 * On iOS platform the virtual keyboard is not closed after
		 * receiving this event.
		 * This method is called only if the edit box mode is
		 * #MAW_EDIT_BOX_SINGLE_LINE.
		 * @param editBox The edit box object that generated the event.
		 */
		virtual void editBoxReturn(NativeUI::EditBox* editBox);

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * Platform: iOS, Android, Windows Phone.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

	private:
		/**
		 * Screen listener.
		 */
		PressedImageScreenListener& mListener;

		/**
		 * Main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Used to get the file path for the pressed image.
		 */
		NativeUI::EditBox* mPathEditBox;

		/**
		 * Used to load the pressed image from a file path.
		 */
		NativeUI::Button* mPressedImageFromPathButton;

		/**
		 * Used to load the pressed image from a handle.
		 */
		NativeUI::Button* mPressedImageFromHandleButton;

		/**
		 * Used to test getter for MAW_IMAGE_BUTTON_PRESSED_IMAGE.
		 */
		NativeUI::Button* mGetPressedImageHandleButton;

		/**
		 * Used to test getter for MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH.
		 */
		NativeUI::Button* mGetPressedImagePathButton;
	};
} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_H_ */
