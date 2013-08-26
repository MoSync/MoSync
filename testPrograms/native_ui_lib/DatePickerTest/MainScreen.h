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
	public EditBoxListener,
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

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

private:
	/**
	 * Main layout.
	 */
    VerticalLayout* mList;

    VerticalLayout* mMaxDateLayout;
    VerticalLayout* mMinDateLayout;
    RelativeLayout* mPropertiesLayout;

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

	int mScreenY, mScreenX;

	void CreateMaxDateLayout();
	void CreateMinDateLayout();
};


#endif /* MAINSCREEN_H_ */
