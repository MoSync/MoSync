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
