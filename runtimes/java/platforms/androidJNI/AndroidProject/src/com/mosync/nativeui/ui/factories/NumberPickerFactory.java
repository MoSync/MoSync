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
