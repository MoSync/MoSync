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
	mList(NULL),
	mSetDate(NULL),
	mDisplayedDate(NULL),
	mSetMaxDate(NULL),
	mGetMaxDate(NULL),
	mGetMaxDateValue(NULL),
	mGetMinDateValue(NULL),
	mSetMinDate(NULL),
	mGetMinDate(NULL),
	mDatePicker(NULL)
{
	createMainLayout();

	mDatePicker->addDatePickerListener(this);

	mSetDate->addButtonListener(this);
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
    mDatePicker->removeDatePickerListener(this);
    mSetDate->removeButtonListener(this);
    mSetMaxDate->removeButtonListener(this);
    mGetMaxDate->removeButtonListener(this);
    mSetMinDate->removeButtonListener(this);
    mGetMinDate->removeButtonListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	setTitle("Date Picker Test");
	// Create and add the main layout to the screen.
	mList = new ListView();
	Screen::setMainWidget(mList);

	mDatePicker = new DatePicker();
	mDatePicker->fillSpaceHorizontally();
	mList->addChild(mDatePicker);

    mDisplayedDate = new Label();
    mDisplayedDate->fillSpaceHorizontally();
    mDisplayedDate->setText("Aug 17 2011");
    mList->addChild(mDisplayedDate);

	mSetDate = new Button();
	mSetDate->setText("Reset Date Aug-17-2011");
	mSetDate->fillSpaceHorizontally();
	mSetDate->wrapContentVertically();
	mList->addChild(mSetDate);

	CreateMaxDateLayout();
	CreateMinDateLayout();

	mList->addChild(mMaxDateLayout);
	mList->addChild(mMinDateLayout);
}

void MainScreen::CreateMaxDateLayout()
{
	mMaxDateLayout = new VerticalLayout();

	mYearLabelMaxDate = new Label();
	mYearLabelMaxDate->setText("MaxDate year:");
	mYearLabelMaxDate->fillSpaceHorizontally();

	mMonthLabelMaxDate = new Label();
	mMonthLabelMaxDate->setText("MaxDate Month:");
	mMonthLabelMaxDate->fillSpaceHorizontally();

	mDayLabelMaxDate = new Label();
	mDayLabelMaxDate->setText("MaxDate Day:");
	mDayLabelMaxDate->fillSpaceHorizontally();

	mYearValueMaxDate = new EditBox();
	mYearValueMaxDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mYearValueMaxDate->fillSpaceHorizontally();

	mMonthValueMaxDate = new EditBox();
	mMonthValueMaxDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mMonthValueMaxDate->fillSpaceHorizontally();

	mDayValueMaxDate = new EditBox();
	mDayValueMaxDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mDayValueMaxDate->fillSpaceHorizontally();

	mSetMaxDate = new Button();
	mSetMaxDate->setText("Set MaxDate");

	mGetMaxDate = new Button();
	mGetMaxDate->setText("Get MaxDate");

	mGetMaxDateValue = new Label();
	mGetMaxDateValue->setText("MaxDate:");
	mGetMaxDateValue->fillSpaceHorizontally();

	mMaxDateLayout->fillSpaceHorizontally();

	mMaxDateLayout->addChild(mYearLabelMaxDate);
	mMaxDateLayout->addChild(mYearValueMaxDate);
	mMaxDateLayout->addChild(mMonthLabelMaxDate);
	mMaxDateLayout->addChild(mMonthValueMaxDate);
	mMaxDateLayout->addChild(mDayLabelMaxDate);
	mMaxDateLayout->addChild(mDayValueMaxDate);
	mMaxDateLayout->addChild(mSetMaxDate);
	mMaxDateLayout->addChild(mGetMaxDate);
	mMaxDateLayout->addChild(mGetMaxDateValue);
}

