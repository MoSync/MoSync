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
	mSetDate(NULL),
	mGetDate(NULL),
	mSetMaxDate(NULL),
	mGetMaxDate(NULL),
	mSetMinDate(NULL),
	mGetMinDate(NULL),
	mDatePicker(NULL)
{
	createMainLayout();

	mDatePicker->addDatePickerListener(this);

	mSetDate->addButtonListener(this);
	mGetDate->addButtonListener(this);
	mSetMaxDate->addButtonListener(this);
	mGetMaxDate->addButtonListener(this);
	mSetMinDate->addButtonListener(this);
	mGetMinDate->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSetDate->removeButtonListener(this);
    mGetDate->removeButtonListener(this);
    mSetMaxDate->removeButtonListener(this);
    mGetMaxDate->removeButtonListener(this);
    mSetMinDate->removeButtonListener(this);
    mGetMinDate->removeButtonListener(this);
    mDatePicker->removeDatePickerListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mDatePicker = new DatePicker();
	mDatePicker->fillSpaceHorizontally();
	mMainLayout->addChild(mDatePicker);

	HorizontalLayout* layout1 = new HorizontalLayout();
	mMainLayout->addChild(layout1);

	mSetDate = new Button();
	mSetDate->setText("Set Date");
	layout1->addChild(mSetDate);

    mGetDate = new Button();
    mGetDate->setText("Get Date");
    layout1->addChild(mGetDate);

    HorizontalLayout* layout2 = new HorizontalLayout();
    mMainLayout->addChild(layout2);

    mSetMaxDate = new Button();
    mSetMaxDate->setText("Set Max Date");
    layout2->addChild(mSetMaxDate);

    mGetMaxDate = new Button();
    mGetMaxDate->setText("Get Max Date");
    layout2->addChild(mGetMaxDate);

    HorizontalLayout* layout3 = new HorizontalLayout();
    mMainLayout->addChild(layout3);

    mSetMinDate = new Button();
    mSetMinDate->setText("Set Min Date");
    layout3->addChild(mSetMinDate);

    mGetMinDate = new Button();
    mGetMinDate->setText("Get Min Date");
    layout3->addChild(mGetMinDate);
}

/**
 * This method is called the selected date was changed by the user.
 * @param datePicker The date picker object that generated the event.
 * @param selectedDate The new selected date.
 */
void MainScreen::datePickerValueChanged(
    DatePicker* datePicker,
    const struct Date& selectedDate)
{
    if (datePicker == mDatePicker)
    {
        printf("mDatePicker value changed : %d-%d-%d",
            selectedDate.day,
            selectedDate.month,
            selectedDate.year);
    }
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mSetDate)
    {
        Date date;
        date.day = 17;
        date.month = 8;
        date.year = 2011;
        mDatePicker->setDate(date);
    }
    else if (button == mGetDate)
    {
        Date date = mDatePicker->getDate();
        printf("get date : %d-%d-%d", date.day, date.month, date.year);
    }
    else if (button == mSetMaxDate)
    {
        Date date;
        date.day = 1;
        date.month = 2;
        date.year = 2015;
        mDatePicker->setMaxDate(date);
    }
    else if (button == mGetMaxDate)
    {
        Date date = mDatePicker->getMaxDate();
        printf("get max date : %d-%d-%d", date.day, date.month, date.year);
    }
    else if (button == mSetMinDate)
    {
        Date date;
        date.day = 1;
        date.month = 1;
        date.year = 2010;
        mDatePicker->setMinDate(date);
    }
    else if (button == mGetMinDate)
    {
        Date date = mDatePicker->getMinDate();
        printf("get min date : %d-%d-%d", date.day, date.month, date.year);
    }
}
