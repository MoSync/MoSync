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
