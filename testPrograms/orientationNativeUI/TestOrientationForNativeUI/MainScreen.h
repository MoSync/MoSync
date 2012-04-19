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
 * @file MainScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for setting supported screen orientations.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * @brief Screen used for setting supported screen orientations.
 */
class MainScreen:
	public Screen,
	public CheckBoxListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	virtual ~MainScreen();

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

	/**
	 * This method is called when the state of the check box was changed
	 * by the user.
	 * @param checkBox The check box object that generated the event.
	 * @param state True if the check box is checked, false otherwise.
	 */
	virtual void checkBoxStateChanged(
		CheckBox* checkBox,
		bool state);

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

	/**
	 * Called just before the screen begins rotating.
	 */
	virtual void orientationWillChange();

private:
	/**
	 * Used for enabling/disabling portrait mode.
	 */
	CheckBox* mPortraitCheckBox;

	/**
	 * Used for enabling/disabling portrait mode.
	 */
	CheckBox* mPortraitUpsideDownCheckBox;

	/**
	 * Used for enabling/disabling landscape left mode.
	 */
	CheckBox* mLandscapeLeftCheckBox;

	/**
	 * Used for enabling/disabling landscape right mode.
	 */
	CheckBox* mLandscapeRightCheckBox;

	/**
	 * Display the current orientation.
	 */
	Label* mOrientationLabel;

	/**
	 * A bit mask consisting of flags describing the supported screen orientations.
	 *  The bit mask is created using MA_SCREEN_ORIENTATION values.
	 */
	int mSupportedOrientations;
};

#endif /* MAINSCREEN_H_ */
