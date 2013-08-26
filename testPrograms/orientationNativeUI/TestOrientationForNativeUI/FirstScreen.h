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
 * @file FirstScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for setting supported screen orientations.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

namespace OrientationTest
{
	using namespace NativeUI;

	/**
	 * @brief Screen used for setting supported screen orientations.
	 */
	class FirstScreen:
		public Screen,
		public ButtonListener,
		public ScreenListener
	{

	public:
		/**
		 * Constructor.
		 */
		FirstScreen();

		/**
		 * Destructor.
		 */
		virtual ~FirstScreen();

		/**
		 * @deprecated: Use addScreenListener(ScreenListener*) instead.
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 */
		virtual void orientationDidChange();
		// From ScreenListener
		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 * @param screenOrientation The new screen orientation.
		 */
		virtual void orientationChanged(Screen* screen, int screenOrientation);
	private:
		/**
		 * Creates and adds main layout to the screen.
		 */
		void createMainLayout();

		/**
		 * Create a new horizontal layout.
		 * Will contain a label and a check box.
		 * @param label The given label.
		 * @param checkBox The given check box.
		 * @return The new created horizontal layout.
		 */
		HorizontalLayout* createRow(Label* label, CheckBox* checkBox);
		ListViewItem* createItem(Label* label, CheckBox* checkBox);

		// From ButtonListener
        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(Widget* button);

		/**
		 * Changes the screen orientation bit mask.
		 * @param cond If true, the orientation flag will be added to the screen
		 * orientation bit mask, otherwise the flag is removed.
		 * @param orientation Orientation value that is needed to be add/removed from
		 * bit mask. Must be one of the MA_SCREEN_ORIENTATION constants.
		 * @param checkBox If the bit mask value will be zero after removing the
		 * orientation value than the checkBox will be set to checked.
		 */
		void changeOrientationBitmask(const bool cond,
			const int orientation,
			CheckBox* checkBox);

		void addOrientationFlag(int orientation);
	private:
		/**
		 * Used for applying the orientation flag.
		 */
		Button* mSetOrientationBtn;

		/**
		 * Used for aligning all orientation check boxes.
		 */
		ListView* mListView;

		/**
		 * Used for enabling/disabling portrait mode.
		 */
		CheckBox* mPortraitCheckBox;

		/**
		 * Used for enabling/disabling portrait mode.
		 */
		CheckBox* mPortraitUpsideDownCheckBox;
		CheckBox* mPortraitUpCheckBox;

		/**
		 * Used for enabling/disabling sensor landscape.
		 */
		CheckBox* mLandscapeCheckBox;
		/**
		 * Used for enabling/disabling landscape left mode.
		 */
		CheckBox* mLandscapeLeftCheckBox;

		/**
		 * Used for enabling/disabling landscape right mode.
		 */
		CheckBox* mLandscapeRightCheckBox;

		/**
		 * Used for enabling/disabling dynamic orientation.
		 */
		CheckBox* mDynamicCheckBox;

		/**
		 * Display the current orientation.
		 */
		Label* mOrientationLabel;

		/**
		 * Describes how to use the set orientation feature.
		 */
		Label *mSetOrientationDescriptionLabel;

		/**
		 * A bit mask consisting of flags describing the supported screen orientations.
		 *  The bit mask is created using MA_SCREEN_ORIENTATION values.
		 */
		int mSupportedOrientations;

		/**
		 * Represents the selected orientation from the mOrientationOptionsListView
		 * Values:
		 * 1 - SCREEN_ORIENTATION_LANDSCAPE
		 * 2 - SCREEN_ORIENTATION_PORTRAIT
		 * 3 - SCREEN_ORIENTATION_DYNAMIC
		 */
		int mSelectedOrientation;

	};

} // namespace OrientationTest

#endif /* MAINSCREEN_H_ */
