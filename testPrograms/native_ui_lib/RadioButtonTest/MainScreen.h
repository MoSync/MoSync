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
 * @author emma
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public ButtonListener,
	public RadioButtonListener,
	public RadioGroupListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

    /**
     * This method is called when the checked state of a radio button is changed.
     * @param radioButton The radio button object that generated the event.
     * @param state The new state of the button.
     */
    virtual void radioButtonStateChanged(
        RadioButton* radioButton,
        bool state);

    /**
     * This method is called when a radio button from this group is selected.
     * @param radioGroup The radio group object that generated the event.
     * @param index The index of the selected radio button inside this group.
     * @param radioButton The radio button handle that generated the event.
     */
    virtual void radioButtonSelected(
        RadioGroup* radioGroup,
        int index,
        RadioButton* radioButton);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * A radio group for gender selection.
	 */
	RadioGroup* mGenderRadioGroup;
	RadioButton* mMaleButton;
	RadioButton* mFemaleButton;
	RadioButton* mUnknownButton;
	Button* mClearGenderSelection;
	Button* mGetGenderSelection;
	Button* mGetGenderChecked;
	Label* mGenderCheckedLabel;
	Label* mGenderSelectionLabel;
	Button* mToggleMaleButton;

	Label* mGenderSelectionEvent;
	Label* mRadioButtonSelectEvent;
	Label* mradioButtonUnselectEvent;
};


#endif /* MAINSCREEN_H_ */
