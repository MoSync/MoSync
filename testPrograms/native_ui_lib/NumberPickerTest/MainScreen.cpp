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
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mNumberPicker(NULL)
{
	createMainLayout();

	mNumberPicker->addNumberPickerListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mNumberPicker->removeNumberPickerListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mNumberPicker = new NumberPicker();
	mNumberPicker->setMaxValue(20);
	mNumberPicker->setMinValue(0);
	mNumberPicker->fillSpaceHorizontally();
	mMainLayout->addChild(mNumberPicker);
}

/**
 * This method is called when the selected number was changed by the
 * user.
 * @param numberPicker The number picker object that generated the event.
 * @param numberPickerValue The new selected value.
 */
void MainScreen::numberPickerValueChanged(
    NumberPicker* numberPicker,
    const int numberPickerValue)
{
    if (numberPicker == mNumberPicker)
    {
        char buf[50];
        sprintf(buf, "The new picker value is %d", numberPicker->getValue());
        maAlert("Alert", buf, "Ok", NULL, NULL);
    }
}
