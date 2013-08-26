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
import android.widget.TimePicker;
import android.widget.TimePicker.OnTimeChangedListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.TimePickerWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates time picker and sets up the default
 * behavior for sending events when clicked.
 *
 * @author emma
 */
public class TimePickerFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, final int handle)
	{
		TimePicker timePicker = new TimePicker( activity);
		// For the moment allow only 24hour mode.
		timePicker.setIs24HourView(true);

		// Init values.
		Calendar today = Calendar.getInstance();
		timePicker.setCurrentHour( today.get(Calendar.HOUR_OF_DAY) );
		timePicker.setCurrentMinute( today.get(Calendar.MINUTE) );

		timePicker.setOnTimeChangedListener( new OnTimeChangedListener()
		{

			@Override
			public void onTimeChanged(TimePicker view, int hourOfDay, int minute) {
				EventQueue.getDefault( ).postTimePickerValueChangedEvent(handle, hourOfDay, minute);
			}
		});

		return new TimePickerWidget( handle, timePicker );
	}
}
