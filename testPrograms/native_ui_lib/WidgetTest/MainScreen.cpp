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
 * @date 17 Jan 2012
 *
 * Application's screen.
 * Displays widgets for setting & getting widget values.
 */

#include <conprint.h>
#include <ma.h>

#include "MainScreen.h"

#define DEMO_BTN_TEXT "Demo wigdet"
#define SET_APLHA_BTN_TEXT "Set alpha"
#define GET_APLHA_BTN_TEXT "Get alpha"

#define SLIDER_MAX_VALUE 100.00

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mDemoWidget(NULL),
	mAlphaValueSlider(NULL),
	mAlphaLabel(NULL),
	mGetAlphaButton(NULL)
{
	createMainLayout();

	mDemoWidget->addButtonListener(this);
	mGetAlphaButton->addButtonListener(this);
	mAlphaValueSlider->addSliderListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mDemoWidget->removeButtonListener(this);
	mGetAlphaButton->removeButtonListener(this);
	mAlphaValueSlider->removeSliderListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mDemoWidget = new Button();
	mDemoWidget->setText(DEMO_BTN_TEXT);
	mMainLayout->addChild(mDemoWidget);

	mAlphaValueSlider = new Slider();
	mAlphaValueSlider->fillSpaceHorizontally();
	mAlphaValueSlider->setMaximumValue((int)SLIDER_MAX_VALUE);
	mAlphaValueSlider->setValue((int)SLIDER_MAX_VALUE);
	mMainLayout->addChild(mAlphaValueSlider);

	mAlphaLabel = new Label();
	mAlphaLabel->fillSpaceHorizontally();
	mMainLayout->addChild(mAlphaLabel);

	mGetAlphaButton = new Button();
	mGetAlphaButton->setText(GET_APLHA_BTN_TEXT);
	mMainLayout->addChild(mGetAlphaButton);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if (button == mDemoWidget)
	{
		printf("mDemoWidget buttonClickedEvent");
	}
	else if (button == mGetAlphaButton)
	{
		float alphaValue = mDemoWidget->getAlpha();
		char buf[MAX_BUF_SIZE];
		sprintf(buf, "Alpha: %f",alphaValue);
		mAlphaLabel->setText(buf);
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
	float alphaValue = (float) sliderValue / SLIDER_MAX_VALUE;
	mDemoWidget->setAlpha(alphaValue);
}
