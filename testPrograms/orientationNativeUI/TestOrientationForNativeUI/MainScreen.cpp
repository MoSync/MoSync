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

#define PORTRAIT_LABEL_TEXT "Portrait"
#define PORTRAIT_UPSIDE_DOWN_LABEL_TEXT "Portrait upside down"
#define LANDSCAPE_LEFT_LABEL_TEXT "Landscape left"
#define LANDSCAPE_RIGHT_LABEL_TEXT "Landscape right"
#define ORIENTATION_LABEL "Orientation:"
#define ORIENTATION_PORTRAIT "Portrait"
#define ORIENTATION_PORTRAIT_UPSIDE_DOWN "Portrait upside down"
#define ORIENTATION_LANDSCAPE_LEFT "Landscape left"
#define ORIENTATION_LANDSCAPE_RIGHT "Landscape right"

#include <conprint.h>
#include <maapi.h>

#include "MainScreen.h"
#include "Util.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mPortraitCheckBox(NULL),
	mPortraitUpsideDownCheckBox(NULL),
	mLandscapeLeftCheckBox(NULL),
	mLandscapeRightCheckBox(NULL),
	mOrientationLabel(NULL)
{
	createMainLayout();
	mPortraitCheckBox->setState(true);

	mPortraitCheckBox->addCheckBoxListener(this);
	mPortraitUpsideDownCheckBox->addCheckBoxListener(this);
	mLandscapeLeftCheckBox->addCheckBoxListener(this);
	mLandscapeRightCheckBox->addCheckBoxListener(this);

	mSupportedOrientations = MA_SCREEN_ORIENTATION_PORTRAIT;
	mOrientationLabel->setText(ORIENTATION_PORTRAIT);

	int result = maScreenSetSupportedOrientations(mSupportedOrientations);
	printf("MainScreen::MainScreen result maScreenSetSupportedOrientations = %d",
		result);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mPortraitCheckBox->removeCheckBoxListener(this);
	mPortraitUpsideDownCheckBox->removeCheckBoxListener(this);
	mLandscapeLeftCheckBox->removeCheckBoxListener(this);
	mLandscapeRightCheckBox->removeCheckBoxListener(this);
}

/**
 * Create a new horizontal layout.
 * Will contain a label and a check box.
 * @param label The given label.
 * @param checkBox The given check box.
 * @return The new created horizontal layout.
 */
