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
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.RadioButton;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.RadioButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a radio button widget, that sends events once it's
 * state changes.
 * Radio buttons are grouped inside RadioGroup element
 * so that no more than one can be selected at a time.
 *
 * @author emma
 */
public class RadioButtonFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		RadioButton radioButton = new RadioButton( activity );
		radioButton.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				EventQueue.getDefault( ).postRadioButtonStateChanged(handle, isChecked);
			}
		});

		return new RadioButtonWidget( handle, radioButton);
		}
}
