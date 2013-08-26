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
 * @file TestScreen.h
 * @author Bogdan Iusco.
 *
 * @brief Shows the tested ImageButton widget.
 */

#ifndef IMAGE_BUTTON_TEST_SCREEN_H_
#define IMAGE_BUTTON_TEST_SCREEN_H_

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

namespace ImageButtonTest
{
	/**
	 * Class that creates a screen that displays all the contacts.
	 */
	class TestScreen:
		public Screen,
		public ButtonListener
	{

	public:
		/**
		 * Constructor.
		 */
		TestScreen();

		/**
		 * Destructor.
		 */
		virtual ~TestScreen();

		/**
		 * Set the foreground image from a given file path.
		 * @param path Image file path.
		 */
		void setImageButtonForeground(const MAUtil::String& path);

		/**
		 * Set the foreground image from resources.
		 * @param handle Handle to a image.
		 */
		void setImageButtonForeground(const MAHandle handle);

		/**
		 * Get the foreground image path.
		 * @return The foreground image path.
		 */
		MAUtil::String getImageButtonForegroundPath();

		/**
		 * Set the background image from a given file path.
		 * @param path Image file path.
		 */
		void setImageButtonBackground(const MAUtil::String& path);

		/**
		 * Set the background image from resources.
		 * @param handle Handle to a image.
		 */
		void setImageButtonBackground(const MAHandle handle);

		/**
		 * Get the background image path.
		 * @return The foreground image path.
		 */
		MAUtil::String getImageButtonBackgroundPath();

		/**
		 * Set the pressed image from a given file path.
		 * @param path Image file path.
		 */
		void setImageButtonPressed(const MAUtil::String& path);

		/**
		 * Set the pressed image from resources.
		 * @param handle Handle to a image.
		 */
		void setImageButtonPressed(const MAHandle handle);

		/**
		 * Get the pressed image handle.
		 * @return The pressed image handle.
		 */
		MAHandle getImageButtonPressedHandle();

		/**
		 * Get the pressed image path.
		 * @return The pressed image path.
		 */
		MAUtil::String getImageButtonPressedPath();

	private:
		/**
		 * This method is called when there is an touch-down event for
		 * a button.
		 * Platform: iOS and Android.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonPressed(Widget* button);

		/**
		 * This method is called when there is an touch-up event for
		 * a button.
		 * Platform: iOS and Android.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonReleased(Widget* button);

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * Platform: iOS and Android.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(Widget* button);

		/**
		 * Creates and adds main layout to the screen.
		 */
		void createMainLayout();

	private:
		/**
		 * Main layout.
		 */
		VerticalLayout* mMainLayout;

		ImageButton* mImageButton;
		ListView* mEvents;
	};

} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_TEST_SCREEN_H_ */
