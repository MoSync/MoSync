/* Copyright (C) 2011 MoSync AB

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

package com.mosync.nativeui.ui.factories;

import java.util.Calendar;

import android.app.Activity;
import android.widget.DatePicker;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.DatePickerWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates date picker and sets up the default
 * behavior for sending events when the date is changed.
 *
 * @author emma
 */
public class DatePickerFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, final int handle)
	{
		DatePicker datePicker = new DatePicker( activity);

		// Listens for when the user is done setting the date.
		DatePicker.OnDateChangedListener mDateChangedListener = new DatePicker.OnDateChangedListener() {

			@Override
			public void onDateChanged(DatePicker view, int year, int monthOfYear,
					int dayOfMonth)
			{
				EventQueue.getDefault().postDatePickerValueChangedEvent(handle, dayOfMonth, monthOfYear+1, year);
			}
		};

		// Initialize the state with the current system date.
		Calendar today = Calendar.getInstance();
		datePicker.init(today.get(Calendar.YEAR), today.get(Calendar.MONTH), today.get(Calendar.DAY_OF_MONTH), mDateChangedListener);

		return new DatePickerWidget( handle, datePicker );
	}
}
