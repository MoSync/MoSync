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
        printf("mNumberPicker value changed : %d", numberPickerValue);
    }
}
