/*
Copyright (C) 2012 MoSync AB

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
		public CheckBoxListener,
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
		 * Called just before the screen begins rotating.
		 */
		virtual void orientationWillChange();

		/**
		 * Called after the screen orientation has changed.
		 * Available only on iOS and Windows Phone 7.1 platforms.
		 */
		virtual void orientationDidChange();

	private:
		/**
		 * Creates and adds main layout to the screen.
		 */
		void createMainLayout();

		/**
		 * Gets the current orientation as a string
		 */
		MAUtil::String getOrientationString();

		/**
		 * Create a new horizontal layout.
		 * Will contain a label and a check box.
		 * @param label The given label.
		 * @param checkBox The given check box.
		 * @return The new created horizontal layout.
		 */
		HorizontalLayout* createRow(Label* label, CheckBox* checkBox);

		// From ButtonListener
        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(Widget* button);

		/**
		 * This method is called when the state of the check box was changed
		 * by the user.
		 * @param checkBox The check box object that generated the event.
		 * @param state True if the check box is checked, false otherwise.
		 */
		virtual void checkBoxStateChanged(
			CheckBox* checkBox,
			bool state);

		// From ScreenListener
		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 * @param screenOrientation The new screen orientation.
		 */
		virtual void orientationChanged(Screen* screen, int screenOrientation);

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

	private:
		/**
		 * Used for applying the orientation flag.
		 */
		Button* mSetOrientationBtn;

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
