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
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mSlider(NULL),
	mEditBox(NULL),
	mSetValueButton(NULL),
	mGetValueButton(NULL),
	mGetValueLabel(NULL),
	mIncreaseValueButton(NULL),
	mDecreseValueButton(NULL)
{
	createMainLayout();

	mSetValueButton->addButtonListener(this);
    mGetValueButton->addButtonListener(this);
    mIncreaseValueButton->addButtonListener(this);
    mDecreseValueButton->addButtonListener(this);
    mSlider->addSliderListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSetValueButton->removeButtonListener(NULL);
    mGetValueButton->removeButtonListener(NULL);
    mIncreaseValueButton->removeButtonListener(NULL);
    mDecreseValueButton->removeButtonListener(NULL);
    mSlider->removeSliderListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mSlider = new Slider();
	mSlider->setMaximumValue(100);
	mSlider->fillSpaceHorizontally();
	mMainLayout->addChild(mSlider);

	mEditBox = new EditBox();
	mEditBox->setText("40");
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	mSetValueButton = new Button();
	mSetValueButton->setText("set value");
	mMainLayout->addChild(mSetValueButton);

    mGetValueButton = new Button();
    mGetValueButton->setText("get value");
    mMainLayout->addChild(mGetValueButton);

    mGetValueLabel = new Label();
    mMainLayout->addChild(mGetValueLabel);

	mIncreaseValueButton = new Button();
	mIncreaseValueButton->setText("increase value with 10");
	mMainLayout->addChild(mIncreaseValueButton);

    mDecreseValueButton = new Button();
    mDecreseValueButton->setText("decrease value with 10");
    mMainLayout->addChild(mDecreseValueButton);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mSetValueButton)
    {
        MAUtil::String stringValue;
        stringValue = mEditBox->getText();
        int value = atoi(stringValue.c_str());
        printf("set slider's value = %d", value);
        mSlider->setValue(value);
    }
    else if (button == mGetValueButton)
    {
		mGetValueLabel->setText(MAUtil::integerToString(mSlider->getValue()));
        int value = mSlider->getValue();
        printf("slier's value = %d", value);
    }
    else if (button == mIncreaseValueButton)
    {
        mSlider->increaseValue(10);
    }
    else if (button == mDecreseValueButton)
    {
        mSlider->decreaseValue(10);
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
    if (slider == mSlider)
    {
        printf("mSlider value changed - %d", sliderValue);
    }
}
