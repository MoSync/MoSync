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
	mTimePicker(NULL),
	mTimePickerValue(NULL)
{
	createMainLayout();

	mTimePicker->addTimePickerListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mTimePicker->removeTimePickerListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mTimePicker = new TimePicker();
	mTimePicker->fillSpaceHorizontally();
	mMainLayout->addChild(mTimePicker);

	mTimePickerValue = new Label();
	mTimePickerValue->setText( MAUtil::integerToString(mTimePicker->getHour())
	+ " : " + MAUtil::integerToString(mTimePicker->getMinute()) );
	mMainLayout->addChild(mTimePickerValue);
}

/**
 * This method is called when the selected time was changed by the user.
 * @param button The time picker object that generated the event.
 * @param hour The selected hour.
 * @param minute The selected minute
 */
void MainScreen::timePickerValueChanged(
    TimePicker* timePicker,
    const int hour,
    const int minute)
{
    if (timePicker == mTimePicker)
    {
		mTimePickerValue->setText( MAUtil::integerToString(mTimePicker->getHour())
		+ " : " + MAUtil::integerToString(mTimePicker->getMinute()) );
        printf("mTimePicker value changed = %d:%d", hour, minute);
    }
}
