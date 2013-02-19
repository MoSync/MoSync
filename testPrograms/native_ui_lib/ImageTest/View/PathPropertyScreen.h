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
 * @file PathPropertyScreen.h
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_IMAGE_PATH property.
 */

#ifndef IMAGE_TEST_PATH_PROPERTY_SCREEN_H_
#define IMAGE_TEST_PATH_PROPERTY_SCREEN_H_

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

namespace ImageTest
{

	class PathPropertyScreenListener;

	class PathPropertyScreen:
		public NativeUI::Screen,
		public NativeUI::EditBoxListener,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param listener Listener for screen.
		 */
		PathPropertyScreen(PathPropertyScreenListener& listener);

		/**
		 * Destructor.
		 */
		virtual ~PathPropertyScreen();

		/**
		 * Set displayed path in the edit box.
		 * @param path Text to set.
		 */
		void setDisplayedImagePath(const MAUtil::String& path);

	private:
		/**
		 * Create screen's UI.
		 */
		void createUI();

		/**
		 * Create layout that contains:
		 * - a Label widget.
		 * - a EditBox widget that will be used to get a file path.
		 */
		void createEditBoxLayout();

		/**
		 * Create an button that will be used to select the image
		 * from a file path.
		 */
		void createSelectImageFromPathButton();

		/**
		 * Create an button that will be used to select the image
		 * from a handle.
		 */
		void createSelectImageFromHandleButton();

		/**
		 * Create an button that will be used to display the image path.
		 */
		void createGetImagePathButton();

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
        PathPropertyScreenListener& mListener;

		/**
		 * Main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Used to get the file path.
		 */
		NativeUI::EditBox* mPathEditBox;

		/**
		 * Used to load an image from a file path.
		 */
		NativeUI::Button* mSelectImageFromPathButton;

		/**
		 * Used to load an image from a handle.
		 */
		NativeUI::Button* mSelectImageFromHandleButton;

		/**
		 * Used to test getter for MAW_IMAGE_IMAGE_PATH.
		 */
		NativeUI::Button* mGetImagePathButton;

	}; // end of PathPropertyScreen

} // end ImageTest

#endif /* IMAGE_TEST_PATH_PROPERTY_SCREEN_H_ */
