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
 * @author emma
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <mastring.h>
#include <MAUtil/String.h>

#include "MainScreen.h"

using namespace MAUtil;


/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mClearGenderSelection(NULL),
	mFemaleButton(NULL),
	mUnknownButton(NULL),
	mGenderCheckedLabel(NULL),
	mGenderRadioGroup(NULL),
	mGenderSelectionEvent(NULL),
	mGenderSelectionLabel(NULL),
	mGetGenderChecked(NULL),
	mGetGenderSelection(NULL),
	mMaleButton(NULL),
	mToggleMaleButton(NULL),
	mRadioButtonSelectEvent(NULL),
	mradioButtonUnselectEvent(NULL)
{
	createMainLayout();

	mMaleButton->addRadioButtonListener(this);
	mFemaleButton->addRadioButtonListener(this);
	mUnknownButton->addRadioButtonListener(this);
	mGenderRadioGroup->addRadioGroupListener(this);
	mGetGenderChecked->addButtonListener(this);
	mGetGenderSelection->addButtonListener(this);
	mToggleMaleButton->addButtonListener(this);
	mClearGenderSelection->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mMaleButton->removeRadioButtonListener(this);
	mFemaleButton->removeRadioButtonListener(this);
	mUnknownButton->removeRadioButtonListener(this);
	mGenderRadioGroup->removeRadioGroupListener(this);
	mGetGenderChecked->removeButtonListener(this);
	mGetGenderSelection->removeButtonListener(this);
	mToggleMaleButton->removeButtonListener(this);
	mClearGenderSelection->removeButtonListener(this);
}
/**
 * This method is called when the checked state of a radio button is changed.
 * @param radioButton The radio button object that generated the event.
 * @param state The new state of the button.
 */
void MainScreen::radioButtonStateChanged(
    RadioButton* radioButton,
    bool state)
{
	MAUtil::String newState = state ? " now selected." : " now unselected.";

	if ( state )
	{
		if ( radioButton == mFemaleButton )
			mRadioButtonSelectEvent->setText("Button event on Female;" + newState);
		else if ( radioButton == mMaleButton )
			mRadioButtonSelectEvent->setText("Button event on Male;" + newState);
		else if ( radioButton == mUnknownButton )
			mRadioButtonSelectEvent->setText("Button event on Alien;" + newState);
	}
	else
	{
		if ( radioButton == mFemaleButton )
			mradioButtonUnselectEvent->setText("Button event on Female;" + newState);
		else if ( radioButton == mMaleButton )
			mradioButtonUnselectEvent->setText("Button event on Male;" + newState);
		else if ( radioButton == mUnknownButton )
			mradioButtonUnselectEvent->setText("Button event on Alien;" + newState);
	}
}

/**
 * This method is called when a radio button from a radio group is selected.
 * @param radioGroup The radio group object that generated the event.
 * @param index The index of the selected radio button inside this group.
 * @param radioButton The radio button handle that generated the event.
 */
void MainScreen::radioButtonSelected(
    RadioGroup* radioGroup,
    int index,
    RadioButton* radioButton)
{
	if ( mGenderRadioGroup == radioGroup )
	{
		if ( radioButton == mFemaleButton )
			mGenderSelectionEvent->setText("Group event: Female was selected");
		else if ( radioButton == mMaleButton )
			mGenderSelectionEvent->setText("Group event: Male was selected");
		else if ( radioButton == mUnknownButton )
			mGenderSelectionEvent->setText("Group event: Alien was selected");
	}

}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if ( mClearGenderSelection == button )
	{
		mGenderRadioGroup->clearCheck();
	}
	else if( mGetGenderSelection == button )
	{
		RadioButton* selected = mGenderRadioGroup->getChecked();
		if ( selected == NULL )
			mGenderSelectionLabel->setText("No gender selected.");
		if ( selected == mFemaleButton )
			mGenderSelectionLabel->setText("Female is selected");
		else if ( selected == mMaleButton )
			mGenderSelectionLabel->setText("Male is selected");
		else if ( selected == mUnknownButton )
			mGenderSelectionLabel->setText("Alien is selected");
	}
	else if( mGetGenderChecked == button )
	{
		if ( mMaleButton->isChecked() )
			mGenderCheckedLabel->setText("Male is checked");
		else
			mGenderCheckedLabel->setText("Male is not checked");
	}
	else if ( mToggleMaleButton == button )
	{
		mMaleButton->toggle();
	}

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x220099);
	Screen::setMainWidget(mMainLayout);

	mGenderRadioGroup = new RadioGroup();
	mMainLayout->addChild(mGenderRadioGroup);
	mFemaleButton = new RadioButton();
	mFemaleButton->setTextColor(0xFF0000);
	mFemaleButton->setText("Female");
	mMaleButton = new RadioButton();
	mMaleButton->setText("Male");
	mMaleButton->setTextColor(0x0000FF);
	mUnknownButton = new RadioButton();
	mUnknownButton->setText("Alien");
	mUnknownButton->setTextColor(0x00FF00);
	mGenderRadioGroup->addView(mFemaleButton);
	mGenderRadioGroup->addView(mMaleButton);
	mGenderRadioGroup->addView(mUnknownButton);
	mClearGenderSelection = new Button();
	mClearGenderSelection->setText("Clear gender selection");
	mMainLayout->addChild(mClearGenderSelection);
	HorizontalLayout* getGenderLayout = new HorizontalLayout();
	getGenderLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mMainLayout->addChild(getGenderLayout);
	mGetGenderSelection = new Button();
	mGetGenderSelection->setText("Get the gender selection");
	getGenderLayout->addChild(mGetGenderSelection);
	mGenderSelectionLabel = new Label();
	mGenderSelectionLabel->fillSpaceHorizontally();
	getGenderLayout->addChild(mGenderSelectionLabel);

	HorizontalLayout* getCheckedLayout = new HorizontalLayout();
	getCheckedLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mMainLayout->addChild(getCheckedLayout);
	mGetGenderChecked = new Button();
	mGetGenderChecked->setText("Get male checked state");
	getCheckedLayout->addChild(mGetGenderChecked);
	mGenderCheckedLabel = new Label();
	mGenderCheckedLabel->fillSpaceHorizontally();
	getCheckedLayout->addChild(mGenderCheckedLabel);

	mToggleMaleButton = new Button();
	mToggleMaleButton->setText("Toggle(check) Male button");
	mMainLayout->addChild(mToggleMaleButton);

	mGenderSelectionEvent = new Label();
	mGenderSelectionEvent->setText("Radio Group event");
	mMainLayout->addChild(mGenderSelectionEvent);
	mRadioButtonSelectEvent = new Label();
	mRadioButtonSelectEvent->setText("Radio Button select event");
	mMainLayout->addChild(mRadioButtonSelectEvent);
	mradioButtonUnselectEvent = new Label();
	mradioButtonUnselectEvent->setText("Radio Button unselect event");
	mMainLayout->addChild(mradioButtonUnselectEvent);

}