HorizontalLayout* MainScreen::createRow(Label* label, CheckBox* checkBox)
{
	HorizontalLayout* hLayout = new HorizontalLayout();
	hLayout->wrapContentVertically();
	hLayout->addChild(label);
	if (checkBox)
	{
		HorizontalLayout* space = new HorizontalLayout();
		space->setWidth(10);
		hLayout->addChild(space);
		hLayout->addChild(checkBox);
	}
	return hLayout;
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	Label* label;

	// Add widgets for enabling/disabling portrait mode.
	label = new Label();
	label->setText(PORTRAIT_LABEL_TEXT);
	mPortraitCheckBox = new CheckBox();
	mainLayout->addChild(this->createRow(label, mPortraitCheckBox));

	// Add widgets for enabling/disabling portrait upside down mode.
	label = new Label();
	label->setText(PORTRAIT_UPSIDE_DOWN_LABEL_TEXT);
	mPortraitUpsideDownCheckBox = new CheckBox();
	mainLayout->addChild(this->createRow(label, mPortraitUpsideDownCheckBox));

	// Add widgets for enabling/disabling landscape left mode.
	label = new Label();
	label->setText(LANDSCAPE_LEFT_LABEL_TEXT);
	mLandscapeLeftCheckBox = new CheckBox();
	mainLayout->addChild(this->createRow(label, mLandscapeLeftCheckBox));

	// Add widgets for enabling/disabling landscape right mode.
	label = new Label();
	label->setText(LANDSCAPE_RIGHT_LABEL_TEXT);
	mLandscapeRightCheckBox = new CheckBox();
	mainLayout->addChild(this->createRow(label, mLandscapeRightCheckBox));

	// Add widgets for displaying the current orientation.
	HorizontalLayout* hLayout = new HorizontalLayout();
	label = new Label();
	label->setText(ORIENTATION_LABEL);
	hLayout->addChild(label);
	mOrientationLabel = new Label();
	hLayout->addChild(mOrientationLabel);
	mainLayout->addChild(hLayout);
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	bool isChecked;
	if (checkBox == mPortraitCheckBox)
	{
		isChecked = mPortraitCheckBox->isChecked();
		printf("MainScreen::checkBoxStateChanged set portrait mode to %d",
			isChecked);
		this->changeOrientationBitmask(isChecked,
			MA_SCREEN_ORIENTATION_PORTRAIT,
			mPortraitCheckBox);
	}
	else if (checkBox == mPortraitUpsideDownCheckBox)
	{
		isChecked = mPortraitUpsideDownCheckBox->isChecked();
		printf("MainScreen::checkBoxStateChanged set portrait upside down mode to %d",
			isChecked);
		this->changeOrientationBitmask(isChecked,
			MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN,
			mPortraitUpsideDownCheckBox);
	}
	else if (checkBox == mLandscapeLeftCheckBox)
	{
		isChecked = mLandscapeLeftCheckBox->isChecked();
		printf("MainScreen::checkBoxStateChanged set landscape left mode to %d",
			isChecked);
		this->changeOrientationBitmask(isChecked,
			MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT,
			mLandscapeLeftCheckBox);
	}
	else if (checkBox == mLandscapeRightCheckBox)
	{
		isChecked = mLandscapeRightCheckBox->isChecked();
		printf("MainScreen::checkBoxStateChanged set landscape right mode to %d",
			isChecked);
		this->changeOrientationBitmask(isChecked,
				MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT,
			mLandscapeRightCheckBox);
	}
	int result = maScreenSetSupportedOrientations(mSupportedOrientations);
	printf("MainScreen::checkBoxStateChanged result maScreenSetSupportedOrientations = %d",
		result);
}

/**
 * Changes the screen orientation bit mask.
 * @param cond If true, the orientation flag will be added to the screen
 * orientation bit mask, otherwise the flag is removed.
 * @param orientation Orientation value that is needed to be add/removed from
 * bit mask. Must be one of the MA_SCREEN_ORIENTATION constants.
 * @param checkBox If the bit mask value will be zero after removing the
 * orientation value than the checkBox will be set to checked.
 */
void MainScreen::changeOrientationBitmask(const bool cond,
    const int orientation,
    CheckBox* checkBox)
{
	if (cond)
	{
		mSupportedOrientations = mSupportedOrientations | orientation;
	}
	else
	{
		mSupportedOrientations = mSupportedOrientations & ~orientation;
	}

	if (0 == mSupportedOrientations)
	{
		mSupportedOrientations = orientation;
		checkBox->setState(true);
	}
}

/**
 * Called just before the screen begins rotating.
 */
void MainScreen::orientationWillChange()
{
	printf("MainScreen::orientationWillChange");
	int screenHeight = this->getHeight();
	int screenWidth = this->getWidth();
	printf("screenHeight = %d screenWidth = %d", screenHeight, screenWidth);

	int orientation = maScreenGetCurrentOrientation();
	MAUtil::String orientationText;
	switch (orientation)
	{
		case MA_SCREEN_ORIENTATION_PORTRAIT:
			orientationText = ORIENTATION_PORTRAIT;
			break;
		case MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
			orientationText = ORIENTATION_PORTRAIT_UPSIDE_DOWN;
			break;
		case MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT:
			orientationText = ORIENTATION_LANDSCAPE_LEFT;
			break;
		case MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT:
			orientationText = ORIENTATION_LANDSCAPE_RIGHT;
			break;
	}
	mOrientationLabel->setText(orientationText);
}
