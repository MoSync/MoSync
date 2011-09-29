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

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	myGender(NULL),
	mButtonForm(NULL),
	mDialog(NULL),
	mGenderFemale(NULL),
	mGenderMale(NULL),
	myAge(NULL),
	mDialogDismiss(NULL)
{
	createMainLayout();
	createDialogMainLayout();

	mButtonForm->addButtonListener(this);
	mDialogDismiss->addButtonListener(this);
	mGenderFemale->addCheckBoxListener(this);
	mGenderMale->addCheckBoxListener(this);
	mAge->addSliderListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mButtonForm->removeButtonListener(this);
	mDialogDismiss->removeButtonListener(this);
	mGenderFemale->removeCheckBoxListener(this);
	mGenderMale->removeCheckBoxListener(this);
	mAge->removeSliderListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mDialogDismiss)
    {
        mDialog->hide();
		myAge->setText("Age: "+  MAUtil::integerToString(mAge->getValue()) );
	    if (mGenderFemale->isChecked())
	    {
			myGender->setText("Gender: female");
	    }
	    else if ( mGenderMale->isChecked() )
	    {
			myGender->setText("Gender: male");
	    }
	    else
	    {
			myGender->setText("Gender: was not selected");
	    }
    }
    else if (button = mButtonForm)
    {
		// Reset the values in the form
		mGenderFemale->setState(false);
		mGenderMale->setState(false);
		mAge->setValue(18);
		mDialog->show();
    }
}

/**
 * This method is called when the value of the slider was modified by
 * the user.
 * @param slider The slider object that generated the event.
 * @param sliderValue The new slider's value.
 */
void MainScreen::sliderValueChanged(
    Slider* slider,
    const int sliderValue)
{
	if ( slider == mAge)
	{
		mAgeLabel->setText(MAUtil::integerToString(sliderValue));
	}
}

/**
 * This method is called when the state of the check box has changed.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
    if (checkBox == mGenderFemale)
    {
		if ( state == true)
		{
			if ( mGenderMale->isChecked() )
				mGenderMale->setState(false);
			myGender->setText("Gender: female");
		}
		else
		{

		}
    }
    else if (checkBox == mGenderMale )
    {
		if (state == true)
		{
			if (mGenderFemale->isChecked() )
				mGenderFemale->setState(false);
			myGender->setText("Gender: male");
		}
    }
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xC1FFC1);
	Screen::setMainWidget(mMainLayout);

	Label* infoLabel = new Label();
	infoLabel->setText("Your current details:");

	myAge = new Label();
	myAge->setText("Age is not set.");
	mMainLayout->addChild(myAge);

	myGender = new Label();
	myGender->setText("Gender is not set.");
	mMainLayout->addChild(myGender);

	mButtonForm = new Button();
	mButtonForm->setText("Fill form");
	mMainLayout->addChild(mButtonForm);

}

void MainScreen::createDialogMainLayout()
{
	VerticalLayout* dialogLayout = new VerticalLayout();
	dialogLayout->setPaddingLeft(10);
	dialogLayout->setPaddingRight(10);


	Label* info = new Label();
	info->setText(" Please fill in all the fields ");
	dialogLayout->addChild(info);

	VerticalLayout* genderLayout = new VerticalLayout();
	// set to wrap content
	genderLayout->setPaddingTop(20);
	genderLayout->setPaddingBottom(20);
	genderLayout->setPaddingLeft(20);

	Label* infoGender = new Label();
	infoGender->setText("Select gender");
	infoGender->setFontColor(0xFF7F24);
	genderLayout->addChild(infoGender);
	HorizontalLayout* femaleL = new HorizontalLayout();
	mGenderFemale = new CheckBox();
	mGenderFemale->setState(false);
	femaleL->addChild(mGenderFemale);
	Label* female = new Label();
	female->setText(" Female");
	femaleL->addChild(female);
	genderLayout->addChild(femaleL);

	HorizontalLayout* maleL = new HorizontalLayout();
	mGenderMale = new CheckBox();
	mGenderMale->setState(false);
	maleL->addChild(mGenderMale);
	Label* male = new Label();
	male->setText(" Male");
	maleL->addChild(male);
	genderLayout->addChild(maleL);

	dialogLayout->addChild(genderLayout);

	VerticalLayout* birthLayout = new VerticalLayout();
	birthLayout->setPaddingTop(20);
	birthLayout->setPaddingBottom(20);
	birthLayout->setPaddingLeft(20);

	Label* infoBirth = new Label();
	infoBirth->setText("Select age");
	infoBirth->setFontColor(0xFF7F24);
	birthLayout->addChild(infoBirth);

	mAge = new Slider();
	mAge->fillSpaceHorizontally();
	mAge->setValue(18);
	mAge->setMaximumValue(110);
	birthLayout->addChild(mAge);
	mAgeLabel = new Label();
	mAgeLabel->setText("18");
	birthLayout->addChild(mAgeLabel);
	dialogLayout->addChild(birthLayout);

	mDialogDismiss = new Button();
	mDialogDismiss->setText("Ready");
	dialogLayout->addChild(mDialogDismiss);

	mDialog = new Dialog();
	mDialog->setTitle("My Form");
	mDialog->canBeDismissed(false);
//	mDialog->setBackgroundColor(0x556B2F); //this will be set to dialogLayout
	mDialog->setProperty(MAW_WIDGET_ALPHA, "0.0");
	mDialog->setMainWidget(dialogLayout);
}
