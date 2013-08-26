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
