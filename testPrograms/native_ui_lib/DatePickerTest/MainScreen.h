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
 * @file MainScreen.h
 * @author Bogdan Iusco, Cipri Filipas
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public DatePickerListener,
	private ButtonListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
     * This method is called the selected date was changed by the user.
     * @param datePicker The date picker object that generated the event.
     * @param selectedDate The new selected date.
     */
    virtual void datePickerValueChanged(
        DatePicker* datePicker,
        const struct Date& selectedDate);

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button) {};

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button) {};

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

private:
	/**
	 * Main layout.
	 */
    ListView* mList;

    VerticalLayout* mMaxDateLayout;
    VerticalLayout* mMinDateLayout;

	Button* mSetDate;
	Button* mGetDate;
	Button* mSetMaxDate;
	Button* mGetMaxDate;
	Button* mSetMinDate;
	Button* mGetMinDate;

	EditBox* mYearValueMaxDate;
	EditBox* mMonthValueMaxDate;
	EditBox* mDayValueMaxDate;
	EditBox* mYearValueMinDate;
	EditBox* mMonthValueMinDate;
	EditBox* mDayValueMinDate;

	Label* mYearLabelMaxDate;
	Label* mMonthLabelMaxDate;
	Label* mDayLabelMaxDate;
	Label* mYearLabelMinDate;
	Label* mMonthLabelMinDate;
	Label* mDayLabelMinDate;
	Label* mDisplayedDate;
	Label* mGetMaxDateValue;
	Label* mGetMinDateValue;

	DatePicker* mDatePicker;

	void CreateMaxDateLayout();
	void CreateMinDateLayout();
};


#endif /* MAINSCREEN_H_ */
