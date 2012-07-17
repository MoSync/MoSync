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
