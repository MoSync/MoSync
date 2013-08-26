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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mCheckBox(NULL),
	mCheckBoxButton(NULL),
	mCheckBoxInfoLabel(NULL),
	mToggleButton(NULL),
	mToggleInfoLabel(NULL),
	mCheckBoxState(false)
{
	createMainLayout();

	mCheckBoxButton->addButtonListener(this);
	mCheckBox->addCheckBoxListener(this);
	mToggleButton->addToggleButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mCheckBoxButton->removeButtonListener(this);
    mCheckBox->removeCheckBoxListener(this);
    mToggleButton->removeToggleButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mCheckBoxButton)
    {
        mCheckBoxState = !mCheckBoxState;
        mCheckBox->setState(mCheckBoxState);

		mToggleButton->setCheckedState(!(mToggleButton->isChecked()));
    }
}

/**
 * This method is called when the state of the check box has changed.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(CheckBox* checkBox, bool state)
{
    if (checkBox == mCheckBox)
    {
		mCheckBoxState = state;
        if (state)
        {
            mCheckBoxInfoLabel->setText("CheckBox event: checked");
            printf("check box state = true");
        }
        else
        {
            mCheckBoxInfoLabel->setText("CheckBox event: unchecked");
            printf("check box state = false");
        }
    }
}

/*
 * toggleButtonStateChanged(ToggleButton, bool)
 * from ToggleButtonListener
 */
void MainScreen::toggleButtonStateChanged(ToggleButton* toggleButton, bool state)
{
	if(toggleButton == mToggleButton) {
		MAUtil::String msg = state ? "ToggleButton event: checked" : "ToggleButton event: unchecked";
		mToggleInfoLabel->setText(msg);
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Label* label = new Label();
	label->setText("CheckBox and ToggleButton test program");
	mMainLayout->addChild(label);

	// the CheckBox
	mCheckBox = new CheckBox();
	mMainLayout->addChild(mCheckBox);

	// info label about the CheckBox
	mCheckBoxInfoLabel = new Label();
	MAUtil::String msg = mCheckBox->isChecked() ? "CheckBox is checked" : "CheckBox is unchecked";
	mCheckBoxInfoLabel->setText(msg);
	mMainLayout->addChild(mCheckBoxInfoLabel);

	mCheckBoxButton = new Button();
	mCheckBoxButton->setText("Change widgets' states");
	mMainLayout->addChild(mCheckBoxButton);

	// the ToggleButton
	mToggleButton = new ToggleButton();
	mMainLayout->addChild(mToggleButton);

	// info label about the ToggleButton
	mToggleInfoLabel = new Label();
	msg = mToggleButton->isChecked() ? "ToggleButton is checked" : "ToggleButton is unchecked";
	mToggleInfoLabel->setText(msg);
	mMainLayout->addChild(mToggleInfoLabel);
}
