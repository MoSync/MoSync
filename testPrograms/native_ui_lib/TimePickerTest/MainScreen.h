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
 * @author Bogdan Iusco
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
	public TimePickerListener
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
     * This method is called when the selected time was changed by the user.
     * @param button The time picker object that generated the event.
     * @param hour The selected hour.
     * @param minute The selected minute
     */
    virtual void timePickerValueChanged(
        TimePicker* timePicker,
        const int hour,
        const int minute);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	TimePicker* mTimePicker;
	Label* mTimePickerValue;
};


#endif /* MAINSCREEN_H_ */