void MainScreen::CreateMinDateLayout()
{
	mMinDateLayout = new VerticalLayout();

	mYearLabelMinDate = new Label();
	mYearLabelMinDate->setText("MinDate year:");
	mYearLabelMinDate->fillSpaceHorizontally();

	mMonthLabelMinDate = new Label();
	mMonthLabelMinDate->setText("MinDate Month:");
	mMonthLabelMinDate->fillSpaceHorizontally();

	mDayLabelMinDate = new Label();
	mDayLabelMinDate->setText("MinDate Day:");
	mDayLabelMinDate->fillSpaceHorizontally();

	mYearValueMinDate = new EditBox();
	mYearValueMinDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mYearValueMinDate->fillSpaceHorizontally();

	mMonthValueMinDate = new EditBox();
	mMonthValueMinDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mMonthValueMinDate->fillSpaceHorizontally();

	mDayValueMinDate = new EditBox();
	mDayValueMinDate->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mDayValueMinDate->fillSpaceHorizontally();

	mSetMinDate = new Button();
	mSetMinDate->setText("Set MinDate");

	mGetMinDate = new Button();
	mGetMinDate->setText("Get MinDate");

    mGetMinDateValue = new Label();
    mGetMinDateValue->setText("MinDate:");
    mGetMinDateValue->fillSpaceHorizontally();

	mMaxDateLayout->fillSpaceHorizontally();

	mMinDateLayout->addChild(mYearLabelMinDate);
	mMinDateLayout->addChild(mYearValueMinDate);
	mMinDateLayout->addChild(mMonthLabelMinDate);
	mMinDateLayout->addChild(mMonthValueMinDate);
	mMinDateLayout->addChild(mDayLabelMinDate);
	mMinDateLayout->addChild(mDayValueMinDate);
	mMinDateLayout->addChild(mSetMinDate);
	mMinDateLayout->addChild(mGetMinDate);
	mMinDateLayout->addChild(mGetMinDateValue);
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
		mDisplayedDate->setText(
			MAUtil::integerToString(selectedDate.day) + "-" +
			MAUtil::integerToString(selectedDate.month)+ "-" +
			MAUtil::integerToString(selectedDate.year));

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
		mDisplayedDate->setText(
			MAUtil::integerToString(date.day) + "-" +
			MAUtil::integerToString(date.month)+ "-" +
			MAUtil::integerToString(date.year));
    }
    else if (button == mSetMaxDate)
    {
		if( mYearValueMaxDate->getText().size() != 0 &&
			mMonthValueMaxDate->getText().size() != 0 &&
			mDayValueMaxDate->getText().size() != 0)
		{
			Date date;
			date.year = atoi(mYearValueMaxDate->getText().c_str());
			date.month = atoi(mMonthValueMaxDate->getText().c_str());
			date.day = atoi(mDayValueMaxDate->getText().c_str());

			mDatePicker->setMaxDate(date);
		}
		else
		{
			maAlert("Error", "Please fill all the required fields.", "OK", "", "");
		}
    }
	else if (button == mGetMaxDate)
    {
        struct Date theDate = mDatePicker->getMaxDate();

        mGetMaxDateValue->setText("MaxDate: " + MAUtil::integerToString(theDate.day)+ "-" +
		MAUtil::integerToString(theDate.month) + "-" + MAUtil::integerToString(theDate.year));
        printf("get max date : %d-%d-%d", theDate.day, theDate.month, theDate.year);
    }
    else if (button == mSetMinDate)
    {
		if( mYearValueMinDate->getText().size() != 0 &&
			mMonthValueMinDate->getText().size() != 0 &&
			mDayValueMinDate->getText().size() != 0)
		{
			Date date;
			date.year = atoi(mYearValueMinDate->getText().c_str());
			date.month = atoi(mMonthValueMinDate->getText().c_str());
			date.day = atoi(mDayValueMinDate->getText().c_str());

			mDatePicker->setMinDate(date);
		}
		else
		{
			maAlert("Error", "Please fill all the required fields.", "OK", "", "");
		}
    }
    else if (button == mGetMinDate)
    {
        struct Date theDate = mDatePicker->getMinDate();
        mGetMinDateValue->setText("MinDate: " + MAUtil::integerToString(theDate.day)+ "-" +
		MAUtil::integerToString(theDate.month) + "-" + MAUtil::integerToString(theDate.year));
        printf("get min date : %d-%d-%d", theDate.day, theDate.month, theDate.year);
    }
}
