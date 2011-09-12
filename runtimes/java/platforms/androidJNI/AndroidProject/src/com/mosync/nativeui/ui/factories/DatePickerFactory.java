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
