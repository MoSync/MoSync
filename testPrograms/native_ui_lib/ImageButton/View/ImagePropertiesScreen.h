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
 * @file ImagePropertiesScreen.h
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_BUTTON_IMAGE_PATH and
 * MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH properties.
 */

#ifndef IMAGE_BUTTON_PATH_PROPERTIES_SCREEN_H_
#define IMAGE_BUTTON_PATH_PROPERTIES_SCREEN_H_

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

	class ImagePropertiesScreenListener;

	class ImagePropertiesScreen:
		public NativeUI::Screen,
		public NativeUI::EditBoxListener,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param listener Listener for screen.
		 */
		ImagePropertiesScreen(ImagePropertiesScreenListener& listener);

		/**
		 * Destructor.
		 */
		virtual ~ImagePropertiesScreen();

		/**
		 * Set displayed foreground image path in the edit box.
		 * @param path Text to set.
		 */
		void setDisplayedForegoundImagePath(const MAUtil::String& path);

		/**
		 * Set displayed background image path in the edit box.
		 * @param path Text to set.
		 */
		void setDisplayedBackgoundImagePath(const MAUtil::String& path);

	private:
		/**
		 * Create screen's UI.
		 */
		void createUI();

		/**
		 * Create layout that contains:
		 * - a Label widget.
		 * - a EditBox widget that will be used to get a file path for the
		 * foreground image.
		 */
		void createEditBoxLayoutForeground();

		/**
		 * Create an button that will be used to select the foreground image
		 * from a file path.
		 */
		void createForegroundImageFromPathButton();

		/**
		 * Create an button that will be used to select the foreground image
		 * from a handle.
		 */
		void createForegroundImageFromHandleButton();

		/**
		 * Create an button that will be used to display the foreground image path.
		 */
		void createGetForegroundImagePathButton();

		/**
		 * Create layout that contains:
		 * - a Label widget.
		 * - a EditBox widget that will be used to get a file path for the
		 * background image.
		 */
		void createEditBoxLayoutBackground();

		/**
		 * Create an button that will be used to select the background image
		 * from a file path.
		 */
		void createBackgroundImageFromPathButton();

		/**
		 * Create an button that will be used to select the background image
		 * from a handle.
		 */
		void createBackgroundImageFromHandleButton();

		/**
		 * Create an button that will be used to display the background image path.
		 */
		void createGetBackgroundImagePathButton();

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
		ImagePropertiesScreenListener& mListener;

		/**
		 * Main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Used to get the file path for the foreground image.
		 */
		NativeUI::EditBox* mPathForegroundEditBox;

		/**
		 * Used to get the file path for the background image.
		 */
		NativeUI::EditBox* mPathBackgroundEditBox;

		/**
		 * Used to load the foreground image from a file path.
		 */
		NativeUI::Button* mForegroundImageFromPathButton;

		/**
		 * Used to load an image from a handle.
		 */
		NativeUI::Button* mForegroundImageFromHandleButton;

		/**
		 * Used to test getter for MAW_IMAGE_BUTTON_IMAGE_PATH.
		 */
		NativeUI::Button* mGetForegroundImagePathButton;

		/**
		 * Used to load the background image from a file path.
		 */
		NativeUI::Button* mBackgroundImageFromPathButton;

		/**
		 * Used to load the background image from a handle.
		 */
		NativeUI::Button* mBackgroundImageFromHandleButton;

		/**
		 * Used to test getter for MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH.
		 */
		NativeUI::Button* mGetBackgroundImagePathButton;

	}; // end of PathPropertyScreen

} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_PATH_PROPERTIES_SCREEN_H_ */
