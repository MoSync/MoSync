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
	mRatingBar(NULL),
	mEventLabel(NULL),
	mGranularity(NULL),
	mRating(NULL),
	mRatingLabel(NULL),
	mStars(NULL),
	mStarsLabel(NULL),
	mGetGranularity(NULL),
	mGetRating(NULL),
	mGetStars(NULL)
{
	createMainLayout();

	mRatingBar->addRatingBarListener(this);
	mGranularity->addEditBoxListener(this);
	mStars->addEditBoxListener(this);
	mRating->addEditBoxListener(this);
	mGetGranularity->addButtonListener(this);
	mGetRating->addButtonListener(this);
	mGetStars->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mRatingBar->removeRatingBarListener(this);
	mGranularity->removeEditBoxListener(this);
	mStars->removeEditBoxListener(this);
	mRating->removeEditBoxListener(this);
	mGetGranularity->removeButtonListener(this);
	mGetRating->removeButtonListener(this);
	mGetStars->removeButtonListener(this);
}

/**
 * This method is called when the value of the rating bar was changed.
 * @param ratingBar The rating bar object that generated the event.
 * @param value The new value of the rating bar.
 * @param romUser True if the value was changed by the user, false if it was
 * set programmaticaly.
 */
void MainScreen::ratingChanged(
    RatingBar* ratingBar,
    float value,
    bool fromUser)
{
    char buffer[20];
    sprintf(buffer, "%f", value);
    MAUtil::String isFromUser = fromUser == true ? ". Changed by user" : ". Not changed by user";
    MAUtil::String toPrint = ("New value: ");
    toPrint += buffer;
    toPrint += isFromUser;
    mEventLabel->setText(toPrint);
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxReturn(EditBox* editBox)
{
	editBox->hideKeyboard();
}

/**
 * This method is called when the edit box text was changed.
 * @param editBox The edit box object that generated the event.
 * @param text The new text.
 */
void MainScreen::editBoxTextChanged(
    EditBox* editBox,
    const MAUtil::String& text)
{
	if ( mGranularity == editBox )
	{
		mRatingBar->setGranularity( editBox->getPropertyFloat(MAW_EDIT_BOX_TEXT) );

	}
	else if ( mRating == editBox )
	{
		mRatingBar->setRating( editBox->getPropertyFloat(MAW_EDIT_BOX_TEXT) );

	}
	else if ( mStars == editBox )
	{
		mRatingBar->setNumberOfStars( editBox->getPropertyInt(MAW_EDIT_BOX_TEXT) );

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
	if ( mGetGranularity == (Button*) button )
	{
		float value = mRatingBar->getGranularity();
	    char buffer[20];
	    sprintf(buffer, "%f", value);
		mGranularityLabel->setText(buffer);
	}
	else if ( mGetRating == (Button*) button )
	{
		float value = mRatingBar->getRating();
	    char buffer[20];
	    sprintf(buffer, "%f", value);
		mRatingLabel->setText(buffer);
	}
	else if ( mGetStars == (Button*) button )
	{
		mStarsLabel->setText( MAUtil::integerToString(mRatingBar->getNumberOfStars()) );
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

	mRatingBar = new RatingBar();
	mMainLayout->addChild(mRatingBar);

	addRow(" Stars ", mStars, mGetStars, "Get stars", mStarsLabel);
	addRow(" Rating ", mRating, mGetRating, "Get rating", mRatingLabel);
	addRow(" Granularity ", mGranularity, mGetGranularity, "Get granularity", mGranularityLabel);

	Label* eventInfo = new Label();
	eventInfo->setText("The latest rating event:");
	mMainLayout->addChild(eventInfo);
	mEventLabel = new Label();
	mEventLabel->setText("Will print the latest event info");
	mMainLayout->addChild(mEventLabel);
}

/**
 * Add rows:
 * On the first line an info label and the edit box.
 * On the second line a button for getting the set info, and a label
 * for displaying the value.
 */
void MainScreen::addRow(MAUtil::String labelText, EditBox* &editBox,
		Button* &button, MAUtil::String buttonText, Label* &getValueLabel)
{
	HorizontalLayout* setValuesLayout = new HorizontalLayout();
	setValuesLayout->wrapContentVertically();
	Label* valueLabel = new Label();
	valueLabel->setText(labelText);
	setValuesLayout->addChild(valueLabel);
	editBox = new EditBox();
	editBox->fillSpaceHorizontally();
	editBox->setInputMode(EDIT_BOX_INPUT_MODE_DECIMAL);
	setValuesLayout->addChild(editBox);
	mMainLayout->addChild(setValuesLayout);

	HorizontalLayout* getValuesLayout = new HorizontalLayout();
	getValuesLayout->wrapContentVertically();
	button = new Button();
	button->setText(buttonText);
	getValuesLayout->addChild(button);
	getValueLabel = new Label();
	getValueLabel->fillSpaceHorizontally();
	getValuesLayout->addChild(getValueLabel);
	mMainLayout->addChild(getValuesLayout);
}
