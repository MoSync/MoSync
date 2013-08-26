/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
