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
