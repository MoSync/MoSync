/* Copyright (C) 2012 MoSync AB

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

import android.app.Activity;
import android.os.Build;
import android.widget.NumberPicker;
import android.widget.NumberPicker.OnValueChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.NumberPickerWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.properties.IntConverter;

/**
 * Creates a number picker widget, that sends events once
 * it's value changes.
 * The number picker is available only since Android level 11.
 *
 * @author emma tresanszki
 */
public class NumberPickerFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		int target = IntConverter.convert( Build.VERSION.SDK );
		if ( target >= Build.VERSION_CODES.HONEYCOMB )
		{
			NumberPicker numberPicker = new NumberPicker( activity );
			numberPicker.setOnValueChangedListener(new OnValueChangeListener() {

				@Override
				public void onValueChange(NumberPicker picker, int oldVal, int newVal) {
					EventQueue.getDefault().postNumberPickerValueChangedEvent(handle, newVal);
				}
			});

			return new NumberPickerWidget( handle, numberPicker);
		}
		else
		{
			return null;
		}
	}
}
